#pragma once


#include "addrinfo.hpp"
#include "socket.hpp"


class ServerSocket : public Socket {
public:
	ServerSocket(AddrInfo&&, uint32_t queue_size);
};
