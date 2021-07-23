#include "pa2mm.h"
#include "src/salon.h"
#include "src/util.h"


#define FALLO -1
#define EXITO 0

#define SALON_1E_1P "archivos_prueba/salon_1e_1p.txt"
#define SALON_1E_SIN_P "archivos_prueba/salon_1e_sin_p.txt"
#define SALON_2E_2P "archivos_prueba/salon_2e_2p.txt"
#define SALON_6E_ALGUNOS_SIN_P "archivos_prueba/salon_6e_con_algunos_sin_p.txt"
#define SALON_2E_CON_P_EN_DISTINTA_CANTIDAD "archivos_prueba/salon_2e_con_p_en_distinta_cantidad.txt"
#define SALON_2E_IGUALES "archivos_prueba/salon_2e_iguales.txt"

#define ARCHIVO_DE_PRUEBA_PARA_ESCRIBIR_1 "archivos_prueba/salon_guardado_prueba_1.txt"
#define ARCHIVO_DE_PRUEBA_PARA_ESCRIBIR_2 "archivos_prueba/salon_guardado_prueba_2.txt"



//////////// DESTRUCCION DE ELEMENTOS EN LISTA ////////////

/**
 * Función de "destrucción" auxiliar de prueba.
 * Le suma uno al entero recibido
 * (Si bien solo aumenta un entero, basta para controlar que se aplicó un destructor a todos los elementos requeridos en vez de complicarse reservando memoria, por ejemplo).
*/
void destructor_de_prueba_gracioso(void* elemento){
    int* entero = elemento;
    (*entero)++;
}


void SinImportarSiSeDaONoSeDaDestructor_AlCrearLista_SeCreaCorrectamente(){

    lista_t* lista = lista_crear(NULL);
    pa2m_afirmar(lista != NULL , "Se puede crear una lista sin destructor de elementos.");
    lista_destruir(lista);

    lista = lista_crear(destructor_de_prueba_gracioso);
    pa2m_afirmar(lista != NULL , "Se puede crear una lista con destructor de elementos.");
    lista_destruir(lista);

    printf("\n");

}


void DadaListaConDestructor_AlDestruirLista_SeDestruyenCorrectamenteTodosLosElementos(){

    lista_t* lista = lista_crear(destructor_de_prueba_gracioso);
    
    int uno = 1 , dos = 2 , tres = 3; //elementos de prueba
    lista_insertar(lista, &uno);
    lista_insertar(lista, &dos);
    lista_insertar(lista, &tres);

    lista_destruir(lista);
    pa2m_afirmar( (uno==2) && (dos==3) && (tres==4) , "Se aplicó el destructor a todos los elementos de una lista al destruirla.");

    printf("\n");

}


void DadaListaConDestructor_AlBorrarElementos_SeBorranCorrectamente(){

    lista_t* lista = lista_crear(destructor_de_prueba_gracioso);
    
    int uno = 1 , dos = 2 , tres = 3, cuatro = 4 , cinco = 5 , seis = 6; //elementos de prueba
    lista_insertar(lista, &uno);
    lista_insertar(lista, &dos);
    lista_insertar(lista, &tres);
    lista_insertar(lista, &cuatro);
    lista_insertar(lista, &cinco);
    lista_insertar(lista, &seis);


    pa2m_afirmar( lista_borrar(lista) == EXITO , "Se borró el último elemento de una lista con destructor.");
    pa2m_afirmar( (seis==7) && (cinco==5) && (cuatro==4) && (tres==3) && (dos==2) && (uno==1), "Dicho elemento (y solo ese) fue destruido correctamente.");

    pa2m_afirmar( lista_borrar_de_posicion(lista, 3) == EXITO , "Se borró elemento de una posición intermedia.");
    pa2m_afirmar( (cuatro==5) && (cinco==5) && (tres==3) && (dos==2) && (uno==1), "Dicho elemento (y solo ese) fue destruido correctamente.");

    pa2m_afirmar( lista_borrar(lista) == EXITO , "Se volvió a borrar el último elemento.");
    pa2m_afirmar( (cinco==6) && (tres==3) && (dos==2) && (uno==1), "Dicho elemento (y solo ese) fue destruido correctamente.");

    pa2m_afirmar( lista_borrar_de_posicion(lista, EXITO) == 0 , "Se borró elemento de la primera posición.");
    pa2m_afirmar( (uno==2) && (tres==3) && (dos==2), "Dicho elemento (y solo ese) fue destruido correctamente.");

    lista_destruir(lista);
    pa2m_afirmar((dos==3) && (tres==4) , "Se aplicó el destructor a todos los elementos restantes al destruir la lista.");
    printf("\n");

}




