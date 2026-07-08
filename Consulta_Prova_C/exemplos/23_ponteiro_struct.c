/*
======================================================================
   23_ponteiro_struct.c
======================================================================
   O QUE FAZ: Demonstra o uso de PONTEIROS para STRUCT em C, cobrindo
              os 5 casos principais que caem na prova:
              (1) acesso por ponto (.) com variavel struct normal
              (2) ponteiro para struct e acesso com operador ->
              (3) funcao que recebe struct POR PONTEIRO e altera campos
              (4) funcao que recebe VETOR de struct (ja e ponteiro)
              (5) alocar struct dinamicamente com malloc e acessar com ->

   TOPICO: struct, ponteiros, operador ->, passagem por referencia,
           alocacao dinamica (malloc).

   PALAVRAS-CHAVE (Ctrl-F):
     struct Produto        - definicao da struct usada em todo o exemplo
     typedef struct        - como dar "apelido" a struct
     p.campo               - acesso por ponto (variavel normal)
     p->campo              - acesso por seta (ponteiro)  *** PONTO CENTRAL ***
     &p                   - obter endereco de uma struct
     passagem por ponteiro - funcao que altera struct original
     vetor de struct       - nome de vetor ja e ponteiro p/ 1o elemento
     malloc                - alocacao dinamica de uma struct
     sizeof(Produto)       - tamanho em bytes da struct
     free                  - liberar memoria alocada
     fgets + strcspn       - leitura segura de string (substitui gets)
     fflush(stdin)         - limpar buffer do teclado (estilo professor)
     menu switch do-while  - menu interativo no estilo do professor

   COMPILACAO:
     gcc -Wall -Wextra 23_ponteiro_struct.c -o 23_ponteiro_struct.exe

   AUTOR: Material de consulta - Franco (Uniftec / Algoritmos II)
======================================================================
*/

/* ---- BIBLIOTECAS ---- */
#include <stdio.h>   /* printf, scanf, fgets, fflush, getchar        */
#include <stdlib.h>  /* malloc, free, system                          */
#include <string.h>  /* strcspn (remover o \n do fgets)             */
#include <conio.h>   /* getche, getch - estilo do professor (Windows) */

/* =====================================================================
   DEFINICAO DA STRUCT
   =====================================================================
   Usamos "typedef struct { ... } Produto;" para criar um NOVO TIPO
   chamado "Produto". Assim podemos declarar "Produto p;" em vez de
   "struct Produto p;" (mais limpo e o que o professor costuma usar).

   Campos:
     codigo - inteiro (id do produto)
     nome   - vetor de 50 chars (string sem alocacao dinamica)
     preco  - float (valor do produto)
   ===================================================================== */
typedef struct {
    int   codigo;   /* inteiro: identificador unico do produto      */
    char  nome[50]; /* string fixa de ate 49 chars + '\0'           */
    float preco;    /* preco unitario (usar float p/ simplificar)   */
} Produto;          /* <-- ponto-e-virgula OBRIGATORIO apos struct!  */


/* =====================================================================
   PROTOTIPOS DAS FUNCOES
   =====================================================================
   Declarar os prototipos antes do main() permite que o compilador
   conheca as funcoes mesmo que sejam implementadas depois do main.
   ===================================================================== */
void mostrarProduto(Produto p);               /* recebe COPIA (so leitura)  */
void alterarPorPonteiro(Produto *p);          /* recebe PONTEIRO (altera)   */
void listarVetor(Produto vet[], int n);       /* recebe vetor (= ponteiro)   */
void cadastrarVetor(Produto vet[], int n);   /* preenche vetor via ponteiro */
void limparBuffer(void);                      /* limpa stdin (fflush)        */


/* =====================================================================
   FUNCAO 1: mostrarProduto
   ---------------------------------------------------------------------
   Recebe a struct POR VALOR (copia). Isso significa que qualquer
   alteracao feita aqui NAO afeta a struct original do chamador.
   Por isso esta funcao so EXIBE os dados, nao altera.

   Note o acesso com PONTO (.): como "p" e uma variavel struct
   (nao ponteiro), usamos p.codigo, p.nome, p.preco.
   ===================================================================== */
void mostrarProduto(Produto p)
{
    /* p.codigo  -> acesso por ponto, pois p e struct normal        */
    /* p.nome    -> nome e um vetor, ja e tratado como ponteiro p/   */
    /*              printf com %s, sem precisar de "." nem "->"      */
    /* p.preco   -> acesso por ponto novamente                      */
    printf("Codigo : %d\n", p.codigo);
    printf("Nome   : %s\n", p.nome);
    printf("Preco  : %.2f\n", p.preco);
}


