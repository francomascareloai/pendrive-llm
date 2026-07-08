/* =========================================================================
 * ARQUIVO: 15_arquivo_binario_nomes.c
 * --------------------------------------------------------------------------
 * O QUE FAZ:
 *   Programa de CADASTRO DE ALUNOS em ARQUIVO BINARIO (nomesbin.bin).
 *   Menu com getche() (conio.h) e switch. Operacoes:
 *     1 - Criar     -> fopen("nomesbin.bin","wb")  (zera/cria o arquivo)
 *     2 - Cadastrar -> fopen("nomesbin.bin","ab")  (adiciona registros no fim)
 *     3 - Relatorio -> fopen("nomesbin.bin","rb")  (le e imprime todos)
 *     4 - Sair      -> exit(0)
 *
 * TOPICO DA PROVA:
 *   - Arquivo BINARIO (fopen wb/ab/rb, fwrite, fread, sizeof)
 *   - struct (typedef struct com alias)
 *   - strings (fgets no lugar de gets, strcspn para tirar o \n)
 *   - Modularizacao (funcoes criar/cadastrar/relatorio/menu)
 *   - Menu com switch + getche (estilo do professor)
 *
 * PALAVRAS-CHAVE (Ctrl+F):
 *   arquivo binario ; fopen wb ; fopen ab ; fopen rb ; fwrite ; fread ;
 *   typedef struct ; sizeof ; fgets ; strcspn ; feof ; limparBuffer ;
 *   fflush stdin ; getche ; menu switch ; struct alunos ; variavel global
 *
 * BUGS CORRIGIDOS EM RELACAO AO CODIGO ORIGINAL DO PROFESSOR:
 *   1) typedef struct alunos { ... }; SEM alias -> NAO compila em C puro.
 *      Corrigido para: typedef struct { ... } alunos;  (alias no fim).
 *   2) while(!feof(ARQ)) com fread ANTES do loop -> le o ultimo registro 2x
 *      e imprime lixo. Corrigido para:
 *      while (fread(&alu, sizeof(alunos), 1, ARQ) == 1) { ... }
 *   3) gets(alu.nome) -> removido do C. Trocado por fgets + strcspn.
 *   4) fflush(stdin) -> indefinido no padrao C, MAS funciona no Windows/MinGW.
 *      MANTIDO (estilo do professor) com comentario explicando o PORQUE.
 *   5) main(void) sem tipo de retorno -> corrigido para int main(void) + return 0.
 *   6) printf "%03    |" faltando o 'd' -> corrigido para "%03d".
 *   7) Verificacao de fopen==NULL no relatorio (ja existia, mantida e melhorada).
 *
 * COMPILACAO:
 *   gcc -Wall -Wextra 15_arquivo_binario_nomes.c -o programa
 *   (no Windows/MinGW o conio.h existe e funciona)
 * =========================================================================
 */

#include <stdio.h>   /* fopen, fclose, fwrite, fread, printf, scanf, fgets  */
#include <stdlib.h>  /* system, exit                                       */
#include <string.h>  /* strcspn (para tirar o \n do fgets)                 */
#include <conio.h>   /* getche, getch  (estilo do professor, Windows/MinGW) */

/* -------------------------------------------------------------------------
 * Variaveis GLOBAIS (estilo do professor).
 *   ARQ -> ponteiro para o arquivo binario aberto pelas funcoes.
 *   alu -> struct que guarda 1 registro de aluno em memoria.
 *   Usar globais evita passar parametros; NAO e boa pratica em projetos
 *   grandes, MAS e o estilo que cai na prova deste professor.
 * ---------------------------------------------------------------------- */
FILE *ARQ;

/* typedef struct SEM alias (como o professor escreveu) NAO compila em C puro:
 *    typedef struct alunos { int cod; ... };   -> "alunos alu;" nao existe.
 * Corrigido: criamos um apelido "alunos" no fim do typedef, assim
 * "alunos alu;" passa a ser uma declaracao valida.                       */
typedef struct {
    int  cod;        /* codigo do aluno (digitar 0 encerra o cadastro)    */
    char nome[20];   /* nome do aluno (string, vetor de char)            */
    int  idade;      /* idade do aluno                                    */
} alunos;            /* <-- alias: agora "alunos" e um tipo valido        */

alunos alu;          /* variavel global que guarda 1 registro            */

/* -------------------------------------------------------------------------
 * Prototipos das funcoes (declarados antes do main para o compilador
 * conhecer as assinaturas). O professor costuma prototipar tudo no topo.
 * ---------------------------------------------------------------------- */
