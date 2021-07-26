#include <stdio.h>
#include <string.h>
#include "salon.h"
#include "abb.h"
#include "util.h"
#include "hash.h"

const char* REGLA_CLASICA = "CLASICO,Coeficiente de batalla: 0.8*nivel + fuerza + 2*velocidad";
const char* REGLA_CLASICA_NOMBRE = "CLASICO";

const char* REGLA_MODERNA = "MODERNO,Coeficiente de batalla: 0.5*nivel + 0.9*defensa + 3*inteligencia";
const char* REGLA_MODERNA_NOMBRE = "MODERNO";

const char* REGLA_RESISTENCIA = "RESISTENTE,Coeficiente de batalla: (0.75*nivel + defensa)/velocidad";
const char* REGLA_RESISTENCIA_NOMBRE = "RESISTENTE";

const char* REGLA_FISICA = "FISICO,Coeficiente de batalla: (0.25*nivel + 0.8*fuerza - 0.1*inteligencia)*velocidad";
const char* REGLA_FISICA_NOMBRE = "FISICO";

const char* REGLA_ELEGANCIA = "ELEGANTE,Coeficiente de batalla: (0.6*nivel + 0.7*velocidad)*inteligencia/defensa";
const char* REGLA_ELEGANCIA_NOMBRE = "ELEGANTE";

#define SALTO_DE_LINEA "\n"
const char SEPARADOR_PARAMETROS_COMANDO = ',';
const char SEPARADOR_EN_LECTURA = ';';
const char SEPARADOR_NOMBRE_COMANDO = ':';
#define CANTIDAD_CAMPOS_ENTRENADOR 2
#define CANTIDAD_CAMPOS_POKEMON 6

#define PARAMETRO_VICTORIAS "victorias"
#define PARAMETRO_POKEMON "pokemon" // Uso: para distinción de parámetros en comando ENTRENADORES

#define COMANDO_ENTRENADORES "ENTRENADORES"
#define COMANDO_EQUIPO "EQUIPO"
#define COMANDO_REGLAS "REGLAS"
#define COMANDO_COMPARAR "COMPARAR"
#define COMANDO_AGREGAR_POKEMON "AGREGAR_POKEMON"
#define COMANDO_QUITAR_POKEMON "QUITAR_POKEMON"
#define COMANDO_GUARDAR "GUARDAR"

#define EXITO 0
#define FALLO -1


typedef char* (*ejecutar_comando_t)(salon_t*, char**);

typedef struct comando{
    char* nombre;
    ejecutar_comando_t ejecutor;
}comando_t;


struct _salon_t{
    abb_t* abb_entrenadores;
    size_t cantidad_entrenadores;
    hash_t* comandos_predeterminados;
};



/**
 * Devuelve un string (reservado en memoria) duplicado del str.
 * Devuelve NULL si falla en reservarlo.
*/
char* duplicar_str(const char* str){

    char* duplicado = malloc((1+strlen(str))*sizeof(char));
    if(!duplicado){
        return NULL;
    }

    strcpy(duplicado, str);
    return duplicado;

}


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
comando_t* comando_crear(char* nombre_comando, ejecutar_comando_t ejecutor){

    if(!nombre_comando || !ejecutor){
        return NULL;
    }

    comando_t* comando = malloc(1*sizeof(comando_t));
    if(!comando){
        return NULL;
    }
    comando->nombre = nombre_comando;
    comando->ejecutor = ejecutor;

    return comando;

}

/**
 * Recibe la información completa de un comando (no solo su nombre,
 * tambien los argumentos adicionales) dividida en un vector de strings
 * (Es decir: {"Nombre" , "Argumentos"}).
 * Ejecuta el comando recibido y devuelve el output correspondiente
 * en un string.
*/
//char* comando_procesar(char** info_comando){}

/**
 * Destruye el comando recibido (siendo comando un comando_t*. Se recibe
 * como puntero genérico para su uso como destructor en hash).
*/
void comando_destruir(void* comando){

    free(comando);
    comando = NULL;

}








/**
 * Libera todos los strings del vector de punteros.
*/
void liberar_strings_recolectados(char** vector_strings, size_t tamanio_vector){

    for(size_t i = 0; i < tamanio_vector; i++){
        free(vector_strings[i]);
    }

}

