#include <iostream>
#include <sstream>
#include <iterator>
#include <cstdint>

#include <netinet/in.h>

#include "lib/connsocket.hpp"


struct Args {
	uint32_t ip;
	short port;
};

Args parse_args(int argc, char** argv) {
	Args args;

	if (argc < 3) {
		std::cerr << "Missing arguments";
		::exit(1);
	}

	// TODO ip

	if (!(std::istringstream(argv[1]) >> args.port)) {
		std::cerr << "Invalid port argument";
		::exit(1);
	}

	return args;
}


std::string read_input() {
	std::ostringstream std_input;

	std_input << std::cin.rdbuf();

	return std_input.str();
}


int main(int argc, char** argv) {
	Args args = parse_args(argc, argv);

	std::string input = read_input();

	std::size_t size = input.length();

	ConnSocket<sockaddr_in6> conn(
		(sockaddr_in6) {
			.sin6_family = AF_INET6,
			.sin6_port = htons(args.port),
			.sin6_addr = in6addr_loopback
		}
	);

	conn.send(
		reinterpret_cast<const uint8_t*>(input.data()),
		size
	);

	input.clear();

	auto result = conn.recv(size);

	std::string output(
		reinterpret_cast<char *>(result.get()),
		size
	);

	std::cout << output;

	return 0;
}
