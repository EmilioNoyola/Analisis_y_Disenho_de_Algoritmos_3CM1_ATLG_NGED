/*
 * ============================================================================
 *  Tarea 12 - Número mínimo de monedas (monedas ilimitadas)
 *  Análisis y Diseño de Algoritmos - ESCOM IPN
 * ----------------------------------------------------------------------------
 *  Problema: una máquina expendedora debe devolver el cambio con el MENOR
 *  número de monedas posible. Hay monedas ilimitadas de 50, 25, 5 y 1.
 *
 *  Estrategia: algoritmo VORAZ (greedy).
 *    En cada paso se toma la moneda de MAYOR denominación que todavía cabe
 *    en lo que falta por devolver; se toman tantas como quepan (división
 *    entera) y se continúa con el residuo (módulo).
 *
 *  Complejidad: O(k), k = número de denominaciones (aquí k = 4).
 *               No depende del monto: usa / y %, no resta moneda por moneda.
 *
 *  ¿Por qué el voraz da el óptimo aquí? El sistema {50, 25, 5, 1} es
 *  "canónico". Además del argumento, el programa incluye la opción 2, que
 *  compara el voraz contra programación dinámica (que SIEMPRE da el
 *  óptimo) para todos los montos de 1 a N.
 *
 *  Compilar:  gcc tarea12_monedas.c -o monedas
 *  Ejecutar:  ./monedas        (en Windows: monedas.exe)
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#define K 4                       /* número de denominaciones */
#define MAX_VERIF 100000          /* tope para la verificación */

/* Colores ANSI */
#define RST "\033[0m"
#define NEG "\033[1m"
#define CYN "\033[36m"
#define VRD "\033[32m"
#define AMA "\033[33m"
#define ROJ "\033[31m"
#define GRS "\033[90m"

static const int DENOM[K] = {50, 25, 5, 1};   /* ordenadas de mayor a menor */

/* ---------------------------------------------------------------------------
 * cambio_voraz: llena cant[i] = cuántas monedas de DENOM[i] se usan.
 * Devuelve el total de monedas. Si 'mostrar' != 0 imprime cada paso.
 * ------------------------------------------------------------------------- */
int cambio_voraz(int monto, int cant[K], int mostrar)
{
    int resto = monto, total = 0;
    for (int i = 0; i < K; i++) {
        cant[i] = resto / DENOM[i];          /* cuántas caben */
        int antes = resto;
        resto   = resto % DENOM[i];          /* lo que falta */
        total  += cant[i];
        if (mostrar) {
            printf("  " GRS "Paso %d" RST "  falta %s%6d%s  →  moneda de %s%2d%s : "
                   "%s%4d%s  (%d / %d)   resto = %d\n",
                   i + 1, AMA, antes, RST, CYN, DENOM[i], RST,
                   VRD, cant[i], RST, antes, DENOM[i], resto);
        }
    }
    return total;
}

/* ---------------------------------------------------------------------------
 * cambio_dp: mínimo de monedas por programación dinámica (solo para verificar).
 * dp[v] = 1 + min(dp[v - d]) para cada denominación d <= v.
 * ------------------------------------------------------------------------- */
int *tabla_dp(int n)
{
    int *dp = malloc((n + 1) * sizeof(int));
    if (!dp) return NULL;
    dp[0] = 0;
    for (int v = 1; v <= n; v++) {
        dp[v] = v;                           /* peor caso: puras monedas de 1 */
        for (int i = 0; i < K; i++)
            if (DENOM[i] <= v && dp[v - DENOM[i]] + 1 < dp[v])
                dp[v] = dp[v - DENOM[i]] + 1;
    }
    return dp;
}

void imprimir_tabla(int monto, int cant[K], int total)
{
    printf("\n  ┌────────────┬──────────┬────────────┐\n");
    printf("  │ " NEG "Moneda" RST "     │ " NEG "Cantidad" RST " │ " NEG "Subtotal" RST "   │\n");
    printf("  ├────────────┼──────────┼────────────┤\n");
    for (int i = 0; i < K; i++) {
        const char *c = cant[i] ? VRD : GRS;
        printf("  │ %s$%-9d%s │ %s%8d%s │ %s%10d%s │\n",
               CYN, DENOM[i], RST, c, cant[i], RST, c, cant[i] * DENOM[i], RST);
    }
    printf("  ├────────────┼──────────┼────────────┤\n");
    printf("  │ " NEG "TOTAL" RST "      │ " VRD NEG "%8d" RST " │ %10d │\n", total, monto);
    printf("  └────────────┴──────────┴────────────┘\n");
}

int leer_entero(const char *msg, int min, int max)
{
    int x; char c;
    for (;;) {
        printf("%s", msg);
        if (scanf("%d", &x) == 1 && x >= min && x <= max) {
            while ((c = getchar()) != '\n' && c != EOF) ;
            return x;
        }
        while ((c = getchar()) != '\n' && c != EOF) ;
        printf(ROJ "  Entrada inválida: escribe un entero entre %d y %d.\n" RST, min, max);
    }
}

void opcion_cambio(void)
{
    int monto = leer_entero("\n  Monto a devolver (entero > 0): ", 1, 100000000);
    int cant[K];
    printf("\n  " NEG "Algoritmo voraz paso a paso" RST "\n");
    int total = cambio_voraz(monto, cant, 1);
    imprimir_tabla(monto, cant, total);
    printf("\n  " VRD "✔ Se devuelve%s %d moneda%s para $%d." RST "\n",
           total == 1 ? "" : "n", total, total == 1 ? "" : "s", monto);
}

void opcion_verificar(void)
{
    int n = leer_entero("\n  Verificar montos de 1 hasta N (N ≤ 100000): ", 1, MAX_VERIF);
    int *dp = tabla_dp(n);
    if (!dp) { printf(ROJ "  Sin memoria.\n" RST); return; }
    int cant[K], fallas = 0;
    for (int v = 1; v <= n; v++) {
        int g = cambio_voraz(v, cant, 0);
        if (g != dp[v]) {
            if (fallas < 10)
                printf(ROJ "  Monto %d: voraz = %d, óptimo = %d\n" RST, v, g, dp[v]);
            fallas++;
        }
    }
    if (fallas == 0)
        printf("\n  " VRD "✔ Para los %d montos, el voraz usó exactamente el mínimo "
               "que calcula programación dinámica." RST "\n", n);
    else
        printf("\n  " ROJ "✘ %d montos donde el voraz NO es óptimo." RST "\n", fallas);
    free(dp);
}

int main(void)
{
#ifdef _WIN32
    system("chcp 65001 > nul");   /* UTF-8 para los caracteres de caja */
#endif
    printf(CYN "\n  ╔══════════════════════════════════════════════╗\n");
    printf("  ║" RST NEG "    NÚMERO MÍNIMO DE MONEDAS  ·  Voraz        " RST CYN "║\n");
    printf("  ║" RST "    Denominaciones: $50  $25  $5  $1          " CYN "║\n");
    printf("  ╚══════════════════════════════════════════════╝" RST "\n");

    int op;
    do {
        printf("\n  " NEG "Menú" RST "\n");
        printf("   1) Calcular cambio de un monto\n");
        printf("   2) Verificar voraz vs. óptimo (programación dinámica)\n");
        printf("   0) Salir\n");
        op = leer_entero("  Opción: ", 0, 2);
        if (op == 1) opcion_cambio();
        else if (op == 2) opcion_verificar();
    } while (op != 0);
    printf("\n  Fin del programa.\n\n");
    return 0;
}
