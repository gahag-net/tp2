#include "serversocket.hpp"

#include <system_error>
#include <cerrno>

#include <sys/socket.h>


ServerSocket::ServerSocket(AddrInfo&& address, uint32_t queue_size)
	: Socket(std::move(address))
{
	int on = 1;

	// allow address reuse if possible:
	if (::setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		throw std::system_error(errno, std::generic_category());

	if (::bind(this->fd, this->addr->ai_addr, this->addr->ai_addrlen) < 0)
		throw std::system_error(errno, std::generic_category());

	if (::listen(this->fd, queue_size) < 0)
		throw std::system_error(errno, std::generic_category());
}
