/*
    PRÁCTICA: SELECTION SORT EN JAVASCRIPT
    --------------------------------------

    Práctica 1.2: Medición del tiempo en Javascript
    Alumnos: 
     - Álvarez Tahuilán Luis Gustavo
     - Noyola Gómez Emilio Damian
    Fecha: 17 de septiembre de 2026

    El programa:

    1. Define una estructura llamada CArray para manejar un arreglo.
    2. Permite llenar ese arreglo de distintas maneras.
    3. Genera el peor caso para Selection Sort usando orden inverso.
    4. Ordena el arreglo mediante Selection Sort.
    5. Mide cuánto tarda el algoritmo usando performance.now().
    6. Repite el experimento para diferentes tamaños de entrada.
    7. Imprime los resultados en un formato fácil de graficar.

    En JavaScript no se especifican tipos como int, float, double, etc.
    El lenguaje determina dinámicamente el tipo de cada valor.
*/

/*
    function CArray(numElements)

    "function" declara una función en JavaScript.

    En este caso CArray se utilizará como FUNCIÓN CONSTRUCTORA.

    Una función constructora es similar conceptualmente a utilizar
    una estructura u objeto en C, aunque JavaScript trabaja con objetos.

    Más adelante se utilizará así:

        var arreglo = new CArray(100);

    La palabra "new" crea un nuevo objeto y hace que "this"
    haga referencia a ese nuevo objeto.

    numElements es el parámetro que indica cuántos elementos tendrá
    el arreglo.

    En C podríamos pensar conceptualmente en algo semejante a:

        struct CArray {
            int *dataStore;
            int pos;
            int numElements;
        };

    Aunque internamente JavaScript funciona de manera diferente.
*/
function CArray(numElements) {

    /*
        this

        "this" hace referencia al objeto que se está creando
        cuando usamos:

            new CArray(...)

        Por ejemplo:

            var arreglo = new CArray(100);

        dentro de CArray:

            this

        hace referencia a:

            arreglo


        dataStore será una propiedad del objeto.

        [] crea un arreglo vacío en JavaScript.

        A diferencia de C, los arreglos de JavaScript son dinámicos:
        pueden crecer automáticamente y no necesitan declarar
        previamente su tamaño.

        Después podríamos tener:

            this.dataStore[0] = 25;
            this.dataStore[1] = 40;
    */
    this.dataStore = [];


    /*
        pos guarda la posición donde se insertaría el siguiente elemento.

        Se inicializa en 0 porque los índices de los arreglos,
        igual que en C, comienzan en 0.
    */
    this.pos = 0;


    /*
        Guardamos el número total de elementos.

        La propiedad:

            this.numElements

        recibe el valor del parámetro:

            numElements

        Si hacemos:

            new CArray(500);

        entonces:

            this.numElements = 500;
    */
    this.numElements = numElements;


    /*
        En JavaScript las funciones pueden almacenarse en variables
        y también pueden asignarse como propiedades de un objeto.

        Aquí estamos indicando que el objeto tendrá un método llamado:

            insert

        y ese método utilizará la función insert declarada más adelante.

        Después podremos hacer:

            arreglo.insert(10);

        En C esto no es habitual directamente; conceptualmente
        sería parecido a asociar funciones con una estructura.
    */
    this.insert = insert;


    /*
        Asociamos el método toString del objeto con la función
        toString declarada posteriormente.

        Sirve para convertir el contenido del arreglo a texto.
    */
    this.toString = toString;


    /*
        Asociamos el método clear con la función clear.

        Su función será colocar todos los elementos en 0.
    */
    this.clear = clear;


    /*
        Asociamos setData con la función setData.

        Esta función llena el arreglo con números aleatorios.
    */
    this.setData = setData;


    /*
        Asociamos setWorstCase.

        Esta función genera el arreglo en orden inverso,
        que utilizaremos como entrada del experimento.
    */
    this.setWorstCase = setWorstCase;


    /*
        Asociamos la función swap.

        swap intercambia dos elementos del arreglo.
    */
    this.swap = swap;


    /*
        Asociamos el algoritmo Selection Sort al objeto.

        Gracias a esto podremos utilizar:

            arreglo.selectionSort();
    */
    this.selectionSort = selectionSort;


    /*
        Inicializamos el arreglo.

        "var" declara una variable.

        JavaScript no requiere especificar un tipo:

            var i = 0;

        en vez de C:

            int i = 0;


        El ciclo funciona prácticamente igual que en C:

            inicialización;
            condición;
            incremento;


        ++i incrementa i en una unidad.

        En este contexto:

            ++i

        e:

            i++

        producen el mismo efecto práctico.
    */
    for (var i = 0; i < numElements; ++i) {

        /*
            Guardamos i en la posición i del arreglo.

            Si numElements = 5:

                dataStore[0] = 0
                dataStore[1] = 1
                dataStore[2] = 2
                dataStore[3] = 3
                dataStore[4] = 4

            El arreglo inicialmente queda:

                [0, 1, 2, 3, 4]
        */
        this.dataStore[i] = i;
    }
}


