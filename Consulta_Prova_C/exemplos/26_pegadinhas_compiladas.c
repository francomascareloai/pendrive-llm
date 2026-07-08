/*
 ============================================================================
 ARQUIVO    : 26_pegadinhas_compiladas.c
 DESCRICAO  : Programa didatico que demonstra as pegadinhas mais comuns
              que caem em prova de C. Cada opcao do menu mostra UMA
              pegadinha com o codigo ERRADO (comentado ou demonstrado) e
              o codigo CERTO (ativo). Feito para o aluno ver na pratica
              o que acontece e por que cada erro e um erro.
 TOPICO     : Pegadinhas / armadilhas da linguagem C
 PALAVRAS-CHAVE (Ctrl-F): pegadinha, armadilha, = vs ==, atribuicao,
              comparacao, & no scanf, strcmp, if strcmp, divisao inteira,
              break switch, fall through, indice limite, vetor fora limite,
              buffer newline, ponto virgula if, while feof, off by one,
              float comparacao, struct comparacao, conio, menu switch,
              fflush stdin, fgets strcspn, typedef struct
 COMPILACAO : gcc -Wall -Wextra 26_pegadinhas_compiladas.c -o pegadinhas.exe
 ============================================================================
*/

/* ---- BIBLIOTECAS ---- */
#include <stdio.h>   /* printf, scanf, fgets, fopen, fscanf, fclose, etc.  */
#include <string.h>  /* strcmp, strcspn, strlen                              */
#include <stdlib.h>  /* system (cls, pause), remove                         */
#include <conio.h>   /* getch, getche - especifica do Windows/MinGW         */

/*
 * typedef struct COM ALIAS (forma correta que o professor exige).
 * Usamos esta struct na Pegadinha 13 (comparacao de struct com ==).
 * O alias "Aluno" permite declarar "Aluno a1;" em vez de
 * "struct aluno a1;" -- e isso que cai na prova.
 */
typedef struct {
    int   matricula;  /* numero de matricula do aluno            */
    char  nome[50];   /* nome do aluno (vetor de chars = string) */
    float nota;       /* nota final (0.0 a 10.0)                */
} Aluno;  /* <-- alias: agora "Aluno" e um tipo, nao precisa de "struct" */

/* ---- PROTOTIPOS DAS FUNCOES ---- */
/* Cada funcao demonstra UMA pegadinha. */
void pegadinha_01_atribuicao_vs_comparacao(void);
void pegadinha_02_e_comercial_scanf(void);
void pegadinha_03_comparacao_strings(void);
void pegadinha_04_retorno_strcmp(void);
void pegadinha_05_divisao_inteira(void);
void pegadinha_06_break_switch(void);
void pegadinha_07_indice_limite(void);
void pegadinha_08_newline_buffer(void);
void pegadinha_09_ponto_virgula_if(void);
void pegadinha_10_while_feof(void);
void pegadinha_11_off_by_one(void);
void pegadinha_12_float_comparacao(void);
void pegadinha_13_struct_comparacao(void);

/* ---- FUNCOES AUXILIARES ---- */
void cabecalho(int numero, const char *titulo);
void pausar(void);
void limpar_buffer_portatil(void);
int  ler_inteiro(const char *mensagem);

/*
 ============================================================================
 FUNCAO PRINCIPAL
 ============================================================================
*/
int main(void) {
    int opcao;  /* guarda a opcao escolhida pelo usuario no menu */

    /*
     * Menu classico do professor: do-while + switch.
     * O do-while garante que o menu aparece PELO MENOS uma vez.
     * O switch direciona para a funcao da pegadinha escolhida.
     */
    do {
        system("cls");  /* limpa a tela (Windows) */

        /* Cabecalho do menu */
        printf("========================================\n");
        printf("   PEGADINHAS DA LINGUAGEM C - PROVA GB\n");
        printf("========================================\n\n");

        printf("01. Atribuicao (=) vs Comparacao (==)\n");
        printf("02. & esquecido no scanf\n");
        printf("03. Comparar strings com ==\n");
        printf("04. if(strcmp(a,b)) - retorno confuso\n");
        printf("05. Divisao inteira (5/2 = 2!)\n");
        printf("06. break esquecido no switch\n");
        printf("07. Indice fora do limite do vetor\n");
        printf("08. \\n deixado no buffer pelo scanf\n");
        printf("09. Ponto e virgula depois do if\n");
        printf("10. while(!feof) - leitura duplicada\n");
        printf("11. Off-by-one: <= vs < no for\n");
        printf("12. Comparacao de float com ==\n");
        printf("13. Comparacao de struct com ==\n");
        printf(" 0. Sair\n\n");

        printf("Escolha uma opcao: ");

        /* Le a opcao do usuario. scanf retorna o numero de itens lidos
         * com sucesso. Se o usuario digitar uma letra, scanf falha
         * (retorna 0) e opcao recebe -1 para cair no default. */
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;  /* entrada invalida: forca cair no default */
        }

        /* fflush(stdin): LIMPA o buffer de entrada (remove o \n que
         * o scanf deixou). NAO e portavel -- o padrao C da ISO diz que
         * fflush so e definido para fluxos de SAIDA (stdout, arquivos).
         * Porem, no Windows com MinGW/MSVC, fflush(stdin) FUNCIONA e
         * e o que o professor usa. Alternativa 100% portavel:
         *   int c; while ((c=getchar()) != '\n' && c != EOF);
         * (veja a funcao limpar_buffer_portatil abaixo). */
        fflush(stdin);

        /* Switch que direciona para a pegadinha escolhida.
         * Cada case chama UMA funcao e depois break. */
        switch (opcao) {
            case 1:  pegadinha_01_atribuicao_vs_comparacao(); break;
            case 2:  pegadinha_02_e_comercial_scanf();        break;
            case 3:  pegadinha_03_comparacao_strings();        break;
            case 4:  pegadinha_04_retorno_strcmp();           break;
            case 5:  pegadinha_05_divisao_inteira();          break;
            case 6:  pegadinha_06_break_switch();              break;
            case 7:  pegadinha_07_indice_limite();             break;
            case 8:  pegadinha_08_newline_buffer();            break;
            case 9:  pegadinha_09_ponto_virgula_if();          break;
            case 10: pegadinha_10_while_feof();                break;
            case 11: pegadinha_11_off_by_one();                break;
            case 12: pegadinha_12_float_comparacao();          break;
            case 13: pegadinha_13_struct_comparacao();         break;
            case 0:
                printf("\nSaindo... Boa sorte na prova!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

        /* Depois de cada pegadinha (exceto Sair), pausa para o aluno
         * ler o resultado antes de limpar a tela. */
        if (opcao != 0) {
            pausar();
        }

    } while (opcao != 0);  /* repete ate o usuario digitar 0 */

    /* Mensagem final antes de encerrar. getch() espera uma tecla. */
    printf("\nPressione qualquer tecla para encerrar...\n");
    getch();  /* conio.h: le um caractere sem ecoar (sem precisar Enter) */

    return 0;  /* retorno 0 = programa terminou com sucesso */
}

/*
 ============================================================================
 FUNCOES AUXILIARES
 ============================================================================
*/

/* Exibe um cabecalho padronizado para cada pegadinha.
 * Recebe o numero da pegadinha e o titulo descritivo. */
void cabecalho(int numero, const char *titulo) {
    printf("\n");
    printf("========================================\n");
    printf("  PEGADINHA %02d: %s\n", numero, titulo);
    printf("========================================\n\n");
}

/* Pausa a execucao para o aluno ler a saida.
 * system("pause") e do Windows (mostra "Pressione qualquer tecla..."). */
void pausar(void) {
    printf("\n");
    system("pause");  /* Windows: pausa e espera tecla */
}

/* Limpa o buffer de entrada de forma PORTAVEL (funciona em qualquer SO).
 * Le e descarta todos os caracteres ate encontrar \n ou EOF.
 * Esta e a alternativa correta ao fflush(stdin). */
void limpar_buffer_portatil(void) {
    int c;  /* variavel int (nao char!) porque EOF e int (-1) */
    /* Descarta cada caractere ate achar \n (Enter) ou EOF (fim) */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* corpo vazio: so queremos descartar os caracteres */
    }
}

