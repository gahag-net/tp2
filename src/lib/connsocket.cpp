#include "connsocket.hpp"

#include <iostream>
#include <cstring>
#include <memory>

#include <sys/socket.h>
#include <netinet/in.h>

#include "socket.hpp"


template<typename Addr>
ConnSocket<Addr>::ConnSocket(
	const Addr& address
) : Socket(::socket(AF_INET6, SOCK_STREAM, 0)) {
	auto sockaddr = reinterpret_cast<const struct sockaddr *>(&address);

	if (::connect(this->fd, sockaddr, sizeof(Addr)) < 0)
		throw std::system_error(errno, std::generic_category());
}


template<typename Addr>
ConnSocket<Addr>::ConnSocket(
	const ServerSocket<Addr>& socket
) : Socket(::accept(socket.descriptor(), nullptr, nullptr)) { }


template<typename Addr>
ConnSocket<Addr>::~ConnSocket() {
	if (::shutdown(this->fd, SHUT_RDWR) < 0)
		std::cerr << "Failed to close connection (fd = " << this->fd << "):"
		          << std::endl
		          << std::strerror(errno);
}


template<typename Addr>
std::unique_ptr<uint8_t[]> ConnSocket<Addr>::recv(std::size_t& size) const {
	auto buffer = std::make_unique<uint8_t[]>(size);

	size = ::recv(this->fd, buffer.get(), size, 0);

	if (size < 0)
		throw std::system_error(errno, std::generic_category());

	return buffer;
}


template<typename Addr>
std::size_t ConnSocket<Addr>::send(const uint8_t buffer[], std::size_t size) const {
	size = ::send(this->fd, buffer, size, 0);

	if (size < 0)
		throw std::system_error(errno, std::generic_category());

	return size;
}


template<typename Addr>
Addr ConnSocket<Addr>::address() const {
	Addr address;

	auto sockaddr = reinterpret_cast<struct sockaddr *>(&address);

	uint32_t size = sizeof(Addr);

	if (::getpeername(this->fd, sockaddr, &size) < 0)
		throw std::system_error(errno, std::generic_category());

	return address;
}


template class ConnSocket<sockaddr_in6>;
template class ConnSocket<sockaddr_in>;
