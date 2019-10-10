#include "addrinfo.hpp"

#include <cerrno>
#include <system_error>


NameInfo AddrInfo::get_nameinfo(struct sockaddr* addr, socklen_t size) {
	char host[256];
	char port[256];

	auto result = ::getnameinfo(
		addr,
		size,
		host,
		sizeof(host),
		port,
		sizeof(port),
		0
	);

	if (result != 0)
		throw AddrInfo::eai_exception(result);

	return {
		.node = std::string(host),
		.service = std::string(port)
	};
}

std::system_error AddrInfo::eai_exception(int eai_error) {
	return eai_error == EAI_SYSTEM
		? std::system_error(
				errno,
				std::generic_category()
			)
		: std::system_error(
				std::error_code(
					eai_error,
					std::generic_category()
				),
				gai_strerror(eai_error)
			);
}


AddrInfo::AddrInfo(
	const std::function<int(int, sockaddr*, socklen_t*)>& filladdr,
	int fd
) {
	struct sockaddr_in6 address; // sockaddr_in6 is the biggest of sockaddrs.

	// filladdr:
	socklen_t size = sizeof(address);
	socklen_t original_size = size;

	auto sockaddr = reinterpret_cast<struct sockaddr *>(&address);

	if (filladdr(fd, sockaddr, &size) < 0)
		throw std::system_error(errno, std::generic_category());

	if (size > original_size)
		throw std::length_error("sockaddr_in6 too small");

	// nameinfo:
	auto nameinfo = AddrInfo::get_nameinfo(sockaddr, size);

	// addrinfo hint:
	addrinfo addrinfo_hint {
		.ai_family = sockaddr->sa_family
	};
	size = sizeof(addrinfo_hint.ai_socktype);
	original_size = size;

	if (::getsockopt(fd, SOL_SOCKET, SO_TYPE, &addrinfo_hint.ai_socktype, &size) < 0)
		throw std::system_error(errno, std::generic_category());

	if (size > original_size)
		throw std::length_error("socktype too small");

	// addrinfo:
	auto result = ::getaddrinfo(
		nameinfo.node.c_str(),
		nameinfo.service.c_str(),
		&addrinfo_hint,
		&this->data
	);

	if (result != 0)
		throw AddrInfo::eai_exception(result);
}

AddrInfo::AddrInfo(int fd)
	: AddrInfo(
	  	[](int fd, sockaddr* addr, socklen_t* size) {
	  		if (fd < 0)
	  			throw std::system_error(errno, std::generic_category());

	  		return ::getpeername(fd, addr, size);
	  	},
	  	fd
	  )
{ }

AddrInfo::AddrInfo(const char* node, const char* service, const addrinfo* hints) {
	auto result = ::getaddrinfo(node, service, hints, &this->data);

	if (result != 0)
		throw AddrInfo::eai_exception(result);
}

AddrInfo::AddrInfo(AddrInfo&& other) : data(other.data) {
	other.data = nullptr;
}

AddrInfo::~AddrInfo() {
	if (this->data != nullptr)
		freeaddrinfo(this->data);
}


AddrInfo& AddrInfo::operator=(AddrInfo&& other) {
	this->~AddrInfo();

	this->data = other.data;

	return *this;
}


addrinfo& AddrInfo::operator*() {
	return *this->data;
}

addrinfo* AddrInfo::operator->() {
	return this->data;
}

const addrinfo& AddrInfo::operator*() const {
	return *this->data;
}

const addrinfo* AddrInfo::operator->() const {
	return this->data;
}


std::ostream& operator<<(std::ostream &stream, const AddrInfo &address) {
	auto nameinfo = AddrInfo::get_nameinfo(address->ai_addr, address->ai_addrlen);

  stream << nameinfo.node << ':' << nameinfo.service;

	switch (address->ai_family) {
		case AF_INET: stream << " (IPv4"; break;
		case AF_INET6: stream << " (IPv6"; break;
		default: stream << " (Unknown"; break;
	}

	switch (address->ai_socktype) {
		case SOCK_STREAM: stream << ", TCP)"; break;
		case SOCK_DGRAM: stream << ", UDP)"; break;
		default: stream << ", RAW)"; break;
	}

	return stream;
}