/* Le um inteiro do teclado de forma SEGURA.
 * Usa fgets (nao gets!) + sscanf para evitar estouro de buffer.
 * Retorna o inteiro digitado (ou 0 se entrada invalida). */
int ler_inteiro(const char *mensagem) {
    char linha[20];  /* buffer para a linha digitada */
    int  valor = 0;  /* valor a ser retornado */

    printf("%s", mensagem);  /* exibe a mensagem (ex: "Digite x: ") */

    /* fgets le ate 19 caracteres (deixa espaco para o \0).
     * NUNCA use gets() -- foi removida do padrao C11 por ser perigosa. */
    if (fgets(linha, sizeof(linha), stdin) != NULL) {
        /* strcspn encontra a posicao do \n e substitui por \0.
         * Isso remove o Enter que o fgets captura. */
        linha[strcspn(linha, "\n")] = '\0';
        /* sscanf extrai o inteiro da string. Retorna 1 se leu com sucesso. */
        if (sscanf(linha, "%d", &valor) != 1) {
            valor = 0;  /* se nao leu um numero, retorna 0 */
        }
    }

    return valor;
}

/*
 ============================================================================
 PEGADINHA 01: ATRIBUICAO (=) vs COMPARACAO (==)
 ============================================================================
 O erro classico: usar = (um sinal) em vez de == (dois sinais) no if.
 =  -> ATRIBUI: x = 5  significa "x recebe 5"
 == -> COMPARA: x == 5 significa "x e igual a 5?"

 Se voce escreve if (x = 5), o C:
   1. ATRIBUI 5 a x (x agora vale 5, nao importa o que tinha antes)
   2. Avalia o RESULTADO da atribuicao (que e 5)
   3. Como 5 != 0, o if e SEMPRE VERDADEIRO

 Resultado: o if sempre executa, e x e modificado!
 ============================================================================
*/
void pegadinha_01_atribuicao_vs_comparacao(void) {
    int x = 10;  /* x comeca valendo 10 */

    cabecalho(1, "ATRIBUICAO (=) vs COMPARACAO (==)");

    printf("x foi inicializado com %d\n\n", x);

    /* ---- ERRADO (comentado para nao compilar) ---- */
    /* if (x = 5) {
    *     printf("x e igual a 5\n");
    * }
    *
    * O QUE ACONTECE:
    * - O = (um sinal) ATRIBUI 5 a x. Agora x vale 5.
    * - O resultado da expressao "x = 5" e 5.
    * - Como 5 != 0, o if e VERDADEIRO.
    * - O printf executa, mas x foi MODIFICADO de 10 para 5!
    * - O gcc -Wall avisa: "suggest parentheses around assignment"
    *   used as truth value" -- preste atencao nos warnings!
    */

    /* ---- CERTO ---- */
    /* == (dois sinais) COMPARA sem modificar x. */
    if (x == 5) {
        /* so executa se x for realmente 5 */
        printf("x e igual a 5\n");
    } else {
        /* como x vale 10, cai aqui */
        printf("x NAO e igual a 5 (x = %d)\n", x);
    }

    printf("\nRESUMO:\n");
    printf("  =  -> ATRIBUI (x = 5: x recebe 5)\n");
    printf("  == -> COMPARA (x == 5: x e igual a 5?)\n");
    printf("  if (x = 5)  -> SEMPRE VERDADEIRO e modifica x!\n");
    printf("  if (x == 5) -> so e verdadeiro se x for 5\n");
}

