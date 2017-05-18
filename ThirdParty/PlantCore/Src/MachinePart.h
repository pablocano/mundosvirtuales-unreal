#pragma once
#include <string>

class MachinePart
{
public:
	MachinePart(std::string name, std::string material, std::string info, std::string shortInfo, std::string pn);
	~MachinePart();

	std::string name;

	std::string material;

	std::string info;

	std::string shortInfo;

	std::string pn;

	bool animated;
};

