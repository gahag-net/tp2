#pragma once

#include <system_error>
#include <ostream>
#include <functional>

#include <netdb.h>


struct NameInfo {
public:
	std::string node;
	std::string service;
};


class AddrInfo {
protected:
	addrinfo* data;

public:
	static NameInfo get_nameinfo(struct sockaddr* addr, socklen_t size);
	static std::system_error eai_exception(int);

	AddrInfo(std::function<int(sockaddr*, socklen_t*)>&& fillAddr);
	AddrInfo(int fd);
	AddrInfo(const char* node, const char* service, const addrinfo* hints);
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


std::ostream& operator<<(std::ostream &stream, const AddrInfo &address);
