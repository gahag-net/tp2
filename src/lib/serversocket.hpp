#pragma once

#include <cstdint>
#include <tuple>
#include <memory>

#include "addrinfo.hpp"
#include "socket.hpp"


class ServerSocket : public Socket {
protected:
	ServerSocket(AddrInfo&&, bool check_udp);

public:
	ServerSocket(AddrInfo&&);
	ServerSocket(AddrInfo&&, uint32_t queue_size);

	std::tuple<std::unique_ptr<uint8_t[]>, AddrInfo> recv(std::size_t&) const;
	std::size_t send(const uint8_t[], std::size_t, AddrInfo&&) const;

	bool is_tcp() const;
	bool is_udp() const;
};
