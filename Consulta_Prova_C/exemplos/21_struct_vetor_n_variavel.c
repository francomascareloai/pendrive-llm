/*
 ============================================================================
 ARQUIVO  : 21_struct_vetor_n_variavel.c
 OBJETIVO : Cadastra N funcionarios (N variavel, ate MAX=10) em um VETOR de
            struct. Possui 3 modulos: leitura, escrita (relacao) e mediafun
            (media dos salarios). Baseado no codigo do professor, porem
            CORRIGIDO e super comentado.
 TOPICO   : struct + vetor com tamanho variavel (definido pelo usuario em
            tempo de execucao, limitado por #define MAX).
 PALAVRAS-CHAVE (Ctrl-F):
    struct , typedef struct , vetor de struct , funcionario ,
    leitura , escrita , mediafun , media de salarios ,
    fgets , strcspn , fflush(stdin) , limpar buffer ,
    #define MAX , for , passagem de vetor por parametro ,
    int main , return 0
 COMPILAR : gcc -Wall -Wextra 21_struct_vetor_n_variavel.c -o programa
 ============================================================================
*/

/* stdio.h  -> printf, scanf, fgets, fflush, getchar, stdin */
#include <stdio.h>
/* stdlib.h -> system("pause") e system("cls") */
#include <stdlib.h>
/* string.h -> strcspn (usada para remover o '\n' do fgets) */
#include <string.h>
/* conio.h  -> getche/getch (estilo do professor; funciona no Windows/MinGW).
   Pode nao existir em outros sistemas, mas cai na prova do professor. */
#include <conio.h>

/* Limite maximo do vetor. Como o vetor e alocado estaticamente em main(),
   o tamanho deve ser uma constante conhecida em tempo de compilacao.
   O usuario pode pedir N <= MAX; se pedir mais, truncamos para MAX. */
#define MAX 10

/* ----------------------------------------------------------------------------
   BUG CORRIGIDO #1: typedef struct.
   Original do professor:
       typedef struct funcionario { ... };
   Esse formato NAO cria um alias em C puro (so cria a tag 'struct funcionario').
   Por isso "funcionario f[MAX];" nao compila em C (compila em C++).
   CORRECAO: typedef struct { ... } Funcionario;  -> cria o alias 'Funcionario'.
   Assim "Funcionario f[MAX];" funciona em C puro. */
typedef struct {
    char nome[40];      /* nome do funcionario (string, 39 chars + '\0') */
    char cargo[20];     /* cargo/ocupacao */
    char endereco[30];  /* endereco resumido */
    char cep[10];       /* CEP (string, nao int, para manter zeros a esquerda) */
    float salario;      /* salario (float para valores com decimais) */
} Funcionario;

/* ----------------------------------------------------------------------------
   PROTOTIPOS das funcoes.
   Importante: quando passamos um VETOR de struct para uma funcao, o que e
   copiado nao sao os dados e sim o ENDERECO do primeiro elemento. Por isso
   qualquer alteracao feita em func[] dentro de leitura() reflete em f[] em
   main(). Vetores sao sempre passados por REFERENCIA (nao por valor). */
void leitura (Funcionario func[MAX], int n);  /* le os dados de n funcionarios */
void escrita (Funcionario func[MAX], int n);  /* imprime a relacao cadastrada  */
void mediafun(Funcionario func[MAX], int n);  /* calcula e mostra media salarial */

/* ----------------------------------------------------------------------------
   Funcao auxiliar para limpar o buffer de entrada (stdin).
   BUG CORRIGIDO #4: fflush(stdin) e "indefinido" pelo padrao C (so funciona
   em alguns compiladores Windows). A forma PORTAVEL e correta e ler e
   descartar caracteres ate achar '\n' ou EOF.
   Usamos esta funcao apos cada scanf para evitar que o '\n' deixado no buffer
   seja lido pelo proximo fgets (o que pularia a leitura do nome). */
void limparBuffer(void) {
    int c;
    /* getchar() le 1 char por vez; descartamos ate achar nova linha ou fim */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* apenas descarta o caractere */
    }
}

/* ----------------------------------------------------------------------------
   MODULO 1: LEITURA
   Le os dados de 'n' funcionarios e guarda no vetor func[].
   Como func[] e um VETOR (ponteiro para o primeiro elemento), as gravacoes
   aqui afetam o vetor original declarado em main(). */
