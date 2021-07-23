#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <sys/types.h>

#include "src/salon.h"

#define ROJO      "\033[91m"
#define VERDE     "\033[92m"
#define AMARILLO  "\033[93m"
#define RESET     "\033[0m"

#define ERROR_INICIO_SERVER "Error al iniciar el servidor"

#define SCRIPT_PRINCIPAL "https://a2m202101tp2.netlify.app/app.js"

#define HTTP_200 "HTTP/1.1 200 OK\r\n\r\n"

const char* string_pagina_principal =
    HTTP_200
    "<!DOCTYPE html><html><head><meta charset='utf-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'/><title>Cargando...</title></head>"
    "<body><script src='" SCRIPT_PRINCIPAL  "'></script></body></html>";

const char* string_404 = "HTTP/1.1 404 Not found\r\n";

// Eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
volatile int SEGUIR_CORRIENDO = 1;
static void ctrl_c(int sig, siginfo_t *si, void *unused){
    SEGUIR_CORRIENDO = 0;
}


typedef int Server;
typedef int Cliente;

Server server_crear(char *puerto){
    Server server = -1;
    int option = 1;
    struct sockaddr_in sin;

    signal(SIGCHLD,SIG_IGN);

    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server<0){
        perror(ROJO ERROR_INICIO_SERVER RESET);
        return -1;
    }
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons((uint16_t)atol(puerto));
    if(bind(server, (struct sockaddr *) &sin, sizeof sin) != 0){
        perror(ROJO ERROR_INICIO_SERVER RESET);
        return -1;
    }


    if ( listen (server, 1) != 0 ){
        perror(ROJO ERROR_INICIO_SERVER RESET);
        return -1;
    }

    printf("Servidor iniciado en" VERDE " http://127.0.0.1:%s" RESET "\n", puerto);

    return server;
}

void server_destruir(Server server){
    if(server >= 0)
        close(server);
}

Cliente cliente_esperar(Server server){
    struct sockaddr_in clientaddr;
    socklen_t addrlen;

    addrlen = sizeof(clientaddr);
    int cliente = accept (server, (struct sockaddr *) &clientaddr, &addrlen);
    return cliente;
}

void cliente_cerrar(Cliente cliente){
    if(cliente >= 0)
        close(cliente);
}


void cliente_responder(Cliente cliente, int (*f)(Cliente, char*, char*, void*), void* extra){
    char buffer[65536];
    long int rcvd;

    rcvd=recv(cliente, buffer, 65535, 0);

    if (rcvd<=0)
        printf(AMARILLO "Conexión cerrada inesperadamente" RESET "\n");
    else{
        buffer[rcvd] = 0;

        char* method = strtok(buffer," \t\r\n");
        char* uri    = strtok(NULL,  " \t");

        if(f && f(cliente, method, uri, extra)){
            fprintf(stderr, VERDE);
        }
        else{
            write(cliente, string_404, strlen(string_404));
            fprintf(stderr, ROJO);
        }

        printf("[%s] %s" RESET "\n", method, uri);
    }
}

int responder_salon(Cliente cliente, char* metodo, char* uri, void* salon){
    if(strcmp(uri, "/")==0){
        write(cliente, string_pagina_principal, strlen(string_pagina_principal));
        return 1;
    }

    char* resultado = salon_ejecutar_comando(salon, uri+1);

    if(!resultado)
        return 0;

    write(cliente, HTTP_200, strlen(HTTP_200));
    write(cliente, resultado, strlen(resultado));
    free(resultado);

    return 1;
}

int main(int argc, char *argv[]){

    //CTRL-C para salir del programa
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = ctrl_c;
    sigaction(SIGINT, &sa, NULL);

    if(argc<3){
        printf("%s archivo puerto\n", argv[0]);
        return -1;
    }

    char* nombre_archivo = argv[1];
    char* puerto = argv[2];

    salon_t* salon = salon_leer_archivo(nombre_archivo);
    if(!salon){
        printf("Error al leer el salon del archivo %s\n", nombre_archivo);
        return -1;
    }

    Server server = server_crear(puerto);
    if(server > 0){
        while(SEGUIR_CORRIENDO){
            Cliente cliente = cliente_esperar(server);
            cliente_responder(cliente, responder_salon, salon);
            cliente_cerrar(cliente);
        }
    }

    salon_destruir(salon);

    return 0;
}
