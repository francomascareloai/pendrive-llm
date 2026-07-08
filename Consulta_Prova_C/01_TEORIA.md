# 01 - TEORIA, PEGADINHAS E CASOS EXTREMOS

Documento de teoria para a parte OBJETIVA da prova (multipla escolha,
analise de codigo, assercoes com PORQUE) e pegadinhas/armadilhas de C.

INDICE DESTE ARQUIVO:
  PARTE 1 - Teoria da prova objetiva (assercoes com PORQUE, recursao, strcmp)
  PARTE 2 - Pegadinhas e armadilhas (46 pegadinhas com errado/certo)
  PARTE 3 - Casos extremos e tratamento de erros (fragil vs robusto)


#############################################################
# PARTE 1 - TEORIA DA PROVA OBJETIVA
#############################################################

# TEORIA PROVA GB - Algoritmos e Programacao II (Uniftec)

> Material de CONSULTA para a parte TEORICA da prova do Grau B.
> Aluno: Franco. Linguagem C. Compilador: gcc (MinGW) no Windows.
> Palavras-chave para Ctrl+F: teoria, multipla escolha, assercao, PORQUE, recursao,
> caso base, printf ordem, strcmp retorno, struct vantagem, analise codigo, prever saida,
> pegadinha, ponteiro, arquivo, feof, gets, switch break, cast inteiro.

---

## INDICE

1. FORMATO das questoes teoricas (multipla escolha, analise de codigo, assercoes com PORQUE)
2. RECURSAO (caso base, caso recursivo, pilha de chamadas, PEGADINHA da ordem do printf)
3. STRCMP (retorno 0/<0/>0, pegadinha if(strcmp) vs if(strcmp==0), assercao completa)
4. STRUCT (vantagem de usar, questao teorica)
5. ANALISE DE CODIGO (5 trechos: o que faz / qual saida / qual erro)
6. ASSERCOES COM PORQUE (6 questoes no formato I/II com gabarito)
7. PONTEIROS (& vs *, parametro para alterar valor, ponteiro para struct com ->)
8. ERROS COMUNS que a prova cobra identificar
9. GABARITO RAPIDO (resumo de todas as respostas)

---

## 1. FORMATO DAS QUESTOES TEORICAS

A prova teorica tem TRES formatos principais. Voce precisa reconhecer cada um
para nao perder pontos por desatencao.

### 1.1 Multipla escolha (a-e)

Questao classica com 5 alternativas. So UMA esta correta.

ESTRATEGIA: leia o enunciado COMPLETO antes de olhar as alternativas. Muitas
vezes uma alternativa "parece" certa ate voce ler a ultima linha do enunciado
que muda o contexto.

Exemplo de estrutura:
```
Assinale a alternativa correta:
a) ...
b) ...
c) ...
d) ...
e) ...
```

PEGADINHA CLASSICA: alternativas com palavras absolutas ("sempre", "nunca",
"obrigatoriamente", "em qualquer caso") costumam ser FALSAS. Em C quase tudo
depende do contexto.

### 1.2 Analise de trecho de codigo

Mostram um pedaco de codigo e perguntam:
- "Qual a saida deste programa?"
- "O que este trecho faz?"
- "Qual o erro neste codigo?"
- "E possivel compilar? Se sim, o que imprime?"

ESTRATEGIA: simule a execuacao LINHA POR LINHA no papel. Anote o valor de cada
variavel a cada iteracao. Nao tente "adivinhar" - C nao tem surpresas se voce
seguir a execucao passo a passo.

### 1.3 Assercoes com PORQUE (formato concurso) -- O MAIS IMPORTANTE

Este formato confunde MUITO aluno. Leia com atencao.

A questao apresenta DUAS proposicoes, uma chamada de ASSESSAO I (ou afirmativa I)
e outra ASSESSAO II (ou afirmativa II). A segunda vem precedida da palavra
"PORQUE". A relacao entre elas e o que voce deve julgar.

Formato padrao:

```
As assercoes I e II sao proposicoes, respectivamente, uma afirmacao e uma
causa, e a respeito delas, assinale a alternativa correta:

I - [afirmacao]
PORQUE
II - [causa/explicacao]

A) se I e II sao verdadeiras e II justifica I.
B) se I e II sao verdadeiras e II nao justifica I.
C) se I e verdadeira e II e falsa.
D) se I e falsa e II e verdadeira.
E) se I e II sao falsas.
```

COMO RESOLVER (passo a passo):

PASSO 1: Julgue I isoladamente. I e verdadeira ou falsa?
PASSO 2: Julgue II isoladamente. II e verdadeira ou falsa?
PASSO 3: Se AMBAS forem verdadeiras, pergunte: II e a CAUSA (a justificativa)
         de I? Ou seja, I e verdadeira PORQUE II e verdadeira?
PASSO 4: Escolha a alternativa conforme o quadro:

| I     | II    | II justifica I? | Alternativa |
|-------|-------|-----------------|-------------|
| V     | V     | SIM             | A           |
| V     | V     | NAO             | B           |
| V     | F     | (nao se aplica) | C           |
| F     | V     | (nao se aplica) | D           |
| F     | F     | (nao se aplica) | E           |

ATENCAO: so faz sentido falar em "justificar" quando AMBAS sao verdadeiras.
Se uma e falsa, a resposta e C, D ou E - nunca A ou B.

PEGADINHA: as duas afirmacoes podem ser verdadeiras mas NAO ter relacao de
causa. Exemplo: "I - A terra e redonda. PORQUE II - A agua ferve a 100C."
Ambas verdadeiras, mas II nao explica I. Resposta: B.

Outra pegadinha: II pode ser verdadeira mas ser a causa ERRADA. Exemplo:
"I - O strcmp retorna 0 quando as strings sao iguais. PORQUE II - O strcmp
compara os ponteiros das strings." I e verdadeira, mas II e FALSA (strcmp
compara os caracteres, nao os ponteiros). Resposta: C.

---

## 2. RECURSAO (QUESTAO 1 CONFIRMADA)

Palavras-chave: recursao, caso base, caso recursivo, pilha de chamadas,
printf ordem, prever saida, pegadinha.

### 2.1 Conceito

Recursao e quando uma funcao chama A SI MESMA. Toda funcao recursiva precisa
de DUAS partes:

1. CASO BASE: condicao de PARADA. Quando o problema e simples o suficiente
   para resolver diretamente, sem nova chamada. SEM caso base, a funcao chama
   a si mesma infinitamente ate estourar a pilha (stack overflow).

2. CASO RECURSIVO: a funcao chama a si mesma com um problema MENOR (mais
   proximo do caso base). A cada chamada, o parametro deve se aproximar do
   caso base, senao a recursao nunca termina.

Estrutura padrao:
```c
void recursao(int n) {
    if (n == 0) {       // CASO BASE: para quando n chega a 0
        return;        // retorna sem fazer nada (para a recursao)
    }
    // caso recursivo: faz algo e chama a si mesma com n-1
    recursao(n - 1);    // chamada recursiva (n diminui, aproxima do base)
}
```

### 2.2 Pilha de chamadas (como funciona por dentro)

Cada chamada recursiva cria um NOVO "quadro" (frame) na pilha de execuacao.
As variaveis locais de cada chamada sao INDEPENDENTES. Quando a funcao retorna,
o quadro e destruido e a execucao volta para a chamada anterior, no ponto
exato onde parou.

Isso e o segredo para entender a PEGADINHA da ordem do printf.

Exemplo: recursao(3)
```
recursao(3) -> chama recursao(2) -> chama recursao(1) -> chama recursao(0)
                                                          caso base, retorna
                                                       volta para recursao(1)
                                                    volta para recursao(2)
                                                 volta para recursao(3)
                                              termina
```

A "ida" (descendo) faz as chamadas. A "volta" (subindo) executa o codigo que
estava DEPOIS da chamada recursiva.

### 2.3 A PEGADINHA DA ORDEM DO PRINTF (CAI NA PROVA)

Esta e a pegadinha MAIS CLASSICA de recursao. Decore isto:

- printf ANTES da chamada recursiva = imprime em ordem DECRESCENTE (na ida).
- printf DEPOIS da chamada recursiva = imprime em ordem CRESCENTE (na volta).

#### Exemplo A: printf ANTES (ordem decrescente)

```c
void funcao(int n) {
    if (n == 0) return;       // caso base
    printf("%d ", n);         // IMPRIME ANTES de chamar
    funcao(n - 1);            // chamada recursiva
}
```

Chamada: `funcao(5);`
SAIDA: `5 4 3 2 1`

PORQUE: o printf executa ANTES de descer para a proxima chamada. Entao imprime
5 (antes de chamar funcao(4)), depois imprime 4 (antes de chamar funcao(3)),
e assim por diante ate 1.

#### Exemplo B: printf DEPOIS (ordem crescente)

```c
void funcao(int n) {
    if (n == 0) return;       // caso base
    funcao(n - 1);            // chamada recursiva PRIMEIRO
    printf("%d ", n);         // IMPRIME DEPOIS de voltar
}
```

Chamada: `funcao(5);`
SAIDA: `1 2 3 4 5`

PORQUE: a chamada recursiva acontece ANTES do printf. Entao a funcao desce
ate o caso base (n=0) SEM imprimir nada. Quando comeca a VOLTAR, o printf
executa na ordem inversa: primeiro n=1 (ultima chamada antes do base),
depois n=2, ..., ate n=5.

#### Exemplo C: os DOIS (ida e volta)

```c
void funcao(int n) {
    if (n == 0) return;
    printf("%d ", n);         // imprime na ida
    funcao(n - 1);
    printf("%d ", n);         // imprime na volta
}
```

Chamada: `funcao(3);`
SAIDA: `3 2 1 1 2 3`

PORQUE: na ida imprime 3, 2, 1 (decrescente). Depois atinge o caso base.
Na volta, imprime 1, 2, 3 (crescente). Resultado espelhado.

### 2.4 Exercicios "prever a saida" (com resposta)

#### Exercicio 1
```c
void f(int n) {
    if (n > 0) {
        printf("%d", n);
        f(n - 1);
    }
}
// chamada: f(4);
```
RESPOSTA: `4321`
COMENTARIO: printf antes da chamada = ordem decrescente. Imprime 4, depois
desce para f(3) que imprime 3, etc. Caso base implicito: n>0 falso (n=0).

#### Exercicio 2
```c
void f(int n) {
    if (n > 0) {
        f(n - 1);
        printf("%d", n);
    }
}
// chamada: f(4);
```
RESPOSTA: `1234`
COMENTARIO: printf depois da chamada = ordem crescente. Desce ate f(0) sem
imprimir, depois volta imprimindo 1, 2, 3, 4.

#### Exercicio 3
```c
void f(int n) {
    if (n == 0) return;
    printf("%d", n % 2);      // imprime o RESTO da divisao por 2
    f(n / 2);                 // divide por 2 a cada chamada
}
// chamada: f(13);
```
RESPOSTA: `1011`
COMENTARIO: 13%2=1, 13/2=6 -> 6%2=0, 6/2=3 -> 3%2=1, 3/2=1 -> 1%2=1, 1/2=0
(caso base). Imprime 1,0,1,1. (Converte 13 para binario lendo ao contrario:
13 em binario e 1101, mas como imprime na ida, sai 1011 que e o reverso.)

#### Exercicio 4
```c
int soma(int n) {
    if (n == 1) return 1;     // caso base
    return n + soma(n - 1);   // caso recursivo
}
// chamada: printf("%d", soma(4));
```
RESPOSTA: `10`
COMENTARIO: soma(4) = 4 + soma(3) = 4 + 3 + soma(2) = 4+3+2+soma(1) =
4+3+2+1 = 10. Recursao que CALCULA (soma de 1 a n). Note que o return
espera a chamada recursiva terminar antes de somar - por isso o calculo
funciona "de baixo para cima".

#### Exercicio 5 (fatorial - classico)
```c
int fat(int n) {
    if (n <= 1) return 1;     // caso base: 0! = 1! = 1
    return n * fat(n - 1);   // caso recursivo
}
// chamada: printf("%d", fat(5));
```
RESPOSTA: `120`
COMENTARIO: fat(5) = 5*fat(4) = 5*4*fat(3) = ... = 5*4*3*2*1 = 120.

#### Exercicio 6 (PEGADINHA: sem caso base correto)
```c
void f(int n) {
    printf("%d", n);
    f(n - 1);                // NAO tem caso base!
}
// chamada: f(3);
```
RESPOSTA: loop infinito ate STACK OVERFLOW (crash). Imprime 3,2,1,0,-1,-2,...
ate estourar a pilha.
COMENTARIO: falta o "if (n == 0) return;". A recursao nunca para. Erro classico.

---

## 3. STRCMP (CONFIRMADO COM ASSERCOES)

Palavras-chave: strcmp retorno, strcmp igual, pegadinha if strcmp, comparar string.

### 3.1 O que strcmp faz

A funcao `strcmp(a, b)` compara duas strings CARACTERE por CARACTERE (nao
compara ponteiros, compara o CONTEUDO). Retorna um inteiro:

| Retorno        | Significado                          |
|----------------|--------------------------------------|
| 0              | strings IGUAIS                       |
| < 0 (negativo) | a vem ANTES de b (a < b)             |
| > 0 (positivo) | a vem DEPOIS de b (a > b)            |

IMPORTANTE: NAO retorna necessariamente -1 e +1. Pode retornar qualquer
valor negativo ou positivo (geralmente a diferenca entre os caracteres
divergentes). Por isso NUNCA compare com `== 1` ou `== -1`. Compare com
`< 0`, `> 0` ou `== 0`.

### 3.2 A PEGADINHA CLASSICA: if(strcmp) vs if(strcmp==0)