/* =====================================================================
   FUNCAO 2: alterarPorPonteiro
   ---------------------------------------------------------------------
   Recebe a struct POR PONTEIRO (Produto *p). Aqui "p" e um ENDERECO.
   Como recebemos o endereco da struct original, qualquer alteracao
   feita aqui AFETA a struct do chamador (passagem por referencia).

   *** DIFERENCA FUNDAMENTAL: p->campo vs p.campo ***
   ---------------------------------------------------------------------
     - Se "p" e STRUCT NORMAL  -> acesso com PONTO:    p.campo
     - Se "p" e PONTEIRO       -> acesso com SETA:    p->campo

   O operador "->" e um ATALHO. As duas linhas abaixo sao EQUIVALENTES:
        (*p).codigo = 5;     <- desreferencia e acessa por ponto
        p->codigo   = 5;     <- atalho: seta faz a mesma coisa

   Usar "->" e mais legivel e e o padrao em C. CAI NA PROVA!
   ===================================================================== */
void alterarPorPonteiro(Produto *p)
{
    /* p->codigo = ... : como p e ponteiro, usamos "->" para gravar  */
    printf("\n--- Alterando via ponteiro (->) ---\n");

    printf("Novo codigo: ");
    scanf("%d", &p->codigo);  /* &p->codigo = endereco do campo codigo */

    /* fflush(stdin): limpa o buffer do teclado apos o scanf.
       ESTILO DO PROFESSOR: ele usa fflush(stdin) entre leituras
       de numeros e strings para evitar que o \n deixado pelo
       scanf seja lido pelo fgets seguinte.
       OBS: fflush(stdin) e comportamento definido no Windows
       (MSVC/MinGW), mas NAO e portavel para Linux/Mac. Na prova
       (Windows + gcc) funciona e e o que o professor ensina.        */
    fflush(stdin);

    printf("Novo nome: ");
    /* fgets le com seguranca (limita tamanho, evita buffer overflow).
       Substitui o gets() que e perigoso e foi removido do C11.
       Ler ate 49 chars para caber o '\0' no vetor de 50.            */
    fgets(p->nome, 50, stdin);

    /* strcspn encontra a posicao do '\n' e substitui por '\0',
       removendo a quebra de linha que o fgets captura.              */
    p->nome[strcspn(p->nome, "\n")] = '\0';

    printf("Novo preco: ");
    scanf("%f", &p->preco);   /* &p->preco = endereco do campo preco */
    fflush(stdin);
}


/* =====================================================================
   FUNCAO 3: cadastrarVetor
   ---------------------------------------------------------------------
   Recebe um VETOR de struct. Em C, quando passamos um vetor para uma
   funcao, o que realmente e passado e o ENDERECO do primeiro elemento.
   Ou seja: "Produto vet[]" e EQUIVALENTE a "Produto *vet".

   Por isso, alteracoes feitas aqui afetam o vetor original do main().
   NAO e necessario usar "&" nem "*" na chamada: basta passar o nome.

   Dentro da funcao, tratamos "vet" como vetor normal (vet[i].campo),
   mas poderiamos usar aritmetica de ponteiros: (vet+i)->campo.
   ===================================================================== */
void cadastrarVetor(Produto vet[], int n)
{
    int i;  /* contador do loop */

    for (i = 0; i < n; i++) {
        printf("\n--- Produto %d ---\n", i + 1);

        printf("Codigo: ");
        scanf("%d", &vet[i].codigo);  /* &vet[i].codigo: endereco do campo */
        fflush(stdin);

        printf("Nome: ");
        fgets(vet[i].nome, 50, stdin);
        vet[i].nome[strcspn(vet[i].nome, "\n")] = '\0';

        printf("Preco: ");
        scanf("%f", &vet[i].preco);
        fflush(stdin);
    }
}


/* =====================================================================
   FUNCAO 4: listarVetor
   ---------------------------------------------------------------------
   Mesma ideia da funcao acima: recebe o vetor (que ja e ponteiro).
   Aqui so percorremos e exibimos. Note o uso de vet[i].campo com
   PONTO, pois vet[i] e uma struct normal (o ponteiro e "vet", nao vet[i]).
   ===================================================================== */
void listarVetor(Produto vet[], int n)
{
    int i;

    printf("\n===== LISTA DE PRODUTOS (%d) =====\n", n);
    for (i = 0; i < n; i++) {
        printf("\nProduto %d:\n", i + 1);
        /* vet[i] e uma struct normal -> acesso por PONTO */
        printf("  Codigo: %d\n", vet[i].codigo);
        printf("  Nome  : %s\n", vet[i].nome);
        printf("  Preco : %.2f\n", vet[i].preco);
    }
}