/**
 * Dado un vector de strings, concatena a cada uno de los mismos en orden separandolos con un "\n".
 * Devuelve el string resultante de la recopilación o NULL en caso de error.
*/
char* recopilacion_y_formateo_de_strings(char** vector_strings, size_t tamanio_vector){

    char* string_resultante = NULL;
    size_t memoria_requerida = 1; // 1 desde el comienzo para considerar el \0. 

    for(int i = 0; i < tamanio_vector; i++){
        memoria_requerida += (strlen(vector_strings[i]) + 1); //+1 para considerar a los \n.
    }

    string_resultante = malloc(memoria_requerida*sizeof(char));
    if(!string_resultante){
        return NULL;
    }
    strcpy(string_resultante, vector_strings[0]);
    strcat(string_resultante, SALTO_DE_LINEA);

    for(int i = 1; i < tamanio_vector; i++){
        strcat(string_resultante, vector_strings[i]);
        strcat(string_resultante, SALTO_DE_LINEA);
    }

    return string_resultante;

}

/**
 * Finaliza con la ejecución del parametro recibido del comando ENTRENADORES.
 * Devuelve el string de output con el listado de entrenadores correspondiente o NULL en caso de error.
*/
char* procesar_parametro_final_entrenador(salon_t* salon, bool (*filtro)(entrenador_t*, void*), void* parametro_final){

    char* string_resultante = NULL;
    lista_t* entrenadores_filtrados = entrenadores_filtrados = salon_filtrar_entrenadores(salon, filtro, parametro_final);

    if(lista_vacia(entrenadores_filtrados)){
        lista_destruir(entrenadores_filtrados);
        string_resultante = calloc(2, sizeof(char));
        return string_resultante;
    }

    size_t cantidad_info_a_concatenar = lista_elementos(entrenadores_filtrados);
    char* info_a_concatenar[cantidad_info_a_concatenar];

    lista_iterador_t* iter_entrenadores = lista_iterador_crear(entrenadores_filtrados);
    int i = 0;
    bool fallo_en_obtencion = false;
    entrenador_t* entrenador_actual = NULL;
    while(lista_iterador_tiene_siguiente(iter_entrenadores) && !fallo_en_obtencion){

        entrenador_actual = lista_iterador_elemento_actual(iter_entrenadores);

        info_a_concatenar[i] = entrenador_obtener_nombre(entrenador_actual);
        if(info_a_concatenar[i]==NULL){
            fallo_en_obtencion = true;
        }

        lista_iterador_avanzar(iter_entrenadores);
        i++;
    }
    lista_iterador_destruir(iter_entrenadores);
    if(fallo_en_obtencion){
        lista_destruir(entrenadores_filtrados);
        return NULL;
    }

    string_resultante = recopilacion_y_formateo_de_strings(info_a_concatenar, cantidad_info_a_concatenar);
    if(!string_resultante){
        liberar_strings_recolectados(info_a_concatenar, cantidad_info_a_concatenar);
        lista_destruir(entrenadores_filtrados);
        return NULL;
    }

    liberar_strings_recolectados(info_a_concatenar, cantidad_info_a_concatenar);
    lista_destruir(entrenadores_filtrados);

    return string_resultante;
    
}

/**
 * Procesa la ejecución de los parametros "victorias" y "pokemon" del comando ENTRENADORES.
 * Devuelve el string de output con el listado de entrenadores correspondiente o NULL en caso de error.
*/
char* procesar_parametros_de_comando_entrenador(salon_t* salon, char** parametros){

    char* string_resultante = NULL;

    if(strcmp(PARAMETRO_VICTORIAS, parametros[0]) == 0){

        int victorias_minimas_pedidas = atoi(parametros[1]);
        string_resultante = procesar_parametro_final_entrenador(salon, entrenador_tiene_victorias_minimas, &victorias_minimas_pedidas);

    }
    else if(strcmp(PARAMETRO_POKEMON, parametros[0]) == 0){

        char* nombre_pokemon_pedido = parametros[1];
        string_resultante = procesar_parametro_final_entrenador(salon, entrenador_tiene_pokemon, nombre_pokemon_pedido);

    }

    return string_resultante;

}

