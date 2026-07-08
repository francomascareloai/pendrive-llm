# 06 - TOPICOS EXTRAS (preenchimento de gaps)

Documento com os topicos de C que faltavam no material, identificados em
analise de gaps. Completa a cobertura para que nao falte NENHUM assunto de C
que possa cair em prova de Algoritmos e Programacao II.

INDICE DESTE ARQUIVO:
  PARTE 1 - Passagem por referencia + fundamentos (modificadores, const, escopo, static, precedencia, formatadores)
  PARTE 2 - Enum, union, bitwise, argc/argv, stdbool, virgula, dangling, funcao retorna ponteiro
  PARTE 3 - qsort, bsearch, rand/srand, time.h, matriz dinamica, strtol/strtod, snprintf, ponteiro para funcao, array de ponteiros, ferror, tmpfile, fgetpos, limits/float, atexit, macros

Palavras-chave Ctrl-F: passagem por referencia, por valor, ponteiro, &, *, ->, troca,
dobrar, modificadores, unsigned, long long, const, escopo, global, local, static,
precedencia, formatador, %x, %o, %u, enum, union, bitwise, &, |, ^, ~, <<, >>,
argc, argv, bool, stdbool, virgula, dangling, retorna ponteiro, qsort, bsearch,
rand, srand, aleatorio, time, clock, matriz dinamica, strtol, strtod, snprintf,
ispunct, isxdigit, ponteiro funcao, callback, array ponteiros, vetor strings,
ferror, tmpfile, fgetpos, fsetpos, limits, float, atexit, macro, #define, #ifdef.


#############################################################
# PARTE 1 - PASSAGEM POR REFERENCIA E FUNDAMENTOS
#############################################################

## 1. PASSAGEM POR REFERENCIA

**Palavras-chave:** PASSAGEM POR REFERENCIA, passagem por valor, ponteiro, endereco, copia, operador &, operador *, dereferenciacao, struct por referencia, seta ->, troca, swap, min max, vetor parametro, matriz parametro, pegadinha &

### Conceito: VALOR vs REFERENCIA

Em C, quando passamos uma variavel para uma funcao, existem duas formas:

- **POR VALOR:** a funcao recebe uma COPIA do valor da variavel. Qualquer alteracao feita dentro da funcao NAO reflete na variavel original. E o comportamento PADRAO do C para tipos simples (int, float, char, double, struct).

- **POR REFERENCIA:** a funcao recebe o ENDERECO da variavel (via ponteiro). Alteracoes feitas dentro da funcao REFLETEM na variavel original. Usa-se o operador `&` na chamada (para obter o endereco) e o operador `*` dentro da funcao (para acessar o valor guardado naquele endereco, chamado de dereferenciacao).

### Exemplo 1: dobrar uma variavel (valor vs referencia)

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// POR VALOR: recebe uma COPIA do valor
// Alteracoes NAO refletem na variavel original
void dobrarValor(int x) {
    x = x * 2;  // altera a COPIA, o original nao muda
    printf("  Dentro de dobrarValor: x = %d\n", x);
}

// POR REFERENCIA: recebe o ENDERECO da variavel
// Alteracoes REFLETEM na variavel original
void dobrarRef(int *x) {
    *x = (*x) * 2;  // *x acessa o valor no endereco recebido
    printf("  Dentro de dobrarRef: *x = %d\n", *x);
}

int main(void) {
    int n = 10;

    printf("--- POR VALOR ---\n");
    printf("Antes: n = %d\n", n);
    dobrarValor(n);  // passa o VALOR (copia) -> SEM &
    printf("Depois: n = %d (NAO mudou!)\n", n);

    printf("\n--- POR REFERENCIA ---\n");
    printf("Antes: n = %d\n", n);
    dobrarRef(&n);  // passa o ENDERECO (&) -> COM &
    printf("Depois: n = %d (mudou!)\n", n);

    system("pause");
    return 0;
}
```

### Exemplo 2: vetor como parametro (referencia implicita)

O nome de um vetor JA E o endereco do primeiro elemento. Por isso, ao passar um vetor para uma funcao, as alteracoes ja refletem no original (referencia implicita). Nao precisa de `&` na chamada nem `*` no parametro.

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// Vetor como parametro: JA E por referencia implicita
// O nome do vetor e o ENDERECO do primeiro elemento
// Nao precisa do & na chamada, nao precisa do * no parametro
void dobrarVetor(int v[], int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        v[i] = v[i] * 2;  // altera o vetor ORIGINAL
    }
}

int main(void) {
    int nums[] = {1, 2, 3, 4, 5};
    int i;

    printf("Antes: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");

    dobrarVetor(nums, 5);  // SEM & (vetor ja e endereco)

    printf("Depois: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");

    system("pause");
    return 0;
}
```

### Exemplo 3: matriz como parametro (precisa das colunas)

Quando passamos uma matriz para uma funcao, o numero de COLUNAS e obrigatorio na declaracao do parametro. O numero de linhas pode ser omitido. Isso acontece porque o compilador precisa saber quantas colunas existem para calcular o endereco de cada elemento (a memoria e linear).

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// Matriz como parametro: PRECISA do numero de COLUNAS
// Linhas podem ser omitidas, colunas NAO
// Sintaxe: tipo nome[][COLUNAS]
void mostrarMatriz(int m[][3], int linhas) {
    int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

// Dobrar todos os elementos da matriz (altera o original)
void dobrarMatriz(int m[][3], int linhas) {
    int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < 3; j++) {
            m[i][j] *= 2;  // altera a matriz original
        }
    }
}

int main(void) {
    int mat[2][3] = {{1, 2, 3}, {4, 5, 6}};

    printf("Matriz original:\n");
    mostrarMatriz(mat, 2);  // passa matriz + numero de linhas

    dobrarMatriz(mat, 2);

    printf("\nMatriz dobrada:\n");
    mostrarMatriz(mat, 2);

    system("pause");
    return 0;
}
```

### Exemplo 4: struct por valor vs por referencia (ponto vs seta)

Quando passamos uma struct POR VALOR, acessamos os campos com ponto (`.`). Quando passamos POR REFERENCIA (ponteiro para struct), acessamos os campos com seta (`->`).

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// Struct com alias (typedef struct + nome curto)
typedef struct {
    int x;
    int y;
} Ponto;

// POR VALOR: recebe COPIA da struct
// Usa ponto (.) para acessar campos
// Alteracoes NAO refletem no original
void moverValor(Ponto p) {
    p.x += 10;  // altera a COPIA
    p.y += 10;
    printf("  Dentro moverValor: (%d, %d)\n", p.x, p.y);
}

// POR REFERENCIA: recebe ENDERECO da struct
// Usa seta (->) para acessar campos via ponteiro
// Alteracoes REFLETEM no original
void moverRef(Ponto *p) {
    p->x += 10;  // seta: acessa campo via ponteiro (equivale a (*p).x)
    p->y += 10;
    printf("  Dentro moverRef: (%d, %d)\n", p->x, p->y);
}

int main(void) {
    Ponto pt = {0, 0};

    printf("--- Struct POR VALOR ---\n");
    printf("Antes: (%d, %d)\n", pt.x, pt.y);
    moverValor(pt);  // passa copia -> SEM &
    printf("Depois: (%d, %d) (NAO mudou)\n", pt.x, pt.y);

    printf("\n--- Struct POR REFERENCIA ---\n");
    printf("Antes: (%d, %d)\n", pt.x, pt.y);
    moverRef(&pt);  // passa endereco -> COM &
    printf("Depois: (%d, %d) (mudou!)\n", pt.x, pt.y);

    system("pause");
    return 0;
}
```

### Exemplo 5: trocar dois valores (swap)

A troca de dois valores PRECISA ser por referencia. Se fosse por valor, trocariamos apenas copias e as variaveis originais continuariam iguais.

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// Troca dois valores usando ponteiros
// Precisa de referencia porque senao trocaria apenas copias
void troca(int *a, int *b) {
    int aux;
    aux = *a;  // guarda o valor apontado por a
    *a = *b;   // a recebe o valor apontado por b
    *b = aux;  // b recebe o valor guardado em aux
}

int main(void) {
    int x = 5, y = 10;

    printf("Antes: x = %d, y = %d\n", x, y);
    troca(&x, &y);  // passa os enderecos de x e y
    printf("Depois: x = %d, y = %d\n", x, y);

    system("pause");
    return 0;
}
```

### Exemplo 6: retornar multiplos valores via ponteiros (min e max)

Em C, uma funcao so pode retornar UM valor com `return`. Para devolver multiplos resultados, usamos ponteiros como parametros de saida. A funcao escreve diretamente nos enderecos recebidos.

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// Retorna multiplos valores via ponteiros
// Calcula min e max de um vetor e devolve pelos dois ponteiros
// Nao precisa de return: os valores ja sao escritos nos enderecos
void minMax(int v[], int tam, int *min, int *max) {
    int i;
    *min = v[0];  // comeca assumindo o primeiro elemento
    *max = v[0];

    for (i = 1; i < tam; i++) {
        if (v[i] < *min) {
            *min = v[i];  // atualiza o minimo
        }
        if (v[i] > *max) {
            *max = v[i];  // atualiza o maximo
        }
    }
    // nao precisa de return: valores ja estao nos enderecos
}

int main(void) {
    int nums[] = {3, 7, 1, 9, 4, 6, 2};
    int menor, maior;  // variaveis que receberao os resultados

    // passa enderecos de menor e maior como parametros de saida
    minMax(nums, 7, &menor, &maior);

    printf("Vetor: 3 7 1 9 4 6 2\n");
    printf("Menor = %d\n", menor);
    printf("Maior = %d\n", maior);

    system("pause");
    return 0;
}
```

### Tabela comparativa: VALOR vs REFERENCIA

| Aspecto | Por VALOR | Por REFERENCIA |
|---|---|---|
| O que e passado | Copia do valor | Endereco da variavel |
| Sintaxe na chamada | `funcao(n)` | `funcao(&n)` |
| Sintaxe no parametro | `int n` | `int *n` |
| Acesso dentro da funcao | `n` (direto) | `*n` (dereferenciado) |
| Alteracoes refletem? | NAO | SIM |
| Usa ponteiro? | NAO | SIM |
| Struct: acesso a campo | ponto (`.`) | seta (`->`) |
| Custo de memoria | copia tudo | so passa o endereco |

### Pegadinhas comuns

**Pegadinha 1: esquecer o `&` na chamada**
```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
dobrarRef(n);   // ERRADO: falta o & -> passa valor, nao endereco
dobrarRef(&n);  // CERTO: passa o endereco de n
```

**Pegadinha 2: esquecer o `*` dentro da funcao**
```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
void dobrarRef(int *x) {
    x = x * 2;   // ERRADO: x e o endereco, nao o valor
    *x = *x * 2;  // CERTO: *x acessa o valor no endereco
}
```

**Pegadinha 3: confundir ponto e seta em struct**
```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
void func(Ponto p)  { p.x;  }   // VALOR: usa ponto
void func(Ponto *p) { p->x; }   // REFERENCIA: usa seta
// p->x equivale a (*p).x
```

---

## 2. MODIFICADORES DE TIPO

**Palavras-chave:** MODIFICADORES DE TIPO, signed, unsigned, short, long, long long, INT_MIN, INT_MAX, limits.h, faixa de valor, estouro, overflow, fatorial, %ld, %lld, %u

### Conceito

Os modificadores de tipo alteram a faixa de valores que um tipo inteiro pode armazenar:

- **signed:** com sinal (aceita negativos e positivos). E o padrao, pode ser omitido.
- **unsigned:** sem sinal (so positivos). Dobra o valor maximo positivo.
- **short:** inteiro menor (geralmente 16 bits).
- **long:** inteiro maior (geralmente 32 bits).
- **long long:** inteiro ainda maior (geralmente 64 bits).

Podem ser combinados: `unsigned long long`, `short int` (o `int` pode ser omitido: `short`).

### Tabela de faixas de valor (limits.h)

| Tipo | Formatador | Minimo | Maximo | Bytes |
|---|---|---|---|---|
| `int` | `%d` | INT_MIN (-2147483648) | INT_MAX (2147483647) | 4 |
| `unsigned int` | `%u` | 0 | UINT_MAX (4294967295) | 4 |
| `short` | `%hd` | SHRT_MIN (-32768) | SHRT_MAX (32767) | 2 |
| `unsigned short` | `%hu` | 0 | USHRT_MAX (65535) | 2 |
| `long` | `%ld` | LONG_MIN | LONG_MAX | 4 |
| `unsigned long` | `%lu` | 0 | ULONG_MAX | 4 |
| `long long` | `%lld` | LLONG_MIN | LLONG_MAX (9223372036854775807) | 8 |
| `unsigned long long` | `%llu` | 0 | ULLONG_MAX | 8 |

### Exemplo: mostrar faixas com limits.h

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <limits.h>  // INT_MIN, INT_MAX, LLONG_MAX, etc.

