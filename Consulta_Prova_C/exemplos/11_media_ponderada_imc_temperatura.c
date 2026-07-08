#include <stdio.h>

int main() {
    float n1, n2, n3;
    float p1, p2, p3;
    float media;
    float peso, altura, imc;
    float celsius, fahrenheit;

    printf("Notas e pesos (n1 p1 n2 p2 n3 p3): ");
    scanf("%f %f %f %f %f %f", &n1, &p1, &n2, &p2, &n3, &p3);
    media = (n1 * p1 + n2 * p2 + n3 * p3) / (p1 + p2 + p3);
    printf("Media ponderada = %.2f\n", media);

    printf("\nPeso e altura: ");
    scanf("%f %f", &peso, &altura);
    imc = peso / (altura * altura);
    printf("IMC = %.2f\n", imc);

    if (imc < 18.5) {
        printf("Abaixo do peso\n");
    } else if (imc < 25.0) {
        printf("Normal\n");
    } else if (imc < 30.0) {
        printf("Sobrepeso\n");
    } else {
        printf("Obesidade\n");
    }

    printf("\nTemperatura em Celsius: ");
    scanf("%f", &celsius);
    fahrenheit = celsius * 9.0 / 5.0 + 32.0;
    printf("Fahrenheit = %.2f\n", fahrenheit);

    return 0;
}

