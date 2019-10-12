#include "server.hpp"

#include <algorithm>
#include <iostream>
#include <cstdlib>

#include <netdb.h>
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