//////////// CREACION DE ENTRENADOR ////////////

void DadosDatosInexistentes_SiSePideCrearEntrenador_SeDevuelveNull(){

    pa2m_afirmar( entrenador_crear(NULL) == NULL , "No se puede crear un entrenador a partir de datos inexistentes.");

    printf("\n");

}


void DadosDatos_AlCrearEntrenador_SeCreaEntrenadorCorrectamente(){

    char* datos_entrenador_prueba_1[2] = {"Mauro" , "20"};
    char* datos_entrenador_prueba_2[2] = {"Franco" , "0"};

    entrenador_t* _entrenador_1 = entrenador_crear(datos_entrenador_prueba_1);
    pa2m_afirmar( _entrenador_1 != NULL , "Se creó un entrenador correctamente.");

    entrenador_t* _entrenador_2 = entrenador_crear(datos_entrenador_prueba_2);
    pa2m_afirmar( _entrenador_2 != NULL , "Se creó un entrenador sin victorias correctamente.");

    entrenador_destruir(_entrenador_1);
    entrenador_destruir(_entrenador_2);
 
    printf("\n");

}





//////////// AGREGAR POKEMON Y TAMAÑO DE EQUIPO ////////////

void DadoEntrenadorODatosPokemonInexistentes_SiSePideAgregarPokemon_SeDevuelveFallo(){

    char* datos_pokemon_prueba[6] = {"Charizard" , "1" , "20" , "30" , "40" , "50"};
    char* datos_entrenador_prueba[2] = {"Mauro" , "20"}; //Los datos simulan strings leidos de un archivo separados en un vector de punteros a los mismos.
    entrenador_t* entrenador_prueba = entrenador_crear(datos_entrenador_prueba);

    pa2m_afirmar( entrenador_agregar_pokemon_leido(NULL, datos_pokemon_prueba)==FALLO , "No se puede agregar pokemon en un entrenador inexistente.");
    pa2m_afirmar( entrenador_agregar_pokemon_leido(entrenador_prueba, NULL)==FALLO , "No se puede agregar pokemon con datos inexistentes del mismo.");

    entrenador_destruir(entrenador_prueba);
    printf("\n");

}


void DadoEntrenadorInexistenteOSinPokemon_SiSePideTamanioDeEquipo_SeDevuelveCero(){

    char* datos_entrenador_prueba[2] = {"Miguel" , "30"};
    entrenador_t* entrenador_prueba = entrenador_crear(datos_entrenador_prueba);

    pa2m_afirmar( entrenador_tamanio_equipo(NULL)==0 , "El tamaño del equipo de un entrenador inexistente es cero.");
    pa2m_afirmar( entrenador_tamanio_equipo(entrenador_prueba)==0 , "El tamaño del equipo de un entrenador al que no se le asignó pokemones todavía es cero.");

    entrenador_destruir(entrenador_prueba);
    printf("\n");

}


