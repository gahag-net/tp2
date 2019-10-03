cc = clang++ # g++
cflags = -g
clibs = -lnsl # -lsocket


%: src/%.cpp
	${cc} ${cflags} ${clibs} $< -o $@


udp: clienteUDP servidorUDP

tcp: clienteTCP servidorTCP