/**
 * Recibe argumentos relacionados al comando ENTRENADORES y un salon (ya inicializado).
 * Ejecuta la solicitud que posean los argumentos y devuelve un string de output con un listado de
 * entrenadores determinado.
*/
char* salon_pedir_entrenadores_listados(salon_t* salon, char** argumentos){

    char* string_resultante = NULL;
    size_t cantidad_argumentos = vtrlen(argumentos); 

    if(cantidad_argumentos == 1){ //Solo se tenía el argumento ENTRENADORES sin parámetros adicionales.

        entrenador_t* vector_entrenadores[salon->cantidad_entrenadores];
        arbol_recorrido_inorden(salon->abb_entrenadores, (void**)vector_entrenadores, salon->cantidad_entrenadores);

        char* informacion_entrenadores[salon->cantidad_entrenadores];
        
        for(int i = 0; i < salon->cantidad_entrenadores; i++){
            informacion_entrenadores[i] = entrenador_obtener_info_concatenada(vector_entrenadores[i]);
        }

        string_resultante = recopilacion_y_formateo_de_strings(informacion_entrenadores, salon->cantidad_entrenadores);
        if(!string_resultante){
            liberar_strings_recolectados(informacion_entrenadores, salon->cantidad_entrenadores);
            return NULL;
        }

        liberar_strings_recolectados(informacion_entrenadores, salon->cantidad_entrenadores);

    }
    else if(cantidad_argumentos == 2){ //Habían más parámetros. No se usa 'else' porque no existen comandos con cantidad de argumentos que no sea 1 ni 2.

        char** parametros_extras_divididos = split(argumentos[1], SEPARADOR_PARAMETROS_COMANDO);
        if(!parametros_extras_divididos){
            return NULL;
        }
        else if(vtrlen(parametros_extras_divididos) != 2){
            vtrfree(parametros_extras_divididos);
            return NULL;
        }

        string_resultante = procesar_parametros_de_comando_entrenador(salon, parametros_extras_divididos);

        vtrfree(parametros_extras_divididos);

    }

    return string_resultante;

}

/**
 * Busca linealmente un entrenador del vector de punteros a entrenadores a partir de su nombre.
 * Devuelve al entrenador buscado o NULL en caso de error.
*/
entrenador_t* buscar_entrenador_con_nombre(char* nombre_entrenador_pedido, entrenador_t** vector_entrenadores, size_t tamanio_vector){
    
    size_t i = 0;
    bool fallo_en_obtencion = false , fue_encontrado_entrenador = false;
    char* auxiliar_nombre_actual = NULL;
    entrenador_t* entrenador_encontrado = NULL;
    while((i < tamanio_vector) && !fallo_en_obtencion && !fue_encontrado_entrenador){

        auxiliar_nombre_actual = entrenador_obtener_nombre(vector_entrenadores[i]);

        if(!auxiliar_nombre_actual){
            fallo_en_obtencion = true;
        }
        else if(strcmp(auxiliar_nombre_actual, nombre_entrenador_pedido)==0){
            fue_encontrado_entrenador = true;
            entrenador_encontrado = vector_entrenadores[i];      
        }

        i++;
        free(auxiliar_nombre_actual);

    }
    if(fallo_en_obtencion){
        return NULL;
    }

    return entrenador_encontrado;

}

/**
 * Recibe argumentos relacionados al comando EQUIPO y un salon (ya inicializado).
 * Ejecuta la solicitud que posean los argumentos y devuelve un string de output con el equipo pokemon
 * de un entrenador determinado.
*/
char* salon_pedir_equipo_de_entrenador(salon_t* salon, char** argumentos){

    if(vtrlen(argumentos) != 2){
        return NULL;
    }

    char* nombre_entrenador_pedido = argumentos[1];
    char* string_resultado = NULL;

    entrenador_t* vector_entrenadores[salon->cantidad_entrenadores];
    arbol_recorrido_inorden(salon->abb_entrenadores, (void**)vector_entrenadores, salon->cantidad_entrenadores);

    entrenador_t* entrenador_buscado = buscar_entrenador_con_nombre(nombre_entrenador_pedido, vector_entrenadores, salon->cantidad_entrenadores);
    if(!entrenador_buscado){
        return NULL;
    }
   
    string_resultado = entrenador_obtener_equipo_concatenado(entrenador_buscado);
    
    return string_resultado;

}


/**
 * Calcula la memoria necesaria para reservar un string con todas las reglas concatenadas del salon.
 * Devuelve el calculo realizado.
*/
size_t calcular_memoria_para_reglas(){

    return (strlen(REGLA_CLASICA)+strlen(REGLA_MODERNA)+strlen(REGLA_RESISTENCIA)+strlen(REGLA_FISICA)+strlen(REGLA_ELEGANCIA) + 6); // +5 para los \n al concatenar y +1 para el \0.

}

