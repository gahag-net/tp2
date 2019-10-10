#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "lib/addrinfo.hpp"
#include "lib/connsocket.hpp"


struct Args {
	AddrInfo address;
};

Args parse_args(int argc, char** argv) {
	if (argc < 3) {
		std::cerr << "Missing arguments" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
		::exit(1);
	}

	const addrinfo addrinfo = {
		.ai_family = AF_UNSPEC, // accept both ipv4 and ipv6
		.ai_socktype = SOCK_STREAM // force TCP
	};

	return (Args) {
		.address = AddrInfo(
			NameInfo(argv[1], argv[2]),
			&addrinfo
		)
	};
}


std::string read_input() {
	std::ios::sync_with_stdio(false);

  std::cin >> std::noskipws;

  return std::string(
		std::istream_iterator<char>(std::cin),
		std::istream_iterator<char>()
	);
}


int main(int argc, char** argv) try {
	Args args = parse_args(argc, argv);

	std::string input = read_input();

	std::size_t size = input.length();

	ConnSocket conn(std::move(args.address));

	std::cout << "Connected to " << conn.address() << std::endl;

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
} catch (std::exception& e) {
	std::cerr << "Fatal: " << e.what();
	return -1;
}
