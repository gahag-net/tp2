#include "serversocket.hpp"

#include <system_error>
#include <cerrno>
#include <memory>
#include <tuple>

#include <sys/socket.h>


ServerSocket::ServerSocket(AddrInfo&& address, uint32_t queue_size)
	: Socket(
	  	std::move(address),
	  	Socket::bind
	  )
{
	if (::listen(this->fd, queue_size) < 0)
		throw std::system_error(errno, std::generic_category());
}
