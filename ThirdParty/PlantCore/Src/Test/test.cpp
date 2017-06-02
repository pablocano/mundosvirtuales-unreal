#include "../Utils/Serialization/json.hpp"

#include "../Utils/network/TcpComm.h"
#include "../Utils/network/UdpComm.h"
#include "../Utils/Security/Cryptography.h"

#include "../Machine.h"

#include <iostream>
#include <string.h>
#include <vector>

#define MAX_LENGTH_BUFFER 10

using json = nlohmann::json;

int main()
{
	
	std::cout << "Test 1" << std::endl;
	TcpComm tcpComm("www.transantiago.cl", 80);
	
	const char *strConn = "GET /predictor/prediccion?codsimt=pd553 HTTP/1.0\r\nHost: www.transantiago.cl\r\nConnection: close\r\n\r\n";
	unsigned char buffer[MAX_LENGTH_BUFFER]; 
	
	if (tcpComm.connected())
	{
		tcpComm.send((const unsigned char*)strConn, (int) strlen(strConn));
		
		std::cout << "Data:" << std::endl;
		buffer[MAX_LENGTH_BUFFER - 1] = 0;
		while (tcpComm.receive(buffer, MAX_LENGTH_BUFFER-1))
			std::cout << (char *) buffer;
		std::cout << (char *)buffer;
	}

	std::cout << "\n\nEnd test 1" << std::endl;
	std::cout << "\nsiguiente test presione enter" << std::endl;
	std::getchar();
	std::cout << "\n\nTest 2" << std::endl;


	uint32_t key[4] = {56324394,73576,12030122,56};

	char strConn2[500] = "GET /predictor/prediccion?codsimt=pd553 HTTP/1.0\r\nHost: www.transantiago.cl\r\nConnection: close\r\n\r\n";

	std::cout << "Original text:" << std::endl;
	std::cout << strConn2 << std::endl;

	Cryptography::encrypt((uint32_t*)strConn2, strlen(strConn2) / 4, key);

	std::cout << "Encrypted" << std::endl;
	std::cout << strConn2 << std::endl;

	Cryptography::decrypt((uint32_t*)strConn2, strlen(strConn2) / 4, key);

	std::cout << "Decrypted" << std::endl;
	std::cout << strConn2 << std::endl;

	std::cout << "\n\nEnd test 2" << std::endl;
	std::cout << "\nsiguiente test presione enter" << std::endl;
	std::getchar();
	std::cout << "\n\nTest 3" << std::endl;

	char test3[5000] = "{\"fechaprediccion\":\"2017-06-02\",\"horaprediccion\" : \"11:40\",\"nomett\" : \"PARADA 4 / (M) GRECIA\",\"paradero\" : \"PD553\",\"respuestaParadero\" : \"\",\"servicios\" : {\"item\":[{\"codigorespuesta\":\"00\",\"distanciabus1\" : \"1156\",\"distanciabus2\" : \"4416\",\"horaprediccionbus1\" : \"Menos de 5 min.\",\"horaprediccionbus2\" : \"Entre 11 Y 17 min. \",\"ppubus1\" : \"BJFJ-26\",\"ppubus2\" : \"BJFH-22\",\"respuestaServicio\" : \"\",\"servicio\" : \"506\"},{ \"codigorespuesta\":\"11\",\"respuestaServicio\" : \"Fuera de horario de operacion para este paradero\",\"servicio\" : \"506E\" },{ \"codigorespuesta\":\"01\",\"distanciabus1\" : \"4345\",\"horaprediccionbus1\" : \"Entre 11 Y 17 min. \",\"ppubus1\" : \"FDJX-76\",\"respuestaServicio\" : \"\",\"servicio\" : \"506V\" },{ \"codigorespuesta\":\"01\",\"distanciabus1\" : \"879\",\"horaprediccionbus1\" : \"Menos de 5 min.\",\"ppubus1\" : \"BJFF-53\",\"respuestaServicio\" : \"\",\"servicio\" : \"507\" },{ \"codigorespuesta\":\"11\",\"respuestaServicio\" : \"Fuera de horario de operacion para este paradero\",\"servicio\" : \"507C\" },{ \"codigorespuesta\":\"00\",\"distanciabus1\" : \"53\",\"distanciabus2\" : \"3653\",\"horaprediccionbus1\" : \"Menos de 5 min.\",\"horaprediccionbus2\" : \"Entre 07 Y 13 min. \",\"ppubus1\" : \"FLXK-44\",\"ppubus2\" : \"BFKC-26\",\"respuestaServicio\" : \"\",\"servicio\" : \"510\" },{ \"codigorespuesta\":\"01\",\"distanciabus1\" : \"3301\",\"horaprediccionbus1\" : \"Entre 07 Y 11 min. \",\"ppubus1\" : \"BFKB-92\",\"respuestaServicio\" : \"\",\"servicio\" : \"511\" },{ \"codigorespuesta\":\"00\",\"distanciabus1\" : \"1779\",\"distanciabus2\" : \"3339\",\"horaprediccionbus1\" : \"Entre 05 Y 09 min. \",\"horaprediccionbus2\" : \"Entre 09 Y 15 min. \",\"ppubus1\" : \"FLXK-20\",\"ppubus2\" : \"BDXR-17\",\"respuestaServicio\" : \"\",\"servicio\" : \"516\" },{ \"codigorespuesta\":\"11\",\"respuestaServicio\" : \"Fuera de horario de operacion para este paradero\",\"servicio\" : \"519E\" }]},\"urlPublicidad\" : \"http://mkt.smsbus.cl/img/Cat11.jpg\" }";

	auto j = json::parse(test3);

	auto j2 = j["servicios"]["item"][3];

	if (!j2.empty())
	{
		std::cout << j2 << std::endl;
	}
	
	std::cout << "\n\nEnd test 3" << std::endl;
	std::cout << "\nsiguiente test presione enter" << std::endl;
	std::getchar();
	std::cout << "\n\nTest 4" << std::endl;

	Machine m(23, "hola", "asdfa", "asdgfg", "adasd");
	Machine m2(42, "chao", "asdfa", "asdgfg", "adasd");


	MachinePart mp1(324532, "part1", "wood", "info1", "asdf1", "pn1");
	MachinePart mp2(21434, "part2", "metal", "info2", "asdf2", "pn2");

	m.machineParts.push_back(mp1);
	m.machineParts.push_back(mp2);

	std::vector<Machine> machines;

	machines.push_back(m);
	machines.push_back(m2);


	json j3 = json{ {"asdf", machines } };

	std::cout << j3.dump(2) << std::endl;

	std::vector<Machine> asdf;

	asdf = j3.at("asdf");

	std::getchar();

}