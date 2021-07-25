#include "entrenador.h"
#include <string.h>
#include <stdio.h>

#define FALLO -1
#define EXITO 0

#define SEPARADOR_CONCATENACION ","
#define SALTO_LINEA "\n"
#define FORMATO_ESCRITURA_ENTRENADOR "%s;%i\n"
#define FORMATO_ESCRITURA_POKEMON "%s;%i;%i;%i;%i;%i\n"

#define REGLA_CLASICA_NOMBRE_MAYUS "CLASICO"
#define REGLA_CLASICA_NOMBRE_MINUS "clasico"

#define REGLA_MODERNA_NOMBRE_MAYUS "MODERNO"
#define REGLA_MODERNA_NOMBRE_MINUS "moderno"

#define REGLA_RESISTENCIA_NOMBRE_MAYUS "RESISTENTE"
#define REGLA_RESISTENCIA_NOMBRE_MINUS "resistente"

#define REGLA_FISICA_NOMBRE_MAYUS "FISICO"
#define REGLA_FISICA_NOMBRE_MINUS "fisico"

#define REGLA_ELEGANCIA_NOMBRE_MAYUS "ELEGANTE"
#define REGLA_ELEGANCIA_NOMBRE_MINUS "elegante"



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



/**
 * Devuele true si la regla de batalla es CLASICA, false en caso contrario
*/
bool regla_es_clasica(char* regla_de_batalla){
    return (strcmp(regla_de_batalla, REGLA_CLASICA_NOMBRE_MAYUS)==0 || strcmp(regla_de_batalla, REGLA_CLASICA_NOMBRE_MINUS)==0);
}

/**
 * Devuele true si la regla de batalla es MODERNA, false en caso contrario
*/
bool regla_es_moderna(char* regla_de_batalla){
    return (strcmp(regla_de_batalla, REGLA_MODERNA_NOMBRE_MAYUS)==0 || strcmp(regla_de_batalla, REGLA_MODERNA_NOMBRE_MINUS)==0);
}

/**
 * Devuele true si la regla de batalla es RESISTENCIA, false en caso contrario
*/
bool regla_es_resistencia(char* regla_de_batalla){
    return (strcmp(regla_de_batalla, REGLA_RESISTENCIA_NOMBRE_MAYUS)==0 || strcmp(regla_de_batalla, REGLA_RESISTENCIA_NOMBRE_MINUS)==0);
}

/**
 * Devuele true si la regla de batalla es FISICA, false en caso contrario
*/
bool regla_es_fisica(char* regla_de_batalla){
    return (strcmp(regla_de_batalla, REGLA_FISICA_NOMBRE_MAYUS)==0 || strcmp(regla_de_batalla, REGLA_FISICA_NOMBRE_MINUS)==0);
}

/**
 * Devuele true si la regla de batalla es ELEGANCIA, false en caso contrario
*/
bool regla_es_elegancia(char* regla_de_batalla){
    return (strcmp(regla_de_batalla, REGLA_ELEGANCIA_NOMBRE_MAYUS)==0 || strcmp(regla_de_batalla, REGLA_ELEGANCIA_NOMBRE_MINUS)==0);
}


/**
 * Compara el calculo de coeficiente de batalla entre ambos pokemones.
 * Devuelve '1' si ganó el primero, '2' si ganó el segundo.
 * En caso de coeficientes coincidentes, gana el primero.
*/
char comparar_coeficiente_clasico(pokemon_t* pokemon_1, pokemon_t* pokemon_2){

    char resultado_batalla = '1';

    double coeficiente_poke_1 = 0.8*(double)(pokemon_1->nivel) + pokemon_1->fuerza + 2*(pokemon_1->velocidad);
    double coeficiente_poke_2 = 0.8*(double)(pokemon_2->nivel) + pokemon_2->fuerza + 2*(pokemon_2->velocidad);

    if(coeficiente_poke_1 >= coeficiente_poke_2){
        resultado_batalla = '1';
    }
    else{
        resultado_batalla = '2';
    }

    return resultado_batalla;

}

