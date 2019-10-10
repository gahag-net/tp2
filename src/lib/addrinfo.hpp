#pragma once

#include <system_error>
#include <ostream>
#include <functional>

#include <netdb.h>


// A node:service pair.
class NameInfo {
public:
	std::string node;
	std::string service;

	NameInfo() = default;
	NameInfo(std::string&& node, std::string&& service);
	NameInfo(struct sockaddr* addr, socklen_t size);
	~NameInfo() = default;
};


// A socket address structure.
class AddrInfo {
protected:
	addrinfo* data; // nullptr when deleted.

public:
	static std::system_error eai_exception(int eai_error);

	// Construct from a file descriptor.
	AddrInfo(int fd);
	// Construct from a file descriptor and a function to fill a sockaddr.
	AddrInfo(int fd, const std::function<int(int, sockaddr*, socklen_t*)>& fillAddr);
	// Construct from a NameInfo.
	AddrInfo(const NameInfo& nameinfo, const addrinfo* hints);
	AddrInfo(const AddrInfo&) = delete;
	AddrInfo(AddrInfo&&);
	~AddrInfo();

	AddrInfo& operator=(const AddrInfo&) = delete;
	AddrInfo& operator=(AddrInfo&&);

	addrinfo& operator*();
	addrinfo* operator->();

	const addrinfo& operator*() const;
	const addrinfo* operator->() const;
};


std::ostream& operator<<(std::ostream& stream, const AddrInfo& address);
