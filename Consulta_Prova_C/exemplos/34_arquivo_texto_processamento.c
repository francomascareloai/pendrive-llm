/*
=====================================================================
   34_arquivo_texto_processamento.c
---------------------------------------------------------------------
   O QUE FAZ: Menu didatico que processa arquivos TEXTO (.txt) em C.
   Mostra como ler linha a linha com fgets, contar linhas, contar
   palavras, contar vogais totais, achar a linha mais longa, buscar
   uma palavra no arquivo (usando strstr) e copiar o conteudo de um
   arquivo para outro (copia de arquivo).

   TOPICO: Arquivos texto - processamento completo.
   PALAVRAS-CHAVE Ctrl+F:
     arquivo texto | fgets | fopen | fclose | contar linhas
     contar palavras | contar vogais | linha mais longa
     strstr | buscar palavra | copiar arquivo | fputc | fgetc
     menu do-while switch | conio.h | system cls | system pause
     fflush stdin | strcspn | strtok | tolower | typedef struct

   COMPILA: gcc -Wall -Wextra 34_arquivo_texto_processamento.c -o prog
   (MinGW no Windows). conio.h funciona no Windows.
=====================================================================
*/

/* ---- Bibliotecas padrao do C ---- */
#include <stdio.h>   /* fopen, fgets, fgetc, fputc, fclose, printf, scanf */
#include <stdlib.h>  /* exit, EXIT_FAILURE, EXIT_SUCCESS */
#include <string.h>  /* strlen, strcspn, strstr, strcpy, strtok */
#include <ctype.h>   /* tolower, isspace (testa espaco em branco) */
#include <conio.h>   /* getche, getch - le tecla sem Enter (Windows) */

/* -------------------------------------------------------------------
   TAMANHO maximo de uma linha lida do arquivo.
   1024 e generoso para arquivos didaticos. Se a linha for maior,
   fgets le apenas os primeiros 1023 caracteres (+ '\0').
   ------------------------------------------------------------------- */
#define TAM_LINHA 1024

/* -------------------------------------------------------------------
   Struct que guarda as ESTATISTICAS de um arquivo texto.
   Usamos typedef struct com alias (Estats) para nao precisar escrever
   "struct estatisticas" toda hora - estilo recomendado pelo professor.
   ------------------------------------------------------------------- */
typedef struct estatisticas {
    int total_linhas;     /* quantas linhas o arquivo possui */
    int total_palavras;   /* quantas palavras no arquivo inteiro */
    int total_vogais;     /* quantas vogais (a,e,i,o,u) no arquivo */
    int tam_linha_maior;  /* tamanho da maior linha encontrada */
    char linha_maior[TAM_LINHA]; /* conteudo da maior linha */
} Estats;

/* ===================================================================
   PROTOTIPOS das funcoes (declaradas antes do main para o compilador
   conhecer as assinaturas). Modularizar deixa o main limpo.
   =================================================================== */
void limpar_tela(void);
void pausar(void);
void limpar_buffer_entrada(void);
void criar_arquivo_exemplo(const char *nome_arq);
Estats processar_arquivo(const char *nome_arq);
void menu_contar_linhas(void);
void menu_contar_palavras(void);
void menu_contar_vogais(void);
void menu_linha_mais_longa(void);
void menu_buscar_palavra(void);
void menu_copiar_arquivo(void);
void menu_estatisticas_completas(void);

/* ===================================================================
   FUNCOES AUXILIARES DE INTERFACE (padrao do professor)
   =================================================================== */

/* limpar_tela: chama system("cls") para limpar a tela no Windows.
   Encapsulado em funcao para reaproveitar e deixar o codigo limpo. */
void limpar_tela(void) {
    system("cls"); /* cls = clear screen no Windows (no Linux seria "clear") */
}

/* pausar: mostra mensagem e espera o usuario teclar algo.
   Usa getch() da conio.h: le 1 tecla sem ecoar e sem precisar de Enter. */
void pausar(void) {
    printf("\nPressione qualquer tecla para voltar ao menu...");
    getch(); /* espera 1 tecla - so funciona no Windows (conio.h) */
}