/* =====================================================================
   FUNCAO AUXILIAR: limparBuffer
   ---------------------------------------------------------------------
   Encapsula fflush(stdin) para reutilizar. Mantemos o estilo do
   professor (fflush no Windows). Se quisessemos portabilidade total,
   usaríamos um loop com getchar() ate encontrar '\n'.
   ===================================================================== */
void limparBuffer(void)
{
    fflush(stdin);  /* limpa o buffer de entrada (estilo professor) */
}


/* =====================================================================
   FUNCAO PRINCIPAL
   =====================================================================
   Demonstra os 5 casos pedidos. Usa menu com switch + do-while no
   estilo do professor (conio.h, system("cls"), system("pause")).
   ===================================================================== */
int main(void)
{
    /* -----------------------------------------------------------------
       CASO 1: variavel struct NORMAL + acesso por PONTO (.)
       -----------------------------------------------------------------
       Aqui "p1" e uma struct alocada na PILHA (stack). Acessamos seus
       campos com PONTO: p1.codigo, p1.nome, p1.preco.
       ----------------------------------------------------------------- */
    Produto p1;                  /* struct normal na stack             */
    p1.codigo = 101;             /* acesso por ponto                    */
    strcpy(p1.nome, "Teclado");  /* strcpy para preencher string       */
    p1.preco  = 49.90f;          /* sufixo f = float (evita warning)    */

    /* -----------------------------------------------------------------
       CASO 2: PONTEIRO para struct + acesso com SETA (->)
       -----------------------------------------------------------------
       Criamos um ponteiro "ptr" que aponta para p1 (&p1 = endereco).
       A partir de agora, acessamos os campos via ptr->campo.
       ptr->campo  ==  (*ptr).campo  (sao equivalentes)
       ----------------------------------------------------------------- */
    Produto *ptr;                /* declara ponteiro para struct         */
    ptr = &p1;                   /* ptr aponta para p1                   */

    /* -----------------------------------------------------------------
       CASO 4 (preparacao): VETOR de struct
       -----------------------------------------------------------------
       Um vetor de struct tambem fica na stack. O nome "estoque" e
       tratado como ponteiro para o primeiro elemento. Por isso,
       ao passar para funcao, passamos so o nome (sem &).
       ----------------------------------------------------------------- */
    Produto estoque[3];          /* vetor de 3 produtos                 */
    int nProdutos = 3;           /* quantidade usada no vetor           */

    /* -----------------------------------------------------------------
       CASO 5 (preparacao): alocacao DINAMICA com malloc
       -----------------------------------------------------------------
       malloc aloca memoria no HEAP e devolve um PONTEIRO void*.
       Fazemos cast para (Produto *) e guardamos em "pdin".
       Depois acessamos os campos com "->" (pois e ponteiro).
       IMPORTANTE: toda memoria alocada com malloc deve ser liberada
       com free() antes do programa terminar (evita vazamento).
       ----------------------------------------------------------------- */
    Produto *pdin = NULL;        /* inicia NULL por seguranca           */

    /* --- Variaveis do menu --- */
    char opcao;                  /* guarda a escolha do menu (getche)    */

    /* =================================================================
       MENU PRINCIPAL (do-while + switch, estilo do professor)
       =================================================================
       O do-while garante que o menu apareca PELO MENOS uma vez.
       getche() le um caractere e ecoa na tela (da conio.h).
       ================================================================= */
    do {
        system("cls");           /* limpa a tela (Windows)              */
        printf("===== PONTEIRO PARA STRUCT =====\n");
        printf("1 - Caso 1: struct normal + acesso por ponto (.)\n");
        printf("2 - Caso 2: ponteiro para struct + acesso por seta (->)\n");
        printf("3 - Caso 3: funcao que altera struct via ponteiro\n");
        printf("4 - Caso 4: vetor de struct (ja e ponteiro)\n");
        printf("5 - Caso 5: alocar struct com malloc + acesso com ->\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        opcao = getche();        /* le opcao + ecoa na tela             */
        printf("\n\n");

        switch (opcao) {

        /* -------------------------------------------------------------
           CASO 1: acesso por PONTO com variavel normal
           ------------------------------------------------------------- */
        case '1':
            printf("CASO 1: struct normal, acesso por PONTO (.)\n");
            printf("p1.codigo = 101; p1.nome = \"Teclado\"; p1.preco = 49.90\n\n");
            /* Como p1 e struct normal, usamos ponto: */
            mostrarProduto(p1);  /* passa por VALOR (copia)             */
            break;

        /* -------------------------------------------------------------
           CASO 2: ponteiro para struct + operador ->
           ------------------------------------------------------------- */
        case '2':
            printf("CASO 2: ponteiro ptr aponta para p1 (&p1)\n");
            printf("Acesso via ptr->campo (equivale a (*ptr).campo)\n\n");
            /* ptr->campo: como ptr e PONTEIRO, usamos SETA */
            printf("ptr->codigo = %d\n", ptr->codigo);
            printf("ptr->nome   = %s\n", ptr->nome);
            printf("ptr->preco  = %.2f\n", ptr->preco);
            printf("\n(*ptr).codigo = %d  (forma equivalente)\n", (*ptr).codigo);
            break;

        /* -------------------------------------------------------------
           CASO 3: funcao que recebe struct POR PONTEIRO e altera
           -------------------------------------------------------------
           Passamos &p1 (endereco). A funcao altera o ORIGINAL.
           ------------------------------------------------------------- */
        case '3':
            printf("CASO 3: alterarPorPonteiro(&p1)\n");
            printf("ANTES da alteracao:\n");
            mostrarProduto(p1);

            alterarPorPonteiro(&p1);  /* passa ENDERECO da struct      */

            printf("\nDEPOIS da alteracao (struct original mudou!):\n");
            mostrarProduto(p1);
            break;

        /* -------------------------------------------------------------
           CASO 4: vetor de struct (nome ja e ponteiro)
           -------------------------------------------------------------
           Passamos "estoque" (sem &). A funcao recebe como ponteiro.
           ------------------------------------------------------------- */
        case '4':
            printf("CASO 4: vetor de struct\n");
            printf("Nome do vetor ja e ponteiro p/ 1o elemento.\n");
            cadastrarVetor(estoque, nProdutos);  /* sem & na chamada  */
            listarVetor(estoque, nProdutos);
            break;

        /* -------------------------------------------------------------
           CASO 5: alocar struct com malloc + acesso com ->
           -------------------------------------------------------------
           malloc(sizeof(Produto)) devolve endereco do bloco alocado.
           Acessamos os campos com "->" (pois e ponteiro).
           No fim, free(pdin) libera a memoria.
           ------------------------------------------------------------- */
        case '5':
            printf("CASO 5: alocacao dinamica com malloc\n");

            /* Aloca memoria para UMA struct Produto no heap.
               sizeof(Produto) = tamanho em bytes da struct.
               Cast (Produto*) converte o void* retornado pelo malloc. */
            pdin = (Produto *) malloc(sizeof(Produto));

            /* SEMPRE conferir se malloc deu certo (pode falhar). */
            if (pdin == NULL) {
                printf("ERRO: sem memoria para alocar!\n");
                break;
            }

            /* Como pdin e PONTEIRO, usamos "->" para acessar campos. */
            pdin->codigo = 999;
            strcpy(pdin->nome, "Mouse Gamer");
            pdin->preco  = 89.90f;

            printf("Struct alocada com malloc, acessada com ->:\n");
            printf("pdin->codigo = %d\n", pdin->codigo);
            printf("pdin->nome   = %s\n", pdin->nome);
            printf("pdin->preco  = %.2f\n", pdin->preco);

            /* LIBERA a memoria alocada (evita vazamento). */
            free(pdin);
            pdin = NULL;          /* boa pratica: anular apos free    */
            break;

        case '0':
            printf("Saindo... Boa sorte na prova!\n");
            break;

        default:
            printf("Opcao invalida! Tente novamente.\n");
            break;
        }

        /* system("pause") no Windows: "Pressione qualquer tecla...".
           Estilo do professor para pausar antes de limpar a tela. */
        if (opcao != '0') {
            system("pause");
        }

    } while (opcao != '0');      /* repete ate digitar 0               */

    return 0;  /* fim do programa com sucesso                          */
}


/* =====================================================================
   RESUMO DIDATICO (revise isto antes da prova!)
   =====================================================================
   1) struct NORMAL  -> acesso com PONTO:    p.campo
   2) PONTEIRO      -> acesso com SETA:    p->campo
      (p->campo equivale a (*p).campo)

   3) Passagem por VALOR (Produto p):
        - funcao recebe COPIA; alteracoes NAO afetam original.
   4) Passagem por PONTEIRO (Produto *p):
        - funcao recebe ENDERECO; alteracoes AFETAM original.
        - na chamada use &:  funcao(&p1)

   5) Vetor de struct:
        - nome do vetor ja e ponteiro p/ 1o elemento.
        - passar para funcao sem &:  funcao(vet, n)
        - dentro da funcao: vet[i].campo (ponto, pois vet[i] e struct)

   6) malloc:
        - Produto *p = malloc(sizeof(Produto));
        - acessar com ->:  p->campo = ...;
        - conferir NULL!
        - liberar com free(p);  e anular: p = NULL;

   PEGADINHA DE PROVA: confundir "." com "->".
     - Se a variavel e STRUCT, use ".".
     - Se a variavel e PONTEIRO, use "->".
   =====================================================================
*/
