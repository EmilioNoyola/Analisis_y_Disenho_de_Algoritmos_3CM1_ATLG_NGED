/*
 * ============================================================================
 *  Práctica 3 · Problema 2 - Multiplicación de matrices cuadradas
 *  Análisis y Diseño de Algoritmos - ESCOM IPN
 * ----------------------------------------------------------------------------
 *  C = A · B,  con A y B de n × n.
 *
 *  MÉTODO DIRECTO (definición)
 *    C[i][j] = Σ_k A[i][k]·B[k][j]       →  n³ multiplicaciones  →  O(n³)
 *
 *  DIVIDE Y VENCERÁS (dos versiones)
 *    Se parte cada matriz en 4 bloques de n/2 × n/2:
 *        | A11 A12 |   | B11 B12 |   | C11 C12 |
 *        | A21 A22 | · | B21 B22 | = | C21 C22 |
 *
 *    (a) Recursiva clásica: 8 productos de bloques
 *        C11 = A11·B11 + A12·B21    C12 = A11·B12 + A12·B22
 *        C21 = A21·B11 + A22·B21    C22 = A21·B12 + A22·B22
 *        T(n) = 8T(n/2) + O(n²)  →  O(n^log2 8) = O(n³)
 *
 *    (b) Strassen: 7 productos y más sumas
 *        M1 = (A11+A22)(B11+B22)   M5 = (A11+A12)B22
 *        M2 = (A21+A22)B11         M6 = (A21−A11)(B11+B12)
 *        M3 = A11(B12−B22)         M7 = (A12−A22)(B21+B22)
 *        M4 = A22(B21−B11)
 *        C11 = M1+M4−M5+M7   C12 = M3+M5   C21 = M2+M4   C22 = M1−M2+M3+M6
 *        T(n) = 7T(n/2) + O(n²)  →  O(n^log2 7) ≈ O(n^2.807)
 *
 *  Si n no es potencia de 2, se rellena con ceros hasta la siguiente
 *  potencia de 2 (no altera el resultado).
 *
 *  UMBRAL: cuando el bloque es de tamaño <= UMBRAL se multiplica directo.
 *  Con UMBRAL = 1 se obtiene la versión "pura" del libro (mismo resultado,
 *  pero mucho más lenta por el costo de las llamadas y de pedir memoria).
 *
 *  Compilar:  gcc -O2 p3_2_matrices.c -o matrices
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define UMBRAL 32

#define RST "\033[0m"
#define NEG "\033[1m"
#define CYN "\033[36m"
#define VRD "\033[32m"
#define ROJ "\033[31m"
#define GRS "\033[90m"

typedef long long ll;

/* Acceso a una matriz guardada por renglones con "zancada" (stride) s:
   el elemento (i,j) está en M[i*s + j]. Así un bloque es solo un puntero. */
#define E(M, s, i, j) ((M)[(size_t)(i) * (s) + (j)])

/* ------------------------------ MÉTODO DIRECTO ---------------------------- */
void directo(const ll *A, const ll *B, ll *C, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            ll s = 0;
            for (int k = 0; k < n; k++) s += A[i * n + k] * B[k * n + j];
            C[i * n + j] = s;
        }
}

/* --------------------- DyV (a): recursiva con 8 productos ----------------- */
/* C += A·B  sobre bloques de n×n que viven dentro de matrices con zancada. */
static void rec8(const ll *A, int sa, const ll *B, int sb, ll *C, int sc, int n)
{
    if (n <= UMBRAL) {                                   /* caso base */
        for (int i = 0; i < n; i++)
            for (int k = 0; k < n; k++) {
                ll a = E(A, sa, i, k);
                for (int j = 0; j < n; j++) E(C, sc, i, j) += a * E(B, sb, k, j);
            }
        return;
    }
    int h = n / 2;
    const ll *A11 = A, *A12 = A + h, *A21 = A + (size_t)h * sa, *A22 = A21 + h;
    const ll *B11 = B, *B12 = B + h, *B21 = B + (size_t)h * sb, *B22 = B21 + h;
    ll *C11 = C, *C12 = C + h, *C21 = C + (size_t)h * sc, *C22 = C21 + h;

    rec8(A11, sa, B11, sb, C11, sc, h);  rec8(A12, sa, B21, sb, C11, sc, h);
    rec8(A11, sa, B12, sb, C12, sc, h);  rec8(A12, sa, B22, sb, C12, sc, h);
    rec8(A21, sa, B11, sb, C21, sc, h);  rec8(A22, sa, B21, sb, C21, sc, h);
    rec8(A21, sa, B12, sb, C22, sc, h);  rec8(A22, sa, B22, sb, C22, sc, h);
}

/* --------------------------- DyV (b): Strassen --------------------------- */
/* R = X ± Y  (bloques h×h) */
static void sumar(const ll *X, int sx, const ll *Y, int sy, ll *R, int h, int signo)
{
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            R[i * h + j] = E(X, sx, i, j) + signo * E(Y, sy, i, j);
}
static void copiar(const ll *X, int sx, ll *R, int h)
{
    for (int i = 0; i < h; i++) memcpy(R + (size_t)i * h, X + (size_t)i * sx, h * sizeof(ll));
}

