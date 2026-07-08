# 03 - EXERCICIOS E DESAFIOS COM GABARITO

Banco de exercicios com enunciado + gabarito de codigo comentado,
do simples ao complexo, cobrindo todos os tipos de questao pratica.

INDICE DESTE ARQUIVO:
  PARTE 1 - Banco de exercicios GB (34 exercicios em 12 categorias)
  PARTE 2 - Banco de desafios e gabaritos (GA)


#############################################################
# PARTE 1 - BANCO DE EXERCICIOS GB
#############################################################

# BANCO DE EXERCICIOS - PROVA GB DE C

Banco de exercicios com enunciado + gabarito de codigo comentado, do simples
ao complexo, cobrindo TODOS os tipos de questao pratica que podem cair no GB.

Use Ctrl+F por categoria ou por palavra-chave.

=========================================================
PALAVRAS-CHAVE PARA CTRL+F (por categoria)
=========================================================

Basico: troco, IMC, menu, switch, do while, operacoes
Vetor: acima da media, pares impares, segundo maior, INT_MIN
Matriz: vendas, total por produto, total por mes, maior matriz, matriz identidade, diagonal
String: vogais, consoantes, palindromo, strtok, CSV, atoi, atof
Struct: cadastro alunos, media situacao, produto, estoque baixo, mais caro, ponteiro struct, ->
Arquivo texto: fprintf, nomes.txt, fgets, contar linhas, CSV, strtok
Arquivo binario: fwrite, fread, func.dat, while fread, fseek, SEEK_SET, acesso direto, sizeof
Struct + arquivo: CRUD, criar, cadastrar, listar, alterar, excluir, temp, rename, duplicidade
Funcoes: calculadora modularizada, leitura impressao media, retorna struct
Recursao: fatorial recursivo, fibonacci recursivo, caso base, printf ordem, decrescente, crescente
Ponteiros: troca, dobrar, malloc, ->, free
Desafios: notas escolar, estoque, valor total, menu cadastro, clientes.dat

=========================================================
INDICE
=========================================================
1. Basico (entrada/saida, if/else, switch, loops)
2. Vetor
3. Matriz
4. String
5. Struct (basico e avancado)
6. Arquivo texto
7. Arquivo binario
8. Struct + arquivo (combinacao critica)
9. Funcoes / Modularizacao
10. Recursao
11. Ponteiros
12. Desafios complexos (integracao)

=========================================================

## [BASICO] Calcular troco
Palavras-chave: `troco`, `if`, `else`, `scanf`, `printf`, `float`
**Enunciado:** Ler o preco de um produto e o valor pago pelo cliente. Calcular o troco. Se o valor pago for menor que o preco, mostrar quanto falta. Se for maior, mostrar o troco (quanto sobra). Se for igual, informar que nao precisa de troco.
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>   // getch, system pause (estilo do professor)
#include <stdlib.h>  // system

int main() {
    float preco, pago, troco;

    // Cabecalho do programa
    system("cls");  // limpa a tela no Windows
    printf("=== CALCULO DE TROCO ===\n");

    // Entrada: preco do produto
    printf("Digite o preco do produto: ");
    scanf("%f", &preco);

    // Entrada: valor pago pelo cliente
    printf("Digite o valor pago: ");
    scanf("%f", &pago);

    // Logica: compara o que foi pago com o preco
    if (pago > preco) {
        // Cliente pagou a mais -> existe troco
        troco = pago - preco;
        printf("Troco a devolver: R$ %.2f\n", troco);
    } else if (pago < preco) {
        // Cliente pagou a menos -> ainda falta dinheiro
        troco = preco - pago;
        printf("Falta pagar: R$ %.2f\n", troco);
    } else {
        // Valores iguais -> nao ha troco
        printf("Valor exato, nao precisa de troco.\n");
    }

    system("pause");  // pausa para o usuario ver o resultado
    return 0;
}
```
**Explicacao:** Compara `pago` com `preco` usando if/else encadeado para cobrir os tres casos (sobra, falta, igual). O troco e sempre a diferenca absoluta entre os dois valores.

## [BASICO] Classificar IMC
Palavras-chave: `imc`, `if`, `else if`, `faixas`, `peso`, `altura`, `float`
**Enunciado:** Ler o peso (kg) e a altura (m) de uma pessoa. Calcular o IMC = peso / (altura * altura) e classificar: abaixo do peso (IMC < 18.5), peso normal (18.5 a 24.9), sobrepeso (25 a 29.9), obesidade (>= 30).
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main() {
    float peso, altura, imc;

    system("cls");
    printf("=== CLASSIFICACAO DE IMC ===\n");

    // Entrada de dados
    printf("Digite o peso (kg): ");
    scanf("%f", &peso);

    printf("Digite a altura (m): ");
    scanf("%f", &altura);

    // Calculo do IMC: peso dividido pela altura ao quadrado
    imc = peso / (altura * altura);

    // Mostra o valor calculado com 2 casas decimais
    printf("IMC = %.2f\n", imc);

    // Classificacao por faixas (else if em cadeia)
    if (imc < 18.5) {
        printf("Classificacao: Abaixo do peso\n");
    } else if (imc < 25.0) {
        // Se chegou aqui, imc >= 18.5 e < 25
        printf("Classificacao: Peso normal\n");
    } else if (imc < 30.0) {
        // imc >= 25 e < 30
        printf("Classificacao: Sobrepeso\n");
    } else {
        // imc >= 30
        printf("Classificacao: Obesidade\n");
    }

    system("pause");
    return 0;
}
```
**Explicacao:** O IMC e peso dividido pela altura ao quadrado. As faixas sao testadas em ordem crescente com else if, pois cada condicao ja exclui as anteriores.