int main(void) {
    int i = 2147483647;               // int comum (maximo)
    unsigned int u = 4000000000U;     // unsigned: so positivos
    short s = 32767;                  // short: menor
    long l = 2000000000L;             // long: maior (sufixo L)
    long long ll = 9000000000000000000LL;  // long long (sufixo LL)

    printf("int: %d\n", i);
    printf("  Faixa: %d a %d\n", INT_MIN, INT_MAX);

    printf("unsigned int: %u\n", u);
    printf("  Faixa: 0 a %u\n", UINT_MAX);

    printf("short: %hd\n", s);
    printf("  Faixa: %d a %d\n", SHRT_MIN, SHRT_MAX);

    printf("long: %ld\n", l);
    printf("  Faixa: %ld a %ld\n", LONG_MIN, LONG_MAX);

    printf("long long: %lld\n", ll);
    printf("  Faixa: %lld a %lld\n", LLONG_MIN, LLONG_MAX);

    system("pause");
    return 0;
}
```

### Exemplo: fatorial com long long (quando int estoura)

O fatorial cresce muito rapido. Com `int`, o fatorial estoura em 13! (overflow). Com `long long`, aguenta ate 20!.

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// Fatorial com long long para valores grandes
// int estoura em ~13!, long long aguenta ate ~20!
long long fatorial(int n) {
    long long resultado = 1;  // long long para nao estourar
    int i;
    for (i = 2; i <= n; i++) {
        resultado *= i;
    }
    return resultado;  // retorna long long
}

int main(void) {
    int n;

    printf("Digite um numero (0 a 20): ");
    scanf("%d", &n);

    if (n < 0 || n > 20) {
        printf("Valor invalido! Use 0 a 20.\n");
    } else {
        // %lld para imprimir long long
        printf("%d! = %lld\n", n, fatorial(n));
    }

    system("pause");
    return 0;
}
```

### Quando usar cada modificador

| Situacao | Tipo recomendado |
|---|---|
| Contador simples, valores pequenos | `int` |
| Contador que nunca sera negativo | `unsigned int` |
| Economizar memoria em vetor grande | `short` (se valores < 32767) |
| Valores acima de 2 bilhoes | `long long` |
| Fatorial, potencias grandes | `long long` |
| ID de registro, CPF, matricula | `unsigned long long` |

---

## 3. CONST (constante)

**Palavras-chave:** CONST, constante, const int, const char, parametro const, #define vs const, variavel somente leitura, promessa nao alterar

### const em variavel

`const` transforma uma variavel em SOMENTE LEITURA. O valor e definido na declaracao e NAO pode ser alterado depois. Tentar modificar gera erro de compilacao.

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
const int MAX = 100;  // constante tipada, nao pode mudar
// MAX = 200;  // ERRO: nao pode alterar const
```

### const em parametro de funcao

Quando colocamos `const` em um parametro ponteiro, estamos PROMETENDO que a funcao nao vai alterar o conteudo apontado. Isso e uma protecao: se alguem tentar modificar dentro da funcao, o compilador da erro.

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
void mostrar(const char *s) {
    // s[0] = 'X';  // ERRO: const proibe alteracao
    printf("%s", s);  // so pode LER, nao escrever
}
```

### Exemplo completo: const em variavel e parametro

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// const em variavel: valor NAO pode ser alterado
const int MAX = 100;  // constante tipada global

// const em parametro: promete nao alterar o que o ponteiro aponta
void mostrarString(const char *s) {
    // s[0] = 'X';  // ERRO se descomentar: const proibe alteracao
    printf("String: %s\n", s);  // so pode LER
}

// Sem const: pode alterar (perigoso se nao deveria)
void inverter(char *s) {
    int i, tam = 0;
    while (s[tam] != '\0') tam++;  // conta tamanho
    for (i = 0; i < tam / 2; i++) {
        char aux = s[i];
        s[i] = s[tam - 1 - i];
        s[tam - 1 - i] = aux;
    }
}

int main(void) {
    printf("MAX = %d\n", MAX);
    // MAX = 200;  // ERRO: nao pode alterar const

    char texto[] = "hello";  // array mutavel (nao e const)
    mostrarString(texto);   // passa para funcao const (seguro)

    inverter(texto);        // inverte (sem const, pode alterar)
    printf("Invertido: %s\n", texto);

    system("pause");
    return 0;
}
```

### const vs #define (diferencas)

| Aspecto | `const` | `#define` |
|---|---|---|
| O que e | Variavel de somente leitura | Substituicao de texto pelo pre-processador |
| Tem tipo? | SIM (int, float, etc.) | NAO (nao tem tipo) |
| Tem escopo? | SIM (local, global, bloco) | NAO (vale de onde aparece ate o fim do arquivo) |
| Ocupa memoria? | As vezes (compilador pode otimizar) | NAO (so substitui texto) |
| Erro detectado por | Compilador (mais seguro) | Pre-processador (erro dificil de achar) |
| Permite endereco? | SIM (pode usar &) | NAO |
| Exemplo | `const double PI = 3.14;` | `#define PI 3.14` |

### Exemplo: const vs #define

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// #define: substituicao de texto pelo pre-processador
// Nao tem tipo, nao tem escopo, erro dificil de detectar
#define PI 3.14159
#define MAX_ELEM 50

// const: variavel constante com tipo e escopo
// Tem tipo, tem escopo, erro detectado pelo compilador
const double PI_CONST = 3.14159;

int main(void) {
    double raio = 5.0;

    // Usando #define (substituicao de texto)
    double area1 = PI * raio * raio;
    printf("Area com #define: %.2f\n", area1);

    // Usando const (variavel tipada)
    double area2 = PI_CONST * raio * raio;
    printf("Area com const: %.2f\n", area2);

    // Resumo das diferencas:
    // #define: substituicao de texto, sem tipo, sem escopo
    // const: variavel com tipo e escopo, somente leitura

    system("pause");
    return 0;
}
```

---

## 4. ESCOPO DE VARIAVEIS

**Palavras-chave:** ESCOPO, variavel local, variavel global, variavel de bloco, sombreamento, shadowing, inicializacao com zero, lixo de memoria, visibilidade

### Conceito

Escopo e a regiao do codigo onde uma variavel existe e pode ser acessada.

- **Variavel LOCAL:** declarada dentro de uma funcao. So existe e e visivel dentro daquela funcao. Nao e inicializada automaticamente (comeca com LIXO). E destruida quando a funcao termina.

- **Variavel GLOBAL:** declarada FORA de todas as funcoes (geralmente no topo do arquivo). E acessivel por TODAS as funcoes do arquivo. E inicializada com ZERO automaticamente.

- **Variavel de BLOCO:** declarada dentro de um bloco `{ }`. So existe dentro daquele bloco. Desaparece ao fechar o `}`.

- **Sombreamento (shadowing):** quando uma variavel local tem o MESMO NOME de uma global. A local "esconde" a global dentro do seu escopo. A global nao e alterada.

### Exemplo: os 3 tipos de escopo + sombreamento

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int global = 100;  // GLOBAL: fora de funcoes, acessivel por todas
                   // Inicializada com 0 se nao atribuir valor

void exemplo(void) {
    int local = 10;  // LOCAL: so existe dentro desta funcao
                     // NAO e inicializada automaticamente (lixo se nao atribuir)
    printf("  exemplo: global = %d, local = %d\n", global, local);
}

int main(void) {
    int local = 5;  // LOCAL de main (outra, diferente da de exemplo)

    printf("global = %d\n", global);  // acessa global
    printf("local de main = %d\n", local);

    exemplo();  // chama funcao com sua propria local

    // Variavel de BLOCO: so existe dentro do { }
    {
        int bloco = 999;  // so existe neste bloco
        printf("dentro do bloco: bloco = %d\n", bloco);
    }
    // printf("%d", bloco);  // ERRO: bloco nao existe fora do bloco

    // SOMBREAMENTO (shadowing): local esconde global de mesmo nome
    {
        int global = 1;  // esconde a global de mesmo nome
        printf("shadowing: global = %d (escondeu a global 100)\n", global);
    }
    printf("fora do bloco: global = %d (global original)\n", global);

    system("pause");
    return 0;
}
```

### Resumo: inicializacao automatica

| Tipo de variavel | Inicializada com | Se nao atribuir valor |
|---|---|---|
| Local | NAO (lixo) | Valor indefinido (lixo de memoria) |
| Global | SIM (zero) | 0 para int, 0.0 para float, NULL para ponteiro |
| Static local | SIM (zero) | 0 |

---

## 5. STATIC (variavel estatica)

**Palavras-chave:** STATIC, variavel estatica, static local, manter valor entre chamadas, contador de chamadas, static global, linkagem interna, limite de escopo ao arquivo

### static em variavel local

Quando usamos `static` em uma variavel local, ela:
1. MANTem o valor entre chamadas da funcao (nao e destruida ao sair).
2. So e INICIALIZADA UMA VEZ (na primeira chamada).
3. Continua existindo na memoria enquanto o programa roda.

### Exemplo: contador de chamadas com static

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// static LOCAL: mantem o valor entre chamadas da funcao
// So e inicializada UMA vez (nao a cada chamada)
void contador(void) {
    static int vezes = 0;  // inicializada so na 1a chamada
                           // nas proximas, mantem o valor anterior
    vezes++;  // incrementa o valor que ficou da chamada anterior
    printf("Esta funcao foi chamada %d vezes\n", vezes);
}

int main(void) {
    contador();  // 1
    contador();  // 2
    contador();  // 3
    contador();  // 4

    // Sem static, sempre imprimiria 1 (reiniciaria a cada chamada)

    system("pause");
    return 0;
}
```

### static em variavel/funcao global (linkagem interna)

Quando usamos `static` em uma variavel global ou funcao, limitamos o escopo ao ARQUIVO atual (linkagem interna). Outros arquivos do projeto nao conseguem acessar essa variavel/funcao, mesmo com `extern`. Isso e util em projetos com multiplos arquivos `.c` para esconder detalhes de implementacao.

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
// --- ARQUIVO: util.c ---
// static global: so visivel neste arquivo (util.c)
// Outros arquivos nao conseguem acessar com extern
static int contadorInterno = 0;

// static em funcao: so pode ser chamada dentro deste arquivo
static void helper(void) {
    contadorInterno++;
}

// Funcao publica (sem static): pode ser chamada de outros arquivos
void incrementar(void) {
    helper();  // chama a funcao static interna
}
```

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
// --- ARQUIVO: main.c ---
// extern int contadorInterno;  // NAO funciona: contadorInterno e static
// helper();  // NAO funciona: helper e static

extern void incrementar(void);  // funciona: nao e static

// Resumo: static global/funcao = esconde do resto do projeto
```

---

## 6. PRECEDENCIA E ASSOCIATIVIDADE DE OPERADORES

**Palavras-chave:** PRECEDENCIA, ASSOCIATIVIDADE, ordem de avaliacao, operadores, parenteses, *p++, a & b == c, armadilha de precedencia, regra pratica

### Conceito

**Precedencia** define qual operador e avaliado PRIMEIRO quando ha varios na mesma expressao. Operadores de precedencia MAIOR sao avaliados antes.

**Associatividade** define a ordem quando dois operadores tem a MESMA precedencia:
- **Esquerda para direita:** avalia da esquerda para a direita (ex: `a - b - c` = `(a - b) - c`).
- **Direita para esquerda:** avalia da direita para a esquerda (ex: `a = b = c` = `a = (b = c)`).

### Tabela completa de precedencia (do MAIOR para o MENOR)

| Nivel | Operadores | Descricao | Associatividade |
|---|---|---|---|
| 1 (maior) | `()` `[]` `->` `.` | chamada, indice, acesso a campo | esquerda -> direita |
| 2 | `!` `~` `++` `--` `+` `-` `*` `&` `(tipo)` `sizeof` | unarios (nao confundir * e & binarios) | direita -> esquerda |
| 3 | `*` `/` `%` | multiplicacao, divisao, modulo | esquerda -> direita |
| 4 | `+` `-` | adicao, subtracao | esquerda -> direita |
| 5 | `<<` `>>` | deslocamento de bits | esquerda -> direita |
| 6 | `<` `<=` `>` `>=` | comparacao relacional | esquerda -> direita |
| 7 | `==` `!=` | comparacao de igualdade | esquerda -> direita |
| 8 | `&` | AND bit a bit | esquerda -> direita |
| 9 | `^` | XOR bit a bit | esquerda -> direita |
| 10 | `\|` | OR bit a bit | esquerda -> direita |
| 11 | `&&` | AND logico | esquerda -> direita |
| 12 | `\|\|` | OR logico | esquerda -> direita |
| 13 | `?:` | ternario (condicional) | direita -> esquerda |
| 14 | `=` `+=` `-=` `*=` `/=` `%=` `<<=` `>>=` `&=` `^=` `\|=` | atribuicao | direita -> esquerda |
| 15 (menor) | `,` | virgula (separa expressoes) | esquerda -> direita |

### Regra pratica

**NA DUVIDA, USE PARENTESES.** Parenteses sempre tem a maior precedencia e deixam a intencao clara. Nao confie na memoria para a tabela completa.

### Armadilhas de precedencia (exemplos)

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main(void) {
    // Armadilha 1: *p++ vs (*p)++
    // * e ++ tem a mesma precedencia (nivel 2), associatividade direita->esquerda
    // *p++ significa *(p++) -> incrementa o PONTEIRO, depois le o valor antigo
    // (*p)++ incrementa o VALOR apontado
    int v[] = {10, 20};
    int *p = v;

    int a = *p++;  // a = 10, p agora aponta para v[1]
    printf("*p++: a = %d, *p = %d\n", a, *p);

    p = v;  // reset: p volta a apontar para v[0]
    int b = (*p)++;  // b = 10, v[0] agora e 11 (incrementou o valor)
    printf("(*p)++: b = %d, v[0] = %d\n", b, v[0]);

    // Armadilha 2: a & b == c
    // == tem MAIOR precedencia que & (bitwise AND)
    // Entao a & b == c vira a & (b == c) -- provavelmente NAO e o que se quer
    int x = 1, y = 1, z = 1;
    // if (x & y == z)     // vira: x & (y == z) = 1 & 1 = 1 (verdadeiro)
    // Correto: if ((x & y) == z)  -> parenteses deixam explicito
    if ((x & y) == z) {
        printf("(x & y) == z: verdadeiro (com parenteses)\n");
    }

    // Armadilha 3: atribuicao vs comparacao
    // = (atribuicao) tem precedencia MENOR que == (comparacao)
    // if (x = 5) atribui 5 a x e e sempre verdadeiro (5 != 0)
    // if (x == 5) compara x com 5
    // Cuidado para nao digitar = no lugar de ==
    int n = 3;
    if (n == 5) {  // comparacao correta
        printf("n e 5\n");
    } else {
        printf("n NAO e 5 (n = %d)\n", n);
    }

    // Armadilha 4: && e || (curto-circuito)
    // && tem precedencia MAIOR que ||
    // a || b && c vira a || (b && c)
    // Use parenteses: (a || b) && c se for o que quer
    int r = 1;
    if (r || r && r) {  // vira: r || (r && r)
        printf("r || (r && r): verdadeiro\n");
    }

    system("pause");
    return 0;
}
```

