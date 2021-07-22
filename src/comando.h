#ifndef COMANDO_H_
#define COMANDO_H_



typedef struct comando comando_t;

typedef char* (*ejecutar_comando_t)(char**);

/**
 * Crea un comando a partir de:
 *  - El string con el que se desea identificar el mismo al momento de
 *    recibir un input a ser interpretado
 *    (Es decir, SOLO el nombre del comando, sin sus
 *    argumentos/subcomandos).
 *  - La función encargada de procesar el comportamiento que se quiere
 *    tener con el comando a crear.
 * Devuelve un puntero al comando creado o NULL en caso de error.
*/
comando_t* comando_crear(char* nombre_comando, ejecutar_comando_t ejecutor);


/**
 * Destruye el comando recibido (siendo comando un comando_t*. Se recibe
 * como puntero genérico para su uso como destructor en hash).
*/
void comando_destruir(void* comando);



#endif // COMANDO_H_