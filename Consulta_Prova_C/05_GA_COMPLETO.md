# 05 - GA COMPLETO (material do Grau A)

Material completo do GA: vetor, matriz, struct basico, string basico,
funcoes, lacos, condicionais e calculos. Use para questoes do GA.

INDICE DESTE ARQUIVO:
  PARTE 1 - Mega guia tudo comentado (67 secoes)
  PARTE 2 - Guia unico resumido


#############################################################
# PARTE 1 - MEGA GUIA TUDO COMENTADO
#############################################################

# Mega guia de prova C - tudo comentado

Este arquivo foi feito para abrir no VS Code e consultar rapido durante estudo ou prova, se a consulta for permitida.

Use `Ctrl+F` por palavras como: `scanf`, `printf`, `if`, `switch`, `for`, `while`, `vetor`, `matriz`, `string`, `struct`, `funcao`, `media`, `porcentagem`, `maior`, `menor`, `buscar`, `ordenar`, `diagonal`, `fgets`, `strcmp`, `cadastro`, `produto`, `dengue`.

## Como pensar qualquer questao

Quase toda prova de C vira uma mistura destes blocos:

- Ler dados: `scanf`, `fgets`.
- Guardar dados: variaveis, vetor, matriz, struct.
- Repetir: `for`, `while`, `do while`.
- Decidir: `if`, `else if`, `switch`.
- Acumular: `soma += valor`.
- Contar: `contador++`.
- Comparar: maior, menor, busca.
- Calcular: media, porcentagem, formula.
- Mostrar resultado: `printf`.
- Separar em funcoes: quando o enunciado pede modularizacao.

## Padroes de enunciado

Quando o enunciado disser:

- "calcule o total": use acumulador `soma`.
- "quantos": use contador.
- "media": use `soma / quantidade`.
- "maior/menor": inicialize com o primeiro valor e compare.
- "vetor/lista": use `for` de `0` ate `n - 1`.
- "tabela", "mes x hospital", "linhas e colunas": use matriz.
- "nome, idade, nota juntos": use `struct`.
- "frase", "nome completo", "senha": use string.
- "opcoes": use `switch` ou menu com `do while`.
- "repetir ate digitar 0": use `while` com sentinela.
- "funcao que retorna": use tipo diferente de `void`.
- "funcao que apenas mostra/altera": use `void`.

## Esqueleto basico de programa

```c
#include <stdio.h> // Biblioteca para printf e scanf.

int main(void) { // Funcao principal: todo programa C comeca aqui.
    // Declare variaveis antes de usar.
    int numero; // Guarda um numero inteiro.

    printf("Digite um numero: "); // Mostra mensagem para o usuario.
    scanf("%d", &numero); // Le um inteiro. O & passa o endereco da variavel.

    printf("Voce digitou: %d\n", numero); // Mostra o valor lido.

    return 0; // Indica que o programa terminou corretamente.
}
```

## Tipos, scanf e printf

```c
#include <stdio.h> // Entrada e saida.

int main(void) {
    int idade; // Numero inteiro.
    float nota; // Numero real com menos precisao.
    double salario; // Numero real com mais precisao.
    char letra; // Um caractere.
    char nome[50]; // String: vetor de char.

    printf("Idade: ");
    scanf("%d", &idade); // %d le int.

    printf("Nota: ");
    scanf("%f", &nota); // %f le float.

    printf("Salario: ");
    scanf("%lf", &salario); // %lf le double no scanf.

    printf("Letra: ");
    scanf(" %c", &letra); // Espaco antes de %c ignora enter anterior.

    printf("Nome sem espaco: ");
    scanf("%49s", nome); // String nao usa & porque nome ja e endereco.

    printf("Idade = %d\n", idade); // %d imprime int.
    printf("Nota = %.2f\n", nota); // %.2f imprime com 2 casas.
    printf("Salario = %.2lf\n", salario); // %.2lf imprime double.
    printf("Letra = %c\n", letra); // %c imprime char.
    printf("Nome = %s\n", nome); // %s imprime string.

    return 0;
}
```

## 1. Media simples e aprovacao

Palavras-chave: `media`, `aprovado`, `reprovado`, `if`, `else`

```c
#include <stdio.h>

int main(void) {
    float n1, n2, media; // Guarda duas notas e a media.

    printf("Digite duas notas: ");
    scanf("%f %f", &n1, &n2); // Le as duas notas.

    media = (n1 + n2) / 2.0; // Usa 2.0 para garantir divisao real.

    printf("Media = %.2f\n", media); // Mostra a media formatada.

    if (media >= 7.0) { // Condicao para aprovado.
        printf("Aprovado\n");
    } else { // Se nao passou na condicao acima, esta reprovado.
        printf("Reprovado\n");
    }

    return 0;
}
```

Variacao comum:

```c
if (media >= 7.0) { // Media boa.
    printf("Aprovado\n");
} else if (media >= 4.0) { // Media intermediaria.
    printf("Recuperacao\n");
} else { // Media menor que 4.
    printf("Reprovado\n");
}
```

## 2. Media ponderada

Palavras-chave: `media ponderada`, `peso`, `nota`

```c
#include <stdio.h>

int main(void) {
    float n1, n2, n3; // Notas.
    float p1, p2, p3; // Pesos.
    float media; // Resultado.

    printf("Digite 3 notas: ");
    scanf("%f %f %f", &n1, &n2, &n3);

    printf("Digite 3 pesos: ");
    scanf("%f %f %f", &p1, &p2, &p3);

    // Multiplica cada nota pelo peso e divide pela soma dos pesos.
    media = (n1 * p1 + n2 * p2 + n3 * p3) / (p1 + p2 + p3);

    printf("Media ponderada = %.2f\n", media);

    return 0;
}
```

## 3. Par, impar, positivo, negativo

Palavras-chave: `par`, `impar`, `positivo`, `negativo`, `%`

```c
#include <stdio.h>

int main(void) {
    int n; // Numero analisado.

    printf("Digite um numero: ");
    scanf("%d", &n);

    if (n % 2 == 0) { // Resto da divisao por 2 igual a zero.
        printf("Par\n");
    } else { // Resto diferente de zero.
        printf("Impar\n");
    }

    if (n > 0) { // Maior que zero.
        printf("Positivo\n");
    } else if (n < 0) { // Menor que zero.
        printf("Negativo\n");
    } else { // Nem positivo nem negativo.
        printf("Zero\n");
    }

    return 0;
}
```

## 4. Maior entre tres numeros

Palavras-chave: `maior`, `menor`, `if`, `comparar`

```c
#include <stdio.h>

int main(void) {
    int a, b, c; // Tres numeros.
    int maior; // Guarda o maior.

    printf("Digite tres numeros: ");
    scanf("%d %d %d", &a, &b, &c);

    maior = a; // Assume que o primeiro e o maior.

    if (b > maior) { // Se b for maior que o maior atual.
        maior = b; // Atualiza maior.
    }

    if (c > maior) { // Se c for maior que o maior atual.
        maior = c; // Atualiza maior.
    }

    printf("Maior = %d\n", maior);

    return 0;
}
```

Variacao com menor:

```c
menor = a; // Comeca assumindo que a e o menor.

if (b < menor) { // Se b for menor.
    menor = b; // Atualiza.
}

if (c < menor) { // Se c for menor.
    menor = c; // Atualiza.
}
```

## 5. Calculadora com switch

Palavras-chave: `switch`, `case`, `calculadora`, `opcao`

```c
#include <stdio.h>

int main(void) {
    float a, b; // Numeros usados nas operacoes.
    int opcao; // Escolha do usuario.

    printf("Digite dois numeros: ");
    scanf("%f %f", &a, &b);

    printf("1 - Somar\n");
    printf("2 - Subtrair\n");
    printf("3 - Multiplicar\n");
    printf("4 - Dividir\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) { // Testa a opcao escolhida.
        case 1:
            printf("Resultado = %.2f\n", a + b);
            break; // Sai do switch.

        case 2:
            printf("Resultado = %.2f\n", a - b);
            break;

        case 3:
            printf("Resultado = %.2f\n", a * b);
            break;

        case 4:
            if (b != 0) { // Evita divisao por zero.
                printf("Resultado = %.2f\n", a / b);
            } else {
                printf("Erro: divisao por zero\n");
            }
            break;

        default: // Se a opcao nao for 1, 2, 3 ou 4.
            printf("Opcao invalida\n");
            break;
    }

    return 0;
}
```

## 6. Menu com do while

Palavras-chave: `menu`, `do while`, `opcao`, `sair`

```c
#include <stdio.h>

int main(void) {
    int opcao; // Guarda a opcao escolhida.

    do { // Executa pelo menos uma vez.
        printf("\nMENU\n");
        printf("1 - Dizer oi\n");
        printf("2 - Mostrar ajuda\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Oi!\n");
                break;

            case 2:
                printf("Escolha uma opcao do menu.\n");
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 0); // Repete ate o usuario escolher sair.

    return 0;
}
```

## 7. Validar nota entre 0 e 10

Palavras-chave: `validar`, `nota`, `while`, `entrada valida`

