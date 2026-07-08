#include <stdio.h>

int main() {
    int opcao;
    int a, b;

    do {
        printf("\n1 - Somar\n");
        printf("2 - Subtrair\n");
        printf("3 - Multiplicar\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite dois numeros: ");
                scanf("%d %d", &a, &b);
                printf("Soma = %d\n", a + b);
                break;
            case 2:
                printf("Digite dois numeros: ");
                scanf("%d %d", &a, &b);
                printf("Subtracao = %d\n", a - b);
                break;
            case 3:
                printf("Digite dois numeros: ");
                scanf("%d %d", &a, &b);
                printf("Multiplicacao = %d\n", a * b);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}