## [BASICO] Menu de operacoes com switch
Palavras-chave: `switch`, `case`, `menu`, `do while`, `soma`, `subtracao`, `multiplicacao`, `divisao`, `break`
**Enunciado:** Montar um menu com as opcoes: 1-soma, 2-subtracao, 3-multiplicacao, 4-divisao, 5-sair. O usuario escolhe a opcao, digita dois numeros e o programa mostra o resultado. Repetir ate escolher sair. Na divisao, validar se o segundo numero e zero.
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main() {
    int opcao;
    float n1, n2, resultado;

    // Loop principal: repete o menu ate o usuario escolher sair (opcao 5)
    do {
        system("cls");
        printf("=== MENU DE OPERACOES ===\n");
        printf("1 - Soma\n");
        printf("2 - Subtracao\n");
        printf("3 - Multiplicacao\n");
        printf("4 - Divisao\n");
        printf("5 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // So pede os numeros se a opcao for uma operacao valida (1 a 4)
        if (opcao >= 1 && opcao <= 4) {
            printf("Digite o primeiro numero: ");
            scanf("%f", &n1);
            printf("Digite o segundo numero: ");
            scanf("%f", &n2);
        }

        // switch decide qual operacao executar conforme a opcao
        switch (opcao) {
            case 1:
                resultado = n1 + n2;
                printf("Soma = %.2f\n", resultado);
                break;
            case 2:
                resultado = n1 - n2;
                printf("Subtracao = %.2f\n", resultado);
                break;
            case 3:
                resultado = n1 * n2;
                printf("Multiplicacao = %.2f\n", resultado);
                break;
            case 4:
                // Validacao: nao pode dividir por zero
                if (n2 == 0) {
                    printf("Erro: divisao por zero!\n");
                } else {
                    resultado = n1 / n2;
                    printf("Divisao = %.2f\n", resultado);
                }
                break;
            case 5:
                printf("Saindo do programa...\n");
                break;
            default:
                // Opcao fora do intervalo 1..5
                printf("Opcao invalida!\n");
        }

        // Pausa antes de voltar ao menu (so se nao for sair)
        if (opcao != 5) {
            system("pause");
        }
    } while (opcao != 5);  // repete enquanto nao escolher sair

    return 0;
}
```
**Explicacao:** O do-while garante que o menu apareca pelo menos uma vez e repita ate a opcao 5. O switch trata cada operacao, com validacao especial para divisao por zero e default para opcoes invalidas.

## [VETOR] Ler N notas e mostrar quantas estao acima da media
Palavras-chave: `vetor`, `notas`, `media`, `acima`, `for`, `contador`, `float`
**Enunciado:** Ler um valor N (quantidade de notas), depois ler N notas em um vetor. Calcular a media das notas e mostrar quantas notas estao acima (estritamente maiores) que a media calculada.
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main() {
    int n, i, acima = 0;  // acima conta quantas notas superam a media
    float notas[100], soma = 0, media;

    system("cls");
    printf("=== NOTAS ACIMA DA MEDIA ===\n");

    // Le a quantidade de notas (limitada a 100 para nao estourar o vetor)
    printf("Quantas notas (max 100)? ");
    scanf("%d", &n);

    // Valiacao simples do tamanho
    if (n <= 0 || n > 100) {
        printf("Quantidade invalida!\n");
        system("pause");
        return 1;
    }

    // Le as notas e acumula a soma para calcular a media
    for (i = 0; i < n; i++) {
        printf("Nota %d: ", i + 1);
        scanf("%f", &notas[i]);
        soma = soma + notas[i];  // acumulador
    }

    // Calcula a media
    media = soma / n;
    printf("Media da turma: %.2f\n", media);

    // Segundo laco: conta quantas notas estao acima da media
    for (i = 0; i < n; i++) {
        if (notas[i] > media) {
            acima++;  // incrementa o contador
        }
    }

    printf("Quantidade acima da media: %d\n", acima);

    system("pause");
    return 0;
}
```
**Explicacao:** Precisa de dois lacos: o primeiro le as notas e acumula a soma para calcular a media; o segundo percorre o vetor novamente contando quantas notas sao maiores que a media. So e possivel contar acima da media apos conhecer a media, por isso os lacos sao separados.

## [VETOR] Contar pares e impares em um vetor de 10 inteiros
Palavras-chave: `vetor`, `par`, `impar`, `modulo`, `for`, `contador`, `int`
**Enunciado:** Ler 10 numeros inteiros em um vetor. Contar quantos sao pares e quantos sao impares. Mostrar os dois totais.
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main() {
    int vet[10];        // vetor fixo de 10 posicoes
    int i;
    int pares = 0, impares = 0;  // contadores zerados

    system("cls");
    printf("=== PARES E IMPARES ===\n");

    // Le os 10 inteiros
    for (i = 0; i < 10; i++) {
        printf("Digite o numero %d: ", i + 1);
        scanf("%d", &vet[i]);
    }

    // Percorre o vetor contando pares e impares
    for (i = 0; i < 10; i++) {
        // Um numero e par se o resto da divisao por 2 e zero
        if (vet[i] % 2 == 0) {
            pares++;
        } else {
            impares++;
        }
    }

    // Saida dos resultados
    printf("\nResultados:\n");
    printf("Pares: %d\n", pares);
    printf("Impares: %d\n", impares);

    system("pause");
    return 0;
}
```
**Explicacao:** O operador `%` (modulo) da o resto da divisao por 2: se for zero o numero e par, senao e impar. Dois contadores separados acumulam cada categoria em um unico laco.

## [VETOR] Achar o segundo maior valor de um vetor
Palavras-chave: `vetor`, `segundo maior`, `maior`, `for`, `if`, `int`
**Enunciado:** Ler N numeros inteiros em um vetor (N informado pelo usuario). Encontrar e mostrar o segundo maior valor. Considere que pode haver valores repetidos.
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <limits.h>  // para INT_MIN

int main() {
    int n, i;
    int vet[100];
    int maior, segMaior;

    system("cls");
    printf("=== SEGUNDO MAIOR ===\n");

    printf("Quantos numeros (max 100)? ");
    scanf("%d", &n);

    if (n < 2 || n > 100) {
        printf("Precisa de pelo menos 2 numeros!\n");
        system("pause");
        return 1;
    }

    // Le os valores
    for (i = 0; i < n; i++) {
        printf("Numero %d: ", i + 1);
        scanf("%d", &vet[i]);
    }

    // Inicializa maior e segundo maior com o menor inteiro possivel
    // Assim qualquer valor lido sera maior que eles na primeira comparacao
    maior = INT_MIN;
    segMaior = INT_MIN;

    // Percorre o vetor atualizando maior e segundo maior
    for (i = 0; i < n; i++) {
        if (vet[i] > maior) {
            // Novo maior encontrado: o maior antigo vira o segundo maior
            segMaior = maior;
            maior = vet[i];
        } else if (vet[i] > segMaior && vet[i] != maior) {
            // Nao e maior que o maior, mas e maior que o segundo maior atual
            // (e diferente do maior para nao repetir o mesmo valor)
            segMaior = vet[i];
        }
    }

    // Caso todos os valores sejam iguais, segMaior continua INT_MIN
    if (segMaior == INT_MIN) {
        printf("Nao existe segundo maior (todos iguais).\n");
    } else {
        printf("Maior: %d\n", maior);
        printf("Segundo maior: %d\n", segMaior);
    }

    system("pause");
    return 0;
}
```
**Explicacao:** Mantem duas variaveis (maior e segMaior) atualizadas a cada elemento: quando acha um novo maior, o maior antigo desce para segMaior. A condicao `vet[i] != maior` evita que valores repetidos ocupem as duas posicoes.

## [MATRIZ] Matriz de vendas (4 produtos x 6 meses), total por produto e por mes
Palavras-chave: `matriz`, `vendas`, `total`, `linha`, `coluna`, `soma`, `for`, `int`
**Enunciado:** Ler uma matriz 4x6 (4 produtos, 6 meses) com as vendas de cada produto em cada mes. Calcular e mostrar: (a) o total de vendas de cada produto (soma de cada linha) e (b) o total de vendas de cada mes (soma de cada coluna).
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define PRODUTOS 4   // linhas
#define MESES 6      // colunas

int main() {
    int vendas[PRODUTOS][MESES];  // matriz 4x6
    int i, j, soma;

    system("cls");
    printf("=== MATRIZ DE VENDAS ===\n");

    // Leitura da matriz: laco externo percorre linhas, interno percorre colunas
    for (i = 0; i < PRODUTOS; i++) {
        printf("\nProduto %d:\n", i + 1);
        for (j = 0; j < MESES; j++) {
            printf("  Vendas no mes %d: ", j + 1);
            scanf("%d", &vendas[i][j]);
        }
    }

    // (a) Total por produto = soma de cada LINHA
    printf("\n--- Total por produto ---\n");
    for (i = 0; i < PRODUTOS; i++) {
        soma = 0;  // zera a soma antes de cada linha
        for (j = 0; j < MESES; j++) {
            soma = soma + vendas[i][j];  // acumula a linha inteira
        }
        printf("Produto %d: %d vendas\n", i + 1, soma);
    }

    // (b) Total por mes = soma de cada COLUNA
    // Atencao: aqui inverte-se a ordem dos lacos (j externo, i interno)
    printf("\n--- Total por mes ---\n");
    for (j = 0; j < MESES; j++) {
        soma = 0;  // zera a soma antes de cada coluna
        for (i = 0; i < PRODUTOS; i++) {
            soma = soma + vendas[i][j];  // acumula a coluna inteira
        }
        printf("Mes %d: %d vendas\n", j + 1, soma);
    }

    system("pause");
    return 0;
}
```
**Explicacao:** Soma por linha (produto fixo, varia mes) da o total de cada produto; soma por coluna (mes fixo, varia produto) da o total de cada mes. A chave e inverter qual indice fica no laco externo: linha externa soma linhas, coluna externa soma colunas.

## [MATRIZ] Produto mais vendido (maior elemento da matriz com posicao)
Palavras-chave: `matriz`, `maior`, `posicao`, `linha`, `coluna`, `for`, `if`
**Enunciado:** Ler uma matriz 4x6 de vendas. Encontrar o maior valor (produto mais vendido em um mes) e mostrar seu valor junto com a posicao (linha e coluna) em que ele aparece.
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define PRODUTOS 4
#define MESES 6

int main() {
    int vendas[PRODUTOS][MESES];
    int i, j;
    int maior, linMaior, colMaior;  // guardam valor e posicao do maior

    system("cls");
    printf("=== PRODUTO MAIS VENDIDO ===\n");

    // Leitura da matriz
    for (i = 0; i < PRODUTOS; i++) {
        for (j = 0; j < MESES; j++) {
            printf("Vendas do produto %d no mes %d: ", i + 1, j + 1);
            scanf("%d", &vendas[i][j]);
        }
    }

    // Inicializa o maior com o primeiro elemento da matriz
    // (evita usar valores magicos e garante comparacao correta)
    maior = vendas[0][0];
    linMaior = 0;
    colMaior = 0;

    // Percorre toda a matriz procurando um valor maior que o atual
    for (i = 0; i < PRODUTOS; i++) {
        for (j = 0; j < MESES; j++) {
            if (vendas[i][j] > maior) {
                maior = vendas[i][j];   // atualiza o maior valor
                linMaior = i;            // guarda a linha
                colMaior = j;            // guarda a coluna
            }
        }
    }

    // Saida: valor e posicao (somando 1 para mostrar 1-based ao usuario)
    printf("\nMaior venda: %d\n", maior);
    printf("Posicao: produto %d, mes %d\n", linMaior + 1, colMaior + 1);

    system("pause");
    return 0;
}
```
**Explicacao:** Inicializa o maior com o primeiro elemento (nao com zero, para funcionar mesmo com todos negativos) e percorre a matriz guardando valor e indices sempre que acha um novo maior. Os indices sao exibidos somando 1 para o usuario ver posicoes de 1 a N.

## [MATRIZ] Matriz identidade 4x4
Palavras-chave: `matriz`, `identidade`, `diagonal`, `for`, `if`, `1`, `0`
**Enunciado:** Gerar e mostrar uma matriz identidade 4x4, ou seja, com 1 na diagonal principal (onde linha == coluna) e 0 nas demais posicoes.
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define TAM 4  // matriz 4x4

int main() {
    int mat[TAM][TAM];
    int i, j;

    system("cls");
    printf("=== MATRIZ IDENTIDADE 4x4 ===\n");

    // Preenchimento: usa a regra da diagonal principal
    // Na diagonal principal, o indice da linha e igual ao da coluna (i == j)
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            if (i == j) {
                mat[i][j] = 1;  // esta na diagonal principal
            } else {
                mat[i][j] = 0;  // fora da diagonal principal
            }
        }
    }

    // Impressao da matriz com formatacao alinhada
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            printf("%d ", mat[i][j]);  // espaco apos cada elemento
        }
        printf("\n");  // quebra de linha ao fim de cada linha da matriz
    }

    system("pause");
    return 0;
}
```
**Explicacao:** A regra da matriz identidade e simples: se o indice da linha e igual ao da coluna (i == j) coloca 1, senao coloca 0. A diagonal principal de uma matriz quadrada sempre satisfaz i == j.

## [STRING] Contar vogais e consoantes de uma frase (fgets)
Palavras-chave: `string`, `fgets`, `vogal`, `consoante`, `for`, `if`, `strcspn`, `char`
**Enunciado:** Ler uma frase (com espacos) usando fgets. Contar quantas vogais e quantas consoantes a frase possui. Desconsiderar acentos e tratar letras maiusculas e minusculas.
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>   // strcspn
#include <ctype.h>    // tolower

int main() {
    char frase[200];
    int i, tam;
    int vogais = 0, consoantes = 0;

    system("cls");
    printf("=== VOGAIS E CONSOANTES ===\n");

    // Le a frase com fgets (aceita espacos)
    // fflush(stdin) limpa o buffer do teclado antes do fgets.
    // Observacao: fflush(stdin) nao e portavel (comportamento indefinido
    // fora do Windows), mas funciona no Windows com o gcc/MinGW.
    fflush(stdin);
    printf("Digite uma frase: ");
    fgets(frase, sizeof(frase), stdin);

    // fgets guarda o '\n' no final da string; removemos trocando por '\0'
    frase[strcspn(frase, "\n")] = '\0';

    tam = strlen(frase);

    // Percorre cada caractere da string
    for (i = 0; i < tam; i++) {
        // Converte para minusculo para facilitar a comparacao
        char c = tolower(frase[i]);

        // Verifica se e uma letra do alfabeto
        if (c >= 'a' && c <= 'z') {
            // Se for vogal, conta como vogal
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vogais++;
            } else {
                // E letra mas nao e vogal -> consoante
                consoantes++;
            }
        }
        // Caracteres que nao sao letras (espaco, pontuacao, numeros) sao ignorados
    }

    printf("Vogais: %d\n", vogais);
    printf("Consoantes: %d\n", consoantes);

    system("pause");
    return 0;
}
```
**Explicacao:** fgets le a frase inteira (com espacos) e strcspn remove o `\n` final. Converte cada caractere para minusculo com tolower e so conta vogal/consoante se for letra (`a`-`z`), ignorando espacos e pontuacao.

## [STRING] Verificar se uma palavra e palindromo
Palavras-chave: `string`, `palindromo`, `for`, `if`, `strlen`, `char`, `comparacao`
**Enunciado:** Ler uma palavra (sem espacos) e verificar se ela e um palindromo, ou seja, se le-se igual de tras para frente. Exemplos: "ovo", "arara", "radar". Mostrar "E palindromo" ou "Nao e palindromo".
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>  // strlen
#include <ctype.h>   // tolower

int main() {
    char palavra[100];
    int i, j, tam, ehPalindromo = 1;  // 1 = verdadeiro (assume que e)

    system("cls");
    printf("=== VERIFICACAO DE PALINDROMO ===\n");

    // Le a palavra com scanf (sem espacos)
    printf("Digite uma palavra: ");
    scanf("%s", palavra);

    tam = strlen(palavra);

    // Estrategia: comparar o primeiro com o ultimo, o segundo com o
    // penultimo, e assim por diante, ate o meio da palavra.
    // i comeca no inicio (0) e j no final (tam-1); i avanca e j recua.
    for (i = 0, j = tam - 1; i < j; i++, j--) {
        // Converte ambos para minusculo para ignorar maiusculas/minusculas
        if (tolower(palavra[i]) != tolower(palavra[j])) {
            // Encontrou um par diferente -> nao e palindromo
            ehPalindromo = 0;
            break;  // nao precisa continuar comparando
        }
    }

    // Saida conforme o resultado da verificacao
    if (ehPalindromo) {
        printf("E palindromo\n");
    } else {
        printf("Nao e palindromo\n");
    }

    system("pause");
    return 0;
}
```
**Explicacao:** Compara os caracteres das pontas para o centro usando dois indices (i avanca, j recua). Se algum par for diferente, marca como nao palindromo e para. O `tolower` garante que a comparacao ignore caixa alta/baixa.

## [STRING] Separar uma linha CSV com strtok e converter com atoi/atof
Palavras-chave: `string`, `strtok`, `atoi`, `atof`, `csv`, `struct`, `fgets`, `strcspn`
**Enunciado:** Ler uma linha no formato CSV "codigo,nome,salario" (exemplo: "101,Joao Silva,2500.50"). Separar os tres campos usando strtok e converter o codigo para int (atoi) e o salario para float (atof). Mostrar os dados separados. Usar um struct para armazenar o funcionario.
**Gabarito:**
```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>  // system, atoi, atof
#include <string.h>  // strtok, strcspn

// Struct para guardar os dados do funcionario separados
typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

