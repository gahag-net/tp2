#include "connsocket.hpp"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <system_error>

#include <sys/socket.h>


ConnSocket::ConnSocket(AddrInfo&& address)
	: Socket(
	  	std::move(address),
	  	Socket::connect
	  )
{ }

ConnSocket::ConnSocket(const ServerSocket& socket)
	: Socket(
	  	// http://man7.org/linux/man-pages/man2/accept.2.html
	  	::accept(socket.descriptor(), nullptr, nullptr)
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

	// http://man7.org/linux/man-pages/man2/recv.2.html
	size = ::recv(this->fd, buffer.get(), size, 0);

	if (size < 0)
		throw std::system_error(errno, std::generic_category());

	return buffer;
}


std::size_t ConnSocket::send(const uint8_t buffer[], std::size_t size) const {
	// http://man7.org/linux/man-pages/man2/sendto.2.html
	size = ::send(this->fd, buffer, size, 0);

	if (size < 0)
		throw std::system_error(errno, std::generic_category());

	return size;
}
