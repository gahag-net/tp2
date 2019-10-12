#include <cstdint>
#include <iostream>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "client.hpp"
#include "lib/addrinfo.hpp"
#include "lib/pushsocket.hpp"


int main(int argc, char** argv) try {
	client::Args args = client::parse_args(argc, argv, SOCK_DGRAM);

	std::string input = client::read_input();

	std::size_t size = input.length();

	PushSocket sock(
		AddrInfo(
			NameInfo("::", "0"),
			(addrinfo) {
				.ai_family = AF_UNSPEC, // accept both ipv4 and ipv6
				.ai_socktype = SOCK_DGRAM // force UDP
			}
		),
		Socket::push
	);

	sock.send(
		reinterpret_cast<const uint8_t*>(input.data()),
		size,
		args.address
	);

	std::cout << "Sent to " << args.address << std::endl;

	input.clear();

	auto [ result, address ] = sock.recv(size);

	std::cout << "Response from " << address << std::endl;

	std::string output(
		reinterpret_cast<char *>(result.get()),
		size
	);

	std::cout << output;

	return 0;
} catch (std::exception& e) {
	std::cerr << "Fatal: " << e.what();
	return -1;
}
