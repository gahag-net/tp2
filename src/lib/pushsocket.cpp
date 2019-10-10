#include "pushsocket.hpp"


PushSocket::PushSocket(AddrInfo&& address, int (&attach)(const Socket&))
	: Socket(
	  	std::move(address),
	  	attach
	  )
{ }


std::tuple<std::unique_ptr<uint8_t[]>, AddrInfo> PushSocket::recv(
	std::size_t& size
) const {
	auto buffer = std::make_unique<uint8_t[]>(size);

	auto recv = [&](int fd, sockaddr* addr, socklen_t* addr_size) {
		size = ::recvfrom(fd, buffer.get(), size, 0, addr, addr_size);
		return size;
	};

	auto addr = AddrInfo(recv, this->fd); // this constructor must be called before moving buffer.

	return std::make_tuple(
		std::move(buffer),
		std::move(addr)
	);
}

std::size_t PushSocket::send(
	const uint8_t buffer[],
	std::size_t size,
	const AddrInfo& addr
) const {
	auto result = sendto(this->fd, buffer, size, 0, addr->ai_addr, addr->ai_addrlen);

	if (result < 0)
		throw std::system_error(errno, std::generic_category());

	return result;
}