/* limpar_buffer_entrada: descarta caracteres restantes no stdin.
   IMPORTANTE: apos um scanf, costuma sobrar um '\n' no buffer que
   atrapalha o proximo fgets. Por isso limpamos o buffer.
   fflush(stdin) NAO e portavel (o padrao C diz que fflush e so para
   saida), mas no Windows com MinGW funciona na pratica. Aqui usamos o
   metodo portavel com getchar() para ser mais correto. */
void limpar_buffer_entrada(void) {
    int c;
    /* consome todos os caracteres ate encontrar '\n' ou fim de arquivo */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* corpo vazio: so queremos descartar os caracteres */
    }
}

/* ===================================================================
   criar_arquivo_exemplo: cria um arquivo .txt de teste para que o
   aluno possa rodar o programa sem precisar digitar nada antes.
   Recebe o nome do arquivo a ser criado.
   =================================================================== */
void criar_arquivo_exemplo(const char *nome_arq) {
    /* Abre para escrita ("w"). Se o arquivo existir, sera sobrescrito.
       Se nao existir, sera criado. */
    FILE *fp = fopen(nome_arq, "w");
    if (fp == NULL) {
        /* Se fopen retornar NULL, houve erro (ex.: disco cheio, caminho
           invalido). Mostramos mensagem e saimos. */
        printf("Erro ao criar o arquivo de exemplo.\n");
        exit(EXIT_FAILURE);
    }

    /* fprintf escreve texto formatado no arquivo, igual ao printf
       faz na tela, mas direcionado para o FILE* passado. */
    fprintf(fp, "Algoritmos e Programacao II\n");
    fprintf(fp, "Prova do Grau B da Uniftec\n");
    fprintf(fp, "Trabalhar com arquivos em C e muito importante\n");
    fprintf(fp, "O fgets le linha por linha com seguranca\n");
    fprintf(fp, "Fim do arquivo de exemplo\n");

    /* Sempre feche o arquivo apos terminar de usa-lo. Isso garante que
       os dados sejam gravados (flush) e libera o recurso. */
    fclose(fp);
}

/* ===================================================================
   processar_arquivo: funcao CENTRAL deste programa.
   Abre o arquivo texto em modo leitura ("r") e percorre linha a linha
   com fgets. A cada linha, atualiza as estatisticas (linhas, palavras,
   vogais, linha mais longa).

   Retorna uma struct Estats preenchida. Assim o main pode imprimir
   o que quiser depois - separacao entre "calcular" e "mostrar".

   PADRAO CORRETO de leitura com fgets:
     while (fgets(linha, TAM, fp) != NULL) { ... }
   NAO usamos while(!feof(fp)) porque feof so retorna verdadeiro DEPOIS
   de uma leitura falhar, o que causaria iteracao extra (bug classico).
   =================================================================== */
