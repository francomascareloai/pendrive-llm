#include <stdio.h>

#define TAM 5

void lerVetor(int v[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Valor %d: ", i);
        scanf("%d", &v[i]);
    }
}

void imprimirVetor(int v[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int somarVetor(int v[], int n) {
    int soma = 0;
    for (int i = 0; i < n; i++) {
        soma += v[i];
    }
    return soma;
}

int maiorValor(int v[], int n) {
    int maior = v[0];
    for (int i = 1; i < n; i++) {
        if (v[i] > maior) {
            maior = v[i];
        }
    }
    return maior;
}

int main() {
    int numeros[TAM];
    int soma;
    float media;

    lerVetor(numeros, TAM);
    imprimirVetor(numeros, TAM);

    soma = somarVetor(numeros, TAM);
    media = soma / (float)TAM;

    printf("Soma = %d\n", soma);
    printf("Media = %.2f\n", media);
    printf("Maior = %d\n", maiorValor(numeros, TAM));

    return 0;
}

