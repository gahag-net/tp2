#pragma once

#include "addrinfo.hpp"


// A basic socket. The derivations of this class provide the useful communication methods.
class Socket {
protected:
	int fd; // The socket's file descriptor, or -1 when deleted.
	AddrInfo addr;

	Socket(int fd);
	Socket(int fd, AddrInfo&& addr);

	bool deleted() const;

public:
	// Construct with an attach function.
	// The function is called right after construction to attach this socket.
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


	// Common attach functions:

	// Bind for server sockets.
	static int bind(const Socket&);
	// Connect for TCP client sockets.
	static int connect(const Socket&);
	// A no-op function for UDP sockets.
	static int push(const Socket&);
};