int main() {
    char linha[200];          // guarda a linha CSV inteira
    Funcionario f;            // variavel do tipo struct Funcionario
    char *campo;              // ponteiro para cada pedaco retornado pelo strtok
    const char sep[2] = ",";  // delimitador do CSV (virgula)

    system("cls");
    printf("=== SEPARACAO DE LINHA CSV ===\n");

    // Le a linha CSV com fgets (pode conter espacos no nome)
    fflush(stdin);  // limpa o buffer antes do fgets (nao portavel, mas funciona no Windows)
    printf("Digite a linha CSV (codigo,nome,salario): ");
    fgets(linha, sizeof(linha), stdin);

    // Remove o '\n' que o fgets coloca no final
    linha[strcspn(linha, "\n")] = '\0';

    // Primeira chamada do strtok: recebe a string e o delimitador
    // Retorna o primeiro campo (codigo) como string
    campo = strtok(linha, sep);
    if (campo != NULL) {
        f.codigo = atoi(campo);  // converte string para int
    }

    // Segunda chamada do strtok: passa NULL para continuar de onde parou
    // Retorna o segundo campo (nome)
    campo = strtok(NULL, sep);
    if (campo != NULL) {
        strcpy(f.nome, campo);  // copia a string para o campo nome do struct
    }

    // Terceira chamada do strtok: continua extraindo o salario
    campo = strtok(NULL, sep);
    if (campo != NULL) {
        f.salario = atof(campo);  // converte string para float
    }

    // Mostra os dados ja separados e convertidos
    printf("\n--- Dados separados ---\n");
    printf("Codigo: %d\n", f.codigo);
    printf("Nome: %s\n", f.nome);
    printf("Salario: %.2f\n", f.salario);

    system("pause");
    return 0;
}
```
**Explicacao:** strtok quebra a string a cada virgula: a primeira chamada recebe a string, as seguintes recebem NULL para continuar de onde parou. atoi converte o codigo para int e atof converte o salario para float, guardando tudo em um struct Funcionario.
## 5. STRUCT (basico e avancado)

### 5.1 Cadastro de alunos (vetor de struct)
Palavras-chave: `typedef struct`, `vetor de struct`, `media`, `situacao`
**Enunciado:** Faca um programa que cadastre 5 alunos em um vetor de struct. Cada aluno tem nome, duas notas, media e situacao ("Aprovado" se media >= 6, senao "Reprovado"). Ao final, liste todos os alunos em formato de tabela.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Definicao da struct Aluno usando typedef
// typedef permite usar "Aluno" como tipo direto, sem precisar escrever "struct Aluno"
typedef struct {
    char nome[50];
    float nota1;
    float nota2;
    float media;
    char situacao[15]; // "Aprovado" ou "Reprovado"
} Aluno;

int main() {
    Aluno alunos[5]; // vetor de struct: 5 posicoes do tipo Aluno
    int i;

    system("cls"); // limpa a tela (Windows)

    // Cadastro dos 5 alunos usando um laco for
    for (i = 0; i < 5; i++) {
        printf("=== Cadastro do aluno %d ===\n", i + 1);

        printf("Nome: ");
        fflush(stdin); // limpa o buffer do teclado (nao e portavel, mas funciona no Windows)
        scanf("%49[^\n]", alunos[i].nome); // le ate a quebra de linha (aceita espacos)

        printf("Nota 1: ");
        scanf("%f", &alunos[i].nota1);

        printf("Nota 2: ");
        scanf("%f", &alunos[i].nota2);

        // Calcula a media aritmetica das duas notas
        alunos[i].media = (alunos[i].nota1 + alunos[i].nota2) / 2.0;

        // Define a situacao baseado na media
        if (alunos[i].media >= 6.0) {
            strcpy(alunos[i].situacao, "Aprovado");
        } else {
            strcpy(alunos[i].situacao, "Reprovado");
        }

        printf("\n");
    }

    // Lista todos os alunos cadastrados em formato de tabela
    system("cls");
    printf("=== Lista de Alunos ===\n\n");
    // %-20s alinha a esquerda com 20 espacos; %6.1f reserva 6 casas com 1 decimal
    printf("%-20s %6s %6s %6s %10s\n", "Nome", "N1", "N2", "Media", "Situacao");
    for (i = 0; i < 5; i++) {
        printf("%-20s %6.1f %6.1f %6.1f %10s\n",
               alunos[i].nome,
               alunos[i].nota1,
               alunos[i].nota2,
               alunos[i].media,
               alunos[i].situacao);
    }

    printf("\n");
    system("pause"); // pausa para o usuario ver o resultado
    return 0;
}
```
**Explicacao:** Cria um vetor de struct Aluno e percorre com for para preencher nome, notas e calcular media/situacao. Depois um segundo for lista tudo em tabela formatada.

---

### 5.2 Struct Produto (mais caro + alerta estoque baixo)
Palavras-chave: `typedef struct`, `busca maior`, `estoque baixo`, `if`
**Enunciado:** Cadastre 5 produtos (codigo, nome, preco, estoque) em um vetor de struct. Ache e mostre o produto mais caro. Depois liste todos os produtos com estoque baixo (menor que 5 unidades), emitindo um alerta para cada um.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int estoque;
} Produto;

int main() {
    Produto produtos[5];
    int i, indiceMaior;
    float maiorPreco;

    system("cls");

    // Cadastro de 5 produtos
    for (i = 0; i < 5; i++) {
        printf("=== Produto %d ===\n", i + 1);
        printf("Codigo: ");
        scanf("%d", &produtos[i].codigo);

        printf("Nome: ");
        fflush(stdin); // limpa buffer (nao portavel, funciona no Windows)
        scanf("%49[^\n]", produtos[i].nome);

        printf("Preco: ");
        scanf("%f", &produtos[i].preco);

        printf("Estoque: ");
        scanf("%d", &produtos[i].estoque);
        printf("\n");
    }

    // Encontra o produto mais caro usando busca do maior
    // Comeca assumindo que o primeiro e o maior
    indiceMaior = 0;
    maiorPreco = produtos[0].preco;
    for (i = 1; i < 5; i++) {
        if (produtos[i].preco > maiorPreco) {
            maiorPreco = produtos[i].preco;
            indiceMaior = i;
        }
    }

    system("cls");

    // Mostra o produto mais caro
    printf("=== Produto mais caro ===\n");
    printf("Codigo: %d\n", produtos[indiceMaior].codigo);
    printf("Nome: %s\n", produtos[indiceMaior].nome);
    printf("Preco: %.2f\n", produtos[indiceMaior].preco);
    printf("Estoque: %d\n\n", produtos[indiceMaior].estoque);

    // Alerta de estoque baixo: percorre todos e verifica quem tem < 5
    printf("=== Alerta de estoque baixo (< 5) ===\n");
    for (i = 0; i < 5; i++) {
        if (produtos[i].estoque < 5) {
            printf("ATENCAO: %s (cod %d) tem apenas %d unidades\n",
                   produtos[i].nome, produtos[i].codigo, produtos[i].estoque);
        }
    }

    printf("\n");
    system("pause");
    return 0;
}
```
**Explicacao:** Usa a tecnica de "busca do maior" guardando o indice do maior preco. Depois um laco separado percorre o vetor testando estoque < 5 para emitir os alertas.

---

### 5.3 Ponteiro para struct (operador ->)
Palavras-chave: `ponteiro struct`, `operador ->`, `passagem por referencia`, `funcao`
**Enunciado:** Crie uma struct Funcionario (codigo, nome, salario). Faca uma funcao que receba a struct por PONTEIRO e aumente o salario em um percentual. Use o operador `->` para acessar os campos. No main, cadastre um funcionario, mostre antes/depois do aumento.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

// Funcao que recebe a struct por PONTEIRO e altera o campo salario
// O operador -> acessa campos de uma struct apontada por ponteiro
// f->salario equivale a (*f).salario
void aumentaSalario(Funcionario *f, float percentual) {
    f->salario = f->salario + (f->salario * percentual / 100.0);
}

int main() {
    Funcionario func;

    system("cls");

    // Cadastro inicial do funcionario
    printf("Codigo: ");
    scanf("%d", &func.codigo);

    printf("Nome: ");
    fflush(stdin); // limpa buffer (nao portavel, funciona no Windows)
    scanf("%49[^\n]", func.nome);

    printf("Salario: ");
    scanf("%f", &func.salario);

    // Mostra ANTES do aumento
    printf("\n--- ANTES do aumento ---\n");
    printf("Codigo: %d | Nome: %s | Salario: %.2f\n",
           func.codigo, func.nome, func.salario);

    // Chama a funcao passando o ENDERECO da struct (&)
    // Como passamos por ponteiro, a alteracao afeta a struct original
    aumentaSalario(&func, 10.0); // aumento de 10%

    // Mostra DEPOIS do aumento
    printf("\n--- DEPOIS do aumento de 10%% ---\n");
    printf("Codigo: %d | Nome: %s | Salario: %.2f\n",
           func.codigo, func.nome, func.salario);

    printf("\n");
    system("pause");
    return 0;
}
```
**Explicacao:** A funcao recebe o endereco da struct (&func), entao modifica o original diretamente via `->`. Sem ponteiro, a alteracao seria perdida (C passa por valor).

---

## 6. ARQUIVO TEXTO

### 6.1 Gravar 5 nomes em nomes.txt (fprintf)
Palavras-chave: `fopen`, `fprintf`, `modo w`, `fclose`
**Enunciado:** Faca um programa que leia 5 nomes do teclado e os grave em um arquivo texto chamado "nomes.txt", um nome por linha, usando fprintf.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main() {
    FILE *arq;       // ponteiro para o arquivo
    char nome[50];
    int i;

    system("cls");

    // Abre o arquivo para ESCRITA (modo "w")
    // "w" = write. Se o arquivo existir, ele sera sobrescrito.
    // Se nao existir, sera criado.
    arq = fopen("nomes.txt", "w");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        system("pause");
        return 1; // retorna 1 indicando erro
    }

    // Le 5 nomes do teclado e grava no arquivo, um por linha
    for (i = 0; i < 5; i++) {
        printf("Digite o nome %d: ", i + 1);
        fflush(stdin); // limpa buffer (nao portavel, funciona no Windows)
        scanf("%49[^\n]", nome);

        // fprintf grava uma linha formatada no arquivo
        // O \n no final faz cada nome ficar em uma linha separada
        fprintf(arq, "%s\n", nome);
    }

    fclose(arq); // SEMPRE feche o arquivo apos terminar de usar

    printf("\n5 nomes gravados em nomes.txt com sucesso!\n");
    system("pause");
    return 0;
}
```
**Explicacao:** Abre o arquivo em modo "w" (escrita), usa fprintf com `\n` para gravar cada nome em uma linha. Sempre feche o arquivo com fclose.

---

### 6.2 Ler nomes.txt com fgets e contar linhas
Palavras-chave: `fgets`, `strcspn`, `modo r`, `contador`
**Enunciado:** Faca um programa que abra o arquivo "nomes.txt" (criado no exercicio anterior) em modo leitura, leia cada linha com fgets, mostre os nomes na tela e conte quantas linhas/nomes existem no total.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

int main() {
    FILE *arq;
    char linha[100];
    int contador = 0;

    system("cls");

    // Abre o arquivo para LEITURA (modo "r" = read)
    arq = fopen("nomes.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo! Verifique se nomes.txt existe.\n");
        system("pause");
        return 1;
    }

    printf("=== Nomes lidos do arquivo ===\n\n");

    // fgets le uma linha inteira (ate \n ou ate 99 caracteres)
    // Retorna NULL quando chega ao fim do arquivo (EOF)
    // Por isso o while continua enquanto fgets != NULL
    while (fgets(linha, 100, arq) != NULL) {
        // fgets inclui o \n no final da string lida
        // strcspn encontra a posicao do \n e substituimos por \0
        linha[strcspn(linha, "\n")] = '\0';

        contador++; // conta mais um nome/linha
        printf("Nome %d: %s\n", contador, linha);
    }

    fclose(arq);

    printf("\nTotal de nomes/linhas: %d\n", contador);

    printf("\n");
    system("pause");
    return 0;
}
```
**Explicacao:** fgets le linha por linha ate retornar NULL (fim do arquivo). strcspn remove o `\n` que o fgets captura. O contador incrementa a cada linha valida.

---

### 6.3 Ler arquivo CSV (fgets + strtok + atoi/atof)
Palavras-chave: `CSV`, `strtok`, `atoi`, `atof`, `fgets`
**Enunciado:** Faca um programa que leia o arquivo "funcionario.txt" no formato CSV (codigo,nome,salario em cada linha). Use fgets para ler cada linha e strtok para separar os campos. Converta codigo com atoi e salario com atof. Liste todos os funcionarios em formato de tabela.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

