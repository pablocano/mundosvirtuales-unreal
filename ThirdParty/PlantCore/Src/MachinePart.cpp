#include "MachinePart.h"



MachinePart::MachinePart()
{
}

MachinePart::MachinePart(int id, std::string name, std::string material, std::string info, std::string shortInfo, std::string pn) :
	machine_id(id), name(name), material(material), info(info), pn(pn), shortInfo(shortInfo), animated(false)
{
}


MachinePart::~MachinePart()
{
}

void to_json(json & j, const MachinePart & m)
{
	j = json{ {"id", m.machine_id},{"name", m.name},{"material",m.material},{"info",m.info},{"pn",m.pn},{"shortInfo",m.shortInfo},{"animated",m.animated} };
}

void from_json(const json & j, MachinePart & m)
{
	m.machine_id = j.at("id").get<int>();
	m.name = j.at("name").get<std::string>();
	m.material = j.at("material").get<std::string>();
	m.info = j.at("info").get<std::string>();
	m.pn = j.at("pn").get<std::string>();
	m.shortInfo = j.at("shortInfo").get<std::string>();
	m.animated = j.at("animated").get<bool>();
}
