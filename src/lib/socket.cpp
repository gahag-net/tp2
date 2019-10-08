#include "socket.hpp"

#include <iostream>
#include <cstring>
#include <cerrno>
#include <stdexcept>

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>


Socket::Socket(int fd) : fd(fd), addr(fd) {
	if (fd < 0)
		throw std::system_error(errno, std::generic_category());
}

Socket::Socket(int fd, AddrInfo&& addr) : fd(fd), addr(std::move(addr)) {
	if (fd < 0)
		throw std::system_error(errno, std::generic_category());
}

Socket::Socket(
	AddrInfo&& address
) : Socket(
			::socket(address->ai_family, address->ai_socktype, address->ai_protocol),
			std::move(address)
		)
{ }

Socket::Socket(Socket&& other) : fd(other.fd), addr(std::move(other.addr)) {
	other.fd = -1; // mark other as deleted.
}

Socket::~Socket() {
	if (!this->deleted())
		if (::close(this->fd) < 0) {
			std::cerr << "Failed to close connection (fd = " << this->fd << "):"
			          << std::endl
			          << std::strerror(errno);
		}
}

Socket& Socket::operator=(Socket&& other) {
	this->~Socket();

	this->fd = other.fd;

	this->addr = std::move(other.addr);

	return *this;
}


bool Socket::deleted() const {
	return this->fd == -1;
}


int Socket::descriptor() const {
	return this->fd;
}

const AddrInfo& Socket::address() const {
	return this->addr;
}
