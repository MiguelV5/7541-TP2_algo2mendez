CFLAGS=-g -O0 -std=c99 -Wall -Wconversion -Wtype-limits -Werror -I src
CFLAGS_SERVER=-g -O2 -Wall -Wconversion -Wtype-limits -I src
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2

ARCHIVO_SALON=salones/salon_estandar.txt
PUERTO_SERVER=8080

valgrind: pruebas
	valgrind $(VFLAGS) ./pruebas 2>&1

valgrind-server: server
	valgrind $(VFLAGS) ./server  $(ARCHIVO_SALON) $(PUERTO_SERVER) 2>&1

pruebas: pruebas.c src/*
	gcc $(CFLAGS) -o pruebas pruebas.c src/*.c 2>&1

server: server.c src/*
	gcc $(CFLAGS_SERVER) -o server server.c src/*.c 2>&1

clean:
	rm pruebas server 2>&1
