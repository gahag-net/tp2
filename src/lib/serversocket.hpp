#pragma once

#include <cstdint>

#include "addrinfo.hpp"
#include "socket.hpp"


class ServerSocket : public Socket {
public:
	ServerSocket(AddrInfo&&, uint32_t queue_size);
};
