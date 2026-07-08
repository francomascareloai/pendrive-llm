/*
=====================================================================
   20_struct_vetor_5_funcionarios.c
---------------------------------------------------------------------
   O QUE FAZ: Cadastra 5 funcionarios (codigo, nome, idade, salario)
              em um VETOR DE STRUCT, depois imprime um relatorio e
              calcula a media dos salarios. Tudo em memoria (SEM arquivo).
   TOPICO: struct + vetor + funcoes (modularizacao).
   PALAVRAS-CHAVE (Ctrl+F): struct, typedef struct, vetor de struct,
              FUNCIONARIO, funcao, passagem de vetor para funcao,
              leitura, imprimir, calcula_media, fgets, strcspn,
              fflush(stdin), media de salarios, for, scanf, printf.
---------------------------------------------------------------------
   COMPILA: gcc -Wall -Wextra 20_struct_vetor_5_funcionarios.c -o prog
=====================================================================
*/

/* Bibliotecas padrao de C */
#include <stdio.h>   /* printf, scanf, fgets, stdin, stdout        */
#include <stdlib.h> /* system("pause")                            */
#include <string.h> /* strcspn (usada para tirar o '\n' do fgets) */

/*
   DEFINICAO DO TIPO DE DADO (STRUCT).
   -----------------------------------------------
   "typedef struct { ... } FUNCIONARIO;" cria um NOVO tipo chamado
   FUNCIONARIO (em maiusculas, por convencao do professor).
   Assim podemos declarar "FUNCIONARIO fun[5];" como se fosse int.

   DETALHE IMPORTANTE (cai na prova!):
   - Em C puro, "struct nome { ... };" SEM alias NAO permite escrever
     so "nome variavel;" -- voce teria que escrever "struct nome var;".
   - Com typedef struct { ... } FUNCIONARIO; o alias FUNCIONARIO ja
     fica disponivel, e "FUNCIONARIO f[5];" compila direto.
   - O codigo original do professor ja usava essa forma correta,
     por isso NAO havia bug aqui (apenas confirmamos).
*/
typedef struct {
    int   codigo;     /* codigo inteiro do funcionario              */
    char  nome[50];   /* nome (string de ate 49 chars + '\0')       */
    int   idade;      /* idade em anos                              */
    float salario;    /* salario (float p/ aceitar centavos)        */
} FUNCIONARIO;        /* <-- alias do tipo                          */

/*
   PROTOTIPOS DAS FUNCOES.
   -----------------------------------------------
   Sempre declare os prototipos ANTES do main() para que o compilador
   conheca as assinaturas. Como recebemos um VETOR, o parametro pode
   ser escrito "FUNCIONARIO f[5]" ou "FUNCIONARIO f[]" -- em C, um
   vetor passado para funcao vira ponteiro, entao o tamanho [5] aqui
   eh so documentacao (o compilador ignora). Mantemos [5] por clareza.
*/
void  leitura(FUNCIONARIO f[5]);        /* preenche o vetor          */
void  imprimir(FUNCIONARIO f[5]);      /* mostra o vetor na tela     */
float calcula_media(FUNCIONARIO f[5]); /* retorna media dos salarios*/

