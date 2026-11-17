/*
 * Práctica 2.4 Programación dinámica I. Planificación de procesos ponderados
 * Programación dinámica recursiva (computeOpt)
 *
 * Alumnos:
 *   Álvarez Tahuilán Luis Gustavo
 *   Noyola Gómez Emilio Damian
 *
 * Grupo: 3CM1
 * Fecha de entrega: 09 de noviembre de 2026
 */

#include <stdio.h>

#define MAX_PROCESOS 64

typedef struct {
    int id;
    int inicio;
    int fin;
    int peso;
} Proceso;

long long llamadas = 0;

void intercambiar(Proceso *a, Proceso *b) {
    Proceso temporal = *a;
    *a = *b;
    *b = temporal;
}

void maxHeapify(Proceso A[], int n, int i) {
    int izquierdo = 2 * i;
    int derecho = 2 * i + 1;
    int mayor = i;

    if (izquierdo <= n && A[izquierdo].fin > A[mayor].fin) {
        mayor = izquierdo;
    }
    if (derecho <= n && A[derecho].fin > A[mayor].fin) {
        mayor = derecho;
    }
    if (mayor != i) {
        intercambiar(&A[i], &A[mayor]);
        maxHeapify(A, n, mayor);
    }
}

void heapSort(Proceso A[], int n) {
    for (int i = n / 2; i >= 1; i--) {
        maxHeapify(A, n, i);
    }
    for (int i = n; i >= 2; i--) {
        intercambiar(&A[1], &A[i]);
        maxHeapify(A, i - 1, 1);
    }
}

int buscarCompatible(Proceso A[], int j) {
    int izquierda = 1;
    int derecha = j - 1;
    int respuesta = 0;

    while (izquierda <= derecha) {
        int medio = (izquierda + derecha) / 2;
        if (A[medio].fin <= A[j].inicio) {
            respuesta = medio;
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    return respuesta;
}

int computeOpt(Proceso A[], int p[], int j) {
    llamadas++;
    if (j == 0) {
        return 0;
    }

    int sinJ = computeOpt(A, p, j - 1);
    int conJ = A[j].peso + computeOpt(A, p, p[j]);

    return (conJ > sinJ) ? conJ : sinJ;
}

void encontrarSolucion(Proceso A[], int p[], int j) {
    if (j == 0) {
        return;
    }
    if (A[j].peso + computeOpt(A, p, p[j]) > computeOpt(A, p, j - 1)) {
        encontrarSolucion(A, p, p[j]);
        printf(" %d", A[j].id);
    } else {
        encontrarSolucion(A, p, j - 1);
    }
}

void resolverCaso(const char *descripcion, Proceso A[], int n) {
    int p[MAX_PROCESOS];

    heapSort(A, n);
    for (int j = 1; j <= n; j++) {
        p[j] = buscarCompatible(A, j);
    }

    printf("%s\n", descripcion);
    printf("   j | proceso | inicio | fin | peso | p(j)\n");
    for (int j = 1; j <= n; j++) {
        printf("  %2d | %7d | %6d | %3d | %4d | %4d\n", j, A[j].id, A[j].inicio, A[j].fin, A[j].peso, p[j]);
    }

    llamadas = 0;
    int optimo = computeOpt(A, p, n);
    printf("  Peso maximo OPT(%d) = %d   (llamadas a computeOpt: %lld)\n", n, optimo, llamadas);
    printf("  Procesos elegidos:");
    encontrarSolucion(A, p, n);
    printf("\n\n");
}

long long contarLlamadas(int n, int tipo) {
    Proceso A[MAX_PROCESOS];
    int p[MAX_PROCESOS];

    for (int j = 1; j <= n; j++) {
        A[j].id = j;
        A[j].peso = 1;
        if (tipo == 1) {
            A[j].inicio = 0;
            A[j].fin = j;
        } else if (tipo == 2) {
            A[j].inicio = 2 * (j - 1);
            A[j].fin = 2 * (j - 1) + 3;
        } else {
            A[j].inicio = j - 1;
            A[j].fin = j;
        }
    }
    for (int j = 1; j <= n; j++) {
        p[j] = buscarCompatible(A, j);
    }

    llamadas = 0;
    computeOpt(A, p, n);
    return llamadas;
}

int main(void) {
    printf("PLANIFICACION DE PROCESOS PONDERADOS (PROGRAMACION DINAMICA RECURSIVA)\n\n");

    Proceso caso1[] = {{0, 0, 0, 0},
                       {1, 0, 3, 20}, {2, 2, 5, 10}, {3, 4, 7, 50},
                       {4, 6, 9, 5}, {5, 8, 11, 30}};
    resolverCaso("Caso 1: ejemplo visto en el laboratorio (5 procesos)", caso1, 5);

    Proceso caso2[] = {{0, 0, 0, 0},
                       {1, 0, 6, 8}, {2, 1, 4, 2}, {3, 3, 5, 4}, {4, 3, 8, 7},
                       {5, 4, 7, 3}, {6, 5, 9, 5}, {7, 6, 10, 6}, {8, 8, 11, 4}};
    resolverCaso("Caso 2: 8 procesos con pesos distintos", caso2, 8);

    printf("Caso 3: crecimiento del numero de llamadas\n");
    printf("   n | traslapados, p(j) = 0 | en capas, p(j) = j - 2 | compatibles, p(j) = j - 1\n");
    for (int n = 5; n <= 25; n += 5) {
        printf("  %2d | %21lld | %22lld | %25lld\n", n, contarLlamadas(n, 1), contarLlamadas(n, 2),
               contarLlamadas(n, 3));
    }

    return 0;
}
