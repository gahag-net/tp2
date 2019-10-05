cc = clang++ # g++
cflags = -std=c++17 -g
clibs = -lnsl


%: src/%.cpp
	${cc} ${cflags} ${clibs} $< src/lib/*.cpp -o $@


udp: clienteUDP servidorUDP

tcp: clienteTCP servidorTCP
