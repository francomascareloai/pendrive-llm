/*
 * ============================================================================
 * GABARITO: AGENDA DE CONTATOS
 * ============================================================================
 * O QUE FAZ: Sistema completo de agenda de contatos persistido em arquivo
 *            texto (contatos.txt). Operacoes: adicionar (fprintf), listar
 *            (fgets + strtok), buscar por nome (strstr - busca parcial),
 *            editar (arquivo temporario + rename), excluir (temp + rename),
 *            contar contatos. Validacao de email (deve conter '@').
 * TOPICO: struct, arquivos texto, strings, strtok, strstr, strcmp, strchr,
 *         modularizacao, ponteiros, arquivo temporario
 * PALAVRAS-CHAVE (Ctrl-F): struct Contato, typedef struct, fprintf, fgets,
 *         strtok, strstr busca parcial, strcmp, strchr, strcspn, fflush stdin,
 *         arquivo texto, temp+rename, editar arquivo, excluir contato,
 *         validar email, menu do-while switch, modularizacao, conio.h getche
 * ============================================================================
 */

#include <stdio.h>   /* fopen, fprintf, fgets, fputs, fclose, remove, rename */
#include <stdlib.h>  /* system */
#include <string.h>  /* strcpy, strtok, strstr, strchr, strcspn, strcmp */
#include <conio.h>   /* getche, getch - biblioteca do Windows (nao e padrao C) */

/* ---- Constantes (define) ---- */
#define ARQUIVO  "contatos.txt"  /* arquivo onde os contatos sao salvos */
#define TEMP     "temp.txt"      /* arquivo temporario para editar/excluir */
#define MAX_NOME  50             /* tamanho maximo do nome */
#define MAX_TEL   20             /* tamanho maximo do telefone */
#define MAX_EMAIL 50             /* tamanho maximo do email */

/* ---- Definicao da struct Contato com typedef (alias) ----
 * O typedef cria um apelido para o tipo, entao em vez de escrever
 * "struct Contato" usamos apenas "Contato". Isso deixa o codigo mais limpo.
 * Os campos sao vetores de char (strings) para armazenar os dados. */
typedef struct {
    char nome[MAX_NOME];      /* nome do contato (string) */
    char telefone[MAX_TEL];   /* telefone do contato (string) */
    char email[MAX_EMAIL];    /* email do contato (string) */
} Contato;  /* alias: agora "Contato" e o nome do tipo */

/* ---- Prototipos das funcoes ----
 * Declaradas antes do main para o compilador conhecer as assinaturas.
 * Isso permite que o main chame funcoes definidas depois no codigo. */
void limparBuffer(void);
void removerNovaLinha(char *str);
int  validarEmail(const char *email);
void lerString(char *dest, int tamanho, const char *mensagem);
void adicionarContato(void);
void listarContatos(void);
void buscarContato(void);
void editarContato(void);
void excluirContato(void);
int  contarContatos(void);
void menu(void);

/* ========================================================================== */

/* Limpa o buffer de entrada (stdin) apos usar scanf.
 * fflush(stdin) NAO e portavel segundo o padrao C (comportamento indefinido
 * em Linux/Mac), mas funciona no Windows com MinGW. O professor usa esse
 * estilo. Alternativa portavel: loop com getchar() ate encontrar '\n'. */
void limparBuffer(void) {
    fflush(stdin);  /* descarta caracteres restantes no buffer do teclado */
}

/* Remove o caractere '\n' que o fgets coloca no final da string.
 * strcspn(str, "\n") retorna o indice da primeira ocorrencia de '\n'.
 * Colocamos '\0' nesse indice para terminar a string ali.
 * Isso e necessario porque fgets inclui o '\n' na string lida. */
void removerNovaLinha(char *str) {
    str[strcspn(str, "\n")] = '\0';  /* troca '\n' por '\0' (fim de string) */
}

/* Valida se o email contem o caractere '@'.
 * strchr procura '@' na string e retorna um ponteiro para ele,
 * ou NULL se nao encontrar. Retorna 1 (valido) ou 0 (invalido). */
int validarEmail(const char *email) {
    if (strchr(email, '@') != NULL) {
        return 1;  /* tem '@' = email valido */
    }
    return 0;  /* nao tem '@' = email invalido */
}

/* Le uma string do teclado de forma segura usando fgets.
 * fgets e preferivel a gets (que foi removida do padrao C11) porque
 * limita a quantidade de caracteres lidos (evita buffer overflow).
 * Depois remove o '\n' final deixado pelo fgets. */
