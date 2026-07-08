# 04 - FORMULAS E CALCULOS MATEMATICOS

Todas as formulas que podem cair em prova, das basicas as avancadas.

INDICE DESTE ARQUIVO:
  PARTE 1 - Formulas basicas e intermediarias (42 topicos)
  PARTE 2 - Formulas extras e avancadas (30 topicos)


#############################################################
# PARTE 1 - FORMULAS BASICAS E INTERMEDIARIAS
#############################################################

# Formulas e calculos comuns em prova de C

Use `Ctrl+F` por: porcentagem, desconto, aumento, media ponderada, regra de tres, IMC, temperatura, salario, comissao, juros, fatorial, fibonacci, area, perimetro, bhaskara, conversao base, binario, hexadecimal, media harmonica, media geometrica, variancia, desvio padrao, IR, INSS, faixa, regra de tres composta, trapezio, losango, volume, cilindro, esfera, cone, km/h, m/s, media movel, serie harmonica, raiz newton, radiano.

## 1. Operacoes basicas

```c
soma = a + b;
subtracao = a - b;
multiplicacao = a * b;
divisao = a / b;
resto = a % b;
```

Cuidados:

- `%` so funciona com inteiros.
- Para divisao com decimal, use `float`, `double` ou divida por `2.0`, `3.0`.

## 2. Porcentagem

Calcular X por cento de um valor:

```c
resultado = valor * percentual / 100.0;
```

Exemplo: 15% de 200:

```c
resultado = 200 * 15 / 100.0; // 30
```

## 3. Desconto

```c
desconto = preco * percentual / 100.0;
precoFinal = preco - desconto;
```

## 4. Aumento

```c
aumento = salario * percentual / 100.0;
novoSalario = salario + aumento;
```

## 5. Media simples

```c
media = (n1 + n2 + n3) / 3.0;
```

## 6. Media ponderada

```c
media = (n1 * p1 + n2 * p2 + n3 * p3) / (p1 + p2 + p3);
```

## 7. Regra de tres

Se `a` esta para `b`, assim como `c` esta para `x`:

```c
x = b * c / a;
```

Exemplo: 2 produtos custam 10, quanto custam 7?

```c
x = 10 * 7 / 2.0;
```

## 8. IMC

```c
imc = peso / (altura * altura);
```

Classificacao comum:

```c
if (imc < 18.5) printf("Abaixo do peso\n");
else if (imc < 25.0) printf("Normal\n");
else if (imc < 30.0) printf("Sobrepeso\n");
else printf("Obesidade\n");
```

## 9. Temperatura

Celsius para Fahrenheit:

```c
fahrenheit = celsius * 9.0 / 5.0 + 32.0;
```

Fahrenheit para Celsius:

```c
celsius = (fahrenheit - 32.0) * 5.0 / 9.0;
```

## 10. Salario com comissao

```c
comissao = vendas * percentual / 100.0;
salarioFinal = salarioBase + comissao;
```

## 11. Juros simples

```c
juros = capital * taxa * tempo / 100.0;
montante = capital + juros;
```

## 12. Conversao de tempo

Segundos para horas, minutos e segundos:

```c
horas = totalSegundos / 3600;
resto = totalSegundos % 3600;
minutos = resto / 60;
segundos = resto % 60;
```

## 13. Area e perimetro

Retangulo:

```c
area = base * altura;
perimetro = 2 * (base + altura);
```

Circulo:

```c
area = 3.14159 * raio * raio;
perimetro = 2 * 3.14159 * raio;
```

Triangulo:

```c
area = base * altura / 2.0;
```

## 14. Potencia e raiz

Use:

```c
#include <math.h>
```

```c
potencia = pow(base, expoente);
raiz = sqrt(numero);
```

No GCC pode precisar compilar com `-lm`:

```sh
gcc arquivo.c -o programa -lm
```

## 15. Bhaskara

```c
delta = b * b - 4 * a * c;

if (delta < 0) {
    printf("Nao existem raizes reais\n");
} else {
    x1 = (-b + sqrt(delta)) / (2 * a);
    x2 = (-b - sqrt(delta)) / (2 * a);
}
```

## 16. Fatorial

```c
int fat = 1;
for (int i = 1; i <= n; i++) {
    fat *= i;
}
```

## 17. Fibonacci

```c
int a = 0, b = 1, prox;

for (int i = 0; i < n; i++) {
    printf("%d ", a);
    prox = a + b;
    a = b;
    b = prox;
}
```

## 18. Multiplicador com faixa

Exemplo parecido com repasse:

```c
if (qtd <= 50) valor = qtd * 1000.0;
else if (qtd <= 100) valor = qtd * 1200.0;
else valor = qtd * 1500.0;
```

## 19. Minimo, maximo e media em vetor

```c
soma = 0;
maior = v[0];
menor = v[0];

for (int i = 0; i < n; i++) {
    soma += v[i];
    if (v[i] > maior) maior = v[i];
    if (v[i] < menor) menor = v[i];
}

media = soma / (float)n;
```

## 20. Porcentagem de ocorrencias

Exemplo: percentual de aprovados:

```c
percentual = aprovados * 100.0 / total;
```

## 21. Juros composto

Use quando o enunciado fala em capital aplicado por varios periodos com taxa acumulando.

```c
#include <math.h>

montante = capital * pow(1.0 + taxa / 100.0, tempo);
juros = montante - capital;
```

## 22. Distancia, velocidade e tempo

```c
distancia = velocidade * tempo;
velocidade = distancia / tempo;
tempo = distancia / velocidade;
```

## 23. Consumo medio

Exemplo: km por litro.

```c
consumo = distancia / litros;
```

## 24. PA e PG

```c
// PA: soma sempre a razao
termo = primeiro + (n - 1) * razao;

// PG: multiplica pela razao
termo = primeiro * pow(razao, n - 1);
```

## 25. Numero primo

```c
int primo = 1;

if (n < 2) {
    primo = 0;
}

for (int d = 2; d * d <= n; d++) {
    if (n % d == 0) {
        primo = 0;
    }
}
```

## 26. MDC e MMC

```c
while (b != 0) {
    int resto = a % b;
    a = b;
    b = resto;
}

mdc = a;
mmc = (x / mdc) * y;
```

## 27. Triangulo e Pitagoras

Validar se 3 lados formam triangulo:

```c
if (a + b > c && a + c > b && b + c > a) {
    printf("Forma triangulo\n");
}
```

Pitagoras:

```c
hipotenusa = sqrt(cateto1 * cateto1 + cateto2 * cateto2);
```

## 28. Media com condicao

Exemplo: media apenas dos valores positivos.

```c
soma = 0;
contador = 0;

for (int i = 0; i < n; i++) {
    if (v[i] > 0) {
        soma += v[i];
        contador++;
    }
}

if (contador > 0) {
    media = soma / (float)contador;
}
```

## 29. Conversao de base

Decimal para binario (loop com %2 e /2, montando em vetor):

```c
int bin[32];
int i = 0;
int num = 13;

if (num == 0) {
    bin[i++] = 0;
}

while (num > 0) {
    bin[i] = num % 2; // resto 0 ou 1
    num = num / 2;
    i++;
}

// imprime ao contrario (do mais significativo para o menos)
for (int j = i - 1; j >= 0; j--) {
    printf("%d", bin[j]);
}
// 13 -> 1101
```

Binario para decimal (loop multiplicando por potencias de 2):

```c
int bin[4] = {1, 1, 0, 1}; // 1101
int n = 4;
int decimal = 0;
int potencia = 1; // 2^0

for (int i = n - 1; i >= 0; i--) {
    decimal += bin[i] * potencia;
    potencia *= 2;
}
// decimal = 13
```

Decimal para hexadecimal (usando %x no printf):

```c
int num = 255;
printf("%x\n", num);  // ff (minusculo)
printf("%X\n", num);  // FF (maiusculo)
```

Decimal para hexadecimal (loop manual, montando em vetor de chars):

```c
int num = 255;
char hex[32];
int i = 0;
int resto;

if (num == 0) {
    hex[i++] = '0';
}

while (num > 0) {
    resto = num % 16;
    if (resto < 10) {
        hex[i] = resto + '0';      // 0..9
    } else {
        hex[i] = resto - 10 + 'A'; // A..F
    }
    num = num / 16;
    i++;
}

for (int j = i - 1; j >= 0; j--) {
    printf("%c", hex[j]);
}
// 255 -> FF
```

## 30. Media harmonica

Formula geral: `media = n / (1/x1 + 1/x2 + ... + 1/xn)`. Para 2 valores: `2*x*y/(x+y)`.

```c
// Cuidado: nenhum xi pode ser zero (divisao por zero)
double x = 4.0, y = 6.0;
double mediaHarmonica;

if (x != 0.0 && y != 0.0) {
    mediaHarmonica = 2.0 * x * y / (x + y);
} else {
    printf("Valores invalidos para media harmonica\n");
}
// 2*4*6 / (4+6) = 48/10 = 4.8
```

Para N valores em um vetor:

```c
double v[5] = {2.0, 4.0, 6.0, 8.0, 10.0};
int n = 5;
double somaInversos = 0.0;

for (int i = 0; i < n; i++) {
    if (v[i] == 0.0) {
        printf("Erro: valor zero na posicao %d\n", i);
        return 1;
    }
    somaInversos += 1.0 / v[i];
}

double mediaHarmonica = n / somaInversos;
```

## 31. Media geometrica

Raiz enesima do produto: `media = pow(x1*x2*...*xn, 1.0/n)`. Precisa de math.h.

```c
#include <math.h>

double v[5] = {2.0, 4.0, 8.0};
int n = 3;
double produto = 1.0;

for (int i = 0; i < n; i++) {
    produto *= v[i];
}

double mediaGeometrica = pow(produto, 1.0 / n);
// pow(64, 1.0/3) = 4.0
```

Cuidado: se algum valor for negativo e n for par, o produto fica negativo e `pow` pode retornar NaN. Use apenas com valores positivos.

## 32. Variancia e desvio padrao (amostral)

Passos: calcular media, somar `(xi - media)^2`, dividir por `(n-1)`, desvio = `sqrt(variancia)`.

```c
#include <math.h>

double v[5] = {2.0, 4.0, 4.0, 4.0, 5.0};
int n = 5;
double soma = 0.0;

// 1. media
for (int i = 0; i < n; i++) {
    soma += v[i];
}
double media = soma / n;

// 2. soma dos quadrados dos desvios
double somaQuadrados = 0.0;
for (int i = 0; i < n; i++) {
    double desvio = v[i] - media;
    somaQuadrados += desvio * desvio;
}

// 3. variancia amostral (divide por n-1)
double variancia = somaQuadrados / (n - 1);

// 4. desvio padrao
double desvioPadrao = sqrt(variancia);
```

OBS: variancia populacional divide por `n`. Confirmar no enunciado se eh amostral ou populacional.

## 33. Calculo de IR (Imposto de Renda) por faixa

Valores aproximados (podem variar por ano - comentar na prova):

```c
double salario = 3000.0;
double ir = 0.0;

if (salario <= 1903.98) {
    ir = 0.0;                              // isento
} else if (salario <= 2826.65) {
    ir = (salario - 1903.98) * 7.5 / 100.0;
} else if (salario <= 3751.05) {
    ir = (salario - 2826.65) * 15.0 / 100.0;
} else if (salario <= 4664.68) {
    ir = (salario - 3751.05) * 22.5 / 100.0;
} else {
    ir = (salario - 4664.68) * 27.5 / 100.0;
}

printf("IR: %.2f\n", ir);
```