Esta pegadinha cai em TODA prova de C. Entenda a logica:

`if (strcmp(a, b))` significa "se strcmp retornar DIFERENTE DE ZERO".
Como strcmp retorna 0 quando as strings sao IGUAIS, `if (strcmp(a,b))` entra
no bloco quando as strings sao DIFERENTES. O contrario do que parece!

`if (strcmp(a, b) == 0)` significa "se strcmp retornar zero", ou seja,
"se as strings sao iguais". Este e o jeito CORRETO de testar igualdade.

```c
// ERRADO - testa se sao DIFERENTES (nao iguais):
if (strcmp(senha, "1234")) {
    printf("Senha correta!");  // VAI IMPRIMIR quando a senha ESTIVER ERRADA!
}

// CERTO - testa se sao IGUAIS:
if (strcmp(senha, "1234") == 0) {
    printf("Senha correta!");  // so imprime quando a senha estiver certa
}
```

DECORE: em C, ZERO significa FALSO e qualquer coisa DIFERENTE DE ZERO significa
VERDADEIRO. Como strcmp retorna 0 para "igual", o if "natural" inverte a logica.

Tambem aceito (menos comum): `if (!strcmp(a, b))` - o `!` inverte, entao
"nao zero" = verdadeiro quando sao iguais.

### 3.3 Questao de assercao completa sobre strcmp

```
As assercoes I e II sao proposicoes, respectivamente, uma afirmacao e uma
causa, e a respeito delas, assinale a alternativa correta:

I - A expressao "if (strcmp(a, b))" executa o bloco do if quando as strings
    a e b sao DIFERENTES.
PORQUE
II - A funcao strcmp retorna o valor 0 (zero) quando as duas strings sao
     iguais, e em C o valor 0 e interpretado como falso em testes condicionais.

A) se I e II sao verdadeiras e II justifica I.
B) se I e II sao verdadeiras e II nao justifica I.
C) se I e verdadeira e II e falsa.
D) se I e falsa e II e verdadeira.
E) se I e II sao falsas.
```

GABARITO: A

EXPLICACAO:
- I e VERDADEIRA: `if (strcmp(a,b))` executa o bloco quando strcmp retorna
  diferente de zero, ou seja, quando as strings sao diferentes.
- II e VERDADEIRA: strcmp retorna 0 para strings iguais, e em C 0 = falso.
- II JUSTIFICA I: e justamente PORQUE strcmp retorna 0 para iguais (e 0 e
  falso) que o if entra no bloco quando sao diferentes. A causa explica a
  afirmacao. Resposta A.

### 3.4 Outras questoes de multipla escolha sobre strcmp

QUESTAO: Qual alternativa usa CORRETAMENTE strcmp para verificar se duas
strings s1 e s2 sao iguais?
a) if (strcmp(s1, s2) == 1)
b) if (strcmp(s1, s2) = 0)
c) if (strcmp(s1, s2) == 0)
d) if (strcmp(s1, s2))
e) if (strcmp(s1, s2) != 0)

GABARITO: c
COMENTARIO: a) errado (nao retorna necessariamente 1). b) errado (usa = que
e atribuicao, nao ==). d) errado (entra quando sao DIFERENTES). e) errado
(idem). c) correto: == 0 testa igualdade.

---

## 4. STRUCT (VANTAGEM DE USAR)

Palavras-chave: struct vantagem, agrupar dados, typedef struct.

### 4.1 O que e struct

Struct (estrutura) e um tipo de dado composto que agrupa variaveis de tipos
DIFERENTES sob um unico nome. Permite representar uma "entidade" com varios
campos relacionados.

```c
struct Aluno {
    char nome[50];
    int matricula;
    float nota;
};
```

### 4.2 Vantagem de usar struct (cai na teorica)

A principal vantagem: AGRUPAR dados relacionados que pertencem a uma mesma
entidade, facilitando a organizacao, passagem para funcoes e manipulacao.
Em vez de passar 5 variaveis separadas para uma funcao, voce passa UMA struct.

Outras vantagens:
- Organiza dados relacionados em um so bloco logico.
- Facilita passar varios dados para uma funcao (passa a struct, nao N campos).
- Permite criar vetores de registros (ex: vetor de 100 alunos).
- Reutilizacao de codigo (define o tipo uma vez, usa varias).

### 4.3 Questao teorica sobre struct

QUESTAO: Assinale a alternativa que descreve CORRETAMENTE a principal vantagem
do uso de struct em C:

a) Permitir a criacao de variaveis do mesmo tipo com nomes diferentes.
b) Agrupar dados relacionados de tipos possivelmente diferentes sob um unico
   nome, facilitando a organizacao e a passagem desses dados para funcoes.
c) Substituir o uso de ponteiros na linguagem C.
d) Aumentar a velocidade de execucao do programa em relacao a variaveis simples.
e) Permitir a alocacao dinamica de memoria automaticamente.

GABARITO: b
COMENTARIO: a) errado (isso qualquer variavel faz). c) errado (struct nao
substitui ponteiro, inclusive usa-se ponteiro para struct). d) errado (struct
nao aumenta velocidade, pode ate deixar mais lento por alinhamento). e) errado
(alocacao dinamica e com malloc, nao automatica por struct). b) correto: agrupar
dados relacionados e a vantagem central.

### 4.4 typedef struct (forma usada pelo professor)

O professor costuma usar `typedef struct` para criar um "apelido" de tipo,
evitando escrever `struct` toda vez:

```c
typedef struct {
    char nome[50];
    int matricula;
    float nota;
} Aluno;                    // agora "Aluno" e um tipo (sem "struct" antes)

// uso:
Aluno a1;                   // em vez de "struct Aluno a1;"
a1.nota = 8.5;              // acesso com ponto (.)
```

ACESSO: variavel simples usa PONTO (`.`). Ponteiro para struct usa SETA (`->`).

---

## 5. ANALISE DE CODIGO (5 TRECHOS)

Palavras-chave: analise codigo, prever saida, qual erro, feof, gets, switch break.

### 5.1 Trecho 1 - PEGADINHA do feof

```c
FILE *f = fopen("dados.txt", "r");
int x;
while (!feof(f)) {          // PEGADINHA CLASSICA
    fscanf(f, "%d", &x);
    printf("%d ", x);
}
fclose(f);
```

PERGUNTA: Qual o problema deste codigo?

RESPOSTA: O loop vai imprimir o ULTIMO valor DUAS VEZES (ou lixo na ultima
iteracao).

EXPLICACAO: `feof(f)` so retorna VERDADEIRO DEPOIS de uma tentativa de leitura
que passou do fim do arquivo. Ou seja, o fscanf falha na ultima leitura (nao
le nada), mas o feof ainda e falso NAQUELE momento, entao o loop executa mais
uma vez com o valor antigo de x (ou lixo). So na iteracao seguinte o feof
vira verdadeiro.

JEITO CERTO: testar o retorno do fscanf:
```c
while (fscanf(f, "%d", &x) == 1) {   // leu 1 item com sucesso
    printf("%d ", x);
}
```

### 5.2 Trecho 2 - PEGADINHA do gets (estouro de buffer)

```c
char nome[10];
gets(nome);                 // PEGADINHA: inseguro!
printf("Ola, %s", nome);
```

PERGUNTA: Qual o problema e qual a alternativa correta?

RESPOSTA: `gets` nao verifica o tamanho do buffer. Se o usuario digitar mais
de 9 caracteres (o 10 e o '\0'), ocorre ESTOURO DE BUFFER (buffer overflow),
gravando fora da memoria reservada - pode corromper dados ou causar crash.

ALTERNATIVA CORRETA: `fgets(nome, 10, stdin);` que limita a leitura ao tamanho
do buffer. Note: fgets inclui o '\n' se couber, e sempre coloca '\0'.

### 5.3 Trecho 3 - PEGADINHA comparar string com ==

```c
char senha[20];
scanf("%s", senha);
if (senha == "1234") {       // PEGADINHA: errado!
    printf("Acesso liberado");
}
```

PERGUNTA: Este codigo funciona? Por que nao?

RESPOSTA: NAO funciona. A comparacao `senha == "1234"` compara PONTEIROS
(enderecos de memoria), nao o conteudo das strings. Mesmo que o usuario digite
"1234", o endereco de senha sera diferente do endereco da string literal
"1234", entao a condicao sera SEMPRE falsa.

JEITO CERTO: `if (strcmp(senha, "1234") == 0)`.

### 5.4 Trecho 4 - PEGADINHA = vs ==

```c
int x = 5;
if (x = 10) {                // PEGADINHA: = e atribuicao, nao comparacao!
    printf("x e dez");
}
```

PERGUNTA: O que acontece?

RESPOSTA: Sempre imprime "x e dez", e x passa a valer 10.

EXPLICACAO: `x = 10` e uma ATRIBUICAO (atribui 10 a x), nao uma comparacao.
O valor da expressao de atribuicao e o valor atribuido (10). Como 10 e
diferente de zero, e considerado VERDADEIRO, e o if sempre entra. O correto
seria `if (x == 10)`. O gcc com -Wall gera WARNING para isto.

### 5.5 Trecho 5 - PEGADINHA esquecer break no switch

```c
int op = 2;
switch (op) {
    case 1: printf("Um ");
    case 2: printf("Dois ");
    case 3: printf("Tres ");
    default: printf("Outro ");
}
```

PERGUNTA: Qual a saida?

RESPOSTA: `Dois Tres Outro `

EXPLICACAO: Sem o `break`, apos entrar no case 2, a execucao CONTINUA pelos
cases seguintes (3 e default) sem testar. Isso se chama "fall-through".
Para imprimir so "Dois", cada case precisa de `break` no final:
```c
case 2: printf("Dois "); break;
```

### 5.6 Trecho 6 - PEGADINHA divisao de inteiros

```c
int a = 5, b = 2;
float r = a / b;
printf("%.2f", r);
```

PERGUNTA: Qual a saida?

RESPOSTA: `2.00` (nao 2.50!)

EXPLICACAO: Como a e b sao int, a divisao `a / b` e INTEIRA (5/2 = 2, descarta
o resto). So DEPOIS o resultado (2) e convertido para float e atribuido a r.
Para obter 2.50, precisa de CAST em pelo menos um operando:
```c
float r = (float) a / b;    // converte a para float antes da divisao
```
Resultado: `2.50`.

---

## 6. ASSERCOES COM PORQUE (6 QUESTOES)

Palavras-chave: assercao, PORQUE, afirmacao I, afirmacao II, justifica.

### 6.1 Questao 1 - Recursao (caso base)

```
I - Toda funcao recursiva deve possuir um caso base para evitar recursao
    infinita.
PORQUE
II - O caso base e a condicao que interrompe as chamadas recursivas, fazendo
     a execucao comecar a retornar.

A) se I e II sao verdadeiras e II justifica I.
B) se I e II sao verdadeiras e II nao justifica I.
C) se I e verdadeira e II e falsa.
D) se I e falsa e II e verdadeira.
E) se I e II sao falsas.
```

GABARITO: A

EXPLICACAO:
- I VERDADEIRA: sem caso base, a funcao chama a si mesma para sempre ate
  estourar a pilha (stack overflow). O caso base e OBRIGATORIO.
- II VERDADEIRA: o caso base e exatamente a condicao de parada que faz a
  execucao voltar (retornar).
- II JUSTIFICA I: e justamente PORQUE o caso base interrompe as chamadas que
  ele e necessario para evitar recursao infinita. A causa explica a afirmacao.

### 6.2 Questao 2 - Recursao (ordem do printf)

```
I - Na funcao recursiva abaixo, a saida para f(4) sera "1234".
    void f(int n) {
        if (n == 0) return;
        f(n - 1);
        printf("%d", n);
    }
PORQUE
II - Quando o printf esta DEPOIS da chamada recursiva, ele executa na "volta"
     da pilha de chamadas, imprimindo em ordem crescente.

A) se I e II sao verdadeiras e II justifica I.
B) se I e II sao verdadeiras e II nao justifica I.
C) se I e verdadeira e II e falsa.
D) se I e falsa e II e verdadeira.
E) se I e II sao falsas.
```

GABARITO: A

EXPLICACAO:
- I VERDADEIRA: f(4) desce ate f(0) sem imprimir, depois volta imprimindo
  1, 2, 3, 4. Saida: "1234".
- II VERDADEIRA: printf depois da chamada executa na volta (crescente).
- II JUSTIFICA I: e justamente PORQUE o printf executa na volta que a saida
  sai em ordem crescente (1234). A causa explica a afirmacao.

### 6.3 Questao 3 - strcmp (pegadinha do if)

```
I - O codigo abaixo imprime "Diferentes" quando as strings sao IGUAIS.
    if (strcmp(a, b)) {
        printf("Diferentes");
    }
PORQUE
II - strcmp retorna 0 quando as strings sao iguais, e 0 e considerado falso
     em C, portanto o bloco do if NAO executa quando sao iguais.

A) se I e II sao verdadeiras e II justifica I.
B) se I e II sao verdadeiras e II nao justifica I.
C) se I e verdadeira e II e falsa.
D) se I e falsa e II e verdadeira.
E) se I e II sao falsas.
```

GABARITO: D

EXPLICACAO:
- I FALSA: o codigo NAO imprime "Diferentes" quando as strings sao iguais.
  Quando sao iguais, strcmp retorna 0 (falso), e o bloco do if NAO executa.
  A afirmacao I inverte a logica.
- II VERDADEIRA: strcmp retorna 0 para iguais, 0 e falso, bloco nao executa.
- Como I e falsa e II e verdadeira, resposta D.
ATENCAO: II explica o comportamento REAL (nao imprime quando iguais), mas
como I esta errada, nao ha como II justificar I. Resposta D.