/**
 * Recibe un salon (ya inicializado). 
 * Los argumentos no son usados porque simplemente es el comando (REGLAS).
 * Devuelve un string de output con todas las reglas de batalla disponibles o NULL en caso de error.
*/
char* salon_pedir_reglas(salon_t* salon, char** argumentos){

    if(vtrlen(argumentos)!=1){
        return NULL;
    }

    size_t tamanio_requerido = calcular_memoria_para_reglas();

    char* string_resultado = malloc(tamanio_requerido*sizeof(char));
    if(!string_resultado){
        return NULL;
    }
    strcpy(string_resultado, REGLA_CLASICA);
    strcat(string_resultado, SALTO_DE_LINEA);
    strcat(string_resultado, REGLA_MODERNA);
    strcat(string_resultado, SALTO_DE_LINEA);
    strcat(string_resultado, REGLA_RESISTENCIA);
    strcat(string_resultado, SALTO_DE_LINEA);
    strcat(string_resultado, REGLA_FISICA);
    strcat(string_resultado, SALTO_DE_LINEA);
    strcat(string_resultado, REGLA_ELEGANCIA);
    strcat(string_resultado, SALTO_DE_LINEA);

    return string_resultado;

}

/**
 * Recibe el nombre de una regla pedida por argumento de comando.
 * Devuelve true si la relga NO está dentro de las disponibles para comparar entrenadores.
 * Devuelve false en caso contrario.
*/
bool regla_dada_no_existe(char* regla_pedida){

    bool es_inexistente = !(strcmp(regla_pedida, REGLA_CLASICA_NOMBRE)==0 || strcmp(regla_pedida, REGLA_MODERNA_NOMBRE)==0 || strcmp(regla_pedida, REGLA_RESISTENCIA_NOMBRE)==0 || strcmp(regla_pedida, REGLA_FISICA_NOMBRE)==0 || strcmp(regla_pedida, REGLA_ELEGANCIA_NOMBRE)==0);

    return es_inexistente;

}

/**
 * Recibe argumentos relacionados al comando COMPARAR y un salon (ya inicializado).
 * Ejecuta la solicitud que posean los argumentos y devuelve un string de output con el resultado de
 * todas las posibles batallas entre los equipos pokemon de dos entrenadores.
*/
char* salon_comparar_entrenadores(salon_t* salon, char** argumentos){

    if(vtrlen(argumentos) != 2){
        return NULL;
    }

    char** parametros_a_procesar = split(argumentos[1], SEPARADOR_PARAMETROS_COMANDO);
    if(vtrlen(parametros_a_procesar) != 3){
        vtrfree(parametros_a_procesar);
        return NULL;
    }
    else if(regla_dada_no_existe(parametros_a_procesar[2])){
        vtrfree(parametros_a_procesar);
        return NULL;
    }

    entrenador_t* vector_entrenadores[salon->cantidad_entrenadores];
    arbol_recorrido_inorden(salon->abb_entrenadores, (void**)vector_entrenadores, salon->cantidad_entrenadores);

    entrenador_t* entrenador_buscado_1 = buscar_entrenador_con_nombre(parametros_a_procesar[0], vector_entrenadores, salon->cantidad_entrenadores);
    if(!entrenador_buscado_1){
        vtrfree(parametros_a_procesar);
        return NULL;
    }
    entrenador_t* entrenador_buscado_2 = buscar_entrenador_con_nombre(parametros_a_procesar[1], vector_entrenadores, salon->cantidad_entrenadores);
    if(!entrenador_buscado_2){
        vtrfree(parametros_a_procesar);
        return NULL;
    }

    char* resultados_enfrentamiento = entrenador_enfrentar(entrenador_buscado_1, entrenador_buscado_2, parametros_a_procesar[2]);

    vtrfree(parametros_a_procesar);

    return resultados_enfrentamiento;

}

/**
 * Duplica los strings que se tenían en los 'parametros_a_procesar' que correspondian a la información
 * de los campos de un pokemon y crea un vector dinámico de punteros finalizado en NULL almacenando los
 * strings duplicados.
 * Devuelve el vector creado o NULL en caso de error.
 * El vector devuelto debe ser liberado con vtrfree.
*/
char** duplicar_parametros_pokemon(char** parametros_a_procesar){

    char** parametros_pokemon = NULL;
    char* vector_strings_auxiliar[6];
    size_t duplicados_hasta_el_momento = 0;
    bool hubo_fallo = false;

    for(int i = 1; (i <= 6) && !hubo_fallo ; i++){ // 6 es la cantidad de campos de un pokemon, parametros_a_procesar[0] es el nombre de entrenador.
        
        vector_strings_auxiliar[i-1] = duplicar_str(parametros_a_procesar[i]);
        if(vector_strings_auxiliar[i-1] == NULL){
            hubo_fallo = true;
            liberar_strings_recolectados(vector_strings_auxiliar, duplicados_hasta_el_momento);
        }

        duplicados_hasta_el_momento++;

    }
    if(hubo_fallo){
        return NULL;
    }

    for(int i = 1; i <= 6; i++){ 
        parametros_pokemon = vtradd(parametros_pokemon, vector_strings_auxiliar[i-1]);
    }

    return parametros_pokemon;

}