```c
#include <stdio.h>

int main(void) {
    float nota; // Nota digitada.

    printf("Digite uma nota entre 0 e 10: ");
    scanf("%f", &nota);

    while (nota < 0 || nota > 10) { // Enquanto estiver fora do intervalo.
        printf("Nota invalida. Digite novamente: ");
        scanf("%f", &nota);
    }

    printf("Nota valida = %.2f\n", nota);

    return 0;
}
```

## 8. Somar de 1 ate N

Palavras-chave: `for`, `soma`, `acumulador`, `1 ate n`

```c
#include <stdio.h>

int main(void) {
    int n; // Limite final.
    int soma = 0; // Acumulador precisa comecar em zero.

    printf("Digite N: ");
    scanf("%d", &n);

    for (int i = 1; i <= n; i++) { // Vai de 1 ate N.
        soma += i; // Mesmo que soma = soma + i.
    }

    printf("Soma = %d\n", soma);

    return 0;
}
```

## 9. Contar pares em um intervalo

Palavras-chave: `contador`, `pares`, `intervalo`, `%`

```c
#include <stdio.h>

int main(void) {
    int inicio, fim; // Limites.
    int contador = 0; // Quantidade de pares encontrados.

    printf("Inicio e fim: ");
    scanf("%d %d", &inicio, &fim);

    for (int n = inicio; n <= fim; n++) { // Percorre o intervalo.
        if (n % 2 == 0) { // Se for par.
            contador++; // Conta mais um.
        }
    }

    printf("Quantidade de pares = %d\n", contador);

    return 0;
}
```

## 10. Repetir ate digitar zero

Palavras-chave: `sentinela`, `while`, `zero`, `soma`

```c
#include <stdio.h>

int main(void) {
    int numero; // Numero digitado.
    int soma = 0; // Soma dos numeros.
    int quantidade = 0; // Quantidade digitada sem contar o zero.

    printf("Digite numeros. Zero encerra.\n");
    scanf("%d", &numero); // Primeira leitura antes do while.

    while (numero != 0) { // Continua enquanto nao for zero.
        soma += numero; // Soma o numero.
        quantidade++; // Conta o numero.

        scanf("%d", &numero); // Le o proximo.
    }

    printf("Soma = %d\n", soma);
    printf("Quantidade = %d\n", quantidade);

    if (quantidade > 0) { // Evita dividir por zero.
        printf("Media = %.2f\n", soma / (float)quantidade);
    }

    return 0;
}
```

## 11. Tabuada

Palavras-chave: `tabuada`, `for`, `multiplicacao`

```c
#include <stdio.h>

int main(void) {
    int n; // Numero da tabuada.

    printf("Digite um numero: ");
    scanf("%d", &n);

    for (int i = 1; i <= 10; i++) { // De 1 ate 10.
        printf("%d x %d = %d\n", n, i, n * i);
    }

    return 0;
}
```

## 12. Fatorial

Palavras-chave: `fatorial`, `for`, `multiplicar`

```c
#include <stdio.h>

int main(void) {
    int n; // Numero.
    int fat = 1; // Fatorial comeca em 1, nao em zero.

    printf("Digite N: ");
    scanf("%d", &n);

    if (n < 0) { // Fatorial de negativo nao e usado no basico.
        printf("Nao existe fatorial de negativo.\n");
    } else {
        for (int i = 1; i <= n; i++) {
            fat *= i; // Multiplica acumulando.
        }

        printf("%d! = %d\n", n, fat);
    }

    return 0;
}
```

## 13. Fibonacci

Palavras-chave: `fibonacci`, `sequencia`, `for`

```c
#include <stdio.h>

int main(void) {
    int n; // Quantidade de termos.
    int a = 0; // Primeiro termo.
    int b = 1; // Segundo termo.
    int proximo; // Proximo termo.

    printf("Quantidade de termos: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("%d ", a); // Mostra o termo atual.

        proximo = a + b; // Calcula o proximo.
        a = b; // Avanca a.
        b = proximo; // Avanca b.
    }

    printf("\n");

    return 0;
}
```

## 14. Numero primo

Palavras-chave: `primo`, `divisor`, `%`

```c
#include <stdio.h>

int main(void) {
    int n; // Numero analisado.
    int primo = 1; // Assume que e primo.

    printf("Digite um numero: ");
    scanf("%d", &n);

    if (n < 2) { // Menor que 2 nao e primo.
        primo = 0;
    }

    for (int d = 2; d * d <= n; d++) { // Testa divisores ate raiz de n.
        if (n % d == 0) { // Achou divisor.
            primo = 0; // Entao nao e primo.
        }
    }

    if (primo) {
        printf("Primo\n");
    } else {
        printf("Nao primo\n");
    }

    return 0;
}
```

## 15. MDC e MMC

Palavras-chave: `mdc`, `mmc`, `resto`, `euclides`

```c
#include <stdio.h>

int main(void) {
    int a, b; // Numeros originais.
    int x, y; // Copias para calcular MDC.
    int resto; // Resto da divisao.
    int mdc, mmc; // Resultados.

    printf("Digite dois numeros: ");
    scanf("%d %d", &a, &b);

    x = a; // Copia a.
    y = b; // Copia b.

    while (y != 0) { // Algoritmo de Euclides.
        resto = x % y; // Calcula resto.
        x = y; // Move y para x.
        y = resto; // Move resto para y.
    }

    mdc = x; // Quando y vira zero, x e o MDC.

    if (mdc != 0) { // Evita divisao por zero.
        mmc = (a / mdc) * b; // Formula do MMC.
        printf("MDC = %d\n", mdc);
        printf("MMC = %d\n", mmc);
    }

    return 0;
}
```

## 16. Porcentagem, desconto e aumento

Palavras-chave: `porcentagem`, `desconto`, `aumento`

```c
#include <stdio.h>

int main(void) {
    float preco, percentual; // Entrada.
    float desconto, precoFinal, aumento; // Resultados.

    printf("Preco e percentual: ");
    scanf("%f %f", &preco, &percentual);

    desconto = preco * percentual / 100.0; // Valor do desconto.
    precoFinal = preco - desconto; // Preco depois do desconto.
    aumento = preco + (preco * percentual / 100.0); // Preco com aumento.

    printf("Desconto = %.2f\n", desconto);
    printf("Preco com desconto = %.2f\n", precoFinal);
    printf("Preco com aumento = %.2f\n", aumento);

    return 0;
}
```

## 17. Regra de tres

Palavras-chave: `regra de tres`, `proporcao`

```c
#include <stdio.h>

int main(void) {
    float a, b, c, x; // Se a esta para b, c esta para x.

    printf("Digite a, b e c: ");
    scanf("%f %f %f", &a, &b, &c);

    // Exemplo: a unidades custam b reais. c unidades custam x.
    x = (b * c) / a;

    printf("Resultado = %.2f\n", x);

    return 0;
}
```

## 18. IMC

Palavras-chave: `IMC`, `peso`, `altura`, `classificacao`

```c
#include <stdio.h>

int main(void) {
    float peso, altura, imc; // Dados.

    printf("Peso e altura: ");
    scanf("%f %f", &peso, &altura);

    imc = peso / (altura * altura); // Formula do IMC.

    printf("IMC = %.2f\n", imc);

    if (imc < 18.5) {
        printf("Abaixo do peso\n");
    } else if (imc < 25.0) {
        printf("Peso normal\n");
    } else if (imc < 30.0) {
        printf("Sobrepeso\n");
    } else {
        printf("Obesidade\n");
    }

    return 0;
}
```

## 19. Temperatura

Palavras-chave: `celsius`, `fahrenheit`, `temperatura`

```c
#include <stdio.h>

int main(void) {
    float celsius, fahrenheit; // Temperaturas.

    printf("Celsius: ");
    scanf("%f", &celsius);

    fahrenheit = celsius * 9.0 / 5.0 + 32.0; // Formula de conversao.

    printf("Fahrenheit = %.2f\n", fahrenheit);

    return 0;
}
```

## 20. Salario, comissao e bonus por faixa

Palavras-chave: `salario`, `comissao`, `bonus`, `faixa`

```c
#include <stdio.h>

int main(void) {
    float salarioBase, vendas, salarioFinal; // Dados.
    float comissao; // Valor calculado.

    printf("Salario base e vendas: ");
    scanf("%f %f", &salarioBase, &vendas);

    if (vendas <= 1000.0) { // Faixa baixa.
        comissao = vendas * 0.03; // 3%.
    } else if (vendas <= 5000.0) { // Faixa media.
        comissao = vendas * 0.05; // 5%.
    } else { // Faixa alta.
        comissao = vendas * 0.08; // 8%.
    }

    salarioFinal = salarioBase + comissao; // Soma salario e comissao.

    printf("Comissao = %.2f\n", comissao);
    printf("Salario final = %.2f\n", salarioFinal);

    return 0;
}
```

## 21. Juros simples e composto

Palavras-chave: `juros simples`, `juros composto`, `math.h`, `pow`