void lerString(char *dest, int tamanho, const char *mensagem) {
    printf("%s", mensagem);        /* exibe o prompt (ex: "Nome: ") */
    fgets(dest, tamanho, stdin);  /* le ate tamanho-1 caracteres ou ate \n */
    removerNovaLinha(dest);       /* remove o \n do final da string */
}

/* ========================================================================== */
/* ---- ADICIONAR CONTATO ----                                                  */
/* ========================================================================== */

/* Abre o arquivo em modo "a" (append = adicionar no final).
 * Se o arquivo nao existir, ele e criado. Se existir, escreve no final
 * sem apagar o conteudo anterior. Usa fprintf para gravar os dados. */
void adicionarContato(void) {
    Contato c;   /* variavel do tipo Contato (struct) para armazenar os dados */
    FILE *fp;    /* ponteiro para o arquivo */

    printf("\n--- ADICIONAR CONTATO ---\n");

    /* Le os dados do contato usando a funcao auxiliar lerString */
    lerString(c.nome, MAX_NOME, "Nome: ");
    lerString(c.telefone, MAX_TEL, "Telefone: ");

    /* Le o email com validacao: repete ate o usuario digitar um email com '@' */
    do {
        lerString(c.email, MAX_EMAIL, "Email: ");
        if (!validarEmail(c.email)) {
            printf("Email invalido! Deve conter '@'. Tente novamente.\n");
        }
    } while (!validarEmail(c.email));  /* repete enquanto o email for invalido */

    /* Abre o arquivo em modo append ("a").
     * "a" = adicionar no final (nao apaga o conteudo existente).
     * Se o arquivo nao existir, ele e criado. */
    fp = fopen(ARQUIVO, "a");
    if (fp == NULL) {
        /* Se fopen retornar NULL, houve erro (ex: disco cheio, sem permissao) */
        printf("Erro ao abrir o arquivo para escrita!\n");
        return;  /* sai da funcao sem gravar nada */
    }

    /* Escreve os dados no arquivo no formato: nome|telefone|email
     * Usamos '|' como separador porque nomes e emails normalmente nao
     * contem esse caractere. O '\n' no final separa cada contato em
     * uma linha diferente do arquivo. */
    fprintf(fp, "%s|%s|%s\n", c.nome, c.telefone, c.email);

    fclose(fp);  /* fecha o arquivo - IMPORTANTE para salvar os dados no disco */
    printf("Contato adicionado com sucesso!\n");
}

/* ========================================================================== */
/* ---- LISTAR CONTATOS ----                                                    */
/* ========================================================================== */

/* Le o arquivo linha por linha com fgets e separa os campos com strtok.
 * strtok divide uma string em partes (tokens) usando um delimitador ('|'). */
void listarContatos(void) {
    FILE *fp;
    char linha[200];   /* buffer para armazenar cada linha lida do arquivo */
    int contador = 0;  /* contador de contatos listados */

    printf("\n--- LISTA DE CONTATOS ---\n");

    fp = fopen(ARQUIVO, "r");  /* modo "r" = leitura */
    if (fp == NULL) {
        /* Se o arquivo nao existe, nao ha contatos cadastrados */
        printf("Nenhum contato cadastrado (arquivo nao encontrado).\n");
        return;
    }

    /* fgets le uma linha inteira do arquivo (ate '\n' ou ate encher o buffer).
     * Retorna NULL quando chega ao fim do arquivo (EOF).
     * Por isso usamos while(fgets(...) != NULL) e NAO while(!feof(fp)),
     * que causaria leitura duplicada do ultimo registro (bug classico). */
    while (fgets(linha, sizeof(linha), fp) != NULL) {
        char *nome, *telefone, *email;

        /* strtok divide a string usando '|' como delimitador.
         * A primeira chamada recebe a string original (linha).
         * strtok MODIFICA a string original (coloca '\0' no lugar do '|'). */
        nome = strtok(linha, "|");

        /* As chamadas seguintes recebem NULL para continuar da mesma string.
         * strtok mantem um ponteiro interno (estatico) com a posicao atual. */
        telefone = strtok(NULL, "|");
        email = strtok(NULL, "|");

        /* strtok retorna NULL se nao encontrar o delimitador.
         * Verificamos se os tres campos existem antes de usar. */
        if (nome != NULL && telefone != NULL && email != NULL) {
            removerNovaLinha(email);  /* tira o '\n' do final do email */
            contador++;  /* incrementa o numero de contatos listados */
            printf("\n[%d] Nome: %s\n", contador, nome);
            printf("    Telefone: %s\n", telefone);
            printf("    Email: %s\n", email);
        }
    }

    fclose(fp);  /* fecha o arquivo apos terminar a leitura */

    if (contador == 0) {
        printf("\nNenhum contato cadastrado.\n");
    } else {
        printf("\nTotal listado: %d contato(s).\n", contador);
    }
}

