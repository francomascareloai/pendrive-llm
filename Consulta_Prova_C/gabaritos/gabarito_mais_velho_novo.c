/*
 ============================================================================
  ARQUIVO : gabarito_mais_velho_novo.c
 ----------------------------------------------------------------------------
  O QUE FAZ : Exercicio 1 de STRUCT. Cadastra 5 alunos (nome, idade, sexo,
              matricula, cidade, curso) e possui 3 modulos:
                1) leitura  -> ler os 5 alunos
                2) impressao -> mostrar todos os alunos cadastrados
                3) mais velho / mais novo -> encontra e IMPRIME dentro da
                   funcao o nome do aluno mais velho e do mais novo
 ----------------------------------------------------------------------------
  TOPICO : struct, strings, modularizacao (funcoes), menu com switch
 ----------------------------------------------------------------------------
  PALAVRAS-CHAVE (Ctrl-F) :
    struct , typedef struct , Aluno , fgets , strcspn , fflush(stdin) ,
    conio.h , getche , system cls , system pause , menu switch do-while ,
    mais velho , mais novo , modularizacao , funcao , vetor de struct ,
    passagem de struct por parametro , strcmp
 ----------------------------------------------------------------------------
  COMPILACAO (Windows / MinGW) :
    gcc -Wall -Wextra gabarito_mais_velho_novo.c -o gabarito_mais_velho_novo
 ----------------------------------------------------------------------------
  OBS DO ESTILO : Mantemos o estilo do professor (conio.h, system("cls"),
  system("pause"), getche, fflush(stdin), menu com switch). Os bugs reais
  foram corrigidos: NAO usa gets() (usa fgets+strcspn), NAO usa
  while(!feof(fp)) com fread antes do laco.
 ============================================================================
*/

/* ---- BIBLIOTECAS ---- */
#include <stdio.h>   /* printf, scanf, fgets, fopen, fclose, fread, fwrite   */
#include <stdlib.h>  /* system, malloc, free                               */
#include <string.h>  /* strcspn (tira o \n do fgets), strcmp               */
#include <conio.h>   /* getche, getch  (estilo do professor, Windows/MinGW)*/

/* ---- DEFINICAO DA STRUCT ----
   typedef struct { campos } NomeTipo;
   Assim podemos declarar "Aluno a;" em vez de "struct Aluno a;".
   Isso cai MUITO na prova: o professor quer o typedef correto.            */
typedef struct {
    char nome[50];      /* nome do aluno (string -> vetor de char)          */
    int  idade;          /* idade do aluno (inteiro)                        */
    char sexo;           /* 'M' ou 'F' (um unico caractere)                 */
    int  matricula;      /* numero de matricula (inteiro)                   */
    char cidade[30];     /* cidade do aluno (string)                        */
    char curso[30];      /* curso do aluno (string)                         */
} Aluno;                 /* <-- ponto-e-virgula OBRIGATORIO no fim do typedef */

/* ---- PROTOTIPOS DAS FUNCOES ----
   Declarar antes do main() para o compilador conhecer as funcoes.
   Isso e MODULARIZACAO: cada funcao faz UMA coisa so.                      */
void lerAlunos(Aluno v[], int n);        /* modulo 1: leitura              */
void imprimirAlunos(Aluno v[], int n);   /* modulo 2: impressao            */
void maisVelhoMaisNovo(Aluno v[], int n);/* modulo 3: acha velho/novo     */
void limparBuffer(void);                 /* limpa o \n deixado pelo scanf  */

/* =========================================================================
   FUNCAO PRINCIPAL
   ========================================================================= */
