#include "MachinePart.h"



MachinePart::MachinePart(std::string name, std::string material, std::string info, std::string shortInfo, std::string pn) :
	name(name), material(material), info(info), pn(pn), shortInfo(shortInfo), animated(false)
{
}


MachinePart::~MachinePart()
{
}
