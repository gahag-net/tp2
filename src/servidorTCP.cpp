#include <iostream>

#include <signal.h>
#include <sys/types.h>

#include "server.hpp"
#include "lib/serversocket.hpp"
#include "lib/connsocket.hpp"


bool quit = false;

int main(int argc, char** argv) try {
	server::Args args = server::parse_args(argc, argv, SOCK_STREAM);

	signal(
		SIGINT,
		[](int sig) { quit = true; }
	);

	ServerSocket server(
		std::move(args.address),
		32
	);

	while (!quit) {
		ConnSocket conn(server);

		// std::cout << "Connection from " << conn.address() << std::endl;

		size_t size = 80;

		auto data = conn.recv(size);

		server::process(data.get(), size);

		if (conn.send(data.get(), size) != size)
			std::cerr << "warning: sent less bytes than received!";
	}

	// std::cout << "Bye!";

	return 0;
} catch (std::exception& e) {
	std::cerr << "Fatal: " << e.what();
	return -1;
}