int main() {
    FILE *arq;
    char linha[200];
    char *token;     // ponteiro para o pedaco separado pela strtok
    Funcionario f;

    system("cls");

    // Abre o arquivo CSV para leitura
    // Formato esperado de cada linha: codigo,nome,salario
    // Exemplo de linha: 1,Joao da Silva,2500.50
    arq = fopen("funcionario.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir funcionario.txt!\n");
        system("pause");
        return 1;
    }

    printf("=== Funcionarios (lidos do CSV) ===\n\n");
    printf("%-8s %-25s %12s\n", "Codigo", "Nome", "Salario");

    // Le o arquivo linha por linha com fgets
    while (fgets(linha, 200, arq) != NULL) {
        // Remove o \n do final da linha lida
        linha[strcspn(linha, "\n")] = '\0';

        // strtok divide a string pelo delimitador ","
        // Primeira chamada passa a string original e o delimitador
        token = strtok(linha, ",");
        f.codigo = atoi(token); // atoi converte string para int

        // Chamadas seguintes passam NULL para continuar de onde parou
        token = strtok(NULL, ",");
        strcpy(f.nome, token); // copia o nome para a struct

        // Terceira chamada pega o salario
        token = strtok(NULL, ",");
        f.salario = atof(token); // atof converte string para float/double

        printf("%-8d %-25s %12.2f\n", f.codigo, f.nome, f.salario);
    }

    fclose(arq);

    printf("\n");
    system("pause");
    return 0;
}
```
**Explicacao:** strtok quebra a linha a cada ",". A primeira chamada recebe a string; as proximas recebem NULL. atoi e atof convertem os campos texto para numero.

---

## 7. ARQUIVO BINARIO

### 7.1 Gravar 3 structs Funcionario em func.dat (fwrite, modo ab)
Palavras-chave: `fwrite`, `modo ab`, `sizeof`, `struct em arquivo`
**Enunciado:** Faca um programa que cadastre 3 funcionarios (struct com codigo, nome, salario) e os grave em um arquivo binario "func.dat" usando fwrite no modo "ab" (append binario).
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

int main() {
    FILE *arq;
    Funcionario f;
    int i;

    system("cls");

    // Abre o arquivo BINARIO para escrita no modo APPEND ("ab")
    // "ab" adiciona dados ao final sem apagar o que ja existe
    // Use "wb" se quiser criar do zero (apaga conteudo anterior)
    arq = fopen("func.dat", "ab");
    if (arq == NULL) {
        printf("Erro ao abrir func.dat!\n");
        system("pause");
        return 1;
    }

    // Cadastra 3 funcionarios e grava cada um no arquivo
    for (i = 0; i < 3; i++) {
        printf("=== Funcionario %d ===\n", i + 1);
        printf("Codigo: ");
        scanf("%d", &f.codigo);

        printf("Nome: ");
        fflush(stdin); // limpa buffer (nao portavel, funciona no Windows)
        scanf("%49[^\n]", f.nome);

        printf("Salario: ");
        scanf("%f", &f.salario);

        // fwrite grava a struct inteira no arquivo binario
        // Parametros: &endereco, tamanho de 1 elemento, quantidade, arquivo
        // sizeof(Funcionario) retorna o tamanho em bytes da struct
        fwrite(&f, sizeof(Funcionario), 1, arq);
        printf("\n");
    }

    fclose(arq); // fecha o arquivo

    printf("3 funcionarios gravados em func.dat!\n");
    system("pause");
    return 0;
}
```
**Explicacao:** fwrite grava a struct inteira como blocos de bytes. O modo "ab" preserva o conteudo existente e adiciona ao final. sizeof garante o tamanho correto.

---

### 7.2 Listar todos de func.dat (while fread == 1)
Palavras-chave: `fread`, `while(fread==1)`, `modo rb`, `listar`
**Enunciado:** Faca um programa que abra "func.dat" em modo leitura binaria ("rb") e liste todos os funcionarios. Use o padrao correto `while(fread(...) == 1)` para ler ate o fim do arquivo (NAO use while(!feof)).
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

int main() {
    FILE *arq;
    Funcionario f;

    system("cls");

    // Abre o arquivo binario para LEITURA (modo "rb" = read binary)
    arq = fopen("func.dat", "rb");
    if (arq == NULL) {
        printf("Erro ao abrir func.dat! Execute o exercicio 7.1 primeiro.\n");
        system("pause");
        return 1;
    }

    printf("=== Lista de Funcionarios ===\n\n");
    printf("%-8s %-25s %12s\n", "Codigo", "Nome", "Salario");

    // fread le 1 struct por vez
    // Retorna o numero de elementos lidos com sucesso:
    //   - 1 se leu um registro completo
    //   - 0 se chegou ao fim do arquivo (ou erro)
    //
    // O CORRETO e: while(fread(...) == 1)
    // NAO usar while(!feof(arq)) porque feof so retorna verdadeiro
    // DEPOIS de uma leitura falha, causando bug de ler o ultimo 2x
    while (fread(&f, sizeof(Funcionario), 1, arq) == 1) {
        printf("%-8d %-25s %12.2f\n", f.codigo, f.nome, f.salario);
    }

    fclose(arq);

    printf("\n");
    system("pause");
    return 0;
}
```
**Explicacao:** fread retorna 1 quando leu um registro completo e 0 no fim. O padrao `while(fread==1)` evita o bug classico do `while(!feof)` que repete o ultimo registro.

---

### 7.3 Buscar funcionario por codigo com fseek (acesso direto)
Palavras-chave: `fseek`, `SEEK_SET`, `acesso direto`, `sizeof`
**Enunciado:** Faca um programa que pergunte um codigo ao usuario e use fseek para posicionar o cursor do arquivo "func.dat" diretamente no registro correspondente, lendo-o com fread. Mostre os dados do funcionario encontrado ou mensagem de "nao encontrado".
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

int main() {
    FILE *arq;
    Funcionario f;
    int codBusca;

    system("cls");

    printf("Digite o codigo do funcionario: ");
    scanf("%d", &codBusca);

    arq = fopen("func.dat", "rb");
    if (arq == NULL) {
        printf("Erro ao abrir func.dat!\n");
        system("pause");
        return 1;
    }

    // ACESSO DIRETO com fseek
    // fseek posiciona o "cursor" do arquivo em um byte especifico
    // Parametros: arquivo, deslocamento em bytes, origem
    //   SEEK_SET = inicio do arquivo (posicao 0)
    //   SEEK_CUR = posicao atual
    //   SEEK_END = fim do arquivo
    //
    // Como cada struct ocupa sizeof(Funcionario) bytes,
    // o registro de indice N esta na posicao (N * sizeof(Funcionario)).
    // Aqui usamos codBusca como indice direto: registro de codigo N
    // fica na posicao N do arquivo (assume codigos sequenciais a partir de 0).
    fseek(arq, codBusca * sizeof(Funcionario), SEEK_SET);

    // Le o registro na posicao calculada
    // fread retorna 1 se leu com sucesso, 0 se passou do fim do arquivo
    if (fread(&f, sizeof(Funcionario), 1, arq) == 1) {
        printf("\n=== Funcionario encontrado ===\n");
        printf("Codigo: %d\n", f.codigo);
        printf("Nome: %s\n", f.nome);
        printf("Salario: %.2f\n", f.salario);
    } else {
        printf("\nCodigo nao encontrado!\n");
    }

    fclose(arq);

    printf("\n");
    system("pause");
    return 0;
}
```
**Explicacao:** fseek pula direto para a posicao `codigo * sizeof(struct)` sem ler os anteriores. Isso e acesso direto (rapido), diferente do acesso sequencial que percorre tudo.

---

## 8. STRUCT + ARQUIVO (combinacao critica)

### 8.1 CRUD completo em arquivo binario
Palavras-chave: `CRUD`, `wb`, `ab`, `rb`, `temp + rename`, `fseek`, `menu switch`
**Enunciado:** Faca um programa com menu (do-while + switch) que implemente um CRUD completo de Funcionario {int codigo; char nome[50]; float salario;} em arquivo binario "func.dat". Opcoes: 1-Criar arquivo (wb), 2-Cadastrar (ab, checando duplicidade de codigo), 3-Listar (rb), 4-Alterar (copia para temp.dat trocando o registro, depois remove+rename), 5-Excluir (copia para temp.dat pulando o excluido, depois remove+rename).
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

// Prototipos das funcoes (declaracao antes do main)
void criarArquivo();
void cadastrar();
void listar();
void alterar();
void excluir();

int main() {
    int op;

    // Menu do-while + switch (estilo do professor)
    do {
        system("cls");
        printf("=== CRUD Funcionario ===\n\n");
        printf("1 - Criar arquivo (wb)\n");
        printf("2 - Cadastrar (ab, checa duplicidade)\n");
        printf("3 - Listar (rb)\n");
        printf("4 - Alterar (temp + rename)\n");
        printf("5 - Excluir (temp + rename, pula excluido)\n");
        printf("0 - Sair\n");
        printf("\nOpcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1: criarArquivo(); break;
            case 2: cadastrar();   break;
            case 3: listar();      break;
            case 4: alterar();     break;
            case 5: excluir();     break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n"); system("pause");
        }
    } while (op != 0);

    return 0;
}

// Opcao 1: Cria o arquivo do zero (modo "wb")
// "wb" apaga o arquivo se ele ja existir e cria um novo vazio
void criarArquivo() {
    FILE *arq = fopen("func.dat", "wb");
    if (arq == NULL) {
        printf("Erro ao criar arquivo!\n");
    } else {
        printf("Arquivo criado com sucesso (vazio)!\n");
        fclose(arq);
    }
    system("pause");
}

// Opcao 2: Cadastra novo funcionario (modo "ab" = append binario)
// Antes de gravar, checa se o codigo ja existe (nao permite duplicidade)
void cadastrar() {
    FILE *arq;
    Funcionario f, temp;
    int duplicado = 0;

    printf("Codigo: ");
    scanf("%d", &f.codigo);

    printf("Nome: ");
    fflush(stdin); // limpa buffer (nao portavel, funciona no Windows)
    scanf("%49[^\n]", f.nome);

    printf("Salario: ");
    scanf("%f", &f.salario);

    // Abre para LEITURA primeiro para checar se o codigo ja existe
    arq = fopen("func.dat", "rb");
    if (arq != NULL) {
        while (fread(&temp, sizeof(Funcionario), 1, arq) == 1) {
            if (temp.codigo == f.codigo) {
                duplicado = 1; // achou codigo repetido
                break;
            }
        }
        fclose(arq);
    }

    if (duplicado) {
        printf("Erro: codigo %d ja existe! Nao pode duplicar.\n", f.codigo);
    } else {
        // Abre em modo "ab" para ADICIONAR no final do arquivo
        arq = fopen("func.dat", "ab");
        if (arq == NULL) {
            printf("Erro ao abrir arquivo!\n");
        } else {
            fwrite(&f, sizeof(Funcionario), 1, arq);
            fclose(arq);
            printf("Funcionario cadastrado com sucesso!\n");
        }
    }
    system("pause");
}

// Opcao 3: Lista todos os funcionarios (modo "rb")
void listar() {
    FILE *arq;
    Funcionario f;

    arq = fopen("func.dat", "rb");
    if (arq == NULL) {
        printf("Arquivo nao existe! Crie primeiro (opcao 1).\n");
        system("pause");
        return;
    }

    system("cls");
    printf("=== Lista de Funcionarios ===\n\n");
    printf("%-8s %-25s %12s\n", "Codigo", "Nome", "Salario");

    // Le todos os registros ate o fim do arquivo
    while (fread(&f, sizeof(Funcionario), 1, arq) == 1) {
        printf("%-8d %-25s %12.2f\n", f.codigo, f.nome, f.salario);
    }

    fclose(arq);
    printf("\n");
    system("pause");
}

