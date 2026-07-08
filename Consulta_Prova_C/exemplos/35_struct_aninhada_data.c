/* ============================================================================
 * ARQUIVO : 35_struct_aninhada_data.c
 * OBJETIVO: Cadastrar pessoas com data de nascimento (struct aninhada),
 *           validar a data, calcular idade, listar e buscar por nome.
 * TOPICO  : Struct aninhada (struct dentro de struct), strings, funcoes,
 *           ponteiros, validacao de dados, strcmp.
 * PALAVRAS-CHAVE Ctrl-F: struct aninhada, Data, Pessoa, typedef struct,
 *   struct dentro de struct, validar data, dia mes ano, calcular idade,
 *   fgets strcspn, strcmp buscar nome, menu switch do-while, conio.h,
 *   fflush stdin, modularizacao, ponteiro struct, passo por referencia.
 * COMPILAR: gcc -Wall -Wextra 35_struct_aninhada_data.c -o 35.exe
 * ==========================================================================*/

/* ---- BIBLIOTECAS --------------------------------------------------------- */
#include <stdio.h>   /* printf, scanf, fgets, fopen, fclose, etc.            */
#include <stdlib.h>  /* system, malloc, exit                                  */
#include <string.h>  /* strcmp, strcspn, strcpy, strlen                       */
#include <time.h>    /* time_t, time(), localtime() -> pegar ano atual        */
#include <ctype.h>   /* tolower (nao usado aqui, mas util para buscas)         */
#include <conio.h>   /* getche, getch -> estilo do professor (Windows/MinGW)   */

/* ---- DEFINICOES DE CONSTANTES -------------------------------------------- */
#define MAX_PESSOAS 100  /* limite do vetor de pessoas (evita estourar memoria) */
#define TAM_NOME    50   /* tamanho do campo nome (49 chars + '\0')             */

/* ============================================================================
 * STRUCT ANINHADA
 * ---------------------------------------------------------------------------
 * AQUI ESTA O CORACAO DO EXEMPLO: uma struct DENTRO de outra struct.
 *
 *   Data  -> struct "filha"   (guarda dia, mes, ano)
 *   Pessoa-> struct "mae"     (tem um campo do tipo Data dentro dela)
 *
 * Por que fazer isso? Porque a data de nascimento tem 3 informacoes que
 * andam juntas (dia, mes, ano). Em vez de criar 3 campos soltos na Pessoa
 * (nascimento_dia, nascimento_mes, nascimento_ano), agrupamos em uma struct
 * Data. Assim o codigo fica organizado e reutilizavel: se amanha precisarmos
 * de uma data de cadastro, basta criar outro campo do tipo Data.
 *
 * ACESSO aos campos da struct aninhada:
 *   pessoa.nascimento.dia   -> ponto para cada nivel de aninhamento.
 *   Se fosse ponteiro: pessoa->nascimento.dia  (ou (*pessoa).nascimento.dia)
 * ==========================================================================*/

/* Struct "filha": Data.
 * Usamos typedef para criar um ALIAS (apelido) "Data" para "struct Data".
 * Assim escrevemos so "Data" em vez de "struct Data" toda hora.
 * IMPORTANTE: a struct Data precisa ser declarada ANTES da struct Pessoa,
 * porque Pessoa tem um campo do tipo Data. Ordem importa! */
typedef struct Data {
    int dia;  /* dia do mes: 1 a 31                          */
    int mes;  /* mes do ano: 1 a 12                         */
    int ano;  /* ano com 4 digitos: ex 2005                 */
} Data;       /* alias: agora "Data" == "struct Data"       */

/* Struct "mae": Pessoa.
 * Repare no campo "Data nascimento;" -> isso e a STRUCT ANINHADA.
 * Um campo cujo tipo e outra struct. */