void DadosParametrosValidos_AlAgregarPokemones_SeAgreganAlEquipoCorrectamente(){

    char* datos_entrenador_prueba[2] = {"Angel" , "20"};
    
    char* datos_pokemon_prueba_1[6] = {"Charizard" , "1" , "20" , "30" , "40" , "50"};
    char* datos_pokemon_prueba_2[6] = {"Bulbasaur" , "3" , "50" , "40" , "30" , "20"};
    char* datos_pokemon_prueba_3[6] = {"Squirtle" , "99" , "60" , "60" , "60" , "70"};

    entrenador_t* entrenador_prueba = entrenador_crear(datos_entrenador_prueba);

    pa2m_afirmar( entrenador_agregar_pokemon_leido(entrenador_prueba, datos_pokemon_prueba_1)==EXITO , "Se agregó un pokemon al equipo de un entrenador.");
    pa2m_afirmar( entrenador_tamanio_equipo(entrenador_prueba)==1 , "El tamaño del equipo del entrenador es 1.");

    pa2m_afirmar( entrenador_agregar_pokemon_leido(entrenador_prueba, datos_pokemon_prueba_2)==EXITO , "Se agregó otro pokemon al equipo.");
    pa2m_afirmar( entrenador_tamanio_equipo(entrenador_prueba)==2 , "El tamaño del equipo del entrenador es 2.");

    pa2m_afirmar( entrenador_agregar_pokemon_leido(entrenador_prueba, datos_pokemon_prueba_3)==EXITO , "Se agregó otro pokemon al equipo.");
    pa2m_afirmar( entrenador_tamanio_equipo(entrenador_prueba)==3 , "El tamaño del equipo del entrenador es 3.");

    entrenador_destruir(entrenador_prueba);
    printf("\n");

}



//////////// SALON_LEER_ARCHIVO ////////////

void DadoNombreDeArchivoInexistente_SiSePideLeerArchivo_SeDevuelveNull(){

    pa2m_afirmar( salon_leer_archivo("archivos_prueba/Holanoexisto.txt")==NULL , "No se puede crear un salón a partir de un archivo inexistente.")
    
    printf("\n");

}



void DadoArchivoCon1EntrenadorCon1Pokemon_AlLeerArchivo_SeCreaSalonCorrectamente(){

    salon_t* salon = salon_leer_archivo(SALON_1E_1P);

    pa2m_afirmar(salon!=NULL , "Se lee correctamente un archivo con 1 entrenador y 1 pokemon.");

    salon_destruir(salon);
    printf("\n");

}



void DadoArchivoCon1EntrenadorSinPokemon_AlLeerArchivo_SeDevuelveNull(){

    pa2m_afirmar(salon_leer_archivo(SALON_1E_SIN_P)==NULL , "Leer un archivo con un entrenador sin pokemon devuelve NULL.");

    printf("\n");

}



void DadoArchivoCon2EntrenadoresCon2PokemonCadaUno_AlLeerArchivo_SeCreaSalonCorrectamente(){

    salon_t* salon = salon_leer_archivo(SALON_2E_2P);

    pa2m_afirmar(salon!=NULL , "Se lee correctamente un archivo con 2 entrenadores de 2 pokemon c/u.");

    salon_destruir(salon);
    printf("\n");

}



void DadoArchivoConVariosEntrenadoresConAlgunosDeEllosSinPokemones_AlLeerArchivo_SeDevuelveNull(){

    pa2m_afirmar(salon_leer_archivo(SALON_6E_ALGUNOS_SIN_P)==NULL , "Leer un archivo con varios entrenadores, de los cuales algunos NO tienen pokemon devuelve NULL.");

    printf("\n");

}



void DadoArchivoCon2EntrenadoresConDistintaCantidadDePokemonesCadaUno_AlLeerArchivo_SeCreaSalonCorrectamente(){

    salon_t* salon = salon_leer_archivo(SALON_2E_CON_P_EN_DISTINTA_CANTIDAD);

    pa2m_afirmar(salon!=NULL , "Se lee correctamente un archivo con 2 entrenadores de distinta cantidad de pokemones c/u.");

    salon_destruir(salon);
    printf("\n");

}



