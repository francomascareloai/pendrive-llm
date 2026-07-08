#include <stdio.h>

#define TAM 6

void ordenar(int v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[i]) {
                int aux = v[i];
                v[i] = v[j];
                v[j] = aux;
            }
        }
    }
}

int buscar(int v[], int n, int valor) {
    for (int i = 0; i < n; i++) {
        if (v[i] == valor) {
            return i;
        }
    }
    return -1;
}

int main() {
    int v[TAM] = {9, 2, 5, 1, 8, 3};
    int valor = 5;
    int pos;

    ordenar(v, TAM);

    printf("Ordenado: ");
    for (int i = 0; i < TAM; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");

    pos = buscar(v, TAM, valor);
    if (pos != -1) {
        printf("Valor %d encontrado na posicao %d.\n", valor, pos);
    } else {
        printf("Valor nao encontrado.\n");
    }

    return 0;
}