/*
 ============================================================================
 PEGADINHA 02: & ESQUECIDO NO SCANF
 ============================================================================
 scanf precisa do ENDERECO da variavel (onde ela esta na memoria),
 nao do seu valor. O & (e comercial) obtem o endereco.

 ERRADO: scanf("%d", num);   -> passa o VALOR (lixo de endereco)
 CERTO: scanf("%d", &num);   -> passa o ENDERECO de num

 Sem o &, o scanf tenta escrever num endereco invalido -> CRASH (segfault).
 ============================================================================
*/
void pegadinha_02_e_comercial_scanf(void) {
    int num;  /* variavel que vai receber o valor digitado */

    cabecalho(2, "& ESQUECIDO NO SCANF");

    printf("Cada variavel na memoria tem um ENDERECO.\n");
    printf("scanf precisa saber ONDE gravar, por isso usa &\n\n");

    printf("Endereco de num: %p\n\n", (void *)&num);

    /* ---- ERRADO (comentado para nao dar crash) ---- */
    /* scanf("%d", num);
    *
    * O QUE ACONTECE:
    * - Sem o &, voce passa o VALOR de num (que e lixo nao inicializado).
    * - O scanf interpreta esse lixo como um ENDERECO de memoria.
    * - Tenta escrever nesse endereco invalido -> CRASH (segfault).
    * - O gcc -Wall avisa: "format '%d' expects type 'int *'"
    *   "but argument has type 'int'" -- LEIA OS WARNINGS!
    */

    /* ---- CERTO ---- */
    /* O & obtem o endereco de num. scanf grava nesse endereco. */
    num = ler_inteiro("Digite um numero (com & no scanf): ");

    printf("\nVoce digitou: %d\n", num);
    printf("scanf gravou no endereco %p\n", (void *)&num);

    printf("\nRESUMO:\n");
    printf("  scanf(\"%%d\",  num);  -> ERRADO: passa valor (crash!)\n");
    printf("  scanf(\"%%d\", &num);  -> CERTO: passa endereco\n");
    printf("  EXCECAO: strings (vetores) NAO precisam de &\n");
    printf("    porque o nome do vetor JA E um endereco.\n");
    printf("    Ex: char s[50]; scanf(\"%%s\", s);  <- sem &\n");
}

/*
 ============================================================================
 PEGADINHA 03: COMPARAR STRINGS COM ==
 ============================================================================
 Em C, strings sao VETORES de char. O nome do vetor e o ENDERECO
 do primeiro caractere. Comparar com == compara ENDERECOS, nao conteudo.

 ERRADO: if (s1 == s2)     -> compara enderecos de memoria
 CERTO: if (strcmp(s1, s2) == 0)  -> compara caractere por caractere

 Duas strings com o mesmo conteudo em enderecos diferentes
 sao consideradas DIFERENTES por ==.
 ============================================================================
*/
void pegadinha_03_comparacao_strings(void) {
    /* Dois vetores separados, mesmo conteudo, enderecos diferentes */
    char s1[] = "abc";  /* s1 esta em um endereco de memoria */
    char s2[] = "abc";  /* s2 esta em OUTRO endereco */

    cabecalho(3, "COMPARAR STRINGS COM ==");

    printf("s1 = \"%s\" (endereco: %p)\n", s1, (void *)s1);
    printf("s2 = \"%s\" (endereco: %p)\n\n", s2, (void *)s2);

    /* ---- ERRADO (mas compila! so da resultado errado) ---- */
    /* s1 e s2 sao enderecos diferentes, entao == da FALSO
     * mesmo o conteudo sendo identico ("abc" == "abc").
     *
     * O pragma abaixo suprime o warning -Warray-compare do gcc
     * APENAS nesta linha, porque comparar arrays com == e PROPOSITAL
     * aqui: e justamente a pegadinha que estamos demonstrando.
     * Sem o pragma, o gcc -Wall -Wextra avisa "comparison between
     * two arrays" -- o que e correto, mas queremos mostrar o erro. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-compare"
    if (s1 == s2) {
        printf("[ERRADO] s1 == s2: IGUAIS\n");
    } else {
        printf("[ERRADO] s1 == s2: DIFERENTES (mas o conteudo e igual!)\n");
    }
#pragma GCC diagnostic pop

    printf("\n");

    /* ---- CERTO ---- */
    /* strcmp compara caractere por caractere.
     * Retorna 0 se forem iguais. */
    if (strcmp(s1, s2) == 0) {
        printf("[CERTO]  strcmp(s1, s2) == 0: IGUAIS\n");
    } else {
        printf("[CERTO]  strcmp(s1, s2) == 0: DIFERENTES\n");
    }

    printf("\nRESUMO:\n");
    printf("  s1 == s2            -> compara ENDERECOS (quase sempre falso)\n");
    printf("  strcmp(s1,s2) == 0  -> compara CONTEUDO (o certo!)\n");
    printf("  strcmp retorna:\n");
    printf("    0  -> strings iguais\n");
    printf("    <0 -> s1 vem antes de s2\n");
    printf("    >0 -> s1 vem depois de s2\n");
}