void DadoArchivoConDosEntrenadoresRepetidos_SiSeIntentaLeer_SeDevuelveNull(){

    pa2m_afirmar( salon_leer_archivo(SALON_2E_IGUALES)==NULL , "No se puede leer archivo que contenga dos entrenadores de nombre idéntico.");

    printf("\n");

}



//////////// SALON_AGREGAR_ENTRENADOR ////////////

void DadoEntrenadorSinPokemonOParametrosInexistentes_SiSeIntentaAgregarEntrenador_SeDevuelveNull(){

    char* datos_entrenador_prueba[2] = {"Roberto" , "20"};
    entrenador_t* entrenador_prueba = entrenador_crear(datos_entrenador_prueba);
    salon_t* salon = salon_leer_archivo(SALON_1E_1P); //Se usa cualquier archivo válido, simplemente para tener un salón correctamente creado para probar.

    pa2m_afirmar(salon_agregar_entrenador(salon, NULL)==NULL , "No se puede agregar un entrenador inexistente a un salón.");
    pa2m_afirmar(salon_agregar_entrenador(NULL, entrenador_prueba)==NULL , "No se puede agregar un entrenador a un salón inexistente.");
    pa2m_afirmar(salon_agregar_entrenador(salon, entrenador_prueba)==NULL, "No se puede agregar un entrenador sin pokemones en un salón.");

    salon_destruir(salon);
    entrenador_destruir(entrenador_prueba);
    printf("\n");

}


void DadoEntrenadorConAlMenosUnPokemon_SiSeAgregaEntrenador_SeDevuelveElSalonConElEntrenadorAlmacenado(){

    char* datos_entrenador_prueba[2] = {"Francisco" , "20"};
    char* datos_pokemon_prueba_1[6] = {"Charizard" , "1" , "20" , "30" , "40" , "50"};
    char* datos_pokemon_prueba_2[6] = {"Bulbasaur" , "3" , "50" , "40" , "30" , "20"};
    char* datos_pokemon_prueba_3[6] = {"Squirtle" , "99" , "60" , "60" , "60" , "70"};

    salon_t* salon = salon_leer_archivo(SALON_1E_1P);
    entrenador_t* entrenador_prueba = entrenador_crear(datos_entrenador_prueba);
    entrenador_agregar_pokemon_leido(entrenador_prueba, datos_pokemon_prueba_1);

    salon = salon_agregar_entrenador(salon, entrenador_prueba);
    pa2m_afirmar(salon!=NULL, "Se agrega un entrenador con un pokemon en un salón correctamente.");
    salon_destruir(salon);


    salon = salon_leer_archivo(SALON_1E_1P);
    entrenador_prueba = entrenador_crear(datos_entrenador_prueba);
    entrenador_agregar_pokemon_leido(entrenador_prueba, datos_pokemon_prueba_1);
    entrenador_agregar_pokemon_leido(entrenador_prueba, datos_pokemon_prueba_2);
    entrenador_agregar_pokemon_leido(entrenador_prueba, datos_pokemon_prueba_3);

    salon = salon_agregar_entrenador(salon, entrenador_prueba);
    pa2m_afirmar(salon!=NULL, "Se agrega un entrenador con más de un pokemon en un salón correctamente.");
    salon_destruir(salon);

    printf("\n");

}



//////////// SALON_GUARDAR_ARCHIVO ////////////

void DadosParametrosInvalidos_SiSePideGuardarArchivo_SeDevuelveFallo(){


    salon_t* salon = salon_leer_archivo(SALON_1E_1P);

    pa2m_afirmar( salon_guardar_archivo(NULL, ARCHIVO_DE_PRUEBA_PARA_ESCRIBIR_1)==FALLO , "No se puede guardar un salon inexistente en un archivo.");
    pa2m_afirmar( salon_guardar_archivo(salon, NULL)==FALLO , "No se puede guardar un salon en un archivo con puntero a nombre NULL.");

    salon_destruir(salon);
    printf("\n");

}



