/*
=====================================================================
   33_alocacao_dinamica.c
---------------------------------------------------------------------
   O QUE FAZ: Demonstra alocacao dinamica de memoria em C usando
               malloc, calloc, realloc e free. Mostra como alocar
               um vetor de int dinamicamente, redimensiona-lo com
               realloc, alocar um vetor de struct dinamicamente,
               verificar se malloc retornou NULL (falha) e contar
               elementos a partir do tamanho em bytes.

   TOPICO: Alocacao dinamica de memoria (stdlib.h).
   PALAVRAS-CHAVE Ctrl-F:
      malloc          calloc          realloc         free
      NULL            sizeof          vetor dinamico  struct dinamica
      ponteiro        heap            memoria         alocacao
      redimensionar   verificar NULL  menu do-while   switch
      typedef struct  fgets strcspn   fflush stdin    conio getche
      contar elementos sizeof division
---------------------------------------------------------------------
   COMPILACAO (Windows, MinGW):
      gcc -Wall -Wextra 33_alocacao_dinamica.c -o 33_alocacao_dinamica.exe
=====================================================================
*/

/* ---- Bibliotecas necessarias ---- */
#include <stdio.h>   /* printf, scanf, fgets, puts, getchar            */
#include <stdlib.h>  /* malloc, calloc, realloc, free, system, exit     */
#include <string.h> /* strcspn (remover '\n' do fgets)                  */
#include <conio.h>   /* getche, getch (leitura de 1 tecla) - estilo prof */

/* -------------------------------------------------------------------
   Definicao de uma struct simples para demonstrar alocacao dinamica
   de um vetor de struct. Usamos typedef para criar um alias (apelido)
   "Pessoa", evitando ter que escrever "struct Pessoa" toda hora.
   ------------------------------------------------------------------- */
typedef struct {
    char  nome[50]; /* nome da pessoa (texto)                          */
    int   idade;    /* idade da pessoa (inteiro)                       */
    float altura;    /* altura em metros (real)                         */
} Pessoa;           /* agora basta escrever "Pessoa" no codigo         */

/* -------------------------------------------------------------------
   Prototipos das funcoes (declaradas antes do main para o compilador
   conhecer as assinaturas). Modularizar deixa o codigo mais legivel e
   reutilizavel - estilo do professor.
   ------------------------------------------------------------------- */
static void limpar_buffer(void);                       /* limpa stdin                */
static void pausar(void);                              /* system("pause") encapsulado*/
static void menu_principal(void);                      /* exibe opcoes do menu        */
static void exemplo_malloc_vetor_int(void);            /* malloc de vetor de int      */
static void exemplo_calloc_vetor_int(void);            /* calloc de vetor de int      */
static void exemplo_realloc_redimensionar(void);       /* realloc cresce/encolhe vetor*/
static void exemplo_vetor_struct(void);                /* malloc de vetor de Pessoa   */
static void exemplo_contar_elementos(void);            /* conta elementos via sizeof  */
static void exemplo_free_correto(void);                /* mostra uso e perigo do free */

/* ===================================================================
   FUNCAO PRINCIPAL
   =================================================================== */
int main(void)
{
    int opcao; /* guarda a opcao escolhida pelo usuario no menu */

    /* Loop do-while + switch: padrao do professor para menus.
       Executa pelo menos 1 vez e repete ate o usuario escolher Sair (0). */
    do {
        menu_principal();          /* mostra as opcoes na tela            */
        printf("Digite a opcao: "); /* pede a escolha do usuario          */
        scanf("%d", &opcao);        /* le a opcao como inteiro             */
        limpar_buffer();            /* descarta o '\n' deixado pelo scanf */

        switch (opcao) {
            case 1:
                exemplo_malloc_vetor_int();      /* malloc vetor de int  */
                break;
            case 2:
                exemplo_calloc_vetor_int();      /* calloc vetor de int  */
                break;
            case 3:
                exemplo_realloc_redimensionar(); /* realloc redimensiona */
                break;
            case 4:
                exemplo_vetor_struct();          /* vetor de struct dinam*/
                break;
            case 5:
                exemplo_contar_elementos();      /* contar via sizeof    */
                break;
            case 6:
                exemplo_free_correto();           /* free e cuidados      */
                break;
            case 0:
                puts("\nSaindo do programa... Ate logo!"); /* fim        */
                break;
            default:
                /* opcao invalida: avisa e volta para o menu             */
                puts("\nOpcao invalida! Escolha entre 0 e 6.");
                break;
        }

        /* Pausa antes de limpar a tela, para o usuario ler a saida.
           So pausa se nao for a opcao de saida. */
        if (opcao != 0) {
            pausar();
        }
    } while (opcao != 0); /* repete enquanto nao escolher Sair (0) */

    return 0; /* fim normal do programa */
}