void criar(void);
void cadastrar(void);
void relatorio(void);
char menu(void);

/* -------------------------------------------------------------------------
 * Funcao para limpar o buffer de entrada.
 *   fflush(stdin) funciona no Windows/MinGW, mas NAO e portatil (o padrao
 *   C so define fflush para fluxos de SAIDA). Para ser 100% correto usamos
 *   esta funcao que descarta caracteres ate achar \n ou EOF.
 *   AINDA ASSIM, mantemos fflush(stdin) no codigo (comentario explicando)
 *   para preservar o estilo do professor que cai na prova.
 * ---------------------------------------------------------------------- */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta cada caractere sobrando no buffer */
    }
}

/* -------------------------------------------------------------------------
 * menu(): mostra as opcoes e le 1 caractere com getche().
 *   getche() le um caractere E MOSTRA na tela (eco), sem precisar Enter.
 *   Retorna o caractere lido (ex: '1', '2', '3', '4').
 * ---------------------------------------------------------------------- */
char menu(void) {
    printf(" \n1 - Criar");        /* opcao criar arquivo                  */
    printf(" \n2 - Cadastrar");   /* opcao cadastrar registros            */
    printf(" \n3 - Relatorio");   /* opcao listar registros               */
    printf(" \n4 - Sair");        /* opcao sair do programa               */
    printf("\nEntre com a opcao : ");
    return (getche());            /* le e ecoa o caractere digitado       */
}

/* -------------------------------------------------------------------------
 * criar(): cria (ou zera) o arquivo binario "nomesbin.bin".
 *   fopen(..., "wb") -> modo WRITE BINARY:
 *     - Se o arquivo NAO existe, ele CRIA.
 *     - Se o arquivo JA existe, ele ZERA (trunca) o conteudo.
 *   Por isso "Criar" apaga tudo que ja estava cadastrado antes.
 * ---------------------------------------------------------------------- */
void criar(void) {
    system("cls");  /* limpa a tela (Windows)                              */
    printf("\nCriacao do arquivo\n");
    ARQ = fopen("nomesbin.bin", "wb");  /* wb = write binary               */
    if (ARQ == NULL) {                  /* se falhou ao criar/abrir        */
        printf("\nERRO ao criar o arquivo!\n");
        system("pause");
        return;                         /* sai da funcao sem fechar (NULL) */
    }
    printf("\n========= Arquivo foi criado ========= \n");
    system("pause");  /* "Pressione qualquer tecla..." (Windows)          */
    fclose(ARQ);      /* SEMPRE fechar o arquivo apos usar                 */
}

/* -------------------------------------------------------------------------
 * cadastrar(): adiciona registros no FIM do arquivo binario.
 *   fopen(..., "ab") -> modo APPEND BINARY: so escreve no fim, nao apaga.
 *   Loop: le o codigo; enquanto codigo != 0, preenche a struct e faz
 *   fwrite (grava 1 bloco de sizeof(alunos) bytes no arquivo).
 *   Digitar 0 no codigo encerra o cadastro.
 * ---------------------------------------------------------------------- */
void cadastrar(void) {
    int codigo;  /* variavel local para ler o codigo antes de copiar p/ alu */

    system("cls");
    printf("\nCadastrar registros\n");
    ARQ = fopen("nomesbin.bin", "ab");  /* ab = append binary              */
    if (ARQ == NULL) {                  /* se nao conseguiu abrir           */
        printf("\nERRO ao abrir o arquivo para cadastro!\n");
        system("pause");
        return;
    }

    printf("\nCodigo = ");
    fflush(stdin);                  /* limpa buffer (Windows; ver nota acima) */
    scanf("%d", &codigo);          /* le o codigo digitado                  */

    /* Loop enquanto o codigo for diferente de 0 (0 = fim do cadastro).    */
    while (codigo != 0) {
        alu.cod = codigo;          /* copia o codigo ja lido para a struct  */

        printf("\nNome = ");
        fflush(stdin);             /* limpa o \n deixado pelo scanf anterior */

        /* gets() foi REMOVIDO do C (perigoso: nao checa tamanho do buffer).
         * Trocado por fgets: le ate 19 caracteres (deixa 1 para o '\0').
         * fgets tambem guarda o '\n' do Enter, por isso removemos depois.  */
        fgets(alu.nome, sizeof(alu.nome), stdin);

        /* strcspn retorna o indice do primeiro '\n' encontrado. Colocando
         * '\0' nessa posicao, "cortamos" o Enter que o fgets adicionou.    */
        alu.nome[strcspn(alu.nome, "\n")] = '\0';

        printf("\nIdade = ");
        fflush(stdin);             /* limpa buffer antes de ler int          */
        scanf("%d", &alu.idade);   /* & obrigatorio para int (nao e string)  */

        /* fwrite grava 1 registro (bloco de sizeof(alunos) bytes) no arquivo.
         *   &alu    -> endereco de inicio da struct na memoria
         *   sizeof(alunos) -> tamanho em bytes de 1 registro
         *   1       -> quantidade de blocos a gravar
         *   ARQ     -> arquivo destino
         * Retorna o numero de blocos gravados (deve ser 1).                */
        fwrite(&alu, sizeof(alunos), 1, ARQ);

        system("cls");
        printf("\nCodigo = ");
        fflush(stdin);
        scanf("%d", &codigo);      /* le o proximo codigo (0 = sair)        */
    }

    fclose(ARQ);  /* fecha o arquivo apos terminar o cadastro              */
}

