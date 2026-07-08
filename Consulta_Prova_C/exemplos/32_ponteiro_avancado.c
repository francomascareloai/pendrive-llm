/*
 * ============================================================
 * ARQUIVO: 32_ponteiro_avancado.c
 *
 * O QUE FAZ: Demonstra os usos avancados de ponteiros em C,
 *           desde o basico (ponteiro para variavel) ate
 *           ponteiro para funcao e vetor de ponteiros.
 *           Mostra a relacao fundamental v[i] == *(v+i).
 *
 * TOPICO: Ponteiros avancados
 *
 * PALAVRAS-CHAVE (Ctrl-F): ponteiro, ponteiro para variavel,
 *   ponteiro para vetor, aritmetica de ponteiros, ponteiro
 *   para struct, operador seta, ->, ponteiro para funcao,
 *   vetor de ponteiros, passar vetor para funcao, v[i], *(v+i),
 *   equivalencia indice ponteiro, typedef struct, malloc,
 *   arrow operator, function pointer, char pointer, free,
 *   ponteiro para ponteiro, diferenca entre ponteiros.
 *
 * COMPILACAO: gcc -Wall -Wextra 32_ponteiro_avancado.c -o 32_ponteiro_avancado.exe
 * ============================================================
 */

#include <stdio.h>   /* printf, scanf, fgets, puts */
#include <stdlib.h>  /* malloc, free, system */
#include <string.h>  /* strcspn, strcpy */
#include <conio.h>   /* getche - especifico do Windows/MinGW */

/* ------------------------------------------------------- */
/* TYPEDEF STRUCT COM ALIAS (como cobra o professor)       */
/* Cria o tipo "Aluno" sem precisar escrever "struct"      */
/* toda vez que declarar uma variavel desse tipo.          */
/* ------------------------------------------------------- */
typedef struct {
    int   codigo;        /* inteiro: codigo do aluno */
    char  nome[50];      /* vetor de char: nome do aluno */
    float nota;          /* real: nota de 0 a 10 */
} Aluno;                 /* alias: agora "Aluno" e um tipo */

/* ------------------------------------------------------- */
/* PROTOTIPOS DAS FUNCOES                                  */
/* O compilador precisa conhecer as assinaturas antes do   */
/* main() para nao dar warning implicit declaration.       */
/* ------------------------------------------------------- */
void ponteiroVariavel(void);
void ponteiroVetor(void);
void ponteiroStruct(void);
void ponteiroFuncao(void);
void vetorPonteiros(void);
void passarVetorFuncao(int *v, int n);

/* Funcoes que serao apontadas por ponteiro-para-funcao */
int soma(int a, int b);
int multiplica(int a, int b);

/* ======================================================== */
/* FUNCAO PRINCIPAL                                         */
/* Menu do-while + switch, estilo do professor.             */
/* ======================================================== */
int main(void) {
    char op;  /* opcao do menu (char para usar com getche) */

    do {
        system("cls");  /* limpa a tela (Windows) */
        printf("=== PONTEIROS AVANCADOS ===\n\n");
        printf("1 - Ponteiro para variavel\n");
        printf("2 - Ponteiro para vetor (aritmetica)\n");
        printf("3 - Ponteiro para struct (operador ->)\n");
        printf("4 - Ponteiro para funcao\n");
        printf("5 - Vetor de ponteiros\n");
        printf("6 - Passar vetor para funcao\n");
        printf("0 - Sair\n\n");
        printf("Escolha: ");

        op = getche();  /* le 1 tecla sem precisar Enter */
        printf("\n\n");

        switch (op) {
            case '1':
                ponteiroVariavel();
                break;
            case '2':
                ponteiroVetor();
                break;
            case '3':
                ponteiroStruct();
                break;
            case '4':
                ponteiroFuncao();
                break;
            case '5':
                vetorPonteiros();
                break;
            case '6': {
                /* Vetor local que sera passado como ponteiro.
                   Dentro do case usamos chaves para poder
                   declarar variaveis (regra do C). */
                int numeros[] = {10, 20, 30, 40, 50};
                int qtd = (int)(sizeof(numeros) / sizeof(numeros[0]));
                passarVetorFuncao(numeros, qtd);
                break;
            }
            case '0':
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }

        if (op != '0') {
            printf("\n");
            system("pause");  /* "Pressione qualquer tecla..." */
        }
    } while (op != '0');

    return 0;
}

