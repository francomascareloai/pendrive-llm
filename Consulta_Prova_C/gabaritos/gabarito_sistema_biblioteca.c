/* =========================================================================
 * GABARITO: SISTEMA DE BIBLIOTECA (CRUD COMPLETO EM ARQUIVO BINARIO)
 * -------------------------------------------------------------------------
 * O QUE FAZ: Gerencia livros em arquivo binario (livros.dat). Operacoes:
 *            Cadastrar, Listar, Buscar por Codigo (fseek acesso direto),
 *            Buscar por Titulo (strstr), Emprestar, Devolver, Excluir.
 * TOPICO: struct, arquivos binarios, fseek, strstr, CRUD, modularizacao.
 * PALAVRAS-CHAVE Ctrl-F: struct, typedef, fwrite, fread, fseek, strstr,
 *                        fopen, fclose, rename, remove, temp, rename,
 *                        menu, switch, do-while, fgets, strcspn, fflush,
 *                        CRUD, emprestar, devolver, excluir, duplicidade.
 * COMPILA: gcc -Wall -Wextra gabarito_sistema_biblioteca.c -o biblio.exe
 * -------------------------------------------------------------------------
 * REGRAS APLICADAS:
 *  - C puro (NAO C++): typedef struct com alias, sem gets(), sem iostream.
 *  - Leitura segura: fgets + strcspn (remove o '\n' do final).
 *  - Loop de leitura com fread==1 (NAO usa while(!feof)).
 *  - Estilo do professor: conio.h, system cls/pause, getche/getch,
 *    menu do-while+switch, fflush(stdin) com comentario explicando.
 *  - Tudo modularizado em funcoes.
 * ========================================================================= */

/* ---- BIBLIOTECAS PADRAO ---- */
#include <stdio.h>   /* fopen, fwrite, fread, fseek, fclose, printf, scanf  */
#include <stdlib.h>   /* system, exit, malloc, free                           */
#include <string.h>  /* strcpy, strcmp, strstr, strcspn                      */
#include <conio.h>    /* getche, getch, clrscr (estilo do professor)         */

/* ---- CONSTANTES ---- */
#define ARQUIVO "livros.dat"   /* nome do arquivo binario de dados          */
#define TEMP    "temp.dat"     /* arquivo temporario para excluir/alterar   */
#define MAX_TITULO 100         /* tamanho maximo do titulo do livro         */
#define MAX_AUTOR   50         /* tamanho maximo do nome do autor           */

/* =========================================================================
 * STRUCT: representa um Livro.
 * - typedef struct + alias "Livro" para nao precisar escrever "struct Livro"
 *   toda vez (estilo C puro correto).
 * - "disponivel" funciona como flag: 1 = disponivel, 0 = emprestado.
 * ========================================================================= */
typedef struct {
    int  codigo;              /* identificador unico do livro               */
    char titulo[MAX_TITULO];  /* vetor de char para o titulo (string C)     */
    char autor[MAX_AUTOR];    /* vetor de char para o autor                 */
    int  ano;                 /* ano de publicacao                          */
    int  disponivel;          /* 1 = disponivel p/ emprestimo, 0 = ocupado  */
} Livro;

/* =========================================================================
 * PROTOTIPOS DAS FUNCOES (declaradas antes do main para o compilador
 * conhecer as assinaturas quando forem chamadas).
 * ========================================================================= */
void limparBuffer(void);                          /* limpa stdin (Windows)  */
void lerString(char *destino, int tamanho);       /* le string com fgets    */
void pausar(void);                                /* system("pause")        */
void menu(void);                                  /* exibe o menu           */
void cadastrar(void);                             /* CREATE - insere livro  */
void listar(void);                                /* READ - lista todos     */
void buscarPorCodigo(void);                       /* READ - fseek direto    */
void buscarPorTitulo(void);                       /* READ - strstr          */
void emprestar(void);                             /* UPDATE - disponivel=0 */
void devolver(void);                              /* UPDATE - disponivel=1  */
void excluir(void);                               /* DELETE - temp+rename   */
int  codigoExiste(int codigo);                    /* checa duplicidade      */

