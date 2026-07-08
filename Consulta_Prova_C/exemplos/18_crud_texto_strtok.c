/*
 * ============================================================================
 * 18_crud_texto_strtok.c
 * ============================================================================
 * O QUE FAZ: CRUD (Create/Read/Update) em arquivo TEXTO no formato CSV.
 *            Arquivo "funcionario.txt" com linhas "codigo,nome,salario".
 *            Menu: A-Iniciar (w), B-Cadastrar (r checa duplicidade + a append),
 *            C-Alterar Salario (via temp.txt + rename), D-Relatorio geral.
 *
 * TOPICO: Arquivo texto + strtok + atoi + atof + fprintf + padrao temp+rename
 *
 * PALAVRAS-CHAVE (Ctrl-F): strtok, atoi, atof, fprintf, fgets, arquivo texto,
 *   CSV, CRUD texto, temp.txt, rename, alterar arquivo texto, limpar buffer,
 *   feof bug, gets removido, fflush stdin, menu switch, conio.h, getche,
 *   padrao temp rename, strtok NULL, strcspn, remover \n, duplicidade
 *
 * BUGS CORRIGIDOS EM RELACAO AO CODIGO ORIGINAL DO PROFESSOR:
 *   1. while(!feof(arq)) + fgets ANTES do loop -> ultimo registro lido 2x
 *      e imprime lixo. Corrigido para: while(fgets(buf,MAX,arq)!=NULL).
 *      MOTIVO: feof() so retorna verdadeiro DEPOIS de uma leitura que falhou
 *      em achar dados. Com fgets antes do loop + feof na condicao, a ultima
 *      linha valida e lida, mas o feok ainda e falso, entao o loop entra
 *      novamente, fgets falha (retorna NULL), mas os dados do buffer anterior
 *      (lixo/ultimo registro) sao processados de novo. Usar o retorno do
 *      proprio fgets como condicao do while resolve isso de forma limpa.
 *   2. gets(nome) -> removido do padrao C (C11 em diante). Trocado por
 *      fgets(nome,sizeof(nome),stdin) + nome[strcspn(nome,"\n")]='\0'.
 *   3. main(void) sem tipo de retorno -> int main(void) + return 0.
 *   4. strtok sem verificar NULL -> adicionado teste de NULL para robustez
 *      (se a linha do CSV estiver malformada, strtok retorna NULL e o atoi/
 *      atof/printf poderiam dar pau/crash).
 *   5. fflush(stdin) MANTIDO (estilo do professor) com comentario explicando
 *      que funciona no Windows/MinGW mas NAO e portatil (indefinido no padrao).
 *   6. fclose(arq) faltando no return de erro de salario negativo (vazamento
 *      de file handle). Adicionado.
 *   7. Verificacao de NULL apos fopen do temp.txt (original nao testava).
 *
 * COMPILACAO: gcc -Wall -Wextra 18_crud_texto_strtok.c -o programa
 * ============================================================================
 */

/* ---- BIBLIOTECAS ---- */
#include <stdio.h>   /* fopen, fclose, fgets, fprintf, printf, scanf, FILE    */
#include <stdlib.h>  /* atoi, atof, system, remove, rename, NULL               */
#include <string.h>  /* strtok, strcspn, strlen                                */
#include <ctype.h>   /* toupper                                                */
#include <conio.h>   /* getche, getch - especifico do Windows/MinGW (nao e    */
                     /* padrao C, mas o professor usa e cai na prova)         */

/* ---- CONSTANTES ---- */
#define MAX 80        /* tamanho maximo de cada linha lida do arquivo (buf)   */
#define MAX_NOME 30   /* tamanho maximo do nome do funcionario                */

/*
 * Funcao: opcao_menu
 * Mostra o menu na tela e retorna a tecla pressionada (convertida para
 * maiuscula, para o switch funcionar tanto com 'a' quanto 'A').
 */
char opcao_menu(void) {
    system("cls");              /* limpa a tela (Windows)                      */
    printf("(A)Iniciar o arquivo\n");
    printf("(B)Cadastrar\n");
    printf("(C)Alterar Salario\n");
    printf("(D)Relatorio geral\n");
    printf("(F)Fim\n");
    return(toupper(getche()));  /* getche() le 1 tecla COM eco (mostra na tela)
                                   toupper converte para maiuscula            */
}

