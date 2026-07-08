/* =========================================================================
 * GABARITO - EXERCICIO 2 DE STRUCT
 * -------------------------------------------------------------------------
 * O QUE FAZ:
 *   Le N alunos (N <= 20). Cada aluno tem codigo, nome, notas de laboratorio,
 *   prova semestral e prova final. Calcula a MEDIA PONDERADA (lab peso 2,
 *   semestral peso 3, final peso 5 -> divide por 10) e uma CLASSIFICACAO
 *   por letra (A/B/C/D/R). Por fim, imprime uma listagem de todos os alunos.
 *
 * TOPICO DA PROVA:
 *   struct (typedef struct), vetor de struct, funcoes/modularizacao,
 *   strings (fgets + strcspn), laco de repeticao, switch/if-else encadeado.
 *
 * PALAVRAS-CHAVE (Ctrl-F para achar rapido):
 *   struct , typedef struct , vetor de struct , media ponderada ,
 *   classificacao , fgets , strcspn , fflush(stdin) , modularizacao ,
 *   funcao que recebe struct , passagem por valor , passagem por referencia ,
 *   menu switch do-while , system cls pause , conio.h getche
 *
 * COMPILACAO (Windows / MinGW):
 *   gcc -Wall -Wextra gabarito_media_ponderada_classificacao.c -o programa
 * ========================================================================= */

/* ---- Bibliotecas ------------------------------------------------------- */
#include <stdio.h>   /* printf, scanf, fgets, fopen, fread, fwrite, fclose   */
#include <stdlib.h>  /* system, malloc/free (nao usado aqui, mas padrao)     */
#include <string.h>  /* strcspn (tirar o '\n' do fgets)                       */
#include <conio.h>   /* getche, getch - estilo do professor no Windows        */

/* ---- Definicoes de constante ------------------------------------------- */
/* Limite maximo de alunos. Usar #define deixa o codigo facil de alterar:
 * se a prova pedir N<=30, basta mudar UM numero aqui, nao em varios lugares. */
#define MAX_ALUNOS 20
/* Tamanho do campo nome. 49 caracteres uteis + '\0' no final. */
#define TAM_NOME 50

/* =========================================================================
 * DEFINICAO DA STRUCT
 * -------------------------------------------------------------------------
 * Em C puro (NAO C++), declaramos a struct com "typedef struct" para poder
 * usar o nome "Aluno" sozinho, sem precisar escrever "struct Aluno" toda
 * hora. Isso cai em prova: o professor costuma pedir exatamente essa forma.
 *
 * DETALHE IMPORTANTE DE SINTAXE:
 *   typedef struct { ... } NomeTipo;
 *   ^^^^^^                ^^^^^^^^
 *   A palavra "typedef" vem ANTES de "struct", e o nome do tipo vai DEPOIS
 *   da chave de fechamento. Inverter isso da erro de compilacao.
 * ========================================================================= */
typedef struct {
    int  codigo;          /* inteiro: matricula/codigo do aluno              */
    char nome[TAM_NOME];  /* vetor de char: nome do aluno (string em C)       */
    float lab;            /* nota do laboratorio (peso 2)                    */
    float semestral;      /* nota da prova semestral (peso 3)               */
    float final;          /* nota da prova final (peso 5)                    */
    float media;          /* media ponderada calculada pelo programa         */
    char  classificacao;  /* letra: A, B, C, D ou R                          */
} Aluno;  /* <-- nome do tipo. Agora "Aluno" pode ser usado como tipo.      */


/* =========================================================================
 * PROTOTIPOS DAS FUNCOES
 * -------------------------------------------------------------------------
 * Em C, se uma funcao for definida depois do main(), ela precisa ser
 * "anunciada" antes com o prototipo (so a assinatura, sem corpo). Isso
 * evita o warning implicit declaration. O professor costuma cobrar isso.
 * ========================================================================= */
int   lerQuantidadeAlunos(void);
void  lerAluno(Aluno *a);            /* por referencia: precisa MODIFICAR o aluno */
float calcularMedia(Aluno a);        /* por valor: so PRECISA LER os campos        */
char  calcularClassificacao(float m);/* recebe a media, devolve a letra           */
void  classificarAluno(Aluno *a);    /* preenche media e classificacao do aluno    */
void  imprimirAluno(Aluno a);       /* imprime um aluno na tela                   */
void  imprimirTodos(Aluno turma[], int n); /* recebe o vetor e a quantidade      */
void  limparBuffer(void);           /* limpa o '\n' deixado pelo scanf            */


