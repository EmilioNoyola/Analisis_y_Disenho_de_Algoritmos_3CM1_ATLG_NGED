/*
 * Práctica 2.3 Algoritmos ávidos I
 * Programa 1: Planificación de eventos en un auditorio (earliest-finish-time-first)
 *
 * Alumnos:
 *   Álvarez Tahuilán Luis Gustavo
 *   Noyola Gómez Emilio Damian
 *
 * Grupo: 3CM1
 * Fecha de entrega: 28 de octubre de 2026
 */

#include <stdio.h>
#include <limits.h>

#define MAX_EVENTOS 50

typedef struct {
    char nombre;
    int inicio;
    int fin;
} Evento;

void intercambiar(Evento *a, Evento *b) {
    Evento temporal = *a;
    *a = *b;
    *b = temporal;
}

void maxHeapify(Evento A[], int n, int i) {
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

void construirMaxHeap(Evento A[], int n) {
    for (int i = n / 2; i >= 1; i--) {
        maxHeapify(A, n, i);
    }
}

void heapSort(Evento A[], int n) {
    construirMaxHeap(A, n);
    for (int i = n; i >= 2; i--) {
        intercambiar(&A[1], &A[i]);
        maxHeapify(A, i - 1, 1);
    }
}

int planificarEventos(Evento A[], int n, Evento S[]) {
    heapSort(A, n);

    int k = 0;
    int ultimoFin = INT_MIN;
    for (int j = 1; j <= n; j++) {
        if (A[j].inicio >= ultimoFin) {
            S[k] = A[j];
            k++;
            ultimoFin = A[j].fin;
        }
    }
    return k;
}

void imprimirEventos(const char *titulo, Evento A[], int desde, int hasta) {
    printf("  %s\n   ", titulo);
    for (int i = desde; i <= hasta; i++) {
        printf(" %c[%d,%d)", A[i].nombre, A[i].inicio, A[i].fin);
    }
    printf("\n");
}

void resolverCaso(const char *descripcion, Evento A[], int n) {
    Evento S[MAX_EVENTOS];

    printf("%s\n", descripcion);
    imprimirEventos("Solicitudes:", A, 1, n);
    int k = planificarEventos(A, n, S);
    imprimirEventos("Ordenadas por hora de fin (heapsort):", A, 1, n);
    imprimirEventos("Eventos programados en el auditorio:", S, 0, k - 1);
    printf("  Total: %d de %d eventos\n\n", k, n);
}

int main(void) {
    printf("PLANIFICACION DE EVENTOS EN UN AUDITORIO (EARLIEST-FINISH-TIME-FIRST)\n\n");

    int valores[] = {7, 5, 20, 1, 3, 8, 14, 16, 23, 15};
    Evento prueba[11];
    for (int i = 1; i <= 10; i++) {
        prueba[i].nombre = ' ';
        prueba[i].inicio = 0;
        prueba[i].fin = valores[i - 1];
    }
    construirMaxHeap(prueba, 10);
    printf("Comprobacion de construirMaxHeap con el arreglo visto en clase\n");
    printf("  Entrada: 7 5 20 1 3 8 14 16 23 15\n  Max-heap:");
    for (int i = 1; i <= 10; i++) {
        printf(" %d", prueba[i].fin);
    }
    printf("\n\n");

    Evento caso1[] = {{0, 0, 0},
                      {'a', 0, 6}, {'b', 1, 4}, {'c', 3, 5}, {'d', 3, 8},
                      {'e', 4, 7}, {'f', 5, 9}, {'g', 6, 10}, {'h', 8, 11}};
    resolverCaso("Caso 1: ejemplo visto en clase (8 solicitudes)", caso1, 8);

    Evento caso2[] = {{0, 0, 0},
                      {'a', 1, 4}, {'b', 3, 5}, {'c', 0, 6}, {'d', 5, 7},
                      {'e', 3, 9}, {'f', 5, 9}, {'g', 6, 10}, {'h', 8, 11},
                      {'i', 8, 12}, {'j', 2, 14}, {'k', 12, 16}};
    resolverCaso("Caso 2: 11 solicitudes", caso2, 11);

    return 0;
}
