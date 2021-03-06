#include "LevelObject.h"

#include "CatalogObject.h"
#include "Starter.h"

using namespace std;
using namespace tinyxml2;

LevelObject::LevelObject()
{
	m_sprite = NULL;
	m_position = sf::Vector2f();
	m_scale = 1.0f;
	m_opacity = 1.0f;
	m_angle = 0.0f;
	m_layer = 0;
	m_objectID = 0;
	m_objectType = "";
	m_category = "";
	m_renderWindow = NULL;
	calculateScrollFactor();
}

LevelObject::LevelObject(XMLElement* element)
{
	m_layer = atoi(element->Attribute("layer"));

	m_objectID = 0;
	m_cameraPos = sf::Vector2f();
	m_objectType = "";
	m_category = "";
	m_sprite = NULL;
	m_renderWindow = NULL;
	calculateScrollFactor();

	m_position = sf::Vector2f((float)atof(element->Attribute("x")), (float)atof(element->Attribute("y")));

	m_angle = (float)atof(element->Attribute("angle"));
	if(element->Attribute("scale") != NULL)
		m_scale = (float)atof(element->Attribute("scale"));
	m_opacity = (float)atof(element->Attribute("opacity"));

	CatalogObject catObj;
	if(catObj.load(element->Attribute("id")))
		getDataFromCatalogObject(catObj);
}

LevelObject::LevelObject(CatalogObject gameObject)
{
	m_position = sf::Vector2f();
	m_cameraPos = sf::Vector2f();
	m_scale = 1.0f;
	m_opacity = 1.0f;
	m_angle = 0.0f;
	m_layer = 0;
	m_objectID = 0;
	m_objectType = "";
	m_category = "";
	m_sprite = NULL;
	m_renderWindow = NULL;
	calculateScrollFactor();

	getDataFromCatalogObject(gameObject);
}

void LevelObject::getDataFromCatalogObject(CatalogObject catObj)
{
	m_sprite = NULL;
	m_name = catObj.getName();
	m_category = catObj.getCategory();
	m_objectType = catObj.getObjectID();
	m_imageFile = catObj.getImageFile();

	if(!m_imageFile.isEmpty())
	{
		sf::Texture* texture = new sf::Texture();
		texture->loadFromFile(string("objects/images/") + m_imageFile);

		m_sprite = new sf::Sprite();
		m_sprite->setTexture(*texture);
		m_sprite->setPosition(m_position);
		m_sprite->setOrigin(texture->getSize().x / 2.0f, texture->getSize().y / 2.0f);
	}

	b2World* world = new b2World(b2Vec2(0, 9.81f));
	b2Body* body = world->CreateBody(&m_bodyDef);

	b2FixtureDef fixDef = b2FixtureDef();
	fixDef.friction = 1;
	fixDef.density = 1;
	
	b2ChainShape* shape = new b2ChainShape();

	b2Vec2 vertices[7];
	vertices[0] = b2Vec2(0, 0);
	vertices[1] = b2Vec2(0, 0);
	vertices[2] = b2Vec2(0, 0);
	vertices[3] = b2Vec2(0, 0);
	vertices[4] = b2Vec2(0, 0);
	vertices[5] = b2Vec2(0, 0);
	vertices[6] = b2Vec2(0, 0);
	shape->CreateChain(vertices, 7);

	

	fixDef.shape = shape;

	b2Fixture* fixture = body->CreateFixture(&fixDef);
}

LevelObject::~LevelObject()
{
	if(m_sprite != NULL) delete m_sprite;
}

void LevelObject::tick(sf::Time elapsedTime)
{
	if(m_renderWindow && m_sprite)
	{
		m_sprite->setPosition((m_position.x - m_cameraPos.x * m_scrollFactor) * Starter::getScreenFactor(), (m_position.y + m_cameraPos.y * m_scrollFactor) * Starter::getScreenFactor());
		m_sprite->setScale(Starter::getScreenFactor() * m_scale, Starter::getScreenFactor() * m_scale);
		m_sprite->setRotation(Starter::RAD_TO_DEG *  m_angle);
		m_sprite->setColor(sf::Color(0xff, 0xff, 0xff, sf::Uint8(0xff * m_opacity)));
		m_renderWindow->draw(*m_sprite);
	}
}

void LevelObject::setLayer(int layer)
{
	m_layer = layer;
	calculateScrollFactor();
}

int LevelObject::getLayer()
{
	return m_layer;
}

void LevelObject::calculateScrollFactor()
{
	switch(m_layer)
	{
	case 0: m_scrollFactor = 0; break;
	case 1: m_scrollFactor = 0.25f; break;
	case 2: m_scrollFactor = 0.5f; break;
	case 3: m_scrollFactor = 0.75; break;
	case 4: m_scrollFactor = 1; break;
	case 5: m_scrollFactor = 1; break;
	case 6: m_scrollFactor = 1; break;
	case 7: m_scrollFactor = 1.25; break;
	case 8: m_scrollFactor = 1.5; break;
	case 9: m_scrollFactor = 2; break;
	case 10: m_scrollFactor = 0; break;
	}
}

void LevelObject::setPosition(sf::Vector2f position)
{
	m_position = position;
}

sf::Vector2f LevelObject::getPosition()
{
	return m_position;
}

void LevelObject::setScale(float scale)
{
	m_scale = scale;
}

float LevelObject::getScale()
{
	return m_scale;
}

void LevelObject::setOpacity(float opacity)
{
	m_opacity = opacity;
}

float LevelObject::getOpacity()
{
	return m_opacity;
}

void LevelObject::setAngle(float angle)
{
	m_angle = angle;
}

float LevelObject::getAngle()
{
	return m_angle;
}

void LevelObject::setName(std::string name)
{
	m_name = name;
}

std::string LevelObject::getName()
{
	return m_name;
}

void LevelObject::setObjectType(std::string id)
{
	m_objectType = id;
}

std::string LevelObject::getObjectType()
{
	return m_objectType;
}

void LevelObject::setObjectID(int id)
{
	m_objectID = id;
}

int LevelObject::getObjectID()
{
	return m_objectID;
}

void LevelObject::setObjectCategory(std::string cat)
{
	m_category = cat;
}

std::string LevelObject::getObjectCategory()
{
	return m_category;
}

void LevelObject::setRenderWindow(sf::RenderWindow* window)
{
	m_renderWindow = window;
}

sf::RenderWindow* LevelObject::getRenderWindow()
{
	return m_renderWindow;
}

void LevelObject::setCameraPos(sf::Vector2f pos)
{
	m_cameraPos = pos;
}

sf::Vector2f LevelObject::getCameraPos() 
{
	return m_cameraPos;
}
