#include "Utils/Loader.h"
#include "Flat.h"

#include <string>

Flat::Flat()
{
	Loader loader("proter", "postgres");
	machines = loader.load_machines();
}

Flat::~Flat()
{
}
