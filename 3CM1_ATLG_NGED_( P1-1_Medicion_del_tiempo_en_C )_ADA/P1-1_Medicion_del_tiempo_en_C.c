/* 
    Práctica 1.1: Medición del tiempo en C
    Alumnos: 
     - Álvarez Tahuilán Luis Gustavo
     - Noyola Gómez Emilio Damian
    Fecha: 09 de septiembre de 2026
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_TAMANOS 11
#define ELEMENTOS_MOVIDOS 10


int compararEnteros(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;

    if (x < y)
        return -1;

    if (x > y)
        return 1;

    return 0;
}


void selectionSort(int arreglo[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int posicionMenor = i;

        for (int j = i + 1; j < n; j++) {
            if (arreglo[j] < arreglo[posicionMenor]) {
                posicionMenor = j;
            }
        }

        if (posicionMenor != i) {
            int auxiliar = arreglo[i];
            arreglo[i] = arreglo[posicionMenor];
            arreglo[posicionMenor] = auxiliar;
        }
    }
}


void generarAleatorio(int arreglo[], int n)
{
    for (int i = 0; i < n; i++)
    {
        arreglo[i] = rand();
    }
}


void invertirArreglo(int arreglo[], int n)
{
    for (int i = 0; i < n / 2; i++)
    {
        int auxiliar = arreglo[i];

        arreglo[i] = arreglo[n - 1 - i];
        arreglo[n - 1 - i] = auxiliar;
    }
}


void casiOrdenar(int arreglo[], int n)
{
    int posiciones[ELEMENTOS_MOVIDOS];

    for (int i = 0; i < ELEMENTOS_MOVIDOS; i++)
    {
        int posicion;
        int repetida;

        do
        {
            posicion = rand() % n;
            repetida = 0;

            for (int j = 0; j < i; j++)
            {
                if (posiciones[j] == posicion)
                {
                    repetida = 1;
                    break;
                }
            }

        } while (repetida);

        posiciones[i] = posicion;
    }

    int auxiliar = arreglo[posiciones[ELEMENTOS_MOVIDOS - 1]];

    for (int i = ELEMENTOS_MOVIDOS - 1; i > 0; i--)
    {
        arreglo[posiciones[i]] = arreglo[posiciones[i - 1]];
    }

    arreglo[posiciones[0]] = auxiliar;
}


int estaOrdenado(const int arreglo[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        if (arreglo[i] > arreglo[i + 1])
        {
            return 0;
        }
    }

    return 1;
}


void ejecutarPrueba(int arreglo[], int n, const char *condicion)
{
    printf("\n----------------------------------------\n");
    printf("Tamano: %d\n", n);
    printf("Condicion: %s\n", condicion);

    clock_t inicio = clock();

    selectionSort(arreglo, n);

    clock_t fin = clock();

    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;

    int correcto = estaOrdenado(arreglo, n);

    printf("Tiempo: %.6f segundos\n", tiempo);
    printf("Ordenamiento correcto: %s\n", correcto ? "SI" : "NO");
}


int main(void)
{
    int tamanos[NUM_TAMANOS] =
    {
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
    };

    srand(12345);

    printf("========================================\n");
    printf("   SELECTION SORT - PRUEBAS DE TIEMPO\n");
    printf("========================================\n");

    for (int t = 0; t < NUM_TAMANOS; t++)
    {
        int n = tamanos[t];

        printf("\n\n========================================\n");
        printf("        PRUEBAS PARA N = %d\n", n);
        printf("========================================\n");

        int *base = (int *)malloc((size_t)n * sizeof(int));

        int *trabajo = (int *)malloc((size_t)n * sizeof(int));

        if (base == NULL || trabajo == NULL)
        {
            printf("Error al reservar memoria.\n");

            free(base);
            free(trabajo);

            return 1;
        }


        // Caso 1:
        generarAleatorio(base, n);
        memcpy(trabajo, base, (size_t)n * sizeof(int));
        ejecutarPrueba(trabajo, n, "Aleatorio");

        // Ordenar arreglo
        qsort(base, n, sizeof(int), compararEnteros);


        // Caso 2:
        memcpy(trabajo, base, (size_t)n * sizeof(int));
        invertirArreglo(trabajo, n);
        ejecutarPrueba(trabajo, n, "Inverso");


        // Caso 3:
        memcpy(trabajo, base, (size_t)n * sizeof(int));
        ejecutarPrueba(trabajo, n, "Ordenado");


        // Caso 4:
        memcpy(trabajo, base, (size_t)n * sizeof(int));
        casiOrdenar(trabajo, n);
        ejecutarPrueba(trabajo, n, "Casi ordenado");


        free(base);
        free(trabajo);
    }

    return 0;
}