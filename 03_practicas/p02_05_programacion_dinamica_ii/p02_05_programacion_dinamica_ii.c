/*
 * Práctica 2.5 Programación dinámica II
 * Problema de la mochila 0/1: top-down (recursiva pura), con memoización e iterativa
 *
 * Alumnos:
 *   Álvarez Tahuilán Luis Gustavo
 *   Noyola Gómez Emilio Damian
 *
 * Grupo: 3CM1
 * Fecha de entrega: 23 de noviembre de 2026
 */

#include <stdio.h>

#define MAX_N 32
#define MAX_W 128
#define VACIO -1

int n, W;
int valor[MAX_N + 1];
int peso[MAX_N + 1];
int M[MAX_N + 1][MAX_W + 1];
long long llamadas = 0;

int maximo(int a, int b) {
    return (a > b) ? a : b;
}

int mochilaRecursiva(int i, int w) {
    llamadas++;
    if (i == 0) {
        return 0;
    }
    if (peso[i] > w) {
        return mochilaRecursiva(i - 1, w);
    }
    return maximo(mochilaRecursiva(i - 1, w),
                  valor[i] + mochilaRecursiva(i - 1, w - peso[i]));
}

int mochilaMemo(int i, int w) {
    llamadas++;
    if (i == 0) {
        return 0;
    }
    if (M[i][w] != VACIO) {
        return M[i][w];
    }
    if (peso[i] > w) {
        M[i][w] = mochilaMemo(i - 1, w);
    } else {
        M[i][w] = maximo(mochilaMemo(i - 1, w),
                         valor[i] + mochilaMemo(i - 1, w - peso[i]));
    }
    return M[i][w];
}

int mochilaIterativa(void) {
    for (int w = 0; w <= W; w++) {
        M[0][w] = 0;
    }
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (peso[i] > w) {
                M[i][w] = M[i - 1][w];
            } else {
                M[i][w] = maximo(M[i - 1][w], valor[i] + M[i - 1][w - peso[i]]);
            }
        }
    }
    return M[n][W];
}

int encontrarSolucion(int elegidos[]) {
    int k = 0;
    int w = W;
    for (int i = n; i >= 1; i--) {
        if (M[i][w] > M[i - 1][w]) {
            elegidos[k] = i;
            k++;
            w -= peso[i];
        }
    }
    return k;
}

void imprimirSolucion(void) {
    int elegidos[MAX_N];
    int k = encontrarSolucion(elegidos);
    int pesoTotal = 0;
    int valorTotal = 0;

    printf("  Conjunto solucion: {");
    for (int j = k - 1; j >= 0; j--) {
        printf(" %d", elegidos[j]);
        pesoTotal += peso[elegidos[j]];
        valorTotal += valor[elegidos[j]];
    }
    printf(" }\n");
    for (int j = k - 1; j >= 0; j--) {
        printf("    objeto %d: valor = %d, peso = %d\n", elegidos[j], valor[elegidos[j]], peso[elegidos[j]]);
    }
    printf("  Valor total = %d, peso total = %d de %d\n", valorTotal, pesoTotal, W);
}

void limpiarTabla(void) {
    for (int w = 0; w <= W; w++) {
        M[0][w] = 0;
    }
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            M[i][w] = VACIO;
        }
    }
}

int contarCeldas(void) {
    int celdas = 0;
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (M[i][w] != VACIO) {
                celdas++;
            }
        }
    }
    return celdas;
}

void imprimirTabla(const char *titulo) {
    printf("  %s\n          w:", titulo);
    for (int w = 0; w <= W; w++) {
        printf("%4d", w);
    }
    printf("\n");
    for (int i = 0; i <= n; i++) {
        printf("    i = %d   ", i);
        for (int w = 0; w <= W; w++) {
            if (M[i][w] == VACIO) {
                printf("   -");
            } else {
                printf("%4d", M[i][w]);
            }
        }
        printf("\n");
    }
}

void cargarInstancia(int tamano) {
    n = tamano;
    W = 2 * tamano;
    for (int i = 1; i <= n; i++) {
        peso[i] = (7 * i) % 10 + 1;
        valor[i] = (13 * i) % 20 + 1;
    }
}

int main(void) {
    int v[] = {0, 1, 6, 18, 22, 28};
    int p[] = {0, 1, 2, 5, 6, 7};
    n = 5;
    W = 11;
    for (int i = 1; i <= n; i++) {
        valor[i] = v[i];
        peso[i] = p[i];
    }

    printf("PROBLEMA DE LA MOCHILA 0/1 (n = %d objetos, capacidad W = %d)\n\n", n, W);

    llamadas = 0;
    int r1 = mochilaRecursiva(n, W);
    printf("1. Top-down (recursiva pura):  OPT(%d, %d) = %d   (llamadas: %lld)\n\n", n, W, r1, llamadas);

    limpiarTabla();
    llamadas = 0;
    int r2 = mochilaMemo(n, W);
    printf("2. Con memoizacion:            OPT(%d, %d) = %d   (llamadas: %lld, celdas calculadas: %d de %d)\n",
           n, W, r2, llamadas, contarCeldas(), n * (W + 1));
    imprimirTabla("Tabla M (- = celda que nunca se calculo):");
    imprimirSolucion();
    printf("\n");

    int r3 = mochilaIterativa();
    printf("3. Iterativa:                  OPT(%d, %d) = %d   (celdas calculadas: %d)\n", n, W, r3, n * (W + 1));
    imprimirTabla("Tabla M:");
    imprimirSolucion();
    printf("\n");

    printf("Crecimiento del trabajo (w_i = (7i mod 10) + 1, v_i = (13i mod 20) + 1, W = 2n)\n");
    printf("   n |  W | recursiva (llamadas) | memoizacion (llamadas) | iterativa (celdas)\n");
    for (int t = 5; t <= 25; t += 5) {
        cargarInstancia(t);
        llamadas = 0;
        int a = mochilaRecursiva(n, W);
        long long recursivas = llamadas;
        limpiarTabla();
        llamadas = 0;
        int b = mochilaMemo(n, W);
        long long memo = llamadas;
        int c = mochilaIterativa();
        printf("  %2d | %2d | %20lld | %22lld | %18d   OPT = %d%s\n", n, W, recursivas, memo,
               n * (W + 1), c, (a == b && b == c) ? "" : "  (diferencia)");
    }

    return 0;
}
