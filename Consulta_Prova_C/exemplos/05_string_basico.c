#include <stdio.h>
#include <string.h>

int main() {
    char nome[50];
    char senha[20];

    printf("Digite seu nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite a senha: ");
    scanf("%19s", senha);

    printf("Tamanho do nome: %zu\n", strlen(nome));

    if (strcmp(senha, "1234") == 0) {
        printf("Ola, %s. Acesso permitido.\n", nome);
    } else {
        printf("Senha incorreta.\n");
    }

    return 0;
}

