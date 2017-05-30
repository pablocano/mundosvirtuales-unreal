#include "../Utils/network/TcpComm.h"
#include "../Utils/network/UdpComm.h"

#include <iostream>
#include <string.h>

#define MAX_LENGTH_BUFFER 256

int main()
{
	TcpComm tcpComm("example.com", 80);
	
	const char *strConn = "GET / HTTP/1.0\r\nHost: example.com/\r\nConnection: close\r\n\r\n";
	unsigned char buffer[MAX_LENGTH_BUFFER]; 
	
	if (tcpComm.connected())
	{
		tcpComm.send(strConn, (int) strlen(strConn));
		
		std::cout << "Data:" << std::endl;
		while (tcpComm.receive(buffer, MAX_LENGTH_BUFFER, true))
			std::cout << (char *) buffer;
	}
	
	std::cout << "\n\n ---end---" << std::endl;
	std::getchar();

	return 0;
}