#include <stdio.h>

int main() {
    int idade;
    float nota;

    printf("Digite sua idade: ");
    scanf("%d", &idade);

    printf("Digite sua nota: ");
    scanf("%f", &nota);

    if (idade >= 18 && nota >= 7.0) {
        printf("Maior de idade e aprovado.\n");
    } else if (nota >= 7.0) {
        printf("Aprovado, mas menor de idade.\n");
    } else {
        printf("Nao aprovado.\n");
    }

    return 0;
}

