#include "server.hpp"

#include <algorithm>
#include <iostream>
#include <cerrno>
#include <cstdlib>

#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>


server::Args server::parse_args(int argc, char** argv, int socktype) {
	if (argc < 2) {
		std::cerr << "Missing port argument";
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		::exit(1);
	}

	return (Args) {
		.address = AddrInfo(
			NameInfo("::", argv[1]),
			(addrinfo) {
				.ai_family = AF_UNSPEC, // accept both ipv4 and ipv6
				.ai_socktype = socktype // force TCP
			}
		)
	};
}


void server::sig_handler(int signal, void (*handler)(int)) {
	struct sigaction a = {
		.sa_handler = handler,
	};

	if (sigemptyset(&a.sa_mask) != 0)
		throw std::system_error(errno, std::generic_category());

	if (sigaction(signal, &a, nullptr) != 0)
		throw std::system_error(errno, std::generic_category());
}

int server::interrupted() {
	std::cerr << "Interrupted!" << std::endl;
	return 1;
}

int server::exception(const std::exception& e) {
	std::cerr << "Fatal: " << e.what() << std::endl;
	return -1;
}

int server::sys_error(const std::system_error& e) {
	return e.code().value() == EINTR
		? server::interrupted()
		: server::exception(e);
}


void server::process(uint8_t* data, std::size_t size) {
	std::reverse(data, data + size);

	std::transform(
		data,
		data + size,
		data,
		[](char c) {
			return std::isupper(c) ? std::tolower(c)
				: std::toupper(c);
		}
	);
}