/* =========================================================================
 * FUNCAO: limparBuffer
 * PORQUE: fflush(stdin) nao e portavel (padrao C diz que fflush e para saida),
 *         mas no Windows/MinGW funciona para limpar o buffer de entrada.
 *         Usamos fflush(stdin) com comentario explicando, conforme o estilo
 *         do professor. Alternativa portavel: ler char ate achar '\n'.
 * ========================================================================= */
void limparBuffer(void) {
    /* fflush(stdin): NAO e portavel segundo o padrao C, mas funciona no
       Windows (MinGW). Por isso usamos aqui. Em Linux/Mac, usar getchar(). */
    fflush(stdin);
}

/* =========================================================================
 * FUNCAO: lerString
 * PORQUE: gets() e perigoso (nao limita tamanho -> buffer overflow) e foi
 *         removido do padrao C11. Usamos fgets() que limita a leitura ao
 *         tamanho do vetor, depois tiramos o '\n' do final com strcspn.
 * PARAMETROS:
 *   destino  - vetor onde a string lida sera guardada
 *   tamanho  - tamanho maximo do vetor (evita estouro de buffer)
 * ========================================================================= */
void lerString(char *destino, int tamanho) {
    /* fgets le ate (tamanho-1) caracteres ou ate achar '\n' (enter).
       O '\n' entra na string, por isso removemos depois. */
    fgets(destino, tamanho, stdin);

    /* strcspn retorna a posicao do primeiro '\n' encontrado.
       Colocamos '\0' nessa posicao, removendo o '\n' do final da string.
       Se nao houver '\n' (usuario digitou mais que o tamanho), o '\n'
       simplesmente nao estara la e nada sera cortado. */
    destino[strcspn(destino, "\n")] = '\0';

    /* Se o usuario digitou mais caracteres que o tamanho do vetor, os
       caracteres extras ficam no buffer do teclado. Precisamos limpar
       para nao sujar a proxima leitura. */
    /* Detectamos isso checando se o ultimo caractere guardado NAO e '\0'
       e se nao encontramos '\n' (ou seja, a leitura encheu o vetor). */
    if (strlen(destino) == (size_t)(tamanho - 1)) {
        /* Ainda pode haver caracteres no buffer -> limpamos. */
        limparBuffer();
    }
}

/* =========================================================================
 * FUNCAO: pausar
 * PORQUE: system("pause") mostra "Pressione qualquer tecla..." e espera.
 *         Simples e funciona no Windows (cmd.exe). Nao e portavel para
 *         Linux/Mac (la nao existe o comando "pause").
 * ========================================================================= */
void pausar(void) {
    system("pause");
}

/* =========================================================================
 * FUNCAO: menu
 * PORQUE: centraliza a exibicao das opcoes. Limpa a tela (system cls) e
 *         mostra as opcoes numeradas.
 * ========================================================================= */
void menu(void) {
    /* system("cls"): limpa a tela no Windows. Nao e portavel (Linux = clear). */
    system("cls");
    printf("===== SISTEMA DE BIBLIOTECA =====\n");
    printf("1. Cadastrar livro\n");
    printf("2. Listar todos os livros\n");
    printf("3. Buscar livro por codigo (fseek)\n");
    printf("4. Buscar livro por titulo (strstr)\n");
    printf("5. Emprestar livro\n");
    printf("6. Devolver livro\n");
    printf("7. Excluir livro\n");
    printf("0. Sair\n");
    printf("=================================\n");
    printf("Opcao: ");
}

/* =========================================================================
 * FUNCAO: codigoExiste
 * PORQUE: ao cadastrar, nao devemos permitir dois livros com o mesmo codigo.
 *         Abrimos o arquivo em modo leitura binaria ("rb") e percorremos
 *         registro a registro com fread==1.
 * RETORNO: 1 se o codigo ja existe, 0 se nao existe.
 * ========================================================================= */