/* C = A·B, todas contiguas de n×n */
static void strassen(const ll *A, const ll *B, ll *C, int n)
{
    if (n <= UMBRAL) {
        memset(C, 0, (size_t)n * n * sizeof(ll));
        rec8(A, n, B, n, C, n, n);   /* con n <= UMBRAL esto es el triple ciclo */
        return;
    }
    int h = n / 2;
    size_t hh = (size_t)h * h;
    const ll *A11 = A, *A12 = A + h, *A21 = A + (size_t)h * n, *A22 = A21 + h;
    const ll *B11 = B, *B12 = B + h, *B21 = B + (size_t)h * n, *B22 = B21 + h;

    ll *buf = malloc(9 * hh * sizeof(ll));          /* M1..M7, T1, T2 */
    ll *M[7], *T1 = buf + 7 * hh, *T2 = buf + 8 * hh;
    for (int i = 0; i < 7; i++) M[i] = buf + i * hh;

    sumar(A11, n, A22, n, T1, h, +1); sumar(B11, n, B22, n, T2, h, +1); strassen(T1, T2, M[0], h);
    sumar(A21, n, A22, n, T1, h, +1); copiar(B11, n, T2, h);            strassen(T1, T2, M[1], h);
    copiar(A11, n, T1, h);            sumar(B12, n, B22, n, T2, h, -1); strassen(T1, T2, M[2], h);
    copiar(A22, n, T1, h);            sumar(B21, n, B11, n, T2, h, -1); strassen(T1, T2, M[3], h);
    sumar(A11, n, A12, n, T1, h, +1); copiar(B22, n, T2, h);            strassen(T1, T2, M[4], h);
    sumar(A21, n, A11, n, T1, h, -1); sumar(B11, n, B12, n, T2, h, +1); strassen(T1, T2, M[5], h);
    sumar(A12, n, A22, n, T1, h, -1); sumar(B21, n, B22, n, T2, h, +1); strassen(T1, T2, M[6], h);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            size_t k = (size_t)i * h + j;
            E(C, n, i, j)         = M[0][k] + M[3][k] - M[4][k] + M[6][k];   /* C11 */
            E(C, n, i, j + h)     = M[2][k] + M[4][k];                       /* C12 */
            E(C, n, i + h, j)     = M[1][k] + M[3][k];                       /* C21 */
            E(C, n, i + h, j + h) = M[0][k] - M[1][k] + M[2][k] + M[5][k];   /* C22 */
        }
    free(buf);
}

/* ------------- Envolturas: rellenar a potencia de 2 y recortar ------------ */
static int pot2(int n) { int p = 1; while (p < n) p <<= 1; return p; }

static ll *rellenar(const ll *X, int n, int p)
{
    ll *R = calloc((size_t)p * p, sizeof(ll));
    for (int i = 0; i < n; i++) memcpy(R + (size_t)i * p, X + (size_t)i * n, n * sizeof(ll));
    return R;
}
static void recortar(const ll *R, int p, ll *C, int n)
{
    for (int i = 0; i < n; i++) memcpy(C + (size_t)i * n, R + (size_t)i * p, n * sizeof(ll));
}

void dyv_8(const ll *A, const ll *B, ll *C, int n)
{
    int p = pot2(n);
    ll *Ap = rellenar(A, n, p), *Bp = rellenar(B, n, p), *Cp = calloc((size_t)p * p, sizeof(ll));
    rec8(Ap, p, Bp, p, Cp, p, p);
    recortar(Cp, p, C, n);
    free(Ap); free(Bp); free(Cp);
}