/**
 * Recibe argumentos relacionados al comando AGREGAR_POKEMON y un salon (ya inicializado).
 * Agrega a un entrenador determinado el pokemon con los datos que posean los argumentos y devuelve
 * un string de output "OK".
*/
char* salon_pedir_agregar_pokemon(salon_t* salon, char** argumentos){

    if(vtrlen(argumentos) != 2){
        return NULL;
    }
    
    char** parametros_a_procesar = split(argumentos[1], SEPARADOR_PARAMETROS_COMANDO);
    if(vtrlen(parametros_a_procesar) != 7){
        vtrfree(parametros_a_procesar);
        return NULL;
    }

    entrenador_t* vector_entrenadores[salon->cantidad_entrenadores];
    arbol_recorrido_inorden(salon->abb_entrenadores, (void**)vector_entrenadores, salon->cantidad_entrenadores);

    entrenador_t* entrenador_buscado = buscar_entrenador_con_nombre(parametros_a_procesar[0], vector_entrenadores, salon->cantidad_entrenadores);
    if(!entrenador_buscado){
        vtrfree(parametros_a_procesar);
        return NULL;
    }

    char** parametros_pokemon = duplicar_parametros_pokemon(parametros_a_procesar);
    if(!parametros_pokemon){
        vtrfree(parametros_a_procesar);
        return NULL;
    }
    
    int resultado_agregado = entrenador_agregar_pokemon_leido(entrenador_buscado, parametros_pokemon);

    vtrfree(parametros_a_procesar);
    vtrfree(parametros_pokemon);

    char* string_resultante = malloc(3*sizeof(char));
    if((resultado_agregado == FALLO) || !string_resultante){
        return NULL;
    }
    strcpy(string_resultante, "OK");

    return string_resultante;

}

/**
 * Recibe argumentos relacionados al comando QUITAR_POKEMON y un salon (ya inicializado).
 * Intenta quitar a un entrenador determinado el pokemon con el nombre que posean los argumentos.
 * Devuelve un string de output "OK" si pudo quitarlo o NULL en caso de que el entrenador tenga solo
 * un pokemon restante en su equipo (o en caso de error).
*/
char* salon_pedir_quitar_pokemon(salon_t* salon, char** argumentos){

    if(vtrlen(argumentos) != 2){
        return NULL;
    }
    char** parametros_a_procesar = split(argumentos[1], SEPARADOR_PARAMETROS_COMANDO);
    if(vtrlen(parametros_a_procesar) != 2){
        vtrfree(parametros_a_procesar);
        return NULL;
    }

    entrenador_t* vector_entrenadores[salon->cantidad_entrenadores];
    arbol_recorrido_inorden(salon->abb_entrenadores, (void**)vector_entrenadores, salon->cantidad_entrenadores);

    entrenador_t* entrenador_buscado = buscar_entrenador_con_nombre(parametros_a_procesar[0], vector_entrenadores, salon->cantidad_entrenadores);
    char* nombre_pokemon_a_quitar = parametros_a_procesar[1];
    if(!entrenador_buscado || (strlen(nombre_pokemon_a_quitar) == 0)){
        vtrfree(parametros_a_procesar);
        return NULL;
    }
    
    int resultado_quitar = entrenador_quitar_pokemon(entrenador_buscado, nombre_pokemon_a_quitar);
    
    vtrfree(parametros_a_procesar);
    
    if(resultado_quitar == FALLO){
        return NULL;
    }

    char* string_resultante = malloc(3*sizeof(char));
    if(!string_resultante){
        return NULL;
    }
    strcpy(string_resultante, "OK");

    return string_resultante;

}

/**
 * Recibe argumentos relacionados al comando GUARDAR y un salon (ya inicializado).
 * Guarda el salon a un archivo a partir del nombre que posea el argumento y devuelve un string
 * de output "OK" si pudo guardarlo, NULL en caso contrario.
*/
char* salon_pedir_guardado(salon_t* salon, char** argumentos){

    if(vtrlen(argumentos)!=2){
        return NULL;
    }

    char** parametros_a_procesar = split(argumentos[1], SEPARADOR_PARAMETROS_COMANDO);
    if(vtrlen(parametros_a_procesar) != 1){
        vtrfree(parametros_a_procesar);
        return NULL;
    }

    char* nombre_archivo_a_guardar = parametros_a_procesar[0];
    if(strlen(nombre_archivo_a_guardar)==0){
        vtrfree(parametros_a_procesar);
        return NULL;
    }

    int resultado_guardado = salon_guardar_archivo(salon, nombre_archivo_a_guardar);

    char* string_resultante = malloc(3*sizeof(char));
    if(!string_resultante || (resultado_guardado==FALLO)){
        vtrfree(parametros_a_procesar);
        return NULL;
    }
    strcpy(string_resultante, "OK");

    vtrfree(parametros_a_procesar);
    return string_resultante;

}