```c
#include <math.h> // Necessario para pow.
#include <stdio.h>

int main(void) {
    double capital, taxa, tempo; // Entrada.
    double jurosSimples, montanteSimples; // Juros simples.
    double montanteComposto; // Juros composto.

    printf("Capital, taxa mensal em %% e tempo em meses: ");
    scanf("%lf %lf %lf", &capital, &taxa, &tempo);

    jurosSimples = capital * (taxa / 100.0) * tempo; // J = C*i*t.
    montanteSimples = capital + jurosSimples; // M = C + J.

    montanteComposto = capital * pow(1.0 + taxa / 100.0, tempo); // M = C*(1+i)^t.

    printf("Montante simples = %.2lf\n", montanteSimples);
    printf("Montante composto = %.2lf\n", montanteComposto);

    return 0;
}
```

No GCC, se usar `math.h`, compile com `-lm`:

```sh
gcc arquivo.c -o programa -lm
```

## 22. Distancia, velocidade e consumo

Palavras-chave: `distancia`, `velocidade`, `tempo`, `consumo`

```c
#include <stdio.h>

int main(void) {
    float velocidade, tempo, distancia; // Movimento.
    float litros, consumo; // Combustivel.

    printf("Velocidade media e tempo: ");
    scanf("%f %f", &velocidade, &tempo);

    distancia = velocidade * tempo; // D = V * T.

    printf("Litros usados: ");
    scanf("%f", &litros);

    consumo = distancia / litros; // Km por litro.

    printf("Distancia = %.2f km\n", distancia);
    printf("Consumo = %.2f km/l\n", consumo);

    return 0;
}
```

## 23. Areas, perimetros e Pitagoras

Palavras-chave: `area`, `perimetro`, `pitagoras`, `sqrt`

```c
#include <math.h>
#include <stdio.h>

int main(void) {
    float base, altura; // Medidas.
    float areaRetangulo, areaTriangulo; // Areas.
    float cateto1, cateto2, hipotenusa; // Pitagoras.

    printf("Base e altura: ");
    scanf("%f %f", &base, &altura);

    areaRetangulo = base * altura; // Area do retangulo.
    areaTriangulo = (base * altura) / 2.0; // Area do triangulo.

    printf("Dois catetos: ");
    scanf("%f %f", &cateto1, &cateto2);

    hipotenusa = sqrt(cateto1 * cateto1 + cateto2 * cateto2); // Pitagoras.

    printf("Area retangulo = %.2f\n", areaRetangulo);
    printf("Area triangulo = %.2f\n", areaTriangulo);
    printf("Hipotenusa = %.2f\n", hipotenusa);

    return 0;
}
```

## 24. Bhaskara

Palavras-chave: `bhaskara`, `delta`, `raiz`, `math.h`

```c
#include <math.h>
#include <stdio.h>

int main(void) {
    double a, b, c; // Coeficientes.
    double delta, x1, x2; // Resultados.

    printf("Digite a, b e c: ");
    scanf("%lf %lf %lf", &a, &b, &c);

    delta = b * b - 4.0 * a * c; // Formula do delta.

    if (a == 0) { // Se a for zero, nao e equacao do segundo grau.
        printf("Nao e equacao do segundo grau.\n");
    } else if (delta < 0) { // Delta negativo nao tem raiz real.
        printf("Nao existem raizes reais.\n");
    } else {
        x1 = (-b + sqrt(delta)) / (2.0 * a); // Primeira raiz.
        x2 = (-b - sqrt(delta)) / (2.0 * a); // Segunda raiz.

        printf("x1 = %.2lf\n", x1);
        printf("x2 = %.2lf\n", x2);
    }

    return 0;
}
```

## 25. PA e PG

Palavras-chave: `PA`, `PG`, `progressao`, `razao`

```c
#include <stdio.h>

int main(void) {
    int primeiro, razao, termos; // Dados da sequencia.
    int atual; // Termo atual.

    printf("Primeiro termo, razao e quantidade: ");
    scanf("%d %d %d", &primeiro, &razao, &termos);

    atual = primeiro; // PA comeca no primeiro termo.
    printf("PA: ");
    for (int i = 0; i < termos; i++) {
        printf("%d ", atual);
        atual += razao; // Na PA, soma a razao.
    }

    atual = primeiro; // PG tambem comeca no primeiro termo.
    printf("\nPG: ");
    for (int i = 0; i < termos; i++) {
        printf("%d ", atual);
        atual *= razao; // Na PG, multiplica pela razao.
    }

    printf("\n");

    return 0;
}
```

## 26. Vetor: ler e imprimir

Palavras-chave: `vetor`, `array`, `for`, `ler vetor`

```c
#include <stdio.h>

#define TAM 5 // Constante para tamanho do vetor.

int main(void) {
    int v[TAM]; // Vetor com 5 inteiros.

    for (int i = 0; i < TAM; i++) { // Indices vao de 0 ate TAM - 1.
        printf("v[%d]: ", i);
        scanf("%d", &v[i]); // Le uma posicao do vetor.
    }

    printf("Valores digitados:\n");
    for (int i = 0; i < TAM; i++) {
        printf("%d ", v[i]); // Imprime cada posicao.
    }

    printf("\n");

    return 0;
}
```

## 27. Vetor: soma, media, maior e menor

Palavras-chave: `soma vetor`, `media vetor`, `maior vetor`, `menor vetor`

```c
#include <stdio.h>

#define TAM 5

int main(void) {
    int v[TAM]; // Vetor.
    int soma = 0; // Acumulador.
    int maior, menor; // Comparadores.
    float media; // Media.

    for (int i = 0; i < TAM; i++) {
        printf("Digite v[%d]: ", i);
        scanf("%d", &v[i]);
    }

    maior = v[0]; // Comeca pelo primeiro elemento.
    menor = v[0]; // Comeca pelo primeiro elemento.

    for (int i = 0; i < TAM; i++) {
        soma += v[i]; // Soma todos.

        if (v[i] > maior) {
            maior = v[i]; // Atualiza maior.
        }

        if (v[i] < menor) {
            menor = v[i]; // Atualiza menor.
        }
    }

    media = soma / (float)TAM; // Converte para float.

    printf("Soma = %d\n", soma);
    printf("Media = %.2f\n", media);
    printf("Maior = %d\n", maior);
    printf("Menor = %d\n", menor);

    return 0;
}
```

## 28. Vetor: contar pares, impares, positivos e negativos

Palavras-chave: `contar pares`, `contador`, `vetor`

```c
#include <stdio.h>

#define TAM 10

int main(void) {
    int v[TAM]; // Vetor.
    int pares = 0, impares = 0; // Contadores.
    int positivos = 0, negativos = 0, zeros = 0; // Contadores.

    for (int i = 0; i < TAM; i++) {
        scanf("%d", &v[i]); // Le cada valor.
    }

    for (int i = 0; i < TAM; i++) {
        if (v[i] % 2 == 0) {
            pares++;
        } else {
            impares++;
        }

        if (v[i] > 0) {
            positivos++;
        } else if (v[i] < 0) {
            negativos++;
        } else {
            zeros++;
        }
    }

    printf("Pares = %d\n", pares);
    printf("Impares = %d\n", impares);
    printf("Positivos = %d\n", positivos);
    printf("Negativos = %d\n", negativos);
    printf("Zeros = %d\n", zeros);

    return 0;
}
```

## 29. Vetor: buscar valor

Palavras-chave: `buscar`, `posicao`, `encontrado`, `vetor`

```c
#include <stdio.h>

#define TAM 5

int main(void) {
    int v[TAM]; // Vetor.
    int procurado; // Valor buscado.
    int posicao = -1; // -1 significa nao encontrado.

    for (int i = 0; i < TAM; i++) {
        scanf("%d", &v[i]);
    }

    printf("Valor procurado: ");
    scanf("%d", &procurado);

    for (int i = 0; i < TAM; i++) {
        if (v[i] == procurado) { // Achou valor.
            posicao = i; // Guarda a posicao.
        }
    }

    if (posicao != -1) {
        printf("Encontrado na posicao %d\n", posicao);
    } else {
        printf("Nao encontrado\n");
    }

    return 0;
}
```

Variacao para parar na primeira ocorrencia:

```c
for (int i = 0; i < TAM; i++) {
    if (v[i] == procurado) {
        posicao = i; // Guarda onde achou.
        break; // Para o for.
    }
}
```

## 30. Vetor: contar ocorrencias

Palavras-chave: `ocorrencias`, `contar`, `valor repetido`

```c
#include <stdio.h>

#define TAM 8

int main(void) {
    int v[TAM]; // Vetor.
    int valor; // Valor procurado.
    int contador = 0; // Quantas vezes apareceu.

    for (int i = 0; i < TAM; i++) {
        scanf("%d", &v[i]);
    }

    printf("Valor para contar: ");
    scanf("%d", &valor);

    for (int i = 0; i < TAM; i++) {
        if (v[i] == valor) {
            contador++; // Conta ocorrencia.
        }
    }

    printf("Apareceu %d vezes\n", contador);

    return 0;
}
```

## 31. Vetor: inverter

Palavras-chave: `inverter vetor`, `fim`, `troca`