/* ========================================================================== */
/* ---- BUSCAR CONTATO POR NOME ----                                            */
/* ========================================================================== */

/* Usa strstr para fazer busca PARCIAL (nao precisa digitar o nome completo).
 * strstr procura uma substring dentro de outra string e retorna um ponteiro
 * para a primeira ocorrencia, ou NULL se nao encontrar.
 * Ex: buscar "ana" encontra "Joana" (se o caso das letras bater).
 * NOTA: strstr e CASE-SENSITIVE (diferencia maiusculas de minusculas).
 * Para ignorar caso, seria necessario converter tudo para minusculas. */
void buscarContato(void) {
    FILE *fp;
    char linha[200];
    char busca[50];    /* termo de busca digitado pelo usuario */
    int encontrados = 0;

    printf("\n--- BUSCAR CONTATO ---\n");
    lerString(busca, sizeof(busca), "Digite o nome (ou parte do nome): ");

    fp = fopen(ARQUIVO, "r");
    if (fp == NULL) {
        printf("Nenhum contato cadastrado (arquivo nao encontrado).\n");
        return;
    }

    printf("\nResultados da busca por '%s':\n", busca);

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        char *nome, *telefone, *email;

        nome = strtok(linha, "|");
        telefone = strtok(NULL, "|");
        email = strtok(NULL, "|");

        if (nome != NULL && telefone != NULL && email != NULL) {
            /* strstr(nome, busca) procura a string 'busca' dentro de 'nome'.
             * Se retornar != NULL, o termo foi encontrado (busca parcial). */
            if (strstr(nome, busca) != NULL) {
                removerNovaLinha(email);
                encontrados++;
                printf("\n[%d] Nome: %s\n", encontrados, nome);
                printf("    Telefone: %s\n", telefone);
                printf("    Email: %s\n", email);
            }
        }
    }

    fclose(fp);

    if (encontrados == 0) {
        printf("\nNenhum contato encontrado com '%s'.\n", busca);
    } else {
        printf("\n%d contato(s) encontrado(s).\n", encontrados);
    }
}

/* ========================================================================== */
/* ---- EDITAR CONTATO ----                                                     */
/* ========================================================================== */

/* Estrategia com arquivo temporario (temp + rename):
 * 1. Abre contatos.txt para leitura e temp.txt para escrita.
 * 2. Le cada linha do original. Se o nome bater com a busca, pede os novos
 *    dados e escreve os dados atualizados no temporario.
 * 3. Se nao bater, copia a linha original para o temporario.
 * 4. No final, remove o arquivo original e renomeia o temporario.
 *
 * Isso e necessario porque nao podemos "editar no meio" de um arquivo texto
 * de tamanho variavel (os campos tem tamanhos diferentes a cada edicao). */