void leitura(Funcionario func[MAX], int n) {
    int j;  /* contador do loop. Declarado fora do for (estilo C antigo) */

    printf("\n***** Cadastro de funcionarios *******\n");

    /* Loop de 0 ate n-1: preenche cada posicao do vetor.
       Usamos j < n (nao j <= n) porque os indices comecam em 0. */
    for (j = 0; j < n; j++) {
        printf("\nDados do Funcionario %d\n", j + 1);

        /* --- NOME ---
           BUG CORRIGIDO #3: gets() foi removido do padrao C (perigoso: nao
           checa tamanho, causa overflow). Trocamos por fgets(), que limita
           a leitura ao tamanho do buffer.
           fgets le no maximo sizeof(func[j].nome)-1 chars e poe '\0' no fim.
           POREM fgets guarda o '\n' digitado pelo Enter. Por isso removemos
           o '\n' com strcspn logo abaixo. */
        printf("Nome ......: ");
        fflush(stdin);  /* estilo do professor; em Windows limpa o buffer */
        fgets(func[j].nome, sizeof(func[j].nome), stdin);
        /* strcspn acha a posicao do '\n' (ou do '\0' se nao houver) e
           colocamos '\0' ali, removendo a quebra de linha. */
        func[j].nome[strcspn(func[j].nome, "\n")] = '\0';

        /* --- CARGO --- (mesma tecnica do nome) */
        printf("Cargo .....: ");
        fflush(stdin);
        fgets(func[j].cargo, sizeof(func[j].cargo), stdin);
        func[j].cargo[strcspn(func[j].cargo, "\n")] = '\0';

        /* --- ENDERECO --- */
        printf("Endereco..: ");
        fflush(stdin);
        fgets(func[j].endereco, sizeof(func[j].endereco), stdin);
        func[j].endereco[strcspn(func[j].endereco, "\n")] = '\0';

        /* --- CEP ---
           CEP guardado como STRING (nao int) para preservar zeros a esquerda
           (ex: 09123-456). Se fosse int, o 0 inicial sumiria. */
        printf("CEP.......: ");
        fflush(stdin);
        fgets(func[j].cep, sizeof(func[j].cep), stdin);
        func[j].cep[strcspn(func[j].cep, "\n")] = '\0';

        /* --- SALARIO ---
           scanf de float SEMPRE precisa do '&' (endereco da variavel).
           Strings (acima) nao usam '&' porque o nome do vetor ja e endereco. */
        printf("Salario...: ");
        fflush(stdin);
        scanf("%f", &func[j].salario);

        /* Apos o scanf de %f, sobra o '\n' do Enter no buffer.
           Chamamos limparBuffer para nao atrapalhar o fgets da proxima iteracao. */
        limparBuffer();
    }
}

/* ----------------------------------------------------------------------------
   MODULO 2: ESCRITA
   Percorre o vetor e imprime todos os funcionarios cadastrados.
   Como aqui so LEMOS os dados (nao alteramos), o vetor poderia ser declarado
   como 'const', mas mantemos o estilo do professor. */
void escrita(Funcionario func[MAX], int n) {
    int j;  /* contador */

    printf("\n========== RELACAO DOS FUNCIONARIOS ==========\n");

    for (j = 0; j < n; j++) {
        printf("\nDADOS DO FUNCIONARIO %d\n", j + 1);
        printf("Nome ....: %s\n",   func[j].nome);       /* %s para string  */
        printf("Cargo ...: %s\n",   func[j].cargo);
        printf("Endereco: %s\n",    func[j].endereco);
        printf("CEP .....: %s\n",   func[j].cep);
        /* %.2f -> float com 2 casas decimais (formato de moeda) */
        printf("Salario.: %.2f\n",  func[j].salario);
    }
}

/* ----------------------------------------------------------------------------
   MODULO 3: MEDIAFUN
   Calcula a media dos salarios de todos os funcionarios cadastrados.
   CUIDADO: se n == 0 ocorre divisao por zero. Por isso testamos antes. */
void mediafun(Funcionario func[MAX], int n) {
    int j;            /* contador */
    float total = 0;  /* acumulador: soma dos salarios. Inicia em 0! */
    float m;          /* media calculada */

    /* Soma todos os salarios do vetor */
    for (j = 0; j < n; j++) {
        total = total + func[j].salario;  /* acumula cada salario */
    }

    /* Protecao contra divisao por zero (se n == 0). Bug classico de prova. */
    if (n == 0) {
        printf("\nNenhum funcionario cadastrado. Nao ha media a calcular.\n");
        return;  /* sai da funcao sem calcular */
    }

    m = total / n;  /* media = soma / quantidade */
    printf("\nMedia dos salarios = %.2f\n", m);
}

/* ----------------------------------------------------------------------------
   FUNCAO PRINCIPAL
   BUG CORRIGIDO #5: o professor escreveu "main(void)" sem tipo de retorno.
   Em C, isso assume int implicitamente (C89), mas em padroes mais novos e
   erro. Corrigimos para "int main(void)" e adicionamos "return 0;" no fim. */
int main(void) {
    Funcionario f[MAX];  /* vetor de MAX funcionarios (alocacao estatica) */
    int num;             /* quantidade que o usuario quer cadastrar */

    /* Pede ao usuario quantos funcionarios deseja cadastrar.
       O valor deve ser <= MAX (10). Validamos logo abaixo. */
    printf("Informe o numero de funcionarios que deseja cadastrar (max %d): ", MAX);
    scanf("%d", &num);   /* & obrigatorio: num e int, nao vetor */
    limparBuffer();      /* remove o '\n' deixado pelo scanf */

    /* Validacao de limite: se o usuario pedir mais que MAX, truncamos.
       Se pedir <= 0, avisamos e encerramos (nao faz sentido cadastrar 0). */
    if (num > MAX) {
        printf("Valor maior que o maximo (%d). Usando %d.\n", MAX, MAX);
        num = MAX;       /* trava no limite maximo */
    }
    if (num <= 0) {
        printf("Quantidade invalida. Encerrando.\n");
        system("pause");
        return 1;         /* retorna codigo de erro (nao zero) */
    }

    /* Chamada dos 3 modulos, passando o VETOR 'f' e a quantidade 'num'.
       Como f e vetor, a passagem e por REFERENCIA: leitura() preenche o
       mesmo vetor que escrita() e mediafun() vao ler. */
    leitura(f, num);     /* 1. cadastra os dados      */
    escrita(f, num);     /* 2. mostra a relacao       */
    mediafun(f, num);    /* 3. calcula a media       */

    system("pause");     /* estilo do professor: congela a tela no Windows */
    return 0;            /* fim normal do programa (int main exige return) */
}
