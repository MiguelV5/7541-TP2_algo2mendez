#include "abb.h"
#include <stdio.h>
#include <stdbool.h>

#define FALLO -1
#define EXITO 0



/**
 * Destruye un elemento (en caso de tener destructor) y hace free del nodo que lo contenía.
*/
void aniquilar_nodo(nodo_abb_t* nodo, abb_liberar_elemento destructor){

    if(destructor != NULL){
        destructor(nodo->elemento);
    }
    free(nodo);

}


abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

    if(!comparador){
        return NULL;
    }

    abb_t* abb = malloc(1*sizeof(abb_t));
    abb->comparador = comparador;
    abb->destructor = destructor;
    abb->nodo_raiz = NULL;

    return abb;

}

/**
 * Inserta un nodo nuevo con el elemento requerido a un arbol usando el comparador como
 * criterio para posicionarlo donde corresponde. 
 * Tiene comportamiento recursivo, por lo que puede devolver:
 *  -NULL en caso de falla al reservar el nodo nuevo.
 *  -El nodo del arbol sobre el cual se encuentra actualmente posicionado.
 * Debido a éste último, al finalizar TODOS los retornos recursivos se devuelve la raíz del abb. 
*/
nodo_abb_t* insercion_de_nodo(nodo_abb_t* nodo_actual, abb_comparador comparador, void* elemento){

    if(nodo_actual == NULL){

        nodo_actual = calloc(1, sizeof(nodo_abb_t));
        if(!nodo_actual){
            return NULL;
        }

        nodo_actual->elemento = elemento;
        return nodo_actual;

    }

    int comparacion_de_elementos = comparador(elemento, nodo_actual->elemento);

    //Convención tomada: Si el elemento a insertar es repetido, se inserta a la izquierda del mismo.
    if(comparacion_de_elementos <= 0){ 

        nodo_actual->izquierda = insercion_de_nodo(nodo_actual->izquierda, comparador, elemento);
    
    }
    else if(comparacion_de_elementos > 0){

        nodo_actual->derecha = insercion_de_nodo(nodo_actual->derecha, comparador, elemento);

    }

    return nodo_actual;

}

int arbol_insertar(abb_t* arbol, void* elemento){

    if(!arbol){
        return FALLO;
    }

    nodo_abb_t* raiz_tras_insercion = insercion_de_nodo(arbol->nodo_raiz , arbol->comparador, elemento);

    if(raiz_tras_insercion != NULL){
        arbol->nodo_raiz = raiz_tras_insercion;
        return EXITO;
    }
    else{
        return FALLO;
    }

}




/**
 * Saca del sub-arbol izquierdo al nodo más derecho que se tenga.
 * (Por ende requiere que, en su primer llamado, se le pase la raíz de dicho sub-arbol como nodo_actual)
 * Ese 'nodo más derecho' se guarda como predecesor.
 * Al finalizar TODOS los retornos recursivos se devuelve la raíz del sub-arbol.
 * (Notar que si no existía el sub-arbol, esa raíz tampoco, por lo tanto se devuelve NULL).
*/
nodo_abb_t* extraccion_de_predecesor(nodo_abb_t* nodo_actual, nodo_abb_t** predecesor){

    if(!nodo_actual){
        return NULL;
    }

    if(nodo_actual->derecha != NULL){

        nodo_actual->derecha = extraccion_de_predecesor(nodo_actual->derecha, predecesor);

    }
    else{ //El nodo actual es el más derecho que hay --->  A lo sumo puede tener hijo izquierdo.

        nodo_abb_t* posible_hijo_izquierdo = nodo_actual->izquierda; 
        (*predecesor) = nodo_actual;

        return posible_hijo_izquierdo;

    }

    return nodo_actual;

}