/*
 ============================================================================
 PEGADINHA 04: if(strcmp(a,b)) - RETORNO CONFUSO
 ============================================================================
 strcmp retorna 0 (ZERO) quando as strings sao IGUAIS.
 Em C, 0 significa FALSO. Entao:

   if (strcmp(s1, s2))  ->  VERDADEIRO quando as strings sao DIFERENTES!
                            (porque strcmp retorna != 0 quando diferentes)

 O aluno acha que if(strcmp) e verdadeiro quando sao iguais,
 mas e o OPOSTO. Esta e uma das pegadinhas mais cobradas em prova.

 CERTO: if (strcmp(s1, s2) == 0)  -> verdadeiro quando IGUAIS
 ============================================================================
*/
void pegadinha_04_retorno_strcmp(void) {
    char senha_digitada[] = "1234";
    char senha_correta[]   = "1234";

    cabecalho(4, "if(strcmp(a,b)) - RETORNO CONFUSO");

    printf("senha_digitada = \"%s\"\n", senha_digitada);
    printf("senha_correta   = \"%s\"\n\n", senha_correta);

    printf("strcmp(senha_digitada, senha_correta) = %d\n\n",
           strcmp(senha_digitada, senha_correta));

    /* ---- ERRADO (logica invertida!) ---- */
    /* O aluno pensa: "se strcmp retornar algo, sao iguais"
     * Mas strcmp retorna 0 para iguais, e 0 = FALSO em C.
     * Entao if(strcmp) executa quando sao DIFERENTES. */
    if (strcmp(senha_digitada, senha_correta)) {
        /* isso executa quando as strings sao DIFERENTES */
        printf("[ERRADO] if(strcmp): \"senhas iguais\" -> MAS NAO ESTAO!\n");
        printf("         (strcmp retornou != 0, que e VERDADEIRO)\n");
    } else {
        /* isso executa quando as strings sao IGUAIS */
        printf("[ERRADO] if(strcmp): \"senhas diferentes\" -> MAS SAO IGUAIS!\n");
        printf("         (strcmp retornou 0, que e FALSO)\n");
    }

    printf("\n");

    /* ---- CERTO ---- */
    /* Comparar explicitamente com 0: strcmp == 0 significa IGUAIS. */
    if (strcmp(senha_digitada, senha_correta) == 0) {
        printf("[CERTO]  strcmp == 0: Senhas IGUAIS (acesso liberado)\n");
    } else {
        printf("[CERTO]  strcmp == 0: Senhas DIFERENTES (acesso negado)\n");
    }

    printf("\nRESUMO:\n");
    printf("  strcmp retorna 0  -> strings IGUAIS\n");
    printf("  Em C, 0 = FALSO. Entao:\n");
    printf("  if (strcmp(a,b))       -> VERDADEIRO quando DIFERENTES\n");
    printf("  if (strcmp(a,b) == 0)  -> VERDADEIRO quando IGUAIS (certo!)\n");
    printf("  DICA: sempre compare strcmp com == 0 ou != 0\n");
}

/*
 ============================================================================
 PEGADINHA 05: DIVISAO INTEIRA
 ============================================================================
 Em C, quando os DOIS operandos sao int, a divisao e INTEIRA
 (trunca as casas decimais). 5 / 2 = 2 (nao 2.5!).

 Para obter divisao real, PELO MENOS UM operando deve ser float/double.

 ERRADO: float r = 5 / 2;        -> r = 2.0 (divisao inteira primeiro!)
 CERTO: float r = (float)5 / 2;  -> r = 2.5 (cast antes da divisao)
 ============================================================================
*/
void pegadinha_05_divisao_inteira(void) {
    int a = 5;
    int b = 2;

    cabecalho(5, "DIVISAO INTEIRA (5/2 = 2!)");

    printf("a = %d, b = %d\n\n", a, b);

    /* ---- ERRADO ---- */
    /* a / b com os dois int -> divisao INTEIRA: 5/2 = 2 (trunca) */
    int resultado_int = a / b;
    printf("[ERRADO] int r = a / b;        -> r = %d (truncou!)\n",
           resultado_int);

    /* ---- AINDA ERRADO (surpresa!) ---- */
    /* Mesmo guardando em float, a divisao a/b e inteira PRIMEIRO,
     * so DEPOIS o resultado (2) e convertido para float (2.0). */
    float resultado_float_errado = a / b;
    printf("[ERRADO] float r = a / b;      -> r = %.1f (ainda inteiro!)\n",
           resultado_float_errado);

    /* ---- CERTO (opcao 1: cast) ---- */
    /* (float)a converte a para 5.0 ANTES da divisao.
     * Agora um operando e float -> divisao real: 5.0/2 = 2.5 */
    float resultado_cast = (float)a / b;
    printf("[CERTO] float r = (float)a / b; -> r = %.1f (divisao real)\n",
           resultado_cast);

    /* ---- CERTO (opcao 2: cast no b) ---- */
    float resultado_cast2 = a / (float)b;
    printf("[CERTO] float r = a / (float)b; -> r = %.1f (tambem funciona)\n",
           resultado_cast2);

    /* ---- CERTO (opcao 3: literal float) ---- */
    /* 2.0f e um literal float. a / 2.0f -> divisao real. */
    float resultado_literal = a / 2.0f;
    printf("[CERTO] float r = a / 2.0f;     -> r = %.1f (literal float)\n",
           resultado_literal);

    printf("\nRESUMO:\n");
    printf("  int / int     -> divisao INTEIRA (trunca decimais)\n");
    printf("  float / int   -> divisao REAL (um ja basta)\n");
    printf("  Para calcular media: (float)soma / quantidade\n");
    printf("  NUNCA: soma / quantidade (da inteiro se ambos forem int)\n");
}

/*
 ============================================================================
 PEGADINHA 06: BREAK ESQUECIDO NO SWITCH
 ============================================================================
 O switch executa o case correspondente E TODOS OS CASES ABAIXO
 se voce nao colocar break. Isso se chama FALL-THROUGH.

 ERRADO: esquecer o break -> executa todos os cases seguintes
 CERTO: colocar break no final de cada case

 O fall-through so e utilil quando voce QUER que varios cases
 facam a mesma coisa (cases agrupados sem codigo entre eles).
 ============================================================================
*/
void pegadinha_06_break_switch(void) {
    int dia = 2;  /* terca-feira (1=domingo, 2=segunda, etc.) */

    cabecalho(6, "BREAK ESQUECIDO NO SWITCH");

    printf("dia = %d\n\n", dia);

    /* ---- ERRADO (sem break) ---- */
    /* Como nao tem break, apos executar case 2, o C continua
     * executando case 3, case 4, etc. (fall-through). */
    printf("--- SEM break (ERRADO) ---\n");
    switch (dia) {
        case 1: printf("Domingo\n");   /* fall through */
        case 2: printf("Segunda\n");   /* fall through */
        case 3: printf("Terca\n");     /* fall through */
        case 4: printf("Quarta\n");    /* fall through */
        case 5: printf("Quinta\n");    /* fall through */
        case 6: printf("Sexta\n");    /* fall through */
        case 7: printf("Sabado\n");    /* fall through */
        default: printf("Dia invalido\n");
    }
    /* Saida: Segunda, Terca, Quarta, Quinta, Sexta, Sabado, Dia invalido
     * Tudo isso porque nao tem break! */

    printf("\n");

    /* ---- CERTO (com break) ---- */
    /* O break faz sair do switch imediatamente apos o case. */
    printf("--- COM break (CERTO) ---\n");
    switch (dia) {
        case 1: printf("Domingo\n");   break;
        case 2: printf("Segunda\n");   break;  /* so este executa */
        case 3: printf("Terca\n");     break;
        case 4: printf("Quarta\n");    break;
        case 5: printf("Quinta\n");    break;
        case 6: printf("Sexta\n");    break;
        case 7: printf("Sabado\n");    break;
        default: printf("Dia invalido\n"); break;
    }
    /* Saida: Segunda (apenas este!) */

    printf("\nRESUMO:\n");
    printf("  Sem break: executa o case E TODOS os seguintes (fall-through)\n");
    printf("  Com break: executa SO o case correspondente e sai\n");
    printf("  EXCECAO: cases agrupados (sem codigo entre eles) e intencional:\n");
    printf("    case 1: case 7: printf(\"Fim de semana\"); break;\n");
}