### 6.4 Questao 4 - Struct (agrupamento)

```
I - O uso de struct permite agrupar variaveis de tipos diferentes sob um
    unico nome, representando uma entidade com varios campos.
PORQUE
II - Struct armazena todos os seus campos em posicoes contiguas de memoria,
     permitindo acessar cada campo pelo operador ponto (.).

A) se I e II sao verdadeiras e II justifica I.
B) se I e II sao verdadeiras e II nao justifica I.
C) se I e verdadeira e II e falsa.
D) se I e falsa e II e verdadeira.
E) se I e II sao falsas.
```

GABARITO: B

EXPLICACAO:
- I VERDADEIRA: struct agrupa campos de tipos diferentes sob um nome.
- II VERDADEIRA: os campos ficam em memoria contigua e acessa-se com ponto.
- II NAO JUSTIFICA I: o fato de os campos estarem em memoria contigua e serem
  acessados com ponto e uma caracteristica de IMPLEMENTACAO, nao a RAZAO pela
  qual struct agrupa dados. A vantagem (I) existe independentemente do layout
  de memoria. As duas sao verdadeiras mas sem relacao de causa. Resposta B.
  (Esta e a pegadinha classica do formato: ambas verdadeiras, mas II nao e a
  causa de I.)

### 6.5 Questao 5 - Arquivo (modo "w" vs "r")

```
I - Abrir um arquivo com fopen no modo "w" apaga o conteudo anterior do
    arquivo se ele ja existir.
PORQUE
II - O modo "w" (write) sempre cria um arquivo novo, descartando qualquer
     conteudo pre-existente, mesmo que o arquivo ja exista.

A) se I e II sao verdadeiras e II justifica I.
B) se I e II sao verdadeiras e II nao justifica I.
C) se I e verdadeira e II e falsa.
D) se I e falsa e II e verdadeira.
E) se I e II sao falsas.
```

GABARITO: A

EXPLICACAO:
- I VERDADEIRA: modo "w" trunca (zera) o arquivo se existir.
- II VERDADEIRA: "w" cria/reescreve, descartando conteudo anterior.
- II JUSTIFICA I: e justamente PORQUE "w" descarta o conteudo pre-existente
  que o arquivo fica "zerado" ao abrir. A causa explica a afirmacao.

### 6.6 Questao 6 - Ponteiro (passagem por referencia)

```
I - Para que uma funcao altere o valor de uma variavel declarada na funcao
    chamadora, e necessario passar o ENDERECO dessa variavel como argumento.
PORQUE
II - Em C, os parametros sao passados por valor (copia), entao alterar o
     parametro dentro da funcao nao afeta a variavel original a menos que
     se use o endereco (ponteiro).

A) se I e II sao verdadeiras e II justifica I.
B) se I e II sao verdadeiras e II nao justifica I.
C) se I e verdadeira e II e falsa.
D) se I e falsa e II e verdadeira.
E) se I e II sao falsas.
```

GABARITO: A

EXPLICACAO:
- I VERDADEIRA: para alterar a variavel original, passa-se &variavel.
- II VERDADEIRA: C passa por valor (copia); so com ponteiro a alteracao
  reflete na original.
- II JUSTIFICA I: e justamente PORQUE C passa por valor que e preciso passar
  o endereco para a alteracao funcionar. A causa explica a afirmacao.

---

## 7. PONTEIROS

Palavras-chave: ponteiro, & vs *, passagem por referencia, ponteiro struct, seta ->.

### 7.1 & vs * (o mais basico)

- `&` (E comercial) = "endereco de". Devolve o ENDERECO de uma variavel.
  Ex: `&x` e o endereco onde x esta na memoria.
- `*` (asterisco) = "conteudo de" / "desreferencia". Acessa o VALOR apontado
  por um ponteiro. Ex: se `p` aponta para x, entao `*p` e o valor de x.

```c
int x = 10;
int *p = &x;        // p recebe o ENDERECO de x
printf("%d", *p);  // imprime 10 (conteudo apontado por p)
*p = 20;           // altera o valor de x ATRAVES do ponteiro
printf("%d", x);   // agora x vale 20
```

DECORE: na DECLARACAO `int *p;` o * indica que p e ponteiro. No USO `*p`
o * significa "valor apontado". Contexto diferente!

### 7.2 Ponteiro como parametro (passagem por referencia)

C passa parametros por VALOR (copia). Para uma funcao MODIFICAR a variavel
da funcao chamadora, passe o ENDERECO (ponteiro):

```c
void dobra(int *n) {        // recebe um PONTEIRO para int
    *n = *n * 2;            // multiplica o VALOR apontado por 2
}

int main() {
    int v = 5;
    dobra(&v);               // passa o ENDERECO de v
    printf("%d", v);         // imprime 10 (v foi alterado!)
    return 0;
}
```

SEM o ponteiro, a funcao trabalharia com uma COPIA e a original nao mudaria:
```c
void dobraErrado(int n) {   // recebe uma COPIA
    n = n * 2;               // altera a copia, original intacta
}
// v continuaria 5 apos a chamada
```

### 7.3 Ponteiro para struct (operador ->)

Quando se tem um PONTEIRO para struct, usa-se o operador SETA (`->`) para
acessar os campos (equivale a `(*p).campo`):

```c
typedef struct {
    char nome[50];
    float nota;
} Aluno;

void aumentaNota(Aluno *a) {     // recebe PONTEIRO para Aluno
    a->nota = a->nota + 1;       // usa -> (seta) para acessar campos
}

int main() {
    Aluno a1 = {"Joao", 7.0};
    aumentaNota(&a1);            // passa o endereco da struct
    printf("%.1f", a1.nota);     // imprime 8.0 (struct simples usa ponto)
    return 0;
}
```

EQUIVALENCIA: `a->nota` e o mesmo que `(*a).nota`. A seta e so um atalho.

RESUMO de acesso:
- variavel SIMPLES de struct: usa PONTO (`a1.nota`).
- PONTEIRO para struct: usa SETA (`p->nota`).

---

## 8. ERROS COMUNS QUE A PROVA COBRA IDENTIFICAR

Palavras-chave: erro comum, = vs ==, esquecer &, comparar string ==, feof errado,
estouro buffer, switch break, cast inteiro.

### 8.1 = vs == (atribuicao vs comparacao)

