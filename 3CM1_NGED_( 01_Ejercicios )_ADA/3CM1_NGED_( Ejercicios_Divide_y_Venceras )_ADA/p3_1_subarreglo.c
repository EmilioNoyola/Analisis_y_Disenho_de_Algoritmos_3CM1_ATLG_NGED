/*
 * ============================================================================
 *  Práctica 3 · Problema 1 - Sub-arreglo de suma máxima de longitud m
 *  Análisis y Diseño de Algoritmos - ESCOM IPN
 * ----------------------------------------------------------------------------
 *  Dado un vector A de longitud n (valores positivos y negativos) y un m<=n,
 *  encontrar el sub-arreglo de EXACTAMENTE m elementos consecutivos cuya
 *  suma sea máxima. Salida: la suma, el índice de inicio y el índice de fin.
 *
 *  MÉTODO DIRECTO (fuerza bruta)
 *    Probar cada inicio i = 0..n-m y sumar sus m elementos.
 *    T(n) = (n-m+1)·m  →  O(n·m)
 *
 *  DIVIDE Y VENCERÁS
 *    Dividir A[lo..hi] a la mitad en mid. La mejor ventana de tamaño m:
 *      (a) está completa en la mitad izquierda   → recursión
 *      (b) está completa en la mitad derecha     → recursión
 *      (c) CRUZA el centro (contiene mid y mid+1) → se calcula directo:
 *          a lo más m-1 ventanas; se suma la primera en O(m) y las demás
 *          se obtienen "deslizando" (+ el que entra, − el que sale).
 *    Caso base: segmento con menos de m elementos → no hay ventana;
 *               segmento con exactamente m        → su suma.
 *    T(n) = 2T(n/2) + O(m)   →   O(n)   (ver análisis en el reporte)
 *
 *  Empates: ambos métodos reportan la ventana que empieza más a la izquierda,
 *  así sus resultados coinciden exactamente.
 *  Índices: base 0 (el primer elemento es A[0]).
 *
 *  Compilar:  gcc p3_1_subarreglo.c -o subarreglo
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define RST "\033[0m"
#define NEG "\033[1m"
#define CYN "\033[36m"
#define VRD "\033[32m"
#define AMA "\033[33m"
#define ROJ "\033[31m"
#define GRS "\033[90m"

typedef struct {
    long long suma;   /* LLONG_MIN = "no existe ventana" */
    int ini, fin;
} Ventana;

static const Ventana NINGUNA = {LLONG_MIN, -1, -1};

/* ------------------------------ MÉTODO DIRECTO ---------------------------- */
Ventana directo(const int *a, int n, int m)
{
    Ventana mejor = NINGUNA;
    for (int i = 0; i + m <= n; i++) {
        long long s = 0;
        for (int j = i; j < i + m; j++) s += a[j];
        if (s > mejor.suma) { mejor.suma = s; mejor.ini = i; mejor.fin = i + m - 1; }
    }
    return mejor;
}

/* ---------------------------- DIVIDE Y VENCERÁS --------------------------- */
/* Mejor ventana de tamaño m que contiene a mid y a mid+1, dentro de [lo,hi]. */
static Ventana cruce(const int *a, int lo, int mid, int hi, int m)
{
    Ventana mejor = NINGUNA;
    int s_ini = mid - m + 2;            /* la ventana debe llegar a mid+1 */
    int s_fin = mid;                    /* y debe empezar a más tardar en mid */
    if (s_ini < lo) s_ini = lo;
    if (s_fin > hi - m + 1) s_fin = hi - m + 1;
    if (s_ini > s_fin) return mejor;    /* (pasa cuando m == 1) */

    long long s = 0;
    for (int j = s_ini; j < s_ini + m; j++) s += a[j];      /* primera: O(m) */
    mejor.suma = s; mejor.ini = s_ini; mejor.fin = s_ini + m - 1;
    for (int i = s_ini + 1; i <= s_fin; i++) {              /* deslizar */
        s += a[i + m - 1] - a[i - 1];
        if (s > mejor.suma) { mejor.suma = s; mejor.ini = i; mejor.fin = i + m - 1; }
    }
    return mejor;
}

