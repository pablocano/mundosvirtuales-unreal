#pragma once

#include <vector>
#include <string>
#include "Machine.h"

class Flat
{
public:
	Flat();
	~Flat();

	static std::string machineList[82];

	static std::string materialList[82];

	static std::string machineInfo[82];

	std::vector<Machine> machines;
};

