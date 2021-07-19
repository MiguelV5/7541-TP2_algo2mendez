#ifndef ENTRENADOR_H_
#define ENTRENADOR_H_

#include <stdlib.h>
#include "lista.h"

typedef struct _pokemon_t pokemon_t;

typedef struct _entrenador_t entrenador_t;


/**
 * Reserva en memoria un entrenador con nombre y cantidad de victorias recibidas.
 * El entrenador creado tiene inicialmente como equipo una lista vacía de pokemones.
 * Devuelve un puntero al entrenador creado o NULL en caso de error.
*/
entrenador_t* entrenador_crear(char* nombre, int cantidad_victorias);


/**
 * Añade el pokemon con los datos dados al equipo del entrenador recibido.
 * Los datos vienen separados en strings (leidos de un archivo) en un vector
 * de punteros a los mismos.
 * Devuelve 0 si pudo o -1 en caso de error.
*/
int entrenador_agregar_pokemon(entrenador_t* entrenador, char** datos_pokemon);


/**
 * Remueve el pokemon con el nombre recibido del equipo del entrenador.
 * Devuelve un puntero al entrenador con su equipo actualizado o NULL en caso de error.
*/
entrenador_t* entrenador_quitar_pokemon(entrenador_t* entrenador, char* nombre_pokemon);


/**
 * Devuelve la cantidad de pokemones que posee el equipo del entrenador dado.
 * Devuelve 0 en caso de error.
*/
size_t entrenador_tamanio_equipo(entrenador_t* entrenador);


/**
 * Devuelve la cantidad de victorias que posee actualmente el entrenador dado.
 * Devuelve 0 en caso de error.
*/
size_t entrenador_cantidad_victorias(entrenador_t* entrenador);


/**
*/
//entrenador_t* entrenador_();

/**
 * Destruye al entrenador recibido y a su equipo pokemon.
*/
void entrenador_destruir(entrenador_t* entrenador);



#endif // ENTRENADOR_H_