```c
#include <stdio.h>

#define TAM 6

int main(void) {
    int v[TAM]; // Vetor.
    int aux; // Variavel temporaria para troca.

    for (int i = 0; i < TAM; i++) {
        scanf("%d", &v[i]);
    }

    for (int i = 0; i < TAM / 2; i++) {
        aux = v[i]; // Guarda o inicio.
        v[i] = v[TAM - 1 - i]; // Move o fim para o inicio.
        v[TAM - 1 - i] = aux; // Move o inicio para o fim.
    }

    for (int i = 0; i < TAM; i++) {
        printf("%d ", v[i]);
    }

    printf("\n");

    return 0;
}
```

## 32. Vetor: ordenar crescente com bubble sort

Palavras-chave: `ordenar`, `crescente`, `bubble sort`, `troca`

```c
#include <stdio.h>

#define TAM 5

int main(void) {
    int v[TAM]; // Vetor.
    int aux; // Auxiliar para trocar.

    for (int i = 0; i < TAM; i++) {
        scanf("%d", &v[i]);
    }

    for (int i = 0; i < TAM - 1; i++) { // Quantidade de passadas.
        for (int j = 0; j < TAM - 1 - i; j++) { // Compara vizinhos.
            if (v[j] > v[j + 1]) { // Se estiver fora de ordem.
                aux = v[j]; // Troca.
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
        }
    }

    printf("Ordenado: ");
    for (int i = 0; i < TAM; i++) {
        printf("%d ", v[i]);
    }

    printf("\n");

    return 0;
}
```

## 33. Vetor: media apenas dos positivos

Palavras-chave: `media positivos`, `contador`, `condicao`

```c
#include <stdio.h>

#define TAM 6

int main(void) {
    int v[TAM]; // Vetor.
    int soma = 0; // Soma apenas dos positivos.
    int contador = 0; // Quantidade de positivos.
    float media; // Media.

    for (int i = 0; i < TAM; i++) {
        scanf("%d", &v[i]);
    }

    for (int i = 0; i < TAM; i++) {
        if (v[i] > 0) { // Filtra positivos.
            soma += v[i]; // Soma positivo.
            contador++; // Conta positivo.
        }
    }

    if (contador > 0) { // Evita divisao por zero.
        media = soma / (float)contador;
        printf("Media dos positivos = %.2f\n", media);
    } else {
        printf("Nenhum positivo digitado.\n");
    }

    return 0;
}
```

## 34. Vetor: separar pares e impares

Palavras-chave: `separar`, `pares`, `impares`, `vetor auxiliar`

```c
#include <stdio.h>

#define TAM 8

int main(void) {
    int v[TAM]; // Vetor original.
    int pares[TAM], impares[TAM]; // Vetores auxiliares.
    int qtdPares = 0, qtdImpares = 0; // Tamanhos usados.

    for (int i = 0; i < TAM; i++) {
        scanf("%d", &v[i]);
    }

    for (int i = 0; i < TAM; i++) {
        if (v[i] % 2 == 0) {
            pares[qtdPares] = v[i]; // Guarda no vetor de pares.
            qtdPares++; // Avanca posicao usada.
        } else {
            impares[qtdImpares] = v[i]; // Guarda no vetor de impares.
            qtdImpares++;
        }
    }

    printf("Pares: ");
    for (int i = 0; i < qtdPares; i++) {
        printf("%d ", pares[i]);
    }

    printf("\nImpares: ");
    for (int i = 0; i < qtdImpares; i++) {
        printf("%d ", impares[i]);
    }

    printf("\n");

    return 0;
}
```

## 35. Vetor em funcao

Palavras-chave: `funcao vetor`, `parametro`, `int v[]`

```c
#include <stdio.h>

#define TAM 5

void lerVetor(int v[], int tamanho) { // Recebe vetor e tamanho.
    for (int i = 0; i < tamanho; i++) {
        printf("v[%d]: ", i);
        scanf("%d", &v[i]); // Altera o vetor original.
    }
}

int somarVetor(int v[], int tamanho) { // Retorna a soma.
    int soma = 0;

    for (int i = 0; i < tamanho; i++) {
        soma += v[i];
    }

    return soma;
}

int main(void) {
    int numeros[TAM]; // Vetor principal.
    int soma; // Resultado.

    lerVetor(numeros, TAM); // Chama funcao para ler.
    soma = somarVetor(numeros, TAM); // Chama funcao para somar.

    printf("Soma = %d\n", soma);

    return 0;
}
```

## 36. Matriz: ler e imprimir

Palavras-chave: `matriz`, `linha`, `coluna`, `for dentro de for`

```c
#include <stdio.h>

#define LINHAS 3
#define COLUNAS 4

int main(void) {
    int m[LINHAS][COLUNAS]; // Matriz 3 x 4.

    for (int i = 0; i < LINHAS; i++) { // Percorre linhas.
        for (int j = 0; j < COLUNAS; j++) { // Percorre colunas.
            printf("m[%d][%d]: ", i, j);
            scanf("%d", &m[i][j]); // Le elemento.
        }
    }

    printf("Matriz:\n");
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            printf("%4d", m[i][j]); // Largura 4 para alinhar.
        }
        printf("\n"); // Nova linha ao terminar uma linha da matriz.
    }

    return 0;
}
```

## 37. Matriz: soma total e media

Palavras-chave: `soma matriz`, `media matriz`, `total matriz`

```c
#include <stdio.h>

#define LINHAS 2
#define COLUNAS 3

int main(void) {
    int m[LINHAS][COLUNAS]; // Matriz.
    int soma = 0; // Soma total.
    float media; // Media geral.

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            scanf("%d", &m[i][j]);
            soma += m[i][j]; // Ja soma enquanto le.
        }
    }

    media = soma / (float)(LINHAS * COLUNAS); // Total de elementos.

    printf("Soma total = %d\n", soma);
    printf("Media geral = %.2f\n", media);

    return 0;
}
```

## 38. Matriz: soma por linha

Palavras-chave: `soma linha`, `total por linha`, `matriz`

```c
#include <stdio.h>

#define LINHAS 3
#define COLUNAS 4

int main(void) {
    int m[LINHAS][COLUNAS]; // Matriz.
    int somaLinha; // Soma de uma linha por vez.

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            scanf("%d", &m[i][j]);
        }
    }

    for (int i = 0; i < LINHAS; i++) {
        somaLinha = 0; // Zera para cada linha.

        for (int j = 0; j < COLUNAS; j++) {
            somaLinha += m[i][j]; // Soma colunas da linha i.
        }

        printf("Soma da linha %d = %d\n", i, somaLinha);
    }

    return 0;
}
```

## 39. Matriz: soma por coluna

Palavras-chave: `soma coluna`, `total por coluna`, `matriz`

```c
#include <stdio.h>

#define LINHAS 3
#define COLUNAS 4

int main(void) {
    int m[LINHAS][COLUNAS]; // Matriz.
    int somaColuna; // Soma de uma coluna por vez.

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            scanf("%d", &m[i][j]);
        }
    }

    for (int j = 0; j < COLUNAS; j++) { // Coluna por fora.
        somaColuna = 0; // Zera para cada coluna.

        for (int i = 0; i < LINHAS; i++) { // Linha por dentro.
            somaColuna += m[i][j];
        }

        printf("Soma da coluna %d = %d\n", j, somaColuna);
    }

    return 0;
}
```

## 40. Matriz: maior e menor com posicao

Palavras-chave: `maior matriz`, `menor matriz`, `posicao`

```c
#include <stdio.h>

#define LINHAS 3
#define COLUNAS 3

int main(void) {
    int m[LINHAS][COLUNAS]; // Matriz.
    int maior, menor; // Valores.
    int linMaior = 0, colMaior = 0; // Posicao do maior.
    int linMenor = 0, colMenor = 0; // Posicao do menor.

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            scanf("%d", &m[i][j]);
        }
    }

    maior = m[0][0]; // Inicializa com primeiro elemento.
    menor = m[0][0]; // Inicializa com primeiro elemento.

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (m[i][j] > maior) {
                maior = m[i][j];
                linMaior = i;
                colMaior = j;
            }

            if (m[i][j] < menor) {
                menor = m[i][j];
                linMenor = i;
                colMenor = j;
            }
        }
    }

    printf("Maior = %d em [%d][%d]\n", maior, linMaior, colMaior);
    printf("Menor = %d em [%d][%d]\n", menor, linMenor, colMenor);

    return 0;
}
```

## 41. Matriz quadrada: diagonais

Palavras-chave: `diagonal principal`, `diagonal secundaria`, `matriz quadrada`

```c
#include <stdio.h>

#define N 3

int main(void) {
    int m[N][N]; // Matriz quadrada.
    int somaPrincipal = 0; // Soma da diagonal principal.
    int somaSecundaria = 0; // Soma da diagonal secundaria.

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &m[i][j]);
        }
    }

    for (int i = 0; i < N; i++) {
        somaPrincipal += m[i][i]; // Principal: linha == coluna.
        somaSecundaria += m[i][N - 1 - i]; // Secundaria: coluna invertida.
    }

    printf("Diagonal principal = %d\n", somaPrincipal);
    printf("Diagonal secundaria = %d\n", somaSecundaria);

    return 0;
}
```

## 42. Matriz: transposta