Estats processar_arquivo(const char *nome_arq) {
    Estats e;                       /* struct local com as estatisticas */
    char linha[TAM_LINHA];          /* buffer para 1 linha do arquivo */
    FILE *fp;                       /* ponteiro para o arquivo aberto */

    /* inicializa todos os campos da struct com zero/strings vazias.
       Zerar antes de acumular evita lixo de memoria. */
    e.total_linhas = 0;
    e.total_palavras = 0;
    e.total_vogais = 0;
    e.tam_linha_maior = 0;
    e.linha_maior[0] = '\0';       /* string vazia */

    /* Abre o arquivo em modo leitura de texto ("r").
       Se o arquivo nao existir, fopen retorna NULL. */
    fp = fopen(nome_arq, "r");
    if (fp == NULL) {
        printf("ERRO: nao foi possivel abrir '%s'. Ele existe?\n", nome_arq);
        /* Retorna a struct zerada para indicar que nada foi processado.
           O chamador pode checar total_linhas==0 para saber se falhou. */
        return e;
    }

    /* LACO PRINCIPAL: le uma linha por vez com fgets.
       fgets le no maximo TAM_LINHA-1 caracteres e adiciona '\0' no fim.
       Para no '\n' (inclui o '\n' na string) ou no fim do arquivo.
       Retorna NULL quando nao ha mais o que ler (fim do arquivo). */
    while (fgets(linha, TAM_LINHA, fp) != NULL) {
        int i;             /* contador generico para percorrer a linha */
        int em_palavra = 0;/* flag: 1 se estamos dentro de uma palavra */

        /* --- Conta 1 linha a mais --- */
        e.total_linhas++;

        /* strcspn encontra a primeira posicao onde aparece '\n'.
           Se houver '\n', substituimos por '\0' para "limpar" a quebra
           de linha. Isso facilita contar vogais e comparar tamanhos.
           strcspn retorna o indice do '\n' (ou o tamanho se nao houver). */
        linha[strcspn(linha, "\n")] = '\0';

        /* --- Conta vogais: percorre cada caractere da linha --- */
        for (i = 0; linha[i] != '\0'; i++) {
            /* tolower converte para minuscula para comparar so uma vez.
               Assim 'A' e 'a' contam como vogal 'a'. */
            char ch = (char)tolower((unsigned char)linha[i]);
            if (ch == 'a' || ch == 'e' || ch == 'i' ||
                ch == 'o' || ch == 'u') {
                e.total_vogais++;
            }
        }

        /* --- Conta palavras: tecnica da "maquina de estados" ---.
           Ideia: quando passamos de "espaco" para "nao-espaco",
           comecamos uma palavra nova -> incrementamos o contador.
           isspace testa espaco, tab, '\n' etc. (ctype.h). */
        for (i = 0; linha[i] != '\0'; i++) {
            if (isspace((unsigned char)linha[i])) {
                em_palavra = 0; /* estamos em branco: fora de palavra */
            } else {
                /* caractere valido: se nao estavamos em palavra,
                   acabamos de entrar em uma -> conta +1 palavra. */
                if (em_palavra == 0) {
                    e.total_palavras++;
                    em_palavra = 1; /* agora estamos dentro de palavra */
                }
                /* se ja estavamos em palavra, so continua (mesma palavra) */
            }
        }

        /* --- Acha a linha mais longa ---.
           strlen retorna o tamanho (numero de caracteres) da string,
           sem contar o '\0'. Comparamos com o maior ja encontrado. */
        if ((int)strlen(linha) > e.tam_linha_maior) {
            e.tam_linha_maior = (int)strlen(linha);
            /* strcpy copia a linha inteira para linha_maior.
             Como linha_maior tem TAM_LINHA de espaco, cabe com folga. */
            strcpy(e.linha_maior, linha);
        }
    }

    /* Fecha o arquivo apos terminar a leitura. Boa pratica: sempre feche! */
    fclose(fp);

    /* Retorna a struct preenchida com todas as estatisticas. */
    return e;
}

/* ===================================================================
   MENU: Contar linhas do arquivo
   =================================================================== */
void menu_contar_linhas(void) {
    char nome_arq[100]; /* nome do arquivo digitado pelo usuario */
    Estats e;

    limpar_tela();
    printf("=== CONTAR LINHAS DO ARQUIVO ===\n\n");
    printf("Digite o nome do arquivo (ex.: dados.txt): ");
    /* fgets para ler o nome do arquivo com seguranca (sem gets!).
       gets e perigoso e foi removido do C11 - nunca use gets. */
    if (fgets(nome_arq, sizeof(nome_arq), stdin) == NULL) {
        printf("Erro na leitura.\n");
        pausar();
        return;
    }
    /* Remove o '\n' final que o fgets captura. */
    nome_arq[strcspn(nome_arq, "\n")] = '\0';

    /* Chama a funcao central de processamento. */
    e = processar_arquivo(nome_arq);

    /* Se total_linhas==0, pode ser arquivo vazio OU erro de abertura.
       A funcao ja imprimiu mensagem de erro nesse caso. */
    if (e.total_linhas > 0) {
        printf("\nO arquivo '%s' possui %d linha(s).\n",
               nome_arq, e.total_linhas);
    }
    pausar();
}

/* ===================================================================
   MENU: Contar palavras do arquivo
   =================================================================== */
void menu_contar_palavras(void) {
    char nome_arq[100];
    Estats e;

    limpar_tela();
    printf("=== CONTAR PALAVRAS DO ARQUIVO ===\n\n");
    printf("Digite o nome do arquivo (ex.: dados.txt): ");
    if (fgets(nome_arq, sizeof(nome_arq), stdin) == NULL) {
        printf("Erro na leitura.\n");
        pausar();
        return;
    }
    nome_arq[strcspn(nome_arq, "\n")] = '\0';

    e = processar_arquivo(nome_arq);
    if (e.total_linhas > 0) {
        printf("\nO arquivo '%s' possui %d palavra(s).\n",
               nome_arq, e.total_palavras);
    }
    pausar();
}

