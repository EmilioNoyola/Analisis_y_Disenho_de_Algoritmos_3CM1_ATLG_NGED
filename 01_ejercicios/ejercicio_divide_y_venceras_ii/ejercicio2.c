/*
 * =====================================================================
 *  DIVIDE Y VENCERÁS — Ejercicios 1 y 2 (explicados paso a paso)
 * =====================================================================
 *
 *  Este archivo compila y corre tal cual:
 *      gcc dyv_explicado.c -o dyv
 *      ./dyv            (en Windows: dyv.exe)
 *
 * ---------------------------------------------------------------------
 *  0. CONCEPTOS QUE NECESITAS ANTES DE EMPEZAR
 * ---------------------------------------------------------------------
 *
 *  ARREGLO: una fila de cajitas numeradas, una pegada a la otra.
 *    En C la numeración (el "índice") empieza en 0:
 *
 *        índice:  0   1   2   3   4
 *        valor:  [1] [2] [3] [3] [4]
 *
 *    a[0] vale 1, a[3] vale 3, etc. El ÍNDICE es la posición (dónde está),
 *    el VALOR es lo que hay guardado dentro (qué hay).
 *
 *  ARREGLO ORDENADO: los valores van de menor a mayor.
 *
 *  FUNCIÓN RECURSIVA: una función que se llama a sí misma con un problema
 *    más pequeño. Siempre necesita un CASO BASE (un momento en que deja de
 *    llamarse y responde directo); si no, se llamaría para siempre.
 *
 *  DIVIDE Y VENCERÁS: estrategia con 3 pasos:
 *    1) DIVIDIR   : partir el problema en pedazos más chicos.
 *    2) VENCER    : resolver los pedazos (normalmente con recursión).
 *    3) COMBINAR  : juntar las respuestas de los pedazos.
 *    En estos dos ejercicios el truco es todavía mejor: al partir el
 *    arreglo a la mitad podemos SABER en cuál mitad está la respuesta,
 *    así que la otra mitad la tiramos completa. Esto se llama
 *    BÚSQUEDA BINARIA, y es el caso más simple de divide y vencerás.
 *
 *  ¿POR QUÉ ES TAN RÁPIDO PARTIR A LA MITAD?
 *    Si tienes 16 elementos y cada paso tiras la mitad:
 *        16 -> 8 -> 4 -> 2 -> 1      (4 pasos)
 *    Con 1,000,000 de elementos bastan unos 20 pasos.
 *    El número de veces que puedes dividir n entre 2 hasta llegar a 1
 *    es log2(n). Por eso decimos que cuesta O(log n).
 *    Recorrer uno por uno, en cambio, cuesta hasta n pasos: O(n).
 *
 *  NOTACIÓN O( ): describe cómo crece el número de pasos cuando n crece.
 *    O(1)     = un número fijo de pasos, sin importar n.
 *    O(log n) = crece muy lento (se duplica n, sumas solo 1 paso).
 *    O(n)     = crece igual que n (se duplica n, se duplican los pasos).
 *
 *  MEJOR / PEOR / PROMEDIO:
 *    Mejor caso   = la entrada más afortunada (menos pasos posibles).
 *    Peor caso    = la entrada más desafortunada (más pasos posibles).
 *    Caso promedio= lo que esperas "en general" si todas las entradas
 *                   posibles fueran igual de probables.
 */

#include <stdio.h>   /* necesario para usar printf (imprimir en pantalla) */


/* =====================================================================
 *  PROBLEMA 1: encontrar el elemento duplicado
 * =====================================================================
 *
 *  ENUNCIADO: arreglo ORDENADO de n elementos con valores consecutivos
 *  de 1 a n-1. Como hay n cajitas pero solo n-1 valores distintos,
 *  exactamente UN valor aparece dos veces. Hay que encontrarlo.
 *
 *  Ejemplo con n = 17 (valores del 1 al 16, el 8 repetido):
 *     [1,2,3,4,5,6,7,8,8,9,10,11,12,13,14,15,16]
 */