/*
    setData()

    Llena el arreglo con números pseudoaleatorios.

    No recibe parámetros porque utiliza directamente
    las propiedades del objeto mediante "this".

    Cuando hacemos:

        arreglo.setData();

    "this" dentro de la función representa a "arreglo".
*/
function setData() {

    /*
        Recorremos desde 0 hasta numElements - 1.

        this.numElements contiene el tamaño configurado
        cuando se creó el objeto.
    */
    for (var i = 0; i < this.numElements; ++i) {

        /*
            Math es un objeto incorporado de JavaScript.

            Proporciona funciones matemáticas, de manera parecida
            a <math.h> en C, aunque no necesita #include.


            Math.random()

            devuelve un número pseudoaleatorio de tipo Number
            dentro del intervalo:

                0 <= x < 1

            Ejemplos posibles:

                0.1435
                0.7932
                0.0014


            this.numElements + 1

            define el rango máximo que queremos utilizar.


            Ejemplo:

                numElements = 100

            entonces:

                Math.random() * 101

            produce valores aproximadamente entre:

                0 y 100.999...


            Math.floor(x)

            redondea hacia abajo.

            Ejemplos:

                Math.floor(4.9)  -> 4
                Math.floor(8.1)  -> 8
                Math.floor(10.99)-> 10


            Por tanto:

                Math.floor(Math.random() * (this.numElements + 1))

            genera enteros entre:

                0 y this.numElements

            aproximadamente con distribución uniforme.
        */
        this.dataStore[i] = Math.floor(
            Math.random() * (this.numElements + 1)
        );
    }
}


/*
    setWorstCase()

    Prepara el arreglo en orden inverso.

    Para una entrada de tamaño 5 genera:

        [5, 4, 3, 2, 1]

    Para una entrada de tamaño 100:

        [100, 99, 98, ..., 2, 1]

    Para la práctica utilizamos esta disposición como el caso
    desfavorable o "peor caso" solicitado.

    Importante:
    Selection Sort realiza prácticamente el mismo número de
    comparaciones independientemente del orden inicial,
    pero el arreglo inverso se utiliza convencionalmente como
    una entrada desfavorable para este tipo de pruebas.
*/
function setWorstCase() {

    /*
        Recorremos todas las posiciones del arreglo.
    */
    for (var i = 0; i < this.numElements; ++i) {

        /*
            En cada posición guardamos:

                numElements - i


            Ejemplo con numElements = 5:

                i = 0 -> 5 - 0 = 5
                i = 1 -> 5 - 1 = 4
                i = 2 -> 5 - 2 = 3
                i = 3 -> 5 - 3 = 2
                i = 4 -> 5 - 4 = 1

            Resultado:

                [5, 4, 3, 2, 1]
        */
        this.dataStore[i] = this.numElements - i;
    }
}


/*
    clear()

    Recorre el arreglo y coloca todos sus elementos en cero.

    No elimina el arreglo ni cambia su tamaño.

    Ejemplo:

        [7, 4, 9, 2]

    pasa a:

        [0, 0, 0, 0]
*/
function clear() {

    /*
        this.dataStore.length

        ".length" es una propiedad de los arreglos de JavaScript.

        Devuelve la cantidad actual de elementos.

        Si:

            dataStore = [10, 20, 30];

        entonces:

            dataStore.length

        vale:

            3

        En C normalmente tendríamos que conservar el tamaño
        en una variable aparte.
    */
    for (var i = 0; i < this.dataStore.length; ++i) {

        /*
            Sustituimos el elemento actual por cero.
        */
        this.dataStore[i] = 0;
    }
}


