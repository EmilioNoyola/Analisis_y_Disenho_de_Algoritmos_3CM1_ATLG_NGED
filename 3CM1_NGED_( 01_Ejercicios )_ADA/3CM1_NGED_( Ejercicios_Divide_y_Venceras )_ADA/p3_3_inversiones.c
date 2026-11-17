/*
 * ============================================================================
 *  Práctica 3 · Problema 3 - Conteo de inversiones
 *  Análisis y Diseño de Algoritmos - ESCOM IPN
 * ----------------------------------------------------------------------------
 *  Dos elementos A[i] y A[j] forman una INVERSIÓN si  i < j  y  A[i] > A[j].
 *  El número de inversiones mide qué tan lejos está el arreglo de estar
 *  ordenado: 0 si ya está ordenado, n(n-1)/2 si está en orden inverso.
 *
 *  MÉTODO DIRECTO
 *    Revisar todos los pares (i, j) con i < j.
 *    n(n-1)/2 comparaciones  →  O(n²)
 *
 *  DIVIDE Y VENCERÁS  ·  Algoritmo de ordenamiento analizado: MERGE SORT
 *    inv(A) = inv(mitad izq) + inv(mitad der) + inv(cruzadas)
 *    Las cruzadas se cuentan DURANTE la mezcla (merge): si al mezclar se toma
 *    un elemento de la derecha R[j] porque es menor que L[i], entonces R[j]
 *    también es menor que L[i], L[i+1], ..., L[fin] (la izquierda ya está
 *    ordenada). Eso suma (elementos que quedan en la izquierda) inversiones
 *    de un solo golpe.
 *    T(n) = 2T(n/2) + O(n)   →   O(n log n)
 *
 *  Compilar:  gcc -O2 p3_3_inversiones.c -o inversiones
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RST "\033[0m"
#define NEG "\033[1m"
#define CYN "\033[36m"
#define VRD "\033[32m"
#define AMA "\033[33m"
#define ROJ "\033[31m"
#define GRS "\033[90m"

typedef long long ll;

/* ------------------------------ MÉTODO DIRECTO ---------------------------- */
ll directo(const int *a, int n, int listar)
{
    ll inv = 0;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] > a[j]) {
                inv++;
                if (listar) printf("    (%d, %d)  " GRS "A[%d]=%d > A[%d]=%d" RST "\n",
                                   a[i], a[j], i, a[i], j, a[j]);
            }
    return inv;
}

/* -------------------- DIVIDE Y VENCERÁS: Merge Sort ---------------------- */
static int nivel_traza = -1;       /* >= 0 → imprimir la traza (arreglos chicos) */

/* Mezcla a[lo..mid] y a[mid+1..hi] (ya ordenados) y cuenta inversiones cruzadas */
static ll mezclar_contar(int *a, int *aux, int lo, int mid, int hi)
{
    ll cruzadas = 0;
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (a[i] <= a[j]) aux[k++] = a[i++];          /* <= : iguales NO son inversión */
        else {
            aux[k++] = a[j++];
            cruzadas += mid - i + 1;                   /* todos los que quedan a la izq */
        }
    }
    while (i <= mid) aux[k++] = a[i++];
    while (j <= hi)  aux[k++] = a[j++];
    memcpy(a + lo, aux + lo, (size_t)(hi - lo + 1) * sizeof(int));
    return cruzadas;
}

static void imprimir_rango(const int *a, int lo, int hi)
{
    printf("[");
    for (int i = lo; i <= hi; i++) printf("%d%s", a[i], i < hi ? " " : "");
    printf("]");
}

static ll merge_sort_contar(int *a, int *aux, int lo, int hi, int prof)
{
    if (lo >= hi) return 0;                            /* 0 o 1 elemento */
    int mid = lo + (hi - lo) / 2;
    ll izq = merge_sort_contar(a, aux, lo, mid, prof + 1);
    ll der = merge_sort_contar(a, aux, mid + 1, hi, prof + 1);
    if (nivel_traza >= 0) {
        printf("  %*s", prof * 2, "");
        imprimir_rango(a, lo, mid); printf(" + "); imprimir_rango(a, mid + 1, hi);
    }
    ll cru = mezclar_contar(a, aux, lo, mid, hi);
    if (nivel_traza >= 0) {
        printf("  →  "); imprimir_rango(a, lo, hi);
        printf("   " AMA "cruzadas = %lld" RST "\n", cru);
    }
    return izq + der + cru;
}

ll dyv(const int *orig, int n)
{
    int *a = malloc(n * sizeof(int)), *aux = malloc(n * sizeof(int));
    memcpy(a, orig, n * sizeof(int));                  /* no se altera el original */
    ll r = merge_sort_contar(a, aux, 0, n - 1, 0);
    free(a); free(aux);
    return r;
}

/* ------------------------------- UTILIDADES ------------------------------- */
int leer_entero(const char *msg, int min, int max)
{
    int x, c;
    for (;;) {
        printf("%s", msg);
        if (scanf("%d", &x) == 1 && x >= min && x <= max) {
            while ((c = getchar()) != '\n' && c != EOF) ;
            return x;
        }
        while ((c = getchar()) != '\n' && c != EOF) ;
        printf(ROJ "  Entrada inválida (entero entre %d y %d).\n" RST, min, max);
    }
}

