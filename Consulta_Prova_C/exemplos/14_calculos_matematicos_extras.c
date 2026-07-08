#include <math.h>
#include <stdio.h>

int calcularMdc(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    while (b != 0) {
        int resto = a % b;
        a = b;
        b = resto;
    }

    return a;
}

int calcularMmc(int a, int b) {
    int mdc = calcularMdc(a, b);

    if (mdc == 0) {
        return 0;
    }

    return (a / mdc) * b;
}

int ehPrimo(int n) {
    if (n < 2) {
        return 0;
    }

    for (int divisor = 2; divisor * divisor <= n; divisor++) {
        if (n % divisor == 0) {
            return 0;
        }
    }

    return 1;
}

void imprimirPaPg(int primeiro, int razao, int termos) {
    int pa = primeiro;
    int pg = primeiro;

    printf("PA: ");
    for (int i = 0; i < termos; i++) {
        printf("%d ", pa);
        pa += razao;
    }

    printf("\nPG: ");
    for (int i = 0; i < termos; i++) {
        printf("%d ", pg);
        pg *= razao;
    }

    printf("\n");
}

int main(void) {
    double capital = 1000.0;
    double taxa = 2.0;
    int meses = 6;
    double montante = capital * pow(1.0 + taxa / 100.0, meses);

    double velocidade = 80.0;
    double tempo = 2.5;
    double distancia = velocidade * tempo;

    double litros = 16.0;
    double consumo = distancia / litros;

    double ladoA = 3.0;
    double ladoB = 4.0;
    double ladoC = 5.0;
    double semiPerimetro = (ladoA + ladoB + ladoC) / 2.0;
    double areaTriangulo = sqrt(
        semiPerimetro *
        (semiPerimetro - ladoA) *
        (semiPerimetro - ladoB) *
        (semiPerimetro - ladoC)
    );

    int inicio = 1;
    int fim = 10;
    int somaIntervalo = 0;

    for (int numero = inicio; numero <= fim; numero++) {
        somaIntervalo += numero;
    }

    printf("Juros composto: %.2f\n", montante);
    printf("Distancia: %.2f km\n", distancia);
    printf("Consumo medio: %.2f km/l\n", consumo);
    printf("Area do triangulo: %.2f\n", areaTriangulo);
    printf("Pitagoras 3-4-5: hipotenusa = %.2f\n", sqrt(3 * 3 + 4 * 4));
    printf("MDC(24, 18) = %d\n", calcularMdc(24, 18));
    printf("MMC(24, 18) = %d\n", calcularMmc(24, 18));
    printf("17 e primo? %s\n", ehPrimo(17) ? "sim" : "nao");
    printf("Soma de %d ate %d = %d\n", inicio, fim, somaIntervalo);
    imprimirPaPg(2, 3, 5);

    return 0;
}
