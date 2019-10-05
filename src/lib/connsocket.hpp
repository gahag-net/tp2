#pragma once

#include <cstdint>
#include <memory>

#include "socket.hpp"
#include "serversocket.hpp"


template<typename Addr>
class ConnSocket : public Socket {
public:
	ConnSocket(const Addr&);
	ConnSocket(const ServerSocket<Addr>&);
	~ConnSocket();

	std::unique_ptr<uint8_t[]> recv(std::size_t&) const;
	std::size_t send(const uint8_t[], std::size_t) const;

	Addr address() const;
};
