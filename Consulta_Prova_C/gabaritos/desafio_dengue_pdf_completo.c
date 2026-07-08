#include <stdio.h>

#define MESES 6
#define HOSPITAIS 5

void lerMatriz(int matriz[MESES][HOSPITAIS]) {
    for (int mes = 0; mes < MESES; mes++) {
        printf("\nMes %d\n", mes + 1);
        for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
            printf("Internados no Hospital %d: ", hospital);
            scanf("%d", &matriz[mes][hospital]);
        }
    }
}

void imprimirMatriz(int matriz[MESES][HOSPITAIS]) {
    printf("\nMatriz de internados:\n");
    for (int mes = 0; mes < MESES; mes++) {
        printf("Mes %d: ", mes + 1);
        for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
            printf("%5d", matriz[mes][hospital]);
        }
        printf("\n");
    }
}

void calcularInternadosPorMes(int internados[MESES],
                              int matriz[MESES][HOSPITAIS]) {
    for (int mes = 0; mes < MESES; mes++) {
        internados[mes] = 0;
        for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
            internados[mes] += matriz[mes][hospital];
        }
    }
}

void calcularPacientesPorHospital(int pacientes[HOSPITAIS],
                                  int matriz[MESES][HOSPITAIS]) {
    for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
        pacientes[hospital] = 0;
        for (int mes = 0; mes < MESES; mes++) {
            pacientes[hospital] += matriz[mes][hospital];
        }
    }
}

double calcularRepasse(int totalPacientes) {
    if (totalPacientes <= 50) {
        return totalPacientes * 1000.0;
    } else if (totalPacientes <= 100) {
        return totalPacientes * 1200.0;
    } else {
        return totalPacientes * 1500.0;
    }
}

void imprimirTotaisPorMes(int internados[MESES]) {
    printf("\nTotal de internados por mes:\n");
    for (int mes = 0; mes < MESES; mes++) {
        printf("Mes %d: %d\n", mes + 1, internados[mes]);
    }
}

void imprimirRelatorioHospitais(int pacientes[HOSPITAIS]) {
    printf("\nRelatorio por hospital:\n");
    for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
        double repasse = calcularRepasse(pacientes[hospital]);
        printf("Hospital %d - pacientes: %d - R$ %.2f\n",
               hospital,
               pacientes[hospital],
               repasse);
    }
}

float calcularMediaMatriz(int matriz[MESES][HOSPITAIS]) {
    int soma = 0;

    for (int mes = 0; mes < MESES; mes++) {
        for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
            soma += matriz[mes][hospital];
        }
    }

    return soma / (float)(MESES * HOSPITAIS);
}

int main() {
    int matriz[MESES][HOSPITAIS];
    int internados[MESES] = {0};
    int pacientes[HOSPITAIS] = {0};
    float media;

    lerMatriz(matriz);
    imprimirMatriz(matriz);

    calcularInternadosPorMes(internados, matriz);
    calcularPacientesPorHospital(pacientes, matriz);

    imprimirTotaisPorMes(internados);
    imprimirRelatorioHospitais(pacientes);

    media = calcularMediaMatriz(matriz);
    printf("\nMedia geral de pacientes na matriz: %.2f\n", media);

    return 0;
}

