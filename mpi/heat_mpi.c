#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define N 1000
#define MAX_ITER 1000

int main(int argc, char **argv) {
    int rank, size;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int linhas_por_proc = N / size;
    int resto = N % size;
    int minhas_linhas = linhas_por_proc + (rank == size - 1 ? resto : 0);
    
    int total_linhas_alocadas = minhas_linhas + 2; 
    
    double *atual_dados = (double *)calloc(total_linhas_alocadas * N, sizeof(double));
    double *nova_dados = (double *)calloc(total_linhas_alocadas * N, sizeof(double));
    double **matriz_atual = (double **)malloc(total_linhas_alocadas * sizeof(double *));
    double **nova_matriz = (double **)malloc(total_linhas_alocadas * sizeof(double *));

    for (int i = 0; i < total_linhas_alocadas; i++) {
        matriz_atual[i] = &atual_dados[i * N];
        nova_matriz[i] = &nova_dados[i * N];
    }

    for (int i = 0; i < total_linhas_alocadas; i++) {
        int linha_global = (rank * linhas_por_proc) + i - 1;
        for (int j = 0; j < N; j++) {
            if (linha_global == 0 || linha_global == N - 1 || j == 0 || j == N - 1) {
                matriz_atual[i][j] = 100.0;
                nova_matriz[i][j] = 100.0;
            }
        }
    }

    double start_time = MPI_Wtime();

    for (int iter = 0; iter < MAX_ITER; iter++) {
        int rank_up = rank - 1;
        int rank_down = rank + 1;

        if (rank < size - 1) {
            MPI_Send(matriz_atual[minhas_linhas], N, MPI_DOUBLE, rank_down, 0, MPI_COMM_WORLD);
            MPI_Recv(matriz_atual[minhas_linhas + 1], N, MPI_DOUBLE, rank_down, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank > 0) {
            MPI_Recv(matriz_atual[0], N, MPI_DOUBLE, rank_up, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(matriz_atual[1], N, MPI_DOUBLE, rank_up, 1, MPI_COMM_WORLD);
        }

        for (int i = 1; i <= minhas_linhas; i++) {
            int linha_global = (rank * linhas_por_proc) + i - 1;
            if (linha_global == 0 || linha_global == N - 1) continue; 

            for (int j = 1; j < N - 1; j++) {
                nova_matriz[i][j] = (matriz_atual[i-1][j] + 
                                     matriz_atual[i+1][j] + 
                                     matriz_atual[i][j-1] + 
                                     matriz_atual[i][j+1]) / 4.0;
            }
        }

        double **temp = matriz_atual;
        matriz_atual = nova_matriz;
        nova_matriz = temp;
    }

    double meu_tempo = MPI_Wtime() - start_time;
    double tempo_maximo;

    MPI_Reduce(&meu_tempo, &tempo_maximo, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    double temp_centro = 0.0;
    int linha_centro_global = N / 2;
    int root_center = linha_centro_global / linhas_por_proc;
    if (root_center >= size) root_center = size - 1;

    if (rank == root_center) {
        int linha_local = linha_centro_global - (rank * linhas_por_proc) + 1;
        temp_centro = matriz_atual[linha_local][N / 2];
        if (rank != 0) {
            MPI_Send(&temp_centro, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
        }
    }
    
    if (rank == 0) {
        if (root_center != 0) {
            MPI_Recv(&temp_centro, 1, MPI_DOUBLE, root_center, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printf("=== Versão MPI ===\n");
        printf("Processos MPI: %d | Tempo: %.6f s | Centro: %.2f °C\n", size, tempo_maximo, temp_centro);
    }

    free(atual_dados); free(nova_dados);
    free(matriz_atual); free(nova_matriz);

    MPI_Finalize();
    return 0;
}
