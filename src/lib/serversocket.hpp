#pragma once


#include "addrinfo.hpp"
#include "socket.hpp"


// A TCP server socket.
class ServerSocket : public Socket {
public:
	ServerSocket(AddrInfo&&, uint32_t queue_size);
};