/*
    insert(element)

    Inserta un elemento en la posición indicada por this.pos.

    element es el valor que queremos insertar.
*/
function insert(element) {

    /*
        Esta línea tiene dos operaciones:

            this.dataStore[this.pos] = element;
            this.pos++;

        Se utiliza:

            this.pos++

        que es POST-INCREMENTO.

        Primero se utiliza el valor actual de pos como índice
        y después pos aumenta en 1.


        Ejemplo:

            pos = 0
            element = 50

        se hace:

            dataStore[0] = 50

        y después:

            pos = 1


        En C esta sintaxis funciona de la misma manera.
    */
    this.dataStore[this.pos++] = element;
}


/*
    toString()

    Convierte todo el contenido del arreglo en una cadena de texto.

    No modifica los datos.

    return devuelve la cadena creada.
*/
function toString() {

    /*
        Creamos una cadena vacía.

        En JavaScript:

            ""

        representa una cadena de caracteres.

        No existe la distinción directa de char* que tendríamos
        que manejar en C.
    */
    var retstr = "";


    /*
        Recorremos todos los elementos del arreglo.
    */
    for (var i = 0; i < this.dataStore.length; ++i) {

        /*
            += significa:

                retstr = retstr + ...


            this.dataStore[i]

            obtiene el elemento actual.


            + " "

            agrega un espacio después del número.


            JavaScript convierte automáticamente el número a texto
            cuando lo concatena con una cadena.

            Ejemplo:

                retstr = "10 20 "
                dataStore[i] = 30

            después:

                retstr = "10 20 30 "
        */
        retstr += this.dataStore[i] + " ";


        /*
            Esta condición introduce un salto de línea
            aproximadamente cada 10 elementos.


            &&

            es el operador lógico AND, igual que en C.


            i > 0

            evita que se introduzca un salto en la posición 0.


            %

            es el operador módulo, también igual que en C.


            i % 10 == 0

            comprueba que i sea múltiplo de 10.


            ==

            realiza una comparación de igualdad.

            JavaScript tiene además:

                ===

            que compara valor Y tipo.

            En JavaScript moderno generalmente se prefiere ===.


            Aquí:

                i % 10 == 0

            funciona correctamente porque ambos operandos son números.
        */
        if (i > 0 && i % 10 == 0) {

            /*
                \n representa un salto de línea,
                igual que en C.
            */
            retstr += "\n";
        }
    }


    /*
        Devolvemos la cadena construida.

        A diferencia de:

            void

        en C, JavaScript no declara anticipadamente el tipo
        de retorno de la función.
    */
    return retstr;
}


/*
    swap(arr, index1, index2)

    Intercambia dos elementos de un arreglo.

    Parámetros:

        arr     -> arreglo
        index1  -> primera posición
        index2  -> segunda posición


    Conceptualmente es exactamente el intercambio clásico de C:

        temp = a;
        a = b;
        b = temp;
*/
function swap(arr, index1, index2) {

    /*
        Guardamos temporalmente el primer valor
        para no perderlo cuando sobrescribamos esa posición.
    */
    var temp = arr[index1];


    /*
        La primera posición recibe el valor de la segunda.
    */
    arr[index1] = arr[index2];


    /*
        La segunda posición recibe el valor original
        de la primera, que habíamos guardado en temp.
    */
    arr[index2] = temp;
}


