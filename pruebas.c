#include "pa2mm.h"
#include "src/salon.h"

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


    pa2m_afirmar( lista_borrar(lista) == 0 , "Se borró el último elemento de una lista con destructor.");
    pa2m_afirmar( (seis==7) && (cinco==5) && (cuatro==4) && (tres==3) && (dos==2) && (uno==1), "Dicho elemento (y solo ese) fue destruido correctamente.");

    pa2m_afirmar( lista_borrar_de_posicion(lista, 3) == 0 , "Se borró elemento de una posición intermedia.");
    pa2m_afirmar( (cuatro==5) && (cinco==5) && (tres==3) && (dos==2) && (uno==1), "Dicho elemento (y solo ese) fue destruido correctamente.");

    pa2m_afirmar( lista_borrar(lista) == 0 , "Se volvió a borrar el último elemento.");
    pa2m_afirmar( (cinco==6) && (tres==3) && (dos==2) && (uno==1), "Dicho elemento (y solo ese) fue destruido correctamente.");

    pa2m_afirmar( lista_borrar_de_posicion(lista, 0) == 0 , "Se borró elemento de la primera posición.");
    pa2m_afirmar( (uno==2) && (tres==3) && (dos==2), "Dicho elemento (y solo ese) fue destruido correctamente.");

    lista_destruir(lista);
    pa2m_afirmar((dos==3) && (tres==4) , "Se aplicó el destructor a todos los elementos restantes al destruir la lista.");
    printf("\n");

}




//////////// CREACION DE ENTRENADOR ////////////

void DadoNombreNull_SiSePideCrearEntrenador_SeDevuelveNull(){

    pa2m_afirmar( entrenador_crear(NULL, 10) == NULL , "No se puede crear un entrenador a partir de nombre inexistente.");

    printf("\n");

}


void DadoUnNombre_AlCrearEntrenador_SeCreaEntrenadorCorrectamente(){

    char* nombres_de_prueba[2] = {"Entre1" , "Entre2"};
    entrenador_t* _entrenador_1 = entrenador_crear(nombres_de_prueba[0], 35);
    pa2m_afirmar( _entrenador_1 != NULL , "Se creó un entrenador correctamente.");

    entrenador_t* _entrenador_2 = entrenador_crear(nombres_de_prueba[1], 0);
    pa2m_afirmar( _entrenador_2 != NULL , "Se creó un entrenador sin victorias correctamente.");

    entrenador_destruir(_entrenador_1);
    entrenador_destruir(_entrenador_2);
 
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
            DadoNombreNull_SiSePideCrearEntrenador_SeDevuelveNull();
            DadoUnNombre_AlCrearEntrenador_SeCreaEntrenadorCorrectamente();


    /*
    //===============================================================================//
    pa2m_nuevo_grupo("//  PRUEBAS DE COMANDOS  //");
        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

    //===============================================================================//
    pa2m_nuevo_grupo("//  PRUEBAS DE SALON  //");
        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

        pa2m_nuevo_grupo("Pruebas de");
            Dado_Si_Se();

    */

    return pa2m_mostrar_reporte();
}