// Opcao 4: Altera um funcionario pelo codigo
// Estrategia: copia tudo para temp.dat, TROCANDO o registro alterado
// Depois remove o original e renomeia temp.dat para func.dat
void alterar() {
    FILE *arq, *temp;
    Funcionario f;
    int codBusca, achou = 0;

    printf("Codigo do funcionario a alterar: ");
    scanf("%d", &codBusca);

    arq = fopen("func.dat", "rb");
    if (arq == NULL) {
        printf("Arquivo nao existe!\n");
        system("pause");
        return;
    }

    temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(arq);
        system("pause");
        return;
    }

    // Le cada registro do original e grava no temporario
    while (fread(&f, sizeof(Funcionario), 1, arq) == 1) {
        if (f.codigo == codBusca && !achou) {
            // Achou o registro: pede os novos dados
            achou = 1;
            printf("Novos dados para codigo %d:\n", codBusca);
            printf("Nome: ");
            fflush(stdin); // limpa buffer (nao portavel, funciona no Windows)
            scanf("%49[^\n]", f.nome);
            printf("Salario: ");
            scanf("%f", &f.salario);
        }
        // Grava o registro (alterado ou copiado igual) no temporario
        fwrite(&f, sizeof(Funcionario), 1, temp);
    }

    fclose(arq);
    fclose(temp);

    if (achou) {
        // remove o original e renomeia o temporario
        remove("func.dat");
        rename("temp.dat", "func.dat");
        printf("Funcionario alterado com sucesso!\n");
    } else {
        remove("temp.dat"); // nao achou, descarta o temporario
        printf("Codigo nao encontrado!\n");
    }
    system("pause");
}

// Opcao 5: Exclui um funcionario pelo codigo
// Estrategia: copia tudo para temp.dat PULANDO o registro excluido
// Depois remove o original e renomeia temp.dat para func.dat
void excluir() {
    FILE *arq, *temp;
    Funcionario f;
    int codBusca, achou = 0;

    printf("Codigo do funcionario a excluir: ");
    scanf("%d", &codBusca);

    arq = fopen("func.dat", "rb");
    if (arq == NULL) {
        printf("Arquivo nao existe!\n");
        system("pause");
        return;
    }

    temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(arq);
        system("pause");
        return;
    }

    // Le cada registro e grava no temporario, PULANDO o excluido
    while (fread(&f, sizeof(Funcionario), 1, arq) == 1) {
        if (f.codigo == codBusca) {
            achou = 1;     // marca que achou o registro
            continue;      // pula para o proximo SEM gravar este (exclui)
        }
        // Grava todos os registros exceto o excluido
        fwrite(&f, sizeof(Funcionario), 1, temp);
    }

    fclose(arq);
    fclose(temp);

    if (achou) {
        remove("func.dat");
        rename("temp.dat", "func.dat");
        printf("Funcionario excluido com sucesso!\n");
    } else {
        remove("temp.dat"); // nao achou, descarta o temporario
        printf("Codigo nao encontrado!\n");
    }
    system("pause");
}
```
**Explicacao:** Alterar e excluir usam a tecnica do arquivo temporario: copia todos os registros para temp.dat (modificando ou pulando o alvo), depois remove o original e renomeia o temp. Isso porque nao da para "apagar" bytes no meio de um arquivo binario diretamente.
## 9. FUNCOES / MODULARIZACAO - Calculadora modularizada
Palavras-chave: `funcao`, `prototipo`
**Enunciado:** Faca uma calculadora que usa funcoes separadas para somar, subtrair, multiplicar e dividir dois numeros. Mostre um menu do-while+switch para o usuario escolher a operacao. A funcao de divisao deve evitar divisao por zero.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // getch, clrscr (estilo do professor)

// Prototipos das funcoes (declarados antes do main para o compilador conhecer)
float somar(float a, float b);
float subtrair(float a, float b);
float multiplicar(float a, float b);
float dividir(float a, float b);

int main(void) {
    int opcao;
    float n1, n2, resultado;

    do {
        system("cls");  // limpa a tela (Windows)
        printf("===== CALCULADORA MODULARIZADA =====\n");
        printf("1 - Somar\n");
        printf("2 - Subtrair\n");
        printf("3 - Multiplicar\n");
        printf("4 - Dividir\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Se a opcao for valida e nao for sair, pede os dois numeros
        if (opcao >= 1 && opcao <= 4) {
            printf("Digite o primeiro numero: ");
            scanf("%f", &n1);
            printf("Digite o segundo numero: ");
            scanf("%f", &n2);
        }

        switch (opcao) {
            case 1:
                resultado = somar(n1, n2);  // chama a funcao
                printf("Resultado: %.2f\n", resultado);
                break;
            case 2:
                resultado = subtrair(n1, n2);
                printf("Resultado: %.2f\n", resultado);
                break;
            case 3:
                resultado = multiplicar(n1, n2);
                printf("Resultado: %.2f\n", resultado);
                break;
            case 4:
                resultado = dividir(n1, n2);  // ja trata divisao por zero dentro
                if (resultado != -1) {
                    printf("Resultado: %.2f\n", resultado);
                }
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

        if (opcao != 0) {
            printf("\nPressione qualquer tecla para continuar...");
            getch();  // espera uma tecla
        }
    } while (opcao != 0);  // repete ate escolher sair

    return 0;
}

// Implementacao das funcoes
float somar(float a, float b) {
    return a + b;  // retorna a soma
}

float subtrair(float a, float b) {
    return a - b;
}

float multiplicar(float a, float b) {
    return a * b;
}

float dividir(float a, float b) {
    // Trata divisao por zero antes de dividir
    if (b == 0) {
        printf("Erro: divisao por zero!\n");
        return -1;  // retorna -1 como flag de erro
    }
    return a / b;
}
```
**Explicacao:** Cada operacao vira uma funcao separada com prototipo antes do main. O menu do-while+switch chama a funcao certa conforme a opcao, e a divisao valida o divisor antes de calcular.

---

## 9. FUNCOES / MODULARIZACAO - Cadastro com funcoes separadas (struct)
Palavras-chave: `struct`, `funcao`, `passagem por valor`
**Enunciado:** Crie uma struct Aluno com nome e 3 notas. Faca tres funcoes: uma para ler os dados do aluno, uma para imprimir os dados, e uma para calcular e retornar a media (float). O main coordena as chamadas.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// Definicao do tipo struct Aluno
typedef struct {
    char nome[50];
    float nota1, nota2, nota3;
} Aluno;

// Prototipos
void ler_aluno(Aluno a);   // recebe por valor (copia) - atencao aqui!
void imprimir_aluno(Aluno a);
float calcula_media(Aluno a);

int main(void) {
    Aluno a;

    ler_aluno(a);        // CUIDADO: passa por valor, nao altera o original
    // Para funcionar de verdade, ler_aluno deveria receber ponteiro.
    // Aqui mantemos por valor para mostrar o padrao, mas lemos no main.
    printf("Digite o nome: ");
    fflush(stdin);  // limpa o buffer do teclado (nao e portavel, mas funciona no Windows)
    gets(a.nome);   // ATENCAO: gets e inseguro - em prova use fgets se possivel
    printf("Nota 1: "); scanf("%f", &a.nota1);
    printf("Nota 2: "); scanf("%f", &a.nota2);
    printf("Nota 3: "); scanf("%f", &a.nota3);

    imprimir_aluno(a);  // imprime os dados
    printf("Media: %.2f\n", calcula_media(a));  // mostra a media

    printf("\nPressione qualquer tecla...");
    getch();
    return 0;
}

// Funcao que le os dados - ATENCAO: por valor nao altera o original do main
void ler_aluno(Aluno a) {
    // Esta versao NAO funciona para preencher o aluno do main,
    // porque recebe uma COPIA. Mostramos aqui apenas como exemplo.
    // O correto seria: void ler_aluno(Aluno *a) e usar a->campo
    printf("(funcao ler_aluno chamada - ver comentario no main)\n");
}

// Funcao que imprime os dados do aluno
void imprimir_aluno(Aluno a) {
    printf("\n--- DADOS DO ALUNO ---\n");
    printf("Nome: %s\n", a.nome);
    printf("Notas: %.1f | %.1f | %.1f\n", a.nota1, a.nota2, a.nota3);
}

// Funcao que calcula e retorna a media das 3 notas
float calcula_media(Aluno a) {
    return (a.nota1 + a.nota2 + a.nota3) / 3.0f;
}
```
**Explicacao:** A struct Aluno e passada para funcoes que leem (comentario sobre valor vs ponteiro), imprimem e calculam a media. A media retorna float para ser usada no main.

---

## 9. FUNCOES / MODULARIZACAO - Funcao que retorna struct
Palavras-chave: `struct`, `return struct`
**Enunciado:** Crie uma struct Produto (codigo, nome, preco). Faca uma funcao `Produto criar_produto()` que le os dados e DEVOLVE um Produto preenchido para o main. O main recebe e imprime.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // necessario para strlen
#include <conio.h>

// Definicao da struct Produto
typedef struct {
    int codigo;
    char nome[50];
    float preco;
} Produto;

// Prototipo: a funcao retorna um Produto (struct)
Produto criar_produto(void);

int main(void) {
    Produto p;  // declara variavel struct

    p = criar_produto();  // chama a funcao e guarda o retorno em p

    // Imprime o produto recebido
    printf("\n--- PRODUTO CRIADO ---\n");
    printf("Codigo: %d\n", p.codigo);
    printf("Nome: %s\n", p.nome);
    printf("Preco: %.2f\n", p.preco);

    printf("\nPressione qualquer tecla...");
    getch();
    return 0;
}

// Funcao que cria e devolve um Produto preenchido
Produto criar_produto(void) {
    Produto p;  // variavel LOCAL da funcao

    printf("Digite o codigo: ");
    scanf("%d", &p.codigo);
    printf("Digite o nome: ");
    fflush(stdin);  // limpa buffer antes de ler string (nao portavel, funciona no Windows)
    fgets(p.nome, 50, stdin);  // le string com seguranca (melhor que gets)
    // Remove o \n do final deixado pelo fgets
    if (p.nome[strlen(p.nome) - 1] == '\n') {
        p.nome[strlen(p.nome) - 1] = '\0';
    }
    printf("Digite o preco: ");
    scanf("%f", &p.preco);

    return p;  // devolve a struct inteira para quem chamou
}
```
**Explicacao:** A funcao declara um Produto local, preenche e o retorna com `return p`. O main guarda o retorno em outra variavel struct. Note o uso de fgets (seguro) no lugar de gets.

---

## 10. RECURSAO - Fatorial recursivo
Palavras-chave: `recursao`, `caso base`
**Enunciado:** Faca uma funcao recursiva para calcular o fatorial de N. Caso base: n <= 1 retorna 1. Caso recursivo: n * fat(n-1). Leia N no main e mostre o resultado.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// Prototipo da funcao recursiva
long fatorial(int n);

int main(void) {
    int n;

    printf("Digite um numero para calcular o fatorial: ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Nao existe fatorial de numero negativo!\n");
    } else {
        printf("%d! = %ld\n", n, fatorial(n));  // %ld para long
    }

    printf("\nPressione qualquer tecla...");
    getch();
    return 0;
}