/* ===================================================================
   FUNCOES AUXILIARES
   =================================================================== */

/* -------------------------------------------------------------------
   limpar_buffer: descarta caracteres restantes no stdin (teclado).
   Importante apos scanf, pois o '\n' (Enter) fica no buffer e pode
   estravar um fgets posterior. fflush(stdin) nao e portavel (nao
   funciona em Linux), mas no Windows funciona; aqui usamos o metodo
   portavel com getchar para maior compatibilidade.
   ------------------------------------------------------------------- */
static void limpar_buffer(void)
{
    int c;
    /* le e descarta todos os caracteres ate encontrar '\n' ou EOF */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* apenas descarta */
    }
}

/* -------------------------------------------------------------------
   pausar: encapsula system("pause") do Windows. Centralizar evita
   repetir o comando em varios lugares.
   ------------------------------------------------------------------- */
static void pausar(void)
{
    system("pause"); /* "Pressione qualquer tecla para continuar..." */
}

/* -------------------------------------------------------------------
   menu_principal: apenas imprime as opcoes do menu na tela.
   ------------------------------------------------------------------- */
static void menu_principal(void)
{
    system("cls"); /* limpa a tela (Windows) - estilo do professor */
    puts("==============================================");
    puts("   ALOCACAO DINAMICA DE MEMORIA - EXEMPLOS    ");
    puts("==============================================");
    puts(" 1 - malloc  : alocar vetor de int            ");
    puts(" 2 - calloc  : alocar e zerar vetor de int    ");
    puts(" 3 - realloc : redimensionar vetor            ");
    puts(" 4 - malloc  : alocar vetor de struct         ");
    puts(" 5 - Contar elementos via sizeof              ");
    puts(" 6 - free    : liberar memoria (cuidados)      ");
    puts(" 0 - Sair                                     ");
    puts("==============================================");
}

/* ===================================================================
   EXEMPLO 1: malloc para vetor de int
   -------------------------------------------------------------------
   malloc aloca UM BLOCO CONTIGUO de bytes na "heap" (memoria livre
   do programa) e devolve um ponteiro void* para o inicio desse bloco.
   A memoria NAO e zerada (pode conter lixo). Por isso, sempre
   inicialize os valores depois de alocar.
   Sempre verifique se o retorno e NULL (falha de alocacao).
   =================================================================== */
static void exemplo_malloc_vetor_int(void)
{
    int n;        /* quantidade de elementos que o usuario quer alocar */
    int *v;       /* ponteiro que vai apontar para o vetor dinamico   */
    int i;        /* contador para os lacos for                       */

    puts("\n--- EXEMPLO 1: malloc para vetor de int ---");

    /* Pede ao usuario quantos inteiiros ele quer no vetor. */
    printf("Quantos inteiros deseja alocar? ");
    scanf("%d", &n);
    limpar_buffer();

    /* Validacao basica: n deve ser positivo. Se for <= 0, nao faz sentido
       alocar. Evita comportamento estranho no malloc. */
    if (n <= 0) {
        puts("Quantidade invalida! Deve ser maior que zero.");
        return; /* sai da funcao sem alocar nada */
    }

    /* ---- ALOCACAO COM malloc ----
       Sintaxe: malloc(numero_de_bytes)
       Queremos n inteiros, cada um ocupa sizeof(int) bytes.
       sizeof(int) devolve o tamanho em bytes de um int (geralmente 4).
       Multiplicamos: n * sizeof(int) = total de bytes necessarios.
       O cast (int*) converte o void* retornado para ponteiro de int.
       Em C puro o cast e opcional (void* converte implicitamente),
       mas muitos professores pedem por clareza/compatibilidade com C++.
    */
    v = (int *) malloc(n * sizeof(int));

    /* ---- VERIFICACAO DE NULL (OBRIGATORIA!) ----
       Se nao houver memoria suficiente, malloc retorna NULL.
       Usar um ponteiro NULL causa crash (segmentation fault).
       Por isso SEMPRE teste apos malloc/calloc/realloc. */
    if (v == NULL) {
        puts("Erro: nao foi possivel alocar memoria (malloc retornou NULL).");
        return; /* aborta a operacao, nao tenta usar v */
    }

    /* Agora v aponta para um bloco valido de n inteiros.
       Preenchemos o vetor com valores de exemplo: i*10. */
    for (i = 0; i < n; i++) {
        v[i] = i * 10; /* atribuicao direta, como vetor normal */
    }

    /* Mostramos os valores alocados e preenchidos. */
    puts("Vetor alocado e preenchido:");
    for (i = 0; i < n; i++) {
        printf("  v[%d] = %d\n", i, v[i]);
    }

    /* ---- LIBERACAO COM free ----
       Toda memoria alocada com malloc/calloc/realloc DEVE ser
       liberada com free quando nao for mais usada. Se esquecer,
       ocorre "vazamento de memoria" (memory leak): a memoria fica
       reservada e indisponivel ate o fim do programa. */
    free(v);

    /* Boa pratica: apos free, anular o ponteiro para evitar uso
       acidental de memoria ja liberada ("dangling pointer"). */
    v = NULL;

    printf("\nMemoria liberada com free. Total alocado: %d bytes (%d x %zu).\n",
           n * (int)sizeof(int), n, sizeof(int));
}