void editarContato(void) {
    FILE *fp, *ft;     /* fp = arquivo original, ft = arquivo temporario */
    char linha[200];
    char busca[50];
    int encontrou = 0;  /* 0 = nao encontrou, 1 = encontrou e editou */

    printf("\n--- EDITAR CONTATO ---\n");
    lerString(busca, sizeof(busca), "Digite o nome EXATO do contato a editar: ");

    fp = fopen(ARQUIVO, "r");  /* abre original para leitura */
    if (fp == NULL) {
        printf("Nenhum contato cadastrado (arquivo nao encontrado).\n");
        return;
    }

    ft = fopen(TEMP, "w");  /* abre temporario para escrita (cria/sobrescreve) */
    if (ft == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(fp);  /* fecha o original antes de sair */
        return;
    }

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        char copia[200];  /* copia da linha (strtok modifica a string original) */
        char *nome, *telefone, *email;

        /* strtok MODIFICA a string (coloca '\0' no lugar do delimitador).
         * Como precisamos da linha original intacta para fputs, fazemos
         * uma copia com strcpy e usamos strtok na copia. */
        strcpy(copia, linha);

        nome = strtok(copia, "|");
        telefone = strtok(NULL, "|");
        email = strtok(NULL, "|");

        if (nome != NULL && telefone != NULL && email != NULL) {
            removerNovaLinha(email);  /* tira o '\n' do ultimo campo */

            /* strcmp compara duas strings. Retorna 0 se forem iguais.
             * Usamos comparacao EXATA (nome completo) para editar.
             * O "!encontrou" garante que so editamos o PRIMEIRO match. */
            if (strcmp(nome, busca) == 0 && !encontrou) {
                Contato novo;  /* struct para armazenar os novos dados */
                encontrou = 1;

                printf("\nContato encontrado!\n");
                printf("Nome atual: %s\n", nome);
                printf("Telefone atual: %s\n", telefone);
                printf("Email atual: %s\n", email);

                printf("\n--- DIGITE OS NOVOS DADOS ---\n");
                lerString(novo.nome, MAX_NOME, "Novo nome: ");
                lerString(novo.telefone, MAX_TEL, "Novo telefone: ");

                do {
                    lerString(novo.email, MAX_EMAIL, "Novo email: ");
                    if (!validarEmail(novo.email)) {
                        printf("Email invalido! Deve conter '@'.\n");
                    }
                } while (!validarEmail(novo.email));

                /* Escreve os dados ATUALIZADOS no arquivo temporario */
                fprintf(ft, "%s|%s|%s\n", novo.nome, novo.telefone, novo.email);
            } else {
                /* Nao e o contato buscado: copia a linha original para o temp.
                 * Usamos fputs com a linha original (nao modificada pelo strtok
                 * porque o strtok foi aplicado na copia, nao na linha). */
                fputs(linha, ft);
            }
        } else {
            /* Linha mal formatada: copia como esta para nao perder dados */
            fputs(linha, ft);
        }
    }

    fclose(fp);  /* fecha o arquivo original */
    fclose(ft);  /* fecha o arquivo temporario (salva os dados no disco) */

    if (encontrou) {
        /* Substitui o original pelo temporario:
         * 1. remove() apaga o arquivo original (contatos.txt)
         * 2. rename() renomeia temp.txt para contatos.txt */
        remove(ARQUIVO);
        rename(TEMP, ARQUIVO);
        printf("\nContato editado com sucesso!\n");
    } else {
        /* Nao encontrou o contato: remove o temporario (nao precisa dele) */
        remove(TEMP);
        printf("\nContato '%s' nao encontrado.\n", busca);
    }
}

/* ========================================================================== */
/* ---- EXCLUIR CONTATO ----                                                    */
/* ========================================================================== */

/* Mesma estrategia do editar (temp + rename), mas em vez de escrever
 * os dados atualizados, simplesmente NAO escreve a linha do contato
 * no arquivo temporario. Assim ele "some" do novo arquivo.
 * Tem confirmacao com getche() (le um caractere sem precisar Enter). */
