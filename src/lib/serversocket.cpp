#include "serversocket.hpp"

#include <iostream>
#include <cstring>
#include <cerrno>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


template<typename Addr>
ServerSocket<Addr>::ServerSocket(
	const Addr& address,
	uint32_t queue_size
) : Socket(::socket(AF_INET6, SOCK_STREAM, 0)) {
	auto sockaddr = reinterpret_cast<const struct sockaddr *>(&address);

	int on = 1;

	// allow address reuse if possible:
	if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		throw std::system_error(errno, std::generic_category());

	if (::bind(this->fd, sockaddr, sizeof(Addr)) < 0)
		throw std::system_error(errno, std::generic_category());

	if (::listen(this->fd, queue_size) < 0)
		throw std::system_error(errno, std::generic_category());
}


template class ServerSocket<sockaddr_in6>;
template class ServerSocket<sockaddr_in>;