//=========================================================================================//





/**
 * Verifica que todos los entrenadores leidos en un archivo (y almacenados en un salon)
 * tengan al menos un pokemon en su equipo.
 * Devuelve 0 si todos los entrenadores son válidos, -1 si encuentra al menos uno que no lo sea.
*/
int verificacion_entrenadores_validos(salon_t* salon){

    bool hubo_entrenador_invalido = false;
    size_t cantidad_de_entrenadores_validos = abb_con_cada_elemento(salon->abb_entrenadores, ABB_RECORRER_INORDEN, entrenador_equipo_vacio, &hubo_entrenador_invalido);
    
    if((cantidad_de_entrenadores_validos != salon->cantidad_entrenadores) || (hubo_entrenador_invalido==true)){
        return FALLO;
    }

    entrenador_t* vector_entrenadores[salon->cantidad_entrenadores];
    arbol_recorrido_inorden(salon->abb_entrenadores, (void**)vector_entrenadores, salon->cantidad_entrenadores);

    size_t i = 0;
    size_t j = 0;
    while((i < salon->cantidad_entrenadores) && (hubo_entrenador_invalido==false)){

        while((j < salon->cantidad_entrenadores) && (hubo_entrenador_invalido==false)){
            if(i != j){
                hubo_entrenador_invalido = entrenador_verificar_identicos(vector_entrenadores[i], vector_entrenadores[j]);
            }
            j++;
        }
        j = 0;
        i++;

    }
    if(hubo_entrenador_invalido==true){
        return FALLO;
    }

    return EXITO;

}



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
        salon->cantidad_entrenadores++;  
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
salon_t* almacenador_salon(char* linea_leida, FILE* archivo_salon, salon_t* salon){

    salon->abb_entrenadores = arbol_crear(entrenador_comparar_nombres, entrenador_destruir);
    if(!salon->abb_entrenadores){
        return NULL;
    }
    salon->cantidad_entrenadores = 0;

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

            entrenador_aux = almacenador_entrenadores(entrenador_aux, entrenador_previo, &es_primer_entrenador, campos_leidos_separados, salon);
            if(entrenador_aux == NULL){
                vtrfree(campos_leidos_separados);
                return NULL;
            }
            
        }
        else if(cantidad_campos_separados == CANTIDAD_CAMPOS_POKEMON ){

            resultado_de_inserciones = entrenador_agregar_pokemon_leido(entrenador_aux, campos_leidos_separados);
            if(resultado_de_inserciones == FALLO){
                vtrfree(campos_leidos_separados);
                return NULL;
            }

        }

        linea_leida = fgets_alloc(archivo_salon);

        if(linea_leida == NULL){
            resultado_de_inserciones = arbol_insertar(salon->abb_entrenadores, entrenador_aux);
            if(resultado_de_inserciones == FALLO){
                vtrfree(campos_leidos_separados);
                return NULL;
            }
            salon->cantidad_entrenadores++;
        }

        vtrfree(campos_leidos_separados);
    
    }

    return salon;

}








/**
 * Hace free de todos los punteros recibidos en caso de falla al agregar los comandos por defecto.
 * (Mini-función auxiliar para ahorrar lineas de verificaciones al agregar los comandos por defecto).
*/
void liberar_anteriores_por_falla(void* ptr_1, void* ptr_2, void* ptr_3 , void* ptr_4, void* ptr_5, void* ptr_6, void* ptr_7){
    
    free(ptr_1);
    free(ptr_2);
    free(ptr_3);
    free(ptr_4);
    free(ptr_5);
    free(ptr_6);
    free(ptr_7);

}

