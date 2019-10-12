#include "client.hpp"

#include <iostream>
#include <iterator>
#include <cstdlib>

#include <netdb.h>
#include <sys/socket.h>


client::Args client::parse_args(int argc, char** argv, int socktype) {
	if (argc < 3) {
		std::cerr << "Missing arguments" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
		::exit(1);
	}

	return (Args) {
		.address = AddrInfo(
			NameInfo(argv[1], argv[2]),
			(addrinfo) {
				.ai_family = AF_UNSPEC, // accept both ipv4 and ipv6
				.ai_socktype = socktype
			}
		)
	};
}


std::string client::read_input() {
	std::ios::sync_with_stdio(false);

  std::cin >> std::noskipws;

  return std::string(
		std::istream_iterator<char>(std::cin),
		std::istream_iterator<char>()
	);
}
