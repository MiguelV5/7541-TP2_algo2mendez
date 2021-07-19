#include "entrenador.h"

struct _pokemon_t{
    char* nombre;
    int nivel;
    int defensa;
    int fuerza;
    int inteligencia;
    int velocidad;
};

struct _entrenador_t{
    char* nombre;
    int victorias;
    lista_t* equipo;
    bool pokemones_agotados;
};




/**
 * Libera el puntero a pokemon recibido.
*/
void destructor_de_pokemon(void* pokemon){

    free(pokemon);

}



entrenador_t* entrenador_crear(char* nombre, int cantidad_victorias){

    if(!nombre){
        return NULL;
    }

    entrenador_t* entrenador = malloc(1*sizeof(entrenador_t));
    if(!entrenador){
        return NULL;
    }

    entrenador->nombre = nombre;
    entrenador->victorias = cantidad_victorias;
    entrenador->pokemones_agotados = false;

    entrenador->equipo = lista_crear(destructor_de_pokemon);
    if(!entrenador->equipo){
        free(entrenador);
        return NULL;
    }

    return entrenador;

}





int entrenador_agregar_pokemon(entrenador_t* entrenador, char** datos_pokemon){

    return -1;

}





entrenador_t* entrenador_quitar_pokemon(entrenador_t* entrenador, char* nombre_pokemon){

    return NULL;

}





size_t entrenador_tamanio_equipo(entrenador_t* entrenador){

    return 0;

}





size_t entrenador_cantidad_victorias(entrenador_t* entrenador){

    return 0;

}




/**
*/
//entrenador_t* entrenador_(){

    //return ;

//}





void entrenador_destruir(entrenador_t* entrenador){

    lista_destruir(entrenador->equipo);
    free(entrenador);

}