int codigoExiste(int codigo) {
    FILE *fp;            /* ponteiro para o arquivo                       */
    Livro l;             /* variavel para guardar cada registro lido     */
    int existe = 0;      /* flag: 0 = nao existe, 1 = existe             */

    /* fopen com "rb": abre para leitura em modo binario.
       Se o arquivo nao existe ainda (primeiro cadastro), fopen retorna NULL.
       Nesse caso, o codigo certamente nao existe -> retornamos 0. */
    fp = fopen(ARQUIVO, "rb");
    if (fp == NULL) {
        return 0;  /* arquivo nao existe -> nenhum codigo cadastrado ainda */
    }

    /* Loop de leitura: fread retorna o numero de registros lidos.
       Enquanto retornar 1, ha registro para processar.
       NAO usamos while(!feof(fp)) porque feof so retorna verdadeiro DEPOIS
       de uma leitura falha, o que causaria processar um registro fantasma. */
    while (fread(&l, sizeof(Livro), 1, fp) == 1) {
        if (l.codigo == codigo) {
            existe = 1;   /* achamos o codigo -> marcamos como existente */
            break;        /* nao precisamos continuar lendo -> saimos    */
        }
    }

    fclose(fp);   /* sempre feche o arquivo apos usar                    */
    return existe;
}

/* =========================================================================
 * FUNCAO: cadastrar (CREATE)
 * PORQUE: adiciona um novo livro no FIM do arquivo binario.
 *         - Abre em modo "ab" (append binario): se o arquivo nao existe,
 *           ele e criado; se existe, escreve no final sem apagar o conteudo.
 *         - Checa duplicidade de codigo antes de gravar.
 * ========================================================================= */
void cadastrar(void) {
    FILE *fp;       /* ponteiro do arquivo                                */
    Livro l;        /* novo livro a ser preenchido                        */

    system("cls");
    printf("===== CADASTRAR LIVRO =====\n");

    /* --- Leitura do codigo --- */
    printf("Codigo: ");
    scanf("%d", &l.codigo);
    limparBuffer();   /* limpa o '\n' deixado pelo scanf no buffer        */

    /* Checa duplicidade: nao deixa cadastrar dois livros com mesmo codigo. */
    if (codigoExiste(l.codigo)) {
        printf("ERRO: Ja existe um livro com o codigo %d!\n", l.codigo);
        pausar();
        return;   /* sai da funcao sem cadastrar                          */
    }

    /* --- Leitura do titulo (string) --- */
    printf("Titulo: ");
    lerString(l.titulo, MAX_TITULO);   /* fgets + strcspn, sem gets()     */

    /* --- Leitura do autor (string) --- */
    printf("Autor: ");
    lerString(l.autor, MAX_AUTOR);

    /* --- Leitura do ano --- */
    printf("Ano: ");
    scanf("%d", &l.ano);
    limparBuffer();

    /* Livro recem cadastrado comeca como disponivel (1). */
    l.disponivel = 1;

    /* --- Gravacao no arquivo --- */
    /* "ab" = append binario: escreve no final, cria se nao existir. */
    fp = fopen(ARQUIVO, "ab");
    if (fp == NULL) {
        printf("ERRO: nao foi possivel abrir o arquivo para escrita!\n");
        pausar();
        return;
    }

    /* fwrite: grava 1 registro (sizeof(Livro) bytes) a partir de &l.
       Conferimos o retorno: se for diferente de 1, houve erro de gravacao. */
    if (fwrite(&l, sizeof(Livro), 1, fp) != 1) {
        printf("ERRO: falha ao gravar o livro no arquivo!\n");
    } else {
        printf("Livro cadastrado com sucesso!\n");
    }

    fclose(fp);   /* fecha o arquivo (libera o conteudo do buffer p/ disco) */
    pausar();
}

/* =========================================================================
 * FUNCAO: listar (READ - todos)
 * PORQUE: percorre o arquivo inteiro e mostra todos os livros cadastrados.
 *         Usa fread==1 no loop (NAO while(!feof)).
 * ========================================================================= */