typedef struct Pessoa {
    char nome[TAM_NOME];   /* nome da pessoa (string, vetor de char)   */
    Data nascimento;       /* <<< STRUCT ANINHADA AQUI >>>             */
    int  idade;            /* idade calculada a partir da data         */
} Pessoa;                  /* alias: "Pessoa" == "struct Pessoa"        */

/* ============================================================================
 * PROTOTIPOS DAS FUNCOES
 * ---------------------------------------------------------------------------
 * Declarar os cabecalhos antes do main() para o compilador conhecer as
 * funcoes mesmo que elas sejam implementadas depois. Isso e MODULARIZACAO.
 * ==========================================================================*/
int  obterAnoAtual(void);                              /* pega ano do sistema */
int  validarData(int dia, int mes, int ano);           /* retorna 1 se ok     */
int  calcularIdade(Data nasc, int anoAtual);           /* calcula idade       */
void limparBuffer(void);                              /* limpa stdin         */
void lerString(char *destino, int tamanho);           /* le string segura    */
void cadastrarPessoa(Pessoa *p, int anoAtual);        /* cadastra 1 pessoa   */
void listarPessoas(Pessoa vet[], int qtd);             /* lista todas         */
void buscarPorNome(Pessoa vet[], int qtd);             /* busca por nome      */
void exibirPessoa(Pessoa p);                           /* mostra 1 pessoa    */
int  diasNoMes(int mes, int ano);                     /* dias de cada mes    */
int  ehBissexto(int ano);                              /* ano bissexto?       */

/* ============================================================================
 * FUNCAO: obterAnoAtual
 * Usa a biblioteca <time.h> para descobrir o ano atual do relogio do PC.
 * Assim nao precisamos pedir o ano para o usuario (evita erro de digitacao).
 * ==========================================================================*/
int obterAnoAtual(void) {
    time_t t;              /* variavel do tipo tempo (segundos desde 1970)     */
    struct tm *infoTempo;  /* ponteiro para struct que guarda data quebrada    */

    time(&t);             /* pega o tempo atual em segundos -> guarda em t    */
    infoTempo = localtime(&t); /* converte segundos -> dia/mes/ano legivel    */

    /* localtime retorna ano desde 1900, entao somamos 1900. */
    return infoTempo->tm_year + 1900;
}

/* ============================================================================
 * FUNCAO: ehBissexto
 * Verifica se um ano e bissexto. Regra:
 *   - Divisivel por 4 E nao divisivel por 100  -> bissexto
 *   - OU divisivel por 400                     -> bissexto
 * Usado para saber se fevereiro tem 28 ou 29 dias.
 * ==========================================================================*/
int ehBissexto(int ano) {
    /* Retorna 1 (verdadeiro) se bissexto, 0 (falso) caso contrario. */
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        return 1;  /* e bissexto */
    }
    return 0;  /* nao e bissexto */
}

/* ============================================================================
 * FUNCAO: diasNoMes
 * Retorna quantos dias tem um determinado mes (considerando ano bissexto
 * para fevereiro). Retorna 0 se o mes for invalido.
 * ==========================================================================*/
int diasNoMes(int mes, int ano) {
    /* Vetor com os dias de cada mes (indice 0 nao usado para facilitar).
     * diasPorMes[1] = janeiro (31), diasPorMes[2] = fevereiro (28), etc. */
    int diasPorMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mes < 1 || mes > 12) {
        return 0;  /* mes invalido -> 0 dias (sinaliza erro) */
    }

    /* Caso especial: fevereiro em ano bissexto tem 29 dias. */
    if (mes == 2 && ehBissexto(ano)) {
        return 29;
    }

    return diasPorMes[mes];  /* retorna o dia padrao do mes */
}

/* ============================================================================
 * FUNCAO: validarData
 * Verifica se uma data (dia, mes, ano) e valida.
 * Retorna 1 se valida, 0 se invalida.
 *
 * Checagens:
 *   1) mes entre 1 e 12
 *   2) dia entre 1 e o maximo de dias daquele mes (considerando bissexto)
 *   3) ano positivo (nao aceita ano 0 ou negativo)
 * ==========================================================================*/