Palavras-chave: `transposta`, `trocar linha coluna`

```c
#include <stdio.h>

#define LINHAS 2
#define COLUNAS 3

int main(void) {
    int m[LINHAS][COLUNAS]; // Matriz original 2x3.
    int t[COLUNAS][LINHAS]; // Transposta vira 3x2.

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            scanf("%d", &m[i][j]);
        }
    }

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            t[j][i] = m[i][j]; // Linha vira coluna.
        }
    }

    for (int i = 0; i < COLUNAS; i++) {
        for (int j = 0; j < LINHAS; j++) {
            printf("%4d", t[i][j]);
        }
        printf("\n");
    }

    return 0;
}
```

## 43. Matriz em funcao

Palavras-chave: `funcao matriz`, `colunas obrigatorias`

```c
#include <stdio.h>

#define LINHAS 2
#define COLUNAS 3

void lerMatriz(int m[LINHAS][COLUNAS]) { // Precisa informar as colunas.
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            scanf("%d", &m[i][j]);
        }
    }
}

int somarMatriz(int m[LINHAS][COLUNAS]) { // Retorna soma total.
    int soma = 0;

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            soma += m[i][j];
        }
    }

    return soma;
}

int main(void) {
    int matriz[LINHAS][COLUNAS]; // Matriz principal.
    int soma; // Resultado.

    lerMatriz(matriz); // Le a matriz.
    soma = somarMatriz(matriz); // Calcula a soma.

    printf("Soma = %d\n", soma);

    return 0;
}
```

## 44. Modelo dengue: matriz 6 x 5

Palavras-chave: `dengue`, `matriz 6x5`, `hospital`, `mes`, `internados`

```c
#include <stdio.h>

#define MESES 6
#define HOSPITAIS 5

void lerDados(int m[MESES][HOSPITAIS]) {
    for (int mes = 0; mes < MESES; mes++) {
        for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
            printf("Mes %d, hospital %d: ", mes + 1, hospital + 1);
            scanf("%d", &m[mes][hospital]); // Internados naquele mes e hospital.
        }
    }
}

void calcularTotalPorMes(int m[MESES][HOSPITAIS], int totalMes[MESES]) {
    for (int mes = 0; mes < MESES; mes++) {
        totalMes[mes] = 0; // Zera antes de somar a linha.

        for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
            totalMes[mes] += m[mes][hospital]; // Soma hospitais do mes.
        }
    }
}

void calcularTotalPorHospital(int m[MESES][HOSPITAIS], int totalHospital[HOSPITAIS]) {
    for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
        totalHospital[hospital] = 0; // Zera antes de somar a coluna.

        for (int mes = 0; mes < MESES; mes++) {
            totalHospital[hospital] += m[mes][hospital]; // Soma meses do hospital.
        }
    }
}

float calcularMedia(int m[MESES][HOSPITAIS]) {
    int soma = 0; // Soma todos os internados.

    for (int mes = 0; mes < MESES; mes++) {
        for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
            soma += m[mes][hospital];
        }
    }

    return soma / (float)(MESES * HOSPITAIS); // Media geral.
}

float calcularRepasse(int quantidade) {
    if (quantidade <= 50) {
        return quantidade * 1000.0; // Faixa 1.
    } else if (quantidade <= 100) {
        return quantidade * 1200.0; // Faixa 2.
    } else {
        return quantidade * 1500.0; // Faixa 3.
    }
}

int main(void) {
    int internados[MESES][HOSPITAIS]; // Matriz principal.
    int totalMes[MESES]; // Vetor com total de cada mes.
    int totalHospital[HOSPITAIS]; // Vetor com total de cada hospital.
    float media; // Media geral.

    lerDados(internados);
    calcularTotalPorMes(internados, totalMes);
    calcularTotalPorHospital(internados, totalHospital);
    media = calcularMedia(internados);

    for (int mes = 0; mes < MESES; mes++) {
        printf("Total do mes %d = %d\n", mes + 1, totalMes[mes]);
    }

    for (int hospital = 0; hospital < HOSPITAIS; hospital++) {
        printf("Total do hospital %d = %d\n", hospital + 1, totalHospital[hospital]);
        printf("Repasse = %.2f\n", calcularRepasse(totalHospital[hospital]));
    }

    printf("Media geral = %.2f\n", media);

    return 0;
}
```

## 45. String: scanf para palavra e fgets para frase

Palavras-chave: `string`, `scanf`, `fgets`, `frase`

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char palavra[30]; // Guarda palavra sem espaco.
    char frase[100]; // Guarda frase com espaco.

    printf("Digite uma palavra: ");
    scanf("%29s", palavra); // Le ate espaco.

    getchar(); // Consome o enter deixado pelo scanf.

    printf("Digite uma frase: ");
    fgets(frase, sizeof(frase), stdin); // Le frase com espacos.

    frase[strcspn(frase, "\n")] = '\0'; // Remove enter final.

    printf("Palavra = %s\n", palavra);
    printf("Frase = %s\n", frase);

    return 0;
}
```

## 46. String: tamanho, vogais e consoantes

Palavras-chave: `strlen`, `vogais`, `string`, `for`

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char texto[100]; // Texto digitado.
    int vogais = 0; // Contador de vogais.
    int consoantes = 0; // Contador de consoantes.

    printf("Texto: ");
    fgets(texto, sizeof(texto), stdin);
    texto[strcspn(texto, "\n")] = '\0'; // Remove enter.

    for (int i = 0; texto[i] != '\0'; i++) { // Vai ate o fim da string.
        char c = texto[i]; // Caractere atual.

        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            vogais++;
        } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            consoantes++; // E letra, mas nao vogal.
        }
    }

    printf("Tamanho = %lu\n", strlen(texto));
    printf("Vogais = %d\n", vogais);
    printf("Consoantes = %d\n", consoantes);

    return 0;
}
```

## 47. String: comparar senha

Palavras-chave: `strcmp`, `senha`, `comparar string`

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char senha[30]; // Senha digitada.

    printf("Senha: ");
    scanf("%29s", senha);

    if (strcmp(senha, "1234") == 0) { // strcmp retorna 0 quando igual.
        printf("Acesso permitido\n");
    } else {
        printf("Acesso negado\n");
    }

    return 0;
}
```

Erro comum:

```c
// Errado para string:
// if (senha == "1234")

// Certo:
// if (strcmp(senha, "1234") == 0)
```

## 48. String: copiar e concatenar

Palavras-chave: `strcpy`, `strcat`, `concatenar`

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char nome[50]; // Primeiro texto.
    char sobrenome[50]; // Segundo texto.
    char completo[120]; // Resultado.

    printf("Nome: ");
    scanf("%49s", nome);

    printf("Sobrenome: ");
    scanf("%49s", sobrenome);

    strcpy(completo, nome); // Copia nome para completo.
    strcat(completo, " "); // Adiciona espaco.
    strcat(completo, sobrenome); // Adiciona sobrenome.

    printf("Completo = %s\n", completo);

    return 0;
}
```

## 49. String: palindromo simples

Palavras-chave: `palindromo`, `string`, `inicio`, `fim`

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char texto[100]; // Texto.
    int inicio = 0; // Comeco.
    int fim; // Fim.
    int palindromo = 1; // Assume que e palindromo.

    printf("Texto sem espacos: ");
    scanf("%99s", texto);

    fim = strlen(texto) - 1; // Ultimo indice valido.

    while (inicio < fim) {
        if (texto[inicio] != texto[fim]) {
            palindromo = 0; // Encontrou diferenca.
        }

        inicio++; // Anda do comeco para frente.
        fim--; // Anda do fim para tras.
    }

    if (palindromo) {
        printf("Palindromo\n");
    } else {
        printf("Nao palindromo\n");
    }

    return 0;
}
```

## 50. Struct: cadastro de aluno

Palavras-chave: `struct`, `typedef`, `aluno`, `cadastro`

```c
#include <stdio.h>

typedef struct {
    char nome[50]; // Campo texto.
    int idade; // Campo inteiro.
    float media; // Campo real.
} Aluno; // Nome do tipo criado.

int main(void) {
    Aluno a; // Variavel do tipo Aluno.

    printf("Nome: ");
    scanf("%49s", a.nome); // Acessa campo com ponto.

    printf("Idade: ");
    scanf("%d", &a.idade);

    printf("Media: ");
    scanf("%f", &a.media);

    printf("Aluno: %s\n", a.nome);
    printf("Idade: %d\n", a.idade);
    printf("Media: %.2f\n", a.media);

    if (a.media >= 7.0) {
        printf("Aprovado\n");
    } else {
        printf("Reprovado\n");
    }

    return 0;
}
```

## 51. Vetor de struct: turma de alunos

Palavras-chave: `vetor de struct`, `turma`, `listar`

```c
#include <stdio.h>

#define TAM 3

typedef struct {
    char nome[50]; // Nome do aluno.
    float nota1; // Primeira nota.
    float nota2; // Segunda nota.
    float media; // Media calculada.
} Aluno;