void listar(void) {
    FILE *fp;
    Livro l;
    int contador = 0;   /* conta quantos livros foram listados           */

    system("cls");
    printf("===== LISTA DE LIVROS =====\n\n");

    /* "rb" = leitura binaria. Se retornar NULL, o arquivo nao existe. */
    fp = fopen(ARQUIVO, "rb");
    if (fp == NULL) {
        printf("Nenhum livro cadastrado ainda (arquivo nao existe).\n");
        pausar();
        return;
    }

    /* Cabecalho da tabela. %-30s alinha a string a esquerda com 30 espacos. */
    printf("%-6s %-30s %-20s %-6s %-10s\n",
           "COD", "TITULO", "AUTOR", "ANO", "STATUS");
    printf("--------------------------------------------------------------\n");

    /* fread==1: le um registro por vez. Quando retornar 0, chegamos ao fim. */
    while (fread(&l, sizeof(Livro), 1, fp) == 1) {
        /* Mostra os dados do livro. O operador ternario (?) escolhe o texto
           do status conforme o valor de disponivel. */
        printf("%-6d %-30.30s %-20.20s %-6d %s\n",
               l.codigo,
               l.titulo,
               l.autor,
               l.ano,
               l.disponivel ? "Disponivel" : "Emprestado");
        contador++;
    }

    fclose(fp);

    if (contador == 0) {
        printf("\nNenhum livro encontrado no arquivo.\n");
    } else {
        printf("\nTotal de livros: %d\n", contador);
    }

    pausar();
}

/* =========================================================================
 * FUNCAO: buscarPorCodigo (READ - acesso direto com fseek)
 * PORQUE: demonstra o uso de fseek para acesso direto (random access).
 *         Em vez de ler todos os registros ate achar o desejado, calculamos
 *         a posicao (offset) do registro no arquivo e pulamos direto para ele.
 *
 *         IMPORTANTE: este metodo assume que os registros estao ordenados
 *         por codigo E que o codigo corresponde a posicao. Para um sistema
 *         real, faria-se busca linear ou indice. Aqui mostramos o conceito
 *         de fseek com duas abordagens:
 *           (a) Busca linear usando fread==1 (sempre funciona).
 *           (b) Acesso direto por fseek (quando codigo = posicao+1).
 *         Implementamos a busca linear para ser correta em qualquer caso,
 *         e mostramos o fseek como alternativa comentada.
 * ========================================================================= */
void buscarPorCodigo(void) {
    FILE *fp;
    Livro l;
    int codigoBusca;
    int achou = 0;   /* flag: 1 se encontrou o livro                     */

    system("cls");
    printf("===== BUSCAR LIVRO POR CODIGO =====\n");
    printf("Digite o codigo: ");
    scanf("%d", &codigoBusca);
    limparBuffer();

    fp = fopen(ARQUIVO, "rb");
    if (fp == NULL) {
        printf("Arquivo nao existe. Nenhum livro cadastrado.\n");
        pausar();
        return;
    }

    /* --- ABORDAGEM 1: busca linear com fread==1 (sempre correta) --- */
    while (fread(&l, sizeof(Livro), 1, fp) == 1) {
        if (l.codigo == codigoBusca) {
            achou = 1;
            break;   /* achou -> sai do loop                            */
        }
    }

    /* --- ABORDAGEM 2 (ALTERNATIVA COM fseek - acesso direto) ---
     * Se os codigos fossem sequenciais (1, 2, 3...) e correspondessem a
     * posicao fisica no arquivo, poderiamos pular direto para o registro:
     *
     *   long offset = (codigoBusca - 1) * sizeof(Livro);
     *   fseek(fp, offset, SEEK_SET);   // SEEK_SET = inicio do arquivo
     *   fread(&l, sizeof(Livro), 1, fp);
     *
     * fseek(fp, offset, SEEK_SET): move o cursor do arquivo para a posicao
     * "offset" a partir do inicio (SEEK_SET). SEEK_CUR = posicao atual,
     * SEEK_END = final do arquivo.
     *
     * Como neste sistema o codigo e livre (digitado pelo usuario), usamos
     * a busca linear. O fseek fica aqui como exemplo didatico.
     */

    if (achou) {
        printf("\nLivro encontrado!\n");
        printf("Codigo: %d\n", l.codigo);
        printf("Titulo: %s\n", l.titulo);
        printf("Autor:  %s\n", l.autor);
        printf("Ano:    %d\n", l.ano);
        printf("Status: %s\n", l.disponivel ? "Disponivel" : "Emprestado");
    } else {
        printf("\nLivro com codigo %d nao encontrado.\n", codigoBusca);
    }

    fclose(fp);
    pausar();
}

