#pragma once

#include "MachinePart.h"
#include <vector>
#include <string>

class Machine
{
public:
	Machine(std::string name);
	~Machine();

	std::vector<MachinePart> machineParts;

	std::string name;
};

