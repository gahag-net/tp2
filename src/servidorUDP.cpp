#include <atomic>
#include <iostream>

#include <signal.h>
#include <sys/types.h>

#include "server.hpp"
#include "lib/pushsocket.hpp"


std::atomic<bool> interrupted = false;


int main(int argc, char** argv) try {
	server::Args args = server::parse_args(argc, argv, SOCK_DGRAM);

	server::sig_handler(
		SIGINT,
		[](int) {
			interrupted = true;
		}
	);

	PushSocket sock(
		std::move(args.address),
		Socket::bind
	);

	while (!interrupted) {
		size_t size = 80;

		auto [ data, addr ] = sock.recv(size);

		// std::cout << "Connection from " << addr << std::endl;

		server::process(data.get(), size);

		if (sock.send(data.get(), size, addr) != size)
			std::cerr << "warning: sent less bytes than received!";
	}

	// std::cout << "Bye!";

	return server::interrupted();
} catch (const std::system_error& e) {
	return server::sys_error(e);
} catch (const std::exception& e) {
	return server::exception(e);
}