/*
    selectionSort()

    Implementa el algoritmo de ordenamiento por selección.

    Idea general:

    En cada iteración:

        1. Consideramos que la posición actual contiene el mínimo.
        2. Recorremos todos los elementos posteriores.
        3. Buscamos el valor más pequeño.
        4. Guardamos su índice.
        5. Intercambiamos ese mínimo con la posición actual.

    Ejemplo:

        [5, 3, 4, 1, 2]

    Primera iteración:

        mínimo encontrado = 1

        [1, 3, 4, 5, 2]

    Segunda:

        mínimo de la parte restante = 2

        [1, 2, 4, 5, 3]

    etc.
*/
function selectionSort() {

    /*
        min almacenará el ÍNDICE del elemento mínimo.

        Es importante:

            min NO guarda directamente el valor mínimo.

        Guarda su posición dentro del arreglo.

        No asignamos todavía ningún valor.
        JavaScript permite declarar:

            var min;

        sin inicializarla inmediatamente.
    */
    var min;


    /*
        CICLO EXTERNO

        outer representa la posición que vamos a colocar
        correctamente en cada iteración.


        Empieza en:

            0

        porque los arreglos comienzan en índice 0.


        La condición es:

            outer < this.dataStore.length - 1

        No necesitamos procesar el último elemento.

        Cuando todos los anteriores están ordenados,
        el último necesariamente también queda en su posición correcta.


        Ejemplo con 5 elementos:

            outer = 0
            outer = 1
            outer = 2
            outer = 3

        No hace falta outer = 4.
    */
    for (
        var outer = 0;
        outer < this.dataStore.length - 1;
        ++outer
    ) {

        /*
            Al comenzar cada iteración asumimos que
            el elemento de la posición outer es el mínimo.

            Ejemplo:

                outer = 2

            entonces:

                min = 2

            Posteriormente buscaremos si existe un elemento
            menor en las posiciones siguientes.
        */
        min = outer;


        /*
            CICLO INTERNO

            Comienza en:

                outer + 1

            porque no necesitamos comparar el elemento
            consigo mismo.


            Si outer = 0:

                inner comienza en 1.

            Si outer = 5:

                inner comienza en 6.


            El ciclo recorre toda la parte del arreglo
            que todavía no está ordenada.
        */
        for (
            var inner = outer + 1;
            inner < this.dataStore.length;
            ++inner
        ) {

            /*
                Comparamos:

                    dataStore[inner]

                contra:

                    dataStore[min]


                Si el elemento actual es menor que el mínimo
                encontrado hasta el momento, encontramos un
                nuevo mínimo.


                Ejemplo:

                    min = 2

                    dataStore[2] = 15

                    inner = 4
                    dataStore[4] = 7

                como:

                    7 < 15

                actualizamos min.
            */
            if (this.dataStore[inner] < this.dataStore[min]) {

                /*
                    Ahora el índice del mínimo es inner.

                    No intercambiamos todavía.

                    Simplemente recordamos dónde encontramos
                    el menor elemento.
                */
                min = inner;
            }
        }


        /*
            Al terminar el ciclo interno:

                min

            contiene el índice del elemento más pequeño
            de la parte no ordenada.


            !==

            es el operador "distinto estricto" de JavaScript.

            Compara valor Y tipo.


            Por ejemplo:

                5 !== 6     -> true
                5 !== 5     -> false
                5 !== "5"   -> true


            JavaScript también tiene:

                !=

            pero != realiza conversión automática de tipos.

            Ejemplo:

                5 != "5"

            da false porque convierte el texto "5" a número.


            Por eso normalmente es recomendable utilizar:

                !==
                ===

            en JavaScript.


            Aquí comprobamos:

                ¿el mínimo está en otra posición?

            Si:

                min == outer

            significa que el elemento ya estaba correctamente colocado
            y no necesitamos intercambiar nada.
        */
        if (min !== outer) {

            /*
                Llamamos al método swap perteneciente al objeto.

                this.swap(...)

                equivale a utilizar la función swap que asociamos
                dentro del constructor:

                    this.swap = swap;


                Parámetros:

                    this.dataStore -> arreglo
                    outer          -> posición actual
                    min            -> posición del mínimo


                Por tanto intercambiamos:

                    arreglo[outer]

                con:

                    arreglo[min]
            */
            this.swap(
                this.dataStore,
                outer,
                min
            );
        }
    }
}


/*
    LISTA DE TAMAÑOS A PROBAR

    [] crea un arreglo de JavaScript.

    A diferencia de C, no necesitamos especificar:

        int elementos[11]

    JavaScript calcula automáticamente su longitud.


    elementos.length

    posteriormente devolverá:

        11
*/
var elementos = [
    100,
    500,
    1500,
    5000,
    10000,
    15000,
    50000,
    100000,
    250000,
    500000,
    1000000
];