---

## 7. TABELA COMPLETA DE FORMATADORES printf/scanf

**Palavras-chave:** FORMATADORES, printf, scanf, %d, %ld, %lld, %u, %f, %lf, %c, %s, %x, %o, %e, %g, %p, %%, largura, precisao, flags, alinhamento, %5d, %-5d, %05d, %.2f, %10.2f

### Tabela de formatadores

| Formatador | Tipo | Uso | printf | scanf |
|---|---|---|---|---|
| `%d` | int | inteiro com sinal | SIM | SIM |
| `%hd` | short | inteiro curto | SIM | SIM |
| `%ld` | long | inteiro longo | SIM | SIM |
| `%lld` | long long | inteiro muito longo | SIM | SIM |
| `%u` | unsigned int | inteiro sem sinal | SIM | SIM |
| `%hu` | unsigned short | inteiro curto sem sinal | SIM | SIM |
| `%lu` | unsigned long | inteiro longo sem sinal | SIM | SIM |
| `%llu` | unsigned long long | inteiro muito longo sem sinal | SIM | SIM |
| `%f` | float / double | decimal (6 casas padrao) | SIM | NAO |
| `%lf` | double | decimal (usado no scanf) | NAO* | SIM |
| `%c` | char | um caractere | SIM | SIM |
| `%s` | char[] (string) | texto (ate \0) | SIM | SIM |
| `%x` | int / unsigned | hexadecimal minusculo (a-f) | SIM | SIM |
| `%X` | int / unsigned | hexadecimal maiusculo (A-F) | SIM | NAO |
| `%o` | int / unsigned | octal | SIM | SIM |
| `%e` | float / double | notacao cientifica (1.5e+03) | SIM | SIM |
| `%E` | float / double | cientifico (E maiusculo) | SIM | NAO |
| `%g` | float / double | cientifico compacto (%e ou %f) | SIM | NAO |
| `%p` | void* | endereco de ponteiro | SIM | NAO |
| `%%` | (literal) | imprime o caractere % | SIM | NAO |

> *Nota: no printf, `%lf` tambem funciona para double (C99+), mas o padrao classico e `%f` no printf e `%lf` no scanf.

### Largura de campo

| Especificador | Efeito | Exemplo (valor 42) |
|---|---|---|
| `%5d` | largura minima 5, alinhado a DIREITA | `[   42]` |
| `%-5d` | largura 5, alinhado a ESQUERDA | `[42   ]` |
| `%05d` | largura 5, preenchido com ZEROS | `[00042]` |
| `%+d` | mostra sinal de + ou - | `[+42]` |
| `% d` | espaco no lugar do sinal + | `[ 42]` |

### Precisao

| Especificador | Efeito | Exemplo (valor 3.14159) |
|---|---|---|
| `%.2f` | 2 casas decimais | `3.14` |
| `%.0f` | 0 casas (sem decimais) | `3` |
| `%10.2f` | largura 10, 2 casas | `[      3.14]` |
| `%-10.2f` | largura 10 esquerda, 2 casas | `[3.14      ]` |
| `%.5s` | no maximo 5 caracteres da string | `hello` (de "hello world") |

### Flags (modificadores)

| Flag | Nome | Efeito |
|---|---|---|
| `-` | menos | alinha a ESQUERDA (padrao e direita) |
| `+` | mais | mostra sinal de + para positivos |
| `0` | zero | preenche com zeros a esquerda |
| (espaco) | espaco | coloca espaco no lugar do sinal + |
| `#` | hash | formato alternativo (0x para hexa, 0 para octal) |

### Exemplo completo: formatadores, largura, precisao e flags

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main(void) {
    int inteiro = 42;
    double real = 3.14159;
    char letra = 'A';
    char nome[] = "Franco";
    unsigned int semSinal = 4000000000U;
    long long grande = 9000000000000LL;
    int *ptr = &inteiro;

    printf("=== FORMATADORES BASICOS ===\n");
    printf("int %%d: %d\n", inteiro);
    printf("unsigned %%u: %u\n", semSinal);
    printf("long long %%lld: %lld\n", grande);
    printf("float/double %%f: %f\n", real);
    printf("char %%c: %c\n", letra);
    printf("string %%s: %s\n", nome);
    printf("hexa %%x: %x\n", inteiro);
    printf("hexa maiusculo %%X: %X\n", inteiro);
    printf("octal %%o: %o\n", inteiro);
    printf("cientifico %%e: %e\n", real);
    printf("cientifico compacto %%g: %g\n", real);
    printf("ponteiro %%p: %p\n", (void*)ptr);
    printf("percentual %%%%\n");

    printf("\n=== LARGURA DE CAMPO ===\n");
    printf("%%5d: [%5d]\n", inteiro);        // [   42]
    printf("%%-5d: [%-5d]\n", inteiro);      // [42   ]
    printf("%%05d: [%05d]\n", inteiro);      // [00042]

    printf("\n=== PRECISAO ===\n");
    printf("%%.2f: [%.2f]\n", real);          // [3.14]
    printf("%%.0f: [%.0f]\n", real);          // [3]
    printf("%%10.2f: [%10.2f]\n", real);      // [      3.14]
    printf("%%-10.2f: [%-10.2f]\n", real);    // [3.14      ]

    printf("\n=== FLAGS ===\n");
    printf("%%+d: [%+d]\n", inteiro);        // [+42]
    printf("%% d: [% d]\n", inteiro);         // [ 42]
    printf("%%#x: [%#x]\n", inteiro);        // [0x2a]

    // Exemplo de leitura com scanf e fflush(stdin)
    int num;
    char ch;
    printf("\nDigite um numero: ");
    scanf("%d", &num);
    fflush(stdin);  // limpa o buffer do teclado (resto do Enter)
    printf("Digite uma letra: ");
    scanf("%c", &ch);
    printf("Numero = %d, Letra = %c\n", num, ch);

    system("pause");
    return 0;
}
```


#############################################################
# PARTE 2 - ENUM, UNION, BITWISE, ARGV, STDBOOL
#############################################################

## 1. ENUM (tipo enumerado)

Palavras-chave: ENUM, tipo enumerado, enum Dia, DOM SEG TER, valores comecam em 0, typedef enum, switch com enum, legibilidade.

ENUM e um tipo definido pelo programador que cria uma lista de constantes inteiras nomeadas. Serve para dar nomes significativos a valores inteiros, deixando o codigo mais legivel. Por padrao, o primeiro valor vale 0 e cada proximo incrementa de 1.

Declaracao basica:

```c
#include <stdio.h>
#include <conio.h>   // getch, system
#include <stdlib.h>  // system

// PALAVRA-CHAVE: ENUM
// Declaracao do tipo enumerado Dia.
// DOM vale 0, SEG vale 1, TER vale 2, ... SAB vale 6.
// Os valores comecam em 0 e incrementam de 1 em 1 automaticamente.
enum Dia {
    DOM,   // 0 - domingo
    SEG,   // 1 - segunda
    TER,   // 2 - terca
    QUA,   // 3 - quarta
    QUI,   // 4 - quinta
    SEX,   // 5 - sexta
    SAB    // 6 - sabado (sem virgula no ultimo)
};

// PALAVRA-CHAVE: typedef enum
// typedef cria um alias (apelido) para o tipo.
// Assim podemos escrever "Dia hoje;" em vez de "enum Dia hoje;".
typedef enum Dia Dia;

// Tambem e possivel atribuir valores especificos aos enumeradores.
// Aqui A=1, B=2, C=4 (pode pular valores, nao precisa ser sequencial).
enum Codigos {
    A = 1,
    B = 2,
    C = 4   // saltei o 3 de proposito
};

// typedef enum feito na mesma linha (forma curta e comum).
typedef enum { FALSE_T = 0, TRUE_T = 1 } Booleano;

int main(void) {
    system("cls"); // limpa a tela (Windows)

    // Declarar variavel do tipo enum e atribuir um valor.
    Dia hoje = SEG; // hoje recebe 1 (SEG)
    printf("Hoje (enum) = %d\n", hoje); // imprime 1

    // PALAVRA-CHAVE: switch com enum
    // Usar enum no switch deixa o codigo muito mais legivel.
    // Em vez de case 1, usamos case SEG, que tem significado claro.
    switch (hoje) {
        case DOM:
            printf("Domingo - descanso\n");
            break;
        case SEG:
            printf("Segunda - inicio da semana\n");
            break;
        case TER:
            printf("Terca\n");
            break;
        case QUA:
            printf("Quarta\n");
            break;
        case QUI:
            printf("Quinta\n");
            break;
        case SEX:
            printf("Sexta - quase ferias\n");
            break;
        case SAB:
            printf("Sabado - ferias\n");
            break;
        default:
            printf("Dia invalido\n");
    }

    // Mostrando os valores atribuidos manualmente.
    printf("A=%d B=%d C=%d\n", A, B, C); // 1, 2, 4
    printf("TRUE_T=%d FALSE_T=%d\n", TRUE_T, FALSE_T); // 1, 0

    // Vantagem do enum: legibilidade.
    // Em vez de if (dia == 1) escrevemos if (dia == SEG).
    // O nome SEG documenta a intencao do programador.

    printf("\nPressione qualquer tecla para sair...");
    fflush(stdin); // limpa o buffer do teclado antes do getch
    getch();
    return 0;
}
```

---

## 2. UNION (uniao)

Palavras-chave: UNION, uniao, union Dado, compartilha memoria, tamanho do maior campo, struct vs union, economizar memoria, tipo variante.

UNION e como uma struct, mas todos os campos compartilham a MESMA area de memoria. So faz sentido usar UM campo por vez. O tamanho da union e igual ao tamanho do seu maior campo. Serve para economizar memoria quando uma variavel pode guardar tipos diferentes em momentos diferentes (tipo variante).

Diferenca fundamental:
- STRUCT: cada campo tem sua propria memoria. Tamanho = soma dos campos (com padding).
- UNION: todos os campos compartilham a mesma memoria. Tamanho = maior campo.

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

// PALAVRA-CHAVE: UNION
// Declaracao da uniao Dado.
// Os tres campos (i, f, s) ocupam a MESMA area de memoria.
// So podemos usar um deles por vez.
union Dado {
    int   i;      // 4 bytes
    float f;      // 4 bytes
    char  s[20];  // 20 bytes (maior campo)
};
// Tamanho da union Dado = 20 bytes (tamanho do maior campo, s[20]).

// Para comparar, uma struct com os mesmos campos:
struct DadoStruct {
    int   i;      // 4 bytes
    float f;      // 4 bytes
    char  s[20];  // 20 bytes
};
// Tamanho da struct = 4 + 4 + 20 = 28 bytes (cada campo separado).

// typedef para criar alias (estilo do professor).
typedef union Dado Dado;
typedef struct DadoStruct DadoStruct;

int main(void) {
    system("cls");

    Dado d; // declara variavel do tipo union

    // PALAVRA-CHAVE: so um campo por vez
    // Guardamos um inteiro.
    d.i = 42;
    printf("Como int: %d\n", d.i); // 42

    // Agora guardamos um float NO MESMO ESPACO.
    // Isso sobrescreve o int anterior - nao podemos mais usar d.i.
    d.f = 3.14f;
    printf("Como float: %.2f\n", d.f); // 3.14
    // Se imprimirmos d.i agora, o valor sera lixo (a memoria mudou).

    // Agora guardamos uma string no mesmo espaco.
    strcpy(d.s, "Ola mundo"); // copia a string para d.s
    printf("Como string: %s\n", d.s); // Ola mundo

    // Mostrando os tamanhos com sizeof.
    printf("\nTamanho da union Dado: %zu bytes\n", sizeof(Dado));        // 20
    printf("Tamanho da struct DadoStruct: %zu bytes\n", sizeof(DadoStruct)); // 28

    // TABELA struct vs union:
    // +-------------------+----------------------------+--------------------------+
    // | Caracteristica    | STRUCT                     | UNION                    |
    // +-------------------+----------------------------+--------------------------+
    // | Memoria           | Separada para cada campo   | Compartilhada (um so)    |
    // | Tamanho           | Soma dos campos (+padding) | Maior campo              |
    // | Campos usados     | Todos ao mesmo tempo       | So um por vez            |
    // | Uso tipico        | Agrupar dados relacionados | Economizar memoria       |
    // +-------------------+----------------------------+--------------------------+

    // PALAVRA-CHAVE: quando usar union
    // - Economizar memoria quando a variavel guarda tipos diferentes.
    // - Tipo variante: uma estrutura que pode conter dados de tipos diferentes
    //   dependendo de uma "tag" (campo que indica qual tipo esta guardado).

    printf("\nPressione qualquer tecla para sair...");
    fflush(stdin); // limpa o buffer do teclado
    getch();
    return 0;
}
```

