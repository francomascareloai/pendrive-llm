#include <stdio.h>

#define LINHAS 3
#define COLUNAS 4

void lerMatriz(int m[LINHAS][COLUNAS]) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            printf("m[%d][%d]: ", i, j);
            scanf("%d", &m[i][j]);
        }
    }
}

void imprimirMatriz(int m[LINHAS][COLUNAS]) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            printf("%4d", m[i][j]);
        }
        printf("\n");
    }
}

void totalPorLinha(int m[LINHAS][COLUNAS], int totais[LINHAS]) {
    for (int i = 0; i < LINHAS; i++) {
        totais[i] = 0;
        for (int j = 0; j < COLUNAS; j++) {
            totais[i] += m[i][j];
        }
    }
}

void totalPorColuna(int m[LINHAS][COLUNAS], int totais[COLUNAS]) {
    for (int j = 0; j < COLUNAS; j++) {
        totais[j] = 0;
        for (int i = 0; i < LINHAS; i++) {
            totais[j] += m[i][j];
        }
    }
}

int main() {
    int matriz[LINHAS][COLUNAS];
    int linhas[LINHAS];
    int colunas[COLUNAS];

    lerMatriz(matriz);
    imprimirMatriz(matriz);

    totalPorLinha(matriz, linhas);
    totalPorColuna(matriz, colunas);

    printf("\nTotais por linha:\n");
    for (int i = 0; i < LINHAS; i++) {
        printf("Linha %d = %d\n", i, linhas[i]);
    }

    printf("\nTotais por coluna:\n");
    for (int j = 0; j < COLUNAS; j++) {
        printf("Coluna %d = %d\n", j, colunas[j]);
    }

    return 0;
}

