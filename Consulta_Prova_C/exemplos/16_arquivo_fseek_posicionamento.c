/*
============================================================================
 ARQUIVO: 16_arquivo_fseek_posicionamento.c
 TOPICO:   Arquivos texto - posicionamento de cursor com fseek e rewind
 OBJETIVO: Demonstrar os 3 usos do fseek (SEEK_SET, SEEK_CUR, SEEK_END)
           e o rewind em arquivo texto. Cria example.txt com fputs e mostra
           como mover o cursor de leitura/escrita dentro do arquivo.
 PALAVRAS-CHAVE (Ctrl+F): fseek, SEEK_SET, SEEK_CUR, SEEK_END, rewind,
                          ftell, fopen, fputs, fgets, arquivo texto,
                          posicionamento, cursor, ponteiro de arquivo
 COMPILA:  gcc -Wall -Wextra 16_arquivo_fseek_posicionamento.c -o prog
============================================================================
*/

#include <stdio.h>   /* fopen, fclose, fputs, fgets, fseek, rewind, ftell */
#include <stdlib.h>  /* system, exit, EXIT_FAILURE, EXIT_SUCCESS          */
#include <string.h>  /* strcspn - para remover o '\n' lido por fgets      */
#include <conio.h>   /* getch, getche - estilo do professor (Windows/MinGW) */

/* -------------------------------------------------------------------------
   Prototipo da funcao de limpeza de buffer.
   fflush(stdin) funciona no Windows (MinGW) mas NAO é portatil segundo o
   padrao C (comportamento indefinido em Linux/Mac). Por fidelidade ao
   estilo do professor usamos fflush(stdin) no main, mas deixamos esta
   funcao comentada como alternativa correta/portatil.
   ------------------------------------------------------------------------- */
/* void limparBuffer(void){ int c; while((c=getchar())!='\n' && c!=EOF); } */

