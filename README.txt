
▒▒▒▒▒▒▒▒▒▒▒▒  Consideraciones y estructuras utilizadas  ▒▒▒▒▒▒▒▒▒▒▒▒ 

    Decidí realizar todas las pruebas en un mismo pruebas.c para
    evitar modificar el makefile al probar cambios en archivos
    distintos, y considero que queda más organizado tener todas
    las pruebas en un solo lugar que tener que ir mirandolas 
    por separado.
    Comenzaré por explicar las estructuras principales (salon_t,
    entrenador_t) e iré adentrando en decisiones tomadas
    para definirlas conforme sea necesario.
    Posteriormente abarcaré la explicación de la estructura usada
    para procesar los comandos que recibe el interprete.

    Se tiene en la estructura salon_t:
        - Un abb_t (arbol binario de búsqueda) que se
        usará para almacenar a los entrenadores.
        -La cantidad de entrenadores que almacene actualmente el salon.
        -Un hash para almacenar los comandos. 

        Decidí utilizar un abb ya que:
            Todos los comandos relacionados exclusivamente a
            entrenadores solicitan la información de los mismos
            ordenados por orden alfabético, con lo cual haciendo
            uso de un comparador se pueden tener los entrenadores
            permanentemente ordenados sencillamente al insertarlos en el
            arbol.

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
            (Como alternativa se podría modificar la implementación
            del tda lista para tener listas ordenadas, pero considero que
            no vale la pena teniendo listo el arbol).
            Al tenerlos en un abb es visible pensar en que 
            se puede usar la función arbol_recorrido_inorden, o el
            iterador de arbol (con recorrido inorden) para 
            obtener todos los entrenadores de manera ordenada e
            insertarlos todos en una lista nueva si asi se quiere.

        Decidí usar un hash para los comandos debido a que:
            Cada comando tiene un nombre único, con lo cual éste puede
            ser usado como la clave de cada comando.
            Para este caso específico no se requiere ningún orden para
            almacenar los comandos, con lo que hacer uso de un hash
            cada que se quiera obtener un comando a utilizar a partir
            de su nombre representa una ventaja en cuanto a complejidad.

            En clase se discutieron distintas formas de abordar esta idea
            tomando en cuenta que reservar memoria para un hash y sus
            respectivos comandos para esta situación es bastante pesado
            si se piensa crear cada que se ejecuta un comando, con lo que 
            se pierde mucho la idea de querer hacerlo eficiente (tomando
            en cuenta además que los comandos a ejecutar son siempre los
            mismos para esta implementación y no se van a querer agregar
            comandos adicionales posteriormente).
            Debido a todo lo anterior, se llegó a pensar como idea en clase
            el querer hacer una variable global static con el hash, 
            proponiendose precisamente porque los comandos iban a ser
            los mismos siempre (Sin embargo se nos advirtió que eso
            conllevaba riesgos adicionales que excedían los conocimientos
            vistos en la materia).
            
            Tomando en cuenta todo lo dicho, decidí tener el hash de 
            comandos en la estructura salon e inicializarla con todos
            los comandos predeterminados a la hora de crear un salon
            (es decir, cuando se lee un archivo).
            De esta forma evito los problemas que conlleve usar variables
            globales, que, si bien se puede justificar su uso para este caso
            en particular, tiene riesgos en su preferencia evitables por
            desconocimiento de los mismos.
            Además, reservar e inicializar el hash (junto con sus comandos) al crear 
            el salón evade también lo mencionado antes (de inicializarlo
            cada que se procese un comando, lo cual generaría una completa
            perdida de eficiencia y se perdería la ventaja deseada de usar un hash).
            
    Se tiene en la estructura entrenador_t:
        -Un string para el nombre del entrenador.
        -La cantidad de victorias del mismo.
        -Una lista_t que se va a encargar de almacenar los pokemones del entrenador.
        
        Considero adecuado usar una lista para los pokemones debido a que
        no se requiere ningún orden específico para los mismos. (Si bien 
        esto puede llegar a pensarse también como un hash, pienso que es mejor
        una lista para cuando se quieran comparar equipos entre entrenadores, 
        por medio del iterador externo de lista).

    
    Se tiene inclusión a "util.h", archivo con .c implementado en el TP1 para manejo de vectores
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


    ▒▒▒▒  Sobre orden de realización de las pruebas  ▒▒▒▒
    
        Para este TP no realicé todas las pruebas en el mismo orden secuencial
        que se ve reflejado en pruebas.c, aunque de igual manera fuí probando
        cada función cuando fué necesario. El orden que figura en pruebas.c
        fué pensado para tener todas las pruebas relacionadas a un mismo
        propósito en bloques organizados.


    ▒▒▒▒  Sobre  ▒▒▒▒

        


    ▒▒▒▒  Sobre  ▒▒▒▒

        


    ▒▒▒▒  Sobre  ▒▒▒▒

       


    ▒▒▒▒  Sobre  ▒▒▒▒

        