Logica de faixa: cada faixa tributa apenas a parte que ultrapassa o limite anterior (tabela progressiva). Se a prova pedir aliquota efetiva sobre o salario todo:

```c
double aliquotaEfetiva = ir * 100.0 / salario;
```

## 34. Calculo de INSS por faixa

Faixas decrescentes de aliquota (valores aproximados, podem variar):

```c
double salario = 3000.0;
double inss = 0.0;

if (salario <= 1302.00) {
    inss = salario * 7.5 / 100.0;
} else if (salario <= 2571.29) {
    inss = 1302.00 * 7.5 / 100.0 + (salario - 1302.00) * 9.0 / 100.0;
} else if (salario <= 3856.94) {
    inss = 1302.00 * 7.5 / 100.0
         + (2571.29 - 1302.00) * 9.0 / 100.0
         + (salario - 2571.29) * 12.0 / 100.0;
} else if (salario <= 7507.49) {
    inss = 1302.00 * 7.5 / 100.0
         + (2571.29 - 1302.00) * 9.0 / 100.0
         + (3856.94 - 2571.29) * 12.0 / 100.0
         + (salario - 3856.94) * 14.0 / 100.0;
} else {
    // teto: acima do limite paga o valor maximo
    inss = 1302.00 * 7.5 / 100.0
         + (2571.29 - 1302.00) * 9.0 / 100.0
         + (3856.94 - 2571.29) * 12.0 / 100.0
         + (7507.49 - 3856.94) * 14.0 / 100.0;
}

printf("INSS: %.2f\n", inss);
```

## 35. Aumento/desconto em FAIXAS

Diferente de percentual unico: cada faixa recebe um percentual diferente.

```c
double salario, novoSalario;

if (salario <= 1000.0) {
    novoSalario = salario * 1.10;       // aumenta 10%
} else if (salario <= 2000.0) {
    novoSalario = salario * 1.08;       // aumenta 8%
} else {
    novoSalario = salario * 1.05;       // aumenta 5%
}

printf("Novo salario: %.2f\n", novoSalario);
```

Mesma logica para desconto em faixas:

```c
double preco, desconto;

if (preco <= 100.0) {
    desconto = preco * 0.05;            // 5%
} else if (preco <= 500.0) {
    desconto = preco * 0.10;            // 10%
} else {
    desconto = preco * 0.15;            // 15%
}

double precoFinal = preco - desconto;
```

## 36. Regra de tres COMPOSTA

Quando ha mais de uma variavel proporcional.

Diretamente proporcional (mais maquinas e mais horas -> mais pecas):

```c
// 3 maquinas fazem 100 pecas em 5 horas
// 5 maquinas fazem X pecas em 8 horas
// X = (100 * 5 * 8) / (3 * 5) = 4000 / 15

int maq1 = 3, pecas1 = 100, horas1 = 5;
int maq2 = 5, horas2 = 8;
int pecas2;

pecas2 = pecas1 * maq2 * horas2 / (maq1 * horas1);
// pecas2 = 100 * 5 * 8 / (3 * 5) = 266
```

Inversamente proporcional (mais operarios -> menos tempo para o mesmo trabalho):

```c
// 6 operarios fazem o trabalho em 20 dias
// 8 operarios fazem o mesmo trabalho em X dias
// X = (6 * 20) / 8 = 15 dias

int op1 = 6, dias1 = 20;
int op2 = 8;
int dias2;

dias2 = op1 * dias1 / op2;
// dias2 = 6 * 20 / 8 = 15
```

Regra geral: monte a proporcao identificando quais variaveis sao diretas (multiplicam) e quais sao inversas (dividem).

## 37. Areas e volumes adicionais

Trapezio (bases paralelas):

```c
area = (baseMaior + baseMenor) * altura / 2.0;
```

Losango (diagonais):

```c
area = diagonalMaior * diagonalMenor / 2.0;
```

Volume do cubo:

```c
volume = aresta * aresta * aresta; // a^3
```

Volume do paralelepipedo:

```c
volume = a * b * c;
```

Volume do cilindro:

```c
#define PI 3.14159
volume = PI * raio * raio * altura;
```

Volume da esfera:

```c
volume = (4.0 / 3.0) * PI * raio * raio * raio; // 4/3 * pi * r^3
```

Volume do cone:

```c
volume = (1.0 / 3.0) * PI * raio * raio * altura; // 1/3 * pi * r^2 * h
```

Cuidado: `4/3` em inteiros da 1. Sempre use `4.0/3.0` ou `1.0/3.0` para nao perder a parte decimal.

## 38. Conversao de unidades

km/h para m/s (dividir por 3.6):

```c
double kmh = 72.0;
double ms = kmh / 3.6;
// 72 km/h = 20 m/s
```

m/s para km/h (multiplicar por 3.6):

```c
double ms = 20.0;
double kmh = ms * 3.6;
// 20 m/s = 72 km/h
```

Litros para metros cubicos (1 m3 = 1000 L):

```c
double litros = 2500.0;
double m3 = litros / 1000.0;
// 2500 L = 2.5 m3
```

Metros cubicos para litros:

```c
double m3 = 2.5;
double litros = m3 * 1000.0;
```

Outras conversoes uteis:

```c
// km para metros
double metros = km * 1000.0;

// hora para segundos
double segundos = horas * 3600.0;

// polegadas para cm (1 pol = 2.54 cm)
double cm = polegadas * 2.54;
```

## 39. Calculo de media movel

Media dos ultimos N valores de um vetor (janela deslizante).

```c
double v[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int n = 10;
int janela = 3; // media dos ultimos 3

double soma = 0.0;
int inicio = n - janela;

if (inicio < 0) inicio = 0; // se vetor menor que a janela

for (int i = inicio; i < n; i++) {
    soma += v[i];
}

double mediaMovel = soma / (n - inicio);
// media dos ultimos 3: (8+9+10)/3 = 9.0
```

Versao que calcula a media movel para cada posicao (gera um novo vetor):

```c
double v[6] = {2, 4, 6, 8, 10, 12};
int n = 6;
int janela = 3;
double media[6];

for (int i = 0; i < n; i++) {
    double soma = 0.0;
    int count = 0;
    for (int j = i - janela + 1; j <= i; j++) {
        if (j >= 0) {
            soma += v[j];
            count++;
        }
    }
    media[i] = soma / count;
}
// media[2] = (2+4+6)/3 = 4.0
// media[5] = (8+10+12)/3 = 10.0
```

## 40. Soma de serie

Serie harmonica (somatorio de 1/i):

```c
int n = 5;
double soma = 0.0;

for (int i = 1; i <= n; i++) {
    soma += 1.0 / i; // 1 + 1/2 + 1/3 + 1/4 + 1/5
}
// soma = 2.2833...
```

Somatorio de i^2:

```c
int n = 5;
int soma = 0;

for (int i = 1; i <= n; i++) {
    soma += i * i; // 1 + 4 + 9 + 16 + 25 = 55
}
```

Somatorio de i (formula fechada): `n*(n+1)/2`.

```c
int n = 100;
int soma = n * (n + 1) / 2; // 5050
```

Somatorio de i^2 (formula fechada): `n*(n+1)*(2n+1)/6`.

```c
int n = 5;
int soma = n * (n + 1) * (2 * n + 1) / 6; // 55
```

## 41. Raiz quadrada sem math.h

Metodo de Newton (aproximacao iterativa). Use quando a prova nao permitir math.h.

```c
double raizQuadrada(double n) {
    if (n < 0) return -1.0; // erro: raiz de negativo
    if (n == 0.0) return 0.0;

    double chute = n / 2.0;        // estimativa inicial
    double anterior;
    double erro = 0.000001;        // precisao

    do {
        anterior = chute;
        chute = (chute + n / chute) / 2.0;
    } while (chute - anterior > erro || anterior - chute > erro);

    return chute;
}
```

Metodo da biseccao (busca binaria entre 0 e n):

```c
double raizBiseccao(double n) {
    if (n < 0) return -1.0;
    if (n == 0.0 || n == 1.0) return n;

    double baixo = 0.0;
    double alto = n;
    double meio;
    double erro = 0.000001;

    while (alto - baixo > erro) {
        meio = (baixo + alto) / 2.0;
        if (meio * meio < n) {
            baixo = meio;
        } else {
            alto = meio;
        }
    }

    return (baixo + alto) / 2.0;
}
```

## 42. Conversao de angulos

Grafos para radianos: `rad = grau * pi / 180`. Necessario para usar com `sin`, `cos`, `tan` da math.h (elas recebem radianos, nao graus).

```c
#include <math.h>

#define PI 3.141592653589793

double grau = 45.0;
double rad = grau * PI / 180.0;
double seno = sin(rad);
double cosseno = cos(rad);
double tangente = tan(rad);
```

Radianos para graus:

```c
double rad = PI / 4.0; // 0.785...
double grau = rad * 180.0 / PI;
// grau = 45.0
```

Cuidado com `tan` proximo de 90 graus (tende ao infinito). Verifique antes:

```c
if (grau == 90.0 || grau == 270.0) {
    printf("Tangente indefinida\n");
} else {
    printf("tan = %.4f\n", tan(grau * PI / 180.0));
}
```


#############################################################
# PARTE 2 - FORMULAS EXTRAS E AVANCADAS
#############################################################

# FORMULAS EXTRAS - GRAU B (GB) - ALGORITMOS E PROGRAMACAO II (UNIFTEC)

Material de CONSULTA complementar ao FORMULAS_CALCULOS_PROVA_C.md (42 topicos basicos).
Linguagem C. Compila com gcc no Windows. Tudo em portugues, SEM acentos.
Codigos C puro, comentados. Cada topico: enunciado, formula, codigo C, exemplo numerico.

===============================================================================
PALAVRAS-CHAVE (Ctrl+F para buscar)
===============================================================================

folha de pagamento, salario bruto, INSS, IR, imposto de renda, salario liquido,
dependentes, vale transporte, vale alimentacao, desconto, contribuição,
conversao moeda, cambio, dolar, real, euro, taxa cambio,
desconto progressivo, desconto por faixa, desconto por quantidade,
tabela price, PMT, parcela financiamento, emprestimo, juros compostos,
depreciacao linear, depreciacao saldo decrescente, bem, valor residual,
combustivel, consumo, autonomia, km por litro, custo viagem, etanol gasolina,
conversao temperatura, celsius, fahrenheit, kelvin,
velocidade media, aceleracao, MRU, MRUV, movimento retilineo,
densidade, massa especifica,
pressao, forca, area, Pascal,
energia cinetica, energia potencial, trabalho, potencia, Joule, Watt,
probabilidade, combinacao, C(n,p), fatorial combinacao,
permutacao, arranjo, A(n,p),
estatistica, media, mediana, moda, variancia, desvio padrao,
regra de tres direta, regra de tres inversa, proporcao,
area triangulo equilatero, area hexagono, area pentagono, area setor circular,
volume piramide, volume cone, volume tronco de cone,
pitagoras, escada parede, teorema pitagoras,
distancia entre dois pontos, ponto medio, equacao reta,
conversao unidades, comprimento, massa, tempo, area, volume, velocidade,
idade, data nascimento, ano bissexto, bissexto,
dias entre datas, diferenca datas,
numero Armstrong, numero perfeito, capicua, palindromo,
crivo Eratostenes, primos, numeros primos,
conversao decimal octal, base 8,
fatorial grande, long long, double fatorial,
somatorio, produtorio, produto, soma serie.

===============================================================================
INDICE
===============================================================================