- `=` atribui um valor. `==` compara dois valores.
- `if (x = 5)` ATRIBUI 5 a x e testa (sempre verdadeiro, pois 5 != 0).
- `if (x == 5)` compara x com 5 (verdadeiro so se x for 5).
- O gcc com -Wall AVISA sobre isto ("suggest parentheses around assignment
  used as truth value").

### 8.2 Esquecer & no scanf

- `scanf("%d", x);` ERRADO - passa o valor (lixo) em vez do endereco.
- `scanf("%d", &x);` CERTO - passa o endereco onde scanf deve gravar.
- EXCECAO: strings (char[]) NAO levam &, pois o nome do vetor ja e um endereco:
  `scanf("%s", nome);` (sem &).

### 8.3 Comparar string com ==

- `if (senha == "1234")` compara PONTEIROS, sempre falso.
- Use `strcmp(senha, "1234") == 0`.

### 8.4 feof errado (loop a mais)

- `while (!feof(f))` executa uma iteracao A MAIS (le o ultimo item duplicado).
- Use `while (fscanf(...) == 1)` testando o retorno da leitura.

### 8.5 Estouro de buffer (gets)

- `gets(buf)` nao limita a leitura - se digitar mais que o tamanho, estoura.
- Use `fgets(buf, tamanho, stdin)`.

### 8.6 Esquecer break no switch

- Sem break, apos entrar num case, a execucao CONTINUA pelos cases seguintes
  (fall-through). quase sempre nao e o desejado.

### 8.7 Divisao de inteiros sem cast

- `5 / 2` com inteiros = 2 (descarta resto).
- Para resultado decimal: `(float) 5 / 2` = 2.5.

### 8.8 Outros erros comuns (bonus)

- Esquecer `return` em funcao nao-void (retorna lixo).
- Usar `==` para comparar floats (precisao: use intervalo ou epsilon).
- Confundir `&&` com `&` (logico vs bitwise).
- Esquecer `fclose` (vaza descritor de arquivo).
- Indice de vetor fora do limite (vetor[10] acessando de 0 a 10 - o 10 e erro).
- `scanf("%s", &nome)` quando nome e string (NAO leva & - ja e endereco).
- Inicializar struct sem chaves na ordem errada dos campos.
- Modo "r" em arquivo inexistente retorna NULL e da crash se nao testar.

---

## 9. GABARITO RAPIDO (RESUMO DAS RESPOSTAS)

Use esta secao para conferir rapidamente as respostas de cada questao.

### 9.1 Analise de codigo (secao 5)
- Trecho 1 (feof): imprime ultimo valor duplicado. Correto: testar fscanf == 1.
- Trecho 2 (gets): estouro de buffer. Correto: fgets.
- Trecho 3 (== em string): sempre falso. Correto: strcmp == 0.
- Trecho 4 (= vs ==): sempre entra, x vira 10. Correto: ==.
- Trecho 5 (switch sem break): "Dois Tres Outro". Correto: break.
- Trecho 6 (divisao int): "2.00". Correto: (float) cast.

### 9.2 Prever saida - recursao (secao 2.4)
- Exercicio 1: 4321
- Exercicio 2: 1234
- Exercicio 3: 1011
- Exercicio 4: 10
- Exercicio 5: 120
- Exercicio 6: stack overflow (sem caso base)

### 9.3 Assercoes (secao 6)
- 6.1 (recursao caso base): A
- 6.2 (recursao ordem printf): A
- 6.3 (strcmp if): D
- 6.4 (struct agrupamento): B
- 6.5 (arquivo modo w): A
- 6.6 (ponteiro passagem): A

### 9.4 Multipla escolha
- 3.4 (strcmp correto): c
- 4.3 (vantagem struct): b

---

## 10. DICAS FINAIS PARA A PROVA TEORICA

1. LEIA O ENUNCIADO INTEIRO antes de marcar. Muita pegadinha esta na ultima linha.

2. Nas assercoes, julgue I e II SEPARADAMENTE primeiro. So pense em
   "justifica" se AMBAS forem verdadeiras.

3. Em recursao, SEMPRE identifique: onde esta o caso base? O printf esta
   ANTES ou DEPOIS da chamada? Isso define a ordem da saida.

4. Em strcmp, lembre: 0 = iguais. if(strcmp) = se diferentes. == 0 = se iguais.

5. Em analise de codigo, SIMULE no papel. Anote variavel por variavel.

6. Cuidado com palavras absolutas ("sempre", "nunca") - em C quase tudo
   depende de contexto.

7. Se a questao mostrar codigo com gets, feof, == em string, = em vez de ==,
   ou switch sem break - o ERRO provavelmente e esse.

8. Em ponteiro para struct, lembre da SETA (->). Em struct simples, PONTO (.).

9. Divisao de inteiros descarta o resto. Use cast para resultado decimal.

10. scanf de int/float/char leva &, scanf de string (vetor) NAO leva &.

---

FIM DO DOCUMENTO TEORIA_PROVA_GB.md
Consulte tambem: ARQUIVOS.md, STRUCT_STRINGS.md, RECURSAO.md (codigos praticos).


#############################################################
# PARTE 2 - PEGADINHAS E ARMADILHAS DE C
#############################################################

# PEGADINHAS E ARMADILHAS DE C - PROVA GB

Documento EXAUSTIVO de pegadinhas de C que caem em prova (parte teorica e
analise de codigo). Para CADA pegadinha: codigo ERRADO, codigo CERTO, e o PORQUE.

Use Ctrl+F por: pegadinha, armadilha, erro, =, ==, scanf, &, string, strcmp,
feof, gets, fgets, divisao, inteira, break, switch, indice, limite, soma,
contador, dividir zero, typedef, alias, ponteiro, seta, ->, char, string,
fopen, NULL, fclose, matriz, coluna, modo, recursao, caso base, printf,
ordem, &&, ||, bitwise, incremento, ternario, curto circuito, overflow,
float, imprecisao, modulo, strlen, sizeof, strcpy, strcat, variavel,
inicializada, escopo, return, void, comentario.

=========================================================
INDICE
=========================================================
Atribuicao e comparacao (1-4)
scanf e leitura (5-11)
String (12-16)
Arquivo (17-21)
Matriz e funcao (22-23)
Recursao (24-25)
Operadores (26-29)
Numeros (30-33)
String.h (34-35)
Variaveis (36-38)
Comentario (39-40)
Questoes de analise (41-46)

=========================================================
ATRIBUICAO E COMPARACAO
=========================================================

## 1. = vs == (PEGADINHA CLASSICA)
O = atribui. O == compara. if(x=5) atribui 5 a x e, como 5 e verdadeiro,
o if SEMPRE executa.

ERRADO:
```c
int x = 3;
if (x = 5) {        // ATRIBUI 5 a x, e 5 e verdadeiro -> sempre entra
    printf("igual a 5");
}
```

CERTO:
```c
int x = 3;
if (x == 5) {       // COMPARA x com 5 -> nao entra (x e 3)
    printf("igual a 5");
}
```
PORQUE: = e atribuicao (sempre verdadeiro se o valor for !=0). == e comparacao.

## 2. Esquecer & no scanf de int/float/double/char
scanf precisa do ENDERECO da variavel. Sem &, ele le para um endereco
invalido -> crash ou lixo.

ERRADO:
```c
int idade;
scanf("%d", idade);    // FALTA o & -> comportamento indefinido
```

CERTO:
```c
int idade;
scanf("%d", &idade);  // & passa o endereco de idade
```
PORQUE: scanf recebe o endereco onde guardar o valor. & pega o endereco.

## 3. Colocar & em string no scanf
String (vetor de char) JA e um endereco (o nome do vetor). Nao usa &.

ERRADO:
```c
char nome[50];
scanf("%49s", &nome);   // ERRADO: nome ja e endereco
```

CERTO:
```c
char nome[50];
scanf("%49s", nome);    // nome sozinho ja e o endereco
```
PORQUE: o nome de um vetor decai para ponteiro para o primeiro elemento.

## 4. %f para double no scanf (deve ser %lf)
No scanf, double usa %lf. No printf, double usa %f (NAO %lf).

ERRADO:
```c
double salario;
scanf("%f", &salario);     // ERRADO: le errado para double
printf("%lf", salario);    // ERRADO: %lf no printf e nao padrao
```

CERTO:
```c
double salario;
scanf("%lf", &salario);    // %lf no scanf para double
printf("%f", salario);     // %f no printf para double
```
PORQUE: scanf precisa saber o tamanho (float=4, double=8 bytes). printf
promove float para double internamente, entao %f serve para ambos.

=========================================================
SCANF E LEITURA
=========================================================

## 5. Comparar string com == em vez de strcmp
== compara ENDERECOS, nao conteudo. Duas strings iguais em lugares
diferentes dao == falso.

ERRADO:
```c
char nome[50] = "Ana";
if (nome == "Ana") {        // ERRADO: compara enderecos, sempre falso
    printf("igual");
}
```

CERTO:
```c
#include <string.h>
if (strcmp(nome, "Ana") == 0) {   // CERTO: compara conteudo
    printf("igual");
}
```
PORQUE: nome e um endereco. "Ana" e outro endereco. == compara enderecos.

## 6. if(strcmp(a,b)) - VERDADEIRO quando DIFEREM (PEGADINHA DE PROVA)
strcmp retorna 0 se IGUAIS, !=0 se DIFERENTES. Em C, !=0 e VERDADEIRO.
Entao if(strcmp(a,b)) executa quando as strings DIFEREM, nao quando sao iguais.

ERRADO (intencao: testar igualdade):
```c
if (strcmp(senha, "1234")) {     // VERDADEIRO se DIFEREM!
    printf("acesso permitido"); // logica invertida
}
```

CERTO:
```c
if (strcmp(senha, "1234") == 0) {   // VERDADEIRO se IGUAIS
    printf("acesso permitido");
}
```
PORQUE: strcmp retorna 0 = iguais. 0 e FALSO em C. Para testar igualdade
tem que comparar com == 0. Esta e a pegadinha MAIS cobrada em prova.

## 7. while(!feof) com fread/fgets antes do loop - le ultimo 2x / lixo
feof so vira verdadeiro DEPOIS de uma leitura falhar. Se voce le antes do
loop e testa feof, le o ultimo registro duas vezes ou imprime lixo.

ERRADO:
```c
fread(&r, sizeof(r), 1, arq);     // le o primeiro
while (!feof(arq)) {              // so vira true DEPOIS de falhar
    printf(...);
    fread(&r, sizeof(r), 1, arq); // le proximo
}
// o ultimo registro e processado 2x ou imprime lixo no fim
```

CERTO:
```c
while (fread(&r, sizeof(r), 1, arq) == 1) {   // le e testa junto
    printf(...);
}
```
PORQUE: fread retorna quantos registros leu (1 se ok, 0 se fim). Testar
o retorno no while e a forma correta. feof e para uso raro, nao no loop.

## 8. gets() - estouro de buffer, removido do C
gets le sem limite. Se o usuario digitar mais que o tamanho do vetor,
estoura a memoria. Foi removido do padrao C (C11).

ERRADO:
```c
char nome[50];
gets(nome);    // PERIGOSO: sem limite, estouro de buffer
```

CERTO:
```c
char nome[50];
fgets(nome, sizeof(nome), stdin);          // le com limite
nome[strcspn(nome, "\n")] = '\0';          // tira o \n
```
PORQUE: fgets recebe o tamanho maximo e nao estoura. Precisa tirar o \n.

## 9. scanf("%c") depois de outro scanf le o enter
Depois de scanf("%d"), o enter (\n) fica no buffer. scanf("%c") le esse
enter em vez de esperar o usuario.

ERRADO:
```c
int n;
char c;
scanf("%d", &n);
scanf("%c", &c);    // le o \n que sobrou, nao espera o usuario
```

CERTO:
```c
scanf("%d", &n);
scanf(" %c", &c);   // espaco antes do %c ignora enter anterior
```
PORQUE: o espaco no formato diz ao scanf "pule espacos e enters antes
de ler o caractere".

## 10. fgets depois de scanf sem limpar buffer - pula leitura
O \n que sobra do scanf e lido pelo fgets, que acha que a linha acabou.

ERRADO:
```c
int idade;
char nome[100];
scanf("%d", &idade);                 // deixa \n no buffer
fgets(nome, sizeof(nome), stdin);    // le o \n -> nome vazio
```

CERTO:
```c
scanf("%d", &idade);
// limpar o buffer:
int ch; while ((ch = getchar()) != '\n' && ch != EOF);  // portavel
// OU fflush(stdin);  (so funciona no Windows, nao e padrao)
fgets(nome, sizeof(nome), stdin);    // agora le certo
```
PORQUE: o \n residual precisa ser consumido antes do fgets.

## 11. Nao tirar o \n do fgets
fgets guarda o \n no fim da string. Isso quebra comparacoes e gravacao.

ERRADO:
```c
fgets(nome, sizeof(nome), stdin);
if (strcmp(nome, "Ana") == 0) { ... }   // nunca igual: nome tem \n
```

CERTO:
```c
#include <string.h>
fgets(nome, sizeof(nome), stdin);
nome[strcspn(nome, "\n")] = '\0';   // troca \n por \0
```
PORQUE: strcspn acha a posicao do \n. Colocar \0 ali corta a string.

=========================================================
STRING
=========================================================

## 12. Divisao inteira: 5/2 = 2 (nao 2.5)
Dividir dois inteiros em C trunca o resultado. Para ter decimal, um dos
operandos deve ser float/double.

ERRADO:
```c
int a = 5, b = 2;
float media = a / b;     // 5/2 = 2 (inteiro), media = 2.0
```

CERTO:
```c
float media = 5.0 / 2;        // um operando float -> 2.5
// OU
float media = (float)a / b;   // cast -> 2.5
// OU
float media = a / 2.0;        // 2.0 e double -> 2.5
```
PORQUE: int/int = int (truncado). Para resultado real, um operando real.

## 13. Esquecer break no switch - cai no proximo case
Sem break, a execucao continua nos cases seguintes ("fall through").

ERRADO:
```c
switch (opcao) {
    case 1: printf("um");      // sem break
    case 2: printf("dois");   // executa tambem se opcao=1
    case 3: printf("tres");   // executa tambem
}
// se opcao=1, imprime "umdoistres"
```

CERTO:
```c
switch (opcao) {
    case 1: printf("um"); break;
    case 2: printf("dois"); break;
    case 3: printf("tres"); break;
}
```
PORQUE: break sai do switch. Sem ele, cai no proximo case.
EXCECAO: as vezes o fall-through e intencional (agrupar cases).

## 14. Indice fora do limite: vetor[5] em vetor tamanho 5
Vetor de tamanho 5 tem indices 0 a 4. Acessar vetor[5] e lixo/crash.

ERRADO:
```c
int v[5];
for (int i = 0; i <= 5; i++) {   // i vai ate 5 -> v[5] fora do limite
    v[i] = i;
}
```

CERTO:
```c
int v[5];
for (int i = 0; i < 5; i++) {    // i de 0 a 4
    v[i] = i;
}
```
PORQUE: indice valido vai de 0 ate tamanho-1. <= tamanho estoura.

## 15. Nao inicializar soma=0 e contador=0 - comeca com lixo
Variaveis locais nao inicializadas tem lixo. Soma/contador devem comecar em 0.

ERRADO:
```c
int soma;          // lixo!
int contador;      // lixo!
for (int i = 0; i < 5; i++) {
    soma += v[i];      // soma lixo + valor
    contador++;
}
```

CERTO:
```c
int soma = 0;          // inicializa
int contador = 0;      // inicializa
for (int i = 0; i < 5; i++) {
    soma += v[i];
    contador++;
}
```
PORQUE: variavel local nao inicializada tem valor indeterminado.

## 16. Dividir por zero (contador=0)
Se nenhum item satisfaz a condicao, contador fica 0 e media = soma/0.

ERRADO:
```c
int soma = 0, contador = 0;
for (int i = 0; i < n; i++) {
    if (v[i] > 0) { soma += v[i]; contador++; }
}
float media = soma / contador;   // se contador=0 -> divisao por zero!
```

CERTO:
```c
if (contador > 0) {
    float media = soma / (float)contador;
    printf("media = %.2f", media);
} else {
    printf("nenhum valor positivo");
}
```
PORQUE: sempre testar o divisor antes de dividir.

=========================================================
ARQUIVO
=========================================================

## 17. typedef struct sem alias - nao compila em C puro
Em C puro, typedef struct precisa do nome do tipo no fim. Sem alias,
nao da pra declarar variaveis com o nome da struct.

ERRADO (nao compila em C, so em C++):
```c
typedef struct alunos {
    int cod;
    char nome[20];
};
alunos alu;    // ERRO em C: 'alunos' nao e um tipo
```

CERTO:
```c
typedef struct {
    int cod;
    char nome[20];
} alunos;      // alias no fim
alunos alu;    // agora funciona
```
PORQUE: typedef cria um alias. Sem o alias apos o }, o nome da struct
so pode ser usado com a palavra 'struct' na frente (struct alunos alu).

## 18. Acessar struct com ponto quando e ponteiro (deveria ser ->)
Variavel struct usa ponto (.). Ponteiro para struct usa seta (->).

ERRADO:
```c
void alterar(Aluno *a) {
    a.media = 7.0;    // ERRADO: a e ponteiro, deveria ser ->
}
```

CERTO:
```c
void alterar(Aluno *a) {
    a->media = 7.0;   // CERTO: ponteiro usa ->
}
// ou equivalente:
(*a).media = 7.0;     // tambem funciona (menos usado)
```
PORQUE: a e um endereco. a->campo acessa o campo da struct apontada.
Ponto seria para variavel normal (Aluno a; a.media).

## 19. Confundir char ('a') com string ("a")
'a' e um caractere (1 byte, aspas simples). "a" e uma string (2 bytes:
'a' + '\0', aspas duplas).

ERRADO:
```c
char c = "a";          // ERRADO: "a" e string, c e char
if (c == "a") { ... }  // ERRADO: compara char com ponteiro
```

CERTO:
```c
char c = 'a';          // CERTO: aspas simples para char
if (c == 'a') { ... } // CERTO: compara char com char
```
PORQUE: 'a' = valor 97 (char). "a" = endereco de um vetor {'a','\0'}.

## 20. Nao testar fopen == NULL
fopen retorna NULL se nao conseguiu abrir. Usar NULL da crash.

ERRADO:
```c
FILE *arq = fopen("dados.txt", "r");
fscanf(arq, "%d", &x);    // se arq==NULL -> crash
```

CERTO:
```c
FILE *arq = fopen("dados.txt", "r");
if (arq == NULL) {
    printf("erro ao abrir arquivo");
    return 1;
}
fscanf(arq, "%d", &x);
```
PORQUE: arquivo pode nao existir, sem permissao, disco cheio. Sempre testar.

## 21. Nao fechar arquivo (fclose) / modos confundidos
Esquecer fclose pode perder dados (buffer nao escrito). Confundir modos:
"w" APAGA o conteudo, "r" nao cria se nao existir, "a" adiciona no fim.

ERRADO:
```c
FILE *arq = fopen("dados.txt", "w");   // "w" APAGA tudo!
// esqueceu fclose -> pode perder dados
```

CERTO:
```c
FILE *arq = fopen("dados.txt", "a");    // "a" preserva, adiciona no fim
if (arq == NULL) { return 1; }
fprintf(arq, "novo dado\n");
fclose(arq);                            // sempre fechar
```
PORQUE: "w" trunca (apaga). "r" so le (nao cria). "a" append (preserva).
Sempre fclose para garantir que o buffer seja escrito.

=========================================================
MATRIZ E FUNCAO
=========================================================

## 22. Passar matriz para funcao sem informar colunas
Em C, ao passar matriz, o numero de COLUNAS e obrigatorio.

ERRADO:
```c
void imprimir(int m[][]) {     // ERRO: falta o numero de colunas
    ...
}
```

CERTO:
```c
#define COL 5
void imprimir(int m[][COL], int linhas) {   // colunas obrigatorias
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < COL; j++)
            printf("%d ", m[i][j]);
}
```
PORQUE: C precisa saber o tamanho de cada linha para calcular o endereco
m[i][j] = base + i*COL + j. Sem COL, nao consegue.

## 23. Recursao sem caso base - stack overflow
Recursao precisa de um caso base que para as chamadas. Sem ele, chama
infinitamente ate estourar a pilha (stack overflow / crash).

ERRADO:
```c
int soma(int n) {
    return n + soma(n - 1);   // SEM caso base -> infinito -> crash
}
```

CERTO:
```c
int soma(int n) {
    if (n <= 0) return 0;      // CASO BASE: para a recursao
    return n + soma(n - 1);   // caso recursivo
}
```
PORQUE: sem caso base, a funcao nunca para. Cada chamada ocupa pilha.

=========================================================
RECURSAO (PEGADINHA DE PROVA)
=========================================================

## 24. Recursao: printf ANTES vs DEPOIS da chamada (PEGADINHA DE PROVA)
A ORDEM do printf muda a saida. Esta e a pegadinha MAIS cobrada em recursao.

