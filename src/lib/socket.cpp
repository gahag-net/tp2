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
	AddrInfo&& address,
	int (&attach)(const Socket&)
) : Socket(
	  	::socket(address->ai_family, address->ai_socktype, address->ai_protocol),
	  	std::move(address)
	  )
{
	if (attach(*this) < 0)
		throw std::system_error(errno, std::generic_category());
}

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


bool Socket::is_tcp() const {
	return this->addr->ai_socktype == SOCK_STREAM;
}

bool Socket::is_udp() const {
	return this->addr->ai_socktype == SOCK_DGRAM;
}


int Socket::bind(const Socket& sock) {
	auto fd = sock.descriptor();
	auto& addr = sock.address();

	int on = 1;

	// allow address reuse if possible:
	if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		throw std::system_error(errno, std::generic_category());

	return ::bind(fd, addr->ai_addr, addr->ai_addrlen);
}

int Socket::connect(const Socket& sock) {
	if (!sock.is_tcp())
		throw std::invalid_argument("Call to tcp connect using udp address");

	auto fd = sock.descriptor();
	auto& addr = sock.address();

	return ::connect(fd, addr->ai_addr, addr->ai_addrlen);
}

int Socket::push(const Socket& sock) {
	if (!sock.is_udp())
		throw std::invalid_argument("Call to udp socket using tcp address");

	return 0;
}
