#include <stdio.h>
#include "salon.h"
#include "abb.h"
#include "util.h"
#include "hash.h"

const char* REGLA_CLASICA = "CLASICO,Coeficiente de batalla: 0,8*nivel + fuerza + 2*velocidad";
const char* REGLA_MODERNA = "MODERNO,Coeficiente de batalla: 0,5*nivel + 0,9*defensa + 3*inteligencia";
const char* REGLA_RESISTENCIA = "ACORAZADO,Coeficiente de batalla: (0,75*nivel + defensa)/velocidad";
const char* REGLA_FISICA = "FISICO,Coeficiente de batalla: (0,25*nivel + 0,8*fuerza - 0,1*inteligencia)*velocidad";
const char* REGLA_ELEGANCIA = "ELEGANTE,Coeficiente de batalla: (0,6*nivel + 0,7*velocidad)*inteligencia/defensa ";


const char SEPARADOR_EN_LECTURA = ';';
const char SEPARADOR_NOMBRE_COMANDO = ':';
const char SEPARADOR_ARGUMENTOS_COMANDO = ',';
#define CANTIDAD_CAMPOS_ENTRENADOR 2
#define CANTIDAD_CAMPOS_POKEMON 6

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

    if(!comando){
        return;
    }
    free(comando);
    comando = NULL;

}




/**
 * Recibe argumentos relacionados al comando ENTRENADORES y un salon (ya inicializado).
 * Ejecuta la solicitud que posean los argumentos y devuelve un string de output correspondiente a
 * las acciones de cada uno.
*/
char* salon_pedir_entrenadores_listados(salon_t* salon, char** argumentos){

    return NULL;

}

/**
 * Recibe argumentos relacionados al comando EQUIPO y un salon (ya inicializado).
 * Ejecuta la solicitud que posean los argumentos y devuelve un string de output correspondiente a
 * las acciones de cada uno.
*/
char* salon_pedir_equipo_de_entrenador(salon_t* salon, char** argumentos){

    return NULL;

}

/**
 * Recibe argumentos relacionados al comando REGLAS y un salon (ya inicializado).
 * Ejecuta la solicitud que posean los argumentos y devuelve un string de output correspondiente a
 * las acciones de cada uno.
*/
char* salon_pedir_reglas(salon_t* salon, char** argumentos){

    return NULL;

}

/**
 * Recibe argumentos relacionados al comando COMPARAR y un salon (ya inicializado).
 * Ejecuta la solicitud que posean los argumentos y devuelve un string de output correspondiente a
 * las acciones de cada uno.
*/
char* salon_comparar_entrenadores(salon_t* salon, char** argumentos){

    return NULL;

}

/**
 * Recibe argumentos relacionados al comando AGREGAR_POKEMON y un salon (ya inicializado).
 * Ejecuta la solicitud que posean los argumentos y devuelve un string de output correspondiente a
 * las acciones de cada uno.
*/
char* salon_pedir_agregar_pokemon(salon_t* salon, char** argumentos){

    return NULL;

}

/**
 * Recibe argumentos relacionados al comando QUITAR_POKEMON y un salon (ya inicializado).
 * Ejecuta la solicitud que posean los argumentos y devuelve un string de output correspondiente a
 * las acciones de cada uno.
*/
char* salon_pedir_quitar_pokemon(salon_t* salon, char** argumentos){

    return NULL;

}

/**
 * Recibe argumentos relacionados al comando GUARDAR y un salon (ya inicializado).
 * Ejecuta la solicitud que posean los argumentos y devuelve un string de output correspondiente a
 * las acciones de cada uno.
*/
char* salon_pedir_guardado(salon_t* salon, char** argumentos){

    return NULL;

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

    return NULL;

}




void salon_destruir(salon_t* salon){

    if(!salon){
        return;
    }

    arbol_destruir(salon->abb_entrenadores);
    hash_destruir(salon->comandos_predeterminados);
    free(salon);

}