void excluirContato(void) {
    FILE *fp, *ft;
    char linha[200];
    char busca[50];
    int encontrou = 0;
    char confirmar;  /* resposta do usuario (s/n) */

    printf("\n--- EXCLUIR CONTATO ---\n");
    lerString(busca, sizeof(busca), "Digite o nome EXATO do contato a excluir: ");

    fp = fopen(ARQUIVO, "r");
    if (fp == NULL) {
        printf("Nenhum contato cadastrado (arquivo nao encontrado).\n");
        return;
    }

    ft = fopen(TEMP, "w");
    if (ft == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(fp);
        return;
    }

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        char copia[200];
        char *nome, *telefone, *email;

        strcpy(copia, linha);  /* copia para nao perder a original (strtok modifica) */

        nome = strtok(copia, "|");
        telefone = strtok(NULL, "|");
        email = strtok(NULL, "|");

        if (nome != NULL && telefone != NULL && email != NULL) {
            removerNovaLinha(email);  /* tira o '\n' do ultimo campo */

            if (strcmp(nome, busca) == 0 && !encontrou) {
                /* Encontrou o contato! Mostra os dados e pede confirmacao. */
                encontrou = 1;
                printf("\nContato encontrado:\n");
                printf("  Nome: %s\n", nome);
                printf("  Telefone: %s\n", telefone);
                printf("  Email: %s\n", email);

                /* getche() le UM caractere do teclado COM eco (mostra na tela)
                 * e NAO precisa pressionar Enter. Biblioteca conio.h (Windows).
                 * Retorna int (codigo ASCII), por isso o cast para char. */
                printf("\nConfirmar exclusao? (s/n): ");
                confirmar = (char)getche();  /* le a resposta do usuario */
                printf("\n");

                if (confirmar == 's' || confirmar == 'S') {
                    /* Usuario confirmou: NAO escreve no temporario.
                     * Como a linha nao e copiada, o contato sera excluido. */
                    printf("Contato excluido.\n");
                } else {
                    /* Usuario cancelou: copia a linha original para manter */
                    fputs(linha, ft);
                    printf("Exclusao cancelada. Contato mantido.\n");
                }
            } else {
                /* Nao e o contato buscado: copia para o temporario */
                fputs(linha, ft);
            }
        } else {
            fputs(linha, ft);
        }
    }

    fclose(fp);
    fclose(ft);

    if (encontrou) {
        /* Substitui o original pelo temporario.
         * O temporario tem todos os contatos EXCETO o excluido
         * (ou todos, se o usuario cancelou a exclusao). */
        remove(ARQUIVO);
        rename(TEMP, ARQUIVO);
        printf("Arquivo atualizado!\n");
    } else {
        /* Nao encontrou: descarta o temporario (nao precisa) */
        remove(TEMP);
        printf("\nContato '%s' nao encontrado.\n", busca);
    }
}

/* ========================================================================== */
/* ---- CONTAR CONTATOS ----                                                    */
/* ========================================================================== */

/* Conta quantas linhas o arquivo tem (cada linha = 1 contato).
 * Retorna o numero de contatos como inteiro. */
int contarContatos(void) {
    FILE *fp;
    char linha[200];
    int count = 0;

    fp = fopen(ARQUIVO, "r");
    if (fp == NULL) {
        return 0;  /* arquivo nao existe = 0 contatos */
    }

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        count++;  /* cada linha lida = 1 contato */
    }

    fclose(fp);
    return count;  /* retorna o total de contatos */
}

/* ========================================================================== */
/* ---- MENU PRINCIPAL ----                                                     */
/* ========================================================================== */

/* Usa a estrutura do-while + switch (estilo do professor).
 * do-while executa o bloco pelo menos uma vez e repete ate o usuario
 * escolher a opcao 0 (sair). */
void menu(void) {
    int opcao;

    do {
        system("cls");  /* limpa a tela (comando do Windows) */

        printf("========================================\n");
        printf("       AGENDA DE CONTATOS (GB)\n");
        printf("========================================\n");
        printf("  1. Adicionar contato\n");
        printf("  2. Listar contatos\n");
        printf("  3. Buscar contato por nome\n");
        printf("  4. Editar contato\n");
        printf("  5. Excluir contato\n");
        printf("  6. Contar contatos\n");
        printf("  0. Sair\n");
        printf("========================================\n");
        printf("Opcao: ");

        /* scanf le um inteiro do teclado.
         * Verificamos o retorno: scanf retorna o numero de itens lidos
         * com sucesso. Se for != 1, a leitura falhou (usuario digitou
         * uma letra em vez de numero). */
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;  /* valor invalido para cair no default do switch */
        }
        limparBuffer();  /* descarta o '\n' deixado pelo scanf no buffer */

        switch (opcao) {
            case 1:
                adicionarContato();
                break;
            case 2:
                listarContatos();
                break;
            case 3:
                buscarContato();
                break;
            case 4:
                editarContato();
                break;
            case 5:
                excluirContato();
                break;
            case 6:
                printf("\nTotal de contatos cadastrados: %d\n", contarContatos());
                break;
            case 0:
                printf("\nSaindo do programa... Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida! Digite um numero de 0 a 6.\n");
        }

        /* Pausa para o usuario ver o resultado antes de limpar a tela.
         * system("pause") mostra "Pressione qualquer tecla..." no Windows. */
        if (opcao != 0) {
            printf("\n");
            system("pause");
        }

    } while (opcao != 0);  /* repete enquanto nao escolher 0 (sair) */
}

/* ========================================================================== */
/* ---- FUNCAO PRINCIPAL ----                                                   */
/* ========================================================================== */

int main(void) {
    menu();      /* chama o menu que controla todo o programa */
    return 0;    /* 0 = programa terminou com sucesso (convencao do SO) */
}