/*
 * Funcao: cria_arquivo  (Opcao A)
 * Cria (ou ZERA, se ja existir) o arquivo funcionario.txt.
 * Modo "w" = write: se o arquivo existe, trunca (zera). Se nao existe, cria.
 * ATENCAO: chamar esta opcao APAGA todos os dados anteriores!
 */
void cria_arquivo(void) {
    FILE *arq;                  /* ponteiro para o arquivo                     */
    system("cls");
    arq = fopen("funcionario.txt", "w");  /* "w" cria ou trunca o arquivo     */
    if (arq == NULL) {          /* SEMPRE testar NULL apos fopen              */
        printf("Erro ao criar arquivo\n");
        getch();                /* pausa para o usuario ler a mensagem         */
        return;
    }
    printf("Arquivo de funcionario criado com sucesso");
    fclose(arq);                /* fecha para garantir que os dados foram      */
                                /* gravados (flush do buffer do SO)           */
}

/*
 * Funcao: novo_cliente  (Opcao B)
 * Cadastra um novo funcionario no arquivo.
 * Fluxo:
 *   1. Abre o arquivo em modo LEITURA ("r") para checar se o codigo ja existe.
 *   2. Percorre todas as linhas com fgets + strtok, comparando codigos.
 *   3. Se o codigo ja existe, avisa e sai sem cadastrar (antiduplicidade).
 *   4. Se nao existe, pede nome e salario, abre em modo APPEND ("a") e grava.
 *
 * FORMATO DA LINHA NO ARQUIVO (CSV):
 *   "codigo,nome,salario\n"
 *   Exemplo: "10,Joao da Silva,2500.00\n"
 *
 * strtok: quebra uma string em pedacos (tokens) usando um delimitador (",").
 *   1a chamada: strtok(string, delimitador) -> retorna o 1o token.
 *   Chamadas seguintes: strtok(NULL, delimitador) -> continua de onde parou.
 *   Retorna NULL quando nao ha mais tokens.
 * atoi: converte string para int.    atof: converte string para double/float.
 */