/*
 ============================================================================
 PEGADINHA 07: INDICE FORA DO LIMITE DO VETOR
 ============================================================================
 Vetores em C sao indexados de 0 a n-1.
 Um vetor de 5 elementos tem indices 0, 1, 2, 3, 4.
 NAO existe o indice 5!

 ERRADO: for (i = 0; i <= 5; i++) -> acessa v[5] (fora do limite!)
 CERTO: for (i = 0; i < 5; i++)   -> acessa v[0] a v[4]

 C NAO verifica limites de vetor. Acessar fora do limite e
 "comportamento indefinido" (undefined behavior): pode dar lixo,
 crash, ou aparentemente funcionar (pior caso, pois esconde o bug).
 ============================================================================
*/
void pegadinha_07_indice_limite(void) {
    /* Vetor de 5 elementos: indices validos sao 0 a 4 */
    int v[5] = {10, 20, 30, 40, 50};
    int i;
    int soma = 0;

    cabecalho(7, "INDICE FORA DO LIMITE DO VETOR");

    printf("Vetor de 5 elementos: {10, 20, 30, 40, 50}\n");
    printf("Indices validos: 0, 1, 2, 3, 4\n");
    printf("Indice 5 NAO EXISTE (e comportamento indefinido!)\n\n");

    /* ---- ERRADO (comentado para nao causar problemas) ---- */
    /* for (i = 0; i <= 5; i++) {
    *     soma += v[i];
    * }
    *
    * O QUE ACONTECE:
    * - Quando i = 5, acessa v[5] que esta FORA do vetor.
    * - C nao verifica limites: le a memoria apos o vetor (lixo).
    * - Pode dar resultado errado, crash, ou aparentemente funcionar.
    * - O gcc -Wall NAO avisa sobre isso (nao da para saber em tempo
    *   de compilacao se o indice estara fora do limite).
    */

    /* ---- CERTO ---- */
    /* i < 5 garante que i vai de 0 a 4 (os 5 indices validos). */
    for (i = 0; i < 5; i++) {
        printf("  v[%d] = %d\n", i, v[i]);
        soma += v[i];  /* acumula a soma */
    }

    printf("\nSoma = %d (10+20+30+40+50 = 150)\n", soma);

    printf("\nRESUMO:\n");
    printf("  Vetor de tamanho N -> indices de 0 a N-1\n");
    printf("  for (i = 0; i <  N; i++) -> CERTO (0 a N-1)\n");
    printf("  for (i = 0; i <= N; i++) -> ERRADO (acessa v[N] = lixo!)\n");
    printf("  C NAO verifica limites em tempo de execucao.\n");
    printf("  E responsabilidade do programador garantir os limites.\n");
}

/*
 ============================================================================
 PEGADINHA 08: \n DEIXADO NO BUFFER PELO SCANF
 ============================================================================
 Quando voce digita "20" e Enter, o scanf le "20" mas o \n (Enter)
 FICA no buffer de entrada. Se a proxima leitura for fgets,
 ela le o \n imediatamente e para (string vazia).

 ERRADO: scanf("%d", &n); fgets(nome, 50, stdin);
         -> fgets le o \n deixado pelo scanf e retorna string vazia!

 CERTO: limpar o buffer entre o scanf e o fgets.
        fflush(stdin) no Windows, ou while(getchar()...) portatil.
 ============================================================================
*/
void pegadinha_08_newline_buffer(void) {
    int  idade;
    char nome[50];

    cabecalho(8, "\\n DEIXADO NO BUFFER PELO SCANF");

    printf("Esta pegadinha requer interacao. Siga as instrucoes.\n\n");

    /* Le a idade com scanf. O scanf le o numero, mas o \n (Enter)
     * fica no buffer de entrada. */
    printf("Digite sua idade: ");
    if (scanf("%d", &idade) != 1) {
        idade = 0;  /* se entrada invalida, idade = 0 */
    }

    /* ---- ERRADO (comentado para demonstrar o conceito) ---- */
    /* Se fizéssemos fgets AQUI, sem limpar o buffer:
    *
    *   fgets(nome, 50, stdin);
    *
    * O fgets encontraria o \n deixado pelo scanf e leria apenas
    * o \n, parando imediatamente. A string nome ficaria vazia
    * (so continha \n). O programa pularia a entrada do nome!
    *
    * Isso e uma das pegadinhas mais comuns em prova pratica.
    */

    /* ---- CERTO ---- */
    /* Limpa o buffer ANTES de usar fgets.
     * fflush(stdin) NAO e portavel pelo padrao ISO C, mas funciona
     * no Windows com MinGW. O professor usa esta forma. */
    fflush(stdin);

    /* Alternativa 100% portavel (descomente para usar):
    * limpar_buffer_portatil();
    */

    /* Agora o buffer esta limpo. fgets vai esperar o usuario digitar. */
    printf("Digite seu nome: ");
    if (fgets(nome, sizeof(nome), stdin) != NULL) {
        /* Remove o \n que o fgets captura no final da string.
         * strcspn encontra a posicao do \n. Substituimos por \0. */
        nome[strcspn(nome, "\n")] = '\0';
    }

    printf("\nResultado:\n");
    printf("  Idade: %d\n", idade);
    printf("  Nome:  %s\n", nome);

    printf("\nRESUMO:\n");
    printf("  scanf deixa o \\n no buffer de entrada.\n");
    printf("  Se a proxima leitura for fgets, ela le o \\n e para.\n");
    printf("  SOLUCAO: limpar o buffer entre scanf e fgets.\n");
    printf("    fflush(stdin);  -> Windows (nao portavel mas funciona)\n");
    printf("    while((c=getchar())!='\\n'&&c!=EOF);  -> portavel\n");
    printf("  DICA: fgets NAO deixa \\n no buffer (ele captura o \\n).\n");
}

