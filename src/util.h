#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>

/**
 * Dado un vector de punteros (funalizado en NULL), devuelve la cantidad de
 * punteros en el vector (sin contar el nulo al final).
 */
size_t vtrlen(void* ptr);

/**
 * Agrega un item a un vector de punteros dinámico.
 *
 * Si ptr es NULL, se crea un nuevo vector.
 * Devuelve un puntero al nuevo vector de punteros.
 */
void* vtradd(void* ptr, void* item);

/**
 * Aplica la función free al vector de punteros y a todos los punteros
 * contenidos en el mismo.
 */
void vtrfree(void* ptr);

/**
 * Divide un string cada vez que encuentra el separador dado y devuelve un
 * vector de strings.
 */
char** split(const char* str, char separador);

/**
 * Lee una linea completa de un archivo y devuelve un puntero al string leido.
 *
 * El string devuelto debe ser liberado con malloc.
 */
char* fgets_alloc(FILE* archivo);

/**
 * Si el archivo no es nulo, lo cierra con fclose.
 */
void fclosen(FILE* archivo);

#endif /* UTIL_H */
