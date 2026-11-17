#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANO 100000

long long contarFuerzaBruta(int arreglo[], int n) {
    long long inversiones = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arreglo[i] > arreglo[j]) {
                inversiones++;
            }
        }
    }

    return inversiones;
}

long long mezclaYCuenta(int arreglo[], int auxiliar[], int inicio, int mitad, int fin) {
    int i = inicio;
    int j = mitad + 1;
    int k = inicio;
    long long inversiones = 0;

    while (i <= mitad && j <= fin) {
        if (arreglo[i] <= arreglo[j]) {
            auxiliar[k] = arreglo[i];
            i++;
        } else {
            auxiliar[k] = arreglo[j];
            j++;
            inversiones += mitad - i + 1;
        }
        k++;
    }

    while (i <= mitad) {
        auxiliar[k] = arreglo[i];
        i++;
        k++;
    }

    while (j <= fin) {
        auxiliar[k] = arreglo[j];
        j++;
        k++;
    }

    for (k = inicio; k <= fin; k++) {
        arreglo[k] = auxiliar[k];
    }

    return inversiones;
}

long long ordenaYCuenta(int arreglo[], int auxiliar[], int inicio, int fin) {
    if (inicio >= fin) {
        return 0;
    }

    int mitad = inicio + (fin - inicio) / 2;

    long long rA = ordenaYCuenta(arreglo, auxiliar, inicio, mitad);
    long long rB = ordenaYCuenta(arreglo, auxiliar, mitad + 1, fin);
    long long rAB = mezclaYCuenta(arreglo, auxiliar, inicio, mitad, fin);

    return rA + rB + rAB;
}

long long contarDivideYVenceras(int arreglo[], int n) {
    int *auxiliar = malloc(n * sizeof(int));
    long long inversiones = ordenaYCuenta(arreglo, auxiliar, 0, n - 1);

    free(auxiliar);
    return inversiones;
}

int main(void) {
    int ejemploA[] = {1, 5, 4, 8, 10, 2, 6, 9, 3, 7};
    int ejemploB[] = {1, 5, 4, 8, 10, 2, 6, 9, 3, 7};

    printf("CONTEO DE INVERSIONES: FUERZA BRUTA Y DIVIDE Y VENCERAS\n\n");

    printf("Comprobacion con el arreglo [1, 5, 4, 8, 10, 2, 6, 9, 3, 7]\n");
    printf("  Fuerza bruta:      %lld inversiones\n", contarFuerzaBruta(ejemploA, 10));
    printf("  Divide y venceras: %lld inversiones\n\n", contarDivideYVenceras(ejemploB, 10));

    int *peorCaso = malloc(TAMANO * sizeof(int));
    int *copia = malloc(TAMANO * sizeof(int));

    for (int i = 0; i < TAMANO; i++) {
        peorCaso[i] = TAMANO - i;
        copia[i] = peorCaso[i];
    }

    printf("Peor caso: %d elementos en orden descendente (%d, %d, ..., 2, 1)\n\n", TAMANO, TAMANO, TAMANO - 1);

    clock_t inicio = clock();
    long long resultadoBruta = contarFuerzaBruta(peorCaso, TAMANO);
    clock_t fin = clock();
    double msBruta = (double)(fin - inicio) * 1000.0 / CLOCKS_PER_SEC;

    inicio = clock();
    long long resultadoDivide = contarDivideYVenceras(copia, TAMANO);
    fin = clock();
    double msDivide = (double)(fin - inicio) * 1000.0 / CLOCKS_PER_SEC;

    printf("%-20s %15s %15s\n", "Metodo", "Inversiones", "Tiempo (ms)");
    printf("%-20s %15lld %15.3f\n", "Fuerza bruta", resultadoBruta, msBruta);
    printf("%-20s %15lld %15.3f\n\n", "Divide y venceras", resultadoDivide, msDivide);

    if (resultadoBruta == resultadoDivide) {
        printf("Los dos metodos obtienen el mismo resultado.\n");
    } else {
        printf("Los resultados no coinciden.\n");
    }

    if (msDivide > 0) {
        printf("Divide y venceras fue %.0f veces mas rapido.\n", msBruta / msDivide);
    }

    free(peorCaso);
    free(copia);

    return 0;
}
