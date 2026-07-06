#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000             
#define MAX_ITER 1000     

void inicializar(double **M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || i == N - 1 || j == 0 || j == N - 1) M[i][j] = 100.0; 
            else M[i][j] = 0.0;   
        }
    }
}

double** alocar_matriz() {
    double **M = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) M[i] = (double *)malloc(N * sizeof(double));
    return M;
}

void liberar_matriz(double **M) {
    for (int i = 0; i < N; i++) free(M[i]);
    free(M);
}

int main() {
    double **matriz_atual = alocar_matriz();
    double **nova_matriz = alocar_matriz();
    inicializar(matriz_atual); inicializar(nova_matriz);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int iter = 0; iter < MAX_ITER; iter++) {
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                nova_matriz[i][j] = (matriz_atual[i-1][j] + matriz_atual[i+1][j] + matriz_atual[i][j-1] + matriz_atual[i][j+1]) / 4.0;
            }
        }
        double **temp = matriz_atual; matriz_atual = nova_matriz; nova_matriz = temp;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double tempo = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("1000x1000_1000iter: %.6f s | Centro: %.2f °C\n", tempo, matriz_atual[N/2][N/2]);

    liberar_matriz(matriz_atual); liberar_matriz(nova_matriz);
    return 0;
}
