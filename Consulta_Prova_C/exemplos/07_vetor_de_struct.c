#include <stdio.h>

#define TAM 3

typedef struct {
    char nome[50];
    int idade;
    float salario;
} Pessoa;

void lerPessoas(Pessoa pessoas[], int n) {
    for (int i = 0; i < n; i++) {
        printf("\nPessoa %d\n", i + 1);
        printf("Nome: ");
        scanf("%49s", pessoas[i].nome);
        printf("Idade: ");
        scanf("%d", &pessoas[i].idade);
        printf("Salario: ");
        scanf("%f", &pessoas[i].salario);
    }
}

void listarPessoas(Pessoa pessoas[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%s - %d anos - R$ %.2f\n",
               pessoas[i].nome,
               pessoas[i].idade,
               pessoas[i].salario);
    }
}

float mediaSalarios(Pessoa pessoas[], int n) {
    float soma = 0;
    for (int i = 0; i < n; i++) {
        soma += pessoas[i].salario;
    }
    return soma / n;
}

int main() {
    Pessoa pessoas[TAM];

    lerPessoas(pessoas, TAM);
    printf("\nLista:\n");
    listarPessoas(pessoas, TAM);
    printf("Media salarial = %.2f\n", mediaSalarios(pessoas, TAM));

    return 0;
}