---

## 3. OPERADORES BITWISE (bit a bit)

Palavras-chave: BITWISE, bit a bit, AND &, OR |, XOR ^, NOT ~, shift left <<, shift right >>, mascara, ligar bit, desligar bit, testar bit, flags, criptografia XOR.

Operadores bitwise manipulam os bits de numeros inteiros diretamente. Sao uteis para flags, mascaras, criptografia simples e operacoes de baixo nivel.

Tabela verdade bitwise (bit a bit):

```
 a | b | a&b (AND) | a|b (OR) | a^b (XOR) | ~a (NOT)
---+---+----------+---------+----------+---------
 0 | 0 |    0     |    0    |    0     |    1
 0 | 1 |    0     |    1    |    1     |    1
 1 | 0 |    0     |    1    |    1     |    0
 1 | 1 |    1     |    1    |    0     |    0
```

- AND (&): 1 so se ambos forem 1. Usa-se para MASCARA (testar bits).
- OR (|): 1 se pelo menos um for 1. Usa-se para LIGAR bits.
- XOR (^): 1 se forem diferentes. Usa-se para INVERTER bits.
- NOT (~): inverte todos os bits.
- << (shift left): desloca bits para a esquerda. Equivale a MULTIPLICAR por 2.
- >> (shift right): desloca bits para a direita. Equivale a DIVIDIR por 2.

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// Funcao auxiliar: imprime um inteiro em binario (8 bits).
void imprimirBinario(int n) {
    int i;
    // Percorre do bit mais significativo (7) ao menos (0).
    for (i = 7; i >= 0; i--) {
        // (n >> i) desloca ate o bit i; & 1 isola so esse bit.
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

int main(void) {
    system("cls");

    int a = 12; // 00001100 em binario
    int b = 10; // 00001010 em binario

    // PALAVRA-CHAVE: AND & (mascara)
    // 12 & 10 = 00001000 = 8
    printf("12 & 10 = %d\n", a & b); // 8

    // PALAVRA-CHAVE: OR | (ligar bit)
    // 12 | 10 = 00001110 = 14
    printf("12 | 10 = %d\n", a | b); // 14

    // PALAVRA-CHAVE: XOR ^ (ou exclusivo, inverter bit)
    // 12 ^ 10 = 00000110 = 6
    printf("12 ^ 10 = %d\n", a ^ b); // 6

    // PALAVRA-CHAVE: NOT ~ (inverte todos os bits)
    // ~12 inverte todos os bits (resultado depende do tipo/sinal).
    printf("~12 = %d\n", ~a); // -13 (em complemento de 2)

    // PALAVRA-CHAVE: shift left << (multiplica por 2)
    // 12 << 1 = 24 (deslocou 1 bit a esquerda = x2)
    printf("12 << 1 = %d\n", a << 1); // 24
    // 12 << 3 = 96 (deslocou 3 bits = x8, pois 2^3=8)
    printf("12 << 3 = %d\n", a << 3); // 96

    // PALAVRA-CHAVE: shift right >> (divide por 2)
    // 12 >> 1 = 6 (deslocou 1 bit a direita = /2)
    printf("12 >> 1 = %d\n", a >> 1); // 6
    // 12 >> 2 = 3 (deslocou 2 bits = /4, pois 2^2=4)
    printf("12 >> 2 = %d\n", a >> 2); // 3

    // ===== APLICACOES PRATICAS COM FLAGS =====
    // PALAVRA-CHAVE: ligar bit 3
    int x = 0;            // 00000000
    x |= (1 << 3);        // liga o bit 3 -> 00001000 = 8
    printf("Apos ligar bit 3: %d\n", x); // 8

    // PALAVRA-CHAVE: testar bit
    // (x & (1 << 3)) retorna nao-zero se o bit 3 estiver ligado.
    if (x & (1 << 3)) {
        printf("Bit 3 esta LIGADO\n");
    }

    // PALAVRA-CHAVE: desligar bit
    x &= ~(1 << 3);       // desliga o bit 3 -> 00000000 = 0
    printf("Apos desligar bit 3: %d\n", x); // 0
    if (!(x & (1 << 3))) {
        printf("Bit 3 esta DESLIGADO\n");
    }

    // PALAVRA-CHAVE: criptografia XOR
    // XOR com uma chave cifra a mensagem; XOR de novo com a mesma chave decifra.
    char chave = 123;        // chave secreta
    char msg = 'A';          // caractere original
    char cifrado = msg ^ chave; // cifra
    char decifrado = cifrado ^ chave; // decifra (volta ao original)
    printf("Original=%d Cifrado=%d Decifrado=%d\n", msg, cifrado, decifrado);
    // decifrado == msg (o XOR aplicado duas vezes cancela)

    printf("\nPressione qualquer tecla para sair...");
    fflush(stdin); // limpa o buffer do teclado
    getch();
    return 0;
}
```

---

## 4. ARGC E ARGV (linha de comando)

Palavras-chave: ARGC, ARGV, linha de comando, int main(int argc char *argv[]), argv[0], atoi, validar argc, argumentos do programa.

argc e argv sao parametros da funcao main que permitem ler argumentos passados na linha de comando quando o programa e executado.

- argc: quantidade de argumentos (INCLUI o nome do programa, entao o minimo e 1).
- argv: vetor de strings (matriz de caracteres) com os argumentos. argv[0] e o nome do programa, argv[1] e o primeiro argumento, etc.

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>  // atoi, system

// PALAVRA-CHAVE: ARGC e ARGV
// argc = quantidade de argumentos (inclui o nome do programa).
// argv = vetor de strings com os argumentos.
// argv[0] = nome do programa.
// argv[1] = primeiro argumento, argv[2] = segundo, etc.
int main(int argc, char *argv[]) {
    system("cls");

    // Sempre imprimir argc para conferir quantos argumentos vieram.
    printf("Quantidade de argumentos (argc): %d\n", argc);

    // Mostra todos os argumentos recebidos.
    int i;
    for (i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    // PALAVRA-CHAVE: validar argc antes de usar argv
    // Se o programa espera 2 numeros (argc == 3: programa + num1 + num2),
    // devemos verificar antes de acessar argv[1] e argv[2].
    if (argc != 3) {
        printf("\nUso: %s <numero1> <numero2>\n", argv[0]);
        printf("Exemplo: soma 5 3\n");
        printf("Pressione qualquer tecla para sair...");
        fflush(stdin); // limpa o buffer do teclado
        getch();
        return 1; // retorna 1 indicando erro
    }

    // PALAVRA-CHAVE: atoi
    // argv[1] e argv[2] sao strings. Usamos atoi para converter para int.
    // atoi = "ASCII to Integer".
    int n1 = atoi(argv[1]); // converte "5" para 5
    int n2 = atoi(argv[2]); // converte "3" para 3
    int soma = n1 + n2;

    printf("\nSoma de %d + %d = %d\n", n1, n2, soma);

    printf("\nPressione qualquer tecla para sair...");
    fflush(stdin); // limpa o buffer do teclado
    getch();
    return 0;
}

// Como compilar e executar:
//   gcc soma.c -o soma
//   soma 5 3
// Resultado:
//   argc = 3
//   argv[0] = "soma"
//   argv[1] = "5"
//   argv[2] = "3"
//   Soma de 5 + 3 = 8
//
// Observacao: se rodar direto no IDE sem argumentos, argc sera 1 e o
// programa vai cair no if (argc != 3) e pedir os argumentos.
```

---

## 5. STDBOOL.H (tipo bool em C99)

Palavras-chave: STDBOOL.H, bool, true, false, C99, tipo booleano, ehPrimo, funcao que retorna bool.

O C original nao tinha tipo booleano (usava-se int: 0 = falso, qualquer valor diferente de 0 = verdadeiro). A partir do padrao C99, o cabecalho <stdbool.h> introduz o tipo bool e as constantes true (1) e false (0).

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h> // PALAVRA-CHAVE: STDBOOL.H (tipo bool em C99)

// PALAVRA-CHAVE: funcao que retorna bool
// Com stdbool.h podemos declarar funcoes que retornam bool (true/false).
// Isso deixa o codigo mais claro do que retornar int.
bool ehPrimo(int n) {
    if (n <= 1) {
        return false; // 0 e 1 nao sao primos
    }
    int i;
    // Testa divisores de 2 ate a raiz de n (aproximado).
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false; // achou divisor, nao e primo
        }
    }
    return true; // nao achou divisor, e primo
}

// Antes do C99 (sem stdbool.h) a mesma funcao seria:
// int ehPrimoAntigo(int n) {
//     if (n <= 1) return 0;       // 0 = falso
//     ...
//     return 1;                   // 1 = verdadeiro
// }
// O comportamento e o mesmo, mas bool deixa a intencao explicita.

int main(void) {
    system("cls");

    // PALAVRA-CHAVE: bool, true, false
    bool terminou = false; // declara variavel bool
    int numero;

    printf("Digite um numero (0 para sair): ");
    scanf("%d", &numero);

    while (!terminou) {
        if (numero == 0) {
            terminou = true; // encerra o loop
        } else {
            // Usamos a funcao que retorna bool diretamente no if.
            if (ehPrimo(numero)) {
                printf("%d e PRIMO\n", numero);
            } else {
                printf("%d NAO e primo\n", numero);
            }
            printf("Digite outro numero (0 para sair): ");
            fflush(stdin); // limpa o buffer do teclado antes do scanf
            scanf("%d", &numero);
        }
    }

    printf("Programa encerrado.\n");
    printf("\nPressione qualquer tecla para sair...");
    fflush(stdin); // limpa o buffer do teclado
    getch();
    return 0;
}
```

---

## 6. OPERADOR VIRGULA

Palavras-chave: OPERADOR VIRGULA, virgula no for, for (i=0 j=10), avaliacao esquerda para direita, resultado e o ultimo, cuidado int a = (1,2,3).

O operador virgula (,) avalia varias expressoes da esquerda para a direita e o resultado final e o valor da expressao mais a direita. E muito usado no cabecalho do for para inicializar/atualizar mais de uma variavel.

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main(void) {
    system("cls");

    int i, j;

    // PALAVRA-CHAVE: operador virgula no for
    // Inicializamos i=0 e j=10 ao mesmo tempo.
    // A cada iteracao: i++ e j-- (i sobe, j desce).
    // Condicao: i < j (para quando se cruzarem).
    for (i = 0, j = 10; i < j; i++, j--) {
        printf("i=%d  j=%d\n", i, j);
    }
    // Saida:
    // i=0  j=10
    // i=1  j=9
    // i=2  j=8
    // i=3  j=7
    // i=4  j=6
    // (para quando i=5, j=5, pois 5 < 5 e falso)

    // PALAVRA-CHAVE: avaliacao da esquerda para direita
    // Todas as expressoes sao avaliadas, mas o resultado e o da direita.
    int x;
    x = (printf("Ola "), printf("Mundo!\n"), 42);
    // printf("Ola ") e avaliado (imprime Ola )
    // printf("Mundo!\n") e avaliado (imprime Mundo!)
    // 42 e o resultado final, atribuido a x.
    printf("x = %d\n", x); // x = 42

    // PALAVRA-CHAVE: cuidado com atribuicao
    // int a = (1, 2, 3); // a vale 3 (o ultimo).
    // Isso NAO inicializa tres variaveis - e um erro comum de iniciante.
    int a = (1, 2, 3);
    printf("a = %d\n", a); // a = 3

    // Para inicializar varias variaveis use declaracoes separadas:
    // int a = 1, b = 2, c = 3;  // forma correta

    // Outro uso: trocar valores sem variavel auxiliar (so para entender).
    int p = 5, q = 9;
    // A expressao entre parenteses usa virgula: avalia da esq para dir.
    // p recebe q, depois q recebe o valor antigo de p.
    // (Cuidado: esse truque e confuso, prefira variavel auxiliar na prova.)
    q = (p + q) - (p = q); // truque aritmetico - evite em prova
    // Melhor forma (clara e segura):
    // int aux = p; p = q; q = aux;

    printf("\nPressione qualquer tecla para sair...");
    fflush(stdin); // limpa o buffer do teclado
    getch();
    return 0;
}
```

---

## 7. DANGLING POINTER (ponteiro solto)

Palavras-chave: DANGLING POINTER, ponteiro solto, ponteiro para memoria liberada, free, p = NULL, comportamento indefinido, testar if (p != NULL).

DANGLING POINTER e um ponteiro que continua apontando para uma area de memoria que ja foi liberada com free(). Acessar esse ponteiro causa comportamento indefinido (pode dar erro, pode parecer funcionar, pode corromper outros dados). A solucao e sempre colocar o ponteiro como NULL logo apos o free e testar if (p != NULL) antes de usar.

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>  // malloc, free, system

