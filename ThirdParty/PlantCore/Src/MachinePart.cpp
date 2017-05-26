#include "MachinePart.h"



MachinePart::MachinePart(int id, std::string name, std::string material, std::string info, std::string shortInfo, std::string pn) :
	machine_id(id), name(name), material(material), info(info), pn(pn), shortInfo(shortInfo), animated(false)
{
}


MachinePart::~MachinePart()
{
}
