#include <iostream>
#include <algorithm>
#include <sstream>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "lib/serversocket.hpp"
#include "lib/connsocket.hpp"


struct Args {
	AddrInfo address;
};

Args parse_args(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Missing port argument";
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		::exit(1);
	}

	const char* port = argv[1];

	const addrinfo addrinfo = {
		.ai_family = AF_UNSPEC, // accept both ipv4 and ipv6
		.ai_socktype = SOCK_STREAM // force TCP
	};

	return (Args) {
		.address = AddrInfo(
			"::",
			port,
			&addrinfo
		)
	};
}


int main(int argc, char** argv) {
	Args args = parse_args(argc, argv);

	ServerSocket server(std::move(args.address), 32);

	while (true) {
		ConnSocket conn(server);

		std::cout << "Connection from " << conn.address() << std::endl;

		size_t size = 80;

		auto data = conn.recv(size);

		auto data_begin = data.get();
		auto data_end = data_begin + size;

		std::reverse(data_begin, data_end);

		std::transform(
			data_begin,
			data_end,
			data_begin,
			[](char c) {
				return std::isupper(c) ? std::tolower(c)
				                       : std::toupper(c);
			}
		);

		if (conn.send(data.get(), size) != size)
			std::cerr << "warning: sent less bytes than received!";
	}
}