/* =========================================================================
 * FUNCAO: limparBuffer
 * -------------------------------------------------------------------------
 * O scanf deixa o ENTER ('\n') no buffer de entrada. Se nao limpar, o
 * proximo fgets le esse '\n' e acha que o usuario digitou vazio. Por isso,
 * depois de TODO scanf, chamamos esta funcao.
 *
 * O professor usa fflush(stdin) para isso, MAS fflush(stdin) eh COMPORTAMENTO
 * NAO DEFINIDO segundo o padrao C (so funciona em alguns compiladores, como
 * o MinGW no Windows). O jeito PORTAVEL e correto eh este loop abaixo.
 * ========================================================================= */
void limparBuffer(void) {
    int c;
    /* le e descarta caracteres ate achar '\n' ou EOF (fim de arquivo) */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* corpo vazio: so queremos descartar os caracteres */
    }
}


/* =========================================================================
 * FUNCAO: lerQuantidadeAlunos
 * -------------------------------------------------------------------------
 * Pede ao usuario quantos alunos serao cadastrados. Valida para que fique
 * entre 1 e MAX_ALUNOS (20). Se o usuario digitar fora disso, pede de novo.
 * Retorna a quantidade valida.
 * ========================================================================= */
int lerQuantidadeAlunos(void) {
    /* Inicializamos n com 0 para evitar COMPORTAMENTO INDEFINIDO: se o
     * scanf abaixo falhar (usuario digitou texto nao numerico), ele
     * retorna 0 e NAO altera n. Se n ficasse sem inicializar, o teste
     * do do-while (n < 1 || n > MAX_ALUNOS) leria lixo -> UB. */
    int n = 0;
    do {
        printf("Quantos alunos deseja cadastrar? (1 a %d): ", MAX_ALUNOS);
        /* Verificamos o RETORNO do scanf. Ele retorna a quantidade de
         * itens lidos com sucesso (1, se leu um int). Se o usuario
         * digitar texto nao numerico (ex: "abc"), scanf retorna 0, NAO
         * altera n e deixa o buffer sujo. Sem essa verificacao, n
         * ficaria com lixo e o buffer poluiria as proximas leituras. */
        if (scanf("%d", &n) != 1) {
            printf("Entrada invalida! Digite um NUMERO entre 1 e %d.\n\n",
                   MAX_ALUNOS);
            limparBuffer(); /* limpa o que ficou no buffer (o texto invalido) */
            n = 0; /* forca valor fora do intervalo para repetir o laco */
            continue;
        }
        limparBuffer(); /* limpa o '\n' deixado pelo scanf */

        /* Validacao: n deve ser >= 1 e <= MAX_ALUNOS.
         * Se nao for, o do-while repete ate digitar um valor valido. */
        if (n < 1 || n > MAX_ALUNOS) {
            printf("Valor invalido! Digite um numero entre 1 e %d.\n\n",
                   MAX_ALUNOS);
        }
    } while (n < 1 || n > MAX_ALUNOS); /* repete enquanto for invalido */

    return n; /* devolve a quantidade ja validada */
}


/* =========================================================================
 * FUNCAO: lerAluno
 * -------------------------------------------------------------------------
 * Recebe um PONTEIRO para Aluno (passagem por REFERENCIA). Como precisamos
 * GRAVAR os dados no aluno original (nao numa copia), usamos ponteiro.
 * Dentro da funcao, acessamos os campos com a seta "->" (nao o ponto "."),
 * porque "a" eh ponteiro.
 *
 * OBS sobre fgets + strcspn (PEGADINHA DE PROVA):
 *   - NUNCA use gets(): eh perigoso (nao limita o tamanho) e foi removido
 *     do padrao C11. O professor moderno exige fgets.
 *   - fgets le ate TAM_NOME-1 caracteres OU ate achar '\n'. O problema eh
 *     que ele GUARDA o '\n' no final da string. Para tirar, usamos strcspn
 *     para achar a posicao do '\n' e trocar por '\0' (fim de string).
 * ========================================================================= */
