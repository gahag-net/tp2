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
	Socket(AddrInfo&&, int (&attach)(const Socket&));
	Socket(const Socket&) = delete;
	Socket(Socket&&);
	~Socket();

	Socket& operator=(const Socket&) = delete;
	Socket& operator=(Socket&&);

	int descriptor() const;
	const AddrInfo& address() const;

	bool is_tcp() const;
	bool is_udp() const;


	static int bind(const Socket&);
	static int connect(const Socket&);
	static int push(const Socket&);
};