void DadoSalonLeidoDeUnArchivoValido_SiSeGuardaEnArchivo_ElArchivoGuardadoEsIgualQueElOriginal(){

    salon_t* salon = salon_leer_archivo(SALON_2E_CON_P_EN_DISTINTA_CANTIDAD);

    pa2m_afirmar( salon_guardar_archivo(salon, ARCHIVO_DE_PRUEBA_PARA_ESCRIBIR_1)==2 , "Se guardó un salón con 2 entrenadores en un archivo.");
    //Para ver si el archivo resultante es igual simplemente se abren los dos archivos (el leido y el escrito) y se comparan.
    salon_destruir(salon);

    salon = salon_leer_archivo(ARCHIVO_DE_PRUEBA_PARA_ESCRIBIR_1);
    pa2m_afirmar(salon != NULL , "Se puede leer el archivo recién escrito correctamente.");
    salon_destruir(salon);
    printf("\n");

}



void DadosSalonesValidosEnGeneral_SiSeGuardanEnArchivos_SeGuardanTodosCorrectamente(){

    salon_t* salon = salon_leer_archivo(SALON_1E_1P);

    char* datos_entrenador_prueba_1[2] = {"Leo" , "20"};
    char* datos_pokemon_prueba_1[6] = {"Charizard" , "1" , "20" , "30" , "40" , "50"};
    
    char* datos_entrenador_prueba_2[2] = {"Lucas" , "30"};
    char* datos_pokemon_prueba_2[6] = {"Bulbasaur" , "3" , "50" , "40" , "30" , "20"};
    char* datos_pokemon_prueba_3[6] = {"Squirtle" , "99" , "60" , "60" , "60" , "70"};

    entrenador_t* entrenador_prueba_1 = entrenador_crear(datos_entrenador_prueba_1);
    entrenador_agregar_pokemon_leido(entrenador_prueba_1, datos_pokemon_prueba_1);

    entrenador_t* entrenador_prueba_2 = entrenador_crear(datos_entrenador_prueba_2);
    entrenador_agregar_pokemon_leido(entrenador_prueba_2, datos_pokemon_prueba_2);
    entrenador_agregar_pokemon_leido(entrenador_prueba_2, datos_pokemon_prueba_3);

    salon = salon_agregar_entrenador(salon, entrenador_prueba_1);
    salon = salon_agregar_entrenador(salon, entrenador_prueba_2);

    pa2m_afirmar( salon_guardar_archivo(salon, ARCHIVO_DE_PRUEBA_PARA_ESCRIBIR_2)==3 , "Se guardó un salón con 3 entrenadores en un archivo, uno de ellos leido de un archivo y los demás agregados adicionalmente.");
    //Para ver si el archivo resultante es correcto se abre el archivo escrito 2.

    salon_destruir(salon);

    salon = salon_leer_archivo(ARCHIVO_DE_PRUEBA_PARA_ESCRIBIR_2);
    pa2m_afirmar(salon != NULL , "Se puede leer el archivo recién escrito correctamente.");
    salon_destruir(salon);

    printf("\n");

}




//////////// SALON_FILTRAR_ENTRENADORES ////////////

/**
 * Función auxiliar de prueba.
 * Devuelve true si el entrenador recibido tiene victorias mayores o iguales a las mínimas recibidas.
 * Devuelve false en caso contrario.
*/
bool filtrar_por_victorias_minimas(entrenador_t* entrenador, void* victorias_minimas){

    int* victorias_min = victorias_minimas;

    return (entrenador_cantidad_victorias(entrenador) >= (*victorias_min));

}