/* ---------------------------------------------------------------------
 *  1a) VERSIÓN SECUENCIAL (recorrer uno por uno)
 * ---------------------------------------------------------------------
 *  Idea: como está ordenado, los dos números iguales están JUNTOS.
 *  Basta comparar cada elemento con el de su izquierda; el primero que
 *  sea igual a su vecino es el duplicado.
 */
int duplicadoSecuencial(int a[], int n) {
    /* Empezamos en i = 1 porque comparamos a[i] con a[i-1];
       si empezáramos en 0, a[-1] no existe. */
    for (int i = 1; i < n; i++) {
        if (a[i] == a[i - 1]) {   /* ¿es igual a su vecino de la izquierda? */
            return a[i];          /* sí: este es el duplicado, terminamos  */
        }
    }
    return -1;   /* solo pasaría si el arreglo no cumple el enunciado */
}


/* ---------------------------------------------------------------------
 *  1b) VERSIÓN DIVIDE Y VENCERÁS
 * ---------------------------------------------------------------------
 *
 *  LA OBSERVACIÓN CLAVE (aquí está todo el truco):
 *
 *  Si NO hubiera repetidos, cada cajita guardaría "su índice + 1":
 *
 *      índice:  0  1  2  3  4  5  6  7  8  9 ...
 *      valor:   1  2  3  4  5  6  7  8  9 10 ...   ->  a[i] == i + 1
 *
 *  Ahora mira qué pasa cuando el 8 está repetido:
 *
 *      índice:  0  1  2  3  4  5  6  7  8  9 10 ...
 *      valor:   1  2  3  4  5  6  7  8  8  9 10 ...
 *               \______ a[i] == i+1 ______/ \__ a[i] == i __/
 *
 *  El número repetido "empuja" todo un lugar a la derecha.
 *    - ANTES de la segunda copia: a[i] == i + 1   (todo "en su lugar")
 *    - DESDE la segunda copia:    a[i] == i       (todo "recorrido")
 *
 *  Entonces el arreglo está partido en dos zonas, y la respuesta es el
 *  valor que está en la PRIMERA posición de la zona "recorrida".
 *
 *  ¿Cómo uso eso para descartar mitades? Me paro en la posición del
 *  medio (mid) y pregunto: ¿a[mid] == mid + 1?
 *    - SÍ  -> mid está en la zona "en su lugar"; el duplicado todavía
 *             no ha aparecido, está A LA DERECHA de mid.
 *             Descarto desde ini hasta mid (inclusive).
 *    - NO  -> mid ya está en la zona "recorrida"; el duplicado está en
 *             mid o A LA IZQUIERDA. Me quedo con ini..mid
 *             (mid NO se descarta: podría ser justo la segunda copia).
 *
 *  CASO BASE: cuando el rango tiene un solo elemento (ini == fin),
 *  esa posición es la primera de la zona "recorrida", y su valor es la
 *  respuesta.
 *
 *  ¿Por qué siempre existe esa zona? El último índice es n-1 y el valor
 *  máximo permitido es n-1, así que a[n-1] == n-1: la última cajita
 *  SIEMPRE está en la zona "recorrida". Nunca nos quedamos sin respuesta.
 *
 *  Parámetros:
 *    a   : el arreglo
 *    ini : primer índice del pedazo que estamos revisando
 *    fin : último índice del pedazo que estamos revisando
 */
int buscarDuplicado(int a[], int ini, int fin) {

    /* CASO BASE: queda un solo candidato -> ese es. */
    if (ini == fin) {
        return a[ini];
    }

    /* DIVIDIR: calculamos el centro del pedazo.
       Es lo mismo que (ini + fin) / 2, pero escrito así nunca se
       desborda aunque ini y fin sean números enormes.
       En C, la división entre enteros redondea hacia abajo: 7/2 = 3. */
    int mid = ini + (fin - ini) / 2;

    /* VENCER: decidimos en qué mitad está la respuesta y solo
       resolvemos esa mitad (la otra se descarta por completo). */
    if (a[mid] == mid + 1) {
        /* mid está "en su lugar": el duplicado está más a la derecha */
        return buscarDuplicado(a, mid + 1, fin);
    } else {
        /* mid ya está "recorrido": la respuesta está en ini..mid */
        return buscarDuplicado(a, ini, mid);
    }

    /* COMBINAR: aquí no hace falta, porque la respuesta de la mitad
       que revisamos ES la respuesta completa. */
}