/*
 ============================================================================
 PEGADINHA 09: PONTO E VIRGULA DEPOIS DO IF
 ============================================================================
 Um ponto e virgula (;) apos o if TERMINA o if (corpo vazio).
 O bloco { ... } que segue executa SEMPRE, independente da condicao.

 ERRADO: if (x == 10); { printf("dez"); }
         -> o ; termina o if (corpo vazio, nao faz nada)
         -> o bloco { printf("dez"); } executa SEMPRE

 CERTO: if (x == 10) { printf("dez"); }
         -> sem o ; o bloco so executa se a condicao for verdadeira

 O gcc -Wall avisa: "empty body in an if-statement"
 ============================================================================
*/
void pegadinha_09_ponto_virgula_if(void) {
    int x = 5;  /* x vale 5, NAO vale 10 */

    cabecalho(9, "PONTO E VIRGULA DEPOIS DO IF");

    printf("x = %d (NAO e 10)\n\n", x);

    /* ---- ERRADO (comentado para nao compilar com warning) ---- */
    /* if (x == 10); {
    *     printf("x e 10!\n");
    * }
    *
    * O QUE ACONTECE:
    * - O ; apos if(x == 10) termina o if com corpo VAZIO.
    * - O if avalia a condicao (5 == 10 = falso), mas nao faz nada.
    * - O bloco { printf("x e 10!\n"); } NAO esta dentro do if.
    * - Ele executa SEMPRE, independente da condicao!
    * - Resultado: imprime "x e 10!" mesmo x sendo 5.
    * - O gcc -Wall avisa: "empty body in an if-statement"
    */

    /* ---- CERTO ---- */
    /* Sem o ; o bloco esta DENTRO do if. So executa se x == 10. */
    if (x == 10) {
        printf("[CERTO] x e 10!\n");  /* nao executa (x = 5) */
    }
    printf("[CERTO] x nao e 10 (x = %d)\n", x);

    printf("\nRESUMO:\n");
    printf("  if (cond);  -> ; termina o if (corpo vazio)\n");
    printf("  O bloco { } seguinte executa SEMPRE (nao e do if)\n");
    printf("  if (cond)   -> sem ; o bloco so executa se cond for verdadeira\n");
    printf("  O mesmo vale para while, for: NUNCA coloque ; antes do bloco\n");
    printf("  while(1); { ... }  -> loop infinito de corpo vazio + bloco unico\n");
}

/*
 ============================================================================
 PEGADINHA 10: while(!feof) - LEITURA DUPLICADA
 ============================================================================
 feof() so retorna VERDADEIRO DEPOIS de uma tentativa de leitura
 que passou do fim do arquivo. Se voce usa while(!feof) e le
 DENTRO do loop, o ultimo registro valido e lido, mas feof ainda
 e falso, entao o loop continua, tenta ler de novo (falha), e
 processa o registro anterior NOVAMENTE (duplicado).

 ERRADO:
   while (!feof(f)) {
       fscanf(f, "%d", &num);
       printf("%d\n", num);  // ultimo numero imprime 2x!
   }

 CERTO:
   while (fscanf(f, "%d", &num) == 1) {
       printf("%d\n", num);  // so processa se leu com sucesso
   }

 A regra de ouro: SEMPRE teste o RETORNO da funcao de leitura
 (fscanf, fread, fgets), nao use feof no while.
 ============================================================================
*/
void pegadinha_10_while_feof(void) {
    FILE *f;        /* ponteiro para o arquivo */
    int   num;      /* numero lido do arquivo */
    char  linha[100];

    cabecalho(10, "while(!feof) - LEITURA DUPLICADA");

    /* Cria um arquivo temporario com 3 numeros (um por linha) */
    f = fopen("temp_pegadinha.txt", "w");
    if (f == NULL) {
        printf("ERRO: nao conseguiu criar o arquivo temporario.\n");
        return;  /* sai da funcao se nao conseguiu criar */
    }
    fprintf(f, "10\n20\n30\n");  /* escreve 3 numeros */
    fclose(f);  /* fecha o arquivo de escrita */

    printf("Arquivo criado com: 10, 20, 30\n\n");

    /* ---- ERRADO (comentado para nao executar) ---- */
    /* f = fopen("temp_pegadinha.txt", "r");
    * while (!feof(f)) {
    *     fscanf(f, "%d", &num);
    *     printf("%d\n", num);
    * }
    * fclose(f);
    *
    * O QUE ACONTECE:
    * 1. Le 10 -> imprime 10. feof = falso.
    * 2. Le 20 -> imprime 20. feof = falso.
    * 3. Le 30 -> imprime 30. feof = falso (ainda!).
    * 4. Tenta ler -> falha (fim do arquivo). feof = verdadeiro.
    *    MAS num ainda tem o valor 30 (nao foi alterado).
    *    Imprime 30 NOVAMENTE (duplicado!).
    * 5. Agora feof = verdadeiro, sai do loop.
    *
    * Resultado: 10, 20, 30, 30 (o 30 sai duplicado!)
    */

    /* ---- CERTO (com fscanf) ---- */
    /* fscanf retorna o numero de itens lidos com sucesso.
     * Se retornar 1, leu um numero. Se retornar EOF (ou 0),
     * nao leu (fim do arquivo ou erro). */
    printf("--- CERTO: while (fscanf == 1) ---\n");
    f = fopen("temp_pegadinha.txt", "r");
    if (f == NULL) {
        printf("ERRO: nao conseguiu abrir o arquivo para leitura.\n");
        return;
    }
    while (fscanf(f, "%d", &num) == 1) {
        /* so entra aqui se fscanf leu 1 numero com sucesso */
        printf("  Lido: %d\n", num);
    }
    fclose(f);

    printf("\n");

    /* ---- CERTO (alternativa com fgets) ---- */
    /* fgets retorna NULL quando chega ao fim do arquivo.
     * Esta e a forma mais segura de ler linhas de texto. */
    printf("--- CERTO: while (fgets != NULL) ---\n");
    f = fopen("temp_pegadinha.txt", "r");
    if (f == NULL) {
        printf("ERRO: nao conseguiu abrir o arquivo para leitura.\n");
        return;
    }
    while (fgets(linha, sizeof(linha), f) != NULL) {
        /* fgets le uma linha inteira (incluindo o \n).
         * Remove o \n para imprimir limpo. */
        linha[strcspn(linha, "\n")] = '\0';
        printf("  Linha: %s\n", linha);
    }
    fclose(f);

    /* Remove o arquivo temporario para nao deixar lixo */
    remove("temp_pegadinha.txt");

    printf("\nRESUMO:\n");
    printf("  feof so e verdadeiro DEPOIS de tentar ler alem do fim.\n");
    printf("  while(!feof) + leitura dentro -> ultimo registro duplicado!\n");
    printf("  CERTO: while (fscanf(f,...) == 1)  -> testa o retorno\n");
    printf("  CERTO: while (fgets(buf,n,f) != NULL)  -> testa o retorno\n");
    printf("  CERTO: while (fread(...) == 1)  -> para leitura binaria\n");
    printf("  REGRA: NUNCA use feof como condicao do while.\n");
}

