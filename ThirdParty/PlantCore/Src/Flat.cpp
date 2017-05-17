#include "Flat.h"
#include <string>


Flat::Flat()
{
	Machine wheel("wheel");

	for (int i = 2; i < 36; i++)
	{
		if (i < 10)
		{
			MachinePart p("00" + std::to_string(i));
			wheel.machineParts.push_back(p);
		}
		else {
			MachinePart p("0" + std::to_string(i));
			wheel.machineParts.push_back(p);
		}	
	}

	machines.push_back(wheel);
}


Flat::~Flat()
{
}