/*
   FUNCAO: leitura
   -----------------------------------------------
   Preenche os 5 funcionarios digitados pelo usuario.
   - Recebe o VETOR por referencia (nao por copia!), entao as
     alteracoes feitas aqui refletem no main().
   - Usa fgets() para ler o nome (NAO usa gets(), que foi removido
     do padrao C e eh perigoso: nao checa tamanho, causa buffer overflow).
*/
void leitura(FUNCIONARIO f[5])
{
    int j;  /* contador do laco (declarado no topo, estilo C antigo) */

    printf("\n\nCadastro de funcionarios da empresa\n");

    /* Laco de 0 a 4 (5 funcionarios). j+1 mostra "Funcionario 1..5" */
    for (j = 0; j < 5; j++)
    {
        printf("\n Funcionario : %d ", j + 1);

        /* codigo: int -> scanf precisa do & (endereco da variavel) */
        printf("\n Digite o codigo:");
        scanf("%d", &f[j].codigo);

        /* nome: string (char[]). Depois de um scanf("%d") o '\n' do
           Enter fica no buffer. Se nao limparmos, o fgets a seguir
           leria esse '\n' e o nome ficaria vazio.
           fflush(stdin) funciona no Windows/MinGW (estilo do professor),
           MAS nao eh portatil (comportamento indefinido no padrao C).
           Em Linux/Mac, o correto seria uma funcao limparBuffer() com
           while((c=getchar())!='\n' && c!=EOF); -- aqui mantemos
           fflush(stdin) para fidelidade ao codigo do professor.        */
        printf("\n Digite o nome:");
        fflush(stdin);
        /* fgets le ate 49 caracteres (deixando 1 para o '\0') ou ate
           o Enter. Ele GUARDA o '\n' no fim da string, por isso
           precisamos remove-lo (ver linha strcspn abaixo).            */
        fgets(f[j].nome, sizeof(f[j].nome), stdin);
        /* strcspn retorna o indice do primeiro '\n' encontrado.
           Colocar '\0' nesse indice "corta" o '\n' do final.          */
        f[j].nome[strcspn(f[j].nome, "\n")] = '\0';

        /* idade: int -> scanf com & */
        printf("\n Digite a idade:");
        scanf("%d", &f[j].idade);

        /* salario: float -> scanf com %f e & (atencao: %f, nao %d!) */
        printf("\n Digite o salario:");
        scanf("%f", &f[j].salario);
    }
}

/*
   FUNCAO: imprimir
   -----------------------------------------------
   Mostra todos os funcionarios cadastrados.
   - So le o vetor, nao modifica -> poderia ser "const", mas mantemos
     a assinatura do professor.
   - %.2f imprime float com 2 casas decimais (formato de dinheiro).
*/
void imprimir(FUNCIONARIO f[5])
{
    int j;  /* contador */

    printf("\n\nRELATORIO DOS FUNCIONARIOS CADASTRADOS da empresa\n");

    for (j = 0; j < 5; j++)
    {
        printf("\n Funcionario : %d ", j + 1);
        printf("\n Codigo: %d",   f[j].codigo);   /* %d para int   */
        printf("\n Nome: %s",     f[j].nome);     /* %s para string*/
        printf("\n Idade: %d",   f[j].idade);     /* %d para int   */
        printf("\n Salario: %.2f", f[j].salario); /* %.2f p/ float */
    }
}

/*
   FUNCAO: calcula_media
   -----------------------------------------------
   Soma todos os salarios e divide por 5.
   - Retorna float (para nao perder a parte decimal da media).
   - "total" comeca em 0 (sempre inicialize acumuladores!).
   - O professor declarava "float total=0, me;" -- mantemos o estilo.
*/
float calcula_media(FUNCIONARIO f[5])
{
    int j;            /* contador                              */
    float total = 0;  /* acumulador da soma (inicia em ZERO!)  */
    float me;         /* guardara a media final                */

    for (j = 0; j < 5; j++)
    {
        total = total + f[j].salario;  /* soma cada salario   */
    }

    me = total / 5;   /* divisao por inteiro 5 -> resultado float */
    return me;        /* devolve a media para quem chamou          */
}

/*
   FUNCAO: main (ponto de entrada)
   -----------------------------------------------
   - CORRECAO DE BUG: o original era "main(void)" sem tipo de retorno.
     Em C moderno (e com -Wall -Wextra) o correto eh "int main(void)"
     e "return 0;" no final, indicando que o programa terminou OK.
   - Declara o VETOR de 5 FUNCIONARIOS na memoria (nao ha arquivo).
*/
int main(void)
{
    FUNCIONARIO fun[5];   /* vetor de 5 funcionarios (na memoria)   */
    float media_salario; /* recebera o retorno de calcula_media    */

    leitura(fun);                 /* 1o modulo: preenche o vetor    */
    imprimir(fun);                /* 2o modulo: mostra o relatorio  */
    media_salario = calcula_media(fun); /* 3o modulo: calcula media*/

    printf("\n Media dos salarios : %.2f", media_salario);

    system("pause");  /* mantem a janela aberta no Windows (estilo) */
    return 0;         /* CORRECAO: main retorna int -> return 0    */
}
