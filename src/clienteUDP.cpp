#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "lib/addrinfo.hpp"
#include "lib/pushsocket.hpp"


struct Args {
	AddrInfo addr;
};

Args parse_args(int argc, char** argv) {
	if (argc < 3) {
		std::cerr << "Missing arguments" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
		::exit(1);
	}

	const char* ip = argv[1];
	const char* port = argv[2];

	const addrinfo addrinfo = {
		.ai_family = AF_UNSPEC, // accept both ipv4 and ipv6
		.ai_socktype = SOCK_DGRAM // force UDP
	};

	return (Args) {
		.addr = AddrInfo(
			ip,
			port,
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

	addrinfo addrinfo = {
		.ai_family = AF_UNSPEC, // accept both ipv4 and ipv6
		.ai_socktype = SOCK_DGRAM // force UDP
	};

	PushSocket sock(
		AddrInfo(
			"::",
			"0",
			&addrinfo
		),
		Socket::push
	);

	sock.send(
		reinterpret_cast<const uint8_t*>(input.data()),
		size,
		args.addr
	);

	std::cout << "Sent to " << args.addr << std::endl;

	input.clear();

	auto [ result, addr ] = sock.recv(size);

	std::cout << "Response from " << addr << std::endl;

	std::string output(
		reinterpret_cast<char *>(result.get()),
		size
	);

	std::cout << output;
} catch (std::exception& e) {
	std::cerr << "Fatal: " << e.what();
	return -1;
}