/* ======================================================== */
/* 1 - PONTEIRO PARA VARIAVEL                               */
/* Mostra: declarar ponteiro, & (endereco), * (conteudo),  */
/*         modificar variavel atraves do ponteiro,          */
/*         ponteiro para ponteiro (**pp).                   */
/* ======================================================== */
void ponteiroVariavel(void) {
    int x = 42;        /* variavel comum do tipo int */
    int *p;             /* ponteiro para int (ainda nao inicializado) */

    p = &x;  /* p recebe o ENDERECO de x (& = "endereco de") */

    printf("--- Ponteiro para variavel ---\n\n");
    printf("Valor de x ...........: %d\n", x);
    printf("Endereco de x (&x) ...: %p\n", (void*)&x);
    printf("Conteudo de p .........: %p  (mesmo endereco de x)\n", (void*)p);
    printf("Acessar via *p ........: %d  (* = dereferencia)\n", *p);

    /* Modificando x ATRAVES do ponteiro.
       *p significa "va ate o endereco guardado em p e grave o valor". */
    *p = 100;
    printf("\nApos *p = 100:\n");
    printf("x agora vale .........: %d (modificado via ponteiro!)\n", x);

    /* --- Ponteiro para ponteiro (cai em prova!) --- */
    int **pp;       /* ponteiro para ponteiro de int */
    pp = &p;        /* pp guarda o endereco de p (nao de x) */
    printf("\n--- Ponteiro para ponteiro ---\n");
    printf("Endereco de p (&p) ...: %p\n", (void*)&p);
    printf("Conteudo de pp .......: %p\n", (void*)pp);
    printf("Acessar x via **pp ...: %d\n", **pp);
    printf("(**pp = duas dereferencias: pp -> p -> x)\n");
}

/* ======================================================== */
/* 2 - PONTEIRO PARA VETOR (ARITMETICA DE PONTEIROS)        */
/* A RELACAO MAIS IMPORTANTE DA PROVA:                       */
/*     v[i]  ==  *(v + i)     (acesso ao CONTEUDO)          */
/*     &v[i] ==  (v + i)      (acesso ao ENDERECO)          */
/* Somar i a um ponteiro avanca i * sizeof(tipo) bytes.     */
/* Como int ocupa 4 bytes, p+1 avanca 4 bytes, nao 1.      */
/* O compilador ja multiplica por sizeof(int) automatico.   */
/* ======================================================== */
void ponteiroVetor(void) {
    int v[] = {5, 10, 15, 20, 25};  /* vetor de 5 int */
    int *p = v;  /* p aponta para o inicio do vetor.
                    Nome de vetor ja e um ponteiro para o 1o elemento,
                    entao NAO precisa de & (p = v equivale a p = &v[0]). */
    int i;
    int qtd = (int)(sizeof(v) / sizeof(v[0]));  /* numero de elementos */

    printf("--- Ponteiro para vetor (aritmetica) ---\n\n");

    /* Tabela mostrando que v[i] e *(v+i) dao o MESMO resultado */
    printf("Equivalencia v[i] e *(v+i):\n\n");
    printf("Indice | v[i] | *(v+i) | &v[i]    | (v+i)\n");
    printf("-------|------|--------|----------|----------\n");
    for (i = 0; i < qtd; i++) {
        printf("  %d   |  %d  |   %d    | %p | %p\n",
               i, v[i], *(v + i), (void*)&v[i], (void*)(v + i));
    }

    printf("\nPercebam: v[i] e *(v+i) dao o MESMO valor.\n");
    printf("          &v[i] e (v+i) dao o MESMO endereco.\n");

    /* Percorrendo vetor SÓ com aritmetica de ponteiro (sem [i]) */
    printf("\nPercorrendo com aritmetica de ponteiro (p++):\n");
    p = v;  /* reposiciona p no inicio do vetor */
    for (i = 0; i < qtd; i++) {
        printf("  *p = %d  (endereco: %p)\n", *p, (void*)p);
        p++;  /* avanca para o proximo int (pula sizeof(int) bytes) */
    }

    /* Diferenca entre ponteiros: p2 - p1 da o numero de ELEMENTOS
       (nao de bytes). O compilador divide por sizeof(int) sozinho. */
    int *p1 = &v[0];
    int *p2 = &v[3];
    printf("\nDiferenca entre ponteiros: p2 - p1 = %ld elementos\n",
           (long)(p2 - p1));
    printf("(nao sao bytes, sao ELEMENTOS do tipo int)\n");
}

