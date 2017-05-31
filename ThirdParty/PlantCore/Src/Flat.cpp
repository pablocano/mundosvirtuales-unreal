#include "Utils/Loader.h"
#include "Flat.h"

#include <iostream>
#include <string>

Flat::Flat()
{
	Loader loader("proter", "postgres", "10.0.42.8", 5432, "test123");
	machines = loader.load_machines();
	std::cout << loader.get_str_connection();
}

Flat::~Flat()
{
}
