cc = clang++ # g++

cflags = -std=c++17 -O2
cincludes := $(shell pkg-config --cflags libnsl)

lflags = -flto
clibs := $(shell pkg-config --libs libnsl)


objdir = obj


${objdir}/%.o: src/%.cpp
	mkdir -p $(dir $@)
	${cc} ${cflags} ${cincludes} -c $< -o $@


clienteTCP: obj/lib/serversocket.o obj/lib/socket.o obj/lib/connsocket.o obj/clienteTCP.o
	${cc} ${lflags} $+ -o clienteTCP


tcp: clienteTCP servidorTCP

udp: clienteUDP servidorUDP


all: udp tcp


clean:
	rm -r ${objdir}