/**
 * Busca el nodo al que pertenece el elemento pedido y lo borra si lo logra encontrar.
 * Adicionalmente, si se pasó un destructor válido, destruye el elemento antes de borrar el nodo.
 * Puede devolver:
 *  -NULL en caso de que el elemento no pertenecía al arbol. En tal caso el arbol no se modifica.
 *  -El nodo del arbol sobre el cual se encuentra actualmente posicionado.
 * Debido a esto último, al finalizar TODOS los retornos recursivos se devuelve la raíz del abb modificado.  
*/
nodo_abb_t* borrador_de_nodo(nodo_abb_t* nodo_actual, abb_comparador comparador, abb_liberar_elemento destructor, void* elemento, bool* se_pudo_borrar){

    if(!nodo_actual){
        (*se_pudo_borrar) = false;
        return NULL;
    }

    int comparacion = comparador(elemento, nodo_actual->elemento);

    if(comparacion < 0){

        nodo_actual->izquierda = borrador_de_nodo(nodo_actual->izquierda, comparador, destructor, elemento, se_pudo_borrar);
    
    }
    else if(comparacion > 0){

        nodo_actual->derecha = borrador_de_nodo(nodo_actual->derecha, comparador, destructor, elemento, se_pudo_borrar);

    }
    else if(comparacion == 0){

        nodo_abb_t* predecesor = NULL;
        nodo_actual->izquierda = extraccion_de_predecesor(nodo_actual->izquierda, &predecesor);

        if(predecesor == NULL){ //Si no hay predecesor, entonces el nodo a borrar no tenía hijo izq.

            nodo_abb_t* posible_hijo_derecho = nodo_actual->derecha;//Pero podría tener hijo derecho. 
            aniquilar_nodo(nodo_actual, destructor);

            return posible_hijo_derecho;
        
        }
        else{

            predecesor->izquierda = nodo_actual->izquierda;
            predecesor->derecha = nodo_actual->derecha;
            aniquilar_nodo(nodo_actual, destructor);

            return predecesor;

        }

    }

    return nodo_actual;

}


int arbol_borrar(abb_t* arbol, void* elemento){

    if(arbol_vacio(arbol)){
        return FALLO;
    }

    bool se_pudo_borrar = true;

    nodo_abb_t* raiz_tras_borrado = borrador_de_nodo(arbol->nodo_raiz, arbol->comparador, arbol->destructor, elemento, &se_pudo_borrar);

    arbol->nodo_raiz = raiz_tras_borrado;
    
    if(se_pudo_borrar){
        return EXITO;
    }
    else{
        return FALLO;
    }

}




/**
 * Intenta encontrar 'elemento_buscado' entre los nodos del arbol usando el comparador como
 * criterio para guiar la busqueda. 
 *  -Si logra encontrarlo, se devuelve el puntero al elemento encontrado.
 *  -Si no lo encuentra, se devuelve NULL.
*/
void* busqueda_en_nodos(nodo_abb_t* nodo_actual, abb_comparador comparador, void* elemento_buscado, void* elemento_encontrado){

    if(nodo_actual == NULL){
        return NULL;
    }

    int comparacion = comparador(elemento_buscado, nodo_actual->elemento);

    if(comparacion < 0){

        elemento_encontrado = busqueda_en_nodos(nodo_actual->izquierda, comparador, elemento_buscado, elemento_encontrado);
    
    }
    else if(comparacion > 0){

        elemento_encontrado = busqueda_en_nodos(nodo_actual->derecha, comparador, elemento_buscado, elemento_encontrado);

    }
    else if(comparacion == 0){

        elemento_encontrado = nodo_actual->elemento;

    }
    
    return elemento_encontrado;

}


void* arbol_buscar(abb_t* arbol, void* elemento){

    if(arbol_vacio(arbol)){
        return NULL;
    }

    void* elemento_encontrado = NULL; 

    elemento_encontrado = busqueda_en_nodos(arbol->nodo_raiz, arbol->comparador, elemento, elemento_encontrado);

    return elemento_encontrado;

}




void* arbol_raiz(abb_t* arbol){

    if(arbol_vacio(arbol)){
        return NULL;
    }

    return arbol->nodo_raiz->elemento;

}




bool arbol_vacio(abb_t* arbol){

    if(!arbol){
        return true;
    }
    else if(arbol->nodo_raiz == NULL){
        return true;
    }
    else{
        return false;
    }

}




/**
 * Almacena los punteros a elementos de un arbol recorriendo sus nodos de forma INORDEN en un vector
 * (quedan guardados de forma ascendente).
 * Dicho guardado se hace hasta que se llene el vector con el tamaño pedido, o hasta que se acaben
 * los elementos del arbol.
 * La cantidad de elementos guardados se va aumentando cada que se agrega un elemento al vector
*/
void almacenar_por_nodos_inorden(nodo_abb_t* nodo_actual, void** array, size_t tamanio_array, size_t* elementos_guardados){
    
    if((*elementos_guardados) == tamanio_array || (nodo_actual == NULL)){
        return;
    }
     
    almacenar_por_nodos_inorden(nodo_actual->izquierda, array, tamanio_array, elementos_guardados);

    if((*elementos_guardados) == tamanio_array){
        return;
    }

    array[(*elementos_guardados)] = nodo_actual->elemento;
    (*elementos_guardados)++;

    almacenar_por_nodos_inorden(nodo_actual->derecha, array, tamanio_array, elementos_guardados);
    
}