int validarData(int dia, int mes, int ano) {
    /* 1) Ano deve ser positivo. Aceitamos qualquer ano > 0. */
    if (ano <= 0) {
        return 0;  /* ano invalido */
    }

    /* 2) Mes deve estar entre 1 e 12. */
    if (mes < 1 || mes > 12) {
        return 0;  /* mes invalido */
    }

    /* 3) Dia deve estar entre 1 e o numero maximo de dias do mes. */
    /*    Chamamos diasNoMes para saber o limite (ex: abril tem 30). */
    int maxDias = diasNoMes(mes, ano);
    if (dia < 1 || dia > maxDias) {
        return 0;  /* dia invalido (ex: 31 de abril) */
    }

    return 1;  /* passou por todas as checagens -> data valida */
}

/* ============================================================================
 * FUNCAO: calcularIdade
 * Calcula a idade de uma pessoa baseada na data de nascimento e no ano atual.
 *
 * Logica:
 *   - Comeca com (anoAtual - anoNascimento).
 *   - Se o mes de nascimento ainda nao chegou, subtrai 1 (aniversario nao passou).
 *   - Se estamos no mes do aniversario, mas o dia ainda nao chegou, subtrai 1.
 *
 * Recebe a struct Data por VALOR (copia) porque so vamos ler, nao alterar.
 * ==========================================================================*/
int calcularIdade(Data nasc, int anoAtual) {
    time_t t;
    struct tm *infoTempo;
    int mesAtual, diaAtual;

    /* Pegamos mes e dia atuais do sistema para saber se o aniversario ja passou. */
    time(&t);
    infoTempo = localtime(&t);
    mesAtual = infoTempo->tm_mon + 1;  /* tm_mon vai de 0 a 11, somamos 1 */
    diaAtual = infoTempo->tm_mday;     /* tm_mday ja e o dia certo (1-31) */

    int idade = anoAtual - nasc.ano;  /* idade "base" so considerando o ano */

    /* Se o mes de nascimento e maior que o mes atual, aniversario nao chegou. */
    if (nasc.mes > mesAtual) {
        idade--;  /* subtrai 1 ano */
    }
    /* Se estamos no MES do aniversario, mas o DIA ainda nao chegou. */
    else if (nasc.mes == mesAtual && nasc.dia > diaAtual) {
        idade--;  /* subtrai 1 ano */
    }

    return idade;
}

/* ============================================================================
 * FUNCAO: limparBuffer
 * Limpa o buffer do teclado (stdin). Usamos depois de scanf para tirar o '\n'
 * que o scanf deixa no buffer (senao o proximo fgets "pega" esse '\n' e
 * parece que pulou a leitura).
 *
 * fflush(stdin) NAO e portavel (o padrao C diz que fflush e para SAIDA, nao
 * entrada), mas no Windows com MinGW funciona. O professor usa com comentario.
 * Aqui usamos um loop com getchar para ser mais portavel e didatico.
 * ==========================================================================*/
void limparBuffer(void) {
    int c;
    /* Le e descarta caracteres ate encontrar '\n' ou EOF (fim de arquivo). */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta o caractere */
    }
}

/* ============================================================================
 * FUNCAO: lerString
 * Le uma string de forma SEGURA usando fgets (nunca use gets!).
 *
 * Por que fgets e melhor que gets/scanf("%s")?
 *   - gets nao controla o tamanho -> estouro de buffer (vulnerabilidade).
 *   - scanf("%s") para no primeiro espaco -> nao le nomes compostos.
 *   - fgets le ate o tamanho maximo OU ate '\n', o que for vir primeiro.
 *
 * fgets inclui o '\n' no final da string lida. Removemos com strcspn.
 * strcspn(str, "\n") retorna o indice do primeiro '\n' encontrado.
 * Colocamos '\0' nessa posicao para "cortar" o '\n'.
 * ==========================================================================*/
