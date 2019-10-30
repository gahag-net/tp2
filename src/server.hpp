#pragma once

#include <cstddef>
#include <exception>
#include <system_error>

#include "lib/addrinfo.hpp"


namespace server {
	struct Args {
		AddrInfo address;
	};

	Args parse_args(int argc, char** argv, int socktype);


	void sig_handler(int signal, void (*handler)(int));
	int interrupted();
	int exception(const std::exception&);
	int sys_error(const std::system_error&);


	void process(uint8_t*, std::size_t);
}