void novo_cliente(void) {
    int num, n;                 /* num=codigo digitado | n=codigo lido do arq */
    float si;                   /* salario informado pelo usuario             */
    char nome[MAX_NOME];        /* nome do funcionario (buffer proprio)       */
    char buf[MAX];              /* buffer para ler cada linha do arquivo       */
    FILE *arq;                  /* ponteiro para o arquivo                     */

    system("cls");

    /* PASSO 1: abre para LEITURA ("r") para checar duplicidade de codigo.
       Se o arquivo nao existe, fopen("r") retorna NULL.
       O usuario deve usar a opcao A (criar arquivo) antes da opcao B. */
    arq = fopen("funcionario.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo do funcionario\n");
        printf("Use a opcao A para criar o arquivo primeiro.\n");
        getch();
        return;
    }

    printf("Digite os dados do funcionario:\n");
    printf("Codigo: ");
    scanf("%d", &num);           /* le o codigo digitado pelo usuario           */
    fflush(stdin);              /* limpa o '\n' deixado pelo scanf no buffer do
                                   teclado. fflush(stdin) FUNCIONA no Windows
                                   (MinGW) mas NAO e portatil - e comportamento
                                   indefinido no padrao C. Em Linux/Mac, use:
                                     while((c=getchar())!='\n' && c!=EOF);   */

    /* PASSO 2: percorre o arquivo linha por linha checando se o codigo
       ja existe (antiduplicidade).

       BUG CORRIGIDO: o codigo original do professor fazia:
         fgets(buf,max,arq);              <- le antes do loop
         while(!feof(arq)) {              <- condicao ERRADA
            ... processa buf ...
            fgets(buf,max,arq);           <- le no final do loop
         }
       O problema: feof() so vira TRUE DEPOIS de uma tentativa de leitura que
       falhou. Entao o loop entra uma vez a mais do que deveria, processa o
       buffer com dados da ultima linha valida (ou lixo), e imprime/cadastra
       duplicado.

       CORRECAO: usar o retorno do proprio fgets como condicao do while.
       fgets retorna NULL quando chega ao fim do arquivo (EOF) ou da erro.
       Assim o loop para exatamente quando nao ha mais dados para ler:
         while(fgets(buf, MAX, arq) != NULL) { ... }
       NAO precisa de fgets antes do loop nem feof. */
    while (fgets(buf, MAX, arq) != NULL) {

        /* strtok(buf, ",") quebra buf na primeira "," e retorna o 1o token
           (o codigo, como string). atoi converte para int. */
        char *tok = strtok(buf, ",");
        if (tok == NULL) continue;   /* linha vazia ou so com \n -> pula      */
        n = atoi(tok);              /* n = codigo do funcionario desta linha  */

        /* 2o token: nome. Lemos para avancar o strtok, mas nao usamos aqui
           (so precisamos do codigo para checar duplicidade). */
        strtok(NULL, ",");

        /* 3o token: salario. Tambem nao precisamos aqui. */
        strtok(NULL, ",");

        /* Se o codigo ja existe no arquivo, avisa e sai sem cadastrar. */
        if (num == n) {
            printf("Funcionario ja cadastrado");
            fclose(arq);        /* fecha o arquivo de leitura antes de sair  */
            getch();
            return;
        }
    }
    fclose(arq);                /* fecha o arquivo de leitura                 */

    /* PASSO 3: pede os dados restantes (nome e salario).
       O codigo (num) ja foi lido acima. */

    printf("Nome: ");

    /* BUG CORRIGIDO: o original usava gets(nome), que foi REMOVIDO do padrao
       C (a partir do C11). gets() nao verifica o tamanho do buffer, permitindo
       overflow (falha de seguranca classica).
       Trocado por fgets, que recebe o tamanho maximo e nao estoura o buffer. */
    fgets(nome, sizeof(nome), stdin);

    /* fgets inclui o '\n' no final da string (se couber). Para gravar no
       arquivo CSV sem o '\n' no meio do nome, removemos com strcspn:
       strcspn(nome, "\n") retorna o indice do primeiro '\n' encontrado.
       Colocamos '\0' (fim de string) nesse indice, removendo o '\n'. */
    nome[strcspn(nome, "\n")] = '\0';

    printf("Salario inicial: ");
    scanf("%f", &si);           /* le o salario como float                    */
    fflush(stdin);              /* limpa o '\n' deixado pelo scanf            */

    /* PASSO 4: abre em modo APPEND ("a") para adicionar no final do arquivo.
       "a" = append: se o arquivo existe, escreve no final. Se nao existe,
       cria o arquivo (igual ao "w", mas nao apaga o conteudo existente). */
    arq = fopen("funcionario.txt", "a");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo para escrita\n");
        getch();
        return;
    }

    /* fprintf grava dados formatados no arquivo (igual printf, mas no arquivo
       em vez da tela). Formato CSV: codigo,nome,salario\n
       %.2f garante 2 casas decimais no salario. */
    fprintf(arq, "%d,%s,%.2f\n", num, nome, si);

    printf("Funcionario cadastrado com sucesso");
    fclose(arq);                /* fecha para garantir gravacao (flush)      */
}

/*
 * Funcao: alteracao  (Opcao C)
 * Altera o salario de um funcionario pelo codigo.
 *
 * PADRAO temp + rename (MUITO IMPORTANTE PARA A PROVA):
 *   Arquivos texto NAO permitem "editar no meio" de forma facil. Se o novo
 *   salario tiver um numero diferente de caracteres do antigo (ex: 100.00 ->
 *   50.00), os dados seguintes seriam sobrescritos incorretamente.
 *   Solucao padrao: copiar o arquivo inteiro para um temporario (temp.txt),
 *   trocando a linha alterada, e depois substituir o original pelo temporario.
 *
 *   Passos:
 *   1. Abre original ("r") e temp.txt ("w").
 *   2. Le cada linha do original com fgets.
 *   3. Se o codigo bate: grava no temp com o NOVO salario.
 *      Se nao bate: grava no temp com o salario ORIGINAL (copia identica).
 *   4. Fecha ambos os arquivos.
 *   5. Se encontrou: remove("funcionario.txt") + rename("temp.txt", original).
 *      Se nao encontrou: remove("temp.txt") (descarta, nada mudou).
 */
