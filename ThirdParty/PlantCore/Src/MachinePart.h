#pragma once

#include <vector>
#include <string>

class MachinePart
{
public:
	MachinePart(int id, std::string name, std::string material, std::string info, std::string shortInfo, std::string pn);
	
	~MachinePart();

	std::string name;

	std::string material;

	std::string info;

	std::string shortInfo;

	std::string pn;

	bool animated;

	int machine_id;
};

typedef std::vector<MachinePart> MachineParts;