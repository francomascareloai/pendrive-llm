#include <math.h>
#include <stdio.h>

#define PI 3.14159

int main() {
    double a, b, c;
    double delta, x1, x2;
    double raio, areaCirculo;
    double base, altura, areaTriangulo;

    printf("Coeficientes a b c da equacao: ");
    scanf("%lf %lf %lf", &a, &b, &c);

    delta = b * b - 4 * a * c;

    if (a == 0) {
        printf("Nao e equacao do segundo grau.\n");
    } else if (delta < 0) {
        printf("Nao existem raizes reais.\n");
    } else {
        x1 = (-b + sqrt(delta)) / (2 * a);
        x2 = (-b - sqrt(delta)) / (2 * a);
        printf("x1 = %.2f, x2 = %.2f\n", x1, x2);
    }

    printf("\nRaio do circulo: ");
    scanf("%lf", &raio);
    areaCirculo = PI * pow(raio, 2);
    printf("Area do circulo = %.2f\n", areaCirculo);

    printf("\nBase e altura do triangulo: ");
    scanf("%lf %lf", &base, &altura);
    areaTriangulo = base * altura / 2.0;
    printf("Area do triangulo = %.2f\n", areaTriangulo);

    return 0;
}