// Funcao recursiva: chama a si mesma ate chegar no caso base
long fatorial(int n) {
    if (n <= 1) {
        return 1;  // CASO BASE: para a recursao
    }
    // CASO RECURSIVO: n * fatorial(n-1)
    // Exemplo: fat(4) = 4 * fat(3) = 4 * 3 * fat(2) = 4*3*2*fat(1) = 4*3*2*1 = 24
    return n * fatorial(n - 1);
}
```
**Explicacao:** A recursao para no caso base (n<=1) e otherwise multiplica n pela chamada de n-1. Cada chamada empilha ate o caso base e depois desempilha multiplicando.

---

## 10. RECURSAO - Fibonacci recursivo
Palavras-chave: `recursao`, `fibonacci`
**Enunciado:** Faca uma funcao recursiva para calcular o N-esimo termo de Fibonacci. Caso base: n <= 1 retorna n. Caso recursivo: fib(n-1) + fib(n-2). Mostre os N primeiros termos.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// Prototipo da funcao recursiva
int fibonacci(int n);

int main(void) {
    int n, i;

    printf("Quantos termos de Fibonacci mostrar? ");
    scanf("%d", &n);

    printf("Sequencia: ");
    for (i = 0; i < n; i++) {
        printf("%d ", fibonacci(i));  // chama a recursao para cada termo
    }
    printf("\n");

    printf("\nPressione qualquer tecla...");
    getch();
    return 0;
}

// Funcao recursiva de Fibonacci
int fibonacci(int n) {
    if (n <= 1) {
        return n;  // CASO BASE: fib(0)=0, fib(1)=1
    }
    // CASO RECURSIVO: fib(n) = fib(n-1) + fib(n-2)
    // Exemplo: fib(5) = fib(4) + fib(3) = 3 + 2 = 5
    return fibonacci(n - 1) + fibonacci(n - 2);
}
```
**Explicacao:** O caso base retorna o proprio n (0 ou 1) e o caso recursivo soma os dois termos anteriores. O loop no main chama a funcao para cada indice de 0 ate N-1.

---

## 10. RECURSAO - Imprimir de N ate 1 (PEGADINHA: antes vs depois da chamada)
Palavras-chave: `recursao`, `ordem impressao`
**Enunciado:** Faca DUAS funcoes recursivas que imprimem de 1 ate N. A primeira coloca o printf ANTES da chamada recursiva (decrescente: 5 4 3 2 1). A segunda coloca o printf DEPOIS da chamada recursiva (crescente: 1 2 3 4 5). Mostre as duas para ver a diferenca.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// Prototipos
void imprimir_decrescente(int n);  // printf ANTES da chamada
void imprimir_crescente(int n);   // printf DEPOIS da chamada

int main(void) {
    int n;

    printf("Digite N: ");
    scanf("%d", &n);

    printf("\nDecrescente (printf ANTES da chamada): ");
    imprimir_decrescente(n);
    // Saida para n=5: 5 4 3 2 1

    printf("\nCrescente (printf DEPOIS da chamada): ");
    imprimir_crescente(n);
    // Saida para n=5: 1 2 3 4 5

    printf("\n\nPressione qualquer tecla...");
    getch();
    return 0;
}

// PEGADINHA 1: printf ANTES da chamada recursiva
// Imprime primeiro, depois desce. Resultado: DECRESCENTE (5 4 3 2 1)
void imprimir_decrescente(int n) {
    if (n < 1) {
        return;  // caso base: para quando n chega em 0
    }
    printf("%d ", n);              // (1) imprime ANTES de chamar
    imprimir_decrescente(n - 1);  // (2) so depois chama a recursao
}

// PEGADINHA 2: printf DEPOIS da chamada recursiva
// Desce ate o caso base primeiro, e vai imprimindo na volta.
// Resultado: CRESCENTE (1 2 3 4 5)
void imprimir_crescente(int n) {
    if (n < 1) {
        return;  // caso base
    }
    imprimir_crescente(n - 1);  // (1) chama a recursao PRIMEIRO
    printf("%d ", n);            // (2) so imprime DEPOIS (na volta da pilha)
}
```
**Explicacao:** A ordem do printf muda tudo. Antes da chamada imprime na descida (decrescente); depois da chamada imprime na volta da pilha (crescente). Decorar essa pegadinha cai em prova.

---

## 11. PONTEIROS - Trocar dois valores com ponteiros
Palavras-chave: `ponteiro`, `passagem por referencia`
**Enunciado:** Faca uma funcao `void troca(int *a, int *b)` que troca os valores de duas variaveis usando ponteiros. Leia dois valores no main, troque e mostre antes/depois.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// Prototipo: recebe enderecos (ponteiros) para alterar as variaveis originais
void troca(int *a, int *b);

int main(void) {
    int x, y;

    printf("Digite o valor de x: ");
    scanf("%d", &x);
    printf("Digite o valor de y: ");
    scanf("%d", &y);

    printf("\nANTES da troca: x = %d | y = %d\n", x, y);

    troca(&x, &y);  // passa os ENDERECOS (&) das variaveis

    printf("DEPOIS da troca: x = %d | y = %d\n", x, y);

    printf("\nPressione qualquer tecla...");
    getch();
    return 0;
}

// Funcao que troca usando ponteiros
void troca(int *a, int *b) {
    int aux;  // variavel auxiliar para guardar um valor temporariamente

    aux = *a;  // aux recebe o valor apontado por a
    *a = *b;   // valor apontado por a recebe o valor apontado por b
    *b = aux;  // valor apontado por b recebe o aux
    // Como mexemos no CONTEUDO dos enderecos, as variaveis do main mudam
}
```
**Explicacao:** Passar `&x` envia o endereco, e a funcao usa `*a` para alterar o conteudo original. Sem ponteiro, a troca nao refletiria no main (passagem por valor).

---

## 11. PONTEIROS - Dobrar um valor via ponteiro
Palavras-chave: `ponteiro`, `void com ponteiro`
**Enunciado:** Faca uma funcao `void dobrar(int *x)` que dobra o valor apontado. Leia um numero no main, chame a funcao e mostre o valor dobrado.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// Prototipo: recebe um ponteiro para int
void dobrar(int *x);

int main(void) {
    int numero;

    printf("Digite um numero: ");
    scanf("%d", &numero);

    printf("\nANTES: %d\n", numero);

    dobrar(&numero);  // passa o endereco da variavel

    printf("DEPOIS: %d\n", numero);  // valor alterado pela funcao

    printf("\nPressione qualquer tecla...");
    getch();
    return 0;
}

// Funcao que dobra o valor apontado pelo ponteiro
void dobrar(int *x) {
    *x = *x * 2;  // dobra o CONTEUDO apontado por x
    // Equivalente: *x *= 2;
    // Como alteramos via ponteiro, a variavel do main e modificada
}
```
**Explicacao:** A funcao void nao retorna nada, mas altera o valor original via ponteiro `*x`. O main passa `&numero` e o conteudo e dobrado diretamente na memoria.

---

## 11. PONTEIROS - Ponteiro para struct (operador ->)
Palavras-chave: `ponteiro`, `struct`, `malloc`, `operador seta`
**Enunciado:** Crie uma struct Produto. Aloque dinamicamente com malloc, preencha os campos via ponteiro usando o operador `->`, e imprima. Nao esqueca do free.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Definicao da struct Produto
typedef struct {
    int codigo;
    char nome[50];
    float preco;
} Produto;

int main(void) {
    Produto *p;  // declara um PONTEIRO para Produto

    // Aloca dinamicamente memoria para um Produto
    p = (Produto *) malloc(sizeof(Produto));

    // Sempre verificar se a alocacao deu certo
    if (p == NULL) {
        printf("Erro: sem memoria!\n");
        return 1;  // sai do programa com erro
    }

    // Preenchendo via ponteiro usando o operador SETA (->)
    // Atencao: com ponteiro para struct usamos -> e nao ponto (.)
    p->codigo = 101;
    strcpy(p->nome, "Teclado USB");  // nao pode atribuir string com =
    p->preco = 89.90;

    // Imprimindo via ponteiro (tambem usa ->)
    printf("--- PRODUTO (alocado dinamicamente) ---\n");
    printf("Codigo: %d\n", p->codigo);
    printf("Nome: %s\n", p->nome);
    printf("Preco: %.2f\n", p->preco);

    free(p);  // LIBERA a memoria alocada (evita vazamento de memoria)

    printf("\nPressione qualquer tecla...");
    getch();
    return 0;
}
```
**Explicacao:** malloc reserva memoria e devolve um ponteiro; acessa-se os campos com `->` (seta) em vez de ponto. Sempre confira se o ponteiro nao e NULL e libere com free no final.

---

## 12. DESAFIOS COMPLEXOS - Sistema de notas escolar
Palavras-chave: `struct`, `vetor`, `media`, `situacao`
**Enunciado:** Crie uma struct Aluno (nome, 3 notas, media, situacao). Cadastre N alunos num vetor. Calcule a media de cada um, defina situacao (Aprovado se media>=7, senao Reprovado), liste os aprovados e calcule a media da turma.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX 50  // numero maximo de alunos

// Definicao da struct Aluno
typedef struct {
    char nome[50];
    float nota1, nota2, nota3;
    float media;
    char situacao[15];  // "Aprovado" ou "Reprovado"
} Aluno;

int main(void) {
    Aluno turma[MAX];  // vetor de alunos
    int n, i;
    float soma_medias = 0, media_turma;

    printf("Quantos alunos cadastrar (max %d)? ", MAX);
    scanf("%d", &n);

    // Cadastro de cada aluno
    for (i = 0; i < n; i++) {
        printf("\n--- ALUNO %d ---\n", i + 1);
        printf("Nome: ");
        fflush(stdin);  // limpa buffer (nao portavel, funciona no Windows)
        fgets(turma[i].nome, 50, stdin);
        // remove o \n do final
        turma[i].nome[strcspn(turma[i].nome, "\n")] = '\0';

        printf("Nota 1: "); scanf("%f", &turma[i].nota1);
        printf("Nota 2: "); scanf("%f", &turma[i].nota2);
        printf("Nota 3: "); scanf("%f", &turma[i].nota3);

        // Calcula a media do aluno
        turma[i].media = (turma[i].nota1 + turma[i].nota2 + turma[i].nota3) / 3.0f;

        // Define a situacao conforme a media
        if (turma[i].media >= 7.0) {
            strcpy(turma[i].situacao, "Aprovado");
        } else {
            strcpy(turma[i].situacao, "Reprovado");
        }

        soma_medias += turma[i].media;  // acumula para media da turma
    }

    // Lista todos os alunos com media e situacao
    system("cls");
    printf("===== LISTA DE ALUNOS =====\n");
    for (i = 0; i < n; i++) {
        printf("%-20s | Media: %.2f | %s\n",
               turma[i].nome, turma[i].media, turma[i].situacao);
    }

    // Lista apenas os aprovados
    printf("\n----- APROVADOS -----\n");
    for (i = 0; i < n; i++) {
        if (strcmp(turma[i].situacao, "Aprovado") == 0) {
            printf("%s (media %.2f)\n", turma[i].nome, turma[i].media);
        }
    }

    // Media da turma
    media_turma = soma_medias / n;
    printf("\nMedia da turma: %.2f\n", media_turma);

    printf("\nPressione qualquer tecla...");
    getch();
    return 0;
}
```
**Explicacao:** Um vetor de structs guarda os alunos; cada media e calculada e a situacao definida com strcmp/strcpy. No final percorre o vetor para listar aprovados e calcular a media da turma.

---

## 12. DESAFIOS COMPLEXOS - Controle de estoque
Palavras-chave: `struct`, `vetor`, `estoque`, `valor total`
**Enunciado:** Crie uma struct Produto (codigo, nome, preco, quantidade). Cadastre N produtos num vetor. Faca um menu para cadastrar, listar, alertar estoque baixo (quantidade < 5) e mostrar o valor total do estoque (soma preco*quantidade).
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX 100  // maximo de produtos

// Definicao da struct Produto
typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
} Produto;

// Prototipos das funcoes
void cadastrar(Produto p[], int *qtd);
void listar(Produto p[], int qtd);
void alertar_estoque_baixo(Produto p[], int qtd);
void valor_total(Produto p[], int qtd);

int main(void) {
    Produto produtos[MAX];  // vetor de produtos
    int qtd = 0;            // contador de produtos cadastrados
    int opcao;

    do {
        system("cls");
        printf("===== CONTROLE DE ESTOQUE =====\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Listar produtos\n");
        printf("3 - Alertar estoque baixo (<5)\n");
        printf("4 - Valor total do estoque\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrar(produtos, &qtd);  // passa endereco de qtd para incrementar
                break;
            case 2:
                listar(produtos, qtd);
                break;
            case 3:
                alertar_estoque_baixo(produtos, qtd);
                break;
            case 4:
                valor_total(produtos, qtd);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

        if (opcao != 0) {
            printf("\nPressione qualquer tecla...");
            getch();
        }
    } while (opcao != 0);

    return 0;
}

// Funcao para cadastrar um produto
void cadastrar(Produto p[], int *qtd) {
    if (*qtd >= MAX) {
        printf("Estoque cheio!\n");
        return;
    }
    printf("\n--- CADASTRO ---\n");
    printf("Codigo: "); scanf("%d", &p[*qtd].codigo);
    printf("Nome: ");
    fflush(stdin);  // limpa buffer (nao portavel, funciona no Windows)
    fgets(p[*qtd].nome, 50, stdin);
    p[*qtd].nome[strcspn(p[*qtd].nome, "\n")] = '\0';  // remove \n
    printf("Preco: "); scanf("%f", &p[*qtd].preco);
    printf("Quantidade: "); scanf("%d", &p[*qtd].quantidade);
    (*qtd)++;  // incrementa o contador (precisa de parenteses por precedencia)
    printf("Produto cadastrado!\n");
}

// Funcao para listar todos os produtos
void listar(Produto p[], int qtd) {
    int i;
    printf("\n--- LISTA DE PRODUTOS ---\n");
    for (i = 0; i < qtd; i++) {
        printf("%d | %-20s | R$ %.2f | Qtd: %d\n",
               p[i].codigo, p[i].nome, p[i].preco, p[i].quantidade);
    }
}

// Funcao para alertar produtos com estoque baixo (< 5)
void alertar_estoque_baixo(Produto p[], int qtd) {
    int i, achou = 0;
    printf("\n--- ESTOQUE BAIXO (<5) ---\n");
    for (i = 0; i < qtd; i++) {
        if (p[i].quantidade < 5) {
            printf("%s - apenas %d unidades!\n", p[i].nome, p[i].quantidade);
            achou = 1;
        }
    }
    if (!achou) {
        printf("Nenhum produto com estoque baixo.\n");
    }
}

// Funcao para calcular o valor total do estoque
void valor_total(Produto p[], int qtd) {
    int i;
    float total = 0;
    for (i = 0; i < qtd; i++) {
        total += p[i].preco * p[i].quantidade;  // soma preco*quantidade
    }
    printf("\nValor total do estoque: R$ %.2f\n", total);
}
```
**Explicacao:** O vetor de structs e manipulado por funcoes que recebem o array e o contador (por ponteiro para incrementar). O valor total soma preco*quantidade de cada produto.

