/*
 ============================================================================
 ARQUIVO    : 22_struct_produto_media.c
 DESCRICAO  : Cadastro de 3 produtos (codigo, descricao, pcusto, pvenda, cor)
              usando struct. Modulos de leitura, impressao e calculo da media
              dos precos de venda.
 TOPICO     : struct, typedef, vetor de struct, modularizacao (funcoes),
              passagem de vetor de struct por parametro.
 PALAVRAS-CHAVE (Ctrl-F): struct, typedef struct, PRODUTO, vetor de struct,
              passagem por parametro, funcao que retorna float, media,
              fgets, strcspn, fflush stdin, modularizacao, leitura,
              impressao, calculo, gets removido.
 COMPILACAO : gcc -Wall -Wextra 22_struct_produto_media.c -o programa
 ============================================================================
*/

#include <stdio.h>   /* printf, scanf, fgets, stdin  */
#include <string.h>  /* strcspn  -> usado para remover o '\n' do fgets */

/* -------------------------------------------------------------------------
   DEFINICAO DA STRUCT (TIPO DE DADO)
   -------------------------------------------------------------------------
   - "typedef struct { ... } PRODUTO;" cria um NOVO TIPO chamado PRODUTO.
   - IMPORTANTE (PEGADINHA DE PROVA): a sintaxe correta em C puro e
       typedef struct { campos } NOME;
     (o nome do tipo vem DEPOIS do fecha-chaves). Se escrever
       typedef struct NOME { ... };   (sem alias depois do '}')
     NAO compila em C puro (so funcionaria em C++). O professor as vezes
     escreve errado; o jeito certo e o que esta aqui.
   - Cada campo guarda uma informacao de um produto:
       codigo    -> inteiro, identificador do produto
       descricao -> texto (vetor de char) ate 29 caracteres + '\0'
       pcusto    -> preco de custo (real)
       pvenda    -> preco de venda (real)
       cor       -> texto (vetor de char) ate 19 caracteres + '\0'
   ------------------------------------------------------------------------- */
typedef struct {
    int   codigo;        /* codigo identificador do produto (inteiro)        */
    char  descricao[30]; /* nome/descricao do produto (29 chars + '\0')     */
    float pcusto;        /* preco de custo (ponto flutuante)                */
    float pvenda;        /* preco de venda (ponto flutuante)                */
    char  cor[20];       /* cor do produto (19 chars + '\0')                */
} PRODUTO;               /* <-- alias do tipo: agora "PRODUTO" e um tipo valido */


/* -------------------------------------------------------------------------
   MODULO LEITURA
   -------------------------------------------------------------------------
   - Recebe um VETOR de 3 PRODUTOs por parametro. Em C, vetores sao sempre
     passados por REFERENCIA (nao por copia): a funcao modifica o vetor
     original declarado no main. Por isso nao precisamos retornar nada.
   - Preenche os 3 produtos lendo do teclado.

   SOBRE fflush(stdin) (ESTILO DO PROFESSOR):
   - Limpa o buffer do teclado apos um scanf (que deixa o '\n' sobrando).
   - Funciona no Windows (MinGW / Dev-Cpp) mas NAO e portatil: o padrao C
     diz que fflush so e definido para fluxos de SAIDA (nao entrada).
   - Mantemos aqui por fidelidade ao estilo do professor (e o que cai na
     prova). A forma 100% portatil seria uma funcao limparBuffer() com
     getchar() em loop.

   SOBRE fgets NO LUGAR DE gets() (BUG CORRIGIDO):
   - gets() foi REMOVIDO do C (a partir do C11) porque e perigoso: ele nao
     limita quantos caracteres le, podendo estourar o buffer (falha classica
     de seguranca). O professor usa gets(); NUNCA use gets() na prova.
   - Trocamos por fgets(destino, tamanho, stdin), que le no maximo
     tamanho-1 caracteres (deixando espaco para o '\0').
   - DIFERENCA: o fgets GUARDA o '\n' (Enter) no final da string. Por isso
     removemos o '\n' com strcspn logo apos o fgets.
   ------------------------------------------------------------------------- */