/* Función con la firma que pide el ejercicio: int duplicado(int a[], int n)
   Solo arranca la búsqueda sobre el arreglo completo (índices 0 a n-1). */
int duplicado(int a[], int n) {
    return buscarDuplicado(a, 0, n - 1);
}

/*
 *  RASTREO A MANO con [1,2,3,3,4,5,...,16]  (n = 17, el 3 repetido)
 *
 *      índice: 0 1 2 3 4 5 6 7 8 ...
 *      valor:  1 2 3 3 4 5 6 7 8 ...
 *
 *  ini=0, fin=16 -> mid=8 : a[8]=8,  ¿8 == 9? NO  -> nos quedamos 0..8
 *  ini=0, fin=8  -> mid=4 : a[4]=4,  ¿4 == 5? NO  -> nos quedamos 0..4
 *  ini=0, fin=4  -> mid=2 : a[2]=3,  ¿3 == 3? SÍ  -> nos vamos a 3..4
 *  ini=3, fin=4  -> mid=3 : a[3]=3,  ¿3 == 4? NO  -> nos quedamos 3..3
 *  ini=3, fin=3  -> caso base -> regresa a[3] = 3   ✔
 *
 *  5 pasos para 17 elementos (log2(17) ≈ 4.1, redondeado hacia arriba = 5).
 */


/* ---------------------------------------------------------------------
 *  1c) COMPLEJIDAD DEL PROBLEMA 1
 * ---------------------------------------------------------------------
 *
 *  SECUENCIAL (contamos comparaciones a[i] == a[i-1]):
 *    Mejor caso   : el duplicado es el 1 -> [1,1,2,...]. Lo encuentra en
 *                   la primera comparación.                    -> O(1)
 *    Peor caso    : el duplicado es el último -> [...,15,16,16]. Hace
 *                   n-1 comparaciones.                          -> O(n)
 *    Caso promedio: si el duplicado puede estar en cualquiera de las n-1
 *                   posiciones con la misma probabilidad, en promedio
 *                   hace (1 + 2 + ... + (n-1)) / (n-1) = n/2 comparaciones.
 *                   n/2 sigue creciendo igual que n.            -> O(n)
 *
 *  DIVIDE Y VENCERÁS:
 *    Cada llamada hace un trabajo fijo (una suma, una comparación) y
 *    luego se llama UNA vez con la MITAD del tamaño. Eso se escribe:
 *
 *        T(n) = T(n/2) + c        (c = trabajo fijo de cada paso)
 *
 *    Desenrollando:  T(n) = T(n/4) + 2c = T(n/8) + 3c = ... = T(1) + c·log2(n)
 *    (También sale con el Teorema Maestro: a=1, b=2, f(n)=O(1) -> caso 2.)
 *
 *    Esta versión NO se detiene antes: siempre baja hasta que queda
 *    1 elemento, sin importar dónde esté el duplicado. Por eso:
 *    Mejor caso   : O(log n)
 *    Peor caso    : O(log n)
 *    Caso promedio: O(log n)
 *
 *  CONCLUSIÓN: en el mejor caso gana la secuencial (O(1)), pero en el
 *  peor y en el promedio gana por mucho divide y vencerás.
 *  Con n = 1,000,000: secuencial ~500,000 comparaciones en promedio;
 *  divide y vencerás ~20 siempre.
 */