void DadosParametrosInvalidos_SiSeIntentaFiltrar_SeDevuelveNull(){

    salon_t* salon = salon_leer_archivo(SALON_1E_1P);
    int victorias_min_prueba = 10;

    pa2m_afirmar( salon_filtrar_entrenadores(NULL, filtrar_por_victorias_minimas, &victorias_min_prueba)==NULL , "No se puede filtrar entrenadores de un salon inexistente.");
    pa2m_afirmar( salon_filtrar_entrenadores(salon, NULL, &victorias_min_prueba)==NULL , "No se puede filtrar entrenadores con una función de filtro inexistente.");
    //pa2m_afirmar( salon_filtrar_entrenadores(salon, filtrar_por_victorias_minimas, NULL)==NULL , "No se puede filtrar entrenadores con el parámetro extra inexistente.");

    salon_destruir(salon);
    printf("\n");

}



void DadaFuncionDeFiltroQueSeCumpleParaTodoEntrenador_AlFiltrar_SeDevuelveListaConTodosLosEntrenadoresDelSalon(){

    salon_t* salon = salon_leer_archivo(SALON_2E_CON_P_EN_DISTINTA_CANTIDAD);
    int victorias_min_prueba = 0; //Cualquier entrenador va a tener mínimo cero victorias, se cumple siempre.

    lista_t* lista_filtrados = salon_filtrar_entrenadores(salon, filtrar_por_victorias_minimas, &victorias_min_prueba);
    entrenador_t* entrenador_1 = lista_elemento_en_posicion(lista_filtrados, 0);
    entrenador_t* entrenador_2 = lista_elemento_en_posicion(lista_filtrados, 1);
    //Deberían estar en ese orden en la lista (en el archivo 2P_CON_P... hay dos entrenadores. Uno de ellos se llama "entrenador1" y el otro "entrenador2". "entrenador1" es menor alfabeticamente que "entrenador2").

    bool entrenador_1_es_correcto = (entrenador_tamanio_equipo(entrenador_1)==3 && entrenador_cantidad_victorias(entrenador_1)==15);
    bool entrenador_2_es_correcto = (entrenador_tamanio_equipo(entrenador_2)==1 && entrenador_cantidad_victorias(entrenador_2)==14);

    pa2m_afirmar(lista_filtrados!=NULL , "Si la función de filtro se cumple para todos los entrenadores, se devuelve una lista no NULL (no hubo errores).");
    pa2m_afirmar( entrenador_1_es_correcto , "El primer entrenador de la lista es el correcto.");
    pa2m_afirmar( entrenador_2_es_correcto , "El segundo entrenador de la lista es el correcto.");
    pa2m_afirmar(entrenador_1_es_correcto && entrenador_2_es_correcto && (lista_elementos(lista_filtrados)==2) , "Se guardaron en la lista todos los entrenadores.");

    salon_destruir(salon);
    lista_destruir(lista_filtrados);
    printf("\n");

}



void DadaFuncionDeFiltroQueNoSeCumpleParaNingunEntrenador_AlFiltrar_SeDevuelveUnaListaVacia(){

    salon_t* salon = salon_leer_archivo(SALON_2E_CON_P_EN_DISTINTA_CANTIDAD);
    int victorias_min_prueba = 9999; //Ningun entrenador en el archivo tiene tal numero de victorias mínimas.

    lista_t* lista_filtrados = salon_filtrar_entrenadores(salon, filtrar_por_victorias_minimas, &victorias_min_prueba);

    pa2m_afirmar(lista_filtrados!=NULL , "Si la función de filtro no se cumple para ningún entrenador, se devuelve una lista no NULL (no hubo errores).");
    pa2m_afirmar(lista_vacia(lista_filtrados)==true , "La lista devuelta está vacía.");

    salon_destruir(salon);
    lista_destruir(lista_filtrados);
    printf("\n");

}



