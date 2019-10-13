#include <iostream>

#include <signal.h>
#include <sys/types.h>

#include "server.hpp"
#include "lib/pushsocket.hpp"


bool quit = false;

int main(int argc, char** argv) try {
	server::Args args = server::parse_args(argc, argv, SOCK_DGRAM);

	signal(
		SIGINT,
		[](int sig) { quit = true; }
	);

	PushSocket sock(
		std::move(args.address),
		Socket::bind
	);

	while (!quit) {
		size_t size = 80;

		auto [ data, addr ] = sock.recv(size);

		// std::cout << "Connection from " << addr << std::endl;

		server::process(data.get(), size);

		if (sock.send(data.get(), size, addr) != size)
			std::cerr << "warning: sent less bytes than received!";
	}

	// std::cout << "Bye!";

	return 0;
} catch (std::exception& e) {
	std::cerr << "Fatal: " << e.what();
	return -1;
}