/* ===================================================================
   EXEMPLO 2: calloc para vetor de int
   -------------------------------------------------------------------
   calloc difere de malloc em 2 pontos:
     1) Recebe DOIS argumentos: numero de elementos e tamanho de cada.
     2) ZERA toda a memoria alocada (inicializa com bits 0).
   Use calloc quando precisar da memoria ja limpa (ex.: contadores,
   somatorios, flags). Custa um pouco mais que malloc por causa do
   preenchimento com zero, mas evita bugs de "valor lixo".
   =================================================================== */
static void exemplo_calloc_vetor_int(void)
{
    int n;        /* numero de elementos                          */
    int *v;       /* ponteiro para o vetor alocado com calloc     */
    int i;        /* contador                                     */

    puts("\n--- EXEMPLO 2: calloc para vetor de int ---");

    printf("Quantos inteiros deseja alocar (zerados)? ");
    scanf("%d", &n);
    limpar_buffer();

    if (n <= 0) {
        puts("Quantidade invalida! Deve ser maior que zero.");
        return;
    }

    /* ---- ALOCACAO COM calloc ----
       Sintaxe: calloc(qtd_elementos, tamanho_de_cada_elemento)
       Aqui: n elementos, cada um sizeof(int) bytes.
       calloc ja zera a memoria, entao todos os v[i] comecam em 0. */
    v = (int *) calloc(n, sizeof(int));

    /* ---- VERIFICACAO DE NULL (sempre!) ---- */
    if (v == NULL) {
        puts("Erro: calloc retornou NULL. Memoria insuficiente.");
        return;
    }

    /* Mostramos que a memoria veio ZERADA pelo calloc.
       Nao preenchemos nada - os valores devem aparecer como 0. */
    puts("Valores logo apos calloc (devem ser todos 0):");
    for (i = 0; i < n; i++) {
        printf("  v[%d] = %d\n", i, v[i]);
    }

    /* Agora preenchemos com valores para mostrar que funciona
       igual a um vetor normal. */
    for (i = 0; i < n; i++) {
        v[i] = (i + 1) * 5; /* 5, 10, 15, 20, ... */
    }

    puts("Apos preencher com (i+1)*5:");
    for (i = 0; i < n; i++) {
        printf("  v[%d] = %d\n", i, v[i]);
    }

    /* ---- LIBERACAO ---- */
    free(v);
    v = NULL; /* anula ponteiro apos free */

    puts("Memoria liberada com free.");
}

/* ===================================================================
   EXEMPLO 3: realloc para redimensionar vetor
   -------------------------------------------------------------------
   realloc altera o tamanho de um bloco ja alocado. Pode:
     - crescer o vetor (mais elementos);
     - encolher o vetor (menos elementos);
     - mover o bloco para outro endereco se nao couber no lugar.
   CUIDADO: realloc pode retornar um NOVO endereco. Por isso usamos
   um ponteiro temporario para nao perder o bloco original se falhar.
   =================================================================== */