int main(void)
{
    FILE *p;                 /* ponteiro para o arquivo que vamos manipular */
    char linha[100];         /* buffer para ler uma linha do arquivo texto  */
    long posicao;            /* guarda a posicao do cursor retornada por ftell */

    /* =====================================================================
       PARTE 1 - CRIAR o arquivo texto "example.txt" no modo escrita ("w")
       =====================================================================
       Modo "w" = write (escrita). Se o arquivo nao existe, ele é criado.
       Se o arquivo JA existe, seu conteudo anterior é APAGADO (truncado).
       Por isso cuidado: abrir "w" sempre comeca um arquivo do zero.       */
    p = fopen("example.txt", "w");

    /* BUG COMUM do professor: ele NAO verifica se fopen retornou NULL.
       Se o disco estiver cheio, pasta inexistente ou sem permissao,
       fopen devolve NULL e qualquer fputs/fclose seguinte dá crash.
       CORRECAO: sempre testar o retorno de fopen antes de usar o arquivo. */
    if (p == NULL)
    {
        printf("Erro ao criar o arquivo example.txt\n");
        system("pause");        /* estilo do professor: pausa antes de sair */
        return EXIT_FAILURE;     /* retorna codigo de erro para o SO         */
    }

    /* fputs escreve a string NO arquivo (sem adicionar '\n' automaticamente).
       Diferente do printf, fputs nao aceita formatacao (%d, %s...), so texto.
       O cursor avanca conforme vamos escrevendo, do inicio para o fim.        */
    fputs("Esta e uma maca.", p);   /* escreve 17 caracteres (0 a 16)        */
    fputs(" amostra ", p);          /* continua escrevendo logo apos         */

    /* fclose ESSENCIAL: é ele que descarrega (flush) o buffer interno para
       o disco. Sem fclose, pode ser que nada (ou só parte) seja gravado.
       Apos fclose o ponteiro p nao pode mais ser usado.                      */
    fclose(p);

    /* =====================================================================
       PARTE 2 - REABRIR o arquivo em modo LEITURA ("r") para demonstrar
       o posicionamento do cursor com fseek e rewind.
       =====================================================================
       Modo "r" = read (leitura). O arquivo DEVE existir, senao fopen dá NULL.
       O cursor comeca na posicao 0 (inicio do arquivo).                    */
    p = fopen("example.txt", "r");
    if (p == NULL)
    {
        printf("Erro ao abrir o arquivo para leitura\n");
        system("pause");
        return EXIT_FAILURE;
    }

    /* ftell(p) devolve a posicao ATUAL do cursor (em bytes, desde o inicio).
       Logo apos abrir em "r", o cursor esta na posicao 0.                  */
    posicao = ftell(p);
    printf("Posicao do cursor ao abrir (r): %ld\n", posicao);

    /* =====================================================================
       PARTE 3 - OS 3 USOS DO fseek
       =====================================================================
       SINTAXE:  fseek(ponteiro, deslocamento, ORIGEM);
         - deslocamento: numero de bytes (positivo = avanca, negativo = recua)
         - ORIGEM: de onde contar o deslocamento. Pode ser:
             SEEK_SET -> a partir do INICIO do arquivo  (origem = 0)
             SEEK_CUR -> a partir da POSICAO ATUAL       (origem = 1)
             SEEK_END -> a partir do FIM do arquivo      (origem = 2)

       No original do professor os fseek vinham COMENTADOS para explicar o
       que cada um faria. Abaixo mantemos essa explicacao comentada E em
       seguida executamos cada um para voce ver o efeito real na leitura.
       ===================================================================== */

    /* --- fseek com SEEK_SET (a partir do INICIO) --------------------------
       fseek(p, 8, SEEK_SET);  -> move o cursor para o byte 8 contando do
                                  inicio do arquivo (origem fixa em 0).
                                  "Esta e uma maca." -> byte 8 é a letra 'm'
                                  de "maca". Independente de onde o cursor
                                  estava, SEEK_SET sempre conta do inicio.
       ---------------------------------------------------------------------- */
    fseek(p, 8, SEEK_SET);          /* posiciona no byte 8 (letra 'm')       */
    posicao = ftell(p);
    printf("\nApos fseek(p, 8, SEEK_SET) -> cursor em: %ld\n", posicao);

    /* fgets le ate 99 caracteres OU ate achar '\n' OU ate o fim do arquivo.
       Como o arquivo texto criado nao tem '\n', o fgets le ate o fim.
       Para mostrar o efeito do posicionamento, lemos so alguns caracteres.  */
    fgets(linha, 6, p);              /* le 5 caracteres a partir do cursor    */
    /* fgets inclui o '\0' no fim, entao le 5 chars + '\0'. Resultado: "maca." */
    printf("Lendo 5 chars a partir do byte 8: \"%s\"\n", linha);

    /* --- fseek com SEEK_CUR (a partir da POSICAO ATUAL) -------------------
       fseek(p, 2, SEEK_CUR);  -> avanca 2 bytes a partir de onde o cursor
                                  esta AGORA. Se estava no byte 13, vai para
                                  o 15. O deslocamento é RELATIVO a posicao
                                  atual. Serve para "pular" bytes no meio da
                                  leitura. Deslocamento negativo recua.
       ---------------------------------------------------------------------- */
    fseek(p, 2, SEEK_CUR);          /* avanca +2 bytes a partir do atual     */
    posicao = ftell(p);
    printf("\nApos fseek(p, 2, SEEK_CUR) -> cursor em: %ld\n", posicao);

    /* --- fseek com SEEK_END (a partir do FIM) -----------------------------
       fseek(p, -5, SEEK_END); -> vai para 5 bytes ANTES do fim do arquivo.
                                  SEEK_END quase sempre é usado com
                                  deslocamento NEGATIVO (para recuar a partir
                                  do fim). Com deslocamento 0 posiciona no
                                  fim (util para anexar/medir tamanho).
       ---------------------------------------------------------------------- */
    fseek(p, -5, SEEK_END);         /* recua 5 bytes a partir do fim         */
    posicao = ftell(p);
    printf("\nApos fseek(p, -5, SEEK_END) -> cursor em: %ld\n", posicao);

    /* Agora lemos o restante a partir dessa posicao recuada.
       O arquivo tem " amostra " no final, entao os ultimos 5 bytes sao
       "tra " (com espaco). Mostra como SEEK_END recua do fim.              */
    fgets(linha, sizeof(linha), p);  /* le do cursor atual ate o fim          */
    printf("Lendo do byte %ld ate o fim: \"%s\"\n", posicao, linha);

    /* =====================================================================
       PARTE 4 - rewind: volta o cursor para o INICIO do arquivo
       =====================================================================
       rewind(p) é equivalente a fseek(p, 0, SEEK_SET), mas é mais legivel
       e nao retorna erro (void). Usado para "reler" o arquivo do comeco
       sem precisar fechar e abrir de novo.                                  */
    rewind(p);                      /* cursor volta para a posicao 0         */
    posicao = ftell(p);
    printf("\nApos rewind(p) -> cursor em: %ld\n", posicao);

    /* Le o arquivo inteiro do inicio para confirmar que rewind funcionou. */
    fgets(linha, sizeof(linha), p);  /* le tudo do inicio ate o fim          */
    printf("Conteudo completo lido do inicio: \"%s\"\n", linha);

    /* =====================================================================
       RESUMO DOS 3 fseek + rewind (comentado, como no original do professor)
       =====================================================================
       fseek(p,  0, SEEK_SET);  // inicio do arquivo           (== rewind)
       fseek(p,  8, SEEK_SET);  // byte 8 a partir do inicio
       fseek(p,  2, SEEK_CUR);  // avanca 2 bytes da posicao atual
       fseek(p, -5, SEEK_END);  // recua 5 bytes a partir do fim
       rewind(p);               // volta ao inicio (equivale a fseek 0 SEEK_SET)
       ===================================================================== */

    fclose(p);                      /* sempre fechar o arquivo ao terminar    */

    printf("\n--- Fim da demonstracao de fseek e rewind ---\n");
    system("pause");                /* estilo do professor: pausa a tela      */
    return 0;                       /* retorno 0 = programa terminou com sucesso */
}

/*
============================================================================
 NOTAS PARA A PROVA:
 - fseek serve para POSICIONAR o cursor em qualquer ponto do arquivo.
 - Em arquivo TEXTO, fseek funciona bem com SEEK_SET e deslocamento 0,
   ou com SEEK_END/-n para descobrir o tamanho. Pular bytes no meio de
   texto pode ser perigoso se houver caracteres multibyte (UTF-8), mas
   em ASCII simples (como nesta prova) funciona byte a byte.
 - Em arquivo BINARIO o fseek é seguro em qualquer posicao (byte exato).
 - ftell devolve a posicao atual; use junto com fseek para depurar.
 - rewind(p) == fseek(p, 0, SEEK_SET), mas é mais claro e nao retorna erro.
 - CUIDADO: fseek NAO verifica limites - se pedir posicao alem do fim,
   o proximo fgets/fread pode falhar ou ler lixo. Sempre confirme com ftell.
============================================================================
*/
