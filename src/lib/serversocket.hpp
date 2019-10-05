#pragma once

#include <cstdint>

#include "socket.hpp"


template<typename Addr>
class ServerSocket : public Socket {
public:
	ServerSocket(const Addr&, uint32_t queue_size);
};
