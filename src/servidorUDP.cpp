#include <iostream>

#include <sys/types.h>

#include "server.hpp"
#include "lib/pushsocket.hpp"


int main(int argc, char** argv) {
	server::Args args = server::parse_args(argc, argv, SOCK_DGRAM);

	PushSocket sock(
		std::move(args.address),
		Socket::bind
	);

	while (true) {
		size_t size = 80;

		auto [ data, addr ] = sock.recv(size);

		std::cout << "Connection from " << addr << std::endl;

		server::process(data.get(), size);

		if (sock.send(data.get(), size, addr) != size)
			std::cerr << "warning: sent less bytes than received!";
	}
}