/*
 ============================================================================
 PEGADINHA 11: OFF-BY-ONE (<= VS < NO FOR)
 ============================================================================
 O erro "off-by-one" (fora por um) acontece quando o loop executa
 uma vez a mais ou a menos do que o esperado, geralmente por usar
 <= em vez de < (ou vice-versa).

 ERRADO: for (i = 0; i <= n; i++) -> executa n+1 vezes (a mais!)
 CERTO: for (i = 0; i < n; i++)   -> executa n vezes (o certo)

 Com vetores, <= causa acesso fora do limite (pegadinha 07).
 Com contadores, <= causa uma iteracao extra (logica errada).
 ============================================================================
*/
void pegadinha_11_off_by_one(void) {
    int n = 5;  /* queremos contar de 1 a 5 */
    int i;
    int contador = 0;

    cabecalho(11, "OFF-BY-ONE: <= VS < NO FOR");

    printf("Queremos contar de 1 a %d (5 numeros)\n\n", n);

    /* ---- ERRADO (comentado) ---- */
    /* for (i = 1; i <= n; i++) {
    *     contador++;
    * }
    * Executa para i = 1, 2, 3, 4, 5 -> 5 vezes.
    * NESTE CASO esta certo para contar de 1 a n.
    * MAS se fosse um VETOR de tamanho n, i <= n acessaria v[n] (errado!).
    */

    /* ---- DEMONSTRACAO: <= vs < com vetor ---- */
    printf("--- Com vetor de tamanho %d ---\n", n);
    printf("  for (i=0; i <  %d; i++) -> acessa v[0] a v[%d] (%d elementos) CERTO\n",
           n, n-1, n);
    printf("  for (i=0; i <= %d; i++) -> acessa v[0] a v[%d] (%d elementos) ERRADO!\n",
           n, n, n+1);

    /* ---- DEMONSTRACAO: <= executa uma vez a mais ---- */
    printf("\n--- Contando com i = 0; i < %d ---\n", n);
    contador = 0;
    for (i = 0; i < n; i++) {
        contador++;  /* conta quantas vezes o loop executou */
    }
    printf("  Executou %d vezes (esperado: %d) -> CERTO\n", contador, n);

    printf("\n--- Contando com i = 0; i <= %d ---\n", n);
    contador = 0;
    for (i = 0; i <= n; i++) {
        contador++;  /* conta quantas vezes o loop executou */
    }
    printf("  Executou %d vezes (esperado: %d) -> UMA A MAIS!\n",
           contador, n);

    printf("\nRESUMO:\n");
    printf("  i <  n -> executa n vezes (0 a n-1) -> para VETORES\n");
    printf("  i <= n -> executa n+1 vezes (0 a n) -> CUIDADO!\n");
    printf("  Para contar de 1 a n: for(i=1; i<=n; i++) -> ok (nao e vetor)\n");
    printf("  Para percorrer vetor de tamanho n: for(i=0; i<n; i++) -> ok\n");
    printf("  O erro off-by-one e um dos bugs mais comuns em programacao.\n");
}

