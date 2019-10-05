#pragma once


class Socket {
protected:
	int fd; // The socket's file descriptor, or -1 when deleted.

	Socket(int fd);

	bool deleted() const;

public:
	Socket(const Socket&) = delete;
	Socket(Socket&&);
	~Socket();

	Socket& operator=(const Socket&) = delete;
	Socket& operator=(Socket&&);

	int descriptor() const;
};
