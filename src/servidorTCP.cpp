#include <iostream>
#include <algorithm>
#include <sstream>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "lib/serversocket.hpp"
#include "lib/connsocket.hpp"


struct Args {
	short port;
};

Args parse_args(int argc, char** argv) {
	Args args;

	if (argc < 2) {
		std::cerr << "Missing port argument";
		::exit(1);
	}

	if (!(std::istringstream(argv[1]) >> args.port)) {
		std::cerr << "Invalid port argument";
		::exit(1);
	}

	return args;
}


std::string addr_to_string(sockaddr_in6 addr) {
	char buffer[INET6_ADDRSTRLEN];

	if (!::inet_ntop(AF_INET6, &addr.sin6_addr, buffer, sizeof(buffer)))
		throw std::system_error(errno, std::generic_category());

	std::ostringstream result;

	result << buffer << ':' << ntohs(addr.sin6_port);

	return result.str();
}


int main(int argc, char** argv) {
	Args args = parse_args(argc, argv);

	ServerSocket<sockaddr_in6> server(
		sockaddr_in6 {
			.sin6_family = AF_INET6,
			.sin6_addr = in6addr_any,
			.sin6_port = htons(args.port)
		},
		32
	);

	while (true) {
		ConnSocket<sockaddr_in6> conn(server);

		std::cout << "Connection from " << addr_to_string(conn.address()) << std::endl;

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