ll directo_sin_listar(const int *a, int n) { return directo(a, n, 0); }

double medir(ll (*f)(const int *, int), const int *a, int n, ll *res)
{
    int rep = 0; clock_t t0 = clock(), t;
    do { *res = f(a, n); rep++; t = clock(); } while (t - t0 < CLOCKS_PER_SEC / 20);
    return 1000.0 * (double)(t - t0) / CLOCKS_PER_SEC / rep;
}

void resolver_y_mostrar(const int *a, int n)
{
    int chico = n <= 12;
    if (chico) {
        printf("\n  A = "); imprimir_rango(a, 0, n - 1); printf("\n");
        printf("\n  " NEG "Directo: pares invertidos" RST "\n");
        directo(a, n, 1);
        printf("\n  " NEG "Merge Sort: traza de las mezclas" RST "\n");
        nivel_traza = 0; dyv(a, n); nivel_traza = -1;
    }
    ll d, r;
    double td = medir(directo_sin_listar, a, n, &d);
    double tr = medir(dyv, a, n, &r);
    printf("\n  ┌──────────────────────────────┬──────────────────┬────────────┐\n");
    printf("  │ " NEG "Método" RST "                       │ " NEG "Inversiones" RST "      │ " NEG "Tiempo ms" RST "  │\n");
    printf("  ├──────────────────────────────┼──────────────────┼────────────┤\n");
    printf("  │ Directo         O(n²)        │ %16lld │ %10.4f │\n", d, td);
    printf("  │ Merge Sort      O(n log n)   │ %16lld │ %10.4f │\n", r, tr);
    printf("  └──────────────────────────────┴──────────────────┴────────────┘\n");
    printf("  %s\n", d == r ? VRD "✔ Ambos métodos coinciden." RST : ROJ "✘ No coinciden." RST);
    printf("  " GRS "Máximo posible para n = %d: n(n-1)/2 = %lld" RST "\n", n, (ll)n * (n - 1) / 2);
}

void llenar_aleatorio(int *a, int n) { for (int i = 0; i < n; i++) a[i] = rand() % 1000; }

void tabla_tiempos(void)
{
    int tams[] = {1000, 5000, 10000, 50000, 100000};
    printf("\n  ┌──────────┬──────────────┬──────────────┬──────────┐\n");
    printf("  │ " NEG "n" RST "        │ " NEG "Directo ms" RST "   │ " NEG "Merge ms" RST "     │ " NEG "¿Igual?" RST "  │\n");
    printf("  ├──────────┼──────────────┼──────────────┼──────────┤\n");
    for (int k = 0; k < 5; k++) {
        int n = tams[k], *a = malloc(n * sizeof(int));
        llenar_aleatorio(a, n);
        ll d, r;
        double td = medir(directo_sin_listar, a, n, &d);
        double tr = medir(dyv, a, n, &r);
        printf("  │ %8d │ %12.3f │ %12.3f │ %s │\n", n, td, tr,
               d == r ? VRD "   sí   " RST : ROJ "   no   " RST);
        fflush(stdout);
        free(a);
    }
    printf("  └──────────┴──────────────┴──────────────┴──────────┘\n");
}

int main(void)
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    srand((unsigned)time(NULL));
    printf(CYN "\n  ╔════════════════════════════════════════════════════╗\n");
    printf("  ║" RST NEG "   CONTEO DE INVERSIONES                             " RST CYN "║\n");
    printf("  ║" RST "   Directo O(n²)  vs.  Merge Sort O(n log n)        " CYN "║\n");
    printf("  ╚════════════════════════════════════════════════════╝" RST "\n");

    int op;
    do {
        printf("\n  " NEG "Menú" RST "\n");
        printf("   1) Capturar el arreglo a mano\n");
        printf("   2) Arreglo aleatorio\n");
        printf("   3) Ejemplo de clase {1, 9, 6, 4, 5}\n");
        printf("   4) Tabla de tiempos (para el reporte)\n");
        printf("   0) Salir\n");
        op = leer_entero("  Opción: ", 0, 4);
        if (op >= 1 && op <= 3) {
            int n, *a;
            if (op == 3) {
                static int ej[] = {1, 9, 6, 4, 5};
                n = 5; a = malloc(n * sizeof(int)); memcpy(a, ej, sizeof ej);
            } else {
                n = leer_entero("\n  n (tamaño del arreglo): ", 1, 200000);
                a = malloc(n * sizeof(int));
                if (op == 1) {
                    char msg[40];
                    for (int i = 0; i < n; i++) { sprintf(msg, "  A[%d] = ", i); a[i] = leer_entero(msg, -1000000, 1000000); }
                } else llenar_aleatorio(a, n);
            }
            resolver_y_mostrar(a, n);
            free(a);
        } else if (op == 4) tabla_tiempos();
    } while (op != 0);
    printf("\n  Fin del programa.\n\n");
    return 0;
}
