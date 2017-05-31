#pragma once

#include <vector>
#include <string>

#include "Machine.h"
#include "Utils/Loader.h"

class Flat
{
public:
	Flat();
	~Flat();

	Loader loader;
	Machines machines;

};