void alteracao(void) {
    int num, nc, achei = 0;     /* num=codigo lido | nc=codigo procurado      */
                                /* achei=flag: 0=nao achou, 1=achou           */
    float s, d;                 /* s=salario atual | d=novo salario           */
    char *nome;                 /* ponteiro p/ nome (retornado por strtok).
                                   ATENCAO: aponta para dentro de buf, so e
                                   valido enquanto buf nao for sobrescrito
                                   (ou seja, dentro da iteracao atual do loop) */
    char buf[MAX];              /* buffer para ler cada linha do arquivo      */
    FILE *arq, *tmp;            /* arq=original | tmp=temporario              */

    system("cls");
    arq = fopen("funcionario.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo\n");
        getch();
        return;
    }
    tmp = fopen("temp.txt", "w");   /* cria arquivo temporario              */
    if (tmp == NULL) {              /* testa NULL (original nao testava)      */
        printf("Erro ao criar arquivo temporario\n");
        fclose(arq);
        getch();
        return;
    }

    printf("Digite o codigo do funcionario: ");
    scanf("%d", &nc);           /* nc = codigo que o usuario quer alterar     */
    fflush(stdin);              /* limpa o '\n' deixado pelo scanf            */

    /* LOOP: le cada linha do original e copia para o temp.
       BUG CORRIGIDO: while(fgets(...)) em vez de fgets+feof+fgets. */
    while (fgets(buf, MAX, arq) != NULL) {

        /* Parseia a linha CSV com strtok (igual ao novo_cliente). */
        char *tok = strtok(buf, ",");
        if (tok == NULL) continue;   /* linha vazia -> pula                  */
        num = atoi(tok);             /* codigo (int)                          */

        nome = strtok(NULL, ",");    /* nome (ponteiro para dentro de buf)    */
        if (nome == NULL) nome = ""; /* se campo faltar, usa string vazia
                                        para nao crashar no printf/fprintf   */

        char *tok_sal = strtok(NULL, ",");
        s = (tok_sal != NULL) ? atof(tok_sal) : 0.0;  /* salario atual       */

        if (num == nc) {
            /* ACHOU o funcionario! Mostra os dados atuais e pede novo salario. */
            achei = 1;          /* marca que encontrou (para o rename no fim)  */

            printf("\n");
            printf(" NUM  |               NOME               |   Salario \n");
            printf("______|__________________________________|___________\n");
            /* %03d = inteiro com 3 digitos, preenchido com zeros a esquerda
               %30s = string alinhada a direita em 30 caracteres
               %9.2f = float com 9 caracteres totais e 2 casas decimais      */
            printf("  %03d |   %30s | %9.2f \n", num, nome, s);
            printf("______|__________________________________|___________\n");

            printf("\nDigite o valor do novo salario: ");
            scanf("%f", &d);    /* d = novo salario                           */
            fflush(stdin);

            /* Validacao: salario nao pode ser negativo. */
            if (d < 0) {
                fclose(tmp);           /* fecha o temporario                 */
                fclose(arq);           /* BUG CORRIGIDO: original nao fechava
                                           o arq aqui (vazamento de handle)  */
                remove("temp.txt");    /* descarta o temp (incompleto)       */
                printf("Impossivel salario negativo\n");
                getch();
                return;
            }

            /* Grava no temp com o NOVO salario (d). */
            fprintf(tmp, "%d,%s,%.2f\n", num, nome, d);
            printf("Salario do Funcionario alterado com sucesso");
        } else {
            /* NAO e o funcionario procurado: copia a linha original sem
               alterar (grava no temp identica a original). */
            fprintf(tmp, "%d,%s,%.2f\n", num, nome, s);
        }
    }

    fclose(arq);                /* fecha o arquivo original                  */
    fclose(tmp);                /* fecha o temporario (grava os dados)       */

    if (achei == 0) {
        /* Nao encontrou o funcionario: descarta o temp (nao mudou nada). */
        printf("Funcionario %d nao existe.", nc);
        remove("temp.txt");
    } else {
        /* Encontrou e alterou: substitui o original pelo temporario.
           remove() apaga o arquivo original.
           rename() renomeia temp.txt para funcionario.txt. */
        remove("funcionario.txt");
        rename("temp.txt", "funcionario.txt");
    }
}