/**
 * Compara el calculo de coeficiente de batalla entre ambos pokemones.
 * Devuelve '1' si ganó el primero, '2' si ganó el segundo.
 * En caso de coeficientes coincidentes, gana el primero.
*/
char comparar_coeficiente_moderno(pokemon_t* pokemon_1, pokemon_t* pokemon_2){

    char resultado_batalla = '1';

    double coeficiente_poke_1 = 0.5*(double)(pokemon_1->nivel) + 0.9*(double)(pokemon_1->defensa) + 3*(pokemon_1->inteligencia);
    double coeficiente_poke_2 = 0.5*(double)(pokemon_2->nivel) + 0.9*(double)(pokemon_2->defensa) + 3*(pokemon_2->inteligencia);

    if(coeficiente_poke_1 >= coeficiente_poke_2){
        resultado_batalla = '1';
    }
    else{
        resultado_batalla = '2';
    }

    return resultado_batalla;

}

/**
 * Compara el calculo de coeficiente de batalla entre ambos pokemones.
 * Devuelve '1' si ganó el primero, '2' si ganó el segundo.
 * En caso de coeficientes coincidentes, gana el primero.
*/
char comparar_coeficiente_resistente(pokemon_t* pokemon_1, pokemon_t* pokemon_2){

    char resultado_batalla = '1';

    double coeficiente_poke_1 = (0.75*(double)(pokemon_1->nivel) + pokemon_1->defensa)/(double)(pokemon_1->velocidad);
    double coeficiente_poke_2 = (0.75*(double)(pokemon_2->nivel) + pokemon_2->defensa)/(double)(pokemon_2->velocidad);

    if(coeficiente_poke_1 >= coeficiente_poke_2){
        resultado_batalla = '1';
    }
    else{
        resultado_batalla = '2';
    }

    return resultado_batalla;

}

/**
 * Compara el calculo de coeficiente de batalla entre ambos pokemones.
 * Devuelve '1' si ganó el primero, '2' si ganó el segundo.
 * En caso de coeficientes coincidentes, gana el primero.
*/
char comparar_coeficiente_fisico(pokemon_t* pokemon_1, pokemon_t* pokemon_2){

    char resultado_batalla = '1';

    double coeficiente_poke_1 = (0.25*(double)(pokemon_1->nivel) + 0.8*(double)(pokemon_1->fuerza) - 0.1*(double)(pokemon_1->inteligencia))*(pokemon_1->velocidad);
    double coeficiente_poke_2 = (0.25*(double)(pokemon_2->nivel) + 0.8*(double)(pokemon_2->fuerza) - 0.1*(double)(pokemon_2->inteligencia))*(pokemon_2->velocidad);

    if(coeficiente_poke_1 >= coeficiente_poke_2){
        resultado_batalla = '1';
    }
    else{
        resultado_batalla = '2';
    }

    return resultado_batalla;

}

/**
 * Compara el calculo de coeficiente de batalla entre ambos pokemones.
 * Devuelve '1' si ganó el primero, '2' si ganó el segundo.
 * En caso de coeficientes coincidentes, gana el primero.
*/
char comparar_coeficiente_elegante(pokemon_t* pokemon_1, pokemon_t* pokemon_2){

    char resultado_batalla = '1';

    double coeficiente_poke_1 = (0.6*(double)(pokemon_1->nivel) + 0.7*(double)(pokemon_1->velocidad))*(double)(pokemon_1->inteligencia)/(double)(pokemon_1->defensa);
    double coeficiente_poke_2 = (0.6*(double)(pokemon_2->nivel) + 0.7*(double)(pokemon_2->velocidad))*(double)(pokemon_2->inteligencia)/(double)(pokemon_2->defensa);

    if(coeficiente_poke_1 >= coeficiente_poke_2){
        resultado_batalla = '1';
    }
    else{
        resultado_batalla = '2';
    }

    return resultado_batalla;

}

