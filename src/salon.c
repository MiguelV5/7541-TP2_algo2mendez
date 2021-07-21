#include <stdio.h>
#include "salon.h"
#include "abb.h"
#include "util.h"

const char SEPARADOR_EN_LECTURA = ';';
#define CANTIDAD_CAMPOS_ENTRENADOR 2
#define CANTIDAD_CAMPOS_POKEMON 6

#define EXITO 0
#define FALLO -1

struct _salon_t{
    abb_t* abb_entrenadores;
};





/**
 * Actualiza un entrenador y guarda el entrenador previo al salón
 * Devuelve la dirección al entrenador actualizado si pudo realizarlo o NULL en caso contrario.
 * -El salón debe llegar previamente reservado en memoria.
 * -valores_a_asignar debe llegar con valores previamente leidos.
*/
entrenador_t* almacenador_entrenadores(entrenador_t* entrenador_actual, entrenador_t* entrenador_previo, bool* es_primer_entrenador, char** valores_a_asignar, salon_t* salon){
    
    entrenador_previo = entrenador_actual;
    int resultado_almacenar = EXITO;

    entrenador_actual = entrenador_crear(valores_a_asignar);
    if(entrenador_actual == NULL){
        return NULL;
    }

    if((*es_primer_entrenador) == false){
        resultado_almacenar = arbol_insertar(salon->abb_entrenadores, entrenador_previo);  
    }
    else if((*es_primer_entrenador) == true){
        (*es_primer_entrenador) = false;
    }

    if(resultado_almacenar==EXITO){
        return entrenador_actual;
    }
    else{
        return NULL;
    }

}



/**
 * Carga en memoria un archivo de salón (ya abierto).
 * Devuelve el salón cargado.
 * -Si no puede leer el archivo o hay un error, devuelve NULL.
 * -El cierre del archivo queda en disposicion de la función de donde se le llama.
 */
salon_t* almacenador_salon(char* linea_leida, FILE* archivo_sal, salon_t* salon){

    salon->abb_entrenadores = arbol_crear(entrenador_comparar_nombres, entrenador_destruir);
    if(!salon->abb_entrenadores){
        return NULL;
    }

    entrenador_t* entrenador_aux = NULL;
    entrenador_t* entrenador_previo = NULL; //Cuando se cambie a un entrenador nuevo, con este se guardará el entrenador previo al salón.
    int resultado_de_inserciones = FALLO;

    bool es_primer_entrenador = true;

    char** campos_leidos_separados = NULL;
    size_t cantidad_campos_separados = 0;

    while(linea_leida != NULL){
    
        campos_leidos_separados = split(linea_leida, SEPARADOR_EN_LECTURA);
        if(campos_leidos_separados == NULL){
            return NULL;
        }
        free(linea_leida);

        cantidad_campos_separados = vtrlen(campos_leidos_separados);

        if(cantidad_campos_separados == CANTIDAD_CAMPOS_ENTRENADOR){

            if(es_primer_entrenador==false && (entrenador_tamanio_equipo(entrenador_previo) == 0)){
                vtrfree(campos_leidos_separados);
                salon_destruir(salon);
                return NULL;
            }

            entrenador_aux = almacenador_entrenadores(entrenador_aux, entrenador_previo, &es_primer_entrenador, campos_leidos_separados, salon);
            if(entrenador_aux == NULL){
                vtrfree(campos_leidos_separados);
                salon_destruir(salon);
                return NULL;
            }
            
        }
        else if(cantidad_campos_separados == CANTIDAD_CAMPOS_POKEMON ){

            resultado_de_inserciones = entrenador_agregar_pokemon(entrenador_aux, campos_leidos_separados);
            if(resultado_de_inserciones == FALLO){
                vtrfree(campos_leidos_separados);
                salon_destruir(salon);
                return NULL;
            }

        }

        linea_leida = fgets_alloc(archivo_sal);

        if(linea_leida == NULL){
            resultado_de_inserciones = arbol_insertar(salon->abb_entrenadores, entrenador_aux);
            if(resultado_de_inserciones == FALLO){
                vtrfree(campos_leidos_separados);
                salon_destruir(salon);
                return NULL;
            }
        }

        vtrfree(campos_leidos_separados);
    
    }

    return salon;

}



salon_t* salon_leer_archivo(const char* nombre_archivo){

    if(!nombre_archivo){
        return NULL;
    }

    FILE* archivo_salon = fopen(nombre_archivo, "r");
    if(archivo_salon == NULL){
        return NULL;
    }
    
    salon_t* salon = malloc(1*sizeof(salon_t));
    if(!salon){
        fclosen(archivo_salon);
        return NULL;
    }

    char* linea_leida = fgets_alloc(archivo_salon);
    if(linea_leida == NULL){
        free(salon);
        fclosen(archivo_salon);
        return NULL;
    }
   
    salon = almacenador_salon(linea_leida, archivo_salon, salon);
    if(!salon){
        fclosen(archivo_salon);
        return NULL;
    }

    fclosen(archivo_salon);

    return salon; 

}




int salon_guardar_archivo(salon_t* salon, const char* nombre_archivo){

    return -1;

}




salon_t* salon_agregar_entrenador(salon_t* salon, entrenador_t* entrenador){

    return NULL;

}





lista_t* salon_filtrar_entrenadores(salon_t* salon , bool (*f)(entrenador_t*, void*), void* extra){

    return NULL;

}




char* salon_ejecutar_comando(salon_t* salon, const char* comando){

    return NULL;

}




void salon_destruir(salon_t* salon){

    if(!salon){
        return;
    }

    arbol_destruir(salon->abb_entrenadores);
    free(salon);

}


