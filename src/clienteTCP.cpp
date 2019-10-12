#include <cstdint>
#include <iostream>

#include <sys/types.h>

#include "client.hpp"
#include "lib/addrinfo.hpp"
#include "lib/connsocket.hpp"


int main(int argc, char** argv) try {
	client::Args args = client::parse_args(argc, argv, SOCK_STREAM);

	std::string input = client::read_input();

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

	return 0;
} catch (std::exception& e) {
	std::cerr << "Fatal: " << e.what();
	return -1;
}