/**
 * Inserta todos los comandos recibidos en el hash.
 * Devuelve true si pudo, false en caso contrario
 * (Mini-función auxiliar para ahorrar lineas de insertar en hash).
*/
bool insertar_comandos_predeterminados(hash_t* hash_comandos, comando_t* c_1, comando_t* c_2, comando_t* c_3 , comando_t* c_4, comando_t* c_5, comando_t* c_6, comando_t* c_7){
    
    int resultado_insercion = hash_insertar(hash_comandos, COMANDO_ENTRENADORES, c_1);
    if(resultado_insercion==FALLO){
        return false;
    }
    
    resultado_insercion = hash_insertar(hash_comandos, COMANDO_EQUIPO, c_2);
    if(resultado_insercion==FALLO){
        return false;
    }

    resultado_insercion = hash_insertar(hash_comandos, COMANDO_REGLAS, c_3);
    if(resultado_insercion==FALLO){
        return false;
    }

    resultado_insercion = hash_insertar(hash_comandos, COMANDO_COMPARAR, c_4);
    if(resultado_insercion==FALLO){
        return false;
    }

    resultado_insercion = hash_insertar(hash_comandos, COMANDO_AGREGAR_POKEMON, c_5);
    if(resultado_insercion==FALLO){
        return false;
    }

    resultado_insercion = hash_insertar(hash_comandos, COMANDO_QUITAR_POKEMON, c_6);
    if(resultado_insercion==FALLO){
        return false;
    }

    resultado_insercion = hash_insertar(hash_comandos, COMANDO_GUARDAR, c_7);
    if(resultado_insercion==FALLO){
        return false;
    }

    return true;

}

/**
 * Dado un salón correctamente creado a partir de un archivo, le inicializa
 * un diccionario de comandos predeterminados a ser usados posteriormente
 * en salon_ejecutar_comando. 
 * Devuelve 0 si pudo inicializarlos correctamente, -1 en caso contrario.
 * (Leer README.txt, sección Aclaraciones).
*/
int salon_agregar_comandos_por_defecto(salon_t* salon){

    comando_t* comando_entrenadores = comando_crear(COMANDO_ENTRENADORES, salon_pedir_entrenadores_listados);
    if(!comando_entrenadores){
        return FALLO;
    }

    comando_t* comando_equipo = comando_crear(COMANDO_EQUIPO, salon_pedir_equipo_de_entrenador);
    if(!comando_equipo){
        liberar_anteriores_por_falla(comando_entrenadores,NULL,NULL,NULL,NULL,NULL,NULL);
        return FALLO;
    }
    
    comando_t* comando_reglas = comando_crear(COMANDO_REGLAS, salon_pedir_reglas);
    if(!comando_reglas){
        liberar_anteriores_por_falla(comando_entrenadores,comando_equipo,NULL,NULL,NULL,NULL,NULL);
        return FALLO;
    }
    
    comando_t* comando_comparar = comando_crear(COMANDO_COMPARAR, salon_comparar_entrenadores);
    if(!comando_comparar){
        liberar_anteriores_por_falla(comando_entrenadores,comando_equipo,comando_reglas,NULL,NULL,NULL,NULL);
        return FALLO;
    }
    
    comando_t* comando_agregar_pokemon = comando_crear(COMANDO_AGREGAR_POKEMON, salon_pedir_agregar_pokemon);
    if(!comando_agregar_pokemon){
        liberar_anteriores_por_falla(comando_entrenadores,comando_equipo,comando_reglas,comando_comparar,NULL,NULL,NULL);
        return FALLO;
    }
    
    comando_t* comando_quitar_pokemon = comando_crear(COMANDO_QUITAR_POKEMON, salon_pedir_quitar_pokemon);
    if(!comando_quitar_pokemon){
        liberar_anteriores_por_falla(comando_entrenadores,comando_equipo,comando_reglas,comando_comparar,comando_agregar_pokemon,NULL,NULL);
        return FALLO;
    }

    comando_t* comando_guardar_salon = comando_crear(COMANDO_GUARDAR, salon_pedir_guardado);
    if(!comando_guardar_salon){
        liberar_anteriores_por_falla(comando_entrenadores,comando_equipo,comando_reglas,comando_comparar,comando_agregar_pokemon,comando_quitar_pokemon,NULL);
        return FALLO;
    }

    salon->comandos_predeterminados = hash_crear(comando_destruir, 10);
    if(!salon->comandos_predeterminados){
        liberar_anteriores_por_falla(comando_entrenadores,comando_equipo,comando_reglas,comando_comparar,comando_agregar_pokemon,comando_quitar_pokemon,comando_guardar_salon);
        return FALLO;
    }

    bool se_insertaron_todos_los_comandos = insertar_comandos_predeterminados(salon->comandos_predeterminados, comando_entrenadores, comando_equipo, comando_reglas, comando_comparar, comando_agregar_pokemon, comando_quitar_pokemon, comando_guardar_salon);

    if(se_insertaron_todos_los_comandos){
        return EXITO;
    }
    else{
        hash_destruir(salon->comandos_predeterminados);
        liberar_anteriores_por_falla(comando_entrenadores,comando_equipo,comando_reglas,comando_comparar,comando_agregar_pokemon,comando_quitar_pokemon,comando_guardar_salon);
        return FALLO;
    }

}