/* ===================================================================
   MENU: Contar vogais totais do arquivo
   =================================================================== */
void menu_contar_vogais(void) {
    char nome_arq[100];
    Estats e;

    limpar_tela();
    printf("=== CONTAR VOGAIS DO ARQUIVO ===\n\n");
    printf("Digite o nome do arquivo (ex.: dados.txt): ");
    if (fgets(nome_arq, sizeof(nome_arq), stdin) == NULL) {
        printf("Erro na leitura.\n");
        pausar();
        return;
    }
    nome_arq[strcspn(nome_arq, "\n")] = '\0';

    e = processar_arquivo(nome_arq);
    if (e.total_linhas > 0) {
        printf("\nO arquivo '%s' possui %d vogal(is) (a,e,i,o,u).\n",
               nome_arq, e.total_vogais);
    }
    pausar();
}

/* ===================================================================
   MENU: Achar a linha mais longa do arquivo
   =================================================================== */
void menu_linha_mais_longa(void) {
    char nome_arq[100];
    Estats e;

    limpar_tela();
    printf("=== LINHA MAIS LONGA DO ARQUIVO ===\n\n");
    printf("Digite o nome do arquivo (ex.: dados.txt): ");
    if (fgets(nome_arq, sizeof(nome_arq), stdin) == NULL) {
        printf("Erro na leitura.\n");
        pausar();
        return;
    }
    nome_arq[strcspn(nome_arq, "\n")] = '\0';

    e = processar_arquivo(nome_arq);
    if (e.total_linhas > 0) {
        printf("\nLinha mais longa tem %d caractere(s):\n",
               e.tam_linha_maior);
        printf("\"%s\"\n", e.linha_maior);
    }
    pausar();
}

/* ===================================================================
   MENU: Buscar uma palavra no arquivo (usando strstr)
   -------------------------------------------------------------------
   strstr(procurar_em, palavra) retorna:
     - ponteiro diferente de NULL se encontrar a palavra
     - NULL se nao encontrar
   Aqui percorremos o arquivo linha a linha e, para cada linha,
   verificamos se a palavra aparece com strstr.
   =================================================================== */
void menu_buscar_palavra(void) {
    char nome_arq[100];
    char palavra[100];   /* palavra que o usuario quer buscar */
    char linha[TAM_LINHA];
    FILE *fp;
    int linha_num;       /* numero da linha atual (para exibir onde achou) */
    int achou_vezes;     /* quantas vezes a palavra foi encontrada */

    limpar_tela();
    printf("=== BUSCAR PALAVRA NO ARQUIVO (strstr) ===\n\n");
    printf("Digite o nome do arquivo (ex.: dados.txt): ");
    if (fgets(nome_arq, sizeof(nome_arq), stdin) == NULL) {
        printf("Erro na leitura.\n");
        pausar();
        return;
    }
    nome_arq[strcspn(nome_arq, "\n")] = '\0';

    printf("Digite a palavra a buscar: ");
    if (fgets(palavra, sizeof(palavra), stdin) == NULL) {
        printf("Erro na leitura.\n");
        pausar();
        return;
    }
    palavra[strcspn(palavra, "\n")] = '\0';

    /* Abre o arquivo para leitura. */
    fp = fopen(nome_arq, "r");
    if (fp == NULL) {
        printf("ERRO: nao foi possivel abrir '%s'.\n", nome_arq);
        pausar();
        return;
    }

    linha_num = 0;       /* comeca na linha 0, incrementa antes de testar */
    achou_vezes = 0;     /* contador de ocorrencias */

    printf("\n--- Resultado da busca ---\n");
    /* Laco padrao com fgets: le linha por linha ate o fim do arquivo. */
    while (fgets(linha, TAM_LINHA, fp) != NULL) {
        linha_num++; /* estamos na proxima linha */
        /* Remove o '\n' para exibir a linha de forma limpa. */
        linha[strcspn(linha, "\n")] = '\0';

        /* strstr procura a 'palavra' dentro de 'linha'.
           Retorna um ponteiro para a 1a ocorrencia, ou NULL se nao achar.
           IMPORTANTE: strstr diferencia maiusculas de minusculas.
           Para busca sem diferenciar, converteriamos ambos para
           minusculas antes (com tolower em um laco). */
        if (strstr(linha, palavra) != NULL) {
            printf("Linha %d: \"%s\"\n", linha_num, linha);
            achou_vezes++;
        }
    }

    fclose(fp); /* sempre feche o arquivo */

    /* Resumo final da busca. */
    if (achou_vezes == 0) {
        printf("Palavra \"%s\" nao encontrada no arquivo.\n", palavra);
    } else {
        printf("\nTotal: a palavra apareceu em %d linha(s).\n", achou_vezes);
    }
    pausar();
}