int main(void) {
    Aluno turma[TAM]; // Vetor de structs.

    for (int i = 0; i < TAM; i++) {
        printf("Nome do aluno %d: ", i + 1);
        scanf("%49s", turma[i].nome);

        printf("Duas notas: ");
        scanf("%f %f", &turma[i].nota1, &turma[i].nota2);

        turma[i].media = (turma[i].nota1 + turma[i].nota2) / 2.0; // Calcula media.
    }

    printf("Relatorio:\n");
    for (int i = 0; i < TAM; i++) {
        printf("%s - media %.2f - ", turma[i].nome, turma[i].media);

        if (turma[i].media >= 7.0) {
            printf("Aprovado\n");
        } else {
            printf("Reprovado\n");
        }
    }

    return 0;
}
```

## 52. Struct: buscar aluno por nome

Palavras-chave: `buscar struct`, `strcmp`, `nome`

```c
#include <stdio.h>
#include <string.h>

#define TAM 3

typedef struct {
    char nome[50];
    float media;
} Aluno;

int main(void) {
    Aluno turma[TAM]; // Vetor de alunos.
    char busca[50]; // Nome procurado.
    int encontrado = 0; // Flag.

    for (int i = 0; i < TAM; i++) {
        scanf("%49s %f", turma[i].nome, &turma[i].media);
    }

    printf("Nome para buscar: ");
    scanf("%49s", busca);

    for (int i = 0; i < TAM; i++) {
        if (strcmp(turma[i].nome, busca) == 0) { // Compara strings.
            printf("Encontrado: %s %.2f\n", turma[i].nome, turma[i].media);
            encontrado = 1; // Marca que achou.
        }
    }

    if (!encontrado) {
        printf("Aluno nao encontrado\n");
    }

    return 0;
}
```

## 53. Struct: produto, estoque e valor total

Palavras-chave: `produto`, `estoque`, `preco`, `quantidade`, `struct`

```c
#include <stdio.h>

#define TAM 4

typedef struct {
    char nome[50]; // Nome do produto.
    int quantidade; // Quantidade em estoque.
    float preco; // Preco unitario.
} Produto;

int main(void) {
    Produto produtos[TAM]; // Vetor de produtos.
    float totalGeral = 0.0; // Valor total do estoque.

    for (int i = 0; i < TAM; i++) {
        printf("Nome, quantidade e preco: ");
        scanf("%49s %d %f", produtos[i].nome, &produtos[i].quantidade, &produtos[i].preco);
    }

    for (int i = 0; i < TAM; i++) {
        float totalProduto = produtos[i].quantidade * produtos[i].preco; // Total do item.
        totalGeral += totalProduto; // Soma no geral.

        printf("%s: total %.2f\n", produtos[i].nome, totalProduto);

        if (produtos[i].quantidade < 5) {
            printf("Estoque baixo\n");
        }
    }

    printf("Total geral = %.2f\n", totalGeral);

    return 0;
}
```

## 54. Struct: ordenar alunos por media

Palavras-chave: `ordenar struct`, `media`, `troca`

```c
#include <stdio.h>

#define TAM 3

typedef struct {
    char nome[50];
    float media;
} Aluno;

int main(void) {
    Aluno turma[TAM]; // Vetor de alunos.
    Aluno aux; // Auxiliar para trocar structs.

    for (int i = 0; i < TAM; i++) {
        scanf("%49s %f", turma[i].nome, &turma[i].media);
    }

    for (int i = 0; i < TAM - 1; i++) {
        for (int j = 0; j < TAM - 1 - i; j++) {
            if (turma[j].media < turma[j + 1].media) { // Ordem decrescente.
                aux = turma[j]; // Troca struct inteira.
                turma[j] = turma[j + 1];
                turma[j + 1] = aux;
            }
        }
    }

    for (int i = 0; i < TAM; i++) {
        printf("%s %.2f\n", turma[i].nome, turma[i].media);
    }

    return 0;
}
```

## 55. Funcoes: retorno, void e prototipo

Palavras-chave: `funcao`, `return`, `void`, `prototipo`

```c
#include <stdio.h>

float calcularMedia(float a, float b); // Prototipo: avisa que a funcao existe.
void mostrarSituacao(float media); // Funcao sem retorno.

int main(void) {
    float n1, n2, media; // Dados.

    scanf("%f %f", &n1, &n2);

    media = calcularMedia(n1, n2); // Chama funcao que retorna valor.
    mostrarSituacao(media); // Chama funcao que imprime.

    return 0;
}

float calcularMedia(float a, float b) {
    return (a + b) / 2.0; // Retorna resultado para quem chamou.
}

void mostrarSituacao(float media) {
    if (media >= 7.0) {
        printf("Aprovado\n");
    } else {
        printf("Reprovado\n");
    }
}
```

## 56. Ponteiro basico: alterar valor em funcao

Palavras-chave: `ponteiro`, `endereco`, `alterar valor`, `*`, `&`

```c
#include <stdio.h>

void dobrar(int *numero) { // Recebe endereco de um inteiro.
    *numero = *numero * 2; // *numero acessa o valor original.
}

int main(void) {
    int x = 10; // Valor inicial.

    dobrar(&x); // Envia endereco de x.

    printf("x = %d\n", x); // Agora x vale 20.

    return 0;
}
```

Regra curta:

```c
int x = 5; // Variavel normal.
int *p = &x; // Ponteiro guarda endereco de x.
printf("%d\n", *p); // *p acessa o valor de x.
```

## 57. Menu modularizado com funcoes

Palavras-chave: `menu modularizado`, `funcao`, `switch`

```c
#include <stdio.h>

