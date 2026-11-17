#include <stdio.h>

long long contarInversiones(int arreglo[], int n) {
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

void imprimirArreglo(int arreglo[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arreglo[i]);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("]");
}

void probar(const char *nombre, int arreglo[], int n) {
    long long maximo = (long long)n * (n - 1) / 2;

    printf("%s\n", nombre);
    printf("  Arreglo:     ");
    imprimirArreglo(arreglo, n);
    printf("\n");
    printf("  Inversiones: %lld de %lld posibles\n\n", contarInversiones(arreglo, n), maximo);
}

int main(void) {
    int ordenado[] = {1, 2, 3, 4, 5};
    int canciones[] = {1, 3, 4, 2, 5};
    int entrada[] = {1, 5, 4, 8, 10, 2, 6, 9, 3, 7};
    int invertido[] = {5, 4, 3, 2, 1};

    printf("CONTEO DE INVERSIONES POR FUERZA BRUTA\n\n");

    probar("Caso 1: arreglo ordenado", ordenado, 5);
    probar("Caso 2: ranking de canciones", canciones, 5);
    probar("Caso 3: entrada de 10 elementos", entrada, 10);
    probar("Caso 4: arreglo en orden inverso", invertido, 5);

    return 0;
}
