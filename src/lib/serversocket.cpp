#include "serversocket.hpp"

#include <system_error>
#include <cerrno>
#include <memory>
#include <tuple>

#include <sys/socket.h>


ServerSocket::ServerSocket(AddrInfo&& address, bool check_udp)
	: Socket(std::move(address))
{
	if (check_udp)
		if (!this->is_udp())
			throw std::invalid_argument("Call to udp constructor using tcp address");

	int on = 1;

	// allow address reuse if possible:
	if (::setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		throw std::system_error(errno, std::generic_category());

	if (::bind(this->fd, this->addr->ai_addr, this->addr->ai_addrlen) < 0)
		throw std::system_error(errno, std::generic_category());
}

ServerSocket::ServerSocket(AddrInfo&& address)
	: ServerSocket(std::move(address), true)
{ }

ServerSocket::ServerSocket(AddrInfo&& address, uint32_t queue_size)
	: ServerSocket(std::move(address), false)
{
		if (!this->is_tcp())
			throw std::invalid_argument("Call to tcp constructor using udp address");

		if (::listen(this->fd, queue_size) < 0)
			throw std::system_error(errno, std::generic_category());
}


std::tuple<std::unique_ptr<uint8_t[]>, AddrInfo> ServerSocket::recv(
	std::size_t& size
) const {
	auto buffer = std::make_unique<uint8_t[]>(size);

	auto recv = [&](sockaddr* addr, socklen_t* addr_size) {
		size = ::recvfrom(this->fd, buffer.get(), size, 0, addr, addr_size);
		return size;
	};

	auto addr = AddrInfo(recv); // this constructor must be called before moving buffer.

	return std::make_tuple(
		std::move(buffer),
		std::move(addr)
	);
}

std::size_t ServerSocket::send(
	const uint8_t buffer[],
	std::size_t size,
	AddrInfo&& addr
) const {
	auto result = sendto(this->fd, buffer, size, 0, addr->ai_addr, addr->ai_addrlen);

	if (result < 0)
		throw std::system_error(errno, std::generic_category());

	return result;
}


bool ServerSocket::is_tcp() const {
	return this->addr->ai_socktype == SOCK_STREAM;
}

bool ServerSocket::is_udp() const {
	return this->addr->ai_socktype == SOCK_DGRAM;
}