/*
 * Funcao: mostra_tudo  (Opcao D)
 * Le todo o arquivo e mostra um relatorio formatado de todos os funcionarios.
 */
void mostra_tudo(void) {
    int num;                    /* codigo lido de cada linha                  */
    float s;                    /* salario lido de cada linha                 */
    char *nome;                 /* ponteiro para nome (strtok)                */
    char buf[MAX];              /* buffer para ler cada linha                 */
    FILE *arq;

    system("cls");
    arq = fopen("funcionario.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo\n");
        getch();
        return;
    }

    /* Cabecalho da tabela. */
    printf(" NUM  |               NOME               |   Salario \n");
    printf("______|__________________________________|____________\n");

    /* BUG CORRIGIDO: while(fgets(...)) em vez de fgets+feof+fgets.
       fgets retorna NULL no fim do arquivo, entao o loop para sozinho. */
    while (fgets(buf, MAX, arq) != NULL) {

        /* Parseia a linha CSV com strtok (mesma logica das outras funcoes). */
        char *tok = strtok(buf, ",");
        if (tok == NULL) continue;   /* linha vazia -> pula                 */
        num = atoi(tok);             /* codigo                              */

        nome = strtok(NULL, ",");    /* nome                                */
        if (nome == NULL) nome = ""; /* protege contra campo faltante        */

        char *tok_sal = strtok(NULL, ",");
        s = (tok_sal != NULL) ? atof(tok_sal) : 0.0;  /* salario            */

        /* Mostra a linha formatada. */
        printf("  %03d |   %30s | %9.2f \n", num, nome, s);
    }
    printf("______|__________________________________|____________\n");
    fclose(arq);
}

/*
 * Funcao: main
 * Loop principal com menu switch. Repete ate o usuario pressionar 'F' (Fim).
 */
int main(void) {                /* BUG CORRIGIDO: era "main(void)" sem int   */
    char op;                    /* opcao escolhida no menu                    */

    do {
        op = opcao_menu();      /* mostra menu e le a tecla                   */
        switch (op) {
            case 'A': cria_arquivo();  break;
            case 'B': novo_cliente();  break;
            case 'C': alteracao();     break;
            case 'D': mostra_tudo();   break;
            /* 'F' nao tem case: o while(op!='F') encerra o loop. */
        }
        printf("\n");
        getche();               /* pausa: espera uma tecla antes de voltar
                                   ao menu (limpa e mostra de novo)          */
    } while (op != 'F');

    return 0;                   /* BUG CORRIGIDO: faltava return 0           */
}

/*
 * ============================================================================
 * RESUMO DOS PADROES IMPORTANTES PARA A PROVA (Ctrl-F: resumo prova):
 *
 * 1. PADRAO temp+rename PARA ALTERAR ARQUIVO TEXTO:
 *    - Abre original em "r", temp em "w".
 *    - Copia linha por linha; na linha alterada, grava o novo valor.
 *    - Fecha ambos. remove(original). rename(temp, original).
 *    - Se nao achou o registro: remove(temp) e nada muda.
 *
 * 2. PADRAO DE LEITURA CORRETO (sem bug do feof):
 *    while(fgets(buf, sizeof(buf), arq) != NULL) { ... }
 *    NAO usar: fgets antes do loop + while(!feof(arq)) + fgets no final.
 *
 * 3. PARSEAR CSV COM strtok:
 *    tok1 = strtok(buf, ",");   // 1o campo
 *    tok2 = strtok(NULL, ",");  // 2o campo (NULL = continua da mesma string)
 *    tok3 = strtok(NULL, ",");  // 3o campo
 *    int   = atoi(tok);   float = atof(tok);
 *    SEMPRE testar se tok == NULL antes de usar (linha malformada).
 *
 * 4. LER STRING DO TECLADO (substituto do gets):
 *    fgets(str, sizeof(str), stdin);
 *    str[strcspn(str, "\n")] = '\0';  // remove o \n do final
 *
 * 5. fflush(stdin): funciona no Windows (MinGW) mas NAO e portatil.
 *    Para codigo portatil, use: while((c=getchar())!='\n' && c!=EOF);
 * ============================================================================
 */