static void exemplo_realloc_redimensionar(void)
{
    int n_inicial = 3;   /* comecamos com 3 elementos                  */
    int n_novo;          /* novo tamanho pedido pelo usuario            */
    int *v;              /* ponteiro do vetor dinamico                  */
    int *tmp;            /* ponteiro temporario para realloc seguro     */
    int i;               /* contador                                    */

    puts("\n--- EXEMPLO 3: realloc (redimensionar) ---");

    /* ---- Passo 1: alocar vetor inicial com malloc ---- */
    v = (int *) malloc(n_inicial * sizeof(int));
    if (v == NULL) {
        puts("Erro ao alocar vetor inicial.");
        return;
    }

    /* Preenchemos os 3 elementos iniciais. */
    for (i = 0; i < n_inicial; i++) {
        v[i] = (i + 1) * 100; /* 100, 200, 300 */
    }

    puts("Vetor inicial (3 elementos):");
    for (i = 0; i < n_inicial; i++) {
        printf("  v[%d] = %d\n", i, v[i]);
    }

    /* ---- Passo 2: pedir novo tamanho ao usuario ---- */
    printf("\nDigite o novo tamanho do vetor (ex.: 6 para crescer, 2 para encolher): ");
    scanf("%d", &n_novo);
    limpar_buffer();

    if (n_novo <= 0) {
        puts("Tamanho invalido! Deve ser maior que zero.");
        free(v); /* libera o que ja alocamos antes de sair */
        v = NULL;
        return;
    }

    /* ---- Passo 3: realloc SEGURO com ponteiro temporario ----
       Por que usar tmp? Se realloc falhar (retornar NULL) e fizermos
       direto "v = realloc(v, ...)", perderiamos o ponteiro original
       e nao conseguiriamos liberar a memoria antiga -> vazamento.
       Com tmp, so atribuimos a v se realloc der certo. */
    tmp = (int *) realloc(v, n_novo * sizeof(int));
    if (tmp == NULL) {
        /* realloc falhou, mas o bloco original (v) continua valido! */
        puts("Erro: realloc falhou. O vetor original permanece intacto.");
        free(v); /* libera o bloco original */
        v = NULL;
        return;
    }

    /* realloc deu certo: atualizamos v com o novo endereco. */
    v = tmp;

    /* Se o vetor cresceu, os novos elementos podem conter LIXO
       (realloc nao zera como calloc). Inicializamos por seguranca
       apenas os elementos novos (a partir de n_inicial). */
    if (n_novo > n_inicial) {
        for (i = n_inicial; i < n_novo; i++) {
            v[i] = 0; /* zera os novos para evitar lixo */
        }
    }

    /* Mostramos o vetor redimensionado. */
    printf("\nVetor apos realloc para %d elementos:\n", n_novo);
    for (i = 0; i < n_novo; i++) {
        printf("  v[%d] = %d\n", i, v[i]);
    }

    /* ---- Liberacao final ---- */
    free(v);
    v = NULL;

    puts("\nMemoria liberada com free apos realloc.");
}

/* ===================================================================
   EXEMPLO 4: alocar vetor de STRUCT dinamicamente
   -------------------------------------------------------------------
   Alocar um vetor de struct nao difere de alocar um vetor de int:
   basta usar sizeof(TipoStruct) como tamanho de cada elemento.
   Acessamos os campos com v[i].campo, igual a um vetor estatico.
   =================================================================== */
