#include "MachinePart.h"



MachinePart::MachinePart(std::string name, std::string material, std::string info) :
	name(name), material(material), info(info),
	animated(false)
{
}


MachinePart::~MachinePart()
{
}