int main(void) {
    system("cls");

    // Alocamos memoria dinamicamente.
    int *p = (int *) malloc(sizeof(int));
    if (p == NULL) {
        printf("Erro de alocacao\n");
        return 1;
    }

    *p = 100; // guarda valor na memoria alocada
    printf("Valor antes do free: %d\n", *p); // 100

    // PALAVRA-CHAVE: free libera a memoria
    free(p);
    // A partir daqui a memoria foi devolvida ao sistema.
    // p ainda guarda o endereco antigo, mas aquela memoria NAO e mais nossa.

    // PALAVRA-CHAVE: dangling pointer (ERRO!)
    // *p = 10;  // COMPORTAMENTO INDEFINIDO!
    // printf("%d", *p); // tambem perigoso
    // Pode parecer funcionar, pode dar crash, pode corromper dados.
    // NUNCA faca isso em prova - o professor derruba.

    // PALAVRA-CHAVE: solucao - p = NULL apos free
    p = NULL; // agora p nao aponta para lugar nenhum

    // PALAVRA-CHAVE: testar if (p != NULL) antes de usar
    if (p != NULL) {
        *p = 10; // so entra aqui se p for valido
        printf("Valor: %d\n", *p);
    } else {
        printf("Ponteiro nulo - memoria ja liberada, nao posso usar.\n");
    }

    // Resumo da regra de ouro:
    // 1. Sempre teste o retorno do malloc (pode falhar e retornar NULL).
    // 2. Sempre de free quando nao precisar mais da memoria.
    // 3. Logo apos o free, faca p = NULL.
    // 4. Antes de usar o ponteiro, teste if (p != NULL).

    printf("\nPressione qualquer tecla para sair...");
    fflush(stdin); // limpa o buffer do teclado
    getch();
    return 0;
}
```

---

## 8. FUNCAO QUE RETORNA PONTEIRO

Palavras-chave: FUNCAO QUE RETORNA PONTEIRO, retornar ponteiro, malloc dentro de funcao, criarVetor, nao retornar ponteiro para variavel local, variavel static, quem chama da free.

Uma funcao pode retornar um ponteiro. O uso mais seguro e alocar memoria dinamicamente (malloc) dentro da funcao e retornar esse ponteiro. O cuidado principal: NUNCA retornar o endereco de uma variavel local, pois a variavel deixa de existir quando a funcao termina - o ponteiro retornado vira um dangling pointer. Para retornar um ponteiro valido, use malloc (memoria do heap) ou uma variavel static.

```c
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>  // malloc, free, system

// PALAVRA-CHAVE: funcao que retorna ponteiro
// Esta funcao aloca um vetor dinamicamente, preenche e retorna o ponteiro.
// O tipo de retorno e "int *" (ponteiro para int).
int *criarVetor(int n) {
    // Aloca memoria para n inteiros no heap.
    int *v = (int *) malloc(n * sizeof(int));
    if (v == NULL) {
        printf("Erro de alocacao\n");
        return NULL; // retorna NULL se falhar
    }

    // Preenche o vetor com valores de exemplo.
    int i;
    for (i = 0; i < n; i++) {
        v[i] = i * 10; // 0, 10, 20, 30, ...
    }

    // Retorna o ponteiro para a memoria alocada.
    // Como foi alocada com malloc (heap), ela continua valida apos a funcao.
    return v;
}

// PALAVRA-CHAVE: NAO retornar ponteiro para variavel local (ERRO!)
// int *funcaoErrada(void) {
//     int x = 10;        // variavel local (na pilha/stack)
//     return &x;         // ERRO! x deixa de existir ao sair da funcao.
// }
// Quem receber esse ponteiro estara acessando memoria invalida (dangling).

// PALAVRA-CHAVE: variavel static (alternativa valida)
// Se precisar retornar o endereco de uma variavel, declare-a como static.
// Variaveis static sobrevivem apos o fim da funcao.
int *obterReferencia(void) {
    static int x = 99; // static: nao e destruida ao sair da funcao
    return &x;          // valido: x continua existindo
}

int main(void) {
    system("cls");

    int n = 5;

    // A funcao retorna um ponteiro para memoria alocada.
    int *vetor = criarVetor(n);
    if (vetor == NULL) {
        printf("Falha ao criar vetor\n");
        return 1;
    }

    // Usamos o vetor normalmente.
    int i;
    printf("Vetor criado pela funcao:\n");
    for (i = 0; i < n; i++) {
        printf("vetor[%d] = %d\n", i, vetor[i]);
    }

    // Testando a funcao com variavel static.
    int *ref = obterReferencia();
    printf("Valor da variavel static: %d\n", *ref); // 99

    // PALAVRA-CHAVE: quem chama precisa dar free
    // A funcao alocou com malloc, entao o main (quem chamou) e responsavel
    // por liberar a memoria com free quando nao precisar mais.
    free(vetor);
    vetor = NULL; // boa pratica: anular apos free

    // NAO damos free em "ref" porque aponta para variavel static,
    // que nao foi alocada com malloc (nao se da free em variavel static).

    printf("\nPressione qualquer tecla para sair...");
    fflush(stdin); // limpa o buffer do teclado
    getch();
    return 0;
}

// Resumo das regras para funcao que retorna ponteiro:
// 1. Pode retornar: ponteiro de malloc/calloc (heap) ou de variavel static.
// 2. NAO pode retornar: endereco de variavel local comum (vira dangling).
// 3. Se a funcao alocou com malloc, quem chamou deve dar free depois.
// 4. Sempre teste if (p != NULL) ao receber o ponteiro.
```


#############################################################
# PARTE 3 - STDLIB, ALOCACAO AVANCADA E PREPROCESSADOR
#############################################################

## QSORT - ordenacao da biblioteca stdlib

Palavras-chave: `qsort`, `stdlib.h`, `ordenacao`, `comparador`, `ponteiro para funcao`, `O(n log n)`.

O `qsort` e a funcao de ordenacao padrao da biblioteca `stdlib.h`. Ele ordena um vetor "in-place" (direto na memoria, sem criar copia) usando o algoritmo quicksort, que e rapido - complexidade O(n log n) em media.

Assinatura oficial:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
void qsort(void *base, size_t n, size_t tam, int (*compar)(const void*, const void*));
```

- `base` - endereco do primeiro elemento do vetor (so passar o nome do vetor).
- `n` - quantidade de elementos.
- `tam` - tamanho em bytes de cada elemento (use `sizeof`).
- `compar` - ponteiro para a funcao comparadora.

A funcao comparadora e o coracao do qsort. Ela recebe dois ponteiros genericos `const void*` (um para cada elemento a comparar) e retorna:

- valor **menor que 0** se o primeiro deve vir antes do segundo.
- **0** se sao iguais.
- valor **maior que 0** se o primeiro deve vir depois do segundo.

Dentro do comparador, precisamos fazer **cast** do `const void*` para o tipo real (geralmente um ponteiro para o tipo do elemento). Para inverter a ordem (decrescente), basta trocar a ordem da subtracao.

Exemplo 1 - ordenar vetor de int:

```c
#include <stdio.h>
#include <stdlib.h>  // qsort

// Comparador para int (crescente).
// Recebe ponteiros genericos, faz cast para int* e compara.
int comparar_int(const void *a, const void *b) {
    int x = *(const int *)a;  // converte void* para int* e derreferencia
    int y = *(const int *)b;
    // return x - y;  // ATENCAO: pode dar overflow com valores extremos
    if (x < y) return -1;  // x antes de y
    if (x > y) return 1;   // x depois de y
    return 0;              // iguais
}

int main(void) {
    int v[] = {5, 2, 9, 1, 7, 3, 8, 4, 6, 0};
    int n = 10;

    // Ordena o vetor: base=v, n elementos, tamanho de cada int, comparador
    qsort(v, n, sizeof(int), comparar_int);

    printf("Vetor ordenado: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");

    return 0;
}
```

Exemplo 2 - ordenar vetor de struct por campo (nome com strcmp e por numero):

```c
#include <stdio.h>
#include <stdlib.h>   // qsort
#include <string.h>   // strcmp

// Struct com alias (typedef) - estilo do professor
typedef struct {
    char nome[50];
    int idade;
    float nota;
} Aluno;

// Comparador por NOME (usa strcmp).
// strcmp retorna <0, 0, >0 - ja no formato que o qsort espera.
int comparar_por_nome(const void *a, const void *b) {
    const Aluno *pa = (const Aluno *)a;  // cast void* -> Aluno*
    const Aluno *pb = (const Aluno *)b;
    return strcmp(pa->nome, pb->nome);   // crescente por nome
}

// Comparador por NOTA (crescente).
int comparar_por_nota(const void *a, const void *b) {
    const Aluno *pa = (const Aluno *)a;
    const Aluno *pb = (const Aluno *)b;
    if (pa->nota < pb->nota) return -1;
    if (pa->nota > pb->nota) return 1;
    return 0;
    // Para DECRESCENTE (maior nota primeiro), trocar -1 e 1 de lugar,
    // ou simplesmente inverter a subtracao: return (pb->nota > pa->nota) - ...
}

// Comparador por IDADE DECRESCENTE (exemplo de ordem inversa)
int comparar_por_idade_dec(const void *a, const void *b) {
    const Aluno *pa = (const Aluno *)a;
    const Aluno *pb = (const Aluno *)b;
    if (pa->idade > pb->idade) return -1;  // maior idade primeiro
    if (pa->idade < pb->idade) return 1;
    return 0;
}

int main(void) {
    Aluno turma[] = {
        {"Carlos", 20, 7.5},
        {"Ana",    22, 9.1},
        {"Bia",    19, 8.0},
        {"Diego",  21, 6.3}
    };
    int n = 4;

    // Ordena por nome (alfabetico)
    qsort(turma, n, sizeof(Aluno), comparar_por_nome);
    printf("Ordenado por nome:\n");
    for (int i = 0; i < n; i++) {
        printf("  %-10s %d %.1f\n", turma[i].nome, turma[i].idade, turma[i].nota);
    }

    // Ordena por nota (crescente)
    qsort(turma, n, sizeof(Aluno), comparar_por_nota);
    printf("\nOrdenado por nota (crescente):\n");
    for (int i = 0; i < n; i++) {
        printf("  %-10s %d %.1f\n", turma[i].nome, turma[i].idade, turma[i].nota);
    }

    return 0;
}
```

Vantagem do `qsort`: e rapido (O(n log n)), ja esta pronto e testado, e funciona com qualquer tipo de dado - so precisa escrever o comparador certo.

---

## BSEARCH - busca binaria da biblioteca

Palavras-chave: `bsearch`, `busca binaria`, `stdlib.h`, `vetor ordenado`, `comparador`.

A funcao `bsearch` (binary search) faz busca binaria em um vetor **ordenado**. Se o vetor nao estiver ordenado, o resultado e indefinido (lixo). A assinatura e parecida com a do `qsort`, mas tem um parametro a mais - a chave procurada:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
void *bsearch(const void *chave, const void *base, size_t n, size_t tam,
              int (*compar)(const void*, const void*));
```

- `chave` - ponteiro para o valor procurado.
- `base`, `n`, `tam`, `compar` - iguais ao qsort.
- Retorna: ponteiro para o elemento encontrado, ou `NULL` se nao achou.

Importante: o comparador deve ser o **mesmo** usado para ordenar o vetor. Se ordenou por nome, busque com o comparador de nome.

Exemplo - buscar inteiro em vetor ordenado:

```c
#include <stdio.h>
#include <stdlib.h>  // qsort, bsearch

// Mesmo comparador do qsort (crescente)
int comparar_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int main(void) {
    int v[] = {5, 2, 9, 1, 7, 3, 8, 4, 6, 0};
    int n = 10;

    // PASSO 1: ordenar o vetor (bsearch exige vetor ordenado!)
    qsort(v, n, sizeof(int), comparar_int);

    printf("Vetor ordenado: ");
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");

    // PASSO 2: buscar varios valores
    int procurados[] = {7, 42, 0, 9};
    for (int i = 0; i < 4; i++) {
        int chave = procurados[i];
        // bsearch retorna ponteiro para o elemento, ou NULL
        int *achou = (int *) bsearch(&chave, v, n, sizeof(int), comparar_int);

        if (achou != NULL) {
            // achou aponta para o elemento dentro do vetor v
            // indice = endereco encontrado - endereco inicial do vetor
            int indice = (int)(achou - v);
            printf("Valor %d encontrado no indice %d\n", chave, indice);
        } else {
            printf("Valor %d NAO encontrado\n", chave);
        }
    }

    return 0;
}
```

Dica: para descobrir o indice do elemento encontrado, subtraia o endereco base: `indice = (int)(achou - v);`. Isso funciona porque aritmetica de ponteiros conta em elementos, nao em bytes.

---

## RAND E SRAND - numeros aleatorios

Palavras-chave: `rand`, `srand`, `time`, `time.h`, `aleatorio`, `RAND_MAX`, `dado`, `faixa`.

A funcao `rand()` (declarada em `stdlib.h`) gera numeros pseudo-aleatorios entre 0 e `RAND_MAX` (uma constante grande, geralmente 32767 ou maior). Sao "pseudo" porque seguem uma formula matematica - se nao mudar a semente, a sequencia e sempre a mesma a cada execucao.

Para ter sequencias diferentes a cada execucao, usamos `srand(semente)` para inicializar o gerador. A pratica padrao e usar `srand(time(NULL))` - `time(NULL)` (de `time.h`) retorna os segundos desde 01/01/1970, entao cada execucao em momento diferente gera uma semente diferente.

Para gerar um numero em uma faixa `[min, max]`, use a formula:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
numero = rand() % (max - min + 1) + min;
```

