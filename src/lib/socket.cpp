#include "socket.hpp"

#include <iostream>
#include <cstring>
#include <cerrno>

#include <sys/socket.h>
#include <unistd.h>


Socket::Socket(int fd) : fd(fd) {
	if (fd < 0)
		throw std::system_error(errno, std::generic_category());
}

Socket::Socket(Socket&& other) : fd(other.fd) {
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
	if (!this->deleted())
		Socket(this->fd); // call destructor for the inner socket.

	this->fd = other.fd;

	return *this;
}


bool Socket::deleted() const {
	return this->fd == -1;
}


int Socket::descriptor() const {
	return this->fd;
}