void lerString(char *destino, int tamanho) {
    /* fgets le do teclado (stdin), guarda em destino, no maximo 'tamanho' chars. */
    if (fgets(destino, tamanho, stdin) != NULL) {
        /* strcspn acha a posicao do '\n'. Substituimos por '\0' (fim de string). */
        destino[strcspn(destino, "\n")] = '\0';
    }
}

/* ============================================================================
 * FUNCAO: exibirPessoa
 * Mostra os dados de UMA pessoa na tela.
 * Recebe por VALOR (copia) porque so vamos imprimir, nao modificar.
 *
 * Repare no acesso ao campo aninhado: p.nascimento.dia
 *   p          -> a struct Pessoa
 *   .nascimento-> entra na struct Data que esta dentro de Pessoa
 *   .dia       -> pega o dia dentro da struct Data
 * ==========================================================================*/
void exibirPessoa(Pessoa p) {
    /* %02d -> imprime com 2 digitos, preenchendo com zero a esquerda (ex: 05). */
    printf("Nome       : %s\n", p.nome);
    printf("Nascimento : %02d/%02d/%d\n", p.nascimento.dia, p.nascimento.mes, p.nascimento.ano);
    printf("Idade      : %d anos\n", p.idade);
    printf("--------------------------------------------------\n");
}

/* ============================================================================
 * FUNCAO: cadastrarPessoa
 * Cadastra UMA pessoa. Recebe um PONTEIRO para Pessoa (passagem por REFERENCIA)
 * porque vamos MODIFICAR a struct original (preencher os campos).
 *
 * Se passassemos por valor, as alteracoes seriam perdidas (copia temporaria).
 * Por isso usamos "->" (seta) para acessar campos via ponteiro.
 * ==========================================================================*/
void cadastrarPessoa(Pessoa *p, int anoAtual) {
    int dia, mes, ano;  /* variaveis temporarias para validar antes de guardar */

    printf("\n--- CADASTRO DE PESSOA ---\n");

    /* ---- NOME ---- */
    printf("Digite o nome: ");
    /* fflush(stdin): limpa o buffer antes de ler. NAO e portavel (padrao C
     * diz que fflush e para saida), mas no Windows/MinGW funciona. O professor
     * usa com comentario explicativo. Aqui tambem chamamos limparBuffer para
     * garantir. */
    fflush(stdin);  /* NAO PORTAVEL, mas funciona no Windows (comentario exigido) */
    lerString(p->nome, TAM_NOME);  /* le nome de forma segura (fgets+strcspn) */

    /* ---- DATA DE NASCIMENTO (com validacao) ---- */
    /* Fazemos um loop ate o usuario digitar uma data VALIDA.
     * Usamos do-while porque precisamos pedir a data pelo menos uma vez. */
    do {
        printf("Digite a data de nascimento (dd mm aaaa): ");
        /* scanf retorna o numero de itens lidos com sucesso. Se ler 3 ints,
         * retorna 3. Se der erro, retorna menos. */
        if (scanf("%d %d %d", &dia, &mes, &ano) != 3) {
            printf("ERRO: digite 3 numeros separados por espaco.\n");
            limparBuffer();  /* limpa o que ficou no buffer */
            continue;        /* volta pro inicio do loop sem validar */
        }

        /* Chamamos validarData. Se retornar 0, a data e invalida -> repete. */
        if (!validarData(dia, mes, ano)) {
            printf("ERRO: data invalida! Verifique dia (1-%d), mes (1-12) e ano.\n",
                   diasNoMes(mes, ano));
        }
    } while (!validarData(dia, mes, ano));  /* repete enquanto data for invalida */

    /* Depois de validar, GUARDAMOS na struct aninhada.
     * Como p e ponteiro, usamos "->" para acessar os campos. */
    p->nascimento.dia = dia;   /* acessa campo dia da struct Data dentro de Pessoa */
    p->nascimento.mes = mes;   /* acessa campo mes da struct Data dentro de Pessoa */
    p->nascimento.ano = ano;   /* acessa campo ano da struct Data dentro de Pessoa */

    /* Calcula a idade chamando a funcao. Passamos a struct Data por valor. */
    p->idade = calcularIdade(p->nascimento, anoAtual);

    printf("Cadastro realizado com sucesso!\n");
    printf("Idade calculada: %d anos\n", p->idade);
}

