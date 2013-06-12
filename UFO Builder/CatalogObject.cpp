#include "CatalogObject.h"

using namespace tinyxml2;

CatalogObject::CatalogObject()
{
	m_imageFile = "";
	m_name = "";
	m_objectID = "";
}

CatalogObject::~CatalogObject()
{

}

bool CatalogObject::load(std::string objectID)
{
	XMLDocument doc;

	// load XML file
	if(doc.LoadFile(("objects\\" + objectID + ".xml").c_str()) != XML_SUCCESS)
		return false;

	// get object node
	
	XMLElement* objectNode = doc.RootElement();
	if(objectNode == NULL)
		return false;

	// parse attributes
	if(objectNode->Attribute("id") == NULL)
		return false;
	m_objectID = objectNode->Attribute("id");

	if(objectNode->Attribute("name") != NULL)
		m_name = objectNode->Attribute("name");

	if(objectNode->Attribute("image") != NULL)
		m_imageFile = objectNode->Attribute("image");

	// get physics node
	XMLElement* physicsNode = objectNode->FirstChildElement("physics");

	// parse body type
	if(physicsNode->Attribute("type") == "dynamic")
		m_physProps.setBodyType(StorablePhysProps::Dynamic);
	else
		m_physProps.setBodyType(StorablePhysProps::Static);

	// parse fixtures
	m_physProps.getFixtures()->clear();

	XMLElement* fixtureNode = physicsNode->FirstChildElement("fixture");
	while(fixtureNode != NULL)
	{
		StorableFixture fixture;

		// parse fixture type
		std::string type = fixtureNode->Attribute("type");
		if(type == "line")
			fixture.setType(StorableFixture::Line);
		else if(type == "polygon")
			fixture.setType(StorableFixture::Polygon);
		else if(type == "circle")
			fixture.setType(StorableFixture::Circle);
		else
			fixture.setType(StorableFixture::Chain);

		// parse fixture position
		fixture.setPosition(b2Vec2(std::stof(fixtureNode->Attribute("x")), std::stof(fixtureNode->Attribute("y"))));

		// parse fixture friction
		fixture.setFriction(std::stof(fixtureNode->Attribute("friction")));

		// parse fixture density
		fixture.setDensity(std::stof(fixtureNode->Attribute("density")));

		// parse fixture vertices
		XMLElement* pointNode = fixtureNode->FirstChildElement("point");
		while(pointNode != NULL)
		{			
			fixture.getVertices()->push_back(b2Vec2(std::stof(pointNode->Attribute("x")), std::stof(pointNode->Attribute("y"))));
			pointNode = pointNode->NextSiblingElement("point");
		}

		m_physProps.getFixtures()->push_back(fixture);

		// get next node
		fixtureNode = fixtureNode->NextSiblingElement("fixture");
	}
	return true;
}

bool CatalogObject::save()
{	
	XMLDocument doc;

	// create object node
	XMLElement* objectNode = doc.NewElement("object");
	if(objectNode == NULL)
		return false;

	// set attributes
	objectNode->SetAttribute("id", m_objectID.c_str());
	objectNode->SetAttribute("name", m_name.c_str());
	objectNode->SetAttribute("image", m_imageFile.c_str());

	// create physics node
	XMLElement* physicsNode = doc.NewElement("physics");
	objectNode->InsertEndChild(physicsNode);

	// set body type
	if(m_physProps.getBodyType() == StorablePhysProps::Dynamic)
		physicsNode->SetAttribute("type", "dynamic");
	else
		physicsNode->SetAttribute("type", "static");



	std::list<StorableFixture>::iterator pos = m_physProps.getFixtures()->begin();
	while(pos != m_physProps.getFixtures()->end())
	{
		
		// create fixture node
		XMLElement* fixtureNode = doc.NewElement("fixture");
		physicsNode->InsertEndChild(fixtureNode);

		std::string type;
		switch(pos->getType())
		{
		case StorableFixture::Line:
			type = "line";
			break;
		case StorableFixture::Polygon:
			type = "polygon";
			break;
		case StorableFixture::Circle:
			type = "circle";
			break;
		default: 
			type = "chain";
		}

		fixtureNode->SetAttribute("type", type.c_str());

		fixtureNode->SetAttribute("x", pos->getPosition().x);
		fixtureNode->SetAttribute("y", pos->getPosition().y);

		fixtureNode->SetAttribute("friction", pos->getFriction());
		fixtureNode->SetAttribute("density", pos->getDensity());

		// save point

		std::list<b2Vec2>::iterator pointPos = pos->getVertices()->begin();

		while(pointPos != pos->getVertices()->end())
		{
			XMLElement* pointNode = doc.NewElement("point");

			pointNode->SetAttribute("x", pointPos->x);
			pointNode->SetAttribute("y", pointPos->y);

			fixtureNode->InsertEndChild(pointNode);

			pointPos++;
		}

		pos++;
	}

	doc.InsertEndChild(objectNode);


	if(doc.SaveFile(("objects\\" + m_objectID + ".xml").c_str()) != XML_SUCCESS)
		return false;

	return true;
}