ANTES da chamada = imprime na IDA (decrescente):
```c
void rec(int n) {
    if (n == 0) return;
    printf("%d ", n);    // ANTES da chamada
    rec(n - 1);
}
// rec(5) imprime: 5 4 3 2 1  (decrescente, na ida)
```

DEPOIS da chamada = imprime na VOLTA (crescente):
```c
void rec(int n) {
    if (n == 0) return;
    rec(n - 1);          // chamada primeiro
    printf("%d ", n);    // DEPOIS da chamada
}
// rec(5) imprime: 1 2 3 4 5  (crescente, na volta)
```
PORQUE: a pilha de chamadas desce ate o caso base (n=0) e sobe de volta.
Printf antes executa na descida (5,4,3,2,1). Printf depois executa na
subida (1,2,3,4,5). QUESTAO 1 DA PROVA ERA EXATAMENTE ISSO.

## 25. Operadores: && vs & (bitwise), || vs |
&& e || sao logicos (verdadeiro/falso). & e | sao bitwise (bit a bit).

ERRADO (intencao logica):
```c
if (a & b) { ... }     // bitwise AND, nao logico
```

CERTO:
```c
if (a && b) { ... }    // logico: ambos verdadeiros
```
PORQUE: & opera bit a bit. Para logica, use && (curto-circuito).

=========================================================
OPERADORES
=========================================================

## 26. Pre-incremento vs pos-incremento (++i vs i++)
++i incrementa ANTES de usar. i++ incrementa DEPOIS de usar.

```c
int i = 5;
int a = i++;   // a=5, depois i vira 6 (pos-incremento)
int b = ++i;   // i vira 7 ANTES, b=7 (pre-incremento)
```
Em um for simples, tanto faz (i++ ou ++i). A diferenca importa ao
atribuir ou passar como argumento.

## 27. Operador ternario: cond ? a : b
Atalho para if/else. Se cond verdadeira, retorna a; senao, b.

```c
int maior = (x > y) ? x : y;   // se x>y, maior=x; senao maior=y
```
CUIDADO com precedencia: use parenteses para evitar surpresas.

## 28. Curto-circuito: && e || param cedo
&& para no primeiro falso. || para no primeiro verdadeiro. A ordem
importa para evitar erros (ex: dividir por zero).

```c
if (b != 0 && a/b > 1) { ... }   // se b==0, nao avalia a/b (curto-circuito)
// se invertesse: if (a/b > 1 && b != 0) -> dividiria por zero antes!
```
PORQUE: && so avalia o segundo se o primeiro for verdadeiro. Coloque
a condicao de seguranca (b!=0) primeiro.

## 29. Overflow de int (fatorial grande)
int guarda ate ~2 bilhoes. Fatorial de 13 ja estoura. Use long long
ou double para valores grandes.

ERRADO:
```c
int fat = 1;
for (int i = 1; i <= 20; i++) fat *= i;   // estoura int a partir de 13!
printf("%d", fat);   // valor errado/lixo
```

CERTO:
```c
long long fat = 1;                          // long long aguenta mais
for (int i = 1; i <= 20; i++) fat *= i;
printf("%lld", fat);                        // %lld para long long
```
PORQUE: int estoura em ~2.1 bilhoes. long long aguenta ~9 quintilhoes.

=========================================================
NUMEROS
=========================================================

## 30. Comparar float com == (imprecisao)
Float tem imprecisao. 0.1+0.2 pode nao ser exatamente 0.3. Comparar
com == pode falhar.

ERRADO:
```c
float x = 0.1 + 0.2;
if (x == 0.3) { ... }   // pode ser FALSO por imprecisao
```

CERTO:
```c
#include <math.h>
if (fabs(x - 0.3) < 0.0001) { ... }   // tolerancia
```
PORQUE: float nao representa todos os decimais exatamente. Use tolerancia.

## 31. Modulo com negativo
-5 % 2 pode ser -1 ou 1 dependendo do compilador (C99 padroniza -1).
Para resto sempre positivo: ((n % m) + m) % m.

```c
int r = -5 % 2;   // -1 na maioria dos compiladores (C99+)
int rPos = ((-5 % 2) + 2) % 2;   // sempre positivo
```

## 32. strlen nao conta \0, sizeof conta
strlen retorna o numero de caracteres ate o \0 (sem contar o \0).
sizeof retorna o tamanho do vetor inteiro (incluindo espaco do \0).

```c
char s[50] = "Ana";
strlen(s);   // 3 (so os caracteres)
sizeof(s);   // 50 (o vetor inteiro)
sizeof("Ana"); // 4 (3 letras + \0)
```
PORQUE: strlen para no \0. sizeof e o tamanho alocado.

## 33. strcpy/strcat sem espaco suficiente
Se o destino nao tem espaco, estoura o buffer.

ERRADO:
```c
char dest[5];
strcpy(dest, "texto muito longo");   // estoura dest (so tem 5)
```

CERTO:
```c
char dest[50];
strcpy(dest, "texto");   // cabe
// ou com limite:
strncpy(dest, "texto", 49); dest[49] = '\0';
```

=========================================================
VARIAVEIS
=========================================================

## 34. Variavel nao inicializada - lixo
Variavel local sem valor inicial tem lixo da pilha.

ERRADO:
```c
int x;          // lixo
if (x > 0) { ... }   // comportamento indefinido
```

CERTO:
```c
int x = 0;      // inicializa
```

## 35. Escopo de variavel em for (int i...)
Variavel declarada no for so existe dentro dele.

```c
for (int i = 0; i < 5; i++) { ... }
printf("%d", i);   // ERRO: i nao existe fora do for
```
Solucao: declarar i antes do for se precisar fora.

## 36. Return em void
Funcao void nao retorna valor.

ERRADO:
```c
void mostrar() {
    return 5;    // ERRO: void nao retorna valor
}
```
CERTO: return; (so para sair) ou nada.

## 37. Falta de return em funcao nao-void
Funcao que declara retorno (int, float...) precisa de return.

ERRADO:
```c
int calcular(int n) {
    if (n > 0) return n * 2;
    // se n<=0, nao tem return -> comportamento indefinido
}
```
CERTO: garantir return em todos os caminhos.

## 38. Comentario /* sem fechar
Esquecer de fechar o comentario /* faz o resto do codigo virar comentario.

ERRADO:
```c
/* comentario que esqueci de fechar
int x = 5;   // tudo vira comentario ate achar outro */
```
CUIDADO: // e comentario de uma linha so. /* */ pode ter varias linhas.

## 39. Tamanho de string na leitura (scanf %s sem limite)
scanf("%s", nome) sem limite pode estourar o buffer.

ERRADO:
```c
char nome[50];
scanf("%s", nome);    // sem limite: se digitar 100 chars, estoura
```
CERTO:
```c
scanf("%49s", nome);   // limita a 49 (deixa 1 para o \0)
```

## 40. Misturar tipos no printf
Usar formatador errado le o tamanho errado de bytes.

ERRADO:
```c
int x = 10;
printf("%f", x);     // ERRADO: %f espera double, x e int -> lixo
```
CERTO:
```c
printf("%d", x);     // int usa %d
```

=========================================================
QUESTOES DE ANALISE (estilo prova)
=========================================================

## 41. Analise: qual a saida?
```c
int x = 5;
if (x = 10)
    printf("%d", x);
```
RESPOSTA: imprime "10". PORQUE: x=10 atribui 10 (verdadeiro), entra no if,
e x agora vale 10. Pegadinha do = vs ==.

## 42. Analise: o que acontece?
```c
char senha[20];
scanf("%s", senha);
if (strcmp(senha, "1234"))
    printf("acesso permitido");
else
    printf("acesso negado");