/* =========================================================================
 * FUNCAO: buscarPorTitulo (READ - busca parcial com strstr)
 * PORQUE: strstr procura uma substring dentro de uma string maior.
 *         Retorna um ponteiro para a primeira ocorrencia, ou NULL se nao
 *         achar. Assim o usuario pode digitar apenas parte do titulo.
 *         Ex: buscar "senhor" acha "O Senhor dos Aneis".
 * ========================================================================= */
void buscarPorTitulo(void) {
    FILE *fp;
    Livro l;
    char trecho[MAX_TITULO];   /* o que o usuario vai digitar            */
    int achou = 0;             /* flag: pelo menos um livro encontrado   */

    system("cls");
    printf("===== BUSCAR LIVRO POR TITULO =====\n");
    printf("Digite parte do titulo: ");
    lerString(trecho, MAX_TITULO);   /* le o trecho com fgets            */

    fp = fopen(ARQUIVO, "rb");
    if (fp == NULL) {
        printf("Arquivo nao existe. Nenhum livro cadastrado.\n");
        pausar();
        return;
    }

    printf("\nResultados:\n");
    printf("--------------------------------------------------------------\n");

    /* Percorre todos os registros. */
    while (fread(&l, sizeof(Livro), 1, fp) == 1) {
        /* strstr(str1, str2): procura str2 dentro de str1.
           Retorna ponteiro != NULL se achar, NULL se nao achar.
           Aqui procuramos o trecho digitado dentro do titulo do livro. */
        if (strstr(l.titulo, trecho) != NULL) {
            printf("COD:%-6d | %-30.30s | %s | %s\n",
                   l.codigo,
                   l.titulo,
                   l.autor,
                   l.disponivel ? "Disponivel" : "Emprestado");
            achou = 1;
        }
    }

    if (!achou) {
        printf("Nenhum livro encontrado com \"%s\" no titulo.\n", trecho);
    }

    fclose(fp);
    pausar();
}

/* =========================================================================
 * FUNCAO: emprestar (UPDATE)
 * PORQUE: marca um livro como emprestado (disponivel = 0).
 *         Como arquivos binarios nao permitem "inserir no meio", a tecnica
 *         e: ler o registro, alterar na memoria, e regravar por cima na
 *         MESMA posicao usando fseek para voltar o cursor.
 *
 *         Aqui usamos a tecnica do arquivo temporario (temp + rename),
 *         que tambem serve para excluir. Copiamos todos os registros para
 *         temp.dat, alterando o que tem o codigo buscado, e depois
 *         renomeamos temp.dat para livros.dat.
 * ========================================================================= */
void emprestar(void) {
    FILE *fp, *ft;     /* fp = original, ft = temporario                */
    Livro l;
    int codigoBusca;
    int achou = 0;     /* flag: 1 se o livro foi encontrado e alterado   */

    system("cls");
    printf("===== EMPRESTAR LIVRO =====\n");
    printf("Digite o codigo do livro: ");
    scanf("%d", &codigoBusca);
    limparBuffer();

    fp = fopen(ARQUIVO, "rb");   /* abre original para leitura           */
    if (fp == NULL) {
        printf("Arquivo nao existe. Nenhum livro cadastrado.\n");
        pausar();
        return;
    }

    /* "wb" = escrita binaria (cria do zero / apaga se existir). */
    ft = fopen(TEMP, "wb");
    if (ft == NULL) {
        printf("ERRO: nao foi possivel criar arquivo temporario!\n");
        fclose(fp);
        pausar();
        return;
    }

    /* Le cada registro do original e copia para o temporario.
       Se for o livro buscado, altera disponivel para 0 antes de copiar. */
    while (fread(&l, sizeof(Livro), 1, fp) == 1) {
        if (l.codigo == codigoBusca) {
            achou = 1;
            if (l.disponivel == 1) {
                l.disponivel = 0;   /* marca como emprestado            */
                printf("Livro \"%s\" emprestado com sucesso!\n", l.titulo);
            } else {
                /* O livro ja estava emprestado -> nao alteramos. */
                printf("AVISO: o livro ja estava emprestado!\n");
            }
        }
        /* Grava o registro (alterado ou nao) no arquivo temporario. */
        fwrite(&l, sizeof(Livro), 1, ft);
    }

    fclose(fp);
    fclose(ft);

    if (achou) {
        /* remove o arquivo original e renomeia o temporario para o nome
           original. Assim o temp vira o novo livros.dat. */
        remove(ARQUIVO);
        rename(TEMP, ARQUIVO);
    } else {
        /* Se nao achou o livro, apagamos o temp (que e igual ao original). */
        remove(TEMP);
        printf("Livro com codigo %d nao encontrado.\n", codigoBusca);
    }

    pausar();
}

