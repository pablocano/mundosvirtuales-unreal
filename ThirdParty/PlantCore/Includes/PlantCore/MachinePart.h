#pragma once
#include <string>

class MachinePart
{
public:
	MachinePart(std::string name, std::string material);
	~MachinePart();

	std::string name;

	std::string material;

	bool animated;
};