O `+1` e necessario porque `%` gera de 0 ate (divisor-1). Para um dado de 6 faces: `rand() % 6 + 1` gera 1 a 6.

Exemplo - jogar dado e gerar vetor aleatorio:

```c
#include <stdio.h>
#include <stdlib.h>  // rand, srand
#include <time.h>    // time

int main(void) {
    // Inicializa o gerador com a hora atual - so chamar UMA VEZ no programa.
    // Se chamar srand(time(NULL)) varias vezes rapido, a semente repete.
    srand(time(NULL));

    // Jogar um dado 10 vezes (face 1 a 6)
    printf("Lancando o dado 10 vezes:\n");
    for (int i = 0; i < 10; i++) {
        int dado = rand() % 6 + 1;  // formula: rand() % (max-min+1) + min
        printf("  Lancamento %d: %d\n", i + 1, dado);
    }

    // Gerar vetor de 20 numeros aleatorios entre 0 e 99
    int v[20];
    for (int i = 0; i < 20; i++) {
        v[i] = rand() % 100;  // 0 a 99
    }

    printf("\nVetor aleatorio (0-99):\n");
    for (int i = 0; i < 20; i++) {
        printf("%3d ", v[i]);
        if ((i + 1) % 10 == 0) printf("\n");  // 10 por linha
    }

    // Mostrar RAND_MAX
    printf("\nRAND_MAX = %d\n", RAND_MAX);

    return 0;
}
```

Atencao: chame `srand(time(NULL))` **uma unica vez** no inicio do programa. Se colocar dentro de um loop, a semente sera redefinida com o mesmo valor de segundos e voce tera numeros repetidos.

---

## TIME.H - funcoes de tempo

Palavras-chave: `time.h`, `time`, `clock`, `difftime`, `CLOCKS_PER_SEC`, `medir tempo`.

A biblioteca `time.h` oferece funcoes para trabalhar com tempo. As mais usadas em provas:

- `time(NULL)` - retorna o tempo em segundos desde 01/01/1970 (epoch). Usado para semear `srand`.
- `clock()` - retorna o tempo de processamento (CPU) usado pelo programa, em "ticks". Para converter para segundos, divida por `CLOCKS_PER_SEC`.
- `difftime(a, b)` - retorna a diferenca `a - b` em segundos (como `double`).

Exemplo - medir tempo de execucao de um trecho:

```c
#include <stdio.h>
#include <time.h>    // clock, CLOCKS_PER_SEC, time, difftime

int main(void) {
    // ---- Medindo tempo de CPU com clock() ----
    clock_t inicio = clock();  // marca o inicio (ticks de CPU)

    // Trecho que queremos medir: soma de 1 a 100 milhoes
    double soma = 0;
    for (long i = 1; i <= 100000000L; i++) {
        soma += i;
    }

    clock_t fim = clock();  // marca o fim
    // Tempo em segundos = (fim - inicio) / CLOCKS_PER_SEC
    double tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Soma = %.0f\n", soma);
    printf("Tempo de CPU: %.4f segundos\n", tempo_cpu);

    // ---- Medindo tempo "de parede" com time() e difftime() ----
    time_t t1 = time(NULL);  // segundos desde 1970

    // Simula algum trabalho
    for (volatile long i = 0; i < 500000000L; i++) { }

    time_t t2 = time(NULL);
    double diff = difftime(t2, t1);  // diferenca em segundos
    printf("Tempo de parede: %.0f segundos\n", diff);

    // ---- Mostrar o valor de time(NULL) ----
    printf("Segundos desde 1970: %ld\n", (long)time(NULL));

    return 0;
}
```

Diferenca importante: `clock()` mede tempo de CPU (quanto o processador trabalhou), enquanto `time()` mede tempo "de parede" (o tempo real que passou, incluindo esperas). Para medir algoritmos, geralmente usamos `clock()`.

---

## MATRIZ DINAMICA - alocacao

Palavras-chave: `matriz dinamica`, `malloc`, `ponteiro para ponteiro`, `int **`, `free`, `alocacao dinamica`.

Em C, podemos criar matrizes com tamanho definido em tempo de execucao usando alocacao dinamica. A tecnica e alocar um **vetor de ponteiros** (uma linha de ponteiros), e depois alocar cada linha separadamente. O resultado e acessado com a sintaxe familiar `m[i][j]`.

Para uma matriz `linhas x colunas` de int:

1. Alocar vetor de `linhas` ponteiros para int: `int **m = malloc(linhas * sizeof(int*));`
2. Para cada linha `i`, alocar `colunas` inteiros: `m[i] = malloc(colunas * sizeof(int));`
3. Acessar normalmente: `m[i][j] = valor;`
4. Para liberar, fazer o **inverso**: primeiro liberar cada linha, depois liberar o vetor de ponteiros. Se fizer ao contrario (liberar `m` antes das linhas), perde-se o acesso as linhas e vira vazamento de memoria.

Exemplo completo:

```c
#include <stdio.h>
#include <stdlib.h>  // malloc, free

int main(void) {
    int linhas = 3, colunas = 4;

    // PASSO 1: alocar vetor de ponteiros (um para cada linha)
    int **m = malloc(linhas * sizeof(int *));
    if (m == NULL) {  // sempre conferir se malloc deu certo
        printf("Erro de alocacao\n");
        return 1;
    }

    // PASSO 2: alocar cada linha (vetor de int)
    for (int i = 0; i < linhas; i++) {
        m[i] = malloc(colunas * sizeof(int));
        if (m[i] == NULL) {
            printf("Erro de alocacao na linha %d\n", i);
            return 1;
        }
    }

    // PASSO 3: usar a matriz normalmente com m[i][j]
    // Preencher com i*10 + j so para ter dados
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            m[i][j] = i * 10 + j;
        }
    }

    // Imprimir
    printf("Matriz %dx%d:\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%3d ", m[i][j]);
        }
        printf("\n");
    }

    // PASSO 4: liberar - PRIMEIRO as linhas, DEPOIS o vetor de ponteiros
    for (int i = 0; i < linhas; i++) {
        free(m[i]);   // libera cada linha
    }
    free(m);          // libera o vetor de ponteiros
    m = NULL;         // boa pratica: anular o ponteiro

    return 0;
}
```

Vantagem: o numero de linhas e colunas pode vir de uma variavel ou do usuario. Cada linha pode ate ter tamanhos diferentes (matriz "jagged"), se quiser.

---

## STRTOL E STRTOD - conversao robusta

Palavras-chave: `strtol`, `strtod`, `stdlib.h`, `conversao`, `base`, `deteccao de erro`, `atoi`.

As funcoes `atoi` e `atof` convertem string para numero, mas **nao detectam erro**: se a string for "abc", `atoi` retorna 0 - e voce nao sabe se era "0" ou se era lixo. As funcoes `strtol` (string to long) e `strtod` (string to double) resolvem isso: elas recebem um ponteiro `fim` que indica **onde a conversao parou**. Se `fim == string`, nada foi convertido (erro).

Assinaturas:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
long strtol(const char *s, char **fim, int base);
double strtod(const char *s, char **fim);
```

- `s` - a string de entrada.
- `fim` - ponteiro para ponteiro; apos a chamada, aponta para o primeiro caractere nao convertido. Passe `&fim` de uma variavel `char *fim`.
- `base` - base numerica: 10 (decimal), 16 (hexa), 8 (octal), 0 (auto-detectar prefixo).

Exemplo com base 10 e base 16:

```c
#include <stdio.h>
#include <stdlib.h>  // strtol, strtod
#include <string.h>

int main(void) {
    char s1[] = "1234abc";
    char s2[] = "3.14xyz";
    char s3[] = "nao_e_numero";
    char s4[] = "FF";        // hexa
    char *fim;               // vai apontar para onde parou

    // ---- strtol base 10 ----
    long v1 = strtol(s1, &fim, 10);
    printf("String: \"%s\"\n", s1);
    printf("  Valor convertido: %ld\n", v1);
    printf("  Parou em: \"%s\"\n", fim);  // mostra o que sobrou
    if (fim == s1) {
        printf("  -> Nao converteu nada (erro)\n");
    } else {
        printf("  -> Converteu parcialmente\n");
    }

    // ---- strtod (double) ----
    double v2 = strtod(s2, &fim);
    printf("\nString: \"%s\"\n", s2);
    printf("  Valor convertido: %.2f\n", v2);
    printf("  Parou em: \"%s\"\n", fim);

    // ---- Caso de erro total ----
    long v3 = strtol(s3, &fim, 10);
    printf("\nString: \"%s\"\n", s3);
    printf("  Valor: %ld, fim == s3? %s\n", v3, (fim == s3) ? "SIM (erro)" : "nao");
    // fim == s3 significa que nenhum caractere foi convertido

    // ---- strtol base 16 (hexa) ----
    long v4 = strtol(s4, &fim, 16);
    printf("\nString hexa: \"%s\"\n", s4);
    printf("  Valor decimal: %ld\n", v4);  // FF hexa = 255 decimal
    printf("  Parou em: \"%s\"\n", fim);

    // ---- Base 0: auto-detecta pelo prefixo (0x = hexa, 0 = octal) ----
    long v5 = strtol("0x1A", &fim, 0);  // 0x -> base 16
    printf("\n0x1A (auto) = %ld\n", v5);  // 26
    long v6 = strtol("010", &fim, 0);    // 0 inicial -> base 8
    printf("010 (auto) = %ld\n", v6);    // 8

    return 0;
}
```

Vantagem sobre `atoi`: com `strtol`, se `fim == s` sabemos que a string nao era um numero - da para tratar o erro. Com `atoi`, "abc" e "0" dao o mesmo resultado (0) e nao tem como distinguir.

---

## SNPRINTF - sprintf seguro

Palavras-chave: `snprintf`, `sprintf`, `stdio.h`, `buffer`, `seguranca`, `limite`.

A funcao `sprintf(dest, formato, ...)` monta uma string formatada em um buffer, mas ela **nao conhece o tamanho do buffer** - se o resultado for maior que o espaco, ocorre estouro de buffer (buffer overflow), um dos bugs mais perigosos em C.

A versao segura e `snprintf`, que recebe o tamanho maximo do buffer e nunca escreve alem dele:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
int snprintf(char *dest, size_t tamanho, const char *formato, ...);
```

- `dest` - buffer de destino.
- `tamanho` - tamanho maximo do buffer (inclui o `\0` final). O `snprintf` escreve no maximo `tamanho-1` caracteres e poe o `\0`.
- Retorna o numero de caracteres que **teriam sido** escritos se o buffer fosse grande o suficiente (util para saber se truncou).

Exemplo - montar string com limite:

```c
#include <stdio.h>

int main(void) {
    char nome[] = "Franco";
    int idade = 22;
    float nota = 8.75;
    char buffer[50];  // buffer de tamanho fixo

    // snprintf: seguro, nunca estoura o buffer
    int escritos = snprintf(buffer, sizeof(buffer),
                            "Aluno %s, idade %d, nota %.2f",
                            nome, idade, nota);
    printf("String montada: %s\n", buffer);
    printf("Caracteres escritos (sem \\0): %d\n", escritos);
    printf("Tamanho do buffer: %zu\n", sizeof(buffer));

    // Verificar se truncou: se escritos >= tamanho, cortou
    if (escritos >= (int)sizeof(buffer)) {
        printf("AVISO: a string foi truncada!\n");
    }

    // ---- Exemplo de truncamento forçado ----
    char pequeno[10];
    int r = snprintf(pequeno, sizeof(pequeno), "Texto muito longo que nao cabe");
    printf("\nBuffer pequeno: \"%s\"\n", pequeno);  // so cabe 9 chars + \0
    printf("Teriam sido escritos: %d\n", r);        // 34 (truncou)

    // Comparacao: sprintf NAO tem limite - perigoso!
    // sprintf(buffer, "....");  // NAO use em codigo novo

    return 0;
}
```

Regra: em codigo novo, **sempre** use `snprintf` no lugar de `sprintf`. O custo de passar o tamanho e zero e o beneficio (evitar bugs de seguranca) e enorme.

---

## ISPUNCT E ISXDIGIT - ctype

Palavras-chave: `ispunct`, `isxdigit`, `ctype.h`, `pontuacao`, `hexadecimal`.

A biblioteca `ctype.h` tem varias funcoes de classificacao de caracteres. Duas uteis:

- `ispunct(c)` - retorna nao-zero se `c` e um caractere de **pontuacao**: `! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ \ ] ^ _ ` { | } ~`. Ou seja, imprimivel mas nao letra, digito nem espaco.
- `isxdigit(c)` - retorna nao-zero se `c` e um **digito hexadecimal**: `0-9`, `a-f`, `A-F`.

Essas funcoes recebem um `int` (o caractere) e retornam `int` (zero ou nao-zero). Devem ser usadas com `unsigned char` ou `EOF` - se passar um `char` com sinal negativo, pode dar errado em algumas implementacoes.

Exemplos:

```c
#include <stdio.h>
#include <ctype.h>   // ispunct, isxdigit