/* =====================================================================
 *  PROBLEMA 2: ¿existe un índice i tal que Arr[i] == i?
 * =====================================================================
 *
 *  ENUNCIADO: arreglo ORDENADO Arr[1..n] (el enunciado numera desde 1).
 *  Queremos saber si existe una posición i cuyo valor sea igual a i.
 *
 *  DETALLE DE C: en C los arreglos empiezan en 0. Para respetar el
 *  enunciado al pie de la letra, guardamos los datos en las posiciones
 *  1..n y simplemente NO usamos la posición 0 (ponemos ahí cualquier
 *  cosa). Por eso los arreglos de prueba tienen n+1 cajitas.
 *
 *  SUPOSICIÓN IMPORTANTE: los números son ENTEROS y DISTINTOS (no hay
 *  repetidos). Sin esto no se puede garantizar O(log n). Ejemplo de por
 *  qué: Arr = [-1, 0, 4, 4, 5] tiene Arr[4]=4 y Arr[5]=5, pero la
 *  búsqueda binaria se para en mid=3, ve Arr[3]=4 > 3, se va a la
 *  izquierda y nunca los encuentra.
 */


/* ---------------------------------------------------------------------
 *  2a) FUERZA BRUTA
 * ---------------------------------------------------------------------
 *  Idea: revisar TODAS las posiciones, una por una, hasta encontrar
 *  una donde Arr[i] == i.
 *
 *  Costo:
 *    Mejor caso: Arr[1] == 1, lo encuentra a la primera.      -> O(1)
 *    Peor caso : no existe (o está al final): revisa las n.    -> O(n)
 */
int indiceFuerzaBruta(int arr[], int n) {
    for (int i = 1; i <= n; i++) {   /* i va de 1 a n, como el enunciado */
        if (arr[i] == i) {
            return i;                /* encontrado: regresamos el índice */
        }
    }
    return -1;   /* -1 significa "no existe"; no puede confundirse con un
                    índice válido porque los índices van de 1 a n */
}


/* ---------------------------------------------------------------------
 *  2b) DIVIDE Y VENCERÁS — O(log n)
 * ---------------------------------------------------------------------
 *
 *  LA OBSERVACIÓN CLAVE:
 *  Si los números son enteros, distintos y ordenados, cada valor es
 *  al menos 1 más grande que el anterior:  Arr[i+1] >= Arr[i] + 1.
 *  Y el índice también sube exactamente de 1 en 1.
 *  O sea: el VALOR sube igual de rápido o más rápido que el ÍNDICE.
 *
 *  Consecuencia: me paro en mid y comparo Arr[mid] con mid.
 *
 *    - Arr[mid] == mid  -> ¡lo encontré!
 *
 *    - Arr[mid] >  mid  -> el valor ya va "adelantado" al índice.
 *          Como a la derecha el valor sube igual o más rápido que el
 *          índice, NUNCA lo van a alcanzar: seguirá adelantado.
 *          Ejemplo: Arr[4] = 6. Entonces Arr[5] >= 7 > 5, Arr[6] >= 8 > 6...
 *          -> La respuesta solo puede estar a la IZQUIERDA.
 *
 *    - Arr[mid] <  mid  -> el valor va "atrasado" respecto al índice.
 *          Hacia la izquierda el valor baja igual o más rápido que el
 *          índice, así que seguirá atrasado.
 *          Ejemplo: Arr[4] = 1. Entonces Arr[3] <= 0 < 3, Arr[2] <= -1 < 2...
 *          -> La respuesta solo puede estar a la DERECHA.
 *
 *  CASO BASE: si el rango quedó vacío (ini > fin), ya descartamos todo
 *  y no existe tal índice -> regresamos -1.
 *
 *  Nota: si hay varios índices que cumplen, este algoritmo regresa uno
 *  de ellos (el enunciado solo pide saber si existe y dar uno).
 */