/* ===================================================================
   MENU: Copiar arquivo (copia o conteudo de origem para destino)
   -------------------------------------------------------------------
   Estrategia: ler caractere por caractere com fgetc e escrever com
   fputc. Tambem poderiamos ler linha a linha com fgets e escrever com
   fputs - ambas funcionam. fgetc/fputc e o exemplo mais classico.
   =================================================================== */
void menu_copiar_arquivo(void) {
    char origem[100];    /* nome do arquivo de origem */
    char destino[100];  /* nome do arquivo de destino */
    FILE *fp_in;         /* ponteiro para o arquivo de origem (leitura) */
    FILE *fp_out;        /* ponteiro para o arquivo de destino (escrita) */
    int ch;              /* guarda 1 caractere lido por fgetc */

    limpar_tela();
    printf("=== COPIAR ARQUIVO ===\n\n");
    printf("Digite o nome do arquivo ORIGEM: ");
    if (fgets(origem, sizeof(origem), stdin) == NULL) {
        printf("Erro na leitura.\n");
        pausar();
        return;
    }
    origem[strcspn(origem, "\n")] = '\0';

    printf("Digite o nome do arquivo DESTINO: ");
    if (fgets(destino, sizeof(destino), stdin) == NULL) {
        printf("Erro na leitura.\n");
        pausar();
        return;
    }
    destino[strcspn(destino, "\n")] = '\0';

    /* Abre origem para leitura ("r"). */
    fp_in = fopen(origem, "r");
    if (fp_in == NULL) {
        printf("ERRO: nao foi possivel abrir a origem '%s'.\n", origem);
        pausar();
        return;
    }

    /* Abre destino para escrita ("w"). Se existir, sera sobrescrito. */
    fp_out = fopen(destino, "w");
    if (fp_out == NULL) {
        printf("ERRO: nao foi possivel criar o destino '%s'.\n", destino);
        fclose(fp_in); /* fecha a origem antes de sair para nao vazar */
        pausar();
        return;
    }

    /* LACO DE COPIA: fgetc le 1 caractere por vez.
       Retorna o caractere lido (0..255) ou a constante EOF (fim do arquivo).
       Por isso 'ch' deve ser int (nao char!) para conseguir guardar EOF.
       Escrevemos o caractere lido no destino com fputc. */
    while ((ch = fgetc(fp_in)) != EOF) {
        fputc(ch, fp_out); /* escreve 1 caractere no arquivo de destino */
    }

    /* Fecha os DOIS arquivos. Ordem nao importa muito, mas feche sempre. */
    fclose(fp_in);
    fclose(fp_out);

    printf("\nArquivo copiado com sucesso: '%s' -> '%s'\n", origem, destino);
    pausar();
}

/* ===================================================================
   MENU: Estatisticas completas (mostra tudo de uma vez)
   -------------------------------------------------------------------
   Chama processar_arquivo uma unica vez e exibe todas as estatisticas.
   E mais eficiente que chamar varias vezes (abre o arquivo so 1 vez).
   =================================================================== */
