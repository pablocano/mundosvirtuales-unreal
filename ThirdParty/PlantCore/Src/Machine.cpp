#include "Machine.h"



Machine::Machine(std::string name, std::string info, std::string shortInfo, std::string pn, bool canShowInfo) :
	name(name), info(info), canShowInfo(canShowInfo), shortInfo(shortInfo), pn(pn)
{
}

Machine::~Machine()
{
}