int buscarIndice(int arr[], int ini, int fin) {

    /* CASO BASE: rango vacío -> no hay respuesta en este pedazo. */
    if (ini > fin) {
        return -1;
    }

    /* DIVIDIR: posición del centro. */
    int mid = ini + (fin - ini) / 2;

    /* ¿Ya lo encontramos? */
    if (arr[mid] == mid) {
        return mid;
    }

    /* VENCER: elegimos la única mitad donde puede estar. */
    if (arr[mid] > mid) {
        /* valor adelantado -> a la derecha es imposible -> izquierda */
        return buscarIndice(arr, ini, mid - 1);
    } else {
        /* valor atrasado -> a la izquierda es imposible -> derecha */
        return buscarIndice(arr, mid + 1, fin);
    }
    /* Aquí mid SÍ se descarta (mid - 1 / mid + 1) porque ya lo
       revisamos y sabemos que no es la respuesta. */
}

/* Arranca la búsqueda sobre todo el arreglo (índices 1 a n). */
int indiceIgual(int arr[], int n) {
    return buscarIndice(arr, 1, n);
}

/*
 *  RASTREO A MANO con n = 8:
 *
 *      índice: 1    2   3  4  5  6  7   8
 *      valor: -10  -5   0  1  5  7  9  12
 *
 *  ini=1, fin=8 -> mid=4 : Arr[4]=1  < 4 (atrasado)   -> derecha 5..8
 *  ini=5, fin=8 -> mid=6 : Arr[6]=7  > 6 (adelantado) -> izquierda 5..5
 *  ini=5, fin=5 -> mid=5 : Arr[5]=5 == 5              -> regresa 5   ✔
 *
 *  RASTREO cuando NO existe, n = 5:   [2, 3, 4, 5, 6]
 *  ini=1, fin=5 -> mid=3 : Arr[3]=4 > 3 -> izquierda 1..2
 *  ini=1, fin=2 -> mid=1 : Arr[1]=2 > 1 -> izquierda 1..0
 *  ini=1, fin=0 -> rango vacío          -> regresa -1   ✔
 *
 *  COMPLEJIDAD:
 *    Igual que antes: T(n) = T(n/2) + c  ->  O(log n).
 *    Mejor caso   : el primer mid ya cumple Arr[mid]==mid  -> O(1)
 *    Peor caso    : no existe; se parte hasta vaciar       -> O(log n)
 *    Caso promedio:                                        -> O(log n)
 */


/* =====================================================================
 *  main: prueba todo con los ejemplos del ejercicio
 * ===================================================================== */
int main(void) {

    /* Los tres ejemplos del pizarrón: n = 17, valores 1..16 */
    int a1[] = {1,2,3,4,5,6,7,8,8,9,10,11,12,13,14,15,16};   /* 8 repetido  */
    int a2[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,16};   /* 16 repetido */
    int a3[] = {1,2,3,3,4,5,6,7,8,9,10,11,12,13,14,15,16};   /* 3 repetido  */
    int n = 17;

    printf("===== Problema 1: elemento duplicado =====\n");
    printf("Ejemplo 1 -> secuencial: %d | divide y venceras: %d\n",
           duplicadoSecuencial(a1, n), duplicado(a1, n));
    printf("Ejemplo 2 -> secuencial: %d | divide y venceras: %d\n",
           duplicadoSecuencial(a2, n), duplicado(a2, n));
    printf("Ejemplo 3 -> secuencial: %d | divide y venceras: %d\n",
           duplicadoSecuencial(a3, n), duplicado(a3, n));

    /* Problema 2: la posición 0 no se usa (vale 0 solo de relleno) */
    int b1[] = {0, -10, -5, 0, 1, 5, 7, 9, 12};   /* n = 8, Arr[5] = 5 */
    int b2[] = {0, 2, 3, 4, 5, 6};                /* n = 5, no existe  */

    printf("\n===== Problema 2: indice con Arr[i] == i =====\n");
    printf("Ejemplo 1 -> fuerza bruta: %d | divide y venceras: %d\n",
           indiceFuerzaBruta(b1, 8), indiceIgual(b1, 8));
    printf("Ejemplo 2 -> fuerza bruta: %d | divide y venceras: %d  (-1 = no existe)\n",
           indiceFuerzaBruta(b2, 5), indiceIgual(b2, 5));

    return 0;
}