void lerAluno(Aluno *a) {
    printf("--- Cadastro do aluno ---\n");

    /* 1) Codigo: inteiro, lido com scanf. */
    printf("  Codigo: ");
    scanf("%d", &a->codigo);   /* "->" porque a eh ponteiro */
    limparBuffer();             /* tira o '\n' do buffer */

    /* 2) Nome: string. Usamos fgets para ler com seguranca. */
    printf("  Nome: ");
    /* fgets(destino, tamanho, origem):
     *   destino = a->nome (o vetor da struct, acessado por ponteiro)
     *   tamanho = TAM_NOME (limite do vetor, evita estourar o buffer)
     *   origem  = stdin (teclado) */
    fgets(a->nome, TAM_NOME, stdin);

    /* PEGADINHA: se o usuario digitou um nome MAIOR que TAM_NOME-1
     * caracteres, o fgets trunca em TAM_NOME-1 mas NAO le o '\n' final
     * (ele fica no buffer junto com o restante do nome). O proximo
     * scanf("%f", &a->lab) tentaria ler esses caracteres restantes,
     * falharia (retornaria 0) e deixaria a->lab com valor lixo - e o
     * mesmo afetaria a->semestral e a->final em cascata.
     *
     * Solucao: verificamos ANTES de remover o '\n' se ele esta presente.
     * Se NAO houver '\n' na string lida, eh porque o nome foi truncado
     * (nao coube). Nesse caso, descartamos o restante do buffer ate o
     * '\n' (ou EOF) para que os proximos scanf comecem limpos. */
    if (strchr(a->nome, '\n') == NULL) {
        /* nao achou '\n' -> nome foi truncado; limpa o resto do buffer */
        limparBuffer();
    }

    /* Agora tiramos o '\n' que o fgets colocou no final (se houver).
     * strcspn(a->nome, "\n") retorna o indice do primeiro '\n' encontrado.
     * Se nao achar, retorna o tamanho da string (e ai nao faz nada errado,
     * pois ja limpamos o buffer acima).
     * Trocamos essa posicao por '\0' para fechar a string corretamente. */
    a->nome[strcspn(a->nome, "\n")] = '\0';

    /* 3) Notas: tres floats. Podemos ler os tres de uma vez com scanf. */
    printf("  Nota laboratorio (0 a 10): ");
    scanf("%f", &a->lab);
    printf("  Nota semestral  (0 a 10): ");
    scanf("%f", &a->semestral);
    printf("  Nota final       (0 a 10): ");
    scanf("%f", &a->final);
    limparBuffer(); /* limpa o '\n' do ultimo scanf */

    /* A media e a classificacao NAO sao lidas do teclado: sao calculadas.
     * Por isso chamamos a funcao que calcula e preenche esses campos. */
    classificarAluno(a);

    printf("\n"); /* linha em branco para separar visualmente os cadastros */
}


/* =========================================================================
 * FUNCAO: calcularMedia
 * -------------------------------------------------------------------------
 * Recebe o aluno POR VALOR (so precisamos LER as notas, nao modificar).
 * Calcula a media ponderada:
 *      media = (lab*2 + semestral*3 + final*5) / (2+3+5)
 *            = (lab*2 + semestral*3 + final*5) / 10
 *
 * Os pesos 2, 3 e 5 somam 10, por isso dividimos por 10. Se os pesos fossem
 * diferentes, dividiriamos pela SOMA DOS PESOS (regra geral da media
 * ponderada). Cuidado: NAO confunda com media aritmetica simples (soma/3).
 * ========================================================================= */
float calcularMedia(Aluno a) {
    /* soma_ponderada = cada nota multiplicada pelo seu peso */
    float soma_ponderada = a.lab * 2.0f + a.semestral * 3.0f + a.final * 5.0f;
    /* usamos 2.0f, 3.0f, 5.0f (com .0f) para forcar calculo em float,
     * evitando warnings de conversao double->float. */

    /* divide pela soma dos pesos (2+3+5 = 10) */
    return soma_ponderada / 10.0f;
}


/* =========================================================================
 * FUNCAO: calcularClassificacao
 * -------------------------------------------------------------------------
 * Recebe a media (float) e devolve a letra de classificacao (char).
 * Regras (ATENCAO aos intervalos - pegadinha de prova):
 *      [8.0 , 10.0] -> 'A'
 *      [7.0 , 7.9 ] -> 'B'
 *      [6.0 , 6.9 ] -> 'C'
 *      [5.0 , 5.9 ] -> 'D'
 *      [0.0 , 4.9 ] -> 'R' (Reprovado)
 *
 * Como os intervalos sao "de cima para baixo" e nao se sobrepoe, usamos
 * if-else encadeado: assim que achar o primeiro verdadeiro, sai sem testar
 * os outros. Comecamos pelo MAIOR (>= 8) e descemos.
 *
 * Por que comecar do maior? Porque se comecasemos do menor (>= 0), TODOS
 * entrariam no primeiro if. A ordem dos testes importa!
 * ========================================================================= */
