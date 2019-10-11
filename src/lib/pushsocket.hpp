#pragma once

#include <cstdint>
#include <memory>
#include <tuple>

#include "addrinfo.hpp"
#include "socket.hpp"


// A UDP socket
class PushSocket : public Socket {
public:
	// attach must be bind for a server, or push for a client.
	PushSocket(AddrInfo&&, int (&attach)(const Socket&));

	std::tuple<std::unique_ptr<uint8_t[]>, AddrInfo> recv(std::size_t&) const;
	std::size_t send(const uint8_t[], std::size_t, const AddrInfo&) const;
};
