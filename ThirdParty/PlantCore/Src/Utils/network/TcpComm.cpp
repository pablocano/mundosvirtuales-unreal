#include "TcpComm.h"

#include <iostream>
#ifndef WINCE
#include <cerrno>
#include <fcntl.h>
#endif

#ifdef WIN32

#ifndef WINCE
#include <sys/types.h>
#endif

#define ERRNO WSAGetLastError()
#define RESET_ERRNO WSASetLastError(0)
#undef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#undef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#define NON_BLOCK(socket) ioctlsocket(socket, FIONBIO, (u_long*) "NONE")
#define CLOSE(socket) closesocket(socket)

class _WSAFramework
{
public:
  _WSAFramework()
  {
    WORD wVersionRequested = MAKEWORD(1, 0);
    WSADATA wsaData;
    WSAStartup(wVersionRequested, &wsaData);
  }
  ~_WSAFramework() {WSACleanup();}
} _wsaFramework;

#else

#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define ERRNO errno
#define RESET_ERRNO errno = 0
#define NON_BLOCK(socket) fcntl(socket,F_SETFL,O_NONBLOCK)
#define CLOSE(socket) close(socket)

#endif

TcpComm::TcpComm(const char* ip, int port, int maxPackageSendSize, int maxPackageReceiveSize) :
  createSocket(0),
  transferSocket(0),
  overallBytesSent(0),
  overallBytesReceived(0),
  maxPackageSendSize(maxPackageSendSize),
  maxPackageReceiveSize(maxPackageReceiveSize),
  wasConnected(false)
{
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  if (ip) // connect as client?
  {
	  struct hostent* he;
	  if( ( he = gethostbyname(ip) ) == NULL)
		  address.sin_addr.s_addr = inet_addr(ip);
	  else
		  memcpy((void *)&address.sin_addr, (const void *)he->h_addr_list[0], he->h_length);
  }
  else
  {
    createSocket = (int)socket(AF_INET, SOCK_STREAM, 0);
    if (createSocket > 0)
	{
		int val = 1;
		setsockopt(createSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &val, sizeof(val));
		address.sin_addr.s_addr = INADDR_ANY;
		if(bindSocket())
			std::cerr << "Problem Binding" << std::endl;
		if(listenToClient())
			std::cerr << "Problem Listening" << std::endl;
		NON_BLOCK(createSocket);
	}
  }
  checkConnection();
}

TcpComm::~TcpComm()
{
  if(connected())
    closeTransferSocket();
  if(createSocket > 0)
    CLOSE(createSocket);
}

bool TcpComm::checkConnection()
{
  if(!connected())
  {
#ifndef WIN32
    unsigned int addrlen = sizeof(sockaddr_in);
#else
    int addrlen = sizeof(sockaddr_in);
#endif

    if(createSocket)
      transferSocket = (int)accept(createSocket, (sockaddr*) &address, &addrlen);
    else if(!wasConnected)
    {
      transferSocket = (int)socket(AF_INET, SOCK_STREAM, 0);
      if (!connected())
		  return false;
      if(connect(transferSocket, (sockaddr*) &address, sizeof(sockaddr_in)) != 0)
      {
        CLOSE(transferSocket);
        transferSocket = 0;
      }
    }

    if(connected())
    {
      wasConnected = true;
      NON_BLOCK(transferSocket); // switch socket to nonblocking
#ifdef MACOSX
      int yes = 1;
      if(!setsockopt(transferSocket, SOL_SOCKET, SO_NOSIGPIPE, &yes, sizeof(yes)))
		  return false;
#endif
      if(maxPackageSendSize && !setsockopt(transferSocket, SOL_SOCKET, SO_SNDBUF, (char*)&maxPackageSendSize, sizeof(maxPackageSendSize)))
		  return false;
      if(maxPackageReceiveSize && !setsockopt(transferSocket, SOL_SOCKET, SO_RCVBUF, (char*)&maxPackageReceiveSize, sizeof(maxPackageReceiveSize)))
		  return false;
      return true;
    }
    else
      return false;
  }
  else
    return true;
}

void TcpComm::closeTransferSocket()
{
  CLOSE(transferSocket);
  transferSocket = 0;
}

bool TcpComm::receive(unsigned char* buffer, int size, bool wait)
{
  if(!checkConnection())
    return false;

  if(!wait)
  {
    RESET_ERRNO;
#ifndef WIN32
    int received = recv(transferSocket, (char*) buffer, size, MSG_PEEK);
    if(received < size)
    {
      if(!received || (received < 0 && ERRNO != EWOULDBLOCK && ERRNO != EINPROGRESS))
        closeTransferSocket();
      return false;
    }
#else
    char c;
    int received = recv(transferSocket, &c, 1, MSG_PEEK);
    if(!received || (received < 0 && ERRNO != EWOULDBLOCK && ERRNO != EINPROGRESS) ||
       ioctlsocket(transferSocket, FIONREAD, (u_long*) &received) != 0)
    {
      closeTransferSocket();
      return false;
    }
    else if(received == 0)
      return false;
#endif
  }

  int received = 0;
  while(received < size)
  {
    RESET_ERRNO;

    int received2 = recv(transferSocket, (char*) buffer + received,
                         size - received, 0);

    if(!received2 || (received2 < 0 && ERRNO != EWOULDBLOCK && ERRNO != EINPROGRESS))  // error during reading of package
    {
      closeTransferSocket();
      return false;
    }
    else if(ERRNO == EWOULDBLOCK || ERRNO == EINPROGRESS) // wait for the rest
    {
      received2 = 0;
      timeval timeout;
      timeout.tv_sec = 0;
      timeout.tv_usec = 100000;
      fd_set rset;
      FD_ZERO(&rset);
      FD_SET(transferSocket, &rset);
      if(select(transferSocket + 1, &rset, 0, 0, &timeout) == -1)
      {
        closeTransferSocket();
        return false; // error while waiting
      }
    }
    received += received2;
    overallBytesReceived += received2;
  }
  return true; // ok, data received
}

int TcpComm::receiveSys(void* buffer, unsigned int len, int flags)
{
  return recv(transferSocket, (char*) buffer, len, flags);
}

bool TcpComm::send(const char* buffer, int size)
{
  if(!checkConnection())
    return false;

  RESET_ERRNO;
  int sent = ::send(transferSocket, (const char*) buffer, size, 0);
  if(sent > 0)
  {
    overallBytesSent += sent;
    while(sent < size && (ERRNO == EWOULDBLOCK || ERRNO == EINPROGRESS || ERRNO == 0))
    {
      timeval timeout;
      timeout.tv_sec = 0;
      timeout.tv_usec = 100000;
      fd_set wset;
      FD_ZERO(&wset);
      FD_SET(transferSocket, &wset);
      RESET_ERRNO;
      if(select(transferSocket + 1, 0, &wset, 0, &timeout) == -1)
        break;
      RESET_ERRNO;
      int sent2 = ::send(transferSocket, (const char*) buffer + sent, size - sent, 0);
      if(sent2 >= 0)
      {
        sent += sent2;
        overallBytesSent += sent;
      }
    }
  }

  if(ERRNO == 0 && sent == size)
    return true;
  else
  {
    closeTransferSocket();
    return false;
  }
}

bool TcpComm::bindSocket()
{
	if (bind(createSocket, (sockaddr*)&address, sizeof(sockaddr_in)) < 0)
	{
		return false;
	}

	return true;
}

bool TcpComm::listenToClient()
{
	if(listen(createSocket, SOMAXCONN) < 0)
	{
		return false;
	}

	return true;
}