01. Folha de pagamento completa
02. Conversao de moedas com taxa
03. Desconto progressivo (faixas)
04. Tabela Price (parcela de financiamento)
05. Depreciacao linear e em saldo decrescente
06. Combustivel: consumo, autonomia, custo, etanol vs gasolina
07. Conversao de temperatura (C, F, K)
08. Velocidade media, aceleracao, MRU, MRUV
09. Densidade e massa especifica
10. Pressao, forca, area (P = F/A)
11. Energia cinetica, potencial, trabalho, potencia
12. Probabilidade simples e combinacao C(n,p)
13. Permutacao e arranjo
14. Estatistica: media, mediana, moda, variancia, desvio padrao
15. Regra de tres direta e inversa
16. Areas: triangulo equilatero, hexagono, pentagono, setor circular
17. Volumes: piramide, cone, tronco de cone
18. Pitagoras aplicado (escada na parede, distancia)
19. Distancia entre dois pontos
20. Ponto medio
21. Equacao da reta
22. Conversao de unidades
23. Idade a partir de data de nascimento
24. Ano bissexto
25. Dias entre duas datas (aproximado)
26. Numero de Armstrong, perfeito, capicua
27. Crivo de Eratostenes (primos ate N)
28. Conversao decimal / octal
29. Fatorial grande (long long / double)
30. Somatorio e produtorio

===============================================================================
01. FOLHA DE PAGAMENTO COMPLETA
===============================================================================

ENUNCIADO: Calcular salario bruto, descontos de INSS, IR, vale transporte e
vale alimentacao, considerando dependentes, e obter o salario liquido.

FORMULAS:
- Salario bruto = valor hora * horas trabalhadas
- INSS (faixas 2024, aproximado):
    ate R$ 1412,00      -> 7,5%
    de 1412,01 a 2666,68 -> 9%
    de 2666,69 a 4000,03 -> 12%
    de 4000,04 a 7786,02 -> 14%
    (teto INSS = 14% sobre 7786,02)
- Base de calculo IR = salario bruto - INSS - (dependentes * 189,59)
- IR (tabela 2024, aproximado):
    ate 2259,20          -> isento
    de 2259,21 a 2826,65 -> 7,5%  - deduzir 169,44
    de 2826,66 a 3751,05 -> 15%   - deduzir 381,44
    de 3751,06 a 4664,68 -> 22,5% - deduzir 662,77
    acima de 4664,68     -> 27,5% - deduzir 896,00
- Vale transporte = 6% do salario bruto (se o funcionario optar)
- Vale alimentacao = valor fixo por dia trabalhado (ex: R$ 30/dia)
- Salario liquido = bruto - INSS - IR - vale transporte + vale alimentacao
  (vale alimentacao geralmente e beneficio, nao desconto)

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float valorHora, horasTrab, bruto, inss, baseIR, ir, liquido;
    int dependentes, diasTrab, optaVT;
    float valorVA = 30.0f;   // vale alimentacao por dia
    float valorPorDep = 189.59f;

    printf("Valor da hora: ");
    scanf("%f", &valorHora);
    printf("Horas trabalhadas no mes: ");
    scanf("%f", &horasTrab);
    printf("Numero de dependentes: ");
    scanf("%d", &dependentes);
    printf("Dias trabalhados (vale alimentacao): ");
    scanf("%d", &diasTrab);
    printf("Opta pelo vale transporte? (1=sim, 0=nao): ");
    scanf("%d", &optaVT);

    // Salario bruto
    bruto = valorHora * horasTrab;

    // INSS por faixa (aproximado 2024)
    if (bruto <= 1412.00f)
        inss = bruto * 0.075f;
    else if (bruto <= 2666.68f)
        inss = 1412.00f * 0.075f + (bruto - 1412.00f) * 0.09f;
    else if (bruto <= 4000.03f)
        inss = 1412.00f * 0.075f + (2666.68f - 1412.00f) * 0.09f
             + (bruto - 2666.68f) * 0.12f;
    else if (bruto <= 7786.02f)
        inss = 1412.00f * 0.075f + (2666.68f - 1412.00f) * 0.09f
             + (4000.03f - 2666.68f) * 0.12f
             + (bruto - 4000.03f) * 0.14f;
    else
        inss = 1412.00f * 0.075f + (2666.68f - 1412.00f) * 0.09f
             + (4000.03f - 2666.68f) * 0.12f
             + (7786.02f - 4000.03f) * 0.14f;  // teto

    // Base de calculo do IR
    baseIR = bruto - inss - (dependentes * valorPorDep);
    if (baseIR < 0) baseIR = 0;

    // IR por faixa (aproximado 2024)
    if (baseIR <= 2259.20f)
        ir = 0;
    else if (baseIR <= 2826.65f)
        ir = baseIR * 0.075f - 169.44f;
    else if (baseIR <= 3751.05f)
        ir = baseIR * 0.15f - 381.44f;
    else if (baseIR <= 4664.68f)
        ir = baseIR * 0.225f - 662.77f;
    else
        ir = baseIR * 0.275f - 896.00f;
    if (ir < 0) ir = 0;

    // Vale transporte (6% do bruto)
    float vt = 0;
    if (optaVT == 1) {
        vt = bruto * 0.06f;
    }

    // Vale alimentacao (beneficio, nao desconta)
    float va = diasTrab * valorVA;

    // Salario liquido
    liquido = bruto - inss - ir - vt;

    printf("\n--- FOLHA DE PAGAMENTO ---\n");
    printf("Salario bruto:        R$ %.2f\n", bruto);
    printf("INSS:                 R$ %.2f\n", inss);
    printf("Base calc. IR:        R$ %.2f\n", baseIR);
    printf("IR:                   R$ %.2f\n", ir);
    printf("Vale transporte:      R$ %.2f\n", vt);
    printf("Vale alimentacao:     R$ %.2f (beneficio)\n", va);
    printf("Salario liquido:      R$ %.2f\n", liquido);
    return 0;
}
```

EXEMPLO NUMERICO:
- valorHora = 30, horasTrab = 220, dependentes = 1, diasTrab = 22, optaVT = 1
- bruto = 30 * 220 = 6600,00
- INSS: 4000,03 esta na faixa de 14%
  inss = 1412*0,075 + 1254,68*0,09 + 1333,35*0,12 + (6600-4000,03)*0,14
       = 105,90 + 112,92 + 160,00 + 363,99 = 742,81
- baseIR = 6600 - 742,81 - 189,59 = 5667,60
- IR = 5667,60 * 0,275 - 896,00 = 1558,59 - 896,00 = 662,59
- VT = 6600 * 0,06 = 396,00
- VA = 22 * 30 = 660,00 (beneficio)
- liquido = 6600 - 742,81 - 662,59 - 396,00 = 4798,60

===============================================================================
02. CONVERSAO DE MOEDAS COM TAXA
===============================================================================

ENUNCIADO: Converter um valor entre moedas (real, dolar, euro) aplicando uma
taxa de cambio e, opcionalmente, uma taxa de operacao (spread) do banco.

FORMULAS:
- valorConvertido = valorOrigem * taxaCambio
- com spread: valorConvertido = valorOrigem * taxaCambio * (1 - spread/100)
- inverso: valorOrigem = valorConvertido / taxaCambio

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float valor, taxa, spread, resultado;
    int opcao;

    printf("Converter:\n");
    printf("1 - Real para Dolar\n");
    printf("2 - Dolar para Real\n");
    printf("3 - Real para Euro\n");
    printf("4 - Euro para Real\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    printf("Valor a converter: ");
    scanf("%f", &valor);
    printf("Taxa de cambio: ");
    scanf("%f", &taxa);
    printf("Spread do banco (%%): ");
    scanf("%f", &spread);

    switch (opcao) {
        case 1: // Real -> Dolar (divide pela taxa)
            resultado = valor / taxa * (1 - spread / 100.0f);
            printf("R$ %.2f = US$ %.2f\n", valor, resultado);
            break;
        case 2: // Dolar -> Real (multiplica pela taxa)
            resultado = valor * taxa * (1 - spread / 100.0f);
            printf("US$ %.2f = R$ %.2f\n", valor, resultado);
            break;
        case 3: // Real -> Euro
            resultado = valor / taxa * (1 - spread / 100.0f);
            printf("R$ %.2f = EUR %.2f\n", valor, resultado);
            break;
        case 4: // Euro -> Real
            resultado = valor * taxa * (1 - spread / 100.0f);
            printf("EUR %.2f = R$ %.2f\n", valor, resultado);
            break;
        default:
            printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- Converter R$ 1000 para dolar, taxa = 5,20, spread = 2%
- resultado = 1000 / 5,20 * (1 - 0,02) = 192,31 * 0,98 = 188,46
- R$ 1000,00 = US$ 188,46

===============================================================================
03. DESCONTO PROGRESSIVO (MAIS COMPRA, MAIS DESCONTO)
===============================================================================

ENUNCIADO: Aplicar desconto por faixas de quantidade comprada. Quanto mais
unidades, maior o percentual de desconto sobre o valor total.

FORMULAS:
- valorBruto = precoUnitario * quantidade
- desconto por faixa:
    ate 10 unidades   -> 0%
    de 11 a 50        -> 5%
    de 51 a 100       -> 10%
    acima de 100      -> 15%
- valorDesconto = valorBruto * percentual
- valorFinal = valorBruto - valorDesconto

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float precoUnit, valorBruto, desconto, valorFinal;
    int quantidade;
    float perc;

    printf("Preco unitario: ");
    scanf("%f", &precoUnit);
    printf("Quantidade: ");
    scanf("%d", &quantidade);

    valorBruto = precoUnit * quantidade;

    // Desconto progressivo por faixa de quantidade
    if (quantidade <= 10)
        perc = 0.0f;
    else if (quantidade <= 50)
        perc = 0.05f;
    else if (quantidade <= 100)
        perc = 0.10f;
    else
        perc = 0.15f;

    desconto = valorBruto * perc;
    valorFinal = valorBruto - desconto;

    printf("Valor bruto:   R$ %.2f\n", valorBruto);
    printf("Desconto (%.0f%%): R$ %.2f\n", perc * 100, desconto);
    printf("Valor final:   R$ %.2f\n", valorFinal);
    return 0;
}
```

EXEMPLO NUMERICO:
- precoUnit = 50, quantidade = 75
- valorBruto = 50 * 75 = 3750,00
- faixa 51-100 -> 10%
- desconto = 3750 * 0,10 = 375,00
- valorFinal = 3750 - 375 = 3375,00

===============================================================================
04. TABELA PRICE (PARCELA DE FINANCIAMENTO)
===============================================================================

ENUNCIADO: Calcular a parcela (PMT) de um financiamento pelo sistema Price
(prestacoes fixas), dado o valor presente (PV), taxa de juros (i) e numero
de parcelas (n).

FORMULA:
- PMT = PV * i / (1 - (1 + i)^-n)
  onde:
    PV = valor presente (valor financiado)
    i  = taxa de juros por periodo (em decimal, ex: 1% = 0,01)
    n  = numero de parcelas

CODIGO C:

```c
#include <stdio.h>
#include <math.h>  // usa pow()

int main(void) {
    double PV, i, n, PMT;
    double totalPago, jurosTotal;

    printf("Valor financiado (PV): ");
    scanf("%lf", &PV);
    printf("Taxa de juros ao mes (%%): ");
    scanf("%lf", &i);
    printf("Numero de parcelas (n): ");
    scanf("%lf", &n);

    // converte taxa de % para decimal
    i = i / 100.0;

    // Formula da Tabela Price
    PMT = PV * i / (1 - pow(1 + i, -n));

    totalPago = PMT * n;
    jurosTotal = totalPago - PV;

    printf("\n--- FINANCIAMENTO ---\n");
    printf("Valor financiado: R$ %.2f\n", PV);
    printf("Parcela mensal:   R$ %.2f\n", PMT);
    printf("Total pago:       R$ %.2f\n", totalPago);
    printf("Juros totais:    R$ %.2f\n", jurosTotal);
    return 0;
}
```

