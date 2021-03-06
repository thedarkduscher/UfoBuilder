#pragma once

#include <SFML\Graphics.hpp>
#include <string>

#include "StorablePhysProps.h"
#include "StorableFixture.h"
#include "tinyxml2.h"

class CatalogObject
{
protected:
	std::string m_objectID;
	std::string m_name;
	std::string m_imageFile;
	std::string m_category;

	StorablePhysProps m_physProps;

public:
	CatalogObject();
	~CatalogObject();

	bool save();
	bool load(std::string objectID);


	void setObjectID(std::string objectID);
	std::string getObjectID();

	void setName(std::string name);
	std::string getName();

	void setImageFile(std::string imageFile);
	std::string getImageFile();

	void setPhysProps(StorablePhysProps physProps);
	StorablePhysProps getPhysProps();

	void setCategory(std::string cat);
	std::string getCategory();
};

