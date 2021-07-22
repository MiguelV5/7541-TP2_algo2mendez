#include "comando.h"
#include <stddef.h>

struct comando{
    char* nombre;
    ejecutar_comando_t ejecutor;
};


comando_t* comando_crear(char* nombre_comando, ejecutar_comando_t ejecutor){

    return NULL;

}



void comando_destruir(void* comando){

    return;

}