#include "Machine.h"


Machine::Machine()
{

}

Machine::Machine(int id, std::string name, std::string info, std::string shortInfo, std::string pn, bool canBeSelected, bool canShowInfo) :
	machine_id(id), name(name), info(info), canShowInfo(canShowInfo), shortInfo(shortInfo), pn(pn), canBeSelected(canBeSelected)
{
}

Machine::~Machine()
{
}