int main(void) {
    // ---- ispunct: pontuacao ----
    char teste1[] = "A1!@#. ,?";
    printf("ispunct em cada caractere de \"%s\":\n", teste1);
    for (int i = 0; teste1[i] != '\0'; i++) {
        // cast para unsigned char para evitar problema com char com sinal
        if (ispunct((unsigned char)teste1[i])) {
            printf("  '%c' E pontuacao\n", teste1[i]);
        } else {
            printf("  '%c' NAO e pontuacao\n", teste1[i]);
        }
    }

    // ---- isxdigit: digito hexadecimal ----
    char teste2[] = "0123456789ABCDEFabcdefGxyz";
    printf("\nisxdigit em cada caractere de \"%s\":\n", teste2);
    for (int i = 0; teste2[i] != '\0'; i++) {
        if (isxdigit((unsigned char)teste2[i])) {
            printf("  '%c' E hexa\n", teste2[i]);
        } else {
            printf("  '%c' NAO e hexa\n", teste2[i]);
        }
    }

    // ---- Uso pratico: validar se uma string so tem hexa ----
    char hex[] = "1A2B3F";
    int valida = 1;
    for (int i = 0; hex[i] != '\0'; i++) {
        if (!isxdigit((unsigned char)hex[i])) {
            valida = 0;
            break;
        }
    }
    printf("\n\"%s\" %s um numero hexa valido\n",
           hex, valida ? "e" : "NAO e");

    return 0;
}
```

Outras funcoes do ctype para lembrar: `isdigit` (0-9), `isalpha` (letra), `isalnum` (letra ou digito), `isspace` (espaco/tab/newline), `isupper`/`islower` (maiuscula/minuscula), `toupper`/`tolower` (conversao).

---

## PONTEIRO PARA FUNCAO

Palavras-chave: `ponteiro para funcao`, `int (*op)(int,int)`, `callback`, `vetor de funcoes`, `calculadora`, `qsort`.

Um ponteiro para funcao guarda o endereco de uma funcao, permitindo chama-la indiretamente. E a base dos "callbacks" (como o comparador do `qsort`) e de tabelas de funcoes (como uma calculadora que escolhe a operacao em tempo de execucao).

Declaracao - a sintaxe e meio estranha porque os parenteses sao obrigatorios:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
int (*operacao)(int, int);  // ponteiro para funcao que recebe 2 int e retorna int
```

Sem os parenteses em volta de `*operacao`, seria uma funcao que retorna um ponteiro - coisa totalmente diferente.

- Atribuir: `operacao = soma;` (o nome da funcao sozinho vira endereco; `&soma` tambem funciona).
- Chamar: `operacao(2, 3);` ou `(*operacao)(2, 3);` - as duas formas sao validas.

Exemplo completo - calculadora com vetor de funcoes:

```c
#include <stdio.h>

// Funcoes com a MESMA assinatura: int f(int, int)
int soma(int a, int b)    { return a + b; }
int subtrai(int a, int b) { return a - b; }
int multiplica(int a, int b) { return a * b; }
int divide(int a, int b) {
    if (b == 0) {
        printf("Erro: divisao por zero\n");
        return 0;
    }
    return a / b;
}

int main(void) {
    // Declarar ponteiro para funcao com a assinatura int(int, int)
    int (*operacao)(int, int);

    int x = 10, y = 3;

    // Atribuir e chamar - soma
    operacao = soma;            // ou &soma
    printf("%d + %d = %d\n", x, y, operacao(x, y));

    // Subtracao
    operacao = subtrai;
    printf("%d - %d = %d\n", x, y, operacao(x, y));

    // Multiplicacao
    operacao = multiplica;
    printf("%d * %d = %d\n", x, y, operacao(x, y));

    // Divisao
    operacao = divide;
    printf("%d / %d = %d\n", x, y, operacao(x, y));

    // ---- Vetor de ponteiros para funcao (tabela de despacho) ----
    // Cada elemento do vetor e um ponteiro para funcao
    int (*tabela[4])(int, int) = { soma, subtrai, multiplica, divide };
    char *nomes[] = { "soma", "subtracao", "multiplicacao", "divisao" };

    printf("\nUsando vetor de funcoes:\n");
    for (int i = 0; i < 4; i++) {
        printf("  %s: %d\n", nomes[i], tabela[i](x, y));
    }

    // ---- Uso real: o comparador do qsort e um callback ----
    // qsort(v, n, sizeof(int), comparar_int);
    // comparar_int e passado como ponteiro para funcao.

    return 0;
}
```

O comparador do `qsort`/`bsearch` e o exemplo classico de callback: voce passa o endereco de uma funcao sua, e a biblioteca a chama quando precisa comparar dois elementos.

---

## ARRAY DE PONTEIROS / VETOR DE STRINGS

Palavras-chave: `array de ponteiros`, `char *nomes[]`, `vetor de strings`, `char nomes[3][20]`, `matriz de char`.

Em C, um "vetor de strings" pode ser feito de duas formas principais:

**Forma 1 - Array de ponteiros** (strings de tamanhos variaveis):

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
char *nomes[] = {"Ana", "Bia", "Carlos"};
```

Aqui `nomes` e um vetor de 3 ponteiros para `char`. Cada ponteiro aponta para uma string literal (que fica em memoria so-leitura). As strings podem ter tamanhos diferentes ("Ana" = 4 bytes, "Carlos" = 7 bytes). Vantagem: economiza memoria quando os tamanhos variam muito. Desvantagem: as strings literais **nao podem ser modificadas** (e comportamento indefinido escrever nelas).

**Forma 2 - Matriz de char** (tamanho fixo por linha):

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
char nomes[3][20];
```

Aqui cada linha tem exatamente 20 caracteres. Ocupa `3 * 20 = 60` bytes, mesmo que as strings sejam curtas. Vantagem: o conteudo **pode ser modificado** (e memoria gravavel).

Exemplo comparando as duas:

```c
#include <stdio.h>
#include <string.h>  // strcpy, strcmp

int main(void) {
    // ---- Forma 1: array de ponteiros (strings literais) ----
    char *nomes1[] = {"Ana", "Bia", "Carlos", "Daniel"};
    int n1 = 4;

    printf("Array de ponteiros:\n");
    for (int i = 0; i < n1; i++) {
        printf("  nomes1[%d] = \"%s\" (tamanho %zu)\n",
               i, nomes1[i], strlen(nomes1[i]));
    }
    // nomes1[0][0] = 'X';  // ERRADO! string literal e so-leitura

    // ---- Forma 2: matriz de char (modificavel) ----
    char nomes2[3][20];  // 3 linhas, 20 colunas cada

    // Copiar strings para a matriz (precisa usar strcpy, nao =)
    strcpy(nomes2[0], "Ana");
    strcpy(nomes2[1], "Beatriz");
    strcpy(nomes2[2], "Carlos");

    printf("\nMatriz de char:\n");
    for (int i = 0; i < 3; i++) {
        printf("  nomes2[%d] = \"%s\"\n", i, nomes2[i]);
    }

    // Aqui PODE modificar (e memoria gravavel)
    nomes2[0][0] = 'X';  // muda "Ana" para "Xna"
    printf("Apos modificar: nomes2[0] = \"%s\"\n", nomes2[0]);

    // ---- Ordenar o array de ponteiros (so troca ponteiros, nao copia strings) ----
    // Como os ponteiros sao variaveis, podemos reordenar sem mover as strings
    for (int i = 0; i < n1 - 1; i++) {
        for (int j = 0; j < n1 - 1 - i; j++) {
            if (strcmp(nomes1[j], nomes1[j + 1]) > 0) {
                // troca so os ponteiros (rapido, nao copia caracteres)
                char *tmp = nomes1[j];
                nomes1[j] = nomes1[j + 1];
                nomes1[j + 1] = tmp;
            }
        }
    }

    printf("\nOrdenado (array de ponteiros):\n");
    for (int i = 0; i < n1; i++) {
        printf("  %s\n", nomes1[i]);
    }

    return 0;
}
```

Resumo da diferenca:
- **Array de ponteiros** (`char *v[]`): tamanhos variaveis, economiza memoria, mas strings literais sao so-leitura. Para strings modificaveis, cada ponteiro pode apontar para um `malloc`.
- **Matriz de char** (`char v[N][TAM]`): tamanho fixo por linha, gasta mais memoria, mas e gravavel.

---

## FERROR - erro em arquivo

Palavras-chave: `ferror`, `feof`, `arquivo`, `erro de leitura`, `erro de escrita`, `stdio.h`.

Ao ler/escrever arquivos, duas funcoes testam condicoes especiais:

- `feof(arq)` - retorna nao-zero se chegou ao **fim do arquivo** (end-of-file). Usado para saber quando parar de ler.
- `ferror(arq)` - retorna nao-zero se ocorreu um **erro** de leitura ou escrita (ex.: disco cheio, arquivo corrompido, permissao negada).

A diferenca e importante: `feof` sinaliza "acabou normalmente", `ferror` sinaliza "deu erro". Um loop de leitura pode parar por fim de arquivo OU por erro - e preciso distinguir os dois casos.

Exemplo:

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // ---- Tentar abrir um arquivo que existe ----
    FILE *arq = fopen("dados.txt", "r");
    if (arq == NULL) {
        printf("Nao consegui abrir o arquivo (fopen falhou)\n");
        return 1;
    }

    // Ler caractere por caractere ate o fim
    int c;
    printf("Conteudo lido:\n");
    while ((c = fgetc(arq)) != EOF) {
        putchar(c);
    }

    // Agora parou - foi fim de arquivo ou erro?
    if (ferror(arq)) {
        printf("\n>> ERRO durante a leitura!\n");
    } else if (feof(arq)) {
        printf("\n>> Fim de arquivo alcancado normalmente.\n");
    }

    fclose(arq);

    // ---- Exemplo de erro real: abrir so para leitura e tentar escrever ----
    FILE *arq2 = fopen("saida.txt", "w");
    if (arq2 == NULL) {
        printf("Erro ao criar saida.txt\n");
        return 1;
    }
    fprintf(arq2, "Linha de teste\n");
    fclose(arq2);

    // Reabrir so para leitura e tentar escrever (vai dar erro)
    arq2 = fopen("saida.txt", "r");
    fprintf(arq2, "tentando escrever");  // modo "r" nao permite escrita
    if (ferror(arq2)) {
        printf(">> Erro detectado: tentou escrever em arquivo aberto para leitura\n");
        clearerr(arq2);  // limpa a flag de erro para continuar usando o arquivo
    }
    fclose(arq2);

    return 0;
}
```

Dica: `clearerr(arq)` limpa as flags de erro e fim-de-arquivo, permitindo reusar o arquivo apos um erro. Use `ferror` apos loops de leitura para confirmar que parou por fim normal e nao por erro.

---

## TMPFILE - arquivo temporario

Palavras-chave: `tmpfile`, `arquivo temporario`, `stdio.h`, `dados intermediarios`.

A funcao `tmpfile()` cria um arquivo temporario que **se apaga automaticamente** quando e fechado (`fclose`) ou quando o programa termina. E util para guardar dados intermediarios sem deixar lixo no disco.

Assinatura:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
FILE *tmpfile(void);
```

Retorna um `FILE *` ja aberto para leitura e escrita (`"w+b"`), ou `NULL` se falhou. O arquivo existe apenas durante a execucao - nao tem nome visivel (ou tem um nome unico gerado internamente).

Exemplo:

```c
#include <stdio.h>

int main(void) {
    // Cria arquivo temporario - some ao fechar
    FILE *tmp = tmpfile();
    if (tmp == NULL) {
        printf("Erro ao criar arquivo temporario\n");
        return 1;
    }

    // Escrever alguns dados
    int valores[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        fprintf(tmp, "%d\n", valores[i]);
    }

    // Voltar ao inicio para ler (precisa rebobinar)
    rewind(tmp);  // ou fseek(tmp, 0, SEEK_SET);

    // Ler de volta
    int x;
    printf("Lendo do arquivo temporario:\n");
    while (fscanf(tmp, "%d", &x) == 1) {
        printf("  %d\n", x);
    }

    // Fechar - o arquivo e apagado automaticamente
    fclose(tmp);
    printf("Arquivo temporario apagado ao fechar.\n");

    // Alternativa: tmpnam() gera um nome de arquivo temporario,
    // mas ai voce precisa abrir/fechar/remover manualmente.
    // tmpfile() e mais seguro porque se apaga sozinho.

    return 0;
}
```

Cuidado: depois de escrever no arquivo temporario, e preciso **rebobinar** (`rewind` ou `fseek` para o inicio) antes de ler, porque o indicador de posicao esta no final.

---

## FGETPOS E FSETPOS - posicao em arquivo

Palavras-chave: `fgetpos`, `fsetpos`, `fpos_t`, `fseek`, `arquivo grande`, `stdio.h`.

As funcoes `fseek`/`ftell` usam `long int` para a posicao, o que pode nao ser suficiente para arquivos muito grandes (acima de 2 GB em sistemas de 32 bits). As funcoes `fgetpos` e `fsetpos` usam um tipo opaco `fpos_t` que pode ser maior, sendo a alternativa portavel para arquivos grandes.