/* ======================================================== */
/* 3 - PONTEIRO PARA STRUCT (operador seta ->)               */
/* Mostra: acessar campos de struct via ponteiro com ->,   */
/*         alocacao dinamica com malloc, entrada de dados   */
/*         com fgets + strcspn, liberar com free.           */
/*                                                         */
/* REGRA DE OURO:                                           */
/*   - struct DIRETA  (Aluno a)  => acesso com PONTO (.)    */
/*   - PONTEIRO struct (Aluno *p) => acesso com SETA (->)    */
/*   pa->codigo equivale a (*pa).codigo                     */
/* ======================================================== */
void ponteiroStruct(void) {
    printf("--- Ponteiro para struct (operador ->) ---\n\n");

    /* --- CASO 1: struct na stack, ponteiro aponta para ela --- */
    Aluno a1;            /* variavel struct (nao e ponteiro) */
    Aluno *pa;           /* ponteiro para Aluno */

    a1.codigo = 1;
    strcpy(a1.nome, "Franco");  /* copia string para o campo nome */
    a1.nota = 8.5f;

    pa = &a1;  /* pa aponta para a1 */

    /* struct DIRETA: usa PONTO (.) */
    printf("Acesso direto (ponto): %s - nota %.1f\n", a1.nome, a1.nota);

    /* PONTEIRO para struct: usa SETA (->) */
    /* pa->codigo e o mesmo que (*pa).codigo */
    printf("Acesso via ponteiro (seta): codigo=%d, nome=%s, nota=%.1f\n",
           pa->codigo, pa->nome, pa->nota);

    /* Modificando struct atraves do ponteiro */
    pa->nota = 9.5f;  /* muda a1.nota indiretamente */
    printf("Apos pa->nota = 9.5: a1.nota = %.1f\n", a1.nota);

    /* --- CASO 2: alocacao dinamica com malloc + entrada do usuario --- */
    printf("\n--- Alocacao dinamica (malloc) + entrada ---\n");
    Aluno *pd = malloc(sizeof(Aluno));  /* aloca na heap (memoria dinamica) */
    if (pd == NULL) {  /* SEMPRE verificar se malloc falhou */
        printf("Erro: memoria insuficiente!\n");
        return;
    }

    printf("Digite o codigo do aluno: ");
    scanf("%d", &pd->codigo);  /* &pd->codigo = endereco do campo codigo */
    fflush(stdin);  /* limpa o \n deixado pelo scanf no buffer.
                        NAO e portavel (nao funciona em Linux), mas
                        funciona no Windows/MinGW. Por isso usamos aqui. */

    printf("Digite o nome do aluno: ");
    /* fgets le ate 49 caracteres (deixa espaco para \0).
       NAO usamos gets() porque e perigoso (sem limite) e
       foi removido do padrao C11. */
    fgets(pd->nome, 50, stdin);
    /* fgets inclui o \n no final da string; removemos com strcspn.
       strcspn retorna a posicao do primeiro \n encontrado. */
    pd->nome[strcspn(pd->nome, "\n")] = '\0';

    printf("Digite a nota do aluno: ");
    scanf("%f", &pd->nota);
    fflush(stdin);  /* limpa o buffer novamente (mesmo comentario acima) */

    printf("\nAluno cadastrado (via ponteiro ->):\n");
    printf("  Codigo: %d\n", pd->codigo);
    printf("  Nome..: %s\n", pd->nome);
    printf("  Nota..: %.1f\n", pd->nota);

    free(pd);    /* libera a memoria alocada (evita vazamento!) */
    pd = NULL;   /* boa pratica: anular o ponteiro apos free */
    printf("(memoria liberada com free)\n");
}

/* ======================================================== */
/* 4 - PONTEIRO PARA FUNCAO                                 */
/* Sintaxe: tipo (*nome)(parametros)                        */
/* Os parenteses em (*nome) sao OBRIGATORIOS. Sem eles,     */
/* int *pf(int,int) seria uma FUNCAO que devolve int*,      */
/* nao um ponteiro para funcao.                             */
/* ======================================================== */

/* --- Funcoes que serao "apontadas" pelo ponteiro --- */
int soma(int a, int b) {
    return a + b;
}

int multiplica(int a, int b) {
    return a * b;
}