char calcularClassificacao(float m) {
    if (m >= 8.0f) {
        /* 8.0 a 10.0 -> A (Excelente) */
        return 'A';
    } else if (m >= 7.0f) {
        /* Se chegou aqui, eh porque m < 8.0. Entao 7.0 a 7.9 -> B */
        return 'B';
    } else if (m >= 6.0f) {
        /* Se chegou aqui, m < 7.0. Entao 6.0 a 6.9 -> C */
        return 'C';
    } else if (m >= 5.0f) {
        /* Se chegou aqui, m < 6.0. Entao 5.0 a 5.9 -> D */
        return 'D';
    } else {
        /* Se chegou aqui, m < 5.0. Entao 0.0 a 4.9 -> R (Reprovado) */
        return 'R';
    }
}


/* =========================================================================
 * FUNCAO: classificarAluno
 * -------------------------------------------------------------------------
 * Recebe o aluno por REFERENCIA (ponteiro) porque precisa MODIFICAR dois
 * campos: media e classificacao. Chama as funcoes de calculo e grava o
 * resultado na struct original.
 *
 * Centralizar isso aqui evita repetir codigo: se a regra da media mudar,
 * so alteramos em calcularMedia(). Isso eh o espirito da MODULARIZACAO.
 * ========================================================================= */
void classificarAluno(Aluno *a) {
    /* Calcula a media chamando a funcao e guarda no campo "media" da struct.
     * Como a->media eh float e calcularMedia retorna float, tipos batem. */
    a->media = calcularMedia(*a);  /* *a = conteudo apontado (o aluno) */

    /* Calcula a classificacao a partir da media recem-calculada. */
    a->classificacao = calcularClassificacao(a->media);
}


/* =========================================================================
 * FUNCAO: imprimirAluno
 * -------------------------------------------------------------------------
 * Recebe o aluno POR VALOR (so vamos LER para imprimir, nao modificar).
 * Mostra os dados de UM aluno formatado na tela.
 *
 * "%.2f" formata float com 2 casas decimais (ex: 7.50).
 * "%c"  imprime um unico char (a classificacao).
 * ========================================================================= */
void imprimirAluno(Aluno a) {
    /* %-20s alinha o nome a esquerda em 20 espacos (tabela bonita). */
    printf("Codigo: %-5d  Nome: %-20s  ", a.codigo, a.nome);
    printf("Lab: %-4.1f  Sem: %-4.1f  Fin: %-4.1f  ",
           a.lab, a.semestral, a.final);
    printf("Media: %-4.1f  Classificacao: %c\n", a.media, a.classificacao);
}


/* =========================================================================
 * FUNCAO: imprimirTodos
 * -------------------------------------------------------------------------
 * Recebe o VETOR de alunos (turma) e a quantidade (n). Percorre o vetor
 * com um for e imprime cada aluno chamando imprimirAluno().
 *
 * Em C, quando passamos um vetor para uma funcao, nao passamos o tamanho
 * junto - por isso precisamos passar "n" separadamente. O vetor decai para
 * ponteiro, entao nao da para saber o tamanho la dentro (sizeof nao funciona).
 * ========================================================================= */
void imprimirTodos(Aluno turma[], int n) {
    int i;
    printf("\n========== LISTAGEM DOS ALUNOS ==========\n");
    /* Cabecalho da tabela (so para ficar bonito e didatico) */
    printf("%-7s %-20s %-5s %-5s %-5s %-6s %s\n",
           "Cod", "Nome", "Lab", "Sem", "Fin", "Media", "Class");
    printf("------------------------------------------------------------\n");

    /* Laco for classico: i vai de 0 ate n-1 (indices validos do vetor).
     * Cuidado: vetor comeca em 0, nao em 1. Acessar turma[n] eh erro. */
    for (i = 0; i < n; i++) {
        imprimirAluno(turma[i]); /* imprime o aluno da posicao i */
    }
    printf("==========================================\n");
}


/* =========================================================================
 * FUNCAO: main
 * -------------------------------------------------------------------------
 * Ponto de entrada do programa. Aqui juntamos tudo:
 *   1. Le a quantidade de alunos.
 *   2. Le cada aluno (preenchendo media e classificacao automaticamente).
 *   3. Imprime a listagem completa.
 *
 * O professor gosta de menu com switch + do-while. Mesmo sendo um exercicio
 * simples, mantemos o estilo para treinar o que cai na prova.
 * ========================================================================= */
