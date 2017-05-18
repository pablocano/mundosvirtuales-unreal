#pragma once

#include "MachinePart.h"
#include <vector>
#include <string>

class Machine
{
public:
	Machine(std::string name, std::string info, std::string shortInfo, std::string pn, bool canBeSelected = false, bool canShowInfo = false);
	~Machine();

	std::vector<MachinePart> machineParts;

	std::string name;

	bool canShowInfo;

	std::string info;

	std::string shortInfo;

	std::string pn;

	bool canBeSelected;

};