void ponteiroFuncao(void) {
    printf("--- Ponteiro para funcao ---\n\n");

    /* Declaracao do ponteiro para funcao:
       "pf e um ponteiro para uma funcao que recebe (int, int)
        e devolve int". */
    int (*pf)(int, int);

    int x = 10, y = 3;
    int resultado;

    /* Atribuindo o endereco de soma ao ponteiro.
       O nome da funcao (soma) ja e o endereco dela,
       assim como o nome de um vetor ja e seu endereco. */
    pf = soma;
    resultado = pf(x, y);  /* chama soma(10, 3) via ponteiro */
    printf("soma(%d, %d) = %d\n", x, y, resultado);

    /* Trocando para apontar para multiplica */
    pf = multiplica;
    resultado = pf(x, y);  /* chama multiplica(10, 3) via ponteiro */
    printf("multiplica(%d, %d) = %d\n", x, y, resultado);

    /* Por que usar? Permite escolher a funcao em tempo de execucao.
       E a base de callbacks, qsort, menus dinamicos, etc. */
    printf("\nO ponteiro pf aponta para funcoes diferentes em cada momento.\n");
    printf("Isso permite escolher comportamento em tempo de execucao.\n");
}

/* ======================================================== */
/* 5 - VETOR DE PONTEIROS                                   */
/* Cada elemento do vetor e um ponteiro.                    */
/* Caso classico: vetor de strings (char *).                */
/* ======================================================== */
void vetorPonteiros(void) {
    printf("--- Vetor de ponteiros ---\n\n");

    /* --- Vetor de ponteiros para char (vetor de strings) ---
       Cada elemento de nomes[] e um char*, ou seja,
       um ponteiro para o primeiro caractere de uma string.
       As strings literais ficam na area de dados (so leitura). */
    char *nomes[] = {
        "Franco",
        "Ana",
        "Pedro",
        "Maria",
        "Joao"
    };
    int qtdNomes = (int)(sizeof(nomes) / sizeof(nomes[0]));
    int i;

    printf("Vetor de strings (char *nomes[]):\n");
    for (i = 0; i < qtdNomes; i++) {
        /* nomes[i] e um char*; %s espera um char* */
        printf("  nomes[%d] = \"%s\"  (endereco: %p)\n",
               i, nomes[i], (void*)nomes[i]);
    }

    /* --- Vetor de ponteiros para int ---
       Cada elemento aponta para um int diferente. */
    printf("\nVetor de ponteiros para int:\n");
    int a = 100, b = 200, c = 300;
    int *vp[3];  /* vetor de 3 ponteiros para int */

    vp[0] = &a;  /* vp[0] aponta para a */
    vp[1] = &b;  /* vp[1] aponta para b */
    vp[2] = &c;  /* vp[2] aponta para c */

    for (i = 0; i < 3; i++) {
        /* *vp[i]: dereferencia o ponteiro que esta na posicao i */
        printf("  *vp[%d] = %d\n", i, *vp[i]);
    }

    /* Modificando variaveis atraves do vetor de ponteiros */
    *vp[0] = 999;  /* muda 'a' para 999 via ponteiro */
    printf("\nApos *vp[0] = 999: a = %d\n", a);
}

/* ======================================================== */
/* 6 - PASSAR VETOR PARA FUNCAO COMO PONTEIRO               */
/* Ao passar um vetor para funcao, o que se passa e o      */
/* ENDERECO do 1o elemento (nao se copia o vetor inteiro). */
/* Por isso a funcao pode modificar o vetor original.       */
/*                                                         */
/* RECEBER como int *v OU int v[] e A MESMA COISA em C.    */
/* O tamanho n precisa vir como parametro separado, pois  */
/* sizeof(v) aqui devolve tamanho de ponteiro (8 bytes em  */
/* 64 bits), nao o tamanho do vetor original.              */
/* ======================================================== */
void passarVetorFuncao(int *v, int n) {
    int i;

    printf("--- Passar vetor para funcao (como ponteiro) ---\n\n");

    printf("Vetor recebido (antes de dobrar):\n  ");
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);  /* v[i] funciona mesmo sendo ponteiro */
    }
    printf("\n");

    /* Dobrando cada elemento.
       Como v aponta para o vetor ORIGINAL (la na main),
       as alteracoes afetam o vetor de verdade. */
    for (i = 0; i < n; i++) {
        v[i] = v[i] * 2;  /* equivale a *(v+i) = *(v+i) * 2 */
    }

    printf("Vetor apos dobrar (modificado pela funcao):\n  ");
    for (i = 0; i < n; i++) {
        /* Mostrando tambem a forma *(v+i) para reforcar a equivalencia */
        printf("%d ", *(v + i));
    }
    printf("\n");

    printf("\nImportante: as alteracoes feitas aqui persistem na main(),\n");
    printf("porque v aponta para o mesmo endereco de memoria do vetor.\n");
    printf("(Passagem de vetor em C e sempre POR REFERENCIA na pratica.)\n");
}