/*
 ============================================================================
 PEGADINHA 12: COMPARACAO DE FLOAT COM ==
 ============================================================================
 Numeros de ponto flutuante (float, double) NAO sao exatos devido
 a representacao binaria. 0.1 + 0.2 pode nao ser exatamente 0.3.

 ERRADO: if (a == b)  -> pode falhar mesmo quando deveria ser igual
 CERTO: if (fabs(a - b) < 0.0001)  -> compara com tolerancia (epsilon)

 NUNCA compare floats com ==. Use sempre uma tolerancia (epsilon).
 ============================================================================
*/
void pegadinha_12_float_comparacao(void) {
    /* Usamos double para demonstrar (o efeito e mais visivel que float) */
    double a = 0.1;
    double b = 0.2;
    double soma = a + b;   /* 0.1 + 0.2 */
    double esperado = 0.3;

    cabecalho(12, "COMPARACAO DE FLOAT COM ==");

    printf("a = 0.1\n");
    printf("b = 0.2\n");
    printf("soma = a + b\n");
    printf("esperado = 0.3\n\n");

    /* Mostra os valores com muitas casas decimais para ver a imprecisao */
    printf("soma     = %.20f\n", soma);
    printf("esperado = %.20f\n\n", esperado);

    /* ---- ERRADO ---- */
    /* == compara bit a bit. Como ha imprecisao na representacao binaria,
     * 0.1 + 0.2 nao e exatamente igual a 0.3. */
    if (soma == esperado) {
        printf("[ERRADO] soma == esperado: IGUAIS\n");
    } else {
        printf("[ERRADO] soma == esperado: DIFERENTES!\n");
        printf("         (0.1+0.2 nao e exatamente 0.3 em binario)\n");
    }

    printf("\n");

    /* ---- CERTO ---- */
    /* Compara a DIFERENCA com um valor pequeno (epsilon/tolerancia).
     * Se a diferenca for menor que epsilon, consideramos iguais.
     * fabs() e de <math.h>, mas aqui fazemos valor absoluto manual
     * para nao precisar linkar com -lm. */
    double diff = soma - esperado;  /* diferenca entre os valores */
    if (diff < 0) {
        diff = -diff;  /* valor absoluto manual (se negativo, inverte) */
    }

    double epsilon = 0.0001;  /* tolerancia: 4 casas decimais */
    if (diff < epsilon) {
        printf("[CERTO]  |soma - esperado| < epsilon: IGUAIS (com tolerancia)\n");
    } else {
        printf("[CERTO]  |soma - esperado| < epsilon: DIFERENTES\n");
    }

    printf("\nRESUMO:\n");
    printf("  Float/double NAO sao exatos (representacao binaria).\n");
    printf("  0.1 + 0.2 != 0.3 em ponto flutuante!\n");
    printf("  NUNCA use == para comparar floats.\n");
    printf("  CERTO: |a - b| < epsilon (ex: 0.0001)\n");
    printf("  fabs(a-b) < 0.0001  (fabs de <math.h>)\n");
    printf("  ou: diff = a-b; if(diff<0) diff=-diff; if(diff<0.0001)\n");
}

/*
 ============================================================================
 PEGADINHA 13: COMPARACAO DE STRUCT COM ==
 ============================================================================
 Em C, NAO e possivel comparar duas structs com ==.
 O operador == nao funciona para structs (erro de compilacao!).

 ERRADO: if (a1 == a2)  -> ERRO DE COMPILACAO em C!
 CERTO: comparar campo a campo manualmente.

 (Nota: em C++ isso funciona, mas a prova e de C puro!)
 ============================================================================
*/
void pegadinha_13_struct_comparacao(void) {
    /* Duas structs Aluno com os mesmos valores */
    Aluno a1 = {1, "Joao", 8.5f};  /* matricula=1, nome="Joao", nota=8.5 */
    Aluno a2 = {1, "Joao", 8.5f};  /* mesmos valores, mas em outra posicao */

    cabecalho(13, "COMPARACAO DE STRUCT COM ==");

    printf("Aluno a1: matricula=%d, nome=\"%s\", nota=%.2f\n",
           a1.matricula, a1.nome, a1.nota);
    printf("Aluno a2: matricula=%d, nome=\"%s\", nota=%.2f\n\n",
           a2.matricula, a2.nome, a2.nota);

    /* ---- ERRADO (comentado: NAO COMPILA em C!) ---- */
    /* if (a1 == a2) {
    *     printf("Alunos iguais\n");
    * }
    *
    * O QUE ACONTECE:
    * - ERRO DE COMPILACAO: "invalid operands to binary == (have Aluno and Aluno)"
    * - O operador == NAO esta definido para structs em C.
    * - Em C++ isso funciona (operador sobrecarregado), mas em C puro NAO.
    * - O professor cai nessa: o aluno escreve if(a1==a2) e nao compila.
    */

    /* ---- CERTO ---- */
    /* Comparar cada campo individualmente.
     * Para int: == (e seguro para inteiros).
     * Para string (char[]): strcmp == 0.
     * Para float: usar tolerancia (ver pegadinha 12), mas aqui
     *   simplificamos com == direto (nao e ideal para float). */
    if (a1.matricula == a2.matricula &&
        strcmp(a1.nome, a2.nome) == 0) {
        /* Se matricula e nome forem iguais, consideramos o mesmo aluno.
         * (Poderiamos comparar a nota tambem, mas float com == e
         *  outra pegadinha -- ver opcao 12.) */
        printf("[CERTO] Alunos IGUAIS (matricula e nome conferem)\n");
    } else {
        printf("[CERTO] Alunos DIFERENTES\n");
    }

    printf("\nRESUMO:\n");
    printf("  C NAO suporta == entre structs (erro de compilacao!).\n");
    printf("  C++ suporta, mas a prova e de C puro.\n");
    printf("  CERTO: comparar campo a campo:\n");
    printf("    if (a1.id == a2.id && strcmp(a1.nome, a2.nome) == 0)\n");
    printf("  Para copiar uma struct, voce PODE usar = (atribuicao):\n");
    printf("    a2 = a1;  -> copia TODOS os campos (isso funciona em C!)\n");
    printf("  So NAO funciona == (comparacao). = (atribuicao) funciona.\n");
}

/* ===================== FIM DO ARQUIVO ===================== */
/*
 * DICAS FINAIS PARA A PROVA:
 *
 * 1. =  atribui, == compara. No if, SEMPRE use ==.
 * 2. scanf precisa de & (exceto para strings/vetores).
 * 3. Strings se comparam com strcmp, nao com ==.
 * 4. strcmp retorna 0 para IGUAIS. if(strcmp==0) e o certo.
 * 5. int / int = int (divisao inteira). Use cast para real.
 * 6. Nao esqueca o break no switch (a menos que queira fall-through).
 * 7. Vetor de tamanho N: indices 0 a N-1. Use < N, nao <= N.
 * 8. scanf deixa \n no buffer. Limpe antes de fgets.
 * 9. Nao coloque ; depois do if/for/while antes do bloco { }.
 * 10. Nao use while(!feof). Teste o retorno da leitura.
 * 11. Cuidado com off-by-one: < vs <= no for.
 * 12. Nao compare float com ==. Use tolerancia (epsilon).
 * 13. Nao compare struct com ==. Compare campo a campo.
 */
