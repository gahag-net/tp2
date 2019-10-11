cc = clang++ # g++

cflags = -std=c++17 -O2 -g
cincludes := $(shell pkg-config --cflags libnsl)

lflags = -flto -g
llibs := $(shell pkg-config --libs libnsl)


objdir = obj


${objdir}/%.o: src/%.cpp
	mkdir -p $(dir $@)
	${cc} ${cflags} ${cincludes} -c $< -o $@


clienteTCP: obj/lib/addrinfo.o obj/lib/socket.o obj/lib/connsocket.o obj/clienteTCP.o
	${cc} ${lflags} ${llibs} $+ -o $@

servidorTCP: obj/lib/addrinfo.o obj/lib/socket.o obj/lib/serversocket.o obj/lib/connsocket.o obj/servidorTCP.o
	${cc} ${lflags} ${llibs} $+ -o $@

tcp: clienteTCP servidorTCP


clienteUDP: obj/lib/addrinfo.o obj/lib/socket.o obj/lib/pushsocket.o obj/clienteUDP.o
	${cc} ${lflags} ${llibs} $+ -o $@

servidorUDP: obj/lib/addrinfo.o obj/lib/socket.o obj/lib/pushsocket.o obj/servidorUDP.o
	${cc} ${lflags} ${llibs} $+ -o $@

udp: clienteUDP servidorUDP


all: udp tcp


clean:
	rm -r ${objdir}
	rm clienteTCP servidorTCP clienteTCP servidorTCP
