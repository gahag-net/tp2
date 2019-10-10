#pragma once

#include <cstdint>
#include <tuple>
#include <memory>

#include "addrinfo.hpp"
#include "socket.hpp"


class PushSocket : public Socket {
public:
	PushSocket(AddrInfo&&, int (&attach)(const Socket&));

	std::tuple<std::unique_ptr<uint8_t[]>, AddrInfo> recv(std::size_t&) const;
	std::size_t send(const uint8_t[], std::size_t, const AddrInfo&) const;
};