void dyv_strassen(const ll *A, const ll *B, ll *C, int n)
{
    int p = pot2(n);
    ll *Ap = rellenar(A, n, p), *Bp = rellenar(B, n, p), *Cp = malloc((size_t)p * p * sizeof(ll));
    strassen(Ap, Bp, Cp, p);
    recortar(Cp, p, C, n);
    free(Ap); free(Bp); free(Cp);
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

void imprimir(const char *nombre, const ll *M, int n)
{
    if (n > 8) { printf(GRS "  %s: %d×%d (no se imprime, n > 8)\n" RST, nombre, n, n); return; }
    printf("  " NEG "%s" RST " =\n", nombre);
    for (int i = 0; i < n; i++) {
        printf("    │");
        for (int j = 0; j < n; j++) printf("%7lld", M[i * n + j]);
        printf("  │\n");
    }
}

typedef void (*Mult)(const ll *, const ll *, ll *, int);

double medir(Mult f, const ll *A, const ll *B, ll *C, int n)
{
    int rep = 0; clock_t t0 = clock(), t;
    do { f(A, B, C, n); rep++; t = clock(); } while (t - t0 < CLOCKS_PER_SEC / 20);
    return 1000.0 * (double)(t - t0) / CLOCKS_PER_SEC / rep;
}

int iguales(const ll *X, const ll *Y, int n)
{
    for (size_t i = 0; i < (size_t)n * n; i++) if (X[i] != Y[i]) return 0;
    return 1;
}

void llenar_aleatorio(ll *M, int n) { for (int i = 0; i < n * n; i++) M[i] = rand() % 19 - 9; }

void comparar(const ll *A, const ll *B, int n, int mostrar)
{
    size_t bytes = (size_t)n * n * sizeof(ll);
    ll *C1 = malloc(bytes), *C2 = malloc(bytes), *C3 = malloc(bytes);
    double t1 = medir(directo, A, B, C1, n);
    double t2 = medir(dyv_8, A, B, C2, n);
    double t3 = medir(dyv_strassen, A, B, C3, n);
    if (mostrar) { printf("\n"); imprimir("A", A, n); imprimir("B", B, n); imprimir("C = A·B", C1, n); }
    printf("\n  ┌────────────────────────────────┬──────────────┬──────────┐\n");
    printf("  │ " NEG "Método" RST "                         │ " NEG "Tiempo ms" RST "    │ " NEG "¿Igual?" RST "  │\n");
    printf("  ├────────────────────────────────┼──────────────┼──────────┤\n");
    printf("  │ Directo            O(n³)       │ %12.3f │    —     │\n", t1);
    printf("  │ DyV 8 productos    O(n³)       │ %12.3f │ %s │\n", t2, iguales(C1, C2, n) ? VRD "   sí   " RST : ROJ "   no   " RST);
    printf("  │ Strassen           O(n^2.807)  │ %12.3f │ %s │\n", t3, iguales(C1, C3, n) ? VRD "   sí   " RST : ROJ "   no   " RST);
    printf("  └────────────────────────────────┴──────────────┴──────────┘\n");
    free(C1); free(C2); free(C3);
}

void tabla_tiempos(void)
{
    int tams[] = {64, 128, 256, 512, 1024};
    printf("\n  " GRS "UMBRAL = %d. Esto puede tardar ~1 minuto con n = 1024." RST "\n", UMBRAL);
    printf("  ┌────────┬──────────────┬──────────────┬──────────────┐\n");
    printf("  │ " NEG "n" RST "      │ " NEG "Directo ms" RST "   │ " NEG "DyV-8 ms" RST "     │ " NEG "Strassen ms" RST "  │\n");
    printf("  ├────────┼──────────────┼──────────────┼──────────────┤\n");
    for (int k = 0; k < 5; k++) {
        int n = tams[k];
        size_t bytes = (size_t)n * n * sizeof(ll);
        ll *A = malloc(bytes), *B = malloc(bytes), *C = malloc(bytes);
        llenar_aleatorio(A, n); llenar_aleatorio(B, n);
        double t1 = medir(directo, A, B, C, n);
        double t2 = medir(dyv_8, A, B, C, n);
        double t3 = medir(dyv_strassen, A, B, C, n);
        printf("  │ %6d │ %12.2f │ %12.2f │ %12.2f │\n", n, t1, t2, t3);
        fflush(stdout);
        free(A); free(B); free(C);
    }
    printf("  └────────┴──────────────┴──────────────┴──────────────┘\n");
}

int main(void)
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    srand((unsigned)time(NULL));
    printf(CYN "\n  ╔════════════════════════════════════════════════════╗\n");
    printf("  ║" RST NEG "   MULTIPLICACIÓN DE MATRICES CUADRADAS              " RST CYN "║\n");
    printf("  ║" RST "   Directo · DyV (8 productos) · Strassen (7)       " CYN "║\n");
    printf("  ╚════════════════════════════════════════════════════╝" RST "\n");

    int op;
    do {
        printf("\n  " NEG "Menú" RST "\n");
        printf("   1) Capturar A y B a mano\n");
        printf("   2) Matrices aleatorias (valores de -9 a 9)\n");
        printf("   3) Tabla de tiempos (para el reporte)\n");
        printf("   0) Salir\n");
        op = leer_entero("  Opción: ", 0, 3);
        if (op == 1 || op == 2) {
            int n = leer_entero("\n  n (orden de las matrices): ", 1, 2048);
            size_t bytes = (size_t)n * n * sizeof(ll);
            ll *A = malloc(bytes), *B = malloc(bytes);
            if (op == 1) {
                char msg[48];
                for (int i = 0; i < n * n; i++) { sprintf(msg, "  A[%d][%d] = ", i / n, i % n); A[i] = leer_entero(msg, -100000, 100000); }
                for (int i = 0; i < n * n; i++) { sprintf(msg, "  B[%d][%d] = ", i / n, i % n); B[i] = leer_entero(msg, -100000, 100000); }
            } else { llenar_aleatorio(A, n); llenar_aleatorio(B, n); }
            comparar(A, B, n, 1);
            free(A); free(B);
        } else if (op == 3) tabla_tiempos();
    } while (op != 0);
    printf("\n  Fin del programa.\n\n");
    return 0;
}
