#include "util.h"
#include <string.h>
#include <stdbool.h>

const size_t AUMENTO_VECTOR = 2;
const size_t TAM_DEFECTO = 512; //Representa un tamaño en bytes



/**
 * Copia letra por letra en cierto itervalo especifico del str_origin al nuevo_str.
 * Dicho intervalo viene determinado por pos_inicio y pos_fin.
*/
void transcribir_intervalo_str(const char* str_origin, size_t longitud_origin, int pos_inicio, int pos_fin, char* nuevo_str){
    
    if((!str_origin) || (!nuevo_str)){
        return;
    }

    int i = pos_inicio;
    int j = 0;

    while((i < pos_fin) && (i < longitud_origin) && (str_origin[i] != '\n')){

        nuevo_str[j] = str_origin[i];
        i++;
        j++;

    }
    
}



/**
 * Dado un vector de punteros (funalizado en NULL), devuelve la cantidad de
 * punteros en el vector (sin contar el nulo al final).
 */
size_t vtrlen(void* ptr){
    
    if(!ptr){
        return 0;
    }

    size_t contador_tamanio = 0;

    void** ptr_aux = ptr;

    while(( ptr_aux[contador_tamanio] ) != NULL){

        contador_tamanio++;

    }
    
    return contador_tamanio;
}



/**
 * Agrega un item a un vector de punteros dinámico.
 *
 * Si ptr es NULL, se crea un nuevo vector.
 * Devuelve un puntero al nuevo vector de punteros.
 */
void* vtradd(void* ptr, void* item){

    void** ptr_aux = ptr;

    size_t tamanio_vector = vtrlen(ptr_aux);

    ptr_aux = realloc(ptr_aux, (tamanio_vector + AUMENTO_VECTOR)*(sizeof(void*)));
    
    if(ptr_aux == NULL){
        return NULL;
    }

    tamanio_vector++;

    ptr_aux[tamanio_vector-1] = item;

    ptr_aux[tamanio_vector] = NULL;

    return ptr_aux;

}



/**
 * Aplica la función free al vector de punteros y a todos los punteros
 * contenidos en el mismo.
 */
void vtrfree(void* ptr){
    
    if(ptr == NULL){
        return;
    }

    void** ptr_aux = ptr;

    size_t longitud = vtrlen(ptr_aux);

    for(int i = 0; i < longitud; i++){

        if(ptr_aux[i] != NULL){

            free(ptr_aux[i]);

        }

    }

    free(ptr_aux);

}



/**
 * Divide un string cada vez que encuentra el separador dado y devuelve un
 * vector de strings.
 */
char** split(const char* str, char separador){

    if(!str){
        return NULL;
    }
    
    int pos_ult_inicial = 0;
    int pos_ult_separador = 0;  // pos_ult_(...) == posicion ultimo (...)
    
    size_t longitud_origin = strlen(str);
    if(longitud_origin == 0){
        return NULL;
    }

    char* nuevo_sub_str = NULL;
    char** str_divididos = NULL;
    bool sub_str_es_null = false;
    size_t size_sub_str = 0; //tamaño del intervalo a reservar para un substring

    int i = 0;

    while((i <= longitud_origin) && !(sub_str_es_null)){
        
        if((str[i] == separador) || (i == longitud_origin)){

            pos_ult_separador = i;
            size_sub_str = (size_t)((pos_ult_separador - pos_ult_inicial) + 1);
            
            nuevo_sub_str = calloc(size_sub_str, sizeof(char));
            
            if(!nuevo_sub_str){

                sub_str_es_null = true;

            }
            else{

                transcribir_intervalo_str(str, longitud_origin, pos_ult_inicial, pos_ult_separador, nuevo_sub_str);

                str_divididos = vtradd(str_divididos, nuevo_sub_str);

                pos_ult_inicial = (pos_ult_separador + 1);

            }

        }
        i++;

    }

    if(sub_str_es_null && (str_divididos != NULL)){

        vtrfree(str_divididos);
        return NULL;

    }

    return str_divididos;

}




/**
 * Lee una linea completa de un archivo y devuelve un puntero al string leido.
 *
 * El string devuelto debe ser liberado con malloc.
 */
char* fgets_alloc(FILE* archivo){
    
    if(archivo == NULL){
        return NULL;
    }

    char* str = NULL;
    str = malloc(TAM_DEFECTO*sizeof(char));    
    if(!str){
        return NULL;
    }

    char* str_aux = NULL;
    str_aux = malloc(TAM_DEFECTO*sizeof(char));
    if(!str_aux){
        free(str);
        return NULL;
    }

    char* lector_verif = fgets(str, (int)TAM_DEFECTO, archivo);
    if(lector_verif == NULL){
        free(str);
        free(str_aux);
        return NULL;
    }
    
    size_t longitud = 0;
    size_t cantidad_veces_reserva = 1; 

    if(lector_verif != NULL){
        longitud = strlen(str);
    }

    bool str_null = false;

    while( (longitud == ((cantidad_veces_reserva*TAM_DEFECTO) - cantidad_veces_reserva)) && (!str_null)){
        
        str = realloc(str, (cantidad_veces_reserva+1)*TAM_DEFECTO*sizeof(char));
        if(!str){
            str_null = true;
        }

        lector_verif = fgets(str_aux, (int)(TAM_DEFECTO), archivo);
        if((str_aux != NULL) && (!str_null)){
            strcat(str, str_aux);
            longitud = strlen(str);
        }

        cantidad_veces_reserva++;

    }

    if(lector_verif != NULL){
        free(str_aux);
    }

    return str;
    
}


/**
 * Si el archivo no es nulo, lo cierra con fclose.
 */
void fclosen(FILE* archivo){
    
    if(archivo != NULL){
        fclose(archivo);
    }

}
