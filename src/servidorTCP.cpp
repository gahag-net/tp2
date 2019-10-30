#include <atomic>
#include <iostream>
#include <system_error>

#include <signal.h>
#include <sys/types.h>

#include "server.hpp"
#include "lib/serversocket.hpp"
#include "lib/connsocket.hpp"


std::atomic<bool> interrupted = false;


int main(int argc, char** argv) try {
	server::Args args = server::parse_args(argc, argv, SOCK_STREAM);

	server::sig_handler(
		SIGINT,
		[](int) {
			interrupted = true;
		}
	);

	ServerSocket server(
		std::move(args.address),
		32
	);

	while (!interrupted) {
		ConnSocket conn(server);

		// std::cout << "Connection from " << conn.address() << std::endl;

		size_t size = 80;

		auto data = conn.recv(size);

		server::process(data.get(), size);

		if (conn.send(data.get(), size) != size)
			std::cerr << "warning: sent less bytes than received!";
	}

	// std::cout << "Bye!";

	return server::interrupted();
} catch (const std::system_error& e) {
	return server::sys_error(e);
} catch (const std::exception& e) {
	return server::exception(e);
}
