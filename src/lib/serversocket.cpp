#include "serversocket.hpp"

#include <cerrno>
#include <system_error>

#include <sys/socket.h>


ServerSocket::ServerSocket(AddrInfo&& address, uint32_t queue_size)
	: Socket(
	  	std::move(address),
	  	Socket::bind
	  )
{
	// http://man7.org/linux/man-pages/man2/listen.2.html
	if (::listen(this->fd, queue_size) < 0)
		throw std::system_error(errno, std::generic_category());
}
