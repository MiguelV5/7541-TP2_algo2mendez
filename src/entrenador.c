#include "entrenador.h"
#include <string.h>
#include <stdio.h>

#define FALLO -1
#define EXITO 0

#define SEPARADOR_CONCATENACION ","
#define FORMATO_ESCRITURA_ENTRENADOR "%s;%i\n"
#define FORMATO_ESCRITURA_POKEMON "%s;%i;%i;%i;%i;%i\n"

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



bool entrenador_equipo_vacio(void* entrenador, void* hubo_equipo_vacio){

    entrenador_t* _entrenador = entrenador;
    bool* _hubo_equipo_vacio = hubo_equipo_vacio;
    size_t tamanio_equipo = entrenador_tamanio_equipo(_entrenador);
    if(tamanio_equipo==0){
        (*_hubo_equipo_vacio) = true;
        return true;
    }
    else{
        return false;
    }

}




bool entrenador_verificar_identicos(entrenador_t* entrenador_1, entrenador_t* entrenador_2){

    return (strcmp(entrenador_1->nombre, entrenador_2->nombre) == 0);

}



/**
 * Escribe todos los campos de un pokemon en un archivo recibido
 * (previamente abierto).
 * Siempre devuelve false.
*/
bool pokemon_escribir_en_archivo(void* pokemon, void* archivo_a_escribir){

    FILE* archivo = archivo_a_escribir;
    pokemon_t* _pokemon = pokemon;
    
    fprintf(archivo, FORMATO_ESCRITURA_POKEMON, _pokemon->nombre, _pokemon->nivel, _pokemon->defensa, _pokemon->fuerza, _pokemon->inteligencia, _pokemon->velocidad);

    return false;

}


bool entrenador_escribir_en_archivo(void* entrenador, void* archivo_a_escribir){

    FILE* archivo = archivo_a_escribir;
    entrenador_t* _entrenador = entrenador;

    fprintf(archivo, FORMATO_ESCRITURA_ENTRENADOR, _entrenador->nombre, _entrenador->victorias);
    lista_con_cada_elemento(_entrenador->equipo, pokemon_escribir_en_archivo, archivo);

    return false;

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



int entrenador_agregar_pokemon_leido(entrenador_t* entrenador, char** datos_pokemon){

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



int entrenador_comparar_nombres(void* entrenador_1, void* entrenador_2){
    
    entrenador_t* _entrenador_1 = entrenador_1;
    entrenador_t* _entrenador_2 = entrenador_2; 

    int comparacion_nombres = strcmp(_entrenador_1->nombre, _entrenador_2->nombre);

    return comparacion_nombres;

}




char** entrenador_enfrentar(entrenador_t* entrenador_1, entrenador_t* entrenador_2 , char* regla_de_batalla){

    return NULL;

}




entrenador_t* entrenador_quitar_pokemon(entrenador_t* entrenador, char* nombre_pokemon){

    return NULL;

}



char* entrenador_obtener_nombre(entrenador_t* entrenador){

    if(!entrenador){
        return NULL;
    }

    char* nombre_duplicado = malloc((strlen(entrenador->nombre) + 1)*sizeof(char));
    if(!nombre_duplicado){
        return NULL;
    }
    strcpy(nombre_duplicado, entrenador->nombre);

    return nombre_duplicado;

}



size_t entrenador_tamanio_equipo(entrenador_t* entrenador){

    if(!entrenador){
        return 0;
    }

    return lista_elementos(entrenador->equipo);

}





int entrenador_cantidad_victorias(entrenador_t* entrenador){

    if(!entrenador){
        return FALLO;
    }

    return entrenador->victorias;

}


bool entrenador_tiene_victorias_minimas(entrenador_t* entrenador, void* victorias_minimas){

    if(!entrenador || !victorias_minimas){
        return false;
    }

    int* victorias_min = victorias_minimas;

    return (entrenador->victorias >= (*victorias_min));

}


bool entrenador_tiene_pokemon(entrenador_t* entrenador, void* nombre_pokemon){

    if(!entrenador || !nombre_pokemon){
        return false;
    }

    char* _nombre_pokemon = nombre_pokemon;
    bool pokemon_fue_encontrado = false;

    lista_iterador_t* iter_pokemones = lista_iterador_crear(entrenador->equipo);
    while(lista_iterador_tiene_siguiente(iter_pokemones) && (pokemon_fue_encontrado==false)){

        pokemon_t* pokemon_actual = lista_iterador_elemento_actual(iter_pokemones);

        if(strcmp(pokemon_actual->nombre , _nombre_pokemon) == 0){
            pokemon_fue_encontrado = true;
        }

        lista_iterador_avanzar(iter_pokemones);

    }
    lista_iterador_destruir(iter_pokemones);

    return pokemon_fue_encontrado;

}



char* entrenador_obtener_info_concatenada(entrenador_t* entrenador){

    size_t tamanio_nombre = strlen(entrenador->nombre); 
    int tamanio_requerido_para_conversion = snprintf(NULL, 0 , "%d", entrenador->victorias);
    
    size_t tamanio_total_reserva = tamanio_nombre + (size_t)tamanio_requerido_para_conversion + 2 ; //2 adicionales para la ',' y para el '\0'

    char* info_concatenada = malloc(tamanio_total_reserva*sizeof(char));
    if(!info_concatenada){
        return NULL;
    }
    
    char victorias_en_string[tamanio_requerido_para_conversion];
    snprintf(victorias_en_string, (size_t)tamanio_requerido_para_conversion + 1, "%d", entrenador->victorias);

    strcpy(info_concatenada, entrenador->nombre);
    strcat(info_concatenada, SEPARADOR_CONCATENACION);
    strcat(info_concatenada, victorias_en_string);

    return info_concatenada;

}





void entrenador_destruir(void* entrenador){

    entrenador_t* _entrenador = entrenador;
    free(_entrenador->nombre);
    lista_destruir(_entrenador->equipo);
    free(_entrenador);

}