static void exemplo_vetor_struct(void)
{
    int n;        /* quantas pessoas cadastrar                       */
    Pessoa *p;   /* ponteiro para vetor dinamico de Pessoa           */
    int i;       /* contador                                         */
    char lixo;   /* variavel auxiliar para limpar '\n' (nao usada)   */
    (void)lixo;  /* evita warning de variavel nao usada com -Wextra  */

    puts("\n--- EXEMPLO 4: malloc para vetor de struct (Pessoa) ---");

    printf("Quantas pessoas deseja cadastrar? ");
    scanf("%d", &n);
    limpar_buffer();

    if (n <= 0) {
        puts("Quantidade invalida! Deve ser maior que zero.");
        return;
    }

    /* ---- ALOCACAO DO VETOR DE STRUCT ----
       Cada elemento ocupa sizeof(Pessoa) bytes.
       sizeof(Pessoa) = sizeof(char[50]) + sizeof(int) + sizeof(float)
                      (podendo haver padding de alinhamento). */
    p = (Pessoa *) malloc(n * sizeof(Pessoa));

    /* ---- VERIFICACAO DE NULL ---- */
    if (p == NULL) {
        puts("Erro: nao foi possivel alocar memoria para as pessoas.");
        return;
    }

    /* ---- CADASTRO: preenchemos cada struct do vetor ----
       Usamos fgets para ler o nome (mais seguro que gets, que e
       perigoso e removido do padrao C11). Depois removemos o '\n'
       final com strcspn. */
    for (i = 0; i < n; i++) {
        printf("\n--- Pessoa %d ---\n", i + 1);

        /* Le o nome com fgets (seguro: limita a leitura ao tamanho) */
        printf("Nome: ");
        fgets(p[i].nome, sizeof(p[i].nome), stdin);
        /* fgets guarda o '\n' do Enter no final da string.
           strcspn encontra a posicao do '\n' e substituimos por '\0'. */
        p[i].nome[strcspn(p[i].nome, "\n")] = '\0';

        /* Le a idade com scanf */
        printf("Idade: ");
        scanf("%d", &p[i].idade);
        limpar_buffer(); /* descarta '\n' deixado pelo scanf */

        /* Le a altura com scanf */
        printf("Altura (m): ");
        scanf("%f", &p[i].altura);
        limpar_buffer();
    }

    /* ---- LISTAGEM: mostra todas as pessoas cadastradas ---- */
    puts("\n===== LISTA DE PESSOAS CADASTRADAS =====");
    for (i = 0; i < n; i++) {
        printf("%d) Nome: %-15s | Idade: %3d | Altura: %.2f m\n",
               i + 1,
               p[i].nome,
               p[i].idade,
               p[i].altura);
    }

    /* ---- LIBERACAO ----
       Mesmo sendo struct, free libera o bloco inteiro de uma vez.
       Nao precisa liberar campo por campo (aqui todos os campos sao
       estaticos dentro da struct). */
    free(p);
    p = NULL;

    puts("\nMemoria do vetor de struct liberada com free.");
}

/* ===================================================================
   EXEMPLO 5: contar elementos de um vetor dinamico via sizeof
   -------------------------------------------------------------------
   Para um VETOR ESTATICO, sizeof(v)/sizeof(v[0]) da o numero de
   elementos. Mas para um VETOR DINAMICO (ponteiro alocado com malloc),
   sizeof(p) devolve o tamanho do PONTEIRO (8 bytes em 64 bits), NAO o
   tamanho do bloco. Por isso, para vetores dinamicos, devemos GUARDAR
   a quantidade em uma variavel separada. Este exemplo mostra a
   diferenca e como contar corretamente.
   =================================================================== */
static void exemplo_contar_elementos(void)
{
    int estatico[5];   /* vetor estaticico de 5 int (na pilha/stack)    */
    int *dinamico;    /* ponteiro para vetor dinamico (na heap)         */
    int qtd = 7;      /* quantidade que vamos alocar dinamicamente      */
    int i;            /* contador                                       */

    puts("\n--- EXEMPLO 5: contar elementos (sizeof) ---");

    /* Preenchemos o vetor estatico so para ilustrar. */
    for (i = 0; i < 5; i++) {
        estatico[i] = i + 1;
    }

    /* Alocamos o vetor dinamico com qtd elementos. */
    dinamico = (int *) malloc(qtd * sizeof(int));
    if (dinamico == NULL) {
        puts("Erro ao alocar vetor dinamico.");
        return;
    }
    for (i = 0; i < qtd; i++) {
        dinamico[i] = (i + 1) * 10;
    }

    /* ---- VETOR ESTATICO: sizeof funciona ----
       sizeof(estatico) = 5 * sizeof(int) = 20 bytes (se int=4).
       sizeof(estatico[0]) = sizeof(int) = 4 bytes.
       Dividindo: 20 / 4 = 5 elementos. Correto! */
    printf("Vetor ESTATICO:\n");
    printf("  sizeof(estatico)    = %zu bytes\n", sizeof(estatico));
    printf("  sizeof(estatico[0]) = %zu bytes\n", sizeof(estatico[0]));
    printf("  Numero de elementos = %zu\n",
           sizeof(estatico) / sizeof(estatico[0]));

    /* ---- VETOR DINAMICO: sizeof NAO funciona ----
       sizeof(dinamico) = tamanho do PONTEIRO = 8 bytes (64 bits)
                          ou 4 bytes (32 bits). NAO e o tamanho do bloco!
       Por isso a divisao abaixo da um valor ERRADO (1 ou 0), nao 7.
       Licao: para vetor dinamico, GUARDE a quantidade em variavel.

       NOTA: o compilador emite warning -Wsizeof-pointer-div nesta linha
       porque detecta a divisao sizeof(ponteiro)/sizeof(elemento). Esse
       warning e PROPOSITAL aqui: ele prova exatamente o ponto da licao
       (que a formula NAO serve para vetor dinamico). Suprimimos o warning
       com pragma para a compilacao ficar limpa com -Wall -Wextra. */
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsizeof-pointer-div"
#endif
    printf("\nVetor DINAMICO (ponteiro alocado com malloc):\n");
    printf("  sizeof(dinamico)    = %zu bytes (tamanho do PONTEIRO!)\n",
           sizeof(dinamico));
    printf("  sizeof(dinamico[0]) = %zu bytes\n", sizeof(dinamico[0]));
    printf("  Divisao (ERRADA!)   = %zu  <-- nao confiar nisto\n",
           sizeof(dinamico) / sizeof(dinamico[0]));
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
    printf("  Quantidade REAL     = %d  <-- guardamos em variavel\n", qtd);

    /* ---- LIBERACAO ---- */
    free(dinamico);
    dinamico = NULL;

    puts("\nConclusao: para vetor dinamico, SEMPRE guarde a quantidade");
    puts("em uma variavel separada. sizeof so funciona para vetor estatico.");
}

