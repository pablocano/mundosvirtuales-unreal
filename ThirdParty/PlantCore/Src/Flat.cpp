#include "Flat.h"
#include <string>

Flat::Flat() : loader("proter", "postgres")
{
	machines = loader.load_machines();
}


Flat::~Flat()
{
}