/**
 * Dados iteradores correspondientes a los equipos pokemon de dos entrenadores distintos y 
 * una función de comparación de coeficientes, simula las batallas entre dichos equipos
 * y devuelve un string (a liberar con free) con los resultados de todos los enfrentamientos
 * librados (o NULL en caso de error).
*/
char* procesador_de_enfrentamientos(lista_iterador_t* iter_1, lista_iterador_t* iter_2, char (*comparador_de_coeficiente)(pokemon_t*, pokemon_t*)){

    bool hubo_un_fallo = false;
    size_t cantidad_maxima_de_enfrentamientos = lista_elementos(iter_1->lista) + lista_elementos(iter_2->lista);
    char* resultados_finales = calloc(cantidad_maxima_de_enfrentamientos*2 + 1, sizeof(char));
    //cantidad*2 debido a que por cada enfrentamiento se va a concatenar un '\n' al string. +1 para asegurar el espacio del '\0' en el caso en el que se enfrenten todos los pokemon de ambos equipos.

    size_t i = 0;
    pokemon_t* poke_actual_equipo_1 = NULL;
    pokemon_t* poke_actual_equipo_2 = NULL;
    while(!hubo_un_fallo && lista_iterador_tiene_siguiente(iter_1) && lista_iterador_tiene_siguiente(iter_2)){

        poke_actual_equipo_1 = lista_iterador_elemento_actual(iter_1);
        poke_actual_equipo_2 = lista_iterador_elemento_actual(iter_2);

        if(comparador_de_coeficiente(poke_actual_equipo_1, poke_actual_equipo_2)=='1'){

            resultados_finales[i] = '1';
            resultados_finales[i+1] = '\n';
            lista_iterador_avanzar(iter_2);
            i++;

        }
        else{ // ganó '2'

            resultados_finales[i] = '2';
            resultados_finales[i+1] = '\n';
            lista_iterador_avanzar(iter_1);
            i++;

        }

        i++;
    }

    return resultados_finales;

}