/* =========================================================================
 * FUNCAO: devolver (UPDATE)
 * PORQUE: inverso do emprestar. Marca disponivel = 1.
 *         Mesma tecnica de temp + rename.
 * ========================================================================= */
void devolver(void) {
    FILE *fp, *ft;
    Livro l;
    int codigoBusca;
    int achou = 0;

    system("cls");
    printf("===== DEVOLVER LIVRO =====\n");
    printf("Digite o codigo do livro: ");
    scanf("%d", &codigoBusca);
    limparBuffer();

    fp = fopen(ARQUIVO, "rb");
    if (fp == NULL) {
        printf("Arquivo nao existe. Nenhum livro cadastrado.\n");
        pausar();
        return;
    }

    ft = fopen(TEMP, "wb");
    if (ft == NULL) {
        printf("ERRO: nao foi possivel criar arquivo temporario!\n");
        fclose(fp);
        pausar();
        return;
    }

    while (fread(&l, sizeof(Livro), 1, fp) == 1) {
        if (l.codigo == codigoBusca) {
            achou = 1;
            if (l.disponivel == 0) {
                l.disponivel = 1;   /* marca como disponivel novamente   */
                printf("Livro \"%s\" devolvido com sucesso!\n", l.titulo);
            } else {
                /* O livro ja estava disponivel -> nada a fazer. */
                printf("AVISO: o livro ja estava disponivel!\n");
            }
        }
        fwrite(&l, sizeof(Livro), 1, ft);
    }

    fclose(fp);
    fclose(ft);

    if (achou) {
        remove(ARQUIVO);
        rename(TEMP, ARQUIVO);
    } else {
        remove(TEMP);
        printf("Livro com codigo %d nao encontrado.\n", codigoBusca);
    }

    pausar();
}

/* =========================================================================
 * FUNCAO: excluir (DELETE)
 * PORQUE: remove um livro do arquivo. Tecnica classica com arquivo
 *         temporario: copia todos os registros EXCETO o que tem o codigo
 *         buscado para temp.dat, depois renomeia temp.dat para livros.dat.
 *         O registro excluido simplesmente nao e copiado.
 * ========================================================================= */
void excluir(void) {
    FILE *fp, *ft;
    Livro l;
    int codigoBusca;
    int achou = 0;
    char confirma;   /* guarda a resposta do usuario (s/n)              */

    system("cls");
    printf("===== EXCLUIR LIVRO =====\n");
    printf("Digite o codigo do livro a excluir: ");
    scanf("%d", &codigoBusca);
    limparBuffer();

    fp = fopen(ARQUIVO, "rb");
    if (fp == NULL) {
        printf("Arquivo nao existe. Nenhum livro cadastrado.\n");
        pausar();
        return;
    }

    /* Primeiro, vamos mostrar o livro para o usuario confirmar. */
    printf("\nProcurando livro...\n");
    while (fread(&l, sizeof(Livro), 1, fp) == 1) {
        if (l.codigo == codigoBusca) {
            achou = 1;
            printf("Livro encontrado:\n");
            printf("  Codigo: %d\n", l.codigo);
            printf("  Titulo: %s\n", l.titulo);
            printf("  Autor:  %s\n", l.autor);
            printf("  Ano:    %d\n", l.ano);
            break;
        }
    }

    if (!achou) {
        printf("Livro com codigo %d nao encontrado.\n", codigoBusca);
        fclose(fp);
        pausar();
        return;
    }

    /* Confirmacao: getche le um caractere com eco (mostra na tela). */
    printf("\nTem certeza que deseja excluir? (s/n): ");
    confirma = getche();   /* le 1 tecla e mostra na tela (com eco)      */
    printf("\n");

    if (confirma != 's' && confirma != 'S') {
        printf("Operacao cancelada pelo usuario.\n");
        fclose(fp);
        pausar();
        return;
    }

    /* Reposiciona o cursor no inicio do arquivo para reprocessar. */
    rewind(fp);

    /* Cria o temporario. */
    ft = fopen(TEMP, "wb");
    if (ft == NULL) {
        printf("ERRO: nao foi possivel criar arquivo temporario!\n");
        fclose(fp);
        pausar();
        return;
    }

    /* Copia todos os registros EXCETO o que tem o codigo buscado. */
    while (fread(&l, sizeof(Livro), 1, fp) == 1) {
        if (l.codigo != codigoBusca) {
            /* Nao e o livro excluido -> copia para o temporario. */
            fwrite(&l, sizeof(Livro), 1, ft);
        }
        /* Se for o livro excluido, simplesmente nao escrevemos (some). */
    }

    fclose(fp);
    fclose(ft);

    /* Substitui o original pelo temporario. */
    remove(ARQUIVO);
    rename(TEMP, ARQUIVO);

    printf("Livro excluido com sucesso!\n");
    pausar();
}