```
RESPOSTA: se digitar "1234", imprime "acesso negado". Se digitar outra
coisa, imprime "acesso permitido". LOGICA INVERTIDA. PORQUE: strcmp
retorna 0 (falso) quando iguais. if(strcmp) e verdadeiro quando DIFEREM.

## 43. Analise: qual a saida da recursao?
```c
void f(int n) {
    if (n == 0) return;
    printf("%d ", n);
    f(n - 1);
}
// chamada: f(4)
```
RESPOSTA: imprime "4 3 2 1". PORQUE: printf ANTES da chamada = imprime
na descida (decrescente).

## 44. Analise: e se inverter?
```c
void f(int n) {
    if (n == 0) return;
    f(n - 1);
    printf("%d ", n);
}
// chamada: f(4)
```
RESPOSTA: imprime "1 2 3 4". PORQUE: printf DEPOIS da chamada = imprime
na subida (crescente).

## 45. Analise: erro no loop de arquivo?
```c
fread(&r, sizeof(r), 1, arq);
while (!feof(arq)) {
    printf("%d", r.cod);
    fread(&r, sizeof(r), 1, arq);
}
```
RESPOSTA: o ultimo registro e impresso DUAS VEZES (ou imprime lixo no
fim). PORQUE: feof so vira true depois de uma leitura falhar. O fread
antes do loop le o primeiro, e o loop so detecta o fim uma iteracao
atrasada. CORRETO: while(fread(&r,sizeof(r),1,arq)==1).

## 46. Analise: divisao inteira?
```c
int a = 7, b = 2;
float r = a / b;
printf("%.2f", r);
```
RESPOSTA: imprime "3.00". PORQUE: 7/2 e divisao inteira = 3 (truncado),
depois convertido para float 3.0. CORRETO: (float)a/b ou a/2.0 -> 3.50.

=========================================================
RESUMO RAPIDO DAS 10 PEGADINHAS MAIS COBRADAS
=========================================================
1. = vs == (if(x=5) sempre verdadeiro)
2. if(strcmp(a,b)) verdadeiro quando DIFEREM (use ==0 para iguais)
3. Recursao: printf antes=decrescente, depois=crescente
4. while(!feof) com fread antes le ultimo 2x (use fread==1)
5. Divisao inteira 5/2=2 (use 5.0/2 ou (float))
6. Esquecer & no scanf de int/float
7. Comparar string com == (use strcmp)
8. Esquecer break no switch (fall through)
9. Indice vetor[5] em tamanho 5 (vai 0 a 4)
10. typedef struct sem alias nao compila em C puro

=========================================================
CHECKLIST ANTI-PEGADINHAS (antes de entregar)
=========================================================
[ ] Usei == para comparar (nao =)?
[ ] if(strcmp) esta com ==0 para igualdade?
[ ] Recursao tem caso base?
[ ] Printf da recursao esta na ordem certa (antes=desc, depois=cresc)?
[ ] Loop de arquivo usa fread==1 / fgets!=NULL (nao feof)?
[ ] Divisao tem cast ou .0 se precisa de decimal?
[ ] scanf tem & em int/float/double/char?
[ ] String comparada com strcmp (nao ==)?
[ ] Switch tem break em cada case?
[ ] Indices do vetor vao de 0 a tamanho-1?
[ ] typedef struct tem alias no fim?
[ ] Struct ponteiro usa -> (nao ponto)?
[ ] fopen testado contra NULL?
[ ] Arquivo fechado com fclose?
[ ] Soma/contador inicializados em 0?
[ ] Divisao por zero testada (contador>0)?


#############################################################
# PARTE 3 - CASOS EXTREMOS E TRATAMENTO DE ERROS
#############################################################

# CASOS EXTREMOS E TRATAMENTO DE ERROS EM C - MATERIAL DE CONSULTA GB

================================================================
PALAVRAS-CHAVE (Ctrl-F para achar rapido)
================================================================
validacao entrada | repetir ate valido | nota 0-10 | idade positiva
opcao menu | CPF so digitos | email com @ | divisao por zero | divisor != 0
fopen == NULL | malloc == NULL | fim de arquivo | fread == 1 | fgets != NULL
fscanf == n | vetor vazio | n == 0 | contador == 0 | media | string vazia
strlen == 0 | indice fora do limite | overflow | fatorial | estoura int
imprecisao float | fabs | limpar buffer | getchar | entrada nao numerica
scanf retorno | nome longo | %49s | arquivo inexistente | arquivo vazio
codigo duplicado | codigo nao encontrado | opcao invalida | default switch
caractere invalido | funcao defensiva | ponteiro NULL | tamanho negativo
assert | assert.h | perror | strerror | codigo de retorno | return 0 ok
return 1 erro | try-catch simulado | goto | codigos de erro
================================================================

0. INTRODUCAO - POR QUE TRATAR ERROS?
================================================================

Codigo mediano: funciona quando o usuario digita certo, quebra quando digita errado.
Codigo robusto: funciona SEMPRE, mesmo com entrada absurda, arquivo faltando,
memoria cheia, etc. Provas de Algoritmos II dao PONTO EXTRA por tratar casos
limite. Em codigo real (producao), tratar erros evita crash, vazamento de dados,
perda de dinheiro e processos judiciais.

Regra de ouro: NUNCA confie no usuario. NUNCA confie no arquivo. NUNCA confie
na memoria. SEMPRE verifique antes de usar.

Padrao do professor (estilo conio.h + system + fflush(stdin) com comentario):

    #include <stdio.h>
    #include <conio.h>   // getch, clrscr (estilo do professor)
    #include <stdlib.h>  // system, malloc, exit
    #include <string.h>  // strlen, strcmp, strcpy
    #include <ctype.h>   // isdigit, isalpha, toupper
    #include <math.h>    // fabs
    #include <assert.h>  // assert
    #include <errno.h>   // errno, perror, strerror

    int main(void) {
        int op;
        do {
            // fflush(stdin); // limpa buffer do teclado (estilo professor)
            printf("1-Cadastrar 2-Listar 0-Sair: ");
            scanf("%d", &op);
        } while (op < 0 || op > 2);
        system("pause"); // pausa tela no Windows (estilo professor)
        return 0;
    }

Cada secao abaixo mostra o codigo FRAGIL (sem tratamento) e o ROBUSTO (com
tratamento), lado a lado, com explicacao do porquê.


1. VALIDACAO DE ENTRADA - REPETIR ATE VALIDO
================================================================

1.1 NOTA ENTRE 0 E 10
----------------------------------------------------------------

FRAGIL:
    float nota;
    scanf("%f", &nota);  // usuario digita -5 ou 15? aceita sem reclamar
    printf("Nota: %.1f\n", nota);

ROBUSTO:
    float nota;
    do {
        // fflush(stdin); // limpa buffer do teclado (estilo professor)
        printf("Digite a nota (0 a 10): ");
        scanf("%f", &nota);
        if (nota < 0 || nota > 10) {
            printf("ERRO: nota invalida. Use valor entre 0 e 10.\n");
        }
    } while (nota < 0 || nota > 10);
    printf("Nota valida: %.1f\n", nota);

Explicacao: o do-while repete ENQUANTO a nota for invalida. So sai quando o
usuario digita um valor no intervalo correto. E o if interno avisa o motivo
do erro, em vez de ficar repetindo a pergunta sem explicar.

1.2 IDADE POSITIVA
----------------------------------------------------------------

FRAGIL:
    int idade;
    scanf("%d", &idade);  // aceita -200 ou 9999

ROBUSTO:
    int idade;
    do {
        printf("Digite a idade: ");
        scanf("%d", &idade);
        if (idade < 0) {
            printf("ERRO: idade nao pode ser negativa.\n");
        } else if (idade > 150) {
            printf("ERRO: idade improvavel. Confira o valor.\n");
        }
    } while (idade < 0 || idade > 150);

Explicacao: testa dois limites. Negativo e absurdo (>150). Em cadastro real,
defina um teto razoavel (ex.: 120). Repete ate digitar algo plausivel.

1.3 OPCAO DE MENU
----------------------------------------------------------------

FRAGIL:
    int op;
    scanf("%d", &op);
    switch (op) {  // se digitar 9, nenhum case executa, fica sem fazer nada
        case 1: cadastrar(); break;
        case 2: listar(); break;
    }

ROBUSTO:
    int op;
    do {
        printf("1-Cadastrar 2-Listar 0-Sair\nOpcao: ");
        scanf("%d", &op);
        if (op < 0 || op > 2) {
            printf("ERRO: opcao invalida. Escolha 0, 1 ou 2.\n");
        }
    } while (op < 0 || op > 2);
    switch (op) {
        case 1: cadastrar(); break;
        case 2: listar();    break;
        case 0: printf("Saindo...\n"); break;
    }

Explicacao: valida ANTES do switch. Assim o switch so recebe opcao valida e
nao precisa de default (mas e bom ter default mesmo assim - veja secao 19).

1.4 CPF SO COM DIGITOS
----------------------------------------------------------------

FRAGIL:
    char cpf[15];
    scanf("%s", cpf);  // aceita "abc", "123", "joao da silva"

ROBUSTO:
    char cpf[15];
    int ok, i;
    do {
        printf("Digite o CPF (11 digitos, sem pontos): ");
        scanf("%14s", cpf);
        ok = 1;
        if (strlen(cpf) != 11) {
            ok = 0;
            printf("ERRO: CPF deve ter 11 digitos.\n");
        } else {
            for (i = 0; i < 11; i++) {
                if (!isdigit(cpf[i])) {  // ctype.h
                    ok = 0;
                    printf("ERRO: CPF so pode ter digitos.\n");
                    break;
                }
            }
        }
    } while (!ok);

Explicacao: duas checagens - tamanho 11 e todos os caracteres sao digitos
(isdigit do ctype.h). O break interrompe o loop no primeiro caractere invalido.
Nao valida os digitos verificadores (isso e outra historia), mas ja filtra
entrada absurda.

1.5 EMAIL COM @
----------------------------------------------------------------

FRAGIL:
    char email[50];
    scanf("%s", email);  // aceita "nao tem arroba"

ROBUSTO:
    char email[50];
    do {
        printf("Digite o email: ");
        scanf("%49s", email);
        if (strchr(email, '@') == NULL) {  // strchr procura caractere
            printf("ERRO: email deve conter @.\n");
        }
    } while (strchr(email, '@') == NULL);

Explicacao: strchr (string.h) retorna ponteiro para a primeira ocorrencia de
'@'. Se nao achar, retorna NULL. Validacao simples mas filtra o caso mais
comum de email digitado errado. Validacao completa de email e complexa (ponto,
dominio, etc.), mas para prova o @ ja conta ponto.


2. TRATAR DIVISAO POR ZERO
================================================================

FRAGIL:
    int a, b;
    scanf("%d %d", &a, &b);
    printf("Resultado: %d\n", a / b);  // se b == 0, CRASH (divisao por zero)

ROBUSTO:
    int a, b;
    scanf("%d %d", &a, &b);
    if (b != 0) {
        printf("Resultado: %d\n", a / b);
    } else {
        printf("ERRO: divisao por zero nao permitida.\n");
    }

Versao com do-while (repete ate digitar divisor valido):
    int a, b;
    scanf("%d", &a);
    do {
        printf("Divisor: ");
        scanf("%d", &b);
        if (b == 0) printf("ERRO: divisor nao pode ser zero.\n");
    } while (b == 0);
    printf("Resultado: %d\n", a / b);

Explicacao: divisao por zero em inteiros causa encerramento anormal do
programa (excecao de ponto flutuante no Linux, crash no Windows). SEMPRE teste
o divisor antes de dividir. Em float, divisao por zero nao crasha mas gera
inf/nan - tambem deve ser tratado.


3. TRATAR fopen == NULL
================================================================

FRAGIL:
    FILE *f = fopen("dados.txt", "r");
    fscanf(f, "%d", &x);  // se f == NULL, fscanf da crash/segfault

ROBUSTO:
    FILE *f = fopen("dados.txt", "r");
    if (f == NULL) {
        printf("ERRO: nao consegui abrir dados.txt\n");
        // perror("dados.txt"); // alternativa: mostra mensagem do sistema
        return 1;  // sai com codigo de erro
    }
    fscanf(f, "%d", &x);
    fclose(f);

Explicacao: fopen retorna NULL quando o arquivo nao existe, nao tem permissao,
caminho errado, disco cheio (modo escrita), etc. SEMPRE teste f == NULL antes
de usar o ponteiro. Use perror (secao 23) para mostrar a causa real do erro.


4. TRATAR malloc == NULL
================================================================

FRAGIL:
    int *v = malloc(1000 * sizeof(int));
    v[0] = 42;  // se malloc falhou (NULL), v[0] da segfault

ROBUSTO:
    int *v = malloc(1000 * sizeof(int));
    if (v == NULL) {
        printf("ERRO: memoria insuficiente.\n");
        return 1;
    }
    v[0] = 42;
    free(v);  // nao esqueca de liberar

Versao com saida imediata:
    int *v = malloc(n * sizeof(int));
    if (v == NULL) {
        fprintf(stderr, "Sem memoria para %d inteiros.\n", n);
        exit(1);  // stdlib.h
    }

Explicacao: malloc retorna NULL quando nao ha memoria suficiente. Em aula
raramente falha, mas em codigo real (alocacoes grandes, sistemas rodando
ha dias) e comum. SEMPRE verifique. E nao esqueca do free no fim para evitar
vazamento de memoria.


5. TRATAR FIM DE ARQUIVO (EOF)
================================================================

5.1 fread - VERIFICAR RETORNO == 1
----------------------------------------------------------------

FRAGIL:
    FILE *f = fopen("dados.bin", "rb");
    struct Pessoa p;
    while (!feof(f)) {        // feof nao e confiavel assim
        fread(&p, sizeof(p), 1, f);
        printf("%s\n", p.nome);  // imprime o ultimo duas vezes
    }

ROBUSTO:
    FILE *f = fopen("dados.bin", "rb");
    if (f == NULL) return 1;
    struct Pessoa p;
    while (fread(&p, sizeof(p), 1, f) == 1) {  // leu 1 bloco com sucesso
        printf("%s\n", p.nome);
    }

Explicacao: fread retorna o NUMERO DE BLOCOS lidos. Se pediu 1 bloco e leu 1,
retorna 1. Se chegou no fim (ou deu erro), retorna 0. Testar fread == 1 e o
padrao correto - leu, processa; nao leu, sai. Nao use feof no while, ele so
vira verdadeiro DEPOIS de uma leitura falha, causando o bug do "ultimo
registro impresso duas vezes".

5.2 fgets - VERIFICAR != NULL
----------------------------------------------------------------

FRAGIL:
    FILE *f = fopen("texto.txt", "r");
    char linha[100];
    while (!feof(f)) {
        fgets(linha, 100, f);
        printf("%s", linha);  // ultima linha pode repetir
    }

ROBUSTO:
    FILE *f = fopen("texto.txt", "r");
    if (f == NULL) return 1;
    char linha[100];
    while (fgets(linha, 100, f) != NULL) {
        printf("%s", linha);
    }

Explicacao: fgets retorna NULL quando nao consegue ler (fim de arquivo ou
erro). Testar != NULL e o padrao correto. fgets ja inclui o \n no final da
string (se couber), entao printf com %s sem \n adicional mantem o formato.

5.3 fscanf - VERIFICAR == n (numero de campos esperados)
----------------------------------------------------------------

FRAGIL:
    FILE *f = fopen("notas.txt", "r");
    char nome[50];
    float nota;
    while (!feof(f)) {
        fscanf(f, "%s %f", nome, &nota);
        printf("%s: %.1f\n", nome, nota);  // ultima linha repete
    }

ROBUSTO:
    FILE *f = fopen("notas.txt", "r");
    if (f == NULL) return 1;
    char nome[50];
    float nota;
    while (fscanf(f, "%49s %f", nome, &nota) == 2) {  // leu os 2 campos
        printf("%s: %.1f\n", nome, nota);
    }

Explicacao: fscanf retorna o numero de CAMPOS convertidos com sucesso. Se
pediu 2 (nome e nota) e leu 2, retorna 2. Se chegou no fim ou deu erro de
conversao, retorna EOF ou menos que 2. Testar == 2 garante que so processa
quando os dois campos vieram corretos.


6. TRATAR VETOR VAZIO (n == 0)
================================================================

FRAGIL:
    int v[100], n;
    scanf("%d", &n);
    printf("Primeiro: %d\n", v[0]);  // se n == 0, v[0] nao existe (lixo)

ROBUSTO:
    int v[100], n;
    scanf("%d", &n);
    if (n == 0) {
        printf("Vetor vazio, nao ha primeiro elemento.\n");
    } else {
        printf("Primeiro: %d\n", v[0]);
    }

Versao em funcao que retorna indice do maior:
    int maior_indice(int v[], int n) {
        if (n == 0) return -1;  // sinaliza vetor vazio
        int im = 0;
        for (int i = 1; i < n; i++)
            if (v[i] > v[im]) im = i;
        return im;
    }

Explicacao: acessar v[0] de um vetor com 0 elementos e acesso fora dos limites
(comportamento indefinido em C). Sempre verifique n > 0 antes de acessar
qualquer posicao. Em funcoes, retorne um valor sentinela (-1) ou codigo de
erro para indicar que nao ha o que calcular.


7. TRATAR CONTADOR == 0 ANTES DE DIVIDIR (MEDIA)
================================================================

FRAGIL:
    int soma = 0, cont = 0, x;
    while (scanf("%d", &x) == 1 && x != 0) {
        soma += x;
        cont++;
    }
    printf("Media: %.2f\n", (float)soma / cont);  // se cont == 0, divisao por zero

ROBUSTO:
    int soma = 0, cont = 0, x;
    while (scanf("%d", &x) == 1 && x != 0) {
        soma += x;
        cont++;
    }
    if (cont == 0) {
        printf("Nenhum valor digitado, nao ha media.\n");
    } else {
        printf("Media: %.2f\n", (float)soma / cont);
    }

Explicacao: se o usuario digita 0 logo de cara (ou arquivo vazio), cont fica 0
e a divisao soma/cont e divisao por zero. Em int crasha, em float gera nan.
Sempre verifique cont > 0 antes de dividir. E o mesmo padrao da secao 2,
aplicado a medias e porcentagens.


8. TRATAR STRING VAZIA (strlen == 0)
================================================================

FRAGIL:
    char nome[50];
    scanf("%s", nome);
    printf("Inicial: %c\n", nome[0]);  // se string vazia, nome[0] e '\0' ou lixo

ROBUSTO:
    char nome[50];
    scanf("%49s", nome);
    if (strlen(nome) == 0) {
        printf("Nome vazio, nada a fazer.\n");
    } else {
        printf("Inicial: %c\n", nome[0]);
        printf("Tamanho: %zu\n", strlen(nome));  // %zu para size_t
    }

Versao com fgets (pode gerar string so com \n):
    char nome[50];
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = '\0';  // remove \n do fim
    if (strlen(nome) == 0) {
        printf("Voce so apertou Enter.\n");
    }

Explicacao: string vazia e nome[0] == '\0', ou strlen == 0. Acessar nome[0]
nesse caso nao crasha (e o proprio '\0'), mas qualquer logica que assume que
ha conteudo (primeira letra, maiuscula, etc.) vai dar errado. Sempre verifique
o tamanho antes. Com fgets, o usuario pode so apertar Enter - remova o \n e
teste se sobrou algo.


9. TRATAR INDICE FORA DO LIMITE
================================================================

FRAGIL:
    int v[10];
    int i;
    scanf("%d", &i);
    printf("%d\n", v[i]);  // i = -1 ou i = 20? acesso invalido, lixo ou crash

ROBUSTO:
    int v[10];
    int i;
    scanf("%d", &i);
    if (i < 0 || i >= 10) {
        printf("ERRO: indice %d fora do intervalo [0, 9].\n", i);
    } else {
        printf("%d\n", v[i]);
    }

Versao em funcao de busca:
    int busca(int v[], int n, int chave) {
        if (n <= 0) return -1;  // vetor vazio
        for (int i = 0; i < n; i++) {
            if (v[i] == chave) return i;
        }
        return -1;  // nao achou
    }

Explicacao: C NAO verifica limites de vetor. v[20] num vetor de 10 nao da erro
de compilacao nem de execucao - simplesmente le/escreve memoria adjacente
(buffer overflow), causando bug silencioso ou falha de seguranca. SEMPRE valide
o indice: i >= 0 && i < n antes de acessar v[i].


10. TRATAR OVERFLOW (ESTOURO DE INT)
================================================================

10.1 FATORIAL GRANDE
----------------------------------------------------------------

FRAGIL:
    int fat = 1, n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) fat *= i;
    printf("%d\n", fat);  // 13! ja estoura int (da numero negativo ou errado)

ROBUSTO:
    int n;
    long long fat = 1;  // long long aguenta ate 20!
    scanf("%d", &n);
    if (n < 0) {
        printf("ERRO: fatorial de negativo nao existe.\n");
    } else if (n > 20) {
        printf("ERRO: %d! estoura long long. Use valor ate 20.\n", n);
    } else {
        for (int i = 1; i <= n; i++) fat *= i;
        printf("%lld\n", fat);  // %lld para long long
    }

Explicacao: int aguenta ate 2.147.483.647 (~2 bilhoes). 12! = 479.001.600
(cabe), 13! = 6.227.020.800 (NAO cabe, estoura). long long aguenta ate 20!
(2.432.902.008.176.640.000). Acima de 20! precisa de biblioteca de numeros
grandes (bigint). Sempre defina um teto e avise o usuario. Overflow em int
com sinal e comportamento indefinido em C - pode dar negativo, zero, ou
qualquer coisa.

10.2 SOMA QUE ESTOURA INT
----------------------------------------------------------------

FRAGIL:
    int a, b;
    scanf("%d %d", &a, &b);
    printf("Soma: %d\n", a + b);  // 2.000.000.000 + 2.000.000.000 estoura

ROBUSTO:
    int a, b;
    scanf("%d %d", &a, &b);
    long long soma = (long long)a + (long long)b;  // converte antes de somar
    printf("Soma: %lld\n", soma);

Versao com verificacao explicita:
    int a, b;
    scanf("%d %d", &a, &b);
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        printf("ERRO: soma estoura int.\n");
    } else {
        printf("Soma: %d\n", a + b);
    }

Explicacao: somar dois int grandes pode estourar mesmo que cada um caiba.
Converter para long long ANTES da soma evita o estouro. Ou verifique
explicitamente com INT_MAX/INT_MIN (limits.h). Em prova, usar long long e o
jeito mais simples e seguro.


11. TRATAR IMPRECISAO DE FLOAT
================================================================

FRAGIL:
    float a = 0.1 + 0.2;
    if (a == 0.3) {  // FALSO! a vale 0.30000001 por imprecisao
        printf("igual\n");
    }

ROBUSTO:
    #include <math.h>
    float a = 0.1f + 0.2f;
    if (fabs(a - 0.3f) < 0.0001f) {  // diferenca menor que tolerancia
        printf("igual (dentro da tolerancia)\n");
    }

Explicacao: float nao consegue representar exatamente 0.1, 0.2, 0.3 (sao
dizimas em binario). 0.1 + 0.2 da 0.30000001192092896, nao 0.3. Comparar
floats com == quase sempre falha. Use fabs (valor absoluto da diferenca) e
compare com uma tolerancia pequena (epsilon, ex.: 0.0001). Para double,
use 1e-9. NUNCA compare floats com ==.


12. LIMPAR BUFFER CORRETAMENTE (LOOP GETCHAR)
================================================================

FRAGIL:
    int x;
    char c;
    scanf("%d", &x);
    scanf("%c", &c);  // le o \n que sobrou do scanf anterior, nao o caractere

ROBUSTO (loop com getchar):
    int x;
    char c;
    scanf("%d", &x);
    while (getchar() != '\n');  // descarta tudo ate o Enter (limpa buffer)
    scanf("%c", &c);            // agora le o caractere certo

Versao com fflush(stdin) (estilo professor, funciona no Windows com gcc):
    int x;
    char c;
    scanf("%d", &x);
    // fflush(stdin); // limpa buffer do teclado (estilo professor, nao-portatil)
    scanf("%c", &c);

Versao com funcao reutilizavel:
    void limpar_buffer(void) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);  // descarta ate \n ou EOF
    }

    int main(void) {
        int x;
        char c;
        scanf("%d", &x);
        limpar_buffer();
        scanf("%c", &c);
        return 0;
    }

Explicacao: scanf de numero deixa o \n (Enter) no buffer. O proximo scanf de
char le esse \n em vez do caractere que o usuario quer digitar. O loop
while (getchar() != '\n'); descarta todos os caracteres ate (e incluindo) o
\n. fflush(stdin) funciona no Windows com gcc mas NAO e portavel (nao
funciona no Linux) - o padrao C diz que fflush e para saida, nao entrada. Em
prova, o loop getchar e o correto; o professor aceita fflush(stdin) com
comentario explicando.


13. TRATAR ENTRADA NAO-NUMERICA NO SCANF
================================================================

FRAGIL:
    int x;
    scanf("%d", &x);  // usuario digita "abc"? scanf falha, x fica com lixo
    printf("Voce digitou %d\n", x);

ROBUSTO:
    int x, ret;
    do {
        printf("Digite um numero: ");
        ret = scanf("%d", &x);
        if (ret != 1) {
            printf("ERRO: nao e numero. Tente novamente.\n");
            while (getchar() != '\n');  // limpa o que sobrou ("abc")
        }
    } while (ret != 1);
    printf("Voce digitou %d\n", x);

Explicacao: scanf retorna o numero de CAMPOS convertidos com sucesso. Se
pediu 1 int e o usuario digitou "abc", scanf retorna 0 (nao converteu nada) e
deixa "abc" no buffer. Se nao limpar, o proximo scanf tambem falha (loop
infinito!). Por isso: verifique o retorno E limpe o buffer quando falhar. Esse
e um dos erros mais comuns em prova - sempre verifique o retorno do scanf.


14. TRATAR NOME MUITO LONGO (%49s)
================================================================

FRAGIL:
    char nome[50];
    scanf("%s", nome);  // usuario digita 100 chars? estoura o vetor (overflow)

ROBUSTO:
    char nome[50];
    scanf("%49s", nome);  // le no max 49 chars (+ \0), ignora o resto
    // ou:
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = '\0';  // remove \n

Versao com aviso se truncou:
    char nome[50];
    char temp[200];
    scanf("%199s", temp);
    if (strlen(temp) > 49) {
        printf("AVISO: nome muito longo, truncado para 49 caracteres.\n");
        strncpy(nome, temp, 49);
        nome[49] = '\0';
    } else {
        strcpy(nome, temp);
    }

Explicacao: scanf sem limite de tamanho e vulnerabilidade classica (buffer
overflow). Se o vetor tem 50 bytes e o usuario digita 100, scanf escreve 100
bytes e estoura a memoria adjacente. Use %49s (deixa 1 byte para o \0) ou
fgets com tamanho. Em codigo serio, sempre delimite o tamanho da leitura.


15. TRATAR ARQUIVO INEXISTENTE (MODO R FALHA)
================================================================

FRAGIL:
    FILE *f = fopen("config.txt", "r");
    fscanf(f, "%d", &x);  // arquivo nao existe -> f == NULL -> crash

ROBUSTO:
    FILE *f = fopen("config.txt", "r");
    if (f == NULL) {
        printf("ERRO: arquivo config.txt nao encontrado.\n");
        printf("Crie o arquivo ou verifique o caminho.\n");
        // perror("config.txt"); // mostra: "config.txt: No such file or directory"
        return 1;
    }
    fscanf(f, "%d", &x);
    fclose(f);

Versao com criacao automatica se nao existir:
    FILE *f = fopen("config.txt", "r");
    if (f == NULL) {
        printf("Arquivo nao existe. Criando novo...\n");
        f = fopen("config.txt", "w");
        if (f == NULL) {
            printf("ERRO: nao consegui criar o arquivo.\n");
            return 1;
        }
        fprintf(f, "0\n");  // valor padrao
        fclose(f);
        f = fopen("config.txt", "r");
    }
    fscanf(f, "%d", &x);
    fclose(f);

Explicacao: fopen no modo "r" (leitura) FALHA se o arquivo nao existe -
retorna NULL. Sempre verifique. Se o arquivo e essencial, avise e saia. Se e
opcional, pode criar com "w" ou "a". perror mostra a mensagem do sistema
(arquivo inexistente, permissao negada, etc.) - veja secao 23.


16. TRATAR ARQUIVO VAZIO (SEM REGISTROS)
================================================================

FRAGIL:
    FILE *f = fopen("dados.txt", "r");
    char linha[100];
    while (fgets(linha, 100, f) != NULL) {
        printf("%s", linha);
    }
    fclose(f);
    // se arquivo vazio, nao imprime nada e o usuario fica sem saber se
    // rodou ou se deu erro

ROBUSTO:
    FILE *f = fopen("dados.txt", "r");
    if (f == NULL) {
        printf("ERRO: nao abriu o arquivo.\n");
        return 1;
    }
    char linha[100];
    int cont = 0;
    while (fgets(linha, 100, f) != NULL) {
        printf("%s", linha);
        cont++;
    }
    if (cont == 0) {
        printf("Arquivo vazio: nenhum registro encontrado.\n");
    } else {
        printf("--- %d registro(s) lido(s) ---\n", cont);
    }
    fclose(f);

Explicacao: arquivo vazio (0 bytes) nao e erro de fopen (abre normal), mas
nao tem nada para ler. O while simplesmente nao executa. Contar os registros
lidos (cont) permite avisar o usuario que o arquivo estava vazio, em vez de
ficar em silencio. Bom para listagens, relatorios, importacoes.


17. TRATAR CODIGO DUPLICADO EM CADASTRO
================================================================

FRAGIL:
    void cadastrar(struct Produto p[], int *n, int cod, float preco) {
        p[*n].codigo = cod;     // nao verifica se cod ja existe
        p[*n].preco = preco;
        (*n)++;
    }

ROBUSTO:
    int existe_codigo(struct Produto p[], int n, int cod) {
        for (int i = 0; i < n; i++) {
            if (p[i].codigo == cod) return 1;  // ja existe
        }
        return 0;  // nao existe
    }

    int cadastrar(struct Produto p[], int *n, int cod, float preco) {
        if (existe_codigo(p, *n, cod)) {
            printf("ERRO: codigo %d ja cadastrado.\n", cod);
            return 0;  // falhou
        }
        if (*n >= 100) {
            printf("ERRO: vetor cheio (max 100).\n");
            return 0;
        }
        p[*n].codigo = cod;
        p[*n].preco = preco;
        (*n)++;
        return 1;  // sucesso
    }

Explicacao: antes de cadastrar, percorra o vetor procurando o codigo. Se
achar, recuse e avise. Tambem verifique se o vetor nao esta cheio (limite
maximo). Retorne 1 (sucesso) ou 0 (falha) para o chamador saber se cadastrou.
Em arquivo, leia todo o arquivo procurando o codigo antes de gravar.


18. TRATAR CODIGO NAO ENCONTRADO EM BUSCA
================================================================

FRAGIL:
    int buscar(struct Produto p[], int n, int cod) {
        for (int i = 0; i < n; i++) {
            if (p[i].codigo == cod) return i;
        }
        // se nao achar, retorna lixo (sem return explicito)
    }

ROBUSTO:
    int buscar(struct Produto p[], int n, int cod) {
        for (int i = 0; i < n; i++) {
            if (p[i].codigo == cod) return i;  // achou, retorna indice
        }
        return -1;  // nao achou, retorna -1 (sentinela)
    }

    int main(void) {
        int pos = buscar(p, n, cod);
        if (pos == -1) {
            printf("Codigo %d nao encontrado.\n", cod);
        } else {
            printf("Achou: %s - R$ %.2f\n", p[pos].nome, p[pos].preco);
        }
        return 0;
    }

Explicacao: funcao de busca deve retornar -1 (ou outro valor invalido) quando
nao acha. NUNCA deixe a funcao sem return no fim - em C isso e comportamento
indefinido (retorna lixo da pilha). O chamador deve SEMPRE verificar se o
retorno e -1 antes de usar o indice. -1 e convencao universal para "nao
achou".


19. TRATAR OPCAO INVALIDA DE MENU (DEFAULT SWITCH)
================================================================

FRAGIL:
    switch (op) {
        case 1: cadastrar(); break;
        case 2: listar(); break;
        case 3: buscar(); break;
        // se op == 9, nao faz nada e nao avisa
    }

ROBUSTO:
    switch (op) {
        case 1: cadastrar(); break;
        case 2: listar();    break;
        case 3: buscar();    break;
        case 0: printf("Saindo...\n"); break;
        default:
            printf("ERRO: opcao %d invalida. Use 0 a 3.\n", op);
            break;
    }

Explicacao: o case default pega TODAS as opcoes que nao tem case especifico.
Sempre coloque default no switch para tratar o inesperado. Mesmo que voce
valide o menu antes (secao 1.3), o default e defesa em profundidade - se um
dia alguem chamar a funcao sem validar, o default evita silencio. E sempre
coloque break no default (mesmo sendo o ultimo, e bom habito).


20. TRATAR CARACTERE INVALIDO (NAO LETRA, NAO DIGITO)
================================================================

FRAGIL:
    char c;
    scanf("%c", &c);
    if (c >= 'a' && c <= 'z') {  // esquece maiusculas e acentuadas
        printf("letra minuscula\n");
    }

ROBUSTO:
    char c;
    while (getchar() != '\n');  // limpa buffer
    scanf("%c", &c);
    if (isalpha(c)) {            // ctype.h - letra (mai ou min)
        if (isupper(c)) {
            printf("Letra maiuscula: %c\n", c);
        } else {
            printf("Letra minuscula: %c\n", c);
        }
    } else if (isdigit(c)) {     // ctype.h - digito 0-9
        printf("Digito: %c\n", c);
    } else {
        printf("Caractere especial: %c (codigo %d)\n", c, c);
    }

Versao: so aceita S ou N (sim/nao), ignorando maiuscula/minuscula:
    char c;
    do {
        printf("Confirma (S/N): ");
        while (getchar() != '\n');
        scanf("%c", &c);
        c = toupper(c);  // converte para maiuscula
        if (c != 'S' && c != 'N') {
            printf("ERRO: digite S ou N.\n");
        }
    } while (c != 'S' && c != 'N');

Explicacao: ctype.h tem isalpha (letra), isdigit (digito), isupper/islower
(mai/min), toupper/tolower (converte). Use essas funcoes em vez de comparar
manualmente com 'a'..'z' (nao pega maiusculas nem acentos). Para confirmacoes
S/N, converta com toupper e compare so com 'S' e 'N' - aceita 's', 'S', 'n',
'N'.


21. FUNCOES DEFENSIVAS - VERIFICAR PARAMETROS
================================================================

FRAGIL:
    int soma_vetor(int v[], int n) {
        int s = 0;
        for (int i = 0; i < n; i++) s += v[i];
        return s;  // se v == NULL ou n < 0, comportamento indefinido
    }

ROBUSTO:
    int soma_vetor(int v[], int n) {
        if (v == NULL) {
            printf("ERRO: vetor nulo.\n");
            return 0;
        }
        if (n < 0) {
            printf("ERRO: tamanho negativo (%d).\n", n);
            return 0;
        }
        int s = 0;
        for (int i = 0; i < n; i++) s += v[i];
        return s;
    }

Versao com ponteiro para string:
    int conta_vogais(char *s) {
        if (s == NULL) return 0;  // ponteiro nulo
        int cont = 0;
        for (int i = 0; s[i] != '\0'; i++) {
            char c = tolower(s[i]);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                cont++;
            }
        }
        return cont;
    }

Versao com ponteiro para struct:
    void imprime_pessoa(struct Pessoa *p) {
        if (p == NULL) {
            printf("Pessoa nula.\n");
            return;
        }
        printf("%s, %d anos\n", p->nome, p->idade);
    }

Explicacao: funcao defensiva verifica os PARAMETROS antes de usar. Se recebeu
ponteiro NULL, tamanho negativo, indice invalido - avisa e retorna valor
seguro (0, -1, etc.). Isso evita crash e facilita achar o bug. Em codigo
critico (bibliotecas, sistemas reais), toda funcao que recebe ponteiro deve
verificar NULL. Custo: alguns ifs. Beneficio: nunca crasha por parametro
errado.


22. ASSERT() PARA DEPURACAO
================================================================

    #include <assert.h>

    int divide(int a, int b) {
        assert(b != 0);  // se b == 0, aborta o programa com mensagem
        return a / b;
    }

    int main(void) {
        int v[10];
        int i = 15;
        assert(i >= 0 && i < 10);  // protege acesso ao vetor
        printf("%d\n", v[i]);

        int *p = malloc(100 * sizeof(int));
        assert(p != NULL);  // protege malloc
        free(p);
        return 0;
    }

Como desativar em producao:
    // compile com: gcc -DNDEBUG programa.c
    // NDEBUG desativa todos os asserts (viram no-op)

ROBUSTO (assert + tratamento real):
    int divide(int a, int b) {
        assert(b != 0);  // pega bug do programador em desenvolvimento
        if (b == 0) {     // trata em producao (assert desligado)
            printf("ERRO: divisao por zero.\n");
            return 0;
        }
        return a / b;
    }

Explicacao: assert(expr) aborta o programa se expr for falsa, mostrando o
arquivo e a linha. E para DEPURACAO - pega bugs do programador (pre-condicoes
que voce acredita serem verdadeiras). Em producao, compile com -DNDEBUG para
desligar todos os asserts (viram nada). NAO use assert para tratar entrada do
usuario - use if e mensagem. Use assert para invariantes do codigo (ex.:
"acho que b nao e zero aqui"). Combina com if: assert pega bug seu, if trata
caso real.


23. PERROR E STRERROR PARA ERROS DO SISTEMA
================================================================

    #include <errno.h>   // errno
    #include <string.h>  // strerror

    FILE *f = fopen("nao_existe.txt", "r");
    if (f == NULL) {
        // perror imprime: "nao_existe.txt: No such file or directory"
        perror("nao_existe.txt");

        // strerror retorna a mensagem como string (para formatar):
        printf("Erro ao abrir: %s\n", strerror(errno));
        return 1;
    }

Diferenca:
    perror(prefixo)  -> imprime "prefixo: mensagem do sistema\n" em stderr
    strerror(errno) -> retorna string com a mensagem (voce formata)

Erros comuns de errno:
    ENOENT  - arquivo nao existe
    EACCES  - permissao negada
    ENOMEM  - sem memoria
    EINVAL  - argumento invalido
    ERANGE  - resultado fora do intervalo

Versao com malloc:
    int *v = malloc(1000000 * sizeof(int));
    if (v == NULL) {
        perror("malloc");
        // imprime: "malloc: Cannot allocate memory"
        exit(1);
    }

Explicacao: quando uma funcao do sistema falha (fopen, malloc, etc.), ela
define a variavel global errno com um codigo de erro. perror imprime esse
erro com um prefixo seu (o nome do arquivo, por exemplo). strerror retorna a
mensagem como string para voce usar como quiser. Sempre que uma funcao
retornar NULL/erro, use perror ou strerror(errno) para mostrar o MOTIVO real
do erro - muito melhor que "Erro ao abrir" sem explicacao.


24. CODIGOS DE RETORNO DE FUNCAO
================================================================

Convencao: return 0 = sucesso, return != 0 = erro (igual main).

    int salvar_arquivo(struct Produto p[], int n, char *nome) {
        if (p == NULL || nome == NULL) return 1;  // parametro invalido
        if (n < 0) return 2;                       // tamanho invalido

        FILE *f = fopen(nome, "w");
        if (f == NULL) return 3;                   // nao abriu

        for (int i = 0; i < n; i++) {
            if (fprintf(f, "%d %s %.2f\n", p[i].codigo, p[i].nome, p[i].preco) < 0) {
                fclose(f);
                return 4;                          // erro ao escrever
            }
        }
        fclose(f);
        return 0;  // sucesso
    }

    int main(void) {
        int ret = salvar_arquivo(p, n, "produtos.txt");
        switch (ret) {
            case 0: printf("Salvo com sucesso.\n"); break;
            case 1: printf("ERRO: parametro nulo.\n"); break;
            case 2: printf("ERRO: tamanho invalido.\n"); break;
            case 3: printf("ERRO: nao abriu arquivo.\n"); break;
            case 4: printf("ERRO: falha ao escrever.\n"); break;
            default: printf("ERRO desconhecido (%d).\n", ret);
        }
        return ret;  // main retorna o codigo de erro para o SO
    }

Versao com enum (mais legivel):
    enum { OK = 0, ERR_PARAM, ERR_ABRIU, ERR_ESCREVEU };

    int salvar(struct Produto p[], int n) {
        if (p == NULL) return ERR_PARAM;
        FILE *f = fopen("dados.txt", "w");
        if (f == NULL) return ERR_ABRIU;
        // ...
        return OK;
    }

Explicacao: funcao retorna codigo indicando o que aconteceu. 0 = deu certo,
>0 = tipo especifico de erro. O chamador (main) testa o retorno e age
consequentemente. Isso e o padrao do C - main retorna 0 (sucesso) ou != 0
(erro) para o sistema operacional. Use enum para nomes legiveis (OK,
ERR_ABRIU) em vez de numeros magicos. Facilita manutencao e leitura.


25. ESTRUTURA TRY-CATCH SIMULADA EM C (GOTO OU CODIGOS DE ERRO)
================================================================

C nao tem try/catch nativo. Simula-se com goto (para limpeza) ou codigos de
erro encadeados.

25.1 PADRAO GOTO PARA LIMPEZA (mais comum em C)
----------------------------------------------------------------

    int processar_arquivo(char *nome) {
        FILE *f = NULL;
        char *buffer = NULL;
        int ret = 1;  // erro por padrao

        f = fopen(nome, "r");
        if (f == NULL) goto falha;

        buffer = malloc(1000);
        if (buffer == NULL) goto falha;

        if (fgets(buffer, 1000, f) == NULL) goto falha;

        printf("Lido: %s\n", buffer);
        ret = 0;  // sucesso

    falha:
        if (buffer) free(buffer);  // so libera se alocou
        if (f) fclose(f);           // so fecha se abriu
        return ret;
    }

Explicacao: o goto pula direto para a label "falha", que faz a limpeza na
ORDEM INVERSA (fecha arquivo depois de liberar buffer - ou vice-versa,
dependendo da ordem de alocacao). Cada recurso e protegido por if (nao NULL)
para so liberar se foi alocado. Esse padrao e usado no kernel Linux e em
bibliotecas C - goto para limpeza e ACEITO e IDIOMATICO em C (nao e
"pecado"). Evita repetir codigo de limpeza em cada if de erro.

25.2 CODIGOS DE ERRO ENCADEADOS (sem goto)
----------------------------------------------------------------

    int processar(char *nome) {
        FILE *f;
        char *buffer;
        int ret = 1;

        f = fopen(nome, "r");
        if (f == NULL) return 1;  // falha cedo, nada a limpar

        buffer = malloc(1000);
        if (buffer == NULL) {
            fclose(f);  // tem que limpar o que ja abriu
            return 2;
        }

        if (fgets(buffer, 1000, f) == NULL) {
            free(buffer);
            fclose(f);
            return 3;
        }

        printf("Lido: %s\n", buffer);
        free(buffer);
        fclose(f);
        return 0;
    }

Explicacao: sem goto, cada ponto de falha precisa repetir a limpeza manual.
Funciona mas e verboso e propenso a esquecer algo (vazamento). O padrao com
goto (25.1) e mais limpo quando ha muitos recursos.

25.3 MACRO TRY/CATCH (avancado, so para mostrar que da)
----------------------------------------------------------------

    #include <setjmp.h>

    jmp_buf env;

    #define TRY     if (setjmp(env) == 0)
    #define CATCH   else
    #define THROW   longjmp(env, 1)

    int dividir(int a, int b) {
        if (b == 0) THROW;  // "lanca excecao"
        return a / b;
    }

    int main(void) {
        int x = 10, y = 0;
        TRY {
            int r = dividir(x, y);
            printf("Resultado: %d\n", r);
        } CATCH {
            printf("Excecao capturada: divisao por zero.\n");
        }
        return 0;
    }

Explicacao: setjmp/longjmp (setjmp.h) permitem "pular" de um ponto a outro do
codigo, simulando excecao. setjmp salva o estado (0 na primeira chamada),
longjmp restaura (volta para o setjmp com valor != 0). As macros TRY/CATCH/THROW
imitam o try/catch de linguagens como Java/C++. AVANCADO - em prova nao
exigem isso, mas e bom saber que existe. CUIDADO: longjmp nao chama
destrutores nem libera memoria - use com cautela.


RESUMO - CHECKLIST DE ROBUSTEZ
================================================================

Antes de entregar a prova, verifique se o codigo trata:

[ ] Entrada do usuario (valida intervalo, tipo, tamanho)
[ ] Divisao por zero (testa divisor != 0)
[ ] fopen == NULL (arquivo nao abriu)
[ ] malloc == NULL (memoria acabou)
[ ] Fim de arquivo (fread==1, fgets!=NULL, fscanf==n)
[ ] Vetor vazio (n == 0 antes de v[0])
[ ] Contador == 0 antes de media/porcentagem
[ ] String vazia (strlen == 0)
[ ] Indice dentro dos limites (0 <= i < n)
[ ] Overflow (int -> long long, fatorial com teto)
[ ] Float comparado com tolerancia (fabs < epsilon)
[ ] Buffer limpo apos scanf (loop getchar)
[ ] Retorno do scanf verificado (== numero de campos)
[ ] Nome/string com tamanho limitado (%49s, fgets com tamanho)
[ ] Arquivo inexistente (modo r falha)
[ ] Arquivo vazio (sem registros)
[ ] Codigo duplicado em cadastro (busca antes de inserir)
[ ] Codigo nao encontrado em busca (return -1)
[ ] Opcao invalida no menu (default no switch)
[ ] Caractere invalido (isalpha, isdigit, isupper)
[ ] Parametros da funcao (ponteiro NULL, tamanho negativo)
[ ] assert para invariantes em desenvolvimento
[ ] perror/strerror para erros do sistema
[ ] Codigo de retorno da funcao (0 = ok, != 0 = erro)
[ ] Limpeza de recursos no erro (goto ou free+fclose)

Cada item tratado e ponto extra na prova. Bom codigo robusto nao e o que
funciona quando tudo da certo - e o que funciona quando tudo da errado.

FIM DO DOCUMENTO
