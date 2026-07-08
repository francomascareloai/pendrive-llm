#include <stdio.h>

int main() {
    float preco, percentual;
    float desconto, precoFinal;
    float salario, aumento, novoSalario;
    float n1, n2, n3, media;

    printf("Preco e percentual de desconto: ");
    scanf("%f %f", &preco, &percentual);
    desconto = preco * percentual / 100.0;
    precoFinal = preco - desconto;
    printf("Desconto = %.2f, preco final = %.2f\n", desconto, precoFinal);

    printf("\nSalario e percentual de aumento: ");
    scanf("%f %f", &salario, &percentual);
    aumento = salario * percentual / 100.0;
    novoSalario = salario + aumento;
    printf("Aumento = %.2f, novo salario = %.2f\n", aumento, novoSalario);

    printf("\nTres notas: ");
    scanf("%f %f %f", &n1, &n2, &n3);
    media = (n1 + n2 + n3) / 3.0;
    printf("Media simples = %.2f\n", media);

    return 0;
}

