#pragma once

#include "MachinePart.h"
#include <vector>
#include <string>

class Machine
{
public:
	Machine(std::string name, std::string info, bool canShowInfo = false);
	~Machine();

	std::vector<MachinePart> machineParts;

	std::string name;

	bool canShowInfo;

	std::string info;

};

