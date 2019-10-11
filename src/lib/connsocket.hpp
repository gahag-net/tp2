#pragma once

#include <cstdint>
#include <memory>

#include "addrinfo.hpp"
#include "serversocket.hpp"
#include "socket.hpp"


// A TCP connection socket.
class ConnSocket : public Socket {
public:
	// Connect to an address.
	ConnSocket(AddrInfo&&);
	// Accept connection from TCP server.
	ConnSocket(const ServerSocket&);
	~ConnSocket();

	std::unique_ptr<uint8_t[]> recv(std::size_t&) const;
	std::size_t send(const uint8_t[], std::size_t) const;
};