void mostrarMenu(void) {
    printf("\n1 - Somar\n");
    printf("2 - Subtrair\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
}

float somar(float a, float b) {
    return a + b;
}

float subtrair(float a, float b) {
    return a - b;
}

int main(void) {
    int opcao; // Opcao do menu.
    float a, b; // Numeros.

    do {
        mostrarMenu(); // Deixa o main mais limpo.
        scanf("%d", &opcao);

        if (opcao == 1 || opcao == 2) {
            printf("Digite dois numeros: ");
            scanf("%f %f", &a, &b);
        }

        switch (opcao) {
            case 1:
                printf("Resultado = %.2f\n", somar(a, b));
                break;

            case 2:
                printf("Resultado = %.2f\n", subtrair(a, b));
                break;

            case 0:
                printf("Fim.\n");
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
```

## 58. Arquivo em C, se cair

Palavras-chave: `arquivo`, `fopen`, `fprintf`, `fscanf`, `fclose`

Se o professor nao passou arquivos, talvez nao caia. Mas se aparecer, o modelo basico e este:

```c
#include <stdio.h>

int main(void) {
    FILE *arquivo; // Ponteiro para arquivo.
    char nome[50]; // Nome lido.
    float nota; // Nota lida.

    arquivo = fopen("alunos.txt", "w"); // Abre para escrita.

    if (arquivo == NULL) { // Sempre testar se abriu.
        printf("Erro ao abrir arquivo.\n");
        return 1; // Encerra com erro.
    }

    fprintf(arquivo, "Ana 8.5\n"); // Escreve no arquivo.
    fprintf(arquivo, "Bia 7.0\n");

    fclose(arquivo); // Fecha arquivo.

    arquivo = fopen("alunos.txt", "r"); // Abre para leitura.

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return 1;
    }

    while (fscanf(arquivo, "%49s %f", nome, &nota) == 2) { // Le enquanto tiver par nome/nota.
        printf("%s %.2f\n", nome, nota);
    }

    fclose(arquivo);

    return 0;
}
```

## 59. Recursao, se cair

Palavras-chave: `recursao`, `fatorial recursivo`, `funcao chama ela mesma`

```c
#include <stdio.h>

int fatorial(int n) {
    if (n <= 1) { // Caso base: para a recursao.
        return 1;
    }

    return n * fatorial(n - 1); // Chamada recursiva.
}

int main(void) {
    int n;

    scanf("%d", &n);
    printf("%d\n", fatorial(n));

    return 0;
}
```

## 60. Alocacao dinamica, se cair

Palavras-chave: `malloc`, `free`, `stdlib.h`, `alocacao dinamica`

Se nao foi estudado, provavelmente nao cai. Mas o modelo basico e:

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n; // Tamanho definido em tempo de execucao.
    int *v; // Ponteiro que sera usado como vetor.
    int soma = 0;

    printf("Quantidade: ");
    scanf("%d", &n);

    v = malloc(n * sizeof(int)); // Reserva memoria para n inteiros.

    if (v == NULL) { // Verifica se a memoria foi alocada.
        printf("Erro de memoria.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        scanf("%d", &v[i]); // Usa como vetor normal.
        soma += v[i];
    }

    printf("Soma = %d\n", soma);

    free(v); // Libera memoria.

    return 0;
}
```

## 61. Modelos de desafios e como adaptar

### Desafio A: "Leia 10 numeros e mostre a media dos pares"

Padrao:

- Vetor ou leitura direta.
- Contador de pares.
- Soma de pares.
- Dividir apenas se contador > 0.

Codigo base:

```c
int soma = 0; // Soma apenas dos pares.
int contador = 0; // Quantos pares foram encontrados.

for (int i = 0; i < 10; i++) {
    scanf("%d", &numero); // Le um numero por vez.

    if (numero % 2 == 0) {
        soma += numero; // Soma se for par.
        contador++; // Conta se for par.
    }
}

if (contador > 0) {
    printf("%.2f\n", soma / (float)contador); // Media dos pares.
}
```

### Desafio B: "Leia uma matriz e mostre o maior de cada linha"

Padrao:

- Dois `for`.
- Para cada linha, inicializar `maiorLinha = m[i][0]`.
- Comparar colunas da linha.

```c
for (int i = 0; i < LINHAS; i++) {
    int maiorLinha = m[i][0]; // Primeiro da linha.

    for (int j = 0; j < COLUNAS; j++) {
        if (m[i][j] > maiorLinha) {
            maiorLinha = m[i][j]; // Atualiza maior da linha.
        }
    }

    printf("Maior da linha %d = %d\n", i, maiorLinha);
}
```

### Desafio C: "Cadastre produtos e mostre o mais caro"

Padrao:

- Struct Produto.
- Vetor de Produto.
- Maior inicializado com posicao 0.

```c
int posMaior = 0; // Assume que o primeiro e o mais caro.

for (int i = 1; i < TAM; i++) {
    if (produtos[i].preco > produtos[posMaior].preco) {
        posMaior = i; // Guarda a posicao do mais caro.
    }
}

printf("Mais caro: %s %.2f\n", produtos[posMaior].nome, produtos[posMaior].preco);
```

### Desafio D: "Leia nomes e conte quantos comecam com A"

Padrao:

- Vetor de strings ou leitura direta.
- Testar `nome[0]`.

```c
if (nome[0] == 'A' || nome[0] == 'a') {
    contador++; // Nome comeca com A ou a.
}
```

### Desafio E: "Menu de cadastro"

Padrao:

- `do while`.
- `switch`.
- Vetor de struct.
- `quantidade` para controlar quantos foram cadastrados.

```c
if (quantidade < TAM) {
    scanf("%49s", alunos[quantidade].nome); // Cadastra na proxima posicao livre.
    scanf("%f", &alunos[quantidade].media);
    quantidade++; // Aumenta quantidade cadastrada.
} else {
    printf("Cadastro cheio.\n");
}
```

## 62. Erros comuns que tiram ponto

- Esquecer `&` no `scanf` de `int`, `float`, `double`, `char`.
- Colocar `&` em string com `scanf("%s", nome)`.
- Usar `=` no lugar de `==`.
- Esquecer `break` no `switch`.
- Usar indice errado: vetor de tamanho 5 vai de `0` ate `4`.
- Esquecer de inicializar `soma = 0` e `contador = 0`.
- Dividir por zero quando contador e zero.
- Comparar string com `==` em vez de `strcmp`.
- Usar `fgets` depois de `scanf` sem consumir o enter.
- Passar matriz para funcao sem informar colunas.
- Usar `%f` para `double` no `scanf`; para `double`, use `%lf`.
- Usar `int` quando o resultado precisa de casas decimais.

## 63. Checklist rapido antes de entregar

Antes de entregar, confira:

- O programa tem `#include <stdio.h>`?
- Se usa `sqrt` ou `pow`, tem `#include <math.h>`?
- O `main` termina com `return 0;`?
- Toda variavel foi declarada?
- Todo `scanf` esta com formato correto?
- Vetor/matriz nao passa do limite?
- Soma/contador foram inicializados?
- Funcoes foram declaradas antes do `main` ou tem prototipo?
- O enunciado pediu funcao? Se sim, separou em funcao?
- O enunciado pediu retornar para o main? Se sim, a funcao tem `return`.
- O enunciado pediu imprimir no main? Se sim, nao imprima tudo dentro da funcao.
- Strings foram comparadas com `strcmp`?
- O codigo esta indentado?

## 64. Cola mental de estruturas

```c
// Acumulador:
soma += valor;

// Contador:
contador++;

// Media:
media = soma / (float)quantidade;

// Maior:
maior = v[0];
if (v[i] > maior) maior = v[i];

// Menor:
menor = v[0];
if (v[i] < menor) menor = v[i];

// Busca:
if (v[i] == procurado) encontrado = 1;

// String igual:
if (strcmp(a, b) == 0) printf("iguais");

// Matriz:
for (int i = 0; i < LINHAS; i++) {
    for (int j = 0; j < COLUNAS; j++) {
        // usa m[i][j]
    }
}
```

## 65. Como transformar enunciado em codigo

Exemplo de enunciado:

"Leia as notas de 5 alunos, calcule a media da turma e mostre quantos ficaram acima da media."

Traducao:

- "notas de 5 alunos" -> vetor `notas[5]`.
- "media da turma" -> soma todos e divide por 5.
- "quantos acima da media" -> contador com `if (notas[i] > media)`.

Codigo:

```c
#include <stdio.h>

#define TAM 5

int main(void) {
    float notas[TAM]; // Guarda notas.
    float soma = 0.0; // Soma das notas.
    float media; // Media da turma.
    int acima = 0; // Quantidade acima da media.

    for (int i = 0; i < TAM; i++) {
        scanf("%f", &notas[i]); // Le nota.
        soma += notas[i]; // Soma enquanto le.
    }

    media = soma / TAM; // Calcula media.

    for (int i = 0; i < TAM; i++) {
        if (notas[i] > media) {
            acima++; // Conta acima da media.
        }
    }

    printf("Media = %.2f\n", media);
    printf("Acima da media = %d\n", acima);

    return 0;
}
```

## 66. Mini banco de variacoes de prova

Use estes nomes no `Ctrl+F`:

- `media simples`: notas, temperaturas, idades, salarios.
- `media condicional`: media dos pares, media dos positivos, media dos aprovados.
- `maior menor`: maior nota, menor preco, hospital com mais pacientes.
- `contador`: quantidade de aprovados, pares, vogais, produtos abaixo do estoque.
- `porcentagem`: percentual de aprovados, desconto, aumento, comissao.
- `vetor auxiliar`: total por mes, separar pares, guardar medias.
- `matriz linha`: total por mes, total por aluno, total por linha.
- `matriz coluna`: total por hospital, total por produto, total por coluna.
- `struct cadastro`: aluno, produto, paciente, funcionario.
- `string`: nome, senha, frase, buscar por nome.
- `funcao`: ler, calcular, imprimir, retornar resultado.

## 67. Quando usar cada coisa

- Variavel simples: quando tem um unico valor.
- Vetor: varios valores do mesmo tipo, uma dimensao.
- Matriz: tabela, linhas e colunas.
- String: texto.
- Struct: registro com campos diferentes.
- Vetor de struct: varios registros.
- Funcao: quando precisa separar uma tarefa ou o enunciado exige modularizacao.
- Ponteiro: quando uma funcao precisa alterar uma variavel simples do `main`.



#############################################################
# PARTE 2 - GUIA UNICO RESUMIDO
#############################################################

# Guia unico de consulta - Prova de C

Use este material como consulta somente se a prova permitir consulta. Ele foi feito com base no PDF da revisao do GA e nos topicos citados: vetor, matriz, struct, string, funcoes/modulos, menus e logica em C.

Arquivos complementares importantes:

- `BANCO_DESAFIOS_GABARITOS_PALAVRAS_CHAVE.md`: varios tipos de desafio com gabarito e palavras-chave para Ctrl+F.
- `PALAVRAS_CHAVE_BUSCA.md`: lista curta de termos para procurar rapido.
- `FORMULAS_CALCULOS_PROVA_C.md`: formulas e modelos de calculo matematico que costumam aparecer em prova.

## 1. Estrutura basica de um programa

```c
#include <stdio.h>

int main() {
    // comandos aqui
    return 0;
}
```

Partes importantes:

- `#include <stdio.h>`: libera `printf` e `scanf`.
- `int main()`: ponto inicial do programa.
- `{ }`: delimitam bloco de codigo.
- `;`: quase toda instrucao termina com ponto e virgula.
- `return 0;`: indica fim correto do programa.

## 2. Tipos principais

```c
int idade = 20;          // inteiro
float nota = 8.5;        // real simples
double media = 7.85;     // real com maior precisao
char letra = 'A';        // um caractere
char nome[50];           // string, vetor de char
```

Formatadores mais usados:

| Tipo | scanf/printf |
|---|---|
| int | `%d` |
| float | `%f` |
| double no scanf | `%lf` |
| double no printf | `%f` |
| char | `%c` |
| string | `%s` |

Exemplo:

```c
int x;
float nota;

printf("Digite um inteiro: ");
scanf("%d", &x);

printf("Digite uma nota: ");
scanf("%f", &nota);

printf("x = %d, nota = %.2f\n", x, nota);
```

Regra de ouro do `scanf`: para variavel simples use `&`. Para string com vetor de char, normalmente nao usa `&`.

```c
int idade;
char nome[50];

scanf("%d", &idade);
scanf("%49s", nome);
```

## 3. Operadores

| Operador | Uso |
|---|---|
| `+` | soma |
| `-` | subtracao |
| `*` | multiplicacao |
| `/` | divisao |
| `%` | resto da divisao inteira |
| `==` | igual |
| `!=` | diferente |
| `>` `<` `>=` `<=` | comparacoes |
| `&&` | E logico |
| `||` | OU logico |
| `!` | NAO logico |

Par ou impar:

```c
if (numero % 2 == 0) {
    printf("Par\n");
} else {
    printf("Impar\n");
}
```

## 4. Condicionais

### if / else

```c
if (media >= 7.0) {
    printf("Aprovado\n");
} else if (media >= 4.0) {
    printf("Recuperacao\n");
} else {
    printf("Reprovado\n");
}
```

### switch / case

Use quando uma variavel inteira ou char define uma opcao.

```c
switch (opcao) {
    case 1:
        printf("Somar\n");
        break;
    case 2:
        printf("Subtrair\n");
        break;
    default:
        printf("Opcao invalida\n");
}
```

Sem `break`, o programa continua executando os proximos `case`.

## 5. Lacos

### for

Bom quando sabe a quantidade de repeticoes.

```c
for (int i = 0; i < 5; i++) {
    printf("%d\n", i);
}
```

### while

Bom quando repete enquanto uma condicao for verdadeira.

```c
int senha = 0;
while (senha != 123) {
    printf("Senha: ");
    scanf("%d", &senha);
}
```

### do while

Executa pelo menos uma vez. Muito usado em menu.

```c
do {
    printf("1 - Continuar\n");
    printf("0 - Sair\n");
    scanf("%d", &opcao);
} while (opcao != 0);
```

## 6. Vetores

Vetor guarda varios valores do mesmo tipo.

```c
int notas[5];
```

Indices sempre comecam em zero:

```c
notas[0] = 10;
notas[1] = 8;
```

Leitura:

```c
for (int i = 0; i < 5; i++) {
    printf("Valor %d: ", i);
    scanf("%d", &v[i]);
}
```

Soma:

```c
int soma = 0;
for (int i = 0; i < 5; i++) {
    soma += v[i];
}
```

Maior valor:

```c
int maior = v[0];
for (int i = 1; i < 5; i++) {
    if (v[i] > maior) {
        maior = v[i];
    }
}
```

Media:

```c
float media = soma / 5.0;
```

## 7. Matrizes

Matriz e vetor com duas dimensoes: linha e coluna.

```c
int matriz[6][5];
```

No desafio do PDF:

- 6 linhas = meses
- 5 colunas = hospitais
- `matriz[mes][hospital]` = internados naquele mes naquele hospital

Leitura de matriz:

```c
for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 5; j++) {
        printf("Mes %d, Hospital %d: ", i + 1, j);
        scanf("%d", &matriz[i][j]);
    }
}
```

Soma por linha, total de cada mes:

```c
for (int i = 0; i < 6; i++) {
    internados[i] = 0;
    for (int j = 0; j < 5; j++) {
        internados[i] += matriz[i][j];
    }
}
```

Soma por coluna, total de cada hospital:

```c
for (int j = 0; j < 5; j++) {
    pacientes[j] = 0;
    for (int i = 0; i < 6; i++) {
        pacientes[j] += matriz[i][j];
    }
}
```

Soma total da matriz:

```c
int soma = 0;
for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 5; j++) {
        soma += matriz[i][j];
    }
}
```

Media da matriz:

```c
float media = soma / 30.0; // 6 * 5 = 30 elementos
```

## 8. Funcoes e modularizacao

Funcao evita repetir codigo e deixa o programa organizado.

Formato:

```c
tipo_retorno nome_funcao(parametros) {
    comandos;
    return valor;
}
```

Exemplo:

```c
int somar(int a, int b) {
    return a + b;
}
```

Chamada:

```c
int resultado = somar(3, 4);
```

Funcao sem retorno usa `void`:

```c
void mostrarMensagem() {
    printf("Ola\n");
}
```

### Vetor como parametro

```c
void imprimirVetor(int v[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", v[i]);
    }
}
```

### Matriz como parametro

Em C, ao passar matriz, a segunda dimensao precisa aparecer:

```c
#define LINHAS 6
#define COLUNAS 5

void imprimirMatriz(int m[LINHAS][COLUNAS]) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}
```

## 9. Strings

String em C e vetor de `char` terminado por `\0`.

```c
char nome[50];
```

Ler uma palavra:

```c
scanf("%49s", nome);
```

Ler frase com espacos:

```c
fgets(nome, 50, stdin);
```

Biblioteca:

```c
#include <string.h>
```

Funcoes uteis:

```c
strlen(nome);           // tamanho
strcpy(destino, origem); // copia
strcmp(a, b);           // compara, 0 significa igual
strcat(a, b);           // concatena
```

Comparar strings:

```c
if (strcmp(nome, "Ana") == 0) {
    printf("Nome igual\n");
}
```

Remover `\n` do `fgets`:

```c
nome[strcspn(nome, "\n")] = '\0';
```

## 10. Struct

`struct` agrupa dados diferentes em uma mesma variavel.

```c
typedef struct {
    char nome[50];
    int idade;
    float media;
} Aluno;
```

Usar:

```c
Aluno a;
scanf("%49s", a.nome);
scanf("%d", &a.idade);
scanf("%f", &a.media);
```

Vetor de struct:

```c
Aluno turma[30];

for (int i = 0; i < 30; i++) {
    scanf("%49s", turma[i].nome);
    scanf("%f", &turma[i].media);
}
```

Struct + string + vetor costuma cair em prova porque mistura varios assuntos.

## 11. Padroes que podem aparecer em prova

### 11.1 Menu completo

```c
int opcao;
do {
    printf("\n1 - Cadastrar\n");
    printf("2 - Listar\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Cadastrar\n");
            break;
        case 2:
            printf("Listar\n");
            break;
        case 0:
            printf("Saindo\n");
            break;
        default:
            printf("Opcao invalida\n");
    }
} while (opcao != 0);
```

### 11.2 Buscar em vetor

```c
int pos = -1;
for (int i = 0; i < n; i++) {
    if (v[i] == procurado) {
        pos = i;
        break;
    }
}
```

### 11.3 Ordenar vetor simples

```c
for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
        if (v[j] < v[i]) {
            int aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }
    }
}
```

### 11.4 Contar ocorrencias

```c
int cont = 0;
for (int i = 0; i < n; i++) {
    if (v[i] > 10) {
        cont++;
    }
}
```

### 11.5 Acumulador com regra

```c
float total = 0;
if (quantidade <= 50) {
    total = quantidade * 1000.0;
} else if (quantidade <= 100) {
    total = quantidade * 1200.0;
} else {
    total = quantidade * 1500.0;
}
```

## 12. Desafio do PDF: dengue, matriz e vetores

Resumo:

- Matriz `6 x 5` no `main`.
- Ler a matriz em uma funcao.
- Vetor `Internados[6]`: total de cada mes.
- Vetor `Pacientes[5]`: total de cada hospital.
- Calcular valor em reais por hospital:
  - ate 50 pacientes: R$ 1000 por paciente
  - acima de 50 ate 100: R$ 1200 por paciente
  - acima de 100: R$ 1500 por paciente
- Calcular media de toda a matriz em uma funcao, retornar para o `main` e imprimir no `main`.

Ideia principal:

```c
// total por mes = soma das linhas
for (int mes = 0; mes < 6; mes++) {
    internados[mes] = 0;
    for (int hospital = 0; hospital < 5; hospital++) {
        internados[mes] += matriz[mes][hospital];
    }
}

// total por hospital = soma das colunas
for (int hospital = 0; hospital < 5; hospital++) {
    pacientes[hospital] = 0;
    for (int mes = 0; mes < 6; mes++) {
        pacientes[hospital] += matriz[mes][hospital];
    }
}
```

O codigo completo esta em:

`gabaritos/desafio_dengue_pdf_completo.c`

## 13. Erros comuns

1. Esquecer `;`.
2. Usar `=` no lugar de `==`.
3. Acessar vetor fora do limite, exemplo `v[5]` em vetor de tamanho 5.
4. Esquecer `&` no `scanf` de int/float.
5. Comparar string com `==` em vez de `strcmp`.
6. Esquecer `break` no `switch`.
7. Dividir inteiros e perder decimal: use `3.0`, `5.0`, `(float)soma`.
8. Nao inicializar soma/contador com zero.
9. Passar matriz para funcao sem informar a quantidade de colunas.
10. Misturar `scanf` e `fgets` sem limpar o buffer.

## 14. Como compilar

No Windows com GCC instalado:

```sh
gcc arquivo.c -o programa.exe
programa.exe
```

No Linux/WSL:

```sh
gcc arquivo.c -o programa
./programa
```

Com avisos:

```sh
gcc -Wall -Wextra arquivo.c -o programa
```

## 15. Checklist mental antes de entregar

- O programa tem `#include` correto?
- Tem `main`?
- Toda variavel foi declarada?
- Toda soma/contador comeca em zero?
- Vetores e matrizes usam indices corretos?
- Funcoes foram declaradas antes do `main` ou possuem prototipo?
- `scanf` usa `&` quando precisa?
- Strings foram comparadas com `strcmp`?
- O codigo compila?
- A saida responde exatamente o que o enunciado pediu?