/* =========================================================================
 * FUNCAO: main
 * PORQUE: ponto de entrada. Controla o loop principal do menu com
 *         do-while + switch, chamando as funcoes modularizadas.
 * ========================================================================= */
int main(void) {
    int opcao;   /* guarda a opcao escolhida pelo usuario                */

    /* do-while: executa o menu pelo menos uma vez e repete ate opcao == 0. */
    do {
        menu();                    /* mostra as opcoes na tela           */
        scanf("%d", &opcao);       /* le a escolha do usuario            */
        limparBuffer();            /* limpa o '\n' deixado pelo scanf    */

        /* switch: direciona para a funcao conforme a opcao. */
        switch (opcao) {
            case 1:
                cadastrar();        /* CREATE                            */
                break;
            case 2:
                listar();           /* READ (todos)                      */
                break;
            case 3:
                buscarPorCodigo(); /* READ (fseek / acesso direto)      */
                break;
            case 4:
                buscarPorTitulo(); /* READ (strstr / busca parcial)     */
                break;
            case 5:
                emprestar();        /* UPDATE (disponivel = 0)           */
                break;
            case 6:
                devolver();         /* UPDATE (disponivel = 1)           */
                break;
            case 7:
                excluir();          /* DELETE                            */
                break;
            case 0:
                printf("Encerrando o programa. Ate logo!\n");
                break;
            default:
                /* Opcao invalida: avisa o usuario e volta ao menu. */
                printf("Opcao invalida! Tente novamente.\n");
                pausar();
                break;
        }
    } while (opcao != 0);   /* repete enquanto nao escolher sair (0)    */

    return 0;   /* retorno 0 indica que o programa terminou com sucesso  */
}

/* =========================================================================
 * RESUMO DAS TECNICAS DEMONSTRADAS (para revisao da prova):
 * -------------------------------------------------------------------------
 * 1. typedef struct com alias -> "Livro" em vez de "struct Livro".
 * 2. Arquivo binario: fopen "rb"/"wb"/"ab", fwrite, fread, fclose.
 * 3. Loop de leitura correto: while (fread(...) == 1) - NAO while(!feof).
 * 4. fseek: acesso direto (SEEK_SET, SEEK_CUR, SEEK_END) - ver buscarPorCodigo.
 * 5. strstr: busca parcial de substring - ver buscarPorTitulo.
 * 6. CRUD completo: cadastrar(C), listar/buscar(R), emprestar/devolver(U),
 *    excluir(D).
 * 7. Tecnica temp + rename: para alterar ou excluir registros em arquivo.
 * 8. Leitura segura de string: fgets + strcspn (sem gets).
 * 9. Modularizacao: cada operacao em sua propria funcao.
 * 10. Menu do-while + switch com conio.h (getche, system cls/pause).
 * 11. fflush(stdin) com comentario explicando que nao e portavel mas
 *     funciona no Windows.
 * 12. Checagem de duplicidade de codigo antes de cadastrar.
 * 13. Confirmacao antes de excluir (getche para s/n).
 * =========================================================================
 * FIM DO ARQUIVO
 * ========================================================================= */