size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){

    if(arbol_vacio(arbol) || !array || (tamanio_array == 0)){
        return 0;
    }

    size_t elementos_almacenados = 0;
    almacenar_por_nodos_inorden(arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados);

    return elementos_almacenados;

}




/**
 * Almacena los punteros a elementos de un arbol recorriendo sus nodos de forma PREORDEN en un vector.
 * Dicho guardado se hace hasta que se llene el vector con el tamaño pedido, o hasta que se acaben
 * los elementos del arbol.
 * La cantidad de elementos guardados se va aumentando cada que se agrega un elemento al vector
*/
void almacenar_por_nodos_preorden(nodo_abb_t* nodo_actual, void** array, size_t tamanio_array, size_t* elementos_guardados){
    
    if((*elementos_guardados) == tamanio_array || (nodo_actual == NULL)){
        return;
    }
     
    array[(*elementos_guardados)] = nodo_actual->elemento;
    (*elementos_guardados)++;

    if((*elementos_guardados) == tamanio_array){
        return;
    }
    
    almacenar_por_nodos_preorden(nodo_actual->izquierda, array, tamanio_array, elementos_guardados);

    almacenar_por_nodos_preorden(nodo_actual->derecha, array, tamanio_array, elementos_guardados);
    
}


size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){

    if(arbol_vacio(arbol) || !array || (tamanio_array == 0)){
        return 0;
    }

    size_t elementos_almacenados = 0;
    almacenar_por_nodos_preorden(arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados);

    return elementos_almacenados;

}




/**
 * Almacena los punteros a elementos de un arbol recorriendo sus nodos de forma POSTORDEN en un vector.
 * Dicho guardado se hace hasta que se llene el vector con el tamaño pedido, o hasta que se acaben
 * los elementos del arbol.
 * La cantidad de elementos guardados se va aumentando cada que se agrega un elemento al vector
*/
void almacenar_por_nodos_postorden(nodo_abb_t* nodo_actual, void** array, size_t tamanio_array, size_t* elementos_guardados){
    
    if((*elementos_guardados) == tamanio_array || (nodo_actual == NULL)){
        return;
    }
     
    almacenar_por_nodos_postorden(nodo_actual->izquierda, array, tamanio_array, elementos_guardados);

    if((*elementos_guardados) == tamanio_array){
        return;
    }
    
    almacenar_por_nodos_postorden(nodo_actual->derecha, array, tamanio_array, elementos_guardados);

    if((*elementos_guardados) == tamanio_array){
        return;
    }
    
    array[(*elementos_guardados)] = nodo_actual->elemento;
    (*elementos_guardados)++;
    
}


size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){
    
    if(arbol_vacio(arbol) || !array || (tamanio_array == 0)){
        return 0;
    }

    size_t elementos_almacenados = 0;
    almacenar_por_nodos_postorden(arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados);

    return elementos_almacenados;

}





/**
 * Libera todos los nodos de un arbol. En caso de tener destructor, destruye
 * a su vez a cada elemento.
*/
void destruir_nodos(nodo_abb_t* nodo_actual, abb_liberar_elemento destructor){

    if(nodo_actual == NULL){
        return;
    }
    else if(nodo_actual != NULL){
        destruir_nodos(nodo_actual->izquierda, destructor);
        destruir_nodos(nodo_actual->derecha, destructor);
    }

    aniquilar_nodo(nodo_actual, destructor);

}


void arbol_destruir(abb_t* arbol){

    if(!arbol){
        return;
    }

    destruir_nodos(arbol->nodo_raiz, arbol->destructor);
    free(arbol);

}





/**
 * Devuelve true si los parametros pasados al iterador interno son inválidos para usarlo, false en caso
 * contrario.
*/
bool parametros_iterador_son_invalidos(abb_t* abb, int recorrido, bool (*funcion)(void*, void*)){
    
    if( arbol_vacio(abb) || (funcion == NULL) || ((recorrido != ABB_RECORRER_INORDEN) && (recorrido != ABB_RECORRER_PREORDEN) && (recorrido != ABB_RECORRER_POSTORDEN)) ){
        return true;
    }
    else{
        return false;
    }

}


