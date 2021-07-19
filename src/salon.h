#ifndef SALON_H
#define SALON_H

#include "lista.h"
#include "entrenador.h"

typedef struct _salon_t salon_t;

/**
 *  Lee un archivo de salón y lo carga en memoria. Devuelve el salón creado o NULL en caso de error.
 */
salon_t* salon_leer_archivo(const char* nombre_archivo);

/**
 * Guarda el salón a un archivo (reescribiendo el archivo si existe).
 * Devuelve la cantidad de entrenadores guardados o -1 en caso de error.
 */
int salon_guardar_archivo(salon_t* salon, const char* nombre_archivo);

/*
 * Agrega un entrenador al salón. Devuelve el salón o NULL en caso de error.
 */
salon_t* salon_agregar_entrenador(salon_t* salon, entrenador_t* entrenador);

/*
 * Obtiene una lista (a liberar por el usuario) con los entrenadores que cumplen con la función de filtro provista.
 * En caso de error devuelve NULL.
 */
lista_t* salon_filtrar_entrenadores(salon_t* salon , bool (*f)(entrenador_t*, void*), void* extra);

/*
 * Recibe un string con un comando a ejecutar. 
 * Ver comandos.txt para el listado y formato de los comandos.
 * Devuelve un string a liberar por el usuario con free o NULL en caso de error.
 */
char* salon_ejecutar_comando(salon_t* salon, const char* comando);

/*
 * Destruye un salón liberando toda la memoria asociada.
 */
void salon_destruir(salon_t* salon);

#endif /* SALON_H */