int main(void) {
    /* Vetor de struct: declaramos ja com o tamanho maximo. Isso evita
     * usar malloc (alocacao dinamica), que eh topico mais avancado.
     * So usaremos as primeiras "n" posicoes. */
    Aluno turma[MAX_ALUNOS];
    /* Inicializamos n = 0 para evitar COMPORTAMENTO INDEFINIDO: se o
     * usuario escolher a opcao 2 (Listar) ANTES da opcao 1 (Cadastrar),
     * o teste "if (n < 1 || n > MAX_ALUNOS)" abaixo leria n nao
     * inicializado (lixo). Se o lixo casse em [1,20], imprimirTodos()
     * seria chamado e imprimiria registros de turma[] nunca preenchidos
     * (codigos/nomes/notas com lixo; printf %-20s pode ler alem do
     * buffer sem '\0'). Com n = 0, o teste cai no "Nenhum aluno
     * cadastrado" e nada de errado acontece. */
    int n = 0;        /* quantidade real de alunos cadastrados */
    int opcao = 0;    /* opcao do menu (0 = ainda nao escolheu) */
    int i;        /* contador do for */

    /* Limpa a tela no Windows. system() vem da stdlib.h.
     * Em Linux/Mac seria "clear", mas a prova eh Windows (MinGW). */
    system("cls");

    /* Menu principal com do-while + switch: estilo cobrado pelo professor.
     * O do-while garante que o menu aparece PELO MENOS uma vez. */
    do {
        printf("===== SISTEMA DE NOTAS - MEDIA PONDERADA =====\n");
        printf("1 - Cadastrar alunos e calcular medias\n");
        printf("2 - Listar todos os alunos\n");
        printf("3 - Sair\n");
        printf("Escolha uma opcao: ");
        /* Verificamos o RETORNO do scanf. Se o usuario digitar texto nao
         * numerico (ex: "abc"), scanf retorna 0, NAO altera opcao e deixa
         * o buffer sujo. Sem essa verificacao, opcao ficaria com lixo e
         * poderia cair em case 1/2/3 por acidente. */
        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida! Digite um NUMERO entre 1 e 3.\n");
            limparBuffer(); /* limpa o texto invalido do buffer */
            opcao = 0; /* forca valor fora do intervalo do menu */
        } else {
            limparBuffer(); /* limpa o '\n' do scanf */
        }

        system("cls"); /* limpa a tela antes de processar a opcao */

        switch (opcao) {
            case 1:
                /* ---- CADASTRO ---- */
                n = lerQuantidadeAlunos(); /* le e valida a quantidade */

                /* Le cada aluno. O laco for percorre de 0 ate n-1.
                 * Passamos &turma[i] (endereco) porque lerAluno modifica. */
                for (i = 0; i < n; i++) {
                    printf("\n>>> Aluno %d de %d <<<\n", i + 1, n);
                    lerAluno(&turma[i]);
                }

                printf("Cadastro concluido! %d aluno(s) registrado(s).\n", n);
                break;

            case 2:
                /* ---- LISTAGEM ---- */
                /* Antes de listar, verificamos se ja cadastrou alguem.
                 * Como inicializamos n = 0 la em cima, se o usuario
                 * ainda nao passou pela opcao 1, n vale 0 e cai aqui.
                 * O teste de intervalo (n < 1 || n > MAX_ALUNOS) tambem
                 * protege contra qualquer valor invalido. Se nao estiver
                 * no intervalo valido, pedimos para cadastrar primeiro. */
                if (n < 1 || n > MAX_ALUNOS) {
                    printf("Nenhum aluno cadastrado ainda! "
                           "Use a opcao 1 primeiro.\n");
                } else {
                    imprimirTodos(turma, n);
                }
                break;

            case 3:
                /* ---- SAIR ---- */
                printf("Encerrando o programa. Boa prova!\n");
                break;

            default:
                /* Opcao invalida: qualquer numero fora de 1, 2 ou 3. */
                printf("Opcao invalida! Escolha entre 1 e 3.\n");
                break;
        }

        /* system("pause") congela a tela ate o usuario apertar uma tecla.
         * Estilo do professor no Windows. Mostra "Pressione qualquer tecla". */
        if (opcao != 3) {
            system("pause");
            system("cls");
        }

    } while (opcao != 3); /* repete o menu ate escolher Sair (3) */

    /* getche() espera uma tecla ser apertada e a devolve (estilo conio.h).
     * Usado aqui so para a tela nao fechar sozinha em alguns ambientes.
     * O professor usa bastante no fim dos programas. */
    printf("\nPressione qualquer tecla para fechar...");
    getche();

    return 0; /* retorno 0 indica que o programa terminou com sucesso */
}
