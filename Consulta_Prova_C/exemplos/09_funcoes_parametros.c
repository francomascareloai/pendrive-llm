#include <stdio.h>

void tentarAlterar(int x) {
    x = 99;
    printf("Dentro de tentarAlterar, x = %d\n", x);
}

void alterarComPonteiro(int *x) {
    *x = 99;
}

int main() {
    int numero = 10;

    printf("Antes: %d\n", numero);

    tentarAlterar(numero);
    printf("Depois de tentarAlterar: %d\n", numero);

    alterarComPonteiro(&numero);
    printf("Depois de alterarComPonteiro: %d\n", numero);

    return 0;
}
