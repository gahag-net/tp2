#include <iostream>

#include <sys/types.h>

#include "server.hpp"
#include "lib/serversocket.hpp"
#include "lib/connsocket.hpp"


int main(int argc, char** argv) try {
	server::Args args = server::parse_args(argc, argv, SOCK_STREAM);

	ServerSocket server(
		std::move(args.address),
		32
	);

	while (true) {
		ConnSocket conn(server);

		std::cout << "Connection from " << conn.address() << std::endl;

		size_t size = 80;

		auto data = conn.recv(size);

		server::process(data.get(), size);

		if (conn.send(data.get(), size) != size)
			std::cerr << "warning: sent less bytes than received!";
	}
} catch (std::exception& e) {
	std::cerr << "Fatal: " << e.what();
	return -1;
}