void DadaFuncionDeFiltroQueSeCumpleParaAlgunosEntrenadores_AlFiltrar_SeDevuelveListaConLosFiltrados(){

    salon_t* salon = salon_leer_archivo(SALON_2E_CON_P_EN_DISTINTA_CANTIDAD);
    int victorias_min_prueba = 15; 

    char* datos_entrenador_extra[2] = {"Berni" , "20"}; //Se añade un entrenador a los 2 que había en el archivo leido para tener 2 entrenadores que cumplan con el filtro y uno que no.
    char* datos_pokemon_prueba_1[6] = {"Charizard" , "1" , "20" , "30" , "40" , "50"};
    char* datos_pokemon_prueba_2[6] = {"Bulbasaur" , "3" , "50" , "40" , "30" , "20"};

    entrenador_t* entrenador_extra = entrenador_crear(datos_entrenador_extra);
    entrenador_agregar_pokemon_leido(entrenador_extra, datos_pokemon_prueba_1);
    entrenador_agregar_pokemon_leido(entrenador_extra, datos_pokemon_prueba_2);
    salon = salon_agregar_entrenador(salon, entrenador_extra);

    lista_t* lista_filtrados = salon_filtrar_entrenadores(salon, filtrar_por_victorias_minimas, &victorias_min_prueba);
    entrenador_t* entrenador_Berni = lista_elemento_en_posicion(lista_filtrados, 0);
    entrenador_t* entrenador_1 = lista_elemento_en_posicion(lista_filtrados, 1);
    //El orden esperado es: "entrenador1" en la posición 1 de la tabla y "Berni" en la posición 0 ya que es menor alfabéticamente.

    bool entrenador_Berni_es_correcto = (entrenador_tamanio_equipo(entrenador_Berni)==2 && entrenador_cantidad_victorias(entrenador_Berni)==20);
    bool entrenador_1_es_correcto = (entrenador_tamanio_equipo(entrenador_1)==3 && entrenador_cantidad_victorias(entrenador_1)==15);

    pa2m_afirmar(lista_filtrados!=NULL , "Si la función de filtro se cumple para algunos de los entrenadores, se devuelve una lista no NULL (no hubo errores).");
    pa2m_afirmar( lista_elementos(lista_filtrados)==2 , "La cantidad de entrenadores almacenados en la lista es la correcta.")
    pa2m_afirmar( entrenador_Berni_es_correcto , "El primer entrenador de la lista es el correcto.");
    pa2m_afirmar( entrenador_1_es_correcto , "El segundo entrenador de la lista es el correcto.");

    salon_destruir(salon);
    lista_destruir(lista_filtrados);
    printf("\n");

}





//////////// SALON_EJECUTAR_COMANDO ////////////

void DadosParametrosInvalidos_SiSeIntentaEjecutarComando_SeDevuelveNull(){



    printf("\n");

}


/*

void Dado_Si_Se(){



    printf("\n");

}



void Dado_Si_Se(){



    printf("\n");

}



void Dado_Si_Se(){



    printf("\n");

}



void Dado_Si_Se(){



    printf("\n");

}



void Dado_Si_Se(){



    printf("\n");

}



void Dado_Si_Se(){



    printf("\n");

}


*/



/*

void Dado_Si_Se(){

    

    printf("\n");

}

*/




