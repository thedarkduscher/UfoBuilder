#pragma once

#include <string>
#include <sstream>

class StringHelper
{
public:
	static std::string toString(double value);
	static std::string toString(float value);
	static std::string toString(int value);
};
