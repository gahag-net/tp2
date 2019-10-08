#include "connsocket.hpp"

#include <iostream>
#include <cstring>
#include <memory>

#include <sys/socket.h>
#include <netinet/in.h>

#include "socket.hpp"


ConnSocket::ConnSocket(AddrInfo&& address) : Socket(std::move(address)) {
	if (this->addr->ai_socktype != SOCK_STREAM)
		throw std::invalid_argument("ConnSocket must be used with TCP adresses");

	if (::connect(this->fd, this->addr->ai_addr, this->addr->ai_addrlen) < 0)
		throw std::system_error(errno, std::generic_category());
}


ConnSocket::ConnSocket(const ServerSocket& socket)
	: Socket(
			[&] {
				if (socket.address()->ai_socktype != SOCK_STREAM)
					throw std::invalid_argument("ConnSocket must be used with TCP adresses");

				return ::accept(socket.descriptor(), nullptr, nullptr);
			}()
		)
{ }


ConnSocket::~ConnSocket() {
	if (::shutdown(this->fd, SHUT_RDWR) < 0)
		std::cerr << "Failed to close connection (fd = " << this->fd << "):"
		          << std::endl
		          << std::strerror(errno);
}


std::unique_ptr<uint8_t[]> ConnSocket::recv(std::size_t& size) const {
	auto buffer = std::make_unique<uint8_t[]>(size);

	size = ::recv(this->fd, buffer.get(), size, 0);

	if (size < 0)
		throw std::system_error(errno, std::generic_category());

	return buffer;
}


std::size_t ConnSocket::send(const uint8_t buffer[], std::size_t size) const {
	size = ::send(this->fd, buffer, size, 0);

	if (size < 0)
		throw std::system_error(errno, std::generic_category());

	return size;
}
