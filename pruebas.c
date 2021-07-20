#include "pa2mm.h"
#include "src/salon.h"
#include "src/util.h"


#define FALLO -1
#define EXITO 0

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
    char* datos_entrenador_prueba[2] = {"Mauro" , "20"};
    entrenador_t* entrenador_prueba = entrenador_crear(datos_entrenador_prueba);

    pa2m_afirmar( entrenador_agregar_pokemon(NULL, datos_pokemon_prueba)==FALLO , "No se puede agregar pokemon en un entrenador inexistente.");
    pa2m_afirmar( entrenador_agregar_pokemon(entrenador_prueba, NULL)==FALLO , "No se puede agregar pokemon con datos inexistentes del mismo.");

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

    pa2m_afirmar( entrenador_agregar_pokemon(entrenador_prueba, datos_pokemon_prueba_1)==EXITO , "Se agregó un pokemon al equipo de un entrenador.");
    pa2m_afirmar( entrenador_tamanio_equipo(entrenador_prueba)==1 , "El tamaño del equipo del entrenador es 1.");

    pa2m_afirmar( entrenador_agregar_pokemon(entrenador_prueba, datos_pokemon_prueba_2)==EXITO , "Se agregó otro pokemon al equipo.");
    pa2m_afirmar( entrenador_tamanio_equipo(entrenador_prueba)==2 , "El tamaño del equipo del entrenador es 2.");

    pa2m_afirmar( entrenador_agregar_pokemon(entrenador_prueba, datos_pokemon_prueba_3)==EXITO , "Se agregó otro pokemon al equipo.");
    pa2m_afirmar( entrenador_tamanio_equipo(entrenador_prueba)==3 , "El tamaño del equipo del entrenador es 3.");

    entrenador_destruir(entrenador_prueba);
    printf("\n");

}



//////////// SALON_LEER_ARCHIVO ////////////

void DadoNombreDeArchivoInexistente_SiSePideLeerArchivo_SeDevuelveNull(){


    printf("\n");

}



void DadoArchivoCon1EntrenadorCon1Pokemon_AlLeerArchivo_SeCreaSalonCorrectamente(){


    printf("\n");

}



void DadoArchivoCon1EntrenadorSinPokemon_AlLeerArchivo_SeDevuelveNull(){


    printf("\n");

}



void DadoArchivoCon2EntrenadoresCon2PokemonCadaUno_AlLeerArchivo_SeCreaSalonCorrectamente(){


    printf("\n");

}



void DadoArchivoConVariosEntrenadoresConAlgunosDeEllosSinPokemones_AlLeerArchivo_SeDevuelveNull(){


    printf("\n");

}




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


/*
    //===============================================================================//
    pa2m_nuevo_grupo("//  PRUEBAS DE COMANDOS  //");
        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

*/
    //===============================================================================//
    pa2m_nuevo_grupo("//  PRUEBAS DE SALON  //");
        pa2m_nuevo_grupo("Pruebas de lectura de archivos");
            DadoNombreDeArchivoInexistente_SiSePideLeerArchivo_SeDevuelveNull();
            DadoArchivoCon1EntrenadorCon1Pokemon_AlLeerArchivo_SeCreaSalonCorrectamente();
            DadoArchivoCon1EntrenadorSinPokemon_AlLeerArchivo_SeDevuelveNull();
            DadoArchivoCon2EntrenadoresCon2PokemonCadaUno_AlLeerArchivo_SeCreaSalonCorrectamente();
            DadoArchivoConVariosEntrenadoresConAlgunosDeEllosSinPokemones_AlLeerArchivo_SeDevuelveNull();
            
/*
        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();
*/

    return pa2m_mostrar_reporte();
}