/**
 * Aplica la función dada a cada elemento de un arbol. Dicha aplicación se realiza recorriendo INORDEN.
 * La función se aplica mientras que devuelva false. Si devuelve true se detiene la aplicación.
 * La cantidad de elementos modificados se aumenta en medida en que se aplica la misma.
 * La variable 'dejar_de_aplicar' sirve para verificar que no se aplique nuevamente la función tras haber
 * obtenido aunque sea UNA vez true por resultado anteriormente.
*/
void aplicar_con_cada_elemento_inorden(nodo_abb_t* nodo_actual, bool (*funcion)(void*, void*), void* extra, size_t* cantidad_modificados, bool* dejar_de_aplicar){
    
    if((nodo_actual == NULL) || ((*dejar_de_aplicar) == true)){
        return;
    }
        
    aplicar_con_cada_elemento_inorden(nodo_actual->izquierda, funcion, extra, cantidad_modificados, dejar_de_aplicar);
    
    if((*dejar_de_aplicar) == true){
        return;
    }

    (*dejar_de_aplicar) = funcion(nodo_actual->elemento, extra);
    (*cantidad_modificados)++;

    if((*dejar_de_aplicar) == true){ //La doble verificación puede PARECER innecesaria, pero analizando como se comporta, no lo es. Más a detalle en explicación de esta función, README.txt
        return;
    }

    aplicar_con_cada_elemento_inorden(nodo_actual->derecha, funcion, extra, cantidad_modificados, dejar_de_aplicar);
    
}


/**
 * Aplica la función dada a cada elemento de un arbol. Dicha aplicación se realiza recorriendo PREORDEN.
 * La función se aplica mientras que devuelva false. Si devuelve true se detiene la aplicación.
 * La cantidad de elementos modificados se aumenta en medida en que se aplica la misma.
 * La variable 'dejar_de_aplicar' sirve para verificar que no se aplique nuevamente la función tras haber
 * obtenido aunque sea UNA vez true por resultado anteriormente.
*/
void aplicar_con_cada_elemento_preorden(nodo_abb_t* nodo_actual, bool (*funcion)(void*, void*), void* extra, size_t* cantidad_modificados, bool* dejar_de_aplicar){
    
    if((nodo_actual == NULL) || ((*dejar_de_aplicar) == true)){
        return;
    }

    (*dejar_de_aplicar) = funcion(nodo_actual->elemento, extra);
    (*cantidad_modificados)++;

    if((*dejar_de_aplicar) == true){ //Mismo comentario, estas últimas dos verific. son NECESARIAS.
        return;
    }
        
    aplicar_con_cada_elemento_preorden(nodo_actual->izquierda, funcion, extra, cantidad_modificados, dejar_de_aplicar);
    
    if((*dejar_de_aplicar) == true){
        return;
    }

    aplicar_con_cada_elemento_preorden(nodo_actual->derecha, funcion, extra, cantidad_modificados, dejar_de_aplicar);
    
}


/**
 * Aplica la función dada a cada elemento de un arbol. Dicha aplicación se realiza recorriendo POSTORDEN.
 * La función se aplica mientras que devuelva false. Si devuelve true se detiene la aplicación.
 * La cantidad de elementos modificados se aumenta en medida en que se aplica la misma.
 * La variable 'dejar_de_aplicar' sirve para verificar que no se aplique nuevamente la función tras haber
 * obtenido aunque sea UNA vez true por resultado anteriormente.
*/
void aplicar_con_cada_elemento_postorden(nodo_abb_t* nodo_actual, bool (*funcion)(void*, void*), void* extra, size_t* cantidad_modificados, bool* dejar_de_aplicar){
    
    if((nodo_actual == NULL) || ((*dejar_de_aplicar) == true)){
        return;
    }
        
    aplicar_con_cada_elemento_postorden(nodo_actual->izquierda, funcion, extra, cantidad_modificados, dejar_de_aplicar);

    if((*dejar_de_aplicar) == true){
        return;
    }

    aplicar_con_cada_elemento_postorden(nodo_actual->derecha, funcion, extra, cantidad_modificados, dejar_de_aplicar);

    if((*dejar_de_aplicar) == true){
        return;
    }

    (*dejar_de_aplicar) = funcion(nodo_actual->elemento, extra);
    (*cantidad_modificados)++;

}






size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){

    if(parametros_iterador_son_invalidos(arbol, recorrido, funcion)){
        return 0;
    }

    size_t cantidad_modificados = 0;
    bool dejar_de_aplicar = false;
    
    if(recorrido == ABB_RECORRER_INORDEN){

        aplicar_con_cada_elemento_inorden(arbol->nodo_raiz, funcion, extra, &cantidad_modificados, &dejar_de_aplicar);

    }
    else if(recorrido == ABB_RECORRER_PREORDEN){

        aplicar_con_cada_elemento_preorden(arbol->nodo_raiz, funcion, extra, &cantidad_modificados, &dejar_de_aplicar);

    }
    else if(recorrido == ABB_RECORRER_POSTORDEN){

        aplicar_con_cada_elemento_postorden(arbol->nodo_raiz, funcion, extra, &cantidad_modificados, &dejar_de_aplicar);

    }

    return cantidad_modificados;

}
