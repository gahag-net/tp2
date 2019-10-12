#pragma once

#include <cstddef>

#include "lib/addrinfo.hpp"


namespace server {
	struct Args {
		AddrInfo address;
	};


	Args parse_args(int argc, char** argv, int socktype);

	void process(uint8_t*, std::size_t);
}
