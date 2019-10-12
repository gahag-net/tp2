#pragma once

#include <string>

#include "lib/addrinfo.hpp"


namespace client {
	struct Args {
		AddrInfo address;
	};


	Args parse_args(int argc, char** argv, int socktype);

	std::string read_input();
}