void leitura(PRODUTO p[3])
{
    int j;  /* contador do laco (indice do vetor: 0, 1, 2) */

    /* Laco para ler os 3 produtos. j vai de 0 ate 2 (3 posicoes do vetor). */
    for (j = 0; j <= 2; j++)
    {
        printf("\n--- Produto %d ---\n", j + 1);  /* cabecalho de cada produto */

        /* codigo: INTEIRO. scanf de %d PRECISA do & (endereco da variavel). */
        printf("Digite o codigo do produto: ");
        scanf("%d", &p[j].codigo);

        /* descricao: STRING. Apos o scanf do codigo, o '\n' (Enter) fica no
           buffer do teclado. fflush(stdin) descarta esse '\n' para que o
           fgets nao o leia como se fosse uma entrada vazia. */
        fflush(stdin);  /* ver comentario acima: funciona no Windows */
        printf("Digite a descricao do produto: ");
        /* fgets le ate 29 caracteres (sizeof p[j].descricao - 1 == 29) e
           garante o '\0' no final. sizeof p[j].descricao == 30. */
        fgets(p[j].descricao, sizeof(p[j].descricao), stdin);
        /* strcspn retorna o indice do primeiro '\n' encontrado. Colocamos
           '\0' nessa posicao, REMOVENDO a quebra de linha que o fgets deixou. */
        p[j].descricao[strcspn(p[j].descricao, "\n")] = '\0';

        /* pcusto: REAL (float). scanf de %f tambem precisa do &. */
        printf("Digite o preco de custo do produto: ");
        scanf("%f", &p[j].pcusto);

        /* pvenda: REAL (float). */
        printf("Digite o preco de venda do produto: ");
        scanf("%f", &p[j].pvenda);

        /* cor: STRING. Novamente limpamos o buffer antes do fgets, pois o
           scanf do pvenda deixou um '\n' sobrando. */
        fflush(stdin);
        printf("Digite a cor do produto: ");
        fgets(p[j].cor, sizeof(p[j].cor), stdin);
        p[j].cor[strcspn(p[j].cor, "\n")] = '\0';  /* remove o '\n' do fgets */
    }
}


/* -------------------------------------------------------------------------
   MODULO IMPRESSAO
   -------------------------------------------------------------------------
   - Percorre o vetor de 3 PRODUTOs e imprime cada campo formatado.
   - %d    -> imprime inteiro (codigo)
   - %s    -> imprime string (descricao, cor)
   - %.2f  -> imprime float com 2 casas decimais (pcusto, pvenda)
   ------------------------------------------------------------------------- */
void impressao(PRODUTO p[3])
{
    int j;  /* contador do laco */

    for (j = 0; j <= 2; j++)
    {
        printf("\n===== Produto %d =====\n", j + 1);
        printf("Codigo do produto: %d\n",  p[j].codigo);    /* %d  -> int   */
        printf("Descricao: %s\n",         p[j].descricao);  /* %s  -> string*/
        printf("Preco de custo: %.2f\n",  p[j].pcusto);     /* %.2f -> float*/
        printf("Preco de venda: %.2f\n",  p[j].pvenda);     /* %.2f -> float*/
        printf("Cor: %s\n",               p[j].cor);        /* %s  -> string*/
    }
}


/* -------------------------------------------------------------------------
   MODULO CALCULO (MEDIA DOS PRECOS DE VENDA)
   -------------------------------------------------------------------------
   - Soma os pvenda dos 3 produtos e divide por 3.
   - Retorna um float (a media). Por isso o tipo de retorno e "float" e nao
     "void": a funcao PRODUZ um valor que o main vai usar.
   - total comeca em 0: SEMPRE inicialize acumuladores antes do laco, senao
     o programa soma lixo de memoria (bug classico).
   ------------------------------------------------------------------------- */
float calculo(PRODUTO p[3])
{
    int   j;            /* contador do laco                              */
    float total = 0;    /* acumulador da soma dos pvenda (inicia em 0!)  */

    for (j = 0; j <= 2; j++)
    {
        total = total + p[j].pvenda;  /* acumula o pvenda de cada produto */
    }

    /* Divide a soma por 3 para obter a media. Como total e float, a divisao
       e feita em ponto flutuante (resultado com casas decimais). Se total
       fosse int, total/3 faria divisao inteira e perderia os decimais. */
    return total / 3;
}


/* -------------------------------------------------------------------------
   FUNCAO PRINCIPAL
   -------------------------------------------------------------------------
   - "int main(void)" indica que main nao recebe argumentos e devolve um int.
   - O "return 0;" no fim avisa o sistema operacional que o programa terminou
     com sucesso. (main sem tipo de retorno, ou main() sem int, e pratica
     antiga e incorreta - sempre use int main.)
   ------------------------------------------------------------------------- */
int main(void)
{
    PRODUTO prod[3];   /* vetor de 3 produtos (indices validos: 0, 1, 2) */
    float   med;       /* guardara a media dos precos de venda           */

    leitura(prod);        /* chama modulo de leitura (preenche o vetor)   */
    impressao(prod);      /* chama modulo de impressao (mostra os dados)  */

    med = calculo(prod);  /* calculo retorna a media; guardamos em med   */

    printf("\nMedia dos valores de venda: %.2f\n", med);

    return 0;  /* fim do programa, retorno 0 = sucesso */
}
