/* =========================================================================
 * ARQUIVO: 17_arquivo_acesso_direto.c
 * --------------------------------------------------------------------------
 * O QUE FAZ:
 *   - Grava 10 numeros inteiros em um arquivo BINARIO ("matriz.bin") usando
 *     fwrite, no modo "wb" (write binary).
 *   - Reabre o mesmo arquivo em modo "rb" (read binary).
 *   - Usa fseek() para fazer ACESSO DIRETO: pula direto para a 6a posicao
 *     (indice 5) calculando o byte offset com sizeof(int)*5.
 *   - Le e imprime os 5 ultimos inteiros usando fread.
 *
 * TOPICO: Arquivos binarios - acesso direto com fseek (seek_set).
 *
 * PALAVRAS-CHAVE (Ctrl+F):
 *   arquivo binario, fwrite, fread, fseek, SEEK_SET, SEEK_CUR, SEEK_END,
 *   sizeof, byte offset, acesso direto, acesso aleatorio, wb, rb,
 *   matriz.bin, ponteiro de arquivo, FILE *, feof, limparBuffer.
 *
 * COMPILACAO:
 *   gcc -Wall -Wextra 17_arquivo_acesso_direto.c -o 17_arquivo_acesso_direto
 *
 * OBS.: Mantem o estilo do professor (system pause, fflush(stdin) com
 *       comentario explicando portabilidade). Bugs do original corrigidos:
 *       (1) int main com return 0; (2) loop de leitura trocado de
 *       while(!feof) para while(fread==1) evitando ler o ultimo 2x.
 * ========================================================================= */

#include <stdio.h>   /* fopen, fclose, fwrite, fread, fseek, printf, scanf  */
#include <stdlib.h>  /* system("pause")                                       */

/* -------------------------------------------------------------------------
 * limparBuffer: descarta caracteres restantes no stdin (teclado).
 * USO: chamar logo apos scanf("%d",...) para limpar o '\n' deixado pelo
 *      scanf, evitando que ele "pule" o proximo getchar/scanf de string.
 * NOTA SOBRE fflush(stdin): o professor usa fflush(stdin) que FUNCIONA no
 *      Windows (MinGW/MSVC) mas NAO eh portavel (comportamento indefinido
 *      no padrao C). Aqui mantemos fflush(stdin) com comentario para
 *      fidelidade ao estilo do professor, mas esta funcao limparBuffer eh
 *      a forma CORRETA e portatil de fazer a mesma coisa.
 * ---------------------------------------------------------------------- */
void limparBuffer(void) {
    int c;                                  /* variavel p/ guardar cada char  */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* le e descarta ate achar '\n' (Enter) ou fim de arquivo (EOF)      */
    }
}

/* -------------------------------------------------------------------------
 * FUNCAO PRINCIPAL
 * ---------------------------------------------------------------------- */