int main(void)
{
    /* Vetor de 5 alunos. "Aluno" ja e o tipo graas ao typedef.
       Em C puro, sem typedef, teriamos que escrever "struct Aluno turma[5];" */
    Aluno turma[5];

    /* Quantidade de alunos (constante, facilita mudar o tamanho depois) */
    const int N = 5;

    int opcao;   /* opcao do menu (switch) */

    /* ---- MENU com DO-WHILE + SWITCH (estilo do professor) ----
       O do-while garante que o menu aparece PELO MENOS 1 vez.
       O switch trata cada opcao.                                       */
    do {
        system("cls");   /* limpa a tela (Windows). Estilo do professor.  */

        printf("========== MENU - CADASTRO DE ALUNOS ==========\n");
        printf("1 - Ler 5 alunos\n");
        printf("2 - Imprimir todos os alunos\n");
        printf("3 - Mostrar o MAIS VELHO e o MAIS NOVO\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        /* scanf de int: le a opcao digitada pelo usuario.
           Deixa um '\n' no buffer (o Enter), por isso chamamos limparBuffer. */
        scanf("%d", &opcao);
        limparBuffer();   /* tira o '\n' que sobrou do scanf acima          */

        switch (opcao) {
            case 1:
                lerAlunos(turma, N);          /* modulo de leitura         */
                break;
            case 2:
                imprimirAlunos(turma, N);     /* modulo de impressao        */
                break;
            case 3:
                maisVelhoMaisNovo(turma, N);  /* modulo velho/novo         */
                break;
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                /* default: opcao invalida. Sempre coloque um default no switch. */
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

        /* system("pause") so faz sentido se NAO for a opcao de sair.
           Pausa para o usuario ler a saida antes de limpar a tela.        */
        if (opcao != 0) {
            system("pause");
        }

    } while (opcao != 0);   /* repete enquanto nao escolher sair (0)       */

    return 0;   /* fim do programa com sucesso                              */
}

/* =========================================================================
   MODULO 1 : LER ALUNOS
   -------------------------------------------------------------------------
   Recebe o VETOR de Aluno e a quantidade n.
   Em C, quando passamos um vetor para uma funcao, passamos o endereco
   do primeiro elemento. Por isso as alteracoes feitas aqui refletem no
   vetor "turma" do main() (passagem por referencia de vetor).
   ========================================================================= */
void lerAlunos(Aluno v[], int n)
{
    int i;   /* contador do laco */

    printf("\n--- CADASTRO DE %d ALUNOS ---\n", n);

    for (i = 0; i < n; i++) {
        printf("\n== Aluno %d ==\n", i + 1);

        /* --- NOME (string) ---
           NAO usar gets() (obsoleto e perigoso, causa buffer overflow).
           Usar fgets: le ate 49 caracteres (deixa espaco para o '\0').
           O fgets inclui o '\n' no final da string, por isso usamos
           strcspn para achar a posicao do '\n' e trocar por '\0'.        */
        printf("Nome: ");
        fflush(stdin);   /* limpa o buffer antes de ler string.
                            ATENCAO: fflush(stdin) NAO e padrao ANSI/C,
                            mas funciona no Windows (MinGW). O professor
                            usa, entao mantemos. Em Linux/Mac nao funciona. */
        fgets(v[i].nome, 50, stdin);
        /* strcspn retorna o indice do primeiro '\n' encontrado.
           Colocamos '\0' nessa posicao, removendo o Enter.               */
        v[i].nome[strcspn(v[i].nome, "\n")] = '\0';

        /* --- IDADE (inteiro) ---
           scanf de int. Depois limpamos o buffer para nao atrapalhar
           a leitura seguinte de string/char.                              */
        printf("Idade: ");
        scanf("%d", &v[i].idade);
        limparBuffer();

        /* --- SEXO (char unico) ---
           Poderiamos usar scanf(" %c", ...) com espaco antes do %c
           (o espaco ignora whitespace/Enter anterior).
           Aqui usamos getche() no estilo do professor: le 1 caractere
           e mostra na tela, SEM precisar apertar Enter.                   */
        printf("Sexo (M/F): ");
        v[i].sexo = getche();   /* le 1 caractere e ecoa na tela            */
        printf("\n");           /* getche nao pula linha, entao pulamos     */

        /* --- MATRICULA (inteiro) --- */
        printf("Matricula: ");
        scanf("%d", &v[i].matricula);
        limparBuffer();

        /* --- CIDADE (string) --- */
        printf("Cidade: ");
        fflush(stdin);
        fgets(v[i].cidade, 30, stdin);
        v[i].cidade[strcspn(v[i].cidade, "\n")] = '\0';

        /* --- CURSO (string) --- */
        printf("Curso: ");
        fflush(stdin);
        fgets(v[i].curso, 30, stdin);
        v[i].curso[strcspn(v[i].curso, "\n")] = '\0';
    }

    printf("\n>> Cadastro concluido com sucesso!\n");
}

/* =========================================================================
   MODULO 2 : IMPRIMIR TODOS OS ALUNOS
   -------------------------------------------------------------------------
   Percorre o vetor e mostra cada aluno formatado.
   Como so VAMOS LER (nao alterar), poderiamos usar "const Aluno v[]",
   mas mantemos simples para ficar igual ao estilo do professor.
   ========================================================================= */
void imprimirAlunos(Aluno v[], int n)
{
    int i;

    printf("\n--- LISTA DE ALUNOS CADASTRADOS ---\n");

    for (i = 0; i < n; i++) {
        printf("\n----- Aluno %d -----\n", i + 1);
        printf("Nome      : %s\n",  v[i].nome);
        printf("Idade     : %d\n",  v[i].idade);
        printf("Sexo      : %c\n",  v[i].sexo);
        printf("Matricula : %d\n",  v[i].matricula);
        printf("Cidade    : %s\n",  v[i].cidade);
        printf("Curso     : %s\n",  v[i].curso);
    }
}

/* =========================================================================
   MODULO 3 : MAIS VELHO E MAIS NOVO
   -------------------------------------------------------------------------
   Encontra o aluno com a MAIOR idade (mais velho) e o com a MENOR idade
   (mais novo). IMPRIME os nomes DENTRO da funcao (como pede o enunciado).

   LOGICA (muito comum em prova):
     - Comecamos assumindo que o PRIMEIRO aluno (indice 0) e ao mesmo
       tempo o mais velho E o mais novo. Isso e o "ponto de partida".
     - Depois comecamos a comparar a partir do indice 1.
     - Se achar alguem com idade MAIOR que a do "mais velho atual",
       atualizamos o indice do mais velho.
     - Se achar alguem com idade MENOR que a do "mais novo atual",
       atualizamos o indice do mais novo.
   ========================================================================= */
void maisVelhoMaisNovo(Aluno v[], int n)
{
    int i;
    int idxVelho;   /* indice (posicao no vetor) do aluno mais velho      */
    int idxNovo;    /* indice (posicao no vetor) do aluno mais novo       */

    /* Se o vetor estiver vazio, nao faz sentido procurar. Seguranca.     */
    if (n <= 0) {
        printf("\nNenhum aluno cadastrado.\n");
        return;
    }

    /* PONTO DE PARTIDA: o primeiro aluno (indice 0) e, por enquanto,
       tanto o mais velho quanto o mais novo. A medida que percorremos
       o vetor, vamos atualizando esses indices se acharmos alguem
       mais velho ou mais novo.                                           */
    idxVelho = 0;
    idxNovo  = 0;

    /* Comecamos em i = 1 porque o indice 0 ja e nossa referencia inicial. */
    for (i = 1; i < n; i++) {

        /* Achou alguem com idade MAIOR que a do mais velho atual?
           Se sim, esse alguem passa a ser o mais velho.                  */
        if (v[i].idade > v[idxVelho].idade) {
            idxVelho = i;   /* atualiza o indice do mais velho            */
        }

        /* Achou alguem com idade MENOR que a do mais novo atual?
           Se sim, esse alguem passa a ser o mais novo.
           ATENCAO: use < (menor) para o mais novo, e > (maior) para o
           mais velho. Nao confunda os operadores!                         */
        if (v[i].idade < v[idxNovo].idade) {
            idxNovo = i;    /* atualiza o indice do mais novo              */
        }
    }

    /* IMPRIME DENTRO DA FUNCAO (como pede o enunciado).
       Usamos v[idxVelho].nome para acessar o nome do aluno mais velho.   */
    printf("\n--- RESULTADO ---\n");
    printf("Aluno MAIS VELHO : %s  (idade %d)\n",
           v[idxVelho].nome, v[idxVelho].idade);
    printf("Aluno MAIS NOVO  : %s  (idade %d)\n",
           v[idxNovo].nome,  v[idxNovo].idade);
}

/* =========================================================================
   FUNCAO AUXILIAR : limparBuffer
   -------------------------------------------------------------------------
   Le e descarta todos os caracteres que estao no buffer de entrada ate
   (e incluindo) o '\n'. Isso evita que o '\n' deixado pelo scanf de int
   seja lido pelo proximo fgets/getche, o que pularia a leitura da string.

   Por que nao usar fflush(stdin) aqui tambem?
   - fflush(stdin) funciona no Windows (MinGW) mas NAO e padrao ANSI.
   - Esta funcao com getchar() funciona em QUALQUER sistema (portavel).
   - O professor usa fflush(stdin), entao mantemos ambos no codigo para
     voce ver as duas formas.                                              */
void limparBuffer(void)
{
    int c;
    /* getchar() le 1 caractere por vez. Descartamos ate achar '\n' ou EOF. */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta o caractere, nao faz nada com ele */
    }
}