---

## 12. DESAFIOS COMPLEXOS - Menu completo de cadastro em arquivo
Palavras-chave: `arquivo`, `struct`, `fread`, `fwrite`, `menu`
**Enunciado:** Crie uma struct Cliente (codigo, nome, telefone). Salve em arquivo binario `clientes.dat`. Faca um menu do-while+switch com: cadastrar, listar, buscar, alterar, excluir e sair. Tudo modularizado em funcoes.
**Gabarito:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Definicao da struct Cliente
typedef struct {
    int codigo;
    char nome[50];
    char telefone[15];
} Cliente;

// Prototipos das funcoes
void cadastrar(void);
void listar(void);
void buscar(void);
void alterar(void);
void excluir(void);

int main(void) {
    int opcao;

    do {
        system("cls");
        printf("===== CADASTRO DE CLIENTES =====\n");
        printf("1 - Cadastrar\n");
        printf("2 - Listar todos\n");
        printf("3 - Buscar por codigo\n");
        printf("4 - Alterar\n");
        printf("5 - Excluir\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrar(); break;
            case 2: listar();    break;
            case 3: buscar();    break;
            case 4: alterar();   break;
            case 5: excluir();   break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }

        if (opcao != 0) {
            printf("\nPressione qualquer tecla...");
            getch();
        }
    } while (opcao != 0);

    return 0;
}

// CADASTRAR: abre o arquivo em modo append (adiciona no final)
void cadastrar(void) {
    Cliente c;
    FILE *arq = fopen("clientes.dat", "ab");  // ab = append binario

    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("\n--- CADASTRO ---\n");
    printf("Codigo: "); scanf("%d", &c.codigo);
    printf("Nome: ");
    fflush(stdin);  // limpa buffer (nao portavel, funciona no Windows)
    fgets(c.nome, 50, stdin);
    c.nome[strcspn(c.nome, "\n")] = '\0';
    printf("Telefone: ");
    fgets(c.telefone, 15, stdin);
    c.telefone[strcspn(c.telefone, "\n")] = '\0';

    fwrite(&c, sizeof(Cliente), 1, arq);  // grava 1 struct no arquivo
    fclose(arq);
    printf("Cliente cadastrado!\n");
}

// LISTAR: le todos os registros e mostra
void listar(void) {
    Cliente c;
    FILE *arq = fopen("clientes.dat", "rb");  // rb = read binario

    if (arq == NULL) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("\n--- LISTA DE CLIENTES ---\n");
    // Le registro por registro ate o final do arquivo
    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        printf("%d | %-20s | %s\n", c.codigo, c.nome, c.telefone);
    }
    fclose(arq);
}

// BUSCAR: procura um cliente pelo codigo
void buscar(void) {
    Cliente c;
    int codigo, achou = 0;
    FILE *arq = fopen("clientes.dat", "rb");

    if (arq == NULL) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("Digite o codigo para buscar: ");
    scanf("%d", &codigo);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.codigo == codigo) {
            printf("\nENCONTRADO:\n");
            printf("Codigo: %d\n", c.codigo);
            printf("Nome: %s\n", c.nome);
            printf("Telefone: %s\n", c.telefone);
            achou = 1;
            break;
        }
    }
    if (!achou) {
        printf("Cliente nao encontrado.\n");
    }
    fclose(arq);
}

// ALTERAR: reescreve o arquivo trocando o registro encontrado
void alterar(void) {
    Cliente c;
    int codigo, achou = 0;
    FILE *arq = fopen("clientes.dat", "r+b");  // r+b = le/escrita binario

    if (arq == NULL) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("Digite o codigo para alterar: ");
    scanf("%d", &codigo);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.codigo == codigo) {
            // Reposiciona o ponteiro para o inicio deste registro
            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            // Le os novos dados
            printf("Novo nome: ");
            fflush(stdin);
            fgets(c.nome, 50, stdin);
            c.nome[strcspn(c.nome, "\n")] = '\0';
            printf("Novo telefone: ");
            fgets(c.telefone, 15, stdin);
            c.telefone[strcspn(c.telefone, "\n")] = '\0';
            fwrite(&c, sizeof(Cliente), 1, arq);  // sobrescreve
            achou = 1;
            printf("Cliente alterado!\n");
            break;
        }
    }
    if (!achou) {
        printf("Cliente nao encontrado.\n");
    }
    fclose(arq);
}

// EXCLUIR: copia todos exceto o excluido para um arquivo temporario
void excluir(void) {
    Cliente c;
    int codigo, achou = 0;
    FILE *arq = fopen("clientes.dat", "rb");
    FILE *tmp = fopen("temp.dat", "wb");  // arquivo temporario

    if (arq == NULL) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("Digite o codigo para excluir: ");
    scanf("%d", &codigo);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.codigo == codigo) {
            achou = 1;  // nao copia este registro (exclui)
        } else {
            fwrite(&c, sizeof(Cliente), 1, tmp);  // copia os outros
        }
    }
    fclose(arq);
    fclose(tmp);

    remove("clientes.dat");        // apaga o arquivo original
    rename("temp.dat", "clientes.dat");  // renomeia o temporario

    if (achou) {
        printf("Cliente excluido!\n");
    } else {
        printf("Cliente nao encontrado.\n");
    }
}
```
**Explicacao:** Cada opcao do menu vira uma funcao que abre o arquivo binario com o modo certo (ab/rb/r+b). A exclusao usa um arquivo temporario e rename, e a alteracao usa fseek para sobrescrever o registro.


#############################################################
# PARTE 2 - BANCO DE DESAFIOS E GABARITOS (GA)
#############################################################

# Banco de desafios com gabarito e palavras-chave

Use `Ctrl + F` para procurar por palavras-chave. Cada desafio tem um padrao de resposta que pode ser adaptado na prova.

## Indice rapido por palavras-chave

- entrada, saida, scanf, printf
- if, else, aprovado, maior, menor, par, impar
- switch, case, menu, do while
- for, while, repeticao, contador, acumulador
- vetor, array, soma vetor, media vetor, maior vetor, menor vetor
- busca, posicao, encontrado, nao encontrado
- ordenacao, ordenar, crescente, troca, aux
- matriz, linha, coluna, soma linha, soma coluna
- diagonal principal, diagonal secundaria, matriz quadrada
- media matriz, total matriz, maior matriz, menor matriz
- funcao, modularizacao, parametro, retorno, void
- vetor parametro, matriz parametro
- string, char, fgets, strlen, strcmp, strcpy, strcat
- remover enter, strcspn
- struct, registro, cadastro, vetor de struct
- aluno, produto, pessoa, hospital, paciente
- ponteiro, endereco, &, *, alterar valor
- dengue, internados, hospitais, meses, repasse

## 1. Entrada e saida simples

Palavras-chave: `scanf`, `printf`, `entrada`, `saida`, `int`, `float`

Desafio: ler idade e altura e imprimir os valores.

Gabarito:

```c
#include <stdio.h>

int main() {
    int idade;
    float altura;

    printf("Idade: ");
    scanf("%d", &idade);

    printf("Altura: ");
    scanf("%f", &altura);

    printf("Idade = %d, altura = %.2f\n", idade, altura);
    return 0;
}
```

## 2. Media de notas com if/else

Palavras-chave: `media`, `aprovado`, `if`, `else`, `nota`

Desafio: ler duas notas, calcular media e dizer aprovado se media >= 7.

Gabarito:

```c
float n1, n2, media;
scanf("%f %f", &n1, &n2);
media = (n1 + n2) / 2.0;

if (media >= 7.0) {
    printf("Aprovado\n");
} else {
    printf("Reprovado\n");
}
```

## 3. Par ou impar

Palavras-chave: `par`, `impar`, `modulo`, `%`, `resto`

Desafio: ler um inteiro e dizer se e par ou impar.

Gabarito:

```c
int n;
scanf("%d", &n);

if (n % 2 == 0) {
    printf("Par\n");
} else {
    printf("Impar\n");
}
```

## 4. Maior entre tres numeros

Palavras-chave: `maior`, `tres numeros`, `if`, `comparacao`

Gabarito:

```c
int a, b, c, maior;
scanf("%d %d %d", &a, &b, &c);

maior = a;
if (b > maior) maior = b;
if (c > maior) maior = c;

printf("Maior = %d\n", maior);
```

## 5. Menu com switch

Palavras-chave: `menu`, `switch`, `case`, `do while`, `opcao`

Desafio: menu com soma, subtracao e sair.

Gabarito:

```c
int opcao, a, b;

do {
    printf("1 Soma\n2 Subtracao\n0 Sair\n");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            scanf("%d %d", &a, &b);
            printf("%d\n", a + b);
            break;
        case 2:
            scanf("%d %d", &a, &b);
            printf("%d\n", a - b);
            break;
        case 0:
            printf("Fim\n");
            break;
        default:
            printf("Invalido\n");
    }
} while (opcao != 0);
```

## 6. Contador com for

Palavras-chave: `for`, `contador`, `repetir`, `1 a 10`

Gabarito:

```c
for (int i = 1; i <= 10; i++) {
    printf("%d\n", i);
}
```

## 7. Acumulador com while

Palavras-chave: `while`, `acumulador`, `soma`, `sentinela`

Desafio: ler numeros ate digitar 0 e somar.

Gabarito:

```c
int n, soma = 0;

printf("Numero: ");
scanf("%d", &n);

while (n != 0) {
    soma += n;
    printf("Numero: ");
    scanf("%d", &n);
}

printf("Soma = %d\n", soma);
```

## 8. Vetor: ler e imprimir

Palavras-chave: `vetor`, `array`, `ler vetor`, `imprimir vetor`

Gabarito:

```c
int v[5];

for (int i = 0; i < 5; i++) {
    scanf("%d", &v[i]);
}