salon_t* salon_leer_archivo(const char* nombre_archivo){

    if(!nombre_archivo){
        return NULL;
    }

    FILE* archivo_salon = fopen(nombre_archivo, "r");
    if(archivo_salon == NULL){
        return NULL;
    }
    
    salon_t* salon = calloc(1, sizeof(salon_t));
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
    else if(verificacion_entrenadores_validos(salon)==FALLO){
        salon_destruir(salon);
        fclosen(archivo_salon);
        return NULL;
    }

    int resultado_comandos_agregados = salon_agregar_comandos_por_defecto(salon);
    if(resultado_comandos_agregados == FALLO){
        salon_destruir(salon);
        fclosen(archivo_salon);
        return NULL;
    }

    fclosen(archivo_salon);

    return salon; 

}





/**
 * Escribe un salon en un archivo de texto (previamente abierto).
 * Devuelve la cantidad de entrenadores escritos.
 */
int salon_a_texto(salon_t* salon, FILE* archivo_nuevo){

    int cantidad_entrenadores_escritos = (int)abb_con_cada_elemento(salon->abb_entrenadores, ABB_RECORRER_INORDEN, entrenador_escribir_en_archivo, archivo_nuevo);

    return cantidad_entrenadores_escritos;

}





int salon_guardar_archivo(salon_t* salon, const char* nombre_archivo){

    if(!salon || !nombre_archivo){
        return FALLO;
    }

    FILE* archivo_nuevo = fopen(nombre_archivo, "w");
    if(archivo_nuevo == NULL){
        return FALLO;
    }

    int cantidad_entrenadores_escritos = 0;

    cantidad_entrenadores_escritos = salon_a_texto(salon, archivo_nuevo);

    fclosen(archivo_nuevo);

    return cantidad_entrenadores_escritos;

}




salon_t* salon_agregar_entrenador(salon_t* salon, entrenador_t* entrenador){

    
    if((!salon) || (!entrenador)){
        return NULL;
    }
    else if(entrenador_tamanio_equipo(entrenador)==0){
        return NULL;
    }

    int resultado_insercion = arbol_insertar(salon->abb_entrenadores, entrenador);
    if(resultado_insercion == FALLO){
        return NULL;
    }
    salon->cantidad_entrenadores++;
    
    return salon;

}





lista_t* salon_filtrar_entrenadores(salon_t* salon , bool (*f)(entrenador_t*, void*), void* extra){

    if(!salon || !f){
        return NULL;
    }

    lista_t* lista_filtrados = lista_crear(NULL);
    if(!lista_filtrados){
        return NULL;
    }

    entrenador_t* vector_entrenadores[salon->cantidad_entrenadores];
    arbol_recorrido_inorden(salon->abb_entrenadores, (void**)vector_entrenadores, salon->cantidad_entrenadores);

    size_t i = 0;
    int resultado_insercion = EXITO;
    while((i < salon->cantidad_entrenadores) && (resultado_insercion != FALLO)){

        if( f(vector_entrenadores[i], extra) == true ){
            resultado_insercion = lista_insertar(lista_filtrados, vector_entrenadores[i]);
        }
        i++;

    }
    if(resultado_insercion==FALLO){
        lista_destruir(lista_filtrados);
        return NULL;
    }

    return lista_filtrados;

}




char* salon_ejecutar_comando(salon_t* salon, const char* comando){

    if(!salon || !comando){
        return NULL;
    }
    else if(strlen(comando)==0){
        return NULL;
    }

    char** argumentos_a_procesar = split(comando, SEPARADOR_NOMBRE_COMANDO);
    comando_t* comando_requerido = hash_obtener(salon->comandos_predeterminados, argumentos_a_procesar[0]);
    if(!comando_requerido){
        vtrfree(argumentos_a_procesar);
        return NULL;
    }

    char* resultados_de_ejecucion = comando_requerido->ejecutor(salon, argumentos_a_procesar);

    vtrfree(argumentos_a_procesar);
    return resultados_de_ejecucion;

}




void salon_destruir(salon_t* salon){

    if(!salon){
        return;
    }

    arbol_destruir(salon->abb_entrenadores);
    hash_destruir(salon->comandos_predeterminados);
    free(salon);

}