/* ============================================================================
 * FUNCAO: listarPessoas
 * Mostra TODAS as pessoas cadastradas. Recebe o vetor e a quantidade.
 * Como so vamos LER (nao modificar), nao precisamos de ponteiro.
 * ==========================================================================*/
void listarPessoas(Pessoa vet[], int qtd) {
    int i;  /* contador do loop */

    printf("\n--- LISTA DE PESSOAS (%d cadastradas) ---\n", qtd);

    /* Se nao tem ninguem cadastrado, avisa e sai da funcao. */
    if (qtd == 0) {
        printf("Nenhuma pessoa cadastrada ainda.\n");
        return;  /* return sem valor em funcao void -> apenas sai */
    }

    /* Percorre o vetor de 0 ate qtd-1, exibindo cada pessoa. */
    for (i = 0; i < qtd; i++) {
        printf("[%d] ", i + 1);     /* mostra a posicao (1-based para o usuario) */
        exibirPessoa(vet[i]);       /* chama funcao que imprime 1 pessoa */
    }
}

/* ============================================================================
 * FUNCAO: buscarPorNome
 * Procura uma pessoa pelo nome usando strcmp.
 *
 * strcmp(str1, str2):
 *   - Retorna 0 se as strings forem IGUAIS.
 *   - Retorna <0 se str1 < str2 (ordem alfabetica).
 *   - Retorna >0 se str1 > str2.
 *
 * Aqui comparamos o nome digitado com cada nome do vetor.
 * Se strcmp == 0, achamos! Mostramos a pessoa.
 *
 * DICA DE PROVA: strcmp diferencia MAIUSCULAS de minusculas. Para busca
 * "case-insensitive" precisariamos converter tudo para minusculo (tolower).
 * Aqui mantemos simples para focar no conceito de struct aninhada.
 * ==========================================================================*/
void buscarPorNome(Pessoa vet[], int qtd) {
    char busca[TAM_NOME];  /* guarda o nome que o usuario quer procurar */
    int i;
    int achou = 0;         /* flag: 0 = nao achou, 1 = achou pelo menos 1 */

    printf("\n--- BUSCA POR NOME ---\n");
    printf("Digite o nome para buscar: ");
    fflush(stdin);  /* NAO PORTAVEL, mas funciona no Windows (comentario) */
    lerString(busca, TAM_NOME);  /* le o nome da busca de forma segura */

    /* Percorre todo o vetor comparando nomes. */
    for (i = 0; i < qtd; i++) {
        /* strcmp retorna 0 quando as strings sao iguais. */
        if (strcmp(vet[i].nome, busca) == 0) {
            printf("\n>>> Encontrado na posicao %d:\n", i + 1);
            exibirPessoa(vet[i]);  /* mostra a pessoa encontrada */
            achou = 1;             /* marca que achou pelo menos uma */
        }
    }

    /* Se percorreu tudo e achou continua 0, ninguem foi encontrado. */
    if (!achou) {
        printf("Nenhuma pessoa encontrada com o nome \"%s\".\n", busca);
    }
}

/* ============================================================================
 * FUNCAO PRINCIPAL: main
 * Controla o fluxo do programa com um MENU do-while + switch.
 * Esse e o estilo classico do professor.
 * ==========================================================================*/
