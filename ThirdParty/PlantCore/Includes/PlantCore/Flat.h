#pragma once

#include <vector>
#include <string>

#include "Machine.h"
#include "Loader.h"

class Flat
{
public:
	Flat();
	~Flat();

	Loader loader;
	Machines machines;

};