/*
    console

    es un objeto proporcionado por el entorno de JavaScript.


    console.log()

    imprime información en la consola.

    Es conceptualmente similar a:

        printf()

    en C.


    Aquí imprimimos los encabezados para que los resultados
    puedan copiarse posteriormente a un archivo CSV:

        Elementos,Tiempo_ms
*/
console.log("Elementos,Tiempo_ms");


/*
    Recorremos todos los tamaños contenidos en "elementos".

    elementos.length = 11

    por lo que i recorrerá:

        0, 1, 2, ..., 10
*/
for (var i = 0; i < elementos.length; ++i) {

    /*
        Obtenemos el tamaño que corresponde a esta iteración.

        Primera iteración:

            i = 0
            n = elementos[0]
            n = 100

        Segunda:

            i = 1
            n = 500

        etc.
    */
    var n = elementos[i];


    /*
        "new" crea una nueva instancia de CArray.

        Si:

            n = 5000

        entonces:

            new CArray(5000)

        ejecuta la función constructora CArray
        y crea un nuevo objeto con:

            dataStore
            pos
            numElements
            insert
            toString
            clear
            setData
            setWorstCase
            swap
            selectionSort


        La referencia al objeto creado se guarda en:

            arreglo


        Conceptualmente:

            arreglo

        representa nuestro conjunto de datos para esta prueba.
    */
    var arreglo = new CArray(n);


    /*
        Llamamos al método setWorstCase().

        El punto:

            .

        se utiliza para acceder a propiedades o métodos de un objeto.


        arreglo.setWorstCase()

        significa:

            ejecuta el método setWorstCase perteneciente a arreglo.


        Si n = 5:

            antes:

                [0, 1, 2, 3, 4]

            después:

                [5, 4, 3, 2, 1]
    */
    arreglo.setWorstCase();


    /*
        "const" declara una variable cuyo identificador
        no puede ser reasignado posteriormente.

        En este caso:

            ini

        contendrá el instante en el cual comienza la medición.


        performance

        es un objeto que proporciona herramientas para medir
        rendimiento y tiempo de ejecución.


        performance.now()

        devuelve un tiempo de alta resolución expresado
        en MILISEGUNDOS.


        Ejemplo posible:

            1584.413275


        No representa necesariamente una hora del reloj.

        Lo que nos interesa es la diferencia entre dos mediciones.


        Es más apropiado para medir tiempos cortos que Date.now(),
        ya que puede proporcionar resolución fraccionaria
        de milisegundos.
    */
    const ini = performance.now();


    /*
        Ejecutamos Selection Sort.

        IMPORTANTE:

        solamente el algoritmo está situado entre:

            performance.now()

        inicial y final.

        Por tanto NO estamos midiendo:

            creación del arreglo
            generación del caso inverso

        Estamos midiendo principalmente:

            Selection Sort
    */
    arreglo.selectionSort();


    /*
        Tomamos nuevamente el tiempo justo después
        de que termina Selection Sort.
    */
    const fin = performance.now();


    /*
        Calculamos el tiempo transcurrido:

            tiempo = tiempo final - tiempo inicial


        Por ejemplo:

            ini = 2500.40
            fin = 2518.75

        entonces:

            tiempo = 18.35 ms


        Utilizamos const porque no necesitamos cambiar
        posteriormente este resultado.
    */
    const tiempo = fin - ini;


    /*
        Mostramos el resultado.


        Aquí aparecen las COMILLAS INVERTIDAS:

            ` `


        No son comillas normales:

            " "
            ' '

        Se conocen como:

            backticks

        y permiten crear "template literals".


        Dentro de un template literal podemos insertar
        directamente expresiones mediante:

            ${expresion}


        Por ejemplo:

            n = 500
            tiempo = 3.25

        entonces:

            `${n},${tiempo}`

        produce:

            "500,3.25"


        Esto es equivalente aproximadamente a:

            n + "," + tiempo


        pero los template literals suelen ser más claros.


        La coma permite que la salida pueda interpretarse
        posteriormente como CSV:

            Elementos,Tiempo_ms
            100,0.051
            500,1.024
            1500,3.583
            ...


        CSV significa:

            Comma-Separated Values
            Valores Separados por Comas
    */
    console.log(`${n},${tiempo}`);
}