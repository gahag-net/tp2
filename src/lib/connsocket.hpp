#pragma once

#include <cstdint>
#include <memory>

#include "socket.hpp"
#include "addrinfo.hpp"
#include "serversocket.hpp"


class ConnSocket : public Socket {
public:
	ConnSocket(AddrInfo&&);
	ConnSocket(const ServerSocket&);
	~ConnSocket();

	std::unique_ptr<uint8_t[]> recv(std::size_t&) const;
	std::size_t send(const uint8_t[], std::size_t) const;
};