char* entrenador_enfrentar(entrenador_t* entrenador_1, entrenador_t* entrenador_2 , char* regla_de_batalla){

    lista_t* equipo_1 = entrenador_1->equipo;
    lista_t* equipo_2 = entrenador_2->equipo;

    lista_iterador_t* iter_1 = lista_iterador_crear(equipo_1);
    if(!iter_1){
        return NULL;
    }
    lista_iterador_t* iter_2 = lista_iterador_crear(equipo_2);
    if(!iter_2){
        lista_iterador_destruir(iter_1);
        return NULL;
    }

    char* resultado_batallas = NULL;

    if(regla_es_clasica(regla_de_batalla)){
        resultado_batallas = procesador_de_enfrentamientos(iter_1, iter_2, comparar_coeficiente_clasico);
    }
    else if(regla_es_moderna(regla_de_batalla)){
        resultado_batallas = procesador_de_enfrentamientos(iter_1, iter_2, comparar_coeficiente_moderno);
    }
    else if(regla_es_resistencia(regla_de_batalla)){
        resultado_batallas = procesador_de_enfrentamientos(iter_1, iter_2, comparar_coeficiente_resistente);
    }
    else if(regla_es_fisica(regla_de_batalla)){
        resultado_batallas = procesador_de_enfrentamientos(iter_1, iter_2, comparar_coeficiente_fisico);
    }
    else if(regla_es_elegancia(regla_de_batalla)){
        resultado_batallas = procesador_de_enfrentamientos(iter_1, iter_2, comparar_coeficiente_elegante);
    }

    lista_iterador_destruir(iter_1);
    lista_iterador_destruir(iter_2);
    
    return resultado_batallas;

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


/**
 * Devuelve la cantidad necesaria de memoria que se requiere para obtener un string que concatene la
 * información de todos los pokemon en un equipo con formato csv.
*/
size_t calcular_tamanio_reserva_para_equipo(lista_t* equipo, char** strings_a_concatenar, size_t totalidad_strings_a_concatenar){

    size_t cantidad_pokemones = lista_elementos(equipo);

    size_t tamanio_total_reserva =  1 + cantidad_pokemones*6 ; // +1 para tomar en cuenta al '\0', +cantidad_pokemones*6 para tomar en cuenta a las 5 comas ',' que van a separar los campos y el '\n' por cada pokemon.

    for(size_t i = 0; i < totalidad_strings_a_concatenar; i++){
        tamanio_total_reserva += strlen(strings_a_concatenar[i]);
    }

    return tamanio_total_reserva;

}

/**
 * Libera todos los strings del vector de punteros.
*/
void liberar_strings_recopilados(char** vector_strings, size_t tamanio_vector){

    for(size_t i = 0; i < tamanio_vector; i++){
        free(vector_strings[i]);
    }

}

/**
 * Calcula la memoria requerida para hacer una reserva de strings que almacenen los campos de un pokemon.
*/
void calcular_memoria_por_campo_pokemon(size_t* memoria_requerida_por_campo, pokemon_t* pokemon){

    memoria_requerida_por_campo[0] = strlen(pokemon->nombre)+1;
    memoria_requerida_por_campo[1] = (size_t)snprintf(NULL, 0 , "%d", pokemon->nivel) + 1;
    memoria_requerida_por_campo[2] = (size_t)snprintf(NULL, 0 , "%d", pokemon->defensa) + 1;
    memoria_requerida_por_campo[3] = (size_t)snprintf(NULL, 0 , "%d", pokemon->fuerza) + 1;
    memoria_requerida_por_campo[4] = (size_t)snprintf(NULL, 0 , "%d", pokemon->inteligencia) + 1;
    memoria_requerida_por_campo[5] = (size_t)snprintf(NULL, 0 , "%d", pokemon->velocidad) + 1;

}

/**
 * Reserva la memoria requerida para strings que almacenen los campos de un pokemon y almacena su
 * información.
 * Recibe el vector de strings en el que se va a almacenar, la posicion inicial desde donde se
 * debe empezar a almacenar, el vector con los tamaños en memoria por campo y el pokemon.
*/
int almacenar_en_memoria_cada_campo_pokemon(char** strings_por_cada_campo, size_t* posicion, size_t* memoria_requerida_por_campo, pokemon_t* pokemon){
    
    strings_por_cada_campo[*posicion] = malloc(memoria_requerida_por_campo[0]);
    if(!strings_por_cada_campo[*posicion]){
        return FALLO;
    }
    strcpy(strings_por_cada_campo[*posicion], pokemon->nombre);
    (*posicion)++;

    strings_por_cada_campo[*posicion] = malloc(memoria_requerida_por_campo[1]);
    if(!strings_por_cada_campo[*posicion]){
        return FALLO;
    }
    snprintf(strings_por_cada_campo[*posicion], memoria_requerida_por_campo[1], "%d", pokemon->nivel);
    (*posicion)++;

    strings_por_cada_campo[*posicion] = malloc(memoria_requerida_por_campo[2]);
    if(!strings_por_cada_campo[*posicion]){
        return FALLO;
    }
    snprintf(strings_por_cada_campo[*posicion], memoria_requerida_por_campo[2], "%d", pokemon->defensa);
    (*posicion)++;

    strings_por_cada_campo[*posicion] = malloc(memoria_requerida_por_campo[3]);
    if(!strings_por_cada_campo[*posicion]){
        return FALLO;
    }
    snprintf(strings_por_cada_campo[*posicion], memoria_requerida_por_campo[3], "%d", pokemon->fuerza);
    (*posicion)++;

    strings_por_cada_campo[*posicion] = malloc(memoria_requerida_por_campo[4]);
    if(!strings_por_cada_campo[*posicion]){
        return FALLO;
    }
    snprintf(strings_por_cada_campo[*posicion], memoria_requerida_por_campo[4], "%d", pokemon->inteligencia);
    (*posicion)++;

    strings_por_cada_campo[*posicion] = malloc(memoria_requerida_por_campo[5]);
    if(!strings_por_cada_campo[*posicion]){
        return FALLO;
    }
    snprintf(strings_por_cada_campo[*posicion], memoria_requerida_por_campo[5], "%d", pokemon->velocidad);
    (*posicion)++;

    return EXITO;

}

/**
 * Devuelve true si se debe concatenar un salto de linea en la posición justo posterior a la dada a la
 * hora de concatenar la informacion completa de un equipo pokemon.
 * Devuelve false en caso contrario.
 * (Los campos de un pokemon son 6, por ende se hace salto de linea cada que se finaliza con 6
 * campos concatenados).
*/
bool hay_que_saltar_de_linea(size_t posicion){

    return (posicion%6 == 0);

}

/**
 * Realiza la concatenacion final de todos los datos pokemon.
 * Devuelve el string resultante.
*/
char* concatenar_info_con_formato(lista_t* equipo, char** strings_por_cada_campo, size_t totalidad_de_campos){

    size_t tamanio_total_reserva =  calcular_tamanio_reserva_para_equipo(equipo, strings_por_cada_campo, totalidad_de_campos);

    char* info_concatenada = malloc(tamanio_total_reserva*sizeof(char));
    if(!info_concatenada){
        return NULL;
    }

    strcpy(info_concatenada, strings_por_cada_campo[0]);
    strcat(info_concatenada, SEPARADOR_CONCATENACION);
    //Para este caso particular, debido a que se debe hacer strcpy primero y se empieza iteracion desde la posicion 1, se debe verificar la iteracion 5 y no posicion%6 ya que 5 son los campos que faltaban de ese pokemon, estaba "adelantado" por uno con respecto a los demás.

    for(size_t i = 1; i < totalidad_de_campos; i++){

        if(i < 5){
            strcat(info_concatenada, strings_por_cada_campo[i]);
            strcat(info_concatenada, SEPARADOR_CONCATENACION);
        }
        else if(i == 5){
            strcat(info_concatenada, strings_por_cada_campo[i]);
            strcat(info_concatenada, SALTO_LINEA); 
        }
        else if(i == 6){
            strcat(info_concatenada, strings_por_cada_campo[i]);
            strcat(info_concatenada, SEPARADOR_CONCATENACION); 
        }
        else if(hay_que_saltar_de_linea(i+1)==false && (i > 6)){
            strcat(info_concatenada, strings_por_cada_campo[i]);
            strcat(info_concatenada, SEPARADOR_CONCATENACION);
        }
        else if(hay_que_saltar_de_linea(i+1)==true && (i > 6)){
            strcat(info_concatenada, strings_por_cada_campo[i]);
            strcat(info_concatenada, SALTO_LINEA); 
        }

    }

    return info_concatenada;

}

/**
 * Asigna los strings del vector con la información completa por cada pokemon separada con comas ','.
 * Cada string debe ser posteriormente liberado con free.
 * Devuelve 0 si pudo, -1 en caso de error.
*/
char* equipo_obtener_string_concatenado(lista_t* equipo, size_t cantidad_pokemones){

    size_t totalidad_de_campos = cantidad_pokemones*6;
    char* strings_por_cada_campo[totalidad_de_campos]; //Cada pokemon tiene 6 campos, cada campo de la totalidad es un string a concatenar.

    size_t memoria_requerida_por_campo[6]; // Var. auxiliar para obtener el tamaño requerido para reservar cada campo de pokemon como un string 

    pokemon_t* pokemon_actual = NULL;
    size_t almacenados_hasta_el_momento = 0;
    bool hubo_fallo = false;
    size_t i = 0;
    lista_iterador_t* iter_pokemones = lista_iterador_crear(equipo);
    while(lista_iterador_tiene_siguiente(iter_pokemones) && !hubo_fallo){

        pokemon_actual = lista_iterador_elemento_actual(iter_pokemones);

        calcular_memoria_por_campo_pokemon(memoria_requerida_por_campo, pokemon_actual);

        int resultado_reserva = almacenar_en_memoria_cada_campo_pokemon(strings_por_cada_campo, &i, memoria_requerida_por_campo, pokemon_actual);
        if(resultado_reserva==FALLO){
            hubo_fallo = true;
        }

        lista_iterador_avanzar(iter_pokemones);
        almacenados_hasta_el_momento += 6; //Se almacenan 6 campos por cada iteracion.

    }
    lista_iterador_destruir(iter_pokemones);
    if(hubo_fallo){
        liberar_strings_recopilados(strings_por_cada_campo, almacenados_hasta_el_momento);
        return NULL;
    }

    char* resultado_concatenado = concatenar_info_con_formato(equipo, strings_por_cada_campo, totalidad_de_campos);

    liberar_strings_recopilados(strings_por_cada_campo, almacenados_hasta_el_momento);

    return resultado_concatenado;

}


char* entrenador_obtener_equipo_concatenado(entrenador_t* entrenador){

    size_t cantidad_pokemones = lista_elementos(entrenador->equipo);
    char* informacion_completa_equipo = NULL;

    informacion_completa_equipo = equipo_obtener_string_concatenado(entrenador->equipo, cantidad_pokemones);
    if(!informacion_completa_equipo){
        return NULL;
    }

    return informacion_completa_equipo;

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