
▒▒▒▒▒▒▒▒▒▒▒▒  Consideraciones iniciales y estructuras utilizadas  ▒▒▒▒▒▒▒▒▒▒▒▒ 

    Comenzaré por explicar las estructuras principales (salon_t,
    entrenador_t) e iré adentrando en decisiones tomadas
    para definirlas conforme sea necesario.
    Posteriormente abarcaré la explicación de la estructura usada
    para las reglas y para procesar los comandos que recibe el
    interprete.

    Se tiene en la estructura salon_t:
        - Un puntero a abb_t (arbol binario de búsqueda) que se
        usará para almacenar a los entrenadores.

        Decidí utilizar un abb ya que:
            Todos los comandos relacionados exclusivamente a
            entrenadores solicitan la información de los mismos
            ordenados por orden alfabético, con lo cual haciendo
            uso de un comparador se pueden tener los entrenadores
            permanentemente ordenados al insertarlos en el arbol.

            Además de esto, se sabe por enunciado que los
            entrenadores no tienen nombres repetidos, que si bien 
            podría tentar a usar un hash para almacenarlos usando
            sus nombres como claves, no conviene por lo mencionado
            anteriormente sobre el orden (ya que el hash es un
            contenedor de datos no ordenado).

            Luego, la función salon_filtrar_entrenadores requiere 
            devolver una lista con los mismos según cierto filtro,
            lo cual podría llevar a pensar en usar directamente
            una lista de entrenadores desde el principio, pero
            considero más problematico tener que ordenar a los
            entrenadores mientras se insertan en una lista a la hora
            de leer un archivo que directamente usar un abb que
            por medio del comparador ya los tenga ordenados.

            Al tenerlos en un abb no es muy complicado pensar que 
            se puede usar la función arbol_recorrido_inorden, o el
            iterador de arbol (con recorrido inorden) para 
            obtener todos los entrenadores de manera ordenada e
            insertarlos todos en una lista nueva.

        
    Se tiene en la estructura entrenador_t:
        -Un string para el nombre del entrenador.
        -La cantidad de victorias del mismo.
        -Una lista_t que se va a encargar de almacenar los pokemones del entrenador.
        
        Considero adecuado usar una lista para los pokemones debido a que
        no se requiere ningún orden específico para los mismos. (Si bien 
        esto puede llegar a pensarse también como un hash, pienso que es mejor
        una lista para cuando se quieran comparar equipos entre entrenadores, 
        por medio del iterador externo de lista).

        -Un bool para saber si dicho entrenador agotó sus pokemones disponibles para
        la batalla (es decir, es un campo usado para cuando se quieran comparar dos 
        entrenadores por comando. Es como un indicador de derrota).

    
    Se tiene inclusión a "util.h", archivo implementado en el TP1 para manejo de vectores
    de punteros dinámicos terminados en NULL y lectura de lineas de archivos
    de texto separados por caracteres. 
    No se modificó nada, por ende no se hacen pruebas de sus funciones.

    Se modificó lista.h y lista.c para que pueda manejar un destructor de elementos
    almacenados.
    Este se usa cuando se borran elementos de una lista o cuando se destruye,
    pero NO se utiliza cuando se está usando una lista como pila ni como cola.
    Consideré esos dos casos como apartados ya que sus comportamientos son
    distintos entre sí y a como se debería tratar una lista en general.
    Además de eso, no voy a hacer uso de las listas como colas o pilas para
    este TP en particular, por lo que también vi como innecesario modificar el
    desapilado/desencolado.
    En pruebas.c se realizan las debidas verificaciones de funcionamiento
    para lo que se cambió.




▒▒▒▒▒▒▒▒▒▒▒▒  Explicación de implementación:  ▒▒▒▒▒▒▒▒▒▒▒▒ 

    Se omite:
        - Explicación de ciertas funciones o partes de las
        mismas que no la requieren (son intuitivas al leerlas
        o están explicadas en Aclaraciones).
        - Explicación de verificaciones visibles (de parámetros
        inválidos).

    ▒▒▒▒  .c  ▒▒▒▒

        ---  ---
            

        ---  ---
            

        ---  ---
            
            

        ---  ---
            


▒▒▒▒▒▒▒▒▒▒▒▒  Aclaraciones:  ▒▒▒▒▒▒▒▒▒▒▒▒ 


    ▒▒▒▒    ▒▒▒▒

        


    ▒▒▒▒  Sobre  ▒▒▒▒

        


    ▒▒▒▒  Sobre  ▒▒▒▒

        


    ▒▒▒▒  Sobre  ▒▒▒▒

       


    ▒▒▒▒  Sobre  ▒▒▒▒

        