/* -------------------------------------------------------------------------
 * relatorio(): le e imprime TODOS os registros do arquivo binario.
 *   fopen(..., "rb") -> modo READ BINARY: so le, nao altera o arquivo.
 *
 *   BUG CLASSICO DO PROFESSOR (CORRIGIDO AQUI):
 *     O original fazia:
 *        fread(&alu, sizeof(alunos), 1, ARQ);   <- le 1 vez ANTES do loop
 *        while (!feof(ARQ)) {
 *            printf(...);
 *            fread(&alu, sizeof(alunos), 1, ARQ);  <- le de novo no fim
 *        }
 *     O problema: feof so vira TRUE DEPOIS de uma leitura que FALHOU.
 *     Assim o loop roda 1 vez a mais, imprimindo o ULTIMO registro 2x
 *     (ou lixo). Sintoma classico: "imprimiu o ultimo nome repetido".
 *
 *   CORRECAO: usar o proprio retorno do fread como condicao do while.
 *     fread retorna o numero de BLOCOS lidos com sucesso. Se retornar 1,
 *     leu 1 registro valido; se retornar 0, chegou ao fim (ou erro).
 *        while (fread(&alu, sizeof(alunos), 1, ARQ) == 1) { ... }
 *     Assim NAO precisa de fread antes do loop e NAO imprime lixo.
 * ---------------------------------------------------------------------- */
void relatorio(void) {
    system("cls");
    printf("\n=========Relatorio de Registros======\n");

    ARQ = fopen("nomesbin.bin", "rb");  /* rb = read binary                */
    if (ARQ == NULL) {                  /* se o arquivo nao existe ou erro  */
        printf("\nErro ao abrir o arquivo (ele existe?).");
        printf("\nUse a opcao 1 (Criar) antes de listar.\n");
        system("pause");
        return;
    }

    printf("\n___\n");

    /* fread dentro do while: le 1 bloco; se retornar 1, leu com sucesso.
     * Nao ha fread antes do loop -> nao imprime lixo nem repete o ultimo. */
    while (fread(&alu, sizeof(alunos), 1, ARQ) == 1) {
        /* %03d -> inteiro com 3 digitos, preenchido com zeros a esquerda.
         * %-20s -> string alinhada a esquerda, largura 20 (preenche espacos).
         * O original tinha "%03    |" (faltava o 'd') -> corrigido p/ "%03d". */
        printf("%03d  |  %-20s  |  %d\n", alu.cod, alu.nome, alu.idade);
    }

    printf("\n___\n");
    system("pause");
    fclose(ARQ);  /* sempre fechar o arquivo apos terminar de ler          */
}

/* -------------------------------------------------------------------------
 * main(): loop principal do menu.
 *   while(1) -> loop infinito; so sai com exit(0) na opcao 4.
 *   int main(void) + return 0 no fim (o original era so "main(void)").
 * ---------------------------------------------------------------------- */
int main(void) {
    char op;  /* guarda a opcao digitada no menu                           */

    while (1) {           /* loop infinito do menu                          */
        system("cls");
        op = menu();      /* mostra menu e le a opcao com getche           */
        switch (op) {
            case '1':     /* '1' e um caractere, por isso aspas simples    */
                criar();
                break;
            case '2':
                cadastrar();
                break;
            case '3':
                relatorio();
                break;
            case '4':
                exit(0);  /* encerra o programa (stdlib.h)                 */
                break;
            default:
                /* opcao invalida: o loop simplesmente reinicia o menu.    */
                break;
        }
    }

    return 0;  /* boa pratica (embora exit(0) acima ja encerre o programa) */
}
