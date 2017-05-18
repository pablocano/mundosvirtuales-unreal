#pragma once
#include <string>

class MachinePart
{
public:
	MachinePart(std::string name, std::string material, std::string info);
	~MachinePart();

	std::string name;

	std::string material;

	std::string info;

	bool animated;
};