/* ===================================================================
   EXEMPLO 6: free correto e cuidados (double free, dangling pointer)
   -------------------------------------------------------------------
   Erros comuns com free:
     1) ESQUECER o free -> vazamento de memoria (memory leak).
     2) Chamar free DUAS VEZES no mesmo ponteiro -> "double free",
        comportamento indefinido (crash possivel).
     3) Usar o ponteiro DEPOIS do free -> "dangling pointer" (ponteiro
        pendurado), leitura/escrita em memoria liberada -> bug grave.
     4) Chamar free em ponteiro que NAO foi alocado com malloc/calloc/
        realloc -> comportamento indefinido.
   Solucao: apos free, atribuir NULL ao ponteiro. free(NULL) e seguro
   (nao faz nada), o que evita o double free.
   =================================================================== */
static void exemplo_free_correto(void)
{
    int *a; /* ponteiro que vamos alocar e liberar corretamente */
    int *b; /* ponteiro para mostrar o perigo do double free    */

    puts("\n--- EXEMPLO 6: free correto e cuidados ---");

    /* ---- Caso 1: alocar, usar e liberar corretamente ---- */
    a = (int *) malloc(sizeof(int)); /* aloca 1 unico int */
    if (a == NULL) {
        puts("Erro ao alocar 'a'.");
        return;
    }
    *a = 42; /* atribui valor via dereferencia do ponteiro */
    printf("Valor de *a antes do free: %d\n", *a);

    free(a);     /* libera a memoria                          */
    a = NULL;    /* anula o ponteiro -> evita uso acidental   */

    /* Agora a == NULL. Se tentarmos free(a) de novo, e seguro:
       free(NULL) nao faz nada (definido pelo padrao C). */
    free(a); /* seguro: a e NULL, free(NULL) e no-op */
    puts("free(a) chamado 2x sem problema porque a foi anulado para NULL.");

    /* ---- Caso 2: double free PERIGOSO (NAO fazer isto!) ----
       Aqui alocamos b, liberamos, e mostramos o que NAO fazer.
       Nao executamos o double free de verdade para nao crashar. */
    b = (int *) malloc(sizeof(int));
    if (b == NULL) {
        puts("Erro ao alocar 'b'.");
        return;
    }
    *b = 99;
    printf("Valor de *b antes do free: %d\n", *b);

    free(b);
    /* b ainda guarda o endereco antigo (dangling pointer).
       Se fizessemos free(b) de novo AQUI (sem anular), seria
       double free -> CRASH possivel. Por isso SEMPRE anule: */
    b = NULL;

    puts("\nRegras de ouro do free:");
    puts(" 1) Sempre de free em toda memoria alocada com malloc/calloc/realloc.");
    puts(" 2) Apos free, atribua NULL ao ponteiro (evita dangling e double free).");
    puts(" 3) free(NULL) e seguro e nao faz nada - aproveite isso.");
    puts(" 4) Nunca de free em ponteiro que nao veio de malloc/calloc/realloc.");
    puts(" 5) Nunca use o ponteiro apos free (dangling pointer).");
}
