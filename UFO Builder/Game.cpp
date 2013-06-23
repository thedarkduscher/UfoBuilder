#include "Game.h"

Game::Game(Starter* starter, sf::RenderWindow* window)
{
	m_starter = starter;
	m_level = new Level();
	m_renderWindow = window;

	m_mouseIsPressed = false;
	m_keyUp = false;
	m_keyDown = false;
	m_keyLeft = false;
	m_keyRight = false;

	m_sensitivity = 5.0f;

	m_mousePosition = sf::Vector2i(0, 0);
	m_lastClick = sf::Vector2i(0, 0);

	m_acceleration = 0;
	m_direcction = 0;

	// create physical world
	m_physWorld = new b2World(b2Vec2(0.0f, 9.81f));

	// load textures
	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile("images/bg1.png");
	m_spriteTest.setTexture(*texture);

	texture = new sf::Texture();
	texture->loadFromFile("images/body1.png");
	texture->setSmooth(true);
	m_spriteBody.setTexture(*texture);
	m_spriteBody.setOrigin(m_spriteBody.getTexture()->getSize().x / 2.0f, m_spriteBody.getTexture()->getSize().y / 2.0f);

	// load level
	/*
	if(m_level->load("bagdadbahn")) {
	} else {
	}
	*/

	// calculate sprite scales
	resize();

	// create physical bodies
	b2BodyDef* def = new b2BodyDef();
	def->position = b2Vec2(1.0f, 1.0f);
	def->type = b2_dynamicBody;
	m_bodyTest = m_physWorld->CreateBody(def);
	m_bodyTest->SetLinearDamping(0.1f);

	b2CircleShape* shape = new b2CircleShape();
	shape->m_radius = 1.1f;
	m_bodyTest->CreateFixture(shape, 2.0f);	
	m_bodyTest->ApplyTorque(60.0f);	

	def = new b2BodyDef();
	def->position = b2Vec2(0.0f, 5.0f);
	m_bodyGround = m_physWorld->CreateBody(def);

	b2PolygonShape* shape2 = new b2PolygonShape();
	shape2->SetAsBox(10.0f, 0.1f, b2Vec2(0.0f, 0.0f), 0.0f);

	m_bodyGround->CreateFixture(shape2, 2.0f);
}

Game::~Game()
{
	if(m_physWorld) delete m_physWorld;
	if(m_level) delete m_level;
}

void Game::tick(sf::Time elapsedTime)
{
	// update physics
	m_physWorld->Step(elapsedTime.asSeconds(), 6, 2);

	// user event handling
	#pragma region event handling
	sf::Event event;
	while(m_renderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			// the close button was clicked
			m_renderWindow->close();
		}
		else if(event.type == sf::Event::Resized)
		{
			// the windows has been resized
			m_starter->resize(event.size.width, event.size.height);
			resize();
		}
		else if(event.type == sf::Event::KeyPressed)
		{
			// key has been pressed
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				// escape has been pressed -> call Menu
				m_starter->setGamestate(m_starter->Menu);
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				m_keyUp = true;
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				m_keyDown = true;
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				m_keyRight = true;
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				m_keyLeft = true;
			}
		}
		else if(event.type == sf::Event::KeyReleased)
		{
			// key has been released
			if(event.key.code == sf::Keyboard::Up)
			{
				m_keyUp = false;
			}
			else if(event.key.code == sf::Keyboard::Down)
			{
				m_keyDown = false;
			}
			else if(event.key.code == sf::Keyboard::Left)
			{
				m_keyLeft = false;
			}
			else if(event.key.code == sf::Keyboard::Right)
			{
				m_keyRight = false;
			}
		}
		else if(event.type == sf::Event::MouseMoved)
		{
			// mouse has been moved
			m_mousePosition = sf::Mouse::getPosition();
		}
		else if(event.type == sf::Event::MouseButtonPressed)
		{
			// mouse down
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				// left mouse button has been clicked
				m_lastClick = sf::Mouse::getPosition();

				if(isSelected(m_spriteBody))
					m_mouseIsPressed = true;
			}
		}
		else if(event.type == sf::Event::MouseButtonReleased)
		{
			// mouse up
			//if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
				//MessageBox(NULL, "yep", "", 0);
			m_mouseIsPressed = false;
		}
	}
	#pragma endregion

	m_renderWindow->draw(m_spriteTest);

	float scale = m_starter->getScreenFactor();

	// Mouse control

	if(m_mouseIsPressed)
	{
		b2Vec2 pos;
		float x = m_mousePosition.x - m_spriteBody.getTexture()->getSize().x / 2;
		float y = m_mousePosition.y - m_spriteBody.getTexture()->getSize().y / 2;

		m_spriteBody.setPosition(x, y);
		
		m_bodyTest->SetTransform(b2Vec2(x, y), m_bodyTest->GetAngle());
	}
	else
	{
		// draw testobject
		//m_spriteBody.setRotation(m_bodyTest->GetAngle() * Starter::RAD_TO_DEG);	
		//m_spriteBody.setPosition(m_bodyTest->GetPosition().x * 64.0f * scale, m_bodyTest->GetPosition().y * 64.0f * scale);
		m_spriteBody.setPosition(m_bodyTest->GetPosition().x, m_bodyTest->GetPosition().y);
	}

	// Key control

	if(m_keyUp && !m_keyDown)
	{
		m_acceleration += 2;
	}
	else if(m_keyDown && !m_keyUp)
	{
		m_acceleration -= 2;
	}
	else
	{
		if(m_acceleration > 0)
		{
			m_acceleration--;
		}
		else if(m_acceleration < 0)
		{
			m_acceleration++;
		}
	}

	if(m_keyLeft && !m_keyRight)
	{
		m_direcction += 2;
	}
	else if(m_keyRight && !m_keyLeft)
	{
		m_direcction -= 2;
	}
	else
	{
		if(m_direcction > 0)
		{
			m_direcction--;
		}
		else if(m_direcction < 0)
		{
			m_direcction++;
		}
	}

	m_bodyTest->SetTransform(b2Vec2(m_bodyTest->GetPosition().x - (float)m_direcction / m_sensitivity, m_bodyTest->GetPosition().y - (float)m_acceleration / m_sensitivity), m_bodyTest->GetAngle());

	m_renderWindow->draw(m_spriteBody);
}


void Game::resize()
{
	float scale = m_starter->getScreenFactor();

	m_spriteBody.setScale(scale, scale);
	m_spriteTest.setScale(scale * 4.0f, scale * 2.2f);
}

sf::Vector2i Game::getMousePosition()
{
	return m_mousePosition;
}

sf::Vector2i Game::getLastClick()
{
	return m_lastClick;
}

bool Game::isSelected(const sf::Sprite object)
{
	if(m_lastClick.x >= object.getPosition().x)
		if(m_lastClick.x <= object.getPosition().x + object.getTexture()->getSize().x)
			if(m_lastClick.y >= object.getPosition().y)
				if(m_lastClick.y <=  object.getPosition().y + object.getTexture()->getSize().y)
					return true;
	return false;
}
