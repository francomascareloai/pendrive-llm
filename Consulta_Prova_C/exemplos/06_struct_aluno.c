#include <stdio.h>

typedef struct {
    char nome[50];
    float nota1;
    float nota2;
    float media;
} Aluno;

int main() {
    Aluno aluno;

    printf("Nome: ");
    scanf("%49s", aluno.nome);

    printf("Nota 1: ");
    scanf("%f", &aluno.nota1);

    printf("Nota 2: ");
    scanf("%f", &aluno.nota2);

    aluno.media = (aluno.nota1 + aluno.nota2) / 2.0;

    printf("\nAluno: %s\n", aluno.nome);
    printf("Media: %.2f\n", aluno.media);

    if (aluno.media >= 7.0) {
        printf("Aprovado\n");
    } else {
        printf("Reprovado\n");
    }

    return 0;
}