Assinaturas:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
int fgetpos(FILE *stream, fpos_t *pos);  // salva a posicao atual em pos
int fsetpos(FILE *stream, const fpos_t *pos);  // restaura a posicao
```

Ambas retornam 0 em sucesso, nao-zero em erro. O tipo `fpos_t` e definido em `stdio.h` - nao importa o tamanho, voce so passa o endereco de uma variavel `fpos_t`.

Exemplo:

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    // Criar um arquivo de teste
    FILE *arq = fopen("teste_pos.txt", "w+");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    fprintf(arq, "Linha 1\nLinha 2\nLinha 3\nLinha 4\n");

    // Salvar a posicao atual (no fim, apos escrever)
    fpos_t pos1;
    fgetpos(arq, &pos1);
    printf("Posicao salva apos escrita.\n");

    // Voltar ao inicio e ler
    rewind(arq);
    char linha[100];
    printf("Primeira leitura:\n");
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        printf("  %s", linha);
    }

    // Restaurar a posicao salva (voltaria ao fim)
    fsetpos(arq, &pos1);
    printf("\nPosicao restaurada para a salva.\n");

    // Voltar ao inicio de novo para reler
    rewind(arq);
    printf("Segunda leitura (so as 2 primeiras linhas):\n");
    for (int i = 0; i < 2; i++) {
        if (fgets(linha, sizeof(linha), arq) != NULL) {
            printf("  %s", linha);
        }
    }

    // Salvar outra posicao e restaurar
    fpos_t pos2;
    fgetpos(arq, &pos2);  // salva posicao atual (apos 2 linhas)
    printf("\nPosicao salva apos 2 linhas.\n");

    // Ler mais uma linha
    if (fgets(linha, sizeof(linha), arq) != NULL) {
        printf("Terceira linha: %s", linha);
    }

    // Voltar para pos2 (antes da terceira linha) e reler
    fsetpos(arq, &pos2);
    if (fgets(linha, sizeof(linha), arq) != NULL) {
        printf("Relendo terceira linha: %s", linha);
    }

    fclose(arq);
    return 0;
}
```

Use `fgetpos`/`fsetpos` quando precisar "marcar" uma posicao no arquivo e voltar a ela depois, especialmente em arquivos grandes onde `ftell`/`fseek` podem nao ser suficientes.

---

## LIMITS.H E FLOAT.H - limites numericos

Palavras-chave: `limits.h`, `float.h`, `INT_MAX`, `INT_MIN`, `CHAR_MAX`, `LONG_MAX`, `FLT_MAX`, `DBL_MAX`, `FLT_EPSILON`.

Esses headers definem constantes com os limites dos tipos numericos, uteis para evitar overflow e inicializar variaveis com valores extremos.

`limits.h` - limites dos tipos inteiros:
- `INT_MIN`, `INT_MAX` - menor e maior `int` (geralmente -2147483648 e 2147483647).
- `CHAR_MIN`, `CHAR_MAX` - limites do `char`.
- `SHRT_MIN`, `SHRT_MAX` - limites do `short`.
- `LONG_MIN`, `LONG_MAX` - limites do `long`.
- `LLONG_MIN`, `LLONG_MAX` - limites do `long long`.
- `UCHAR_MAX`, `UINT_MAX`, `ULONG_MAX` - limites dos tipos `unsigned`.

`float.h` - limites dos tipos de ponto flutuante:
- `FLT_MIN`, `FLT_MAX` - menor positivo normal e maior `float`.
- `DBL_MIN`, `DBL_MAX` - limites do `double`.
- `FLT_EPSILON` - menor valor tal que `1.0 + FLT_EPSILON != 1.0` (precisao da unidade).
- `DBL_EPSILON` - o mesmo para `double`.
- `FLT_DIG`, `DBL_DIG` - casas decimais de precisao garantida.

Exemplo de uso:

```c
#include <stdio.h>
#include <limits.h>   // INT_MAX, INT_MIN, CHAR_MAX, LONG_MAX
#include <float.h>    // FLT_MAX, DBL_MAX, FLT_EPSILON

int main(void) {
    // ---- Limites de inteiros (limits.h) ----
    printf("=== Limites de inteiros (limits.h) ===\n");
    printf("INT_MIN  = %d\n", INT_MIN);
    printf("INT_MAX  = %d\n", INT_MAX);
    printf("CHAR_MIN = %d\n", CHAR_MIN);
    printf("CHAR_MAX = %d\n", CHAR_MAX);
    printf("SHRT_MAX = %d\n", SHRT_MAX);
    printf("LONG_MAX = %ld\n", LONG_MAX);
    printf("LLONG_MAX = %lld\n", LLONG_MAX);
    printf("UCHAR_MAX = %u\n", UCHAR_MAX);
    printf("UINT_MAX  = %u\n", UINT_MAX);

    // ---- Limites de ponto flutuante (float.h) ----
    printf("\n=== Limites de float (float.h) ===\n");
    printf("FLT_MIN  = %e\n", FLT_MIN);
    printf("FLT_MAX  = %e\n", FLT_MAX);
    printf("DBL_MIN  = %e\n", DBL_MIN);
    printf("DBL_MAX  = %e\n", DBL_MAX);
    printf("FLT_EPSILON = %e\n", FLT_EPSILON);
    printf("DBL_EPSILON = %e\n", DBL_EPSILON);
    printf("FLT_DIG (casas garantidas) = %d\n", FLT_DIG);
    printf("DBL_DIG (casas garantidas) = %d\n", DBL_DIG);

    // ---- Uso pratico: inicializar com valor extremo ----
    int maior = INT_MIN;  // comeca com o menor possivel
    int v[] = {5, 9, 2, 7, 1};
    for (int i = 0; i < 5; i++) {
        if (v[i] > maior) {
            maior = v[i];
        }
    }
    printf("\nMaior do vetor: %d\n", maior);

    // ---- Uso pratico: detectar overflow antes de somar ----
    int a = INT_MAX;
    int b = 1;
    // Antes de somar, conferir se nao estoura
    if (a > INT_MAX - b) {
        printf("Overflow detectado: %d + %d estoura INT_MAX\n", a, b);
    } else {
        printf("Soma segura: %d\n", a + b);
    }

    // ---- FLT_EPSILON: menor diferenca perceptivel perto de 1.0 ----
    float eps = 1.0f;
    while (1.0f + eps / 2.0f != 1.0f) {
        eps = eps / 2.0f;
    }
    printf("\nEpsilon calculado  = %e\n", eps);
    printf("FLT_EPSILON (constante) = %e\n", FLT_EPSILON);

    return 0;
}
```

Usos comuns: inicializar variaveis de max/min (`int maior = INT_MIN;`), detectar overflow antes de operacoes, e comparar floats com tolerancia (`fabs(a - b) < FLT_EPSILON`).

---

## ATEXIT - funcao ao terminar

Palavras-chave: `atexit`, `stdlib.h`, `cleanup`, `finalizacao`, `registro de funcao`.

A funcao `atexit` registra uma funcao para ser executada **automaticamente** quando o programa termina normalmente (via `return` do `main` ou chamada de `exit`). E util para fazer "cleanup" - liberar recursos, salvar dados, fechar arquivos - sem depender do programador lembrar de chamar tudo no fim.

Assinatura:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
int atexit(void (*func)(void));
```

- `func` - ponteiro para uma funcao que **nao recebe e nao retorna nada** (`void f(void)`).
- Retorna 0 se registrou com sucesso, nao-zero se falhou.

Pode-se registrar varias funcoes - elas sao executadas na **ordem inversa** do registro (LIFO: a ultima registrada roda primeiro).

Exemplo:

```c
#include <stdio.h>
#include <stdlib.h>  // atexit, exit

// Funcoes de cleanup - devem ser void(void)
void salvar_log(void) {
    printf("[cleanup] Salvando log no arquivo...\n");
    // Aqui abriria um arquivo e gravaria o log
}

void fechar_conexao(void) {
    printf("[cleanup] Fechando conexao com banco...\n");
}

void liberar_memoria(void) {
    printf("[cleanup] Liberando memoria alocada...\n");
}

int main(void) {
    // Registrar funcoes de finalizacao (executam em ordem INVERSA)
    atexit(salvar_log);       // registrada 1a -> executa por ultimo
    atexit(fechar_conexao);   // registrada 2a -> executa no meio
    atexit(liberar_memoria);  // registrada 3a -> executa primeiro

    printf("Programa rodando...\n");
    printf("Fazendo trabalho principal...\n");

    // Mesmo terminando com return, as funcoes registradas rodam.
    // Tambem rodam se chamar exit(0).
    // NAO rodam se o programa for morto com abort() ou _exit().

    printf("Encerrando...\n");
    return 0;
    // Saida: liberar_memoria -> fechar_conexao -> salvar_log
}
```

Saida esperada (ordem inversa do registro):
```
Programa rodando...
Fazendo trabalho principal...
Encerrando...
[cleanup] Liberando memoria alocada...
[cleanup] Fechando conexao com banco...
[cleanup] Salvando log no arquivo...
```

Importante: as funcoes registradas com `atexit` so rodam em terminacao **normal** (`return` do `main` ou `exit`). Se o programa morrer com `abort()` (ex.: crash) ou `_exit()`, elas **nao** sao chamadas.

---

## PREPROCESSADOR AVANCADO

Palavras-chave: `#define`, `macro`, `#ifdef`, `#ifndef`, `#endif`, `#if`, `#else`, `#elif`, `include guard`, `#undef`, `parametros`.

O preprocessador age **antes** da compilacao, fazendo substituicoes de texto. Comandos comecam com `#`.

### Macro com parametros

Uma macro com parametros e como uma funcao inline feita por substituicao de texto:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
#define QUADRADO(x) ((x) * (x))
```

Cuidado com parenteses! Sem eles, o resultado pode ser errado porque a macro so substitui texto:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
#define ERRADO(x)  x * x
ERRADO(2 + 3)  // vira: 2 + 3 * 2 + 3 = 2 + 6 + 3 = 11 (nao 25!)
#define CERTO(x)  ((x) * (x))
CERTO(2 + 3)   // vira: ((2 + 3) * (2 + 3)) = 25
```

Regra: **sempre** coloque parenteses em volta de cada parametro e em volta da expressao inteira.

Atencao tambem com efeitos colaterais: `QUADRADO(i++)` expande para `((i++) * (i++))`, incrementando `i` duas vezes - bug classico.

### Compilacao condicional

Controla o que e compilado:

- `#ifdef NOME` - compila se `NOME` estiver definido.
- `#ifndef NOME` - compila se `NOME` **nao** estiver definido.
- `#if expressao` - compila se a expressao for verdadeira (usa constantes do preprocessador).
- `#else` - alternativa.
- `#elif` - "else if".
- `#endif` - fecha o bloco.

### Include guards

Evita que um header seja incluido duas vezes (o que causaria erro de redefinicao):

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
#ifndef MEU_HEADER_H
#define MEU_HEADER_H
// ... conteudo do header ...
#endif
```

Na primeira inclusao, `MEU_HEADER_H` nao esta definido, entao define e inclui o conteudo. Na segunda inclusao, ja esta definido, entao pula tudo.

### #undef

Remove a definicao de uma macro:

```c
// snippet de sintaxe (fragmento ilustrativo, nao e programa completo para rodar)
#define TAM 10
// ... usa TAM ...
#undef TAM          // agora TAM nao existe mais
#define TAM 20      // redef ine com outro valor
```

Exemplo completo:

```c
#include <stdio.h>
#include <stdlib.h>

// ---- Macros com parametros ----
#define QUADRADO(x)   ((x) * (x))           // CERTO: parenteses em tudo
#define ERRADO(x)     x * x                 // ERRADO: sem parenteses
#define MAX(a, b)     ((a) > (b) ? (a) : (b))  // macro com ternario
#define DEBUG 1                              // flag de debug

// ---- Include guard (em header real ficaria no .h) ----
#ifndef MEU_HEADER_H
#define MEU_HEADER_H
// conteudo que so entra uma vez
#define VERSAO "1.0"
#endif

int main(void) {
    // ---- Teste de macro com e sem parenteses ----
    printf("QUADRADO(2+3) = %d\n", QUADRADO(2 + 3));  // 25 (certo)
    printf("ERRADO(2+3)  = %d\n", ERRADO(2 + 3));     // 11 (bug!)

    // ---- Macro MAX ----
    int a = 10, b = 20;
    printf("MAX(%d, %d) = %d\n", a, b, MAX(a, b));     // 20

    // ---- Compilacao condicional com #if / #ifdef ----
    #if DEBUG
        printf("[DEBUG] Valores: a=%d, b=%d\n", a, b);
    #endif

    #ifdef VERSAO
        printf("Versao do programa: %s\n", VERSAO);
    #else
        printf("Versao nao definida\n");
    #endif

    // ---- #if com expressao numerica ----
    #define MODO 2
    #if MODO == 1
        printf("Modo 1 selecionado\n");
    #elif MODO == 2
        printf("Modo 2 selecionado\n");
    #else
        printf("Modo desconhecido\n");
    #endif

    // ---- #undef: remove macro ----
    #define LIMITE 100
    printf("LIMITE = %d\n", LIMITE);   // 100
    #undef LIMITE
    #define LIMITE 200
    printf("LIMITE (redefinido) = %d\n", LIMITE);  // 200

    // ---- Plataforma: compilacao condicional por sistema ----
    #ifdef _WIN32
        printf("Compilado para Windows\n");
    #else
        printf("Compilado para outro sistema\n");
    #endif

    return 0;
}
```

Resumo das boas praticas:
- Sempre use parenteses em macros com parametros.
- Use include guards em todos os headers (`.h`).
- Use `#ifdef DEBUG` para cercar mensagens de debug que nao devem ir para producao.
- Cuidado com efeitos colaterais em macros (`QUADRADO(i++)` incrementa duas vezes).
