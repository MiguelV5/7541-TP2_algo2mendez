
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
        no se requiere ningún orden específico para los mismos. Si bien 
        esto puede llegar a pensarse también como un hash, pienso que es mejor
        una lista para cuando se quieran comparar equipos entre entrenadores, 
        por medio del iterador externo de lista. Además, al guardar archivo a partir
        de un salón o al realizar comparaciones entre entrenadores se requiere que
        los pokemon estén en el orden que les correspondía en el archivo leído, con lo cual
        queda completamente descartado un hash. Lo que sí podría haberse usado es un
        vector dinámico de punteros a pokemon, aunque a efectos de comparar entrenadores entre
        sí no cambiaría mucho con respecto a la iteración de dos listas. Es otra posible
        forma de encararlo.

    Se tiene en la estructura comando_t:
        -Un string para el nombre del comando.
        -Un puntero a la función de ejecución que le corresponde a ese comando.

        Considero que de esta forma queda bastante clara la distinción entre un comando
        y otro, también teniendo en cuenta lo anteriormente mencionado acerca del uso de
        hash para almacenar los comandos.
    
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

    Para hacer más fácil la lectura de código se tienen comentarios en
    salon.c y entrenadores.c a modo de separadores que distinguen
    las funciones que tienen comportamientos complementarios entre sí.

    Se omite:
        - Explicación de ciertas funciones o partes de las
        mismas que no la requieren (son intuitivas al leerlas
        o están explicadas en Aclaraciones).
        - Explicación de verificaciones visibles (de parámetros
        inválidos, fallas de reserva en memoria, etc).

    ▒▒▒▒  salon.c  ▒▒▒▒

        --- salon_leer_archivo ---
            (Se hará un mini resumen de lo explicado en el tp1 para la lógica de lectura
            de archivos, pues es exáctamente la misma y no se vió modificada. Lo que cambia
            es la forma de almacenar la información).

            Comenzando por la lógica de lectura:
            En almacenador_salon se hace distinción de las lineas leidas por medio de
            la funcion split. Si los campos leidos son 2 (Nombre y victorias) se leyó
            un entrenador, si son 6 era un pokemon. Los entrenadores se almacenan al salón
            recién despues de haber almacenado todos sus pokemon en el equipo del mismo,
            es por eso que la función almacenador_entrenadores solo inserta el entrenador
            previo y no el actual.
            Exclusivamente en la linea 781 se inserta el entrenador actual, y es porque si
            la linea leida es NULL se acabó el archivo y se debe almacenar el último 
            entrenador.

            Siguiendo con la lógica de almacenamiento:
            (Acá se hace muy presente la ventaja de usar un abb).
            Para los entrenadores es simplemente insertarlos en el arbol, ya teniendo el
            comparador que los ordene en orden alfabético cuando ingresan al mismo.
            Para los pokemon, en entrenador_agregar_pokemon_leido se insertan los mismos
            en una lista en el orden en que llegan, que es el mismo en el que aparecían
            en el archivo leido.

            Tras almacenar todo se verifica que los entrenadores leidos no tengan
            equipo vacío o que no hayan dos entrenadores de nombre idéntico almacenados.
            En caso de toparse con alguno de los dos casos, falla la lectura y se
            devuelve NULL.

            Al salir de lo anterior lo último que se hace es agregar todos los 
            comandos disponibles en el hash de comandos. 
            Para evitar ir acumulando una lista interminable de free's en
            salon_agregar_comandos_por_defecto se creó la función liberar_anteriores_por_falla,
            que hace free de todos los parámetros recibidos.
            (con la ventaja de que hacer free(NULL) no hace nada, con lo que se puede llamar
            aún cuando no se tienen todos los comandos reservados en memoria).
            A mi parecer es mucho más prolijo así que ir acumulando todos los frees cada 
            que se crea un comando al irse verificando.

        --- salon_guardar_archivo ---
            Se usa el iterador de arbol en modo inorden para escribir los entreandores en orden
            alfabético. En entrenador_escribir_en_archivo además se usa el iterador de lista
            para escribir a los pokemon en el mismo orden que se tenía al crear el salón que 
            se está escribiendo.

        --- salon_agregar_entrenador ---
            Es simplemente llamar a arbol_insertar con el entrenador nuevo.
            Se considera como error intentar agregar en un entrenador de equipo vacío dado que 
            un entrenador no puede tener menos de un pokemon para este caso.
            
        --- salon_filtrar_entrenadores ---
            Se obtienen todos los entrenadores del arbol y se almacenan en un vector de punteros
            a los mismos.
            Luego por cada entrenador se llama a la función de filtro recibida, y si el entrenador
            pasa el filtro se inserta en una lista nueva que al final se devuelve si todo se
            pudo insertar debidamente.

        --- salon_ejecutar_comando ---
            Teniendo el hash de comandos, se busca dentro del mismo el comando requerido a partir del
            nombre (que va a ser el primer argumento de la linea recibida por input del usuario. Para
            separar su información se usa la función split del tp1).
            Si no se encuentra el comando pedido, se devuelve error. Si se encuentra, se llama al EJECUTOR
            correspondiente al comando pedido (se explicarán todos los ejecutores más adelante).

        --- salon_destruir ---
            Acá entra en juego el uso de los destructores de arbol y el recientemente
            añadido destructor de lista (para los pokemon).
            
        --- EJECUTORES ---
            
            Para comando ENTRENADORES:
            //// salon_pedir_entrenadores_listados ////
                Dados los argumentos recibidos por el usuario, a partir de la cantidad de los mismos
                se prosigue de manera distinta.
                Si la cantidad es 1 quiere decir que se tenía solo el argumento ENTRENADORES (esto se 
                puede asegurar porque ya previamente fue buscado en el hash de comandos con ese nombre)
                En tal caso :
                    1) Me guardo un vector de entrenadores con todos los del arbol.
                    2) Obtengo la información concatenada completa de cada uno de los entrenadores
                    (explicación de entrenador_obtener_info_concatenada a continuación).

                    // entrenador_obtener_info_concatenada // 
                        Se obtiene el tamaño del nombre del entrenador y del necesario para convertir
                        el entero de victorias a un string (snprintf con los parámetros NULL, 0 y la
                        variable a transformar devuelve dicha cantidad necesaria menos sin contar al '\0'.
                        Por esto es que más adelante se llama a snprintf con dicho tamaño + 1).
                        Se hace reserva de memoria con la suma de dichos tamaños + 2 adicionales para
                        tomar en cuenta la ',' pedida en el formato a devolver y el '\0'.
                        Luego, se concatenan el nombre, la ',' y las victorias ya transformadas en string.
                        Se devuelve el resultado de concatenar.
   
                    3) Teniendo la info de todos los entrenadores, se juntan todos en recopilacion_y_formateo_de_strings
                    con los \n de por medio para acabar de completar el formato.
                    Luego, volviendo a salon_pedir_entrenadores_listados, se libera el vector
                    en el que se tenían los strings con la info separada por cada entrenador y se
                    devuelve el output esperado (o NULL en caso de que algo halla fallado).
                
                En el otro caso: (La cantidad de argumentos era 2) 
                    1) Se divide el segundo argumento (argumentos[1]) por comas.
                    Si la cantidad de parametros divididos no es dos (victorias,n  ó  pokemon,nombre)
                    quiere decir que se pasaron parámetros inválidos, por lo que se devuelve NULL.
                    2) En procesar_parametros_de_comando_entrenador, dependiendo de si los parámetros
                    recibidos fueron petición de victorias o de entrenadores que tuvieran un pokemon
                    específico; se llama a procesar_parametro_final_entrenador pasandole la función
                    de filtro que corresponda.
                    3) En procesar_parametro_final_entrenador se obtiene una lista con los filtrados,
                    se crea un iterador de esa lista y se obtiene el nombre de todos los entrenadores
                    de la misma. Tras tener todos los nombres se llama a recopilacion_y_formateo_de_strings
                    para finalizar el concatenado.
        
            Para comando EQUIPO:
            //// salon_pedir_equipo_de_entrenador ////
                1) Se verifica que los argumentos sean 2 (EQUIPO y el nombre del entrenador requerido).
                2) Se obtiene un vector con todos los entrenadores del arbol y se busca en el mismo
                el que tenga el nombre pedido.
                3) Se llama a entrenador_obtener_equipo_concatenado que devuelve la información completa
                de todos sus pokemon concatenados con el formato pedido.
                (explicación de entrenador_obtener_equipo_concatenado a continuación).

                // entrenador_obtener_equipo_concatenado // 
                        Se llama a equipo_obtener_string_concatenado con el equipo del entrenador y
                        la cantidad de pokemones del mismo.
                        Allí se usa un vector de strings con el tamaño de la totalidad de campos de
                        todos los pokemon del equipo.

                        Se utiliza además un vector de tamaños para ir obteniendo la cantidad en memoria
                        que se necesita para guardar la info de cada uno de los 6 campos de cada
                        pokemon (debido a que cada campo con contenido numérico va a tener que ser
                        transformado en string posteriormente).
                        Uso entonces el iterador externo de lista para ir recorriendola.

                        Por cada iteración se calcula la memoria necesaria para trasformar la info del
                        pokemon actual en strings, y luego se almacenan los mismos en el vector de strings
                        mencionado antes a partir de la posición actual de la iteración (puntero a la
                        variable i. Tener en cuenta que se pasa puntero a ésta para ir aumentandolo
                        como contador cada que se reserva exitosamente un string, dentro de
                        la función almacenar_en_memoria_cada_campo_pokemon).

                        Volviendo a equipo_obtener_string_concatenado, tras acabar con todos los pokemon
                        del equipo, ya se tiene a la mano el vector lleno con todos los strings de cada
                        campo de cada pokemon del equipo. Se procede entonces a concatenar_info_pokemon_con_formato.
                        De allí se llama a calcular_tamanio_reserva_para_equipo. El cálculo que allí se realiza es: Se suma el tamaño de todos los strings recolectados previamente y se le suma la cantidad
                        explicada en el comentario en la inicialización de la variable tamanio_total_reserva.
                        Devolviendonos a concatenar_info_pokemon_con_formato, se reserva un string con el tamaño
                        recién calculado y se procede a concatenar todos los strings con el formato pedido final.

                        [Nota: puede parecer redundante tener 5 casos distintos en el for de concatenación, mas
                        no lo es. Debido a lo explicado en el comentario de la linea 703, se realiza una clase de
                        corrimiento en la iteración que resulta en una concatenación erronea de los '\n'.
                        Es por esto que a partir de la sexta iteración (cuando se acaba con el 1er
                        pokemon) se verifica si hay que concatenar el '\n' en "i + 1".
                        Esto fue encontrado por medio de revisión en gdb].

                        Se retorna en todas las funciones con el resultado final formateado.
            
            Para comando REGLAS:
            //// salon_pedir_reglas ////
                Se verifica que como argumento solo se tenga el nombre del comando (REGLAS), se calcula el
                tamaño necesario para concatenar las reglas (son constantes) y se concatenan con el formato pedido
                en un string reservado con tal tamaño. Se devuelve ese string.
            
            Para comando COMPARAR:
            //// salon_comparar_entrenadores ////
                Se hacen verificaciones sobre la cantidad de parametros nuevamente, y también se verifica si la
                regla recibida no existe.
                Luego se obtiene el vector con todos los entrenadores y se buscan los dos pedidos a enfrentar.
                Por último, se llama a entrenador_enfrentar, que devuelve los resultados de cada batalla entre
                los equipos de los entrenadores buscados, con formato en un string.
                (explicación de entrenador_enfrentar a continuación).
                   
                    // entrenador_enfrentar // 
                        Se crea un iterador externo de ambos equipos, y dependiendo de qué regla de batalla se pidió por
                        parámetro se llama a procesador_de_enfrentamientos con los iteradores y un puntero a
                        la función de calculo de coeficiente que corresponda al enfrentamiento pedido en la regla.
                        En procesador_de_enfrentamientos se itera sobre ambas listas de pokemones a la vez, si se
                        acaba alguna de las dos se corta la iteración.
                        En cada iteración se calcula el coeficiente correspondiente de cada pokemon y se van
                        guardando los resultados de las batallas a la vez que se formatea el string, insertando
                        caracter por caracter.
                        Al final de los enfrentamientos se retorna el string resultante.

            Para comandos AGREGAR_POKEMON y QUITAR_POKEMON:
            //// salon_pedir_agregar_pokemon ////
                Obtengo el vector con los entrenadores, busco el que se requiere con su nombre y reservo un duplicado
                de los argumentos que contienen la información del pokemon a agregar (para poder reutilizar
                la función entrenador_agregar_pokemon_leido que recibe los parametros de pokemon a agregar).
                Una vez agregado el pokemon se reserva el resultado OK y se devuelve si no hubo fallos.
            //// salon_pedir_quitar_pokemon ////
                Mismos pasos iniciales que el agregar. Al tener el entrenador buscado se llama a entrenador_quitar_pokemon.

                    // entrenador_quitar_pokemon // 
                        Verifico que el entrenador del que se pide quitar no tenga solo un pokemon restante en su
                        equipo.
                        Luego se llama a pokemon_buscar_posicion_con_nombre que usa el iterador externo de lista
                        para encontrar al pokemon que se quiere quitar. Se obtiene la posición del mismo en la lista
                        y se elimina dicha posición con lista_borrar_de_posicion. Se devuelve EXITO, o FALLO si no
                        lo pudo encontrar o el entrenador solo tenía un pokemon restante.
                        De allí se reserva un OK y se devuelve si todo anduvo bien.

            //// salon_pedir_guardado ////
                Se verifica que el parametro recibido sea único (el nombre que se le quiere dar al archivo a
                guardar).
                Luego se reutiliza salon_guardar_archivo y se devuelve OK si la operación fué exitosa.