OBS: compilar com -lm no Linux: gcc arquivo.c -o prog -lm
No Windows (gcc/MinGW) geralmente nao precisa do -lm.

EXEMPLO NUMERICO:
- PV = 10000, i = 1,5% ao mes, n = 12
- i = 0,015
- PMT = 10000 * 0,015 / (1 - (1,015)^-12)
- (1,015)^-12 = 1 / 1,19562 = 0,83639
- PMT = 150 / (1 - 0,83639) = 150 / 0,16361 = 916,80
- totalPago = 916,80 * 12 = 11001,60
- jurosTotal = 11001,60 - 10000 = 1001,60

===============================================================================
05. DEPRECIACAO LINEAR E EM SALDO DECRESCENTE
===============================================================================

ENUNCIADO: Calcular a depreciacao de um bem pelo metodo linear e pelo metodo
em saldo decrescente (depreciacao acelerada).

FORMULAS:
- Linear:
    depreciacaoAnual = (valorInicial - valorResidual) / vidaUtil
    valorAnoK = valorInicial - K * depreciacaoAnual
- Saldo decrescente (taxa = 2 * (1/vidaUtil)):
    taxa = 2 / vidaUtil   (metodo do dobro do saldo decrescente)
    depreciacaoAnoK = valorAtual * taxa
    valorAnoK = valorAnoK-1 - depreciacaoAnoK

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float valorInicial, valorResidual, vidaUtil;
    int metodo, ano, k;

    printf("Valor inicial do bem: ");
    scanf("%f", &valorInicial);
    printf("Valor residual: ");
    scanf("%f", &valorResidual);
    printf("Vida util (anos): ");
    scanf("%f", &vidaUtil);
    printf("Metodo (1=linear, 2=saldo decrescente): ");
    scanf("%d", &metodo);

    if (metodo == 1) {
        // Metodo linear
        float depAnual = (valorInicial - valorResidual) / vidaUtil;
        printf("\n--- DEPRECIACAO LINEAR ---\n");
        printf("Depreciacao anual: R$ %.2f\n", depAnual);
        float valorAtual = valorInicial;
        for (ano = 1; ano <= (int)vidaUtil; ano++) {
            valorAtual -= depAnual;
            printf("Ano %d: depreciacao R$ %.2f | valor contabil R$ %.2f\n",
                   ano, depAnual, valorAtual);
        }
    } else if (metodo == 2) {
        // Metodo do dobro do saldo decrescente
        float taxa = 2.0f / vidaUtil;
        printf("\n--- DEPRECIACAO SALDO DECRESCENTE ---\n");
        printf("Taxa anual: %.2f%%\n", taxa * 100);
        float valorAtual = valorInicial;
        for (ano = 1; ano <= (int)vidaUtil; ano++) {
            float dep = valorAtual * taxa;
            if (valorAtual - dep < valorResidual) {
                dep = valorAtual - valorResidual;
            }
            valorAtual -= dep;
            printf("Ano %d: depreciacao R$ %.2f | valor contabil R$ %.2f\n",
                   ano, dep, valorAtual);
        }
    } else {
        printf("Metodo invalido\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- valorInicial = 50000, valorResidual = 5000, vidaUtil = 5
- Linear: depAnual = (50000 - 5000) / 5 = 9000/ano
  Ano 1: 50000 - 9000 = 41000
  Ano 2: 32000 ... Ano 5: 5000
- Saldo decrescente: taxa = 2/5 = 40%
  Ano 1: dep = 50000*0,4 = 20000, valor = 30000
  Ano 2: dep = 30000*0,4 = 12000, valor = 18000
  Ano 3: dep = 18000*0,4 = 7200, valor = 10800
  Ano 4: dep = 10800*0,4 = 4320, valor = 6480
  Ano 5: dep = 6480 - 5000 = 1480 (limita ao residual), valor = 5000

===============================================================================
06. COMBUSTIVEL: CONSUMO, AUTONOMIA, CUSTO VIAGEM, ETANOL VS GASOLINA
===============================================================================

ENUNCIADO: Calcular consumo (km/L), autonomia, custo de uma viagem e decidir
se compensa abastecer com etanol ou gasolina (razao 0,7).

FORMULAS:
- consumo = km percorridos / litros consumidos
- autonomia = consumo * litros no tanque
- custoViagem = (distancia / consumo) * precoLitro
- razao etanol/gasolina = precoEtanol / precoGasolina
    se razao <= 0,70 -> etanol compensa
    se razao > 0,70  -> gasolina compensa

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float km, litros, consumo, autonomia, tanque;
    float distancia, precoLitro, custo;
    float precoEtanol, precoGasolina, razao;

    // Consumo e autonomia
    printf("KM percorridos: ");
    scanf("%f", &km);
    printf("Litros consumidos: ");
    scanf("%f", &litros);
    consumo = km / litros;
    printf("Consumo: %.2f km/L\n", consumo);

    printf("Litros no tanque: ");
    scanf("%f", &tanque);
    autonomia = consumo * tanque;
    printf("Autonomia: %.2f km\n", autonomia);

    // Custo da viagem
    printf("Distancia da viagem (km): ");
    scanf("%f", &distancia);
    printf("Preco do litro (R$): ");
    scanf("%f", &precoLitro);
    custo = (distancia / consumo) * precoLitro;
    printf("Custo da viagem: R$ %.2f\n", custo);

    // Etanol vs gasolina
    printf("Preco do etanol (R$): ");
    scanf("%f", &precoEtanol);
    printf("Preco da gasolina (R$): ");
    scanf("%f", &precoGasolina);
    razao = precoEtanol / precoGasolina;
    printf("Razao etanol/gasolina: %.3f\n", razao);
    if (razao <= 0.70f)
        printf("Etanol compensa\n");
    else
        printf("Gasolina compensa\n");
    return 0;
}
```

EXEMPLO NUMERICO:
- km = 400, litros = 32 -> consumo = 12,5 km/L
- tanque = 45 -> autonomia = 12,5 * 45 = 562,5 km
- distancia = 600, precoLitro = 5,80
  custo = (600 / 12,5) * 5,80 = 48 * 5,80 = 278,40
- precoEtanol = 4,20, precoGasolina = 6,00
  razao = 4,20 / 6,00 = 0,70 -> etanol compensa (no limite)

===============================================================================
07. CONVERSAO DE TEMPERATURA (C, F, K)
===============================================================================

ENUNCIADO: Converter temperaturas entre Celsius, Fahrenheit e Kelvin.

FORMULAS:
- C -> F:  F = C * 9/5 + 32
- F -> C:  C = (F - 32) * 5/9
- C -> K:  K = C + 273,15
- K -> C:  C = K - 273,15
- F -> K:  K = (F - 32) * 5/9 + 273,15
- K -> F:  F = (K - 273,15) * 9/5 + 32

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float temp, resultado;
    int opcao;

    printf("Converter:\n");
    printf("1 - Celsius para Fahrenheit\n");
    printf("2 - Fahrenheit para Celsius\n");
    printf("3 - Celsius para Kelvin\n");
    printf("4 - Kelvin para Celsius\n");
    printf("5 - Fahrenheit para Kelvin\n");
    printf("6 - Kelvin para Fahrenheit\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    printf("Temperatura: ");
    scanf("%f", &temp);

    switch (opcao) {
        case 1:
            resultado = temp * 9.0f / 5.0f + 32;
            printf("%.2f C = %.2f F\n", temp, resultado);
            break;
        case 2:
            resultado = (temp - 32) * 5.0f / 9.0f;
            printf("%.2f F = %.2f C\n", temp, resultado);
            break;
        case 3:
            resultado = temp + 273.15f;
            printf("%.2f C = %.2f K\n", temp, resultado);
            break;
        case 4:
            resultado = temp - 273.15f;
            printf("%.2f K = %.2f C\n", temp, resultado);
            break;
        case 5:
            resultado = (temp - 32) * 5.0f / 9.0f + 273.15f;
            printf("%.2f F = %.2f K\n", temp, resultado);
            break;
        case 6:
            resultado = (temp - 273.15f) * 9.0f / 5.0f + 32;
            printf("%.2f K = %.2f F\n", temp, resultado);
            break;
        default:
            printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- 100 C -> F = 100 * 9/5 + 32 = 212 F
- 32 F -> C = (32 - 32) * 5/9 = 0 C
- 25 C -> K = 25 + 273,15 = 298,15 K
- 300 K -> C = 300 - 273,15 = 26,85 C

===============================================================================
08. VELOCIDADE MEDIA, ACELERACAO, MRU, MRUV
===============================================================================

ENUNCIADO: Calcular velocidade media, aceleracao e aplicar as equacoes do
movimento retilineo uniforme (MRU) e uniformemente variado (MRUV).

FORMULAS:
- Velocidade media:    Vm = deltaS / deltaT
- Aceleracao:          a = deltaV / deltaT
- MRU (velocidade constante): S = S0 + V * t
- MRUV:
    V = V0 + a * t
    S = S0 + V0 * t + (a * t^2) / 2
    Torricelli: V^2 = V0^2 + 2 * a * deltaS

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float s0, v0, v, a, t, s, deltaS;
    int opcao;

    printf("1 - Velocidade media\n");
    printf("2 - Aceleracao\n");
    printf("3 - MRU (posicao final)\n");
    printf("4 - MRUV (velocidade final)\n");
    printf("5 - MRUV (posicao final)\n");
    printf("6 - Torricelli (velocidade final)\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Variacao de espaco (m): ");
            scanf("%f", &deltaS);
            printf("Variacao de tempo (s): ");
            scanf("%f", &t);
            printf("Velocidade media: %.2f m/s\n", deltaS / t);
            break;
        case 2:
            printf("Variacao de velocidade (m/s): ");
            scanf("%f", &v);
            printf("Variacao de tempo (s): ");
            scanf("%f", &t);
            printf("Aceleracao: %.2f m/s^2\n", v / t);
            break;
        case 3:
            printf("Posicao inicial (m): ");
            scanf("%f", &s0);
            printf("Velocidade (m/s): ");
            scanf("%f", &v);
            printf("Tempo (s): ");
            scanf("%f", &t);
            printf("Posicao final: %.2f m\n", s0 + v * t);
            break;
        case 4:
            printf("Velocidade inicial (m/s): ");
            scanf("%f", &v0);
            printf("Aceleracao (m/s^2): ");
            scanf("%f", &a);
            printf("Tempo (s): ");
            scanf("%f", &t);
            printf("Velocidade final: %.2f m/s\n", v0 + a * t);
            break;
        case 5:
            printf("Posicao inicial (m): ");
            scanf("%f", &s0);
            printf("Velocidade inicial (m/s): ");
            scanf("%f", &v0);
            printf("Aceleracao (m/s^2): ");
            scanf("%f", &a);
            printf("Tempo (s): ");
            scanf("%f", &t);
            printf("Posicao final: %.2f m\n", s0 + v0 * t + (a * t * t) / 2);
            break;
        case 6:
            printf("Velocidade inicial (m/s): ");
            scanf("%f", &v0);
            printf("Aceleracao (m/s^2): ");
            scanf("%f", &a);
            printf("Variacao de espaco (m): ");
            scanf("%f", &deltaS);
            v = v0 * v0 + 2 * a * deltaS;
            if (v < 0) v = -v;  // raiz de valor absoluto
            // sqrt sem math.h: aproximacao simples
            float raiz = v / 2;
            int i;
            for (i = 0; i < 20; i++) {
                raiz = (raiz + v / raiz) / 2;  // metodo de Newton
            }
            printf("Velocidade final: %.2f m/s\n", raiz);
            break;
        default:
            printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- Velocidade media: deltaS = 100 m, t = 20 s -> Vm = 5 m/s
- Aceleracao: deltaV = 30 m/s, t = 6 s -> a = 5 m/s^2
- MRU: S0 = 10, V = 5, t = 4 -> S = 10 + 5*4 = 30 m
- MRUV velocidade: V0 = 0, a = 2, t = 5 -> V = 0 + 2*5 = 10 m/s
- MRUV posicao: S0 = 0, V0 = 0, a = 2, t = 5 -> S = 0 + 0 + (2*25)/2 = 25 m
- Torricelli: V0 = 0, a = 2, deltaS = 100 -> V^2 = 0 + 400 -> V = 20 m/s

===============================================================================
09. DENSIDADE E MASSA ESPECIFICA
===============================================================================

ENUNCIADO: Calcular a densidade de um corpo a partir de sua massa e volume,
ou descobrir massa/volume conhecendo os outros dois.

FORMULAS:
- densidade = massa / volume
- massa = densidade * volume
- volume = massa / densidade

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float massa, volume, densidade;
    int opcao;

    printf("1 - Calcular densidade\n");
    printf("2 - Calcular massa\n");
    printf("3 - Calcular volume\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Massa (kg): ");
            scanf("%f", &massa);
            printf("Volume (m^3): ");
            scanf("%f", &volume);
            densidade = massa / volume;
            printf("Densidade: %.2f kg/m^3\n", densidade);
            break;
        case 2:
            printf("Densidade (kg/m^3): ");
            scanf("%f", &densidade);
            printf("Volume (m^3): ");
            scanf("%f", &volume);
            massa = densidade * volume;
            printf("Massa: %.2f kg\n", massa);
            break;
        case 3:
            printf("Massa (kg): ");
            scanf("%f", &massa);
            printf("Densidade (kg/m^3): ");
            scanf("%f", &densidade);
            volume = massa / densidade;
            printf("Volume: %.2f m^3\n", volume);
            break;
        default:
            printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- massa = 500 kg, volume = 0,5 m^3 -> densidade = 1000 kg/m^3 (agua)
- densidade = 7800 kg/m^3 (ferro), volume = 0,1 m^3 -> massa = 780 kg
- massa = 2700 kg (aluminio), densidade = 2700 -> volume = 1 m^3

===============================================================================
10. PRESSAO, FORCA, AREA (P = F/A)
===============================================================================

ENUNCIADO: Calcular pressao a partir de forca e area, ou descobrir forca/area.

FORMULAS:
- pressao = forca / area     (unidade SI: Pascal = N/m^2)
- forca = pressao * area
- area = forca / pressao

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float forca, area, pressao;
    int opcao;

    printf("1 - Calcular pressao\n");
    printf("2 - Calcular forca\n");
    printf("3 - Calcular area\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Forca (N): ");
            scanf("%f", &forca);
            printf("Area (m^2): ");
            scanf("%f", &area);
            pressao = forca / area;
            printf("Pressao: %.2f Pa\n", pressao);
            break;
        case 2:
            printf("Pressao (Pa): ");
            scanf("%f", &pressao);
            printf("Area (m^2): ");
            scanf("%f", &area);
            forca = pressao * area;
            printf("Forca: %.2f N\n", forca);
            break;
        case 3:
            printf("Forca (N): ");
            scanf("%f", &forca);
            printf("Pressao (Pa): ");
            scanf("%f", &pressao);
            area = forca / pressao;
            printf("Area: %.2f m^2\n", area);
            break;
        default:
            printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- forca = 100 N, area = 2 m^2 -> pressao = 50 Pa
- pressao = 101325 Pa (1 atm), area = 1 m^2 -> forca = 101325 N
- forca = 500 N, pressao = 1000 Pa -> area = 0,5 m^2

===============================================================================
11. ENERGIA CINETICA, POTENCIAL, TRABALHO, POTENCIA
===============================================================================

ENUNCIADO: Calcular energia cinetica, energia potencial gravitacional,
trabalho de uma forca e potencia.

FORMULAS:
- Energia cinetica:      Ec = (m * v^2) / 2
- Energia potencial:     Ep = m * g * h   (g = 9,81 m/s^2)
- Trabalho:              T = F * d * cos(theta)
- Potencia:              P = T / t   (ou P = F * v)

CODIGO C:

```c
#include <stdio.h>
#define G 9.81f

int main(void) {
    float m, v, h, forca, d, cosT, t, potencia;
    int opcao;

    printf("1 - Energia cinetica\n");
    printf("2 - Energia potencial\n");
    printf("3 - Trabalho\n");
    printf("4 - Potencia\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Massa (kg): ");
            scanf("%f", &m);
            printf("Velocidade (m/s): ");
            scanf("%f", &v);
            printf("Energia cinetica: %.2f J\n", (m * v * v) / 2);
            break;
        case 2:
            printf("Massa (kg): ");
            scanf("%f", &m);
            printf("Altura (m): ");
            scanf("%f", &h);
            printf("Energia potencial: %.2f J\n", m * G * h);
            break;
        case 3:
            printf("Forca (N): ");
            scanf("%f", &forca);
            printf("Distancia (m): ");
            scanf("%f", &d);
            printf("Cosseno do angulo: ");
            scanf("%f", &cosT);
            printf("Trabalho: %.2f J\n", forca * d * cosT);
            break;
        case 4:
            printf("Trabalho (J): ");
            scanf("%f", &forca);  // reutiliza variavel como trabalho
            printf("Tempo (s): ");
            scanf("%f", &t);
            printf("Potencia: %.2f W\n", forca / t);
            break;
        default:
            printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- Ec: m = 2 kg, v = 10 m/s -> Ec = (2 * 100)/2 = 100 J
- Ep: m = 5 kg, h = 10 m -> Ep = 5 * 9,81 * 10 = 490,5 J
- Trabalho: F = 50 N, d = 4 m, cos(0) = 1 -> T = 50 * 4 * 1 = 200 J
- Potencia: T = 200 J, t = 10 s -> P = 20 W

===============================================================================
12. PROBABILIDADE SIMPLES E COMBINACAO C(n,p)
===============================================================================

ENUNCIADO: Calcular a probabilidade de um evento simples e o numero de
combinacoes possiveis de n elementos tomados p a p.

FORMULAS:
- Probabilidade simples: P(A) = casosFavoraveis / casosPossiveis
- Combinacao: C(n,p) = n! / (p! * (n-p)!)

CODIGO C:

```c
#include <stdio.h>

// funcao fatorial usando long long
long long fatorial(int n) {
    long long f = 1;
    int i;
    for (i = 2; i <= n; i++) {
        f *= i;
    }
    return f;
}

int main(void) {
    int opcao, fav, poss, n, p;
    long long comb;

    printf("1 - Probabilidade simples\n");
    printf("2 - Combinacao C(n,p)\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        printf("Casos favoraveis: ");
        scanf("%d", &fav);
        printf("Casos possiveis: ");
        scanf("%d", &poss);
        printf("Probabilidade: %.4f = %.2f%%\n",
               (float)fav / poss, (float)fav / poss * 100);
    } else if (opcao == 2) {
        printf("n (total): ");
        scanf("%d", &n);
        printf("p (tomados): ");
        scanf("%d", &p);
        if (p > n || p < 0) {
            printf("Invalido: p deve ser entre 0 e n\n");
        } else {
            comb = fatorial(n) / (fatorial(p) * fatorial(n - p));
            printf("C(%d,%d) = %lld\n", n, p, comb);
        }
    } else {
        printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- Probabilidade: fav = 3, poss = 12 -> P = 0,25 = 25%
- Combinacao: n = 10, p = 3 -> C(10,3) = 10!/(3!*7!) = 3628800/(6*5040) = 120

===============================================================================
13. PERMUTACAO E ARRANJO
===============================================================================

ENUNCIADO: Calcular permutacao simples e arranjo de n elementos tomados p a p.

FORMULAS:
- Permutacao simples: P(n) = n!
- Arranjo: A(n,p) = n! / (n-p)!

CODIGO C:

```c
#include <stdio.h>

long long fatorial(int n) {
    long long f = 1;
    int i;
    for (i = 2; i <= n; i++) {
        f *= i;
    }
    return f;
}

int main(void) {
    int opcao, n, p;

    printf("1 - Permutacao simples P(n)\n");
    printf("2 - Arranjo A(n,p)\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        printf("n: ");
        scanf("%d", &n);
        printf("P(%d) = %lld\n", n, fatorial(n));
    } else if (opcao == 2) {
        printf("n: ");
        scanf("%d", &n);
        printf("p: ");
        scanf("%d", &p);
        if (p > n || p < 0) {
            printf("Invalido\n");
        } else {
            printf("A(%d,%d) = %lld\n", n, p,
                   fatorial(n) / fatorial(n - p));
        }
    } else {
        printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- Permutacao: n = 5 -> P(5) = 5! = 120
- Arranjo: n = 10, p = 3 -> A(10,3) = 10!/7! = 720

===============================================================================
14. ESTATISTICA: MEDIA, MEDIANA, MODA, VARIANCIA, DESVIO PADRAO
===============================================================================

ENUNCIADO: Dado um vetor de numeros, calcular media, mediana, moda, variancia
e desvio padrao (amostral).

FORMULAS:
- media = soma / n
- mediana = valor central (se n impar) ou media dos dois centrais (se n par)
- moda = valor que mais se repete
- variancia = soma((xi - media)^2) / (n - 1)   (amostral)
- desvio padrao = raiz(variancia)

CODIGO C:

```c
#include <stdio.h>

#define MAX 100

// funcao para ordenar o vetor (bubble sort)
void ordenar(float v[], int n) {
    int i, j;
    float aux;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (v[j] > v[j + 1]) {
                aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
        }
    }
}

int main(void) {
    float v[MAX], soma = 0, media, mediana, variancia = 0, desvio;
    int n, i, j;

    printf("Quantos numeros (max %d)? ", MAX);
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("v[%d]: ", i);
        scanf("%f", &v[i]);
        soma += v[i];
    }

    // Media
    media = soma / n;

    // Mediana (precisa ordenar)
    ordenar(v, n);
    if (n % 2 == 1) {
        mediana = v[n / 2];
    } else {
        mediana = (v[n / 2 - 1] + v[n / 2]) / 2;
    }

    // Moda
    float moda = v[0];
    int contModa = 1, contAtual;
    for (i = 0; i < n; i++) {
        contAtual = 1;
        for (j = i + 1; j < n; j++) {
            if (v[i] == v[j]) {
                contAtual++;
            } else {
                break;  // vetor ordenado, para de contar
            }
        }
        if (contAtual > contModa) {
            contModa = contAtual;
            moda = v[i];
        }
    }

    // Variancia amostral
    for (i = 0; i < n; i++) {
        variancia += (v[i] - media) * (v[i] - media);
    }
    variancia = variancia / (n - 1);

    // Desvio padrao (raiz por Newton)
    desvio = variancia / 2;
    for (i = 0; i < 30; i++) {
        desvio = (desvio + variancia / desvio) / 2;
    }

    printf("\n--- ESTATISTICA ---\n");
    printf("Media:        %.2f\n", media);
    printf("Mediana:      %.2f\n", mediana);
    printf("Moda:         %.2f (aparece %d vezes)\n", moda, contModa);
    printf("Variancia:    %.2f\n", variancia);
    printf("Desvio padrao: %.2f\n", desvio);
    return 0;
}
```

EXEMPLO NUMERICO:
- v = {2, 4, 4, 4, 5, 5, 7, 9}, n = 8
- soma = 40, media = 5,0
- ordenado: 2,4,4,4,5,5,7,9 -> mediana = (4+5)/2 = 4,5
- moda = 4 (aparece 3 vezes)
- variancia = ((2-5)^2 + 3*(4-5)^2 + 2*(5-5)^2 + (7-5)^2 + (9-5)^2) / 7
            = (9 + 3 + 0 + 4 + 16) / 7 = 32/7 = 4,57
- desvio padrao = raiz(4,57) = 2,14

===============================================================================
15. REGRA DE TRES DIRETA E INVERSA
===============================================================================

ENUNCIADO: Resolver problemas de regra de tres direta (grandezas diretamente
proporcionais) e inversa (inversamente proporcionais).

FORMULAS:
- Direta:   A/B = C/X  ->  X = (B * C) / A
- Inversa:  A * B = C * X  ->  X = (A * B) / C

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float a, b, c, x;
    int opcao;

    printf("1 - Regra de tres direta\n");
    printf("2 - Regra de tres inversa\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    printf("Valor A: ");
    scanf("%f", &a);
    printf("Valor B (correspondente a A): ");
    scanf("%f", &b);
    printf("Valor C (correspondente a X): ");
    scanf("%f", &c);

    if (opcao == 1) {
        // Direta: se A esta para B, entao C esta para X
        x = (b * c) / a;
        printf("X = %.2f\n", x);
    } else if (opcao == 2) {
        // Inversa: se A faz B, entao C faz X (inversamente)
        x = (a * b) / c;
        printf("X = %.2f\n", x);
    } else {
        printf("Opcao invalida\n");
    }

    // Exemplos adicionais
    printf("\nExemplos:\n");
    printf("Direta: 3 produtos custam R$ 30, 7 custam R$ %.2f\n",
           (30.0f * 7) / 3);
    printf("Inversa: 4 pedreiros fazem em 12 dias, 6 fazem em %.2f dias\n",
           (4.0f * 12) / 6);
    return 0;
}
```

EXEMPLO NUMERICO:
- Direta: 3 produtos = R$ 30 -> 7 produtos = (30 * 7) / 3 = R$ 70,00
- Inversa: 4 pedreiros = 12 dias -> 6 pedreiros = (4 * 12) / 6 = 8 dias
- Direta: 5 horas = 200 km -> 8 horas = (200 * 8) / 5 = 320 km
- Inversa: 6 maquinas = 10 dias -> 4 maquinas = (6 * 10) / 4 = 15 dias

===============================================================================
16. AREAS: TRIANGULO EQUILATERO, HEXAGONO, PENTAGONO, SETOR CIRCULAR
===============================================================================

ENUNCIADO: Calcular a area de figuras planas: triangulo equilatero, hexagono
regular, pentagono regular e setor circular.

FORMULAS:
- Triangulo equilatero:  A = (L^2 * raiz(3)) / 4
- Hexagono regular:      A = (3 * L^2 * raiz(3)) / 2
- Pentagono regular:     A = (5 * L^2) / (4 * tan(36 graus))
                         ou A = (perimetro * apotema) / 2
- Setor circular:        A = (angulo * pi * r^2) / 360

CODIGO C:

```c
#include <stdio.h>
#define PI 3.14159265f

// raiz quadrada pelo metodo de Newton
float raiz(float n) {
    float r = n / 2;
    int i;
    for (i = 0; i < 30; i++) {
        r = (r + n / r) / 2;
    }
    return r;
}

// tangente aproximada (em radianos) usando serie de Taylor simplificada
// como alternativa, usamos valor pre-calculado para 36 graus
// tan(36) = 0,7265 (aproximado)

int main(void) {
    float L, r, angulo, area;
    int opcao;

    printf("1 - Triangulo equilatero\n");
    printf("2 - Hexagono regular\n");
    printf("3 - Pentagono regular\n");
    printf("4 - Setor circular\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Lado L: ");
            scanf("%f", &L);
            area = (L * L * raiz(3)) / 4;
            printf("Area do triangulo equilatero: %.2f\n", area);
            break;
        case 2:
            printf("Lado L: ");
            scanf("%f", &L);
            area = (3 * L * L * raiz(3)) / 2;
            printf("Area do hexagono regular: %.2f\n", area);
            break;
        case 3:
            printf("Lado L: ");
            scanf("%f", &L);
            // tan(36) = 0,7265 (aproximado)
            area = (5 * L * L) / (4 * 0.7265f);
            printf("Area do pentagono regular: %.2f\n", area);
            break;
        case 4:
            printf("Raio r: ");
            scanf("%f", &r);
            printf("Angulo (graus): ");
            scanf("%f", &angulo);
            area = (angulo * PI * r * r) / 360;
            printf("Area do setor circular: %.2f\n", area);
            break;
        default:
            printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- Triangulo equilatero: L = 6 -> A = (36 * 1,732)/4 = 15,59
- Hexagono: L = 4 -> A = (3 * 16 * 1,732)/2 = 41,57
- Pentagono: L = 5 -> A = (5 * 25)/(4 * 0,7265) = 125/2,906 = 43,01
- Setor circular: r = 10, angulo = 90 -> A = (90 * 3,1416 * 100)/360 = 78,54

===============================================================================
17. VOLUMES: PIRAMIDE, CONE, TRONCO DE CONE
===============================================================================

ENUNCIADO: Calcular o volume de uma piramide, de um cone e de um tronco de cone.

FORMULAS:
- Piramide:     V = (areaBase * altura) / 3
- Cone:         V = (pi * r^2 * h) / 3
- Tronco de cone: V = (pi * h / 3) * (R^2 + R*r + r^2)
    onde R = raio maior, r = raio menor, h = altura

CODIGO C:

```c
#include <stdio.h>
#define PI 3.14159265f

int main(void) {
    int opcao;
    float areaBase, h, r, R, V;

    printf("1 - Piramide\n");
    printf("2 - Cone\n");
    printf("3 - Tronco de cone\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Area da base: ");
            scanf("%f", &areaBase);
            printf("Altura: ");
            scanf("%f", &h);
            V = (areaBase * h) / 3;
            printf("Volume da piramide: %.2f\n", V);
            break;
        case 2:
            printf("Raio da base: ");
            scanf("%f", &r);
            printf("Altura: ");
            scanf("%f", &h);
            V = (PI * r * r * h) / 3;
            printf("Volume do cone: %.2f\n", V);
            break;
        case 3:
            printf("Raio maior (R): ");
            scanf("%f", &R);
            printf("Raio menor (r): ");
            scanf("%f", &r);
            printf("Altura (h): ");
            scanf("%f", &h);
            V = (PI * h / 3) * (R * R + R * r + r * r);
            printf("Volume do tronco de cone: %.2f\n", V);
            break;
        default:
            printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- Piramide: areaBase = 25, h = 10 -> V = (25 * 10)/3 = 83,33
- Cone: r = 3, h = 10 -> V = (3,1416 * 9 * 10)/3 = 94,25
- Tronco: R = 5, r = 3, h = 8 -> V = (3,1416 * 8/3) * (25 + 15 + 9)
        = 8,378 * 49 = 410,50

===============================================================================
18. PITAGORAS APLICADO (ESCADA NA PAREDE, DISTANCIA)
===============================================================================

ENUNCIADO: Aplicar o teorema de Pitagoras para resolver problemas como: uma
escada apoiada na parede, encontrar a hipotenusa ou um cateto.

FORMULA:
- hipotenusa^2 = cateto1^2 + cateto2^2
- hipotenusa = raiz(c1^2 + c2^2)
- cateto = raiz(hip^2 - cateto^2)

CODIGO C:

```c
#include <stdio.h>

// raiz quadrada pelo metodo de Newton
float raiz(float n) {
    if (n < 0) return -1;  // erro
    float r = n / 2;
    int i;
    for (i = 0; i < 30; i++) {
        r = (r + n / r) / 2;
    }
    return r;
}

int main(void) {
    int opcao;
    float c1, c2, hip;

    printf("1 - Calcular hipotenusa\n");
    printf("2 - Calcular cateto\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        printf("Cateto 1: ");
        scanf("%f", &c1);
        printf("Cateto 2: ");
        scanf("%f", &c2);
        hip = raiz(c1 * c1 + c2 * c2);
        printf("Hipotenusa: %.2f\n", hip);
    } else if (opcao == 2) {
        printf("Hipotenusa: ");
        scanf("%f", &hip);
        printf("Cateto conhecido: ");
        scanf("%f", &c1);
        c2 = raiz(hip * hip - c1 * c1);
        printf("Cateto desconhecido: %.2f\n", c2);
    } else {
        printf("Opcao invalida\n");
    }

    // Exemplo: escada de 10m apoiada, base a 6m da parede
    printf("\nExemplo - escada:\n");
    hip = 10;
    c1 = 6;
    c2 = raiz(hip * hip - c1 * c1);
    printf("Escada de %.0fm, base a %.0fm da parede, altura = %.2f m\n",
           hip, c1, c2);
    return 0;
}
```

EXEMPLO NUMERICO:
- Hipotenusa: c1 = 3, c2 = 4 -> hip = raiz(9 + 16) = raiz(25) = 5
- Cateto: hip = 10, c1 = 6 -> c2 = raiz(100 - 36) = raiz(64) = 8
- Escada: 10m de comprimento, base a 6m da parede -> altura = 8m

===============================================================================
19. DISTANCIA ENTRE DOIS PONTOS
===============================================================================

ENUNCIADO: Calcular a distancia entre dois pontos (x1,y1) e (x2,y2) no plano
cartesiano.

FORMULA:
- d = raiz((x2 - x1)^2 + (y2 - y1)^2)

CODIGO C:

```c
#include <stdio.h>

float raiz(float n) {
    float r = n / 2;
    int i;
    for (i = 0; i < 30; i++) {
        r = (r + n / r) / 2;
    }
    return r;
}

int main(void) {
    float x1, y1, x2, y2, dist;

    printf("Ponto 1 (x1 y1): ");
    scanf("%f %f", &x1, &y1);
    printf("Ponto 2 (x2 y2): ");
    scanf("%f %f", &x2, &y2);

    dist = raiz((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    printf("Distancia: %.2f\n", dist);
    return 0;
}
```

EXEMPLO NUMERICO:
- P1 = (1, 2), P2 = (4, 6)
- d = raiz((4-1)^2 + (6-2)^2) = raiz(9 + 16) = raiz(25) = 5,00

===============================================================================
20. PONTO MEDIO
===============================================================================

ENUNCIADO: Calcular o ponto medio entre dois pontos (x1,y1) e (x2,y2).

FORMULA:
- Xm = (x1 + x2) / 2
- Ym = (y1 + y2) / 2

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float x1, y1, x2, y2, xm, ym;

    printf("Ponto 1 (x1 y1): ");
    scanf("%f %f", &x1, &y1);
    printf("Ponto 2 (x2 y2): ");
    scanf("%f %f", &x2, &y2);

    xm = (x1 + x2) / 2;
    ym = (y1 + y2) / 2;

    printf("Ponto medio: (%.2f, %.2f)\n", xm, ym);
    return 0;
}
```

EXEMPLO NUMERICO:
- P1 = (2, 4), P2 = (8, 10)
- Xm = (2 + 8)/2 = 5
- Ym = (4 + 10)/2 = 7
- Ponto medio = (5, 7)

===============================================================================
21. EQUACAO DA RETA
===============================================================================

ENUNCIADO: Determinar a equacao da reta que passa por dois pontos, na forma
y = ax + b, calculando o coeficiente angular (a) e o coeficiente linear (b).

FORMULAS:
- coeficiente angular:  a = (y2 - y1) / (x2 - x1)
- coeficiente linear:   b = y1 - a * x1
- equacao:               y = a*x + b

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    float x1, y1, x2, y2, a, b, x, y;

    printf("Ponto 1 (x1 y1): ");
    scanf("%f %f", &x1, &y1);
    printf("Ponto 2 (x2 y2): ");
    scanf("%f %f", &x2, &y2);

    if (x2 == x1) {
        printf("Reta vertical: x = %.2f\n", x1);
        return 0;
    }

    a = (y2 - y1) / (x2 - x1);
    b = y1 - a * x1;

    printf("Equacao da reta: y = ");
    if (a == 1)
        printf("x");
    else if (a == -1)
        printf("-x");
    else
        printf("%.2fx", a);

    if (b >= 0)
        printf(" + %.2f\n", b);
    else
        printf(" - %.2f\n", -b);

    // Calcular y para um x dado
    printf("Digite um valor de x para calcular y: ");
    scanf("%f", &x);
    y = a * x + b;
    printf("y = %.2f\n", y);
    return 0;
}
```

EXEMPLO NUMERICO:
- P1 = (1, 3), P2 = (4, 12)
- a = (12 - 3) / (4 - 1) = 9/3 = 3
- b = 3 - 3*1 = 0
- equacao: y = 3x
- para x = 5 -> y = 15

===============================================================================
22. CONVERSAO DE UNIDADES
===============================================================================

ENUNCIADO: Converter entre unidades de comprimento, massa, tempo, area, volume
e velocidade.

FORMULAS:
- Comprimento: 1 m = 100 cm = 1000 mm; 1 km = 1000 m; 1 milha = 1609,34 m
- Massa: 1 kg = 1000 g; 1 ton = 1000 kg; 1 libra = 0,4536 kg
- Tempo: 1 h = 60 min = 3600 s; 1 dia = 24 h
- Area: 1 m^2 = 10000 cm^2; 1 hectare = 10000 m^2; 1 acre = 4046,86 m^2
- Volume: 1 m^3 = 1000 L; 1 L = 1000 mL
- Velocidade: 1 m/s = 3,6 km/h; 1 km/h = 0,2778 m/s; 1 milha/h = 1,609 km/h

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    int categoria, opcao;
    float valor, resultado;

    printf("Categorias:\n");
    printf("1 - Comprimento\n");
    printf("2 - Massa\n");
    printf("3 - Tempo\n");
    printf("4 - Area\n");
    printf("5 - Volume\n");
    printf("6 - Velocidade\n");
    printf("Categoria: ");
    scanf("%d", &categoria);

    switch (categoria) {
        case 1: // Comprimento
            printf("1 - km para m\n2 - m para cm\n3 - m para mm\n");
            printf("4 - milhas para km\n5 - m para pes\n");
            printf("Opcao: ");
            scanf("%d", &opcao);
            printf("Valor: ");
            scanf("%f", &valor);
            if (opcao == 1) resultado = valor * 1000;
            else if (opcao == 2) resultado = valor * 100;
            else if (opcao == 3) resultado = valor * 1000;
            else if (opcao == 4) resultado = valor * 1.60934f;
            else if (opcao == 5) resultado = valor * 3.28084f;
            else { printf("Invalido\n"); return 1; }
            printf("Resultado: %.4f\n", resultado);
            break;
        case 2: // Massa
            printf("1 - kg para g\n2 - ton para kg\n3 - libras para kg\n");
            printf("4 - kg para libras\n");
            printf("Opcao: ");
            scanf("%d", &opcao);
            printf("Valor: ");
            scanf("%f", &valor);
            if (opcao == 1) resultado = valor * 1000;
            else if (opcao == 2) resultado = valor * 1000;
            else if (opcao == 3) resultado = valor * 0.453592f;
            else if (opcao == 4) resultado = valor * 2.20462f;
            else { printf("Invalido\n"); return 1; }
            printf("Resultado: %.4f\n", resultado);
            break;
        case 3: // Tempo
            printf("1 - horas para minutos\n2 - horas para segundos\n");
            printf("3 - dias para horas\n4 - segundos para horas\n");
            printf("Opcao: ");
            scanf("%d", &opcao);
            printf("Valor: ");
            scanf("%f", &valor);
            if (opcao == 1) resultado = valor * 60;
            else if (opcao == 2) resultado = valor * 3600;
            else if (opcao == 3) resultado = valor * 24;
            else if (opcao == 4) resultado = valor / 3600;
            else { printf("Invalido\n"); return 1; }
            printf("Resultado: %.4f\n", resultado);
            break;
        case 4: // Area
            printf("1 - m^2 para cm^2\n2 - hectares para m^2\n");
            printf("3 - m^2 para hectares\n4 - acres para m^2\n");
            printf("Opcao: ");
            scanf("%d", &opcao);
            printf("Valor: ");
            scanf("%f", &valor);
            if (opcao == 1) resultado = valor * 10000;
            else if (opcao == 2) resultado = valor * 10000;
            else if (opcao == 3) resultado = valor / 10000;
            else if (opcao == 4) resultado = valor * 4046.86f;
            else { printf("Invalido\n"); return 1; }
            printf("Resultado: %.4f\n", resultado);
            break;
        case 5: // Volume
            printf("1 - m^3 para litros\n2 - litros para mL\n");
            printf("3 - litros para m^3\n4 - galoes para litros\n");
            printf("Opcao: ");
            scanf("%d", &opcao);
            printf("Valor: ");
            scanf("%f", &valor);
            if (opcao == 1) resultado = valor * 1000;
            else if (opcao == 2) resultado = valor * 1000;
            else if (opcao == 3) resultado = valor / 1000;
            else if (opcao == 4) resultado = valor * 3.78541f;
            else { printf("Invalido\n"); return 1; }
            printf("Resultado: %.4f\n", resultado);
            break;
        case 6: // Velocidade
            printf("1 - m/s para km/h\n2 - km/h para m/s\n");
            printf("3 - km/h para milhas/h\n4 - milhas/h para km/h\n");
            printf("Opcao: ");
            scanf("%d", &opcao);
            printf("Valor: ");
            scanf("%f", &valor);
            if (opcao == 1) resultado = valor * 3.6f;
            else if (opcao == 2) resultado = valor / 3.6f;
            else if (opcao == 3) resultado = valor / 1.60934f;
            else if (opcao == 4) resultado = valor * 1.60934f;
            else { printf("Invalido\n"); return 1; }
            printf("Resultado: %.4f\n", resultado);
            break;
        default:
            printf("Categoria invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- 5 km para m = 5000 m
- 2 kg para g = 2000 g
- 2 h para segundos = 7200 s
- 2 m^2 para cm^2 = 20000 cm^2
- 2 m^3 para litros = 2000 L
- 10 m/s para km/h = 36 km/h
- 72 km/h para m/s = 20 m/s

===============================================================================
23. IDADE A PARTIR DE DATA DE NASCIMENTO
===============================================================================

ENUNCIADO: Calcular a idade de uma pessoa a partir da data de nascimento e a
data atual.

FORMULA:
- idade = anoAtual - anoNascimento
- se (mesAtual < mesNasc) ou (mesAtual == mesNasc e diaAtual < diaNasc):
    idade = idade - 1   (ainda nao fez aniversario)

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    int diaN, mesN, anoN, diaA, mesA, anoA, idade;

    printf("Data de nascimento (dd mm aaaa): ");
    scanf("%d %d %d", &diaN, &mesN, &anoN);
    printf("Data atual (dd mm aaaa): ");
    scanf("%d %d %d", &diaA, &mesA, &anoA);

    idade = anoA - anoN;

    // ajusta se ainda nao fez aniversario no ano atual
    if (mesA < mesN || (mesA == mesN && diaA < diaN)) {
        idade--;
    }

    printf("Idade: %d anos\n", idade);
    return 0;
}
```

EXEMPLO NUMERICO:
- nascimento: 15/03/2000, atual: 06/07/2026
- idade = 2026 - 2000 = 26 (ja fez aniversario em marco) -> 26 anos
- nascimento: 25/12/2000, atual: 06/07/2026
- idade = 26 - 1 = 25 (nao chegou dezembro ainda)

===============================================================================
24. ANO BISSEXTO
===============================================================================

ENUNCIADO: Verificar se um ano e bissexto.

FORMULA:
- E bissexto se:
    (divisivel por 4 E nao divisivel por 100) OU (divisivel por 400)

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    int ano;

    printf("Digite o ano: ");
    scanf("%d", &ano);

    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        printf("%d e bissexto\n", ano);
    } else {
        printf("%d NAO e bissexto\n", ano);
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- 2000 -> divisivel por 400 -> bissexto
- 1900 -> divisivel por 100 mas nao por 400 -> NAO bissexto
- 2024 -> divisivel por 4 e nao por 100 -> bissexto
- 2023 -> nao divisivel por 4 -> NAO bissexto

===============================================================================
25. DIAS ENTRE DUAS DATAS (APROXIMADO)
===============================================================================

ENUNCIADO: Calcular o numero de dias entre duas datas (metodo aproximado,
convertendo cada data para dias desde uma data base).

FORMULA:
- converter data para dias: totalDias = ano*365 + ano/4 - ano/100 + ano/400
                            + diasAteMes + dia
- diasEntre = |totalDias2 - totalDias1|

CODIGO C:

```c
#include <stdio.h>

// dias de cada mes (ano nao bissexto)
int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int ehBissexto(int ano) {
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
        return 1;
    return 0;
}

// converte uma data para o total de dias desde o ano 0 (aproximado)
long totalDias(int dia, int mes, int ano) {
    long total = 0;
    int i;

    // dias dos anos completos
    total = (long)ano * 365 + ano / 4 - ano / 100 + ano / 400;

    // dias dos meses do ano atual
    for (i = 0; i < mes - 1; i++) {
        total += diasMes[i];
    }

    // se o ano for bissexto e ja passou fevereiro, soma 1 dia
    if (mes > 2 && ehBissexto(ano)) {
        total += 1;
    }

    // soma os dias
    total += dia;

    return total;
}

int main(void) {
    int dia1, mes1, ano1, dia2, mes2, ano2;
    long t1, t2, diff;

    printf("Data 1 (dd mm aaaa): ");
    scanf("%d %d %d", &dia1, &mes1, &ano1);
    printf("Data 2 (dd mm aaaa): ");
    scanf("%d %d %d", &dia2, &mes2, &ano2);

    t1 = totalDias(dia1, mes1, ano1);
    t2 = totalDias(dia2, mes2, ano2);

    diff = t2 - t1;
    if (diff < 0) diff = -diff;

    printf("Dias entre as datas: %ld\n", diff);
    return 0;
}
```

EXEMPLO NUMERICO:
- Data 1: 01/01/2024, Data 2: 01/01/2025
- diff = 366 (2024 e bissexto)
- Data 1: 15/03/2024, Data 2: 15/03/2025
- diff = 365

===============================================================================
26. NUMERO DE ARMSTRONG, PERFEITO, CAPICUA
===============================================================================

ENUNCIADO: Verificar se um numero e:
(a) Armstrong: soma dos cubos dos digitos = o proprio numero (para 3 digitos)
(b) Perfeito: soma dos divisores proprios = o numero
(c) Capicua (palindromo): le igual de tras para frente

FORMULAS:
- Armstrong (3 digitos): d1^3 + d2^3 + d3^3 == numero
- Perfeito: soma(divisores de 1 a n-1) == n
- Capicua: inverter os digitos e comparar com o original

CODIGO C:

```c
#include <stdio.h>

// verifica se e Armstrong (para numeros de 3 digitos)
int ehArmstrong(int n) {
    int original = n, soma = 0, dig;
    while (n > 0) {
        dig = n % 10;
        soma += dig * dig * dig;
        n /= 10;
    }
    return soma == original;
}

// verifica se e perfeito
int ehPerfeito(int n) {
    int soma = 0, i;
    for (i = 1; i < n; i++) {
        if (n % i == 0) {
            soma += i;
        }
    }
    return soma == n;
}

// verifica se e capicua (palindromo)
int ehCapicua(int n) {
    int original = n, invertido = 0, dig;
    while (n > 0) {
        dig = n % 10;
        invertido = invertido * 10 + dig;
        n /= 10;
    }
    return invertido == original;
}

int main(void) {
    int numero, opcao;

    printf("Numero: ");
    scanf("%d", &numero);

    printf("\n1 - Armstrong\n2 - Perfeito\n3 - Capicua\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            if (ehArmstrong(numero))
                printf("%d e Armstrong\n", numero);
            else
                printf("%d NAO e Armstrong\n", numero);
            break;
        case 2:
            if (ehPerfeito(numero))
                printf("%d e perfeito\n", numero);
            else
                printf("%d NAO e perfeito\n", numero);
            break;
        case 3:
            if (ehCapicua(numero))
                printf("%d e capicua\n", numero);
            else
                printf("%d NAO e capicua\n", numero);
            break;
        default:
            printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- Armstrong: 153 -> 1^3 + 5^3 + 3^3 = 1 + 125 + 27 = 153 (sim)
- Armstrong: 370 -> 27 + 343 + 0 = 370 (sim)
- Perfeito: 6 -> divisores 1,2,3 -> soma = 6 (sim)
- Perfeito: 28 -> divisores 1,2,4,7,14 -> soma = 28 (sim)
- Perfeito: 12 -> divisores 1,2,3,4,6 -> soma = 16 (nao)
- Capicua: 121 -> invertido 121 (sim)
- Capicua: 12321 -> invertido 12321 (sim)
- Capicua: 123 -> invertido 321 (nao)

===============================================================================
27. CRIVO DE ERATOSTENES (PRIMOS ATE N)
===============================================================================

ENUNCIADO: Encontrar todos os numeros primos de 2 ate N usando o Crivo de
Eratostenes.

FORMULA (algoritmo):
- Cria um vetor de 2 a N, todos marcados como primos
- Para cada numero p a partir de 2:
    se p ainda esta marcado como primo:
        marca todos os multiplos de p (2p, 3p, 4p, ...) como nao primos
- Os que sobraram marcados como primos sao os primos ate N

CODIGO C:

```c
#include <stdio.h>

#define MAX 1000

int main(void) {
    int crivo[MAX + 1];  // indice de 0 a MAX
    int n, i, j;

    printf("Encontrar primos ate (max %d): ", MAX);
    scanf("%d", &n);

    if (n < 2 || n > MAX) {
        printf("Valor invalido\n");
        return 1;
    }

    // inicializa todos como primos (1)
    for (i = 2; i <= n; i++) {
        crivo[i] = 1;
    }

    // aplica o crivo
    for (i = 2; i * i <= n; i++) {
        if (crivo[i] == 1) {
            // marca os multiplos de i como nao primos
            for (j = i * i; j <= n; j += i) {
                crivo[j] = 0;
            }
        }
    }

    // imprime os primos
    printf("Primos ate %d:\n", n);
    int cont = 0;
    for (i = 2; i <= n; i++) {
        if (crivo[i] == 1) {
            printf("%d ", i);
            cont++;
        }
    }
    printf("\nTotal: %d primos\n", cont);
    return 0;
}
```

EXEMPLO NUMERICO:
- N = 30 -> primos: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 (10 primos)
- N = 50 -> primos: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47

===============================================================================
28. CONVERSAO DECIMAL / OCTAL
===============================================================================

ENUNCIADO: Converter um numero decimal para octal (base 8) e vice-versa.

FORMULAS:
- Decimal para octal: dividir sucessivamente por 8, ler os restos de tras
  para frente.
- Octal para decimal: multiplicar cada digito por 8 elevado a sua posicao
  (da direita para esquerda, comecando em 0).

CODIGO C:

```c
#include <stdio.h>
#include <math.h>  // para pow() - no Windows geralmente nao precisa de -lm

int main(void) {
    int opcao, decimal, octal, digito, potencia, resultado;

    printf("1 - Decimal para octal\n");
    printf("2 - Octal para decimal\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        printf("Numero decimal: ");
        scanf("%d", &decimal);

        // converte para octal montando o numero ao contrario
        int mult = 1;
        octal = 0;
        int temp = decimal;
        if (temp == 0) {
            octal = 0;
        } else {
            while (temp > 0) {
                digito = temp % 8;
                octal = octal + digito * mult;
                mult *= 10;
                temp /= 8;
            }
        }
        printf("Octal: %d\n", octal);
    } else if (opcao == 2) {
        printf("Numero octal (digitos 0-7): ");
        scanf("%d", &octal);

        decimal = 0;
        potencia = 0;
        int temp = octal;
        while (temp > 0) {
            digito = temp % 10;
            decimal = decimal + digito * (int)pow(8, potencia);
            potencia++;
            temp /= 10;
        }
        printf("Decimal: %d\n", decimal);
    } else {
        printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- Decimal 100 para octal:
  100 / 8 = 12 resto 4
  12 / 8 = 1 resto 4
  1 / 8 = 0 resto 1
  -> octal = 144
- Octal 144 para decimal:
  4 * 8^0 + 4 * 8^1 + 1 * 8^2 = 4 + 32 + 64 = 100
- Decimal 255 -> 255/8=31 r7, 31/8=3 r7, 3/8=0 r3 -> octal = 377
- Octal 377 -> 7 + 7*8 + 3*64 = 7 + 56 + 192 = 255

===============================================================================
29. FATORIAL GRANDE (USAR LONG LONG OU DOUBLE)
===============================================================================

ENUNCIADO: Calcular o fatorial de um numero, usando long long para valores
maiores (ate 20) ou double para valores ainda maiores (aproximado).

FORMULA:
- n! = n * (n-1) * (n-2) * ... * 2 * 1
- 0! = 1, 1! = 1

LIMITES:
- int (32 bits): ate 12!
- long long (64 bits): ate 20!
- double: ate 170! (aproximado, com perda de precisao)

CODIGO C:

```c
#include <stdio.h>

int main(void) {
    int n, opcao, i;

    printf("Calcular fatorial de: ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Fatorial nao definido para negativos\n");
        return 1;
    }

    printf("1 - int (ate 12)\n2 - long long (ate 20)\n3 - double (ate 170)\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        int fat = 1;
        for (i = 2; i <= n; i++) {
            fat *= i;
        }
        printf("%d! = %d\n", n, fat);
    } else if (opcao == 2) {
        long long fat = 1;
        for (i = 2; i <= n; i++) {
            fat *= i;
        }
        printf("%d! = %lld\n", n, fat);
    } else if (opcao == 3) {
        double fat = 1;
        for (i = 2; i <= n; i++) {
            fat *= i;
        }
        printf("%d! = %.0lf\n", n, fat);
    } else {
        printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- 5! = 120
- 10! = 3628800
- 15! = 1307674368000 (long long)
- 20! = 2432902008176640000 (long long, limite)
- 50! = 3,04140932e+64 (double, aproximado)

===============================================================================
30. SOMATORIO E PRODUTORIO
===============================================================================

ENUNCIADO: Calcular o somatorio e o produtorio de uma sequencia de numeros
fornecida pelo usuario.

FORMULAS:
- Somatorio: S = x1 + x2 + x3 + ... + xn
- Produtorio: P = x1 * x2 * x3 * ... * xn
- Somatorio com formula fechada (1 a n): S = n*(n+1)/2
- Somatorio de quadrados (1^2 a n^2): S = n*(n+1)*(2n+1)/6

CODIGO C:

```c
#include <stdio.h>

#define MAX 100

int main(void) {
    float v[MAX], soma = 0, produto = 1;
    int n, i, opcao;

    printf("1 - Somatorio de vetor\n");
    printf("2 - Produtorio de vetor\n");
    printf("3 - Somatorio de 1 a n (formula fechada)\n");
    printf("4 - Somatorio de quadrados 1^2 a n^2\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Quantos numeros (max %d)? ", MAX);
            scanf("%d", &n);
            for (i = 0; i < n; i++) {
                printf("v[%d]: ", i);
                scanf("%f", &v[i]);
                soma += v[i];
            }
            printf("Somatorio = %.2f\n", soma);
            break;
        case 2:
            printf("Quantos numeros (max %d)? ", MAX);
            scanf("%d", &n);
            for (i = 0; i < n; i++) {
                printf("v[%d]: ", i);
                scanf("%f", &v[i]);
                produto *= v[i];
            }
            printf("Produtorio = %.2f\n", produto);
            break;
        case 3:
            printf("n: ");
            scanf("%d", &n);
            // formula fechada: n*(n+1)/2
            soma = (float)n * (n + 1) / 2;
            printf("Somatorio de 1 a %d = %.0f\n", n, soma);
            break;
        case 4:
            printf("n: ");
            scanf("%d", &n);
            // formula fechada: n*(n+1)*(2n+1)/6
            soma = (float)n * (n + 1) * (2 * n + 1) / 6;
            printf("Somatorio de 1^2 a %d^2 = %.0f\n", n, soma);
            break;
        default:
            printf("Opcao invalida\n");
    }
    return 0;
}
```

EXEMPLO NUMERICO:
- Somatorio do vetor {3, 5, 7, 2, 8} = 25
- Produtorio do vetor {2, 3, 4} = 24
- Somatorio de 1 a 100 = 100*101/2 = 5050
- Somatorio de 1^2 a 10^2 = 10*11*21/6 = 385

===============================================================================
FIM DO DOCUMENTO - FORMULAS EXTRAS GB
===============================================================================

NOTAS FINAIS:
- Para compilar qualquer codigo: gcc arquivo.c -o programa
- Se usar math.h (pow, sqrt) no Linux: gcc arquivo.c -o programa -lm
- No Windows com MinGW geralmente nao precisa do -lm
- Os codigos usam raiz quadrada pelo metodo de Newton para evitar depender
  de math.h quando possivel, mas alguns usam pow() da math.h.
- Sempre validar entradas do usuario em codigo de producao.
- As faixas de INSS e IR sao aproximadas e mudam anualmente; conferir a
  tabela vigente na data da prova.

DICAS DE PROVA:
- Leia o enunciado com atencao e identifique quais variaveis sao dadas.
- Escolha o tipo de dado adequado (int para contagem, float/double para
  valores monetarios e cientificos).
- Use estruturas de repeticao (for, while) para somatorios, produtorios e
  calculos iterativos.
- Use estruturas de decisao (if/else, switch) para faixas e opcoes de menu.
- Comente o codigo para facilitar a leitura pelo corretor.
===============================================================================
