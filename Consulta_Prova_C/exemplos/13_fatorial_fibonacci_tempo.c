#include <stdio.h>

int main() {
    int n;
    int fatorial = 1;
    int a = 0, b = 1, proximo;
    int totalSegundos, horas, minutos, segundos, resto;

    printf("Numero para fatorial: ");
    scanf("%d", &n);

    for (int i = 1; i <= n; i++) {
        fatorial *= i;
    }
    printf("%d! = %d\n", n, fatorial);

    printf("\nQuantidade de termos Fibonacci: ");
    scanf("%d", &n);

    a = 0;
    b = 1;
    for (int i = 0; i < n; i++) {
        printf("%d ", a);
        proximo = a + b;
        a = b;
        b = proximo;
    }
    printf("\n");

    printf("\nTotal de segundos: ");
    scanf("%d", &totalSegundos);
    horas = totalSegundos / 3600;
    resto = totalSegundos % 3600;
    minutos = resto / 60;
    segundos = resto % 60;
    printf("%02d:%02d:%02d\n", horas, minutos, segundos);

    return 0;
}