▒▒▒▒▒▒▒▒▒▒▒▒  Aclaraciones:  ▒▒▒▒▒▒▒▒▒▒▒▒ 


    ▒▒▒▒  Sobre la realización de las pruebas  ▒▒▒▒
    
        Para este TP no realicé muchas pruebas relacionadas exclusivamente a todas las
        funciones de entrenador.h, pues las mismsas tienen por objetivo cumplir con
        con el funcionamiento de lo que se requiere en salon.c, y en su mayoría dependen de
        probarse en conjunto. Hice algunas pruebas sobre las funciones fundamentales de
        entrenador, para al menos confirmar el funcionamiento básico del tda, pero no más que eso.

    ▒▒▒▒  Sobre linea 670, salon.c ▒▒▒▒

        No encontré una forma de evitar que la verificación no fuera O(n^2) tras tener todo previamente almacenado,
        se debe verificar que no haya ningún entrenador coincidente, lo cual requiere ver que para cada entrenador
        se cumpla y no basta con una sola iteración completa. Muy probablemente hay una manera de cortar con la
        lectura del archivo durante el almacenamiento y no después de ya tenerlo almacenado, pero se me dificultó
        pensarlo sin modificar mucho la lógica de lectura que se tenía ya hecha de antes.

    ▒▒▒▒  Sobre funciones entrenador_escribir_en_archivo y pokemon_escribir_en_archivo  ▒▒▒▒

        Esta última siempre devuelve true debido a que se usa únicamente en el iterador interno
        de lista, que aplica la función mientras que la misma devuelva true.
        Caso contrario es lo que sucede con entrenador_escribir_en_archivo, que al ser usada solo
        en el iterador interno de abb requería devovler false para seguirse aplicando.

    ▒▒▒▒  Sobre función entrenador_equipo_vacio  ▒▒▒▒

        Esta función es usada para verificar validez de entrenadores almacenados a la hora de 
        finalizar la lectura de archivos. Su finalidad es ser usada con el iterador de arbol,
        por lo cual está adaptada recibiendo sus parametros como void*.
 