#ifndef ENTRENADOR_H_
#define ENTRENADOR_H_

#include <stdlib.h>
#include "lista.h"

typedef struct _pokemon_t pokemon_t;

typedef struct _entrenador_t entrenador_t;

/**
 * Reserva en memoria un entrenador con los datos recibidos.
 * Los datos vienen separados en strings en un vector
 * de punteros a los mismos.
 * SE REQUIERE que los strings vengan en el siguiente orden en el vector:
 * {"Nombre" , "Victorias"}
 * El entrenador creado tiene inicialmente como equipo una lista vacía de pokemones.
 * Devuelve un puntero al entrenador creado o NULL en caso de error.
*/
entrenador_t* entrenador_crear(char** datos_entrenador);


/**
 * Devuelve true si el entrenador recibido tiene 0 pokemones almacenados en
 * su equipo.
 * Devuelve false si tiene al menos un pokemon.
 * (Leer README.txt , sección Aclaraciones).
*/
bool entrenador_equipo_vacio(void* entrenador, void* hubo_equipo_vacio);


/**
 * Devuelve true si los entrenadores recibidos tienen el mismo nombre,
 * false en caso contrario.
*/
bool entrenador_verificar_identicos(entrenador_t* entrenador_1, entrenador_t* entrenador_2);


/**
 * Escribe todos los campos de un entrenador, y posteriormente
 * todos los de su equipo pokemon en un archivo recibido
 * (previamente abierto).
 * Siempre devuelve false.
 * (Leer README.txt, sección Aclaraciones).
*/
bool entrenador_escribir_en_archivo(void* entrenador, void* archivo_a_escribir);


/**
 * Añade el pokemon con los datos dados al equipo del entrenador recibido.
 * Los datos vienen separados en strings en un vector
 * de punteros a los mismos.
 * SE REQUIERE que los strings vengan en el siguiente orden en el vector:
 * {"Nombre" , "Nivel" , "Defensa" , "Fuerza" , "Inteligencia" , "Velocidad"} 
 * Devuelve 0 si pudo agregarlo o -1 en caso de error.
*/
int entrenador_agregar_pokemon_leido(entrenador_t* entrenador, char** datos_pokemon);


/**
 * Remueve el pokemon con el nombre recibido del equipo del entrenador.
 * Devuelve 0 si pudo quitarlo o -1 en caso de error o de que el 
 * entrenador solo tenía ese pokemon restante en su equipo.
*/
int entrenador_quitar_pokemon(entrenador_t* entrenador, char* nombre_pokemon);


/**
 * Compara los nombres de dos entrenadores entre sí (Recibe a los ENTRENADORES como punteros genéricos).
 * Devuelve:
 * - Un entero positivo si el nombre del entrenador_1 es MAYOR
 *   que el de entrenador_2 alfabéticamente.
 * - Un entero negativo si el nombre del entrenador_1 es MENOR
 *   que el de entrenador_2 alfabéticamente.
 * - 0 si son iguales.
*/
int entrenador_comparar_nombres(void* entrenador_1, void* entrenador_2);


/**
 * Compara los equipos de dos entrenadores pokemon por pokemon según
 * la regla de batalla recibida.
 * Devuelve un string con los resultados de cada enfrentamiento por
 * separado (separados con "\n").
 * (Debe recibir dos entrenadores no NULL y haberse verificado que existe
 * la regla de batalla anteriormente).
 * Devuelve NULL en caso de error.
*/
char* entrenador_enfrentar(entrenador_t* entrenador_1, entrenador_t* entrenador_2 , char* regla_de_batalla);


/**
 * Devuelve un string duplicado del nombre del entrenador recibido.
 * (Duplicado a liberar con free).
 * Devuelve NULL en caso de error.
*/
char* entrenador_obtener_nombre(entrenador_t* entrenador);

/**
 * Devuelve un string con toda la información del equipo del 
 * entrenador dado en el siguiente formato:
 * "Nombre,Nivel,Defensa,Fuerza,Inteligencia,Velocidad\nNombre2..."
 * (String devuelto a liberar con free).
 * Devuelve NULL en caso de error.
*/
char* entrenador_obtener_equipo_concatenado(entrenador_t* entrenador);


/**
 * Devuelve la cantidad de pokemones que posee el equipo del entrenador dado.
 * Devuelve 0 en caso de error.
*/
size_t entrenador_tamanio_equipo(entrenador_t* entrenador);


/**
 * Función de filtro para entrenadores.
 * Devuelve true si el entrenador recibido tiene victorias mayores o
 * iguales a las mínimas recibidas.
 * Devuelve false en caso contrario.
*/
bool entrenador_tiene_victorias_minimas(entrenador_t* entrenador, void* victorias_minimas);


/**
 * Función de filtro para entrenadores.
 * Devuelve true si el entrenador recibido tiene en su equipo al pokemon
 * de nombre dado.
 * Devuelve false en caso contrario.
*/
bool entrenador_tiene_pokemon(entrenador_t* entrenador, void* nombre_pokemon);


/**
 * Devuelve la cantidad de victorias que posee actualmente el entrenador dado.
 * Devuelve -1 en caso de error.
*/
int entrenador_cantidad_victorias(entrenador_t* entrenador);


/**
 * Reserva en memoria y devuelve un string con el nombre y las victorias
 * del entrenador recibido en el siguiente formato: "Nombre,victorias".
 * El string devuelto debe ser liberado posteriormente con free.
 * Devuelve NULL en caso de error.
*/
char* entrenador_obtener_info_concatenada(entrenador_t* entrenador);


/**
 * Destruye al entrenador recibido y a su equipo pokemon.
*/
void entrenador_destruir(void* entrenador);

#endif // ENTRENADOR_H_