void menu_estatisticas_completas(void) {
    char nome_arq[100];
    Estats e;

    limpar_tela();
    printf("=== ESTATISTICAS COMPLETAS DO ARQUIVO ===\n\n");
    printf("Digite o nome do arquivo (ex.: dados.txt): ");
    if (fgets(nome_arq, sizeof(nome_arq), stdin) == NULL) {
        printf("Erro na leitura.\n");
        pausar();
        return;
    }
    nome_arq[strcspn(nome_arq, "\n")] = '\0';

    e = processar_arquivo(nome_arq);
    if (e.total_linhas > 0) {
        printf("\n----- ESTATISTICAS DE '%s' -----\n", nome_arq);
        printf("Total de linhas:        %d\n", e.total_linhas);
        printf("Total de palavras:      %d\n", e.total_palavras);
        printf("Total de vogais:        %d\n", e.total_vogais);
        printf("Tamanho da maior linha: %d\n", e.tam_linha_maior);
        printf("Conteudo da maior linha:\n  \"%s\"\n", e.linha_maior);
    }
    pausar();
}

/* ===================================================================
   FUNCAO PRINCIPAL (main)
   -------------------------------------------------------------------
   Monta o menu classico do professor: do-while + switch.
   O laco do-while garante que o menu aparece pelo menos 1 vez e
   repete ate o usuario escolher a opcao de saida.
   =================================================================== */
int main(void) {
    char opcao; /* guarda a tecla escolhida pelo usuario */

    /* Cria um arquivo de exemplo automaticamente para o aluno poder
       testar todas as opcoes sem precisar digitar nada antes. */
    criar_arquivo_exemplo("dados.txt");

    /* LACO PRINCIPAL DO MENU: repete ate o usuario digitar '0'. */
    do {
        limpar_tela();
        printf("=============================================\n");
        printf("   PROCESSAMENTO DE ARQUIVO TEXTO EM C\n");
        printf("   (Arquivo de exemplo 'dados.txt' criado)\n");
        printf("=============================================\n");
        printf(" 1 - Contar linhas do arquivo\n");
        printf(" 2 - Contar palavras do arquivo\n");
        printf(" 3 - Contar vogais do arquivo\n");
        printf(" 4 - Achar a linha mais longa\n");
        printf(" 5 - Buscar palavra no arquivo (strstr)\n");
        printf(" 6 - Copiar arquivo\n");
        printf(" 7 - Estatisticas completas\n");
        printf(" 0 - Sair\n");
        printf("=============================================\n");
        printf("Escolha uma opcao: ");

        /* getche le 1 tecla e ecoa na tela (mostra o que foi digitado).
           Nao precisa apertar Enter. Da conio.h (Windows). */
        opcao = getche();
        printf("\n\n");

        /* switch direciona para a funcao correspondente a opcao. */
        switch (opcao) {
            case '1':
                menu_contar_linhas();
                break;
            case '2':
                menu_contar_palavras();
                break;
            case '3':
                menu_contar_vogais();
                break;
            case '4':
                menu_linha_mais_longa();
                break;
            case '5':
                menu_buscar_palavra();
                break;
            case '6':
                menu_copiar_arquivo();
                break;
            case '7':
                menu_estatisticas_completas();
                break;
            case '0':
                printf("Encerrando o programa. Ate logo!\n");
                break;
            default:
                /* Opcao invalida: o laco volta a mostrar o menu. */
                printf("Opcao invalida! Tente novamente.\n");
                pausar();
                break;
        }
    } while (opcao != '0'); /* repete enquanto nao escolher sair */

    return 0; /* fim do programa com sucesso */
}

/*
=====================================================================
   RESUMO DIDATICO (para revisar antes da prova):
   -----------------------------------------------------------------
   - fgets(buf, tam, fp): le 1 linha (ate tam-1 chars ou ate '\n').
     Retorna NULL no fim do arquivo. Use no while: while(fgets(...)!=NULL)
   - NUNCA use gets() - perigoso, sem limite de tamanho.
   - NUNCA use while(!feof(fp)) com leitura antes do laco - causa bug
     de repetir a ultima linha. Use o retorno do fgets/fread/fgetc.
   - strcspn(str, "\n"): acha onde esta o '\n' para remove-lo.
   - strstr(texto, palavra): procura palavra, retorna ponteiro ou NULL.
   - fgetc(fp): le 1 caractere (retorna int para poder guardar EOF).
   - fputc(ch, fp): escreve 1 caractere no arquivo.
   - Sempre feche os arquivos com fclose() ao terminar.
   - typedef struct {...} Nome; cria um alias para nao repetir "struct".
=====================================================================
*/