int main(void) {
    FILE *ARQ;       /* ponteiro para o arquivo binario (tipo FILE *)         */
    int i;           /* contador do loop de gravacao (0 a 9)                 */
    int valor;       /* variavel onde lemos cada inteiro digitado/gravado    */

    /* =====================================================================
     * PARTE 1: GRAVACAO - abrir "matriz.bin" em modo "wb" (write binary)
     * =====================================================================
     * "wb" cria (ou zera) o arquivo binario para escrita. Se o arquivo ja
     * existir, seu conteudo anterior eh DESTRUIDO (truncado).
     * Retorna NULL se falhar (ex.: disco cheio, pasta inexistente).
     * ------------------------------------------------------------------ */
    ARQ = fopen("matriz.bin", "wb");
    if (ARQ == NULL) {                       /* testa se a abertura falhou   */
        printf("Erro ao criar o arquivo matriz.bin\n");
        system("pause");                     /* pausa antes de sair (Windows)*/
        return 1;                            /* retorna codigo de erro       */
    }

    /* Loop para ler 10 inteiros do teclado e grava-los um a um no arquivo.
     * Cada fwrite grava 1 bloco de sizeof(int) bytes (geralmente 4 bytes)
     * a partir do endereco de 'valor'. O arquivo cresce sequencialmente.   */
    i = 0;                                   /* comeca no indice 0           */
    while (i < 10) {                         /* repete para i = 0..9 (10x)  */
        printf("Digite o %d elemento: ", i + 1); /* mostra qual elemento le */
        fflush(stdin);                       /* ATENCAO: funciona no Windows
                                              * mas NAO eh portavel (padrao
                                              * C diz que fflush so vale p/
                                              * saida, nao entrada). Para
                                              * codigo portatil use a funcao
                                              * limparBuffer() definida acima. */
        scanf("%d", &valor);                 /* le inteiro do teclado p/ valor*/

        /* fwrite(endereco, tamanho_de_cada_bloco, qtde_de_blocos, arquivo)
         * Aqui: grava 1 bloco de sizeof(int) bytes a partir de &valor.
         * Importante passar o ENDERECO (&valor), nao o valor direto.       */
        fwrite(&valor, sizeof(int), 1, ARQ);

        i++;                                 /* proximo elemento            */
    }

    fclose(ARQ);  /* FECHA o arquivo de escrita - essencial! Sem isso, dados
                   * podem ficar em buffer e nao serem gravados no disco.   */

    /* =====================================================================
     * PARTE 2: LEITURA COM ACESSO DIRETO - reabrir em modo "rb"
     * =====================================================================
     * "rb" abre arquivo binario existente para LEITURA. Nao cria arquivo.
     * Retorna NULL se o arquivo nao existir.                              */
    ARQ = fopen("matriz.bin", "rb");
    if (ARQ == NULL) {                       /* testa se a abertura falhou   */
        printf("Erro ao abrir matriz.bin para leitura\n");
        system("pause");
        return 1;
    }

    /* ---------------------------------------------------------------------
     * fseek: MOVE o "cursor" do arquivo para uma posicao de BYTE especifica.
     *
     * Sintaxe: fseek(ponteiro, offset_em_bytes, origem)
     *
     * Origens possiveis (definidas em stdio.h):
     *   SEEK_SET  -> a partir do INICIO do arquivo   (mais usado)
     *   SEEK_CUR  -> a partir da POSICAO ATUAL
     *   SEEK_END  -> a partir do FIM do arquivo      (so p/ leitura)
     *
     * CALCULO DO BYTE OFFSET (o conceito central deste exemplo):
     *   Queremos pular para a 6a posicao, ou seja, o indice 5 (0-indexado).
     *   Cada inteiro ocupa sizeof(int) bytes (normalmente 4 bytes).
     *   Entao o byte inicial do 6o elemento eh:
     *
     *        offset = sizeof(int) * 5
     *
     *   Exemplo pratico (assumindo sizeof(int) == 4):
     *     elemento 0 -> bytes  0..3   (offset 0)
     *     elemento 1 -> bytes  4..7   (offset 4)
     *     elemento 2 -> bytes  8..11  (offset 8)
     *     elemento 3 -> bytes 12..15 (offset 12)
     *     elemento 4 -> bytes 16..19 (offset 16)
     *     elemento 5 -> bytes 20..23 (offset 20)  <-- pula para AQUI
     *     elemento 6 -> bytes 24..27 (offset 24)
     *     ...
     *
     *   Assim, fseek posiciona o cursor no inicio do 6o inteiro. O proximo
     *   fread vai ler a partir dali (sem precisar ler os 5 primeiros).
     * ------------------------------------------------------------------ */
    fseek(ARQ, (sizeof(int) * 5), SEEK_SET);

    printf("Os valores a partir da 6. posicao sao:\n");

    /* ---------------------------------------------------------------------
     * LEITURA EM LOOP - forma CORRETA (correcao do bug do professor):
     *
     * BUG ORIGINAL (NAO usar):
     *     fread(&valor, sizeof(int), 1, ARQ);          // le 1 antes do loop
     *     while (!feof(ARQ)) {                          // feof so vira TRUE
     *         printf("%d\n", valor);                    // DEPOIS de tentar
     *         fread(&valor, sizeof(int), 1, ARQ);      // ler ALEM do fim ->
     *     }                                             // imprime o ultimo
     *                                                    // 2x / imprime lixo
     *
     * CORRECAO: usar o proprio retorno do fread como condicao do while.
     *   fread retorna o NUMERO DE BLOCOS lidos com sucesso. Como pedimos 1
     *   bloco, ele retorna 1 se leu, ou 0 se chegou ao fim (EOF). Assim o
     *   loop para EXATAMENTE quando nao ha mais dados - sem lixo, sem
     *   repeticao do ultimo registro.
     * ------------------------------------------------------------------ */
    while (fread(&valor, sizeof(int), 1, ARQ) == 1) {
        /* fread le 1 bloco de sizeof(int) bytes de ARQ para &valor.
         * Se conseguiu ler 1 bloco (retorno == 1), entra no loop e imprime. */
        printf("%d\n", valor);
    }
    /* Quando fread retornar 0 (chegou ao fim do arquivo), o while termina.  */

    fclose(ARQ);   /* sempre fechar o arquivo apos terminar de usa-lo       */

    system("pause");  /* pausa a tela no Windows (estilo do professor)        */
    return 0;         /* retorno 0 indica execucao com sucesso               */
}

/* =========================================================================
 * RESUMO DOS BUGS CORRIGIDOS (em relacao ao codigo original do professor):
 *
 *  1. main(void) sem tipo de retorno  ->  int main(void) + return 0;
 *  2. while(!feof(ARQ)) com fread ANTES do loop (le ultimo 2x / lixo)
 *     ->  while(fread(&valor,sizeof(int),1,ARQ) == 1) { ... }  (sem fread
 *         antes do loop). Esta eh a correcao mais importante deste arquivo.
 *  3. fflush(stdin) mantido (estilo do professor) MAS com comentario
 *     explicando que NAO eh portavel; funcao limparBuffer() fornecida como
 *     alternativa correta.
 *  4. Adicionado teste if (ARQ == NULL) apos cada fopen (tratamento de erro
 *     que faltava no original).
 *
 * NAO se aplica aqui (nao ha no original):
 *  - typedef struct (nao ha structs neste exemplo).
 *  - gets() (nao ha leitura de string).
 *  - printf com %03 sem 'd' (nao ha esse padrao aqui).
 * =========================================================================
 */