int main(){

    pa2m_nuevo_grupo("//  PRUEBAS DE DESTRUCCIÓN DE ELEMENTOS EN LISTA  //"); //Debido a cambios efectuados en lista.h y lista.c (Ver README.txt).
        pa2m_nuevo_grupo("Pruebas de creación de lista (modificación)");
            SinImportarSiSeDaONoSeDaDestructor_AlCrearLista_SeCreaCorrectamente();

        pa2m_nuevo_grupo("Pruebas de destrucción de lista (modificación)");
            DadaListaConDestructor_AlDestruirLista_SeDestruyenCorrectamenteTodosLosElementos();

        pa2m_nuevo_grupo("Pruebas de borrado en lista (modificación)");
            DadaListaConDestructor_AlBorrarElementos_SeBorranCorrectamente();

    //===============================================================================//
    pa2m_nuevo_grupo("//  PRUEBAS DE ENTRENADOR  //");
        pa2m_nuevo_grupo("Pruebas de creación de entrenador");
            DadosDatosInexistentes_SiSePideCrearEntrenador_SeDevuelveNull();
            DadosDatos_AlCrearEntrenador_SeCreaEntrenadorCorrectamente();
        
        pa2m_nuevo_grupo("Pruebas de agregar pokemon y tamaño de equipo");
            DadoEntrenadorODatosPokemonInexistentes_SiSePideAgregarPokemon_SeDevuelveFallo();
            DadoEntrenadorInexistenteOSinPokemon_SiSePideTamanioDeEquipo_SeDevuelveCero();
            DadosParametrosValidos_AlAgregarPokemones_SeAgreganAlEquipoCorrectamente();
  
    //===============================================================================//
    pa2m_nuevo_grupo("//  PRUEBAS DE SALON  //");
        pa2m_nuevo_grupo("Pruebas de lectura de archivos");
            DadoNombreDeArchivoInexistente_SiSePideLeerArchivo_SeDevuelveNull();
            DadoArchivoCon1EntrenadorCon1Pokemon_AlLeerArchivo_SeCreaSalonCorrectamente();
            DadoArchivoCon1EntrenadorSinPokemon_AlLeerArchivo_SeDevuelveNull();
            DadoArchivoCon2EntrenadoresCon2PokemonCadaUno_AlLeerArchivo_SeCreaSalonCorrectamente();
            DadoArchivoConVariosEntrenadoresConAlgunosDeEllosSinPokemones_AlLeerArchivo_SeDevuelveNull();
            DadoArchivoCon2EntrenadoresConDistintaCantidadDePokemonesCadaUno_AlLeerArchivo_SeCreaSalonCorrectamente();
            DadoArchivoConDosEntrenadoresRepetidos_SiSeIntentaLeer_SeDevuelveNull();

        pa2m_nuevo_grupo("Pruebas de agregar entrenador");
            DadoEntrenadorSinPokemonOParametrosInexistentes_SiSeIntentaAgregarEntrenador_SeDevuelveNull();
            DadoEntrenadorConAlMenosUnPokemon_SiSeAgregaEntrenador_SeDevuelveElSalonConElEntrenadorAlmacenado();

        pa2m_nuevo_grupo("Pruebas de guardar archivo");
            DadosParametrosInvalidos_SiSePideGuardarArchivo_SeDevuelveFallo();
            DadoSalonLeidoDeUnArchivoValido_SiSeGuardaEnArchivo_ElArchivoGuardadoEsIgualQueElOriginal();
            DadosSalonesValidosEnGeneral_SiSeGuardanEnArchivos_SeGuardanTodosCorrectamente();

        pa2m_nuevo_grupo("Pruebas de filtrar entrenadores");
            DadosParametrosInvalidos_SiSeIntentaFiltrar_SeDevuelveNull();
            DadaFuncionDeFiltroQueSeCumpleParaTodoEntrenador_AlFiltrar_SeDevuelveListaConTodosLosEntrenadoresDelSalon();
            DadaFuncionDeFiltroQueNoSeCumpleParaNingunEntrenador_AlFiltrar_SeDevuelveUnaListaVacia();
            DadaFuncionDeFiltroQueSeCumpleParaAlgunosEntrenadores_AlFiltrar_SeDevuelveListaConLosFiltrados();

        pa2m_nuevo_grupo("Pruebas de ejecución de comandos");
            DadosParametrosInvalidos_SiSeIntentaEjecutarComando_SeDevuelveNull();
            /*
            Dado_Si_Se();
            Dado_Si_Se();
            Dado_Si_Se();
            Dado_Si_Se();
            Dado_Si_Se();
            Dado_Si_Se();
            */

/*
        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();
*/

    return pa2m_mostrar_reporte();
}
