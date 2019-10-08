#pragma once

#include "addrinfo.hpp"


class Socket {
protected:
	int fd; // The socket's file descriptor, or -1 when deleted.
	AddrInfo addr;

	Socket(int fd);
	Socket(int fd, AddrInfo&& addr);

	bool deleted() const;

public:
	Socket(AddrInfo&& address);
	Socket(const Socket&) = delete;
	Socket(Socket&&);
	~Socket();

	Socket& operator=(const Socket&) = delete;
	Socket& operator=(Socket&&);

	int descriptor() const;
	const AddrInfo& address() const;
};