for (int i = 0; i < 5; i++) {
    printf("%d ", v[i]);
}
```

## 9. Vetor: soma e media

Palavras-chave: `soma vetor`, `media vetor`, `acumulador`

Gabarito:

```c
int v[5], soma = 0;
float media;

for (int i = 0; i < 5; i++) {
    scanf("%d", &v[i]);
    soma += v[i];
}

media = soma / 5.0;
printf("Soma = %d, media = %.2f\n", soma, media);
```

## 10. Vetor: maior e menor

Palavras-chave: `maior vetor`, `menor vetor`, `v[0]`

Gabarito:

```c
int maior = v[0];
int menor = v[0];

for (int i = 1; i < n; i++) {
    if (v[i] > maior) maior = v[i];
    if (v[i] < menor) menor = v[i];
}
```

## 11. Vetor: contar pares

Palavras-chave: `contar`, `pares`, `vetor`, `%`

Gabarito:

```c
int cont = 0;

for (int i = 0; i < n; i++) {
    if (v[i] % 2 == 0) {
        cont++;
    }
}

printf("Pares = %d\n", cont);
```

## 12. Vetor: buscar valor

Palavras-chave: `busca`, `buscar`, `encontrado`, `posicao`, `break`

Gabarito:

```c
int procurado, pos = -1;
scanf("%d", &procurado);

for (int i = 0; i < n; i++) {
    if (v[i] == procurado) {
        pos = i;
        break;
    }
}

if (pos != -1) printf("Encontrado na posicao %d\n", pos);
else printf("Nao encontrado\n");
```

## 13. Vetor: ordenar crescente

Palavras-chave: `ordenar`, `ordenacao`, `crescente`, `troca`, `aux`

Gabarito:

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

## 14. Matriz: ler e imprimir

Palavras-chave: `matriz`, `linha`, `coluna`, `for dentro de for`

Gabarito:

```c
int m[3][4];

for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
        scanf("%d", &m[i][j]);
    }
}

for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
        printf("%d ", m[i][j]);
    }
    printf("\n");
}
```

## 15. Matriz: soma total

Palavras-chave: `soma matriz`, `total matriz`, `todos elementos`

Gabarito:

```c
int soma = 0;

for (int i = 0; i < LINHAS; i++) {
    for (int j = 0; j < COLUNAS; j++) {
        soma += m[i][j];
    }
}
```

## 16. Matriz: media geral

Palavras-chave: `media matriz`, `media geral`, `linhas vezes colunas`

Gabarito:

```c
float media = soma / (float)(LINHAS * COLUNAS);
```

## 17. Matriz: soma por linha

Palavras-chave: `soma linha`, `total por linha`, `linha matriz`

Gabarito:

```c
for (int i = 0; i < LINHAS; i++) {
    int somaLinha = 0;
    for (int j = 0; j < COLUNAS; j++) {
        somaLinha += m[i][j];
    }
    printf("Linha %d = %d\n", i, somaLinha);
}
```

## 18. Matriz: soma por coluna

Palavras-chave: `soma coluna`, `total por coluna`, `coluna matriz`

Gabarito:

```c
for (int j = 0; j < COLUNAS; j++) {
    int somaColuna = 0;
    for (int i = 0; i < LINHAS; i++) {
        somaColuna += m[i][j];
    }
    printf("Coluna %d = %d\n", j, somaColuna);
}
```

## 19. Matriz: maior elemento

Palavras-chave: `maior matriz`, `maior elemento`, `m[0][0]`

Gabarito:

```c
int maior = m[0][0];

for (int i = 0; i < LINHAS; i++) {
    for (int j = 0; j < COLUNAS; j++) {
        if (m[i][j] > maior) {
            maior = m[i][j];
        }
    }
}
```

## 20. Matriz quadrada: diagonal principal

Palavras-chave: `diagonal principal`, `matriz quadrada`, `i == j`

Gabarito:

```c
int soma = 0;

for (int i = 0; i < N; i++) {
    soma += m[i][i];
}
```

## 21. Matriz quadrada: diagonal secundaria

Palavras-chave: `diagonal secundaria`, `N - 1 - i`

Gabarito:

```c
int soma = 0;

for (int i = 0; i < N; i++) {
    soma += m[i][N - 1 - i];
}
```

## 22. Funcao simples com retorno

Palavras-chave: `funcao`, `return`, `retorno`, `somar`

Gabarito:

```c
int somar(int a, int b) {
    return a + b;
}
```

Uso:

```c
int r = somar(2, 3);
```

## 23. Funcao void

Palavras-chave: `void`, `procedimento`, `sem retorno`

Gabarito:

```c
void mostrarLinha() {
    printf("----------------\n");
}
```

## 24. Funcao recebendo vetor

Palavras-chave: `vetor parametro`, `funcao vetor`, `int v[]`

Gabarito:

```c
int somarVetor(int v[], int n) {
    int soma = 0;
    for (int i = 0; i < n; i++) {
        soma += v[i];
    }
    return soma;
}
```

## 25. Funcao recebendo matriz

Palavras-chave: `matriz parametro`, `funcao matriz`, `colunas obrigatorias`

Gabarito:

```c
#define LINHAS 6
#define COLUNAS 5

void imprimir(int m[LINHAS][COLUNAS]) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}
```

## 26. String: ler uma palavra

Palavras-chave: `string`, `char`, `scanf`, `palavra`

Gabarito:

```c
char nome[50];
scanf("%49s", nome);
printf("%s\n", nome);
```

## 27. String: ler frase com espaco

Palavras-chave: `fgets`, `frase`, `espaco`, `string`

Gabarito:

```c
char frase[100];
fgets(frase, sizeof(frase), stdin);
```

## 28. String: remover enter do fgets

Palavras-chave: `remover enter`, `strcspn`, `\\n`, `fgets`

Gabarito:

```c
#include <string.h>

frase[strcspn(frase, "\n")] = '\0';
```

## 29. String: comparar

Palavras-chave: `strcmp`, `comparar string`, `igual`, `senha`

Gabarito:

```c
#include <string.h>

if (strcmp(senha, "1234") == 0) {
    printf("Senha correta\n");
} else {
    printf("Senha incorreta\n");
}
```

## 30. String: tamanho

Palavras-chave: `strlen`, `tamanho string`

Gabarito:

```c
printf("Tamanho = %zu\n", strlen(nome));
```

## 31. String: copiar e concatenar

Palavras-chave: `strcpy`, `strcat`, `copiar`, `concatenar`

Gabarito:

```c
char completo[100];
strcpy(completo, nome);
strcat(completo, " ");
strcat(completo, sobrenome);
```

## 32. String: contar uma letra

Palavras-chave: `contar letra`, `string`, `for`, `\\0`

Gabarito:

```c
int cont = 0;

for (int i = 0; texto[i] != '\0'; i++) {
    if (texto[i] == 'a' || texto[i] == 'A') {
        cont++;
    }
}
```

## 33. Struct simples

Palavras-chave: `struct`, `registro`, `typedef`, `aluno`

Gabarito:

```c
typedef struct {
    char nome[50];
    int idade;
    float media;
} Aluno;

Aluno a;
scanf("%49s", a.nome);
scanf("%d", &a.idade);
scanf("%f", &a.media);
```

## 34. Vetor de struct

Palavras-chave: `vetor de struct`, `cadastro`, `lista`, `alunos`

Gabarito:

```c
Aluno turma[30];

for (int i = 0; i < 30; i++) {
    scanf("%49s", turma[i].nome);
    scanf("%f", &turma[i].media);
}
```

## 35. Struct: listar aprovados

Palavras-chave: `struct aprovado`, `listar`, `media`

Gabarito:

```c
for (int i = 0; i < n; i++) {
    if (turma[i].media >= 7.0) {
        printf("%s %.2f\n", turma[i].nome, turma[i].media);
    }
}
```

## 36. Struct: buscar por nome

Palavras-chave: `buscar struct`, `buscar nome`, `strcmp`, `cadastro`

Gabarito:

```c
int pos = -1;

for (int i = 0; i < n; i++) {
    if (strcmp(turma[i].nome, procurado) == 0) {
        pos = i;
        break;
    }
}
```

## 37. Produto com struct e total

Palavras-chave: `produto`, `struct`, `preco`, `quantidade`, `total`

Gabarito:

```c
typedef struct {
    char nome[50];
    float preco;
    int quantidade;
} Produto;

float total = produto.preco * produto.quantidade;
```

## 38. Ponteiro basico para alterar valor

Palavras-chave: `ponteiro`, `alterar valor`, `&`, `*`

Gabarito:

```c
void alterar(int *x) {
    *x = 10;
}

int n = 0;
alterar(&n);
```

## 39. Desafio dengue: totais por mes

Palavras-chave: `dengue`, `internados`, `mes`, `soma linha`, `matriz 6x5`

Gabarito:

```c
for (int mes = 0; mes < 6; mes++) {
    internados[mes] = 0;
    for (int hospital = 0; hospital < 5; hospital++) {
        internados[mes] += matriz[mes][hospital];
    }
}
```

## 40. Desafio dengue: totais por hospital

Palavras-chave: `dengue`, `hospital`, `soma coluna`, `pacientes`

Gabarito:

```c
for (int hospital = 0; hospital < 5; hospital++) {
    pacientes[hospital] = 0;
    for (int mes = 0; mes < 6; mes++) {
        pacientes[hospital] += matriz[mes][hospital];
    }
}
```

## 41. Desafio dengue: repasse

Palavras-chave: `repasse`, `dinheiro`, `1000`, `1200`, `1500`, `pacientes`

Gabarito:

```c
double repasse;

if (total <= 50) {
    repasse = total * 1000.0;
} else if (total <= 100) {
    repasse = total * 1200.0;
} else {
    repasse = total * 1500.0;
}
```

## 42. Desafio dengue: media da matriz

Palavras-chave: `media dengue`, `media matriz`, `retornar main`

Gabarito:

```c
float calcularMedia(int m[6][5]) {
    int soma = 0;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            soma += m[i][j];
        }
    }

    return soma / 30.0;
}
```

## 43. Inicializar vetor com zero

Palavras-chave: `inicializar vetor`, `zero`, `{0}`

Gabarito:

```c
int internados[6] = {0};
int pacientes[5] = {0};
```

## 44. Limpar buffer antes de fgets

Palavras-chave: `scanf fgets`, `buffer`, `getchar`

Gabarito simples:

```c
int c;
while ((c = getchar()) != '\n' && c != EOF) {
}
```

Depois pode usar:

```c
fgets(nome, sizeof(nome), stdin);
```

## 45. Padrao de programa modularizado

Palavras-chave: `modularizado`, `prototipo`, `funcoes antes main`

Gabarito:

```c
#include <stdio.h>

void lerDados();
void processar();
void imprimir();

int main() {
    lerDados();
    processar();
    imprimir();
    return 0;
}

void lerDados() {
}

void processar() {
}

void imprimir() {
}
```

## 46. Ler matriz e gerar vetor auxiliar

Palavras-chave: `matriz para vetor`, `vetor auxiliar`, `totais`

Padrao:

```c
void gerarTotais(int matriz[6][5], int totais[6]) {
    for (int i = 0; i < 6; i++) {
        totais[i] = 0;
        for (int j = 0; j < 5; j++) {
            totais[i] += matriz[i][j];
        }
    }
}
```

## 47. Relatorio formatado

Palavras-chave: `relatorio`, `printf`, `%.2f`, `hospital`

Gabarito:

```c
for (int i = 0; i < 5; i++) {
    printf("Hospital %d - R$ %.2f\n", i, valores[i]);
}
```

## 48. Quando usar cada estrutura

Palavras-chave: `quando usar`, `vetor matriz struct string`

Resposta curta:

- Vetor: varios valores do mesmo tipo em uma dimensao.
- Matriz: tabela com linhas e colunas.
- String: texto, vetor de `char`.
- Struct: registro com campos diferentes.
- Funcao: bloco reutilizavel para organizar o codigo.
- Switch: menu/opcoes.
- For: repeticao com quantidade definida.
- While: repeticao por condicao.
- Do while: menu que deve aparecer pelo menos uma vez.