Ventana dyv(const int *a, int lo, int hi, int m)
{
    int len = hi - lo + 1;
    if (len < m) return NINGUNA;                     /* no cabe ninguna ventana */
    if (len == m) {                                  /* caso base */
        Ventana v = {0, lo, hi};
        for (int j = lo; j <= hi; j++) v.suma += a[j];
        return v;
    }
    int mid = lo + (hi - lo) / 2;
    Ventana izq = dyv(a, lo, mid, m);                /* DIVIDIR y conquistar */
    Ventana cen = cruce(a, lo, mid, hi, m);
    Ventana der = dyv(a, mid + 1, hi, m);

    /* COMBINAR: el mayor; en empate gana el de inicio menor (izq < cen < der) */
    Ventana mejor = izq;
    if (cen.suma > mejor.suma) mejor = cen;
    if (der.suma > mejor.suma) mejor = der;
    return mejor;
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

void imprimir_arreglo(const int *a, int n, Ventana v)
{
    if (n > 40) { printf(GRS "  (arreglo de %d elementos, no se imprime)\n" RST, n); return; }
    printf("  idx: ");
    for (int i = 0; i < n; i++) printf(GRS "%5d" RST, i);
    printf("\n  A  : ");
    for (int i = 0; i < n; i++) {
        if (i >= v.ini && i <= v.fin) printf(VRD NEG "%5d" RST, a[i]);
        else printf("%5d", a[i]);
    }
    printf("\n");
}

double medir(Ventana (*f)(const int *, int, int), const int *a, int n, int m, Ventana *res)
{
    int rep = 0; clock_t t0 = clock(), t;
    do { *res = f(a, n, m); rep++; t = clock(); } while (t - t0 < CLOCKS_PER_SEC / 20);
    return 1000.0 * (double)(t - t0) / CLOCKS_PER_SEC / rep;   /* ms por corrida */
}
Ventana dyv_envoltura(const int *a, int n, int m) { return dyv(a, 0, n - 1, m); }

void resolver_y_mostrar(const int *a, int n, int m)
{
    Ventana d, r;
    double td = medir(directo, a, n, m, &d);
    double tr = medir(dyv_envoltura, a, n, m, &r);

    printf("\n");
    imprimir_arreglo(a, n, r);
    printf("\n  ┌──────────────────────┬──────────────┬────────┬────────┬────────────┐\n");
    printf("  │ " NEG "Método" RST "               │ " NEG "Suma máxima" RST "  │ " NEG "Inicio" RST " │ " NEG "Fin" RST "    │ " NEG "Tiempo ms" RST "  │\n");
    printf("  ├──────────────────────┼──────────────┼────────┼────────┼────────────┤\n");
    printf("  │ Directo  O(n·m)      │ %12lld │ %6d │ %6d │ %10.4f │\n", d.suma, d.ini, d.fin, td);
    printf("  │ Divide y vencerás    │ %12lld │ %6d │ %6d │ %10.4f │\n", r.suma, r.ini, r.fin, tr);
    printf("  └──────────────────────┴──────────────┴────────┴────────┴────────────┘\n");
    if (d.suma == r.suma && d.ini == r.ini && d.fin == r.fin)
        printf("  " VRD "✔ Ambos métodos coinciden." RST "\n");
    else
        printf("  " ROJ "✘ Los métodos NO coinciden." RST "\n");
}

void llenar_aleatorio(int *a, int n) { for (int i = 0; i < n; i++) a[i] = rand() % 201 - 100; }

void tabla_tiempos(void)
{
    int m = leer_entero("\n  Longitud m de la ventana para la tabla (p. ej. 50): ", 1, 100000);
    int tams[] = {1000, 5000, 10000, 50000, 100000, 500000};
    printf("\n  ┌──────────┬──────────────┬──────────────┬──────────┐\n");
    printf("  │ " NEG "n" RST "        │ " NEG "Directo ms" RST "   │ " NEG "DyV ms" RST "       │ " NEG "¿Igual?" RST "  │\n");
    printf("  ├──────────┼──────────────┼──────────────┼──────────┤\n");
    for (int k = 0; k < 6; k++) {
        int n = tams[k];
        if (m > n) continue;
        int *a = malloc(n * sizeof(int));
        llenar_aleatorio(a, n);
        Ventana d, r;
        double td = medir(directo, a, n, m, &d);
        double tr = medir(dyv_envoltura, a, n, m, &r);
        printf("  │ %8d │ %12.4f │ %12.4f │ %s │\n", n, td, tr,
               (d.suma == r.suma && d.ini == r.ini) ? VRD "   sí   " RST : ROJ "   no   " RST);
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
    printf("  ║" RST NEG "   SUB-ARREGLO DE SUMA MÁXIMA (longitud m)           " RST CYN "║\n");
    printf("  ║" RST "   Método directo  vs.  Divide y vencerás           " CYN "║\n");
    printf("  ╚════════════════════════════════════════════════════╝" RST "\n");

    int op;
    do {
        printf("\n  " NEG "Menú" RST "\n");
        printf("   1) Capturar el arreglo a mano\n");
        printf("   2) Arreglo aleatorio (valores de -100 a 100)\n");
        printf("   3) Ejemplo fijo de prueba\n");
        printf("   4) Tabla de tiempos (para el reporte)\n");
        printf("   0) Salir\n");
        op = leer_entero("  Opción: ", 0, 4);
        if (op >= 1 && op <= 3) {
            int n, m, *a;
            if (op == 3) {
                static int ej[] = {2, -8, 3, -2, 4, -10, 5, 1, -1, 6};
                n = 10; m = 3; a = malloc(n * sizeof(int));
                for (int i = 0; i < n; i++) a[i] = ej[i];
                printf(GRS "  A = {2,-8,3,-2,4,-10,5,1,-1,6},  m = 3  (esperado: suma 6, [7..9])\n" RST);
            } else {
                n = leer_entero("\n  n (tamaño del arreglo): ", 1, 1000000);
                m = leer_entero("  m (longitud del sub-arreglo, m <= n): ", 1, n);
                a = malloc(n * sizeof(int));
                if (op == 1) {
                    char msg[40];
                    for (int i = 0; i < n; i++) {
                        sprintf(msg, "  A[%d] = ", i);
                        a[i] = leer_entero(msg, -1000000, 1000000);
                    }
                } else llenar_aleatorio(a, n);
            }
            resolver_y_mostrar(a, n, m);
            free(a);
        } else if (op == 4) tabla_tiempos();
    } while (op != 0);
    printf("\n  Fin del programa.\n\n");
    return 0;
}
