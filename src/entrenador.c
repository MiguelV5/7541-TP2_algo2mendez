#include "entrenador.h"
#include <string.h>

#define FALLO -1
#define EXITO 0


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

    pokemon_t* _pokemon = pokemon;
    free(_pokemon->nombre);
    free(_pokemon);

}




/**
 * Le asigna los datos recibidos a los campos de un entrenador (EXCEPTO el equipo) previamente reservado en memoria
*/
void asignar_datos_entrenador(entrenador_t* entrenador, char** datos_entrenador){
    
    strcpy(entrenador->nombre, datos_entrenador[0]);
    entrenador->victorias = atoi(datos_entrenador[1]);
    entrenador->pokemones_agotados = false;

}


entrenador_t* entrenador_crear(char** datos_entrenador){

    if(!datos_entrenador){
        return NULL;
    }

    entrenador_t* entrenador = malloc(1*sizeof(entrenador_t));
    if(!entrenador){
        return NULL;
    }

    size_t longitud_nombre = strlen(datos_entrenador[0]);
    entrenador->nombre = malloc((longitud_nombre+1)*sizeof(char));
    if(!entrenador->nombre){
        free(entrenador);
        return NULL;
    }

    asignar_datos_entrenador(entrenador, datos_entrenador);

    entrenador->equipo = lista_crear(destructor_de_pokemon);
    if(!entrenador->equipo){
        free(entrenador);
        return NULL;
    }

    return entrenador;

}




/**
 * Le asigna los datos pokemon a los campos de un pokemon previamente reservado en memoria.
*/
void asignar_datos_pokemon(pokemon_t* pokemon, char** datos_pokemon){
    
    strcpy(pokemon->nombre, datos_pokemon[0]);
    pokemon->nivel = atoi(datos_pokemon[1]);
    pokemon->defensa = atoi(datos_pokemon[2]);
    pokemon->fuerza = atoi(datos_pokemon[3]);
    pokemon->inteligencia = atoi(datos_pokemon[4]);
    pokemon->velocidad = atoi(datos_pokemon[5]);

}



int entrenador_agregar_pokemon(entrenador_t* entrenador, char** datos_pokemon){

    if(!entrenador || !datos_pokemon){
        return FALLO;
    }

    pokemon_t* poke_auxiliar = calloc(1, sizeof(pokemon_t));
    if(!poke_auxiliar){
        return FALLO;
    }
    
    size_t longitud_nombre = strlen(datos_pokemon[0]);
    poke_auxiliar->nombre = malloc((longitud_nombre+1)*sizeof(char));
    if(!poke_auxiliar->nombre){
        free(poke_auxiliar);
        return FALLO;
    }

    asignar_datos_pokemon(poke_auxiliar, datos_pokemon);
    int resultado_agregar = lista_insertar(entrenador->equipo, poke_auxiliar);

    return resultado_agregar;

}



char** entrenador_comparar(entrenador_t* entrenador_1, entrenador_t* entrenador_2 , char* regla_de_batalla){

    return NULL;

}




entrenador_t* entrenador_quitar_pokemon(entrenador_t* entrenador, char* nombre_pokemon){

    return NULL;

}





size_t entrenador_tamanio_equipo(entrenador_t* entrenador){

    if(!entrenador){
        return 0;
    }

    return lista_elementos(entrenador->equipo);

}





//size_t entrenador_cantidad_victorias(entrenador_t* entrenador){

    //return 0;

//}




/**
*/
//entrenador_t* entrenador_(){

    //return ;

//}





void entrenador_destruir(entrenador_t* entrenador){

    free(entrenador->nombre);
    lista_destruir(entrenador->equipo);
    free(entrenador);

}