int main(void) {
    /* Vetor de structs Pessoa. Alocado estaticamente com MAX_PESSOAS posicoes.
     * Cada posicao e uma struct Pessoa completa (com nome, Data e idade). */
    Pessoa pessoas[MAX_PESSOAS];

    int qtd = 0;            /* quantas pessoas ja foram cadastradas (contador) */
    int opcao;              /* opcao escolhida no menu                          */
    int anoAtual;           /* ano atual do sistema (para calcular idade)        */

    /* Pega o ano atual UMA vez no inicio (nao precisa pegar a cada cadastro). */
    anoAtual = obterAnoAtual();

    /* ---- MENU PRINCIPAL (do-while + switch) ----
     * O do-while garante que o menu aparece PELO MENOS uma vez.
     * O loop continua enquanto a opcao for diferente de 0 (sair). */
    do {
        /* system("cls") limpa a tela. NAO e portavel (so Windows).
         * Em Linux seria system("clear"). Estilo do professor. */
        system("cls");

        printf("============================================\n");
        printf("   CADASTRO DE PESSOAS - STRUCT ANINHADA     \n");
        printf("============================================\n");
        printf("  1 - Cadastrar pessoa\n");
        printf("  2 - Listar pessoas\n");
        printf("  3 - Buscar por nome\n");
        printf("  0 - Sair\n");
        printf("============================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        /* switch executa o bloco correspondente a opcao escolhida. */
        switch (opcao) {
            case 1:
                /* Verifica se ainda cabe gente no vetor (evita estouro). */
                if (qtd < MAX_PESSOAS) {
                    /* Passamos &pessoas[qtd] (endereco da proxima posicao livre).
                     * Como cadastrarPessoa recebe PONTEIRO, usamos & para passar
                     * o endereco. A funcao vai preencher essa posicao direto. */
                    cadastrarPessoa(&pessoas[qtd], anoAtual);
                    qtd++;  /* incrementa o contador de cadastrados */
                } else {
                    printf("\nLimite de %d pessoas atingido!\n", MAX_PESSOAS);
                }
                break;

            case 2:
                /* Lista todas as pessoas. Passamos o vetor e a quantidade. */
                listarPessoas(pessoas, qtd);
                break;

            case 3:
                /* Busca por nome. Se nao tem ninguem, avisa antes. */
                if (qtd == 0) {
                    printf("\nNenhuma pessoa cadastrada para buscar.\n");
                } else {
                    buscarPorNome(pessoas, qtd);
                }
                break;

            case 0:
                printf("\nSaindo do programa... Ate logo!\n");
                break;

            default:
                /* Se digitou opcao inexistente, cai aqui. */
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

        /* system("pause") mostra "Pressione qualquer tecla para continuar..."
         * e espera. Tipico do Windows/estilo do professor. */
        if (opcao != 0) {
            printf("\n");
            system("pause");
        }

    } while (opcao != 0);  /* repete enquanto nao escolher sair (0) */

    return 0;  /* fim do programa com sucesso */
}

/* ============================================================================
 * RESUMO DIDATICO - STRUCT ANINHADA
 * ---------------------------------------------------------------------------
 * 1) Declare a struct "filha" (Data) ANTES da struct "mae" (Pessoa).
 * 2) Dentro de Pessoa, crie um campo do tipo Data:  "Data nascimento;"
 * 3) Para ACESSAR campos aninhados, use ponto para cada nivel:
 *        pessoa.nascimento.dia
 *    Se for PONTEIRO, use seta no primeiro nivel e ponto no resto:
 *        p->nascimento.dia   (ou  (*p).nascimento.dia)
 * 4) Validacao de data: confira mes (1-12), dia (1 a diasNoMes), ano (>0).
 * 5) Calcular idade: anoAtual - anoNascimento, ajustando se aniversario
 *    ainda nao chegou (compare mes e dia com a data atual do sistema).
 * 6) Busca por nome: use strcmp == 0 para comparar strings (nunca use ==).
 * 7) Leitura de string segura: fgets + strcspn (nunca gets!).
 * ==========================================================================*/
