/* =====================================================================
 * 28_calculos_estatisticos.c
 * ---------------------------------------------------------------------
 * O QUE FAZ: Le N valores reais (float) do usuario, armazena em vetor e
 *            calcula varias estatisticas: media, mediana, moda, variancia,
 *            desvio padrao, valor minimo e valor maximo. Cada calculo em
 *            sua propria funcao. Mostra um menu para escolher a operacao.
 *
 * TOPICO: vetores, funcoes/modularizacao, calculos, estatistica basica.
 *
 * PALAVRAS-CHAVE (Ctrl-F): media, mediana, moda, variancia, desvio padrao,
 *   desvioPadrao, minimo, maximo, ordenar, bubble sort, vetor, funcao,
 *   modularizacao, menu, do-while, switch, conio.h, getche, fflush stdin,
 *   typedef struct, float, sqrt, pow, fabs, copia de vetor.
 *
 * COMPILACAO: gcc -Wall -Wextra 28_calculos_estatisticos.c -o estatistica.exe -lm
 *   (o -lm linka a biblioteca matematica sqrt/pow do math.h)
 * ===================================================================== */

/* ---- Bibliotecas padrao ---- */
#include <stdio.h>   /* printf, scanf, fgets, etc.                          */
#include <stdlib.h>  /* system, malloc, free                                */
#include <math.h>    /* sqrt e pow (precisa de -lm ao compilar)             */
#include <conio.h>   /* getche, getch (estilo do professor, Windows/MinGW)  */

/* ---- Constantes ---- */
#define MAX_N 100  /* tamanho maximo do vetor. Limita a entrada do usuario. */

/* =====================================================================
 * typedef struct para agrupar todos os resultados estatisticos.
 * Assim a funcao que calcula tudo pode devolver um unico "pacote"
 * em vez de usar varios ponteiros de saida. Mostra uso de struct + alias.
 * ---------------------------------------------------------------------
 * O alias "Resultados" (sem a palavra struct) so existe POR CAUSA do
 * typedef. Sem typedef teriamos que escrever "struct Resultados" sempre.
 * ===================================================================== */
typedef struct {
    float media;        /* media aritmetica simples                       */
    float mediana;      /* valor central apos ordenacao                  */
    float moda;         /* valor que mais aparece                        */
    float variancia;    /* variancia amostral (divide por N-1)           */
    float desvioPadrao; /* raiz da variancia                             */
    float minimo;       /* menor valor do vetor                          */
    float maximo;       /* maior valor do vetor                          */
} Resultados;           /* <-- alias: agora "Resultados" e um tipo valido */

/* =====================================================================
 * 1) lerValores: le N valores do teclado e guarda no vetor v[].
 *    Devolve a quantidade real de valores lidos (inteiro N).
 * ---------------------------------------------------------------------
 * Recebe o vetor por PONTEIRO (nome do vetor ja e endereco do 1o elemento).
 * Como so vamos LER dentro da funcao, nao precisamos devolver o vetor:
 * ele e modificado "in place" porque ponteiros compartilham memoria.
 * ===================================================================== */
int lerValores(float v[], int max)
{
    int n = 0, i;         /* n = quantidade (inicializada p/ evitar lixo)    */
    char entrada[64];      /* buffer para ler linha como texto (fgets)      */

    /* Pede a quantidade de valores. Repete se digitar fora do intervalo.   */
    do {
        printf("Quantos valores deseja digitar? (1 a %d): ", max);
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            /* fgets devolve NULL em EOF/erro de leitura: sai com 0.         */
            return 0;
        }
        /* Converte o texto lido para inteiro. sscanf ja ignora o \n.
         * Se a conversao falhar (texto nao numerico), sscanf retorna < 1,
         * entao forçamos n=0 para cair no teste de faixa e repetir a leitura. */
        if (sscanf(entrada, "%d", &n) != 1) {
            n = 0;
        }
        if (n < 1 || n > max) {
            printf("Valor invalido! Digite entre 1 e %d.\n", max);
        }
    } while (n < 1 || n > max);  /* repete enquanto estiver fora da faixa   */

    /* Le cada valor. Usamos fgets+sscanf para evitar problemas de buffer.  */
    for (i = 0; i < n; i++) {
        printf("  Valor %d: ", i + 1);
        /* Tenta ler ate conseguir uma linha valida. Se fgets devolver NULL
         * por EOF (fim de entrada/redirecionamento), nao adianta repetir:
         * o EOF nao se resolve sozinho. Nesse caso, abortamos a leitura
         * devolvendo apenas os valores lidos ate aqui (i).                   */
        while (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            /* EOF/erro persistente: nao tenta de novo infinitamente.       */
            return i;
        }
        /* sscanf converte o texto para float e guarda direto no vetor.
         * Se a conversao falhar (texto nao numerico), v[i] nao e modificado
         * e ficaria com lixo. Por isso checamos o retorno: se falhar,
         * repetimos a leitura deste mesmo indice sem avancar i.             */
        if (sscanf(entrada, "%f", &v[i]) != 1) {
            printf("  Valor invalido! Tente novamente.\n");
            i--;  /* refaz este indice no proximo passo do for              */
        }
    }

    return n;  /* devolve a quantidade lida para o chamador                  */
}

/* =====================================================================
 * 2) ordenarVetor: ordena o vetor em ordem CRESCENTE usando Bubble Sort.
 * ---------------------------------------------------------------------
 * Por que Bubble Sort? E simples de entender e nao precisa de biblioteca.
 * Compara pares vizinhos e troca se estiverem fora de ordem. Repete
 * ate que nenhuma troca seja feita em uma passada inteira.
 *
 * IMPORTANTE: recebe o vetor por ponteiro, entao modifica o ORIGINAL.
 * Por isso, no main() fazemos uma COPIA antes de ordenar para nao
 * perder a ordem original digitada pelo usuario.
 * ===================================================================== */
void ordenarVetor(float v[], int n)
{
    int i, j;            /* contadores de loop                              */
    float aux;           /* variavel auxiliar para a troca (swap)           */
    int trocou;          /* flag: 1 se houve troca nesta passada            */

    /* Loop externo: no pior caso precisa de n-1 passadas.                  */
    for (i = 0; i < n - 1; i++) {
        trocou = 0;  /* comeca cada passada assumindo que nao vai trocar    */

        /* Loop interno: compara vizinhos. Note o limite n-1-i: a cada
         * passada, o maior ja "borbulha" para o final, entao nao precisamos
         * revisitar as posicoes finais ja ordenadas.                        */
        for (j = 0; j < n - 1 - i; j++) {
            if (v[j] > v[j + 1]) {
                /* Troca os dois valores usando a variavel auxiliar.         */
                aux      = v[j];
                v[j]     = v[j + 1];
                v[j + 1] = aux;
                trocou   = 1;  /* marca que houve troca nesta passada       */
            }
        }

        /* Otimizacao: se uma passada inteira nao trocou nada, o vetor ja
         * esta ordenado. Saimos antes para economizar tempo.               */
        if (trocou == 0) {
            break;  /* sai do for externo                                   */
        }
    }
}

/* =====================================================================
 * 3) calcularMedia: soma todos os valores e divide por N.
 *    Formula: media = (v[0] + v[1] + ... + v[n-1]) / n
 * ===================================================================== */
float calcularMedia(float v[], int n)
{
    float soma = 0.0f;   /* acumulador. SEMPRE inicialize com 0!            */
    int i;

    for (i = 0; i < n; i++) {
        soma = soma + v[i];   /* acumula cada valor                         */
    }

    /* Atencao: se n for 0 teriamos divisao por zero. Como lerValores so
     * devolve n>=1, isso nao acontece neste programa. Mesmo assim, e uma
     * boa pratica defensiva verificar.                                      */
    if (n == 0) {
        return 0.0f;
    }

    return soma / (float)n;  /* cast para float: divisao real, nao inteira */
}

/* =====================================================================
 * 4) calcularMediana: valor do MEIO do vetor ordenado.
 * ---------------------------------------------------------------------
 * Regras:
 *  - Se N for IMPAR: mediana = v[N/2] (elemento central unico).
 *  - Se N for PAR:   mediana = media dos dois centrais:
 *                    (v[N/2 - 1] + v[N/2]) / 2.
 *
 * PRE-CONDICAO: o vetor recebido ja deve estar ORDENADO crescente.
 * Por isso o main() ordena uma copia antes de chamar esta funcao.
 * ===================================================================== */
float calcularMediana(float vOrdenado[], int n)
{
    if (n % 2 == 1) {
        /* Impar: pega o elemento do meio. Ex: N=5 -> indice 2 (5/2=2).     */
        return vOrdenado[n / 2];
    } else {
        /* Par: media dos dois do meio. Ex: N=4 -> indices 1 e 2.           */
        float esquerda  = vOrdenado[n / 2 - 1];
        float direita   = vOrdenado[n / 2];
        return (esquerda + direita) / 2.0f;
    }
}

/* =====================================================================
 * 5) calcularModa: valor que MAIS aparece no vetor.
 * ---------------------------------------------------------------------
 * Estrategia: para cada valor v[i], conta quantas vezes ele aparece
 * percorrendo todo o vetor. Guarda o valor com a maior frequencia.
 *
 * Observacoes:
 *  - Se houver empate (dois valores com a mesma frequencia maxima),
 *    esta versao simples devolve o PRIMEIRO que alcancou o maximo.
 *    Uma versao mais completa listaria todos os empatados.
 *  - Se todos os valores forem distintos, a "moda" sera o primeiro
 *    elemento (frequencia 1). Avisamos isso ao usuario no menu.
 * ===================================================================== */
float calcularModa(float v[], int n)
{
    int i, j;                 /* contadores de loop                          */
    int freqAtual;            /* frequencia do valor v[i] nesta iteracao    */
    int freqMaxima = 0;       /* maior frequencia encontrada ate agora      */
    float moda = v[0];        /* comeca assumindo que o primeiro e a moda   */

    /* Para cada elemento, conta quantas vezes ele aparece no vetor todo.   */
    for (i = 0; i < n; i++) {
        freqAtual = 0;  /* zera a contagem para este valor                  */

        for (j = 0; j < n; j++) {
            /* Compara com uma pequena tolerancia (epsilon) porque float pode
             * ter pequenas diferencas de arredondamento. Usar == direto e
             * frágil; fabs(v[j]-v[i]) < epsilon e a pratica recomendada.    */
            if (fabs(v[j] - v[i]) < 1e-6f) {
                freqAtual++;
            }
        }

        /* Se este valor apareceu mais vezes que o maximo atual, atualiza.  */
        if (freqAtual > freqMaxima) {
            freqMaxima = freqAtual;
            moda       = v[i];
        }
    }

    return moda;
}

/* =====================================================================
 * 6) calcularVariancia: medida de dispersao dos dados.
 * ---------------------------------------------------------------------
 * Variancia AMOSTRAL (divide por N-1):
 *   var = soma( (v[i] - media)^2 ) / (n - 1)
 *
 * Por que N-1 e nao N? Quando trabalhamos com uma AMOSTRA (e nao a
 * populacao inteira), dividir por N-1 da uma estimativa nao viesada.
 * Se fosse populacao inteira, dividiriamos por N. Em provas, leia bem
 * o enunciado para saber qual usar.
 *
 * Precisa da media ja calculada para nao recalcular aqui.
 * ===================================================================== */
float calcularVariancia(float v[], int n, float media)
{
    float soma = 0.0f;   /* acumulador das diferencas ao quadrado          */
    int i;

    for (i = 0; i < n; i++) {
        /* Diferenca entre o valor e a media, elevada ao quadrado.
         * pow(x, 2) eleva x a 2. Poderiamos usar (v[i]-media)*(v[i]-media)
         * que e mais rapido, mas pow deixa a intencao mais clara.          */
        float diff = v[i] - media;
        soma = soma + pow(diff, 2);
    }

    /* Divisao por n-1 (variancia amostral). Cuidado com n==1: n-1=0!        */
    if (n <= 1) {
        return 0.0f;  /* com 1 so valor nao ha dispersao                    */
    }

    return soma / (float)(n - 1);
}

/* =====================================================================
 * 7) calcularDesvioPadrao: raiz quadrada da variancia.
 *    Formula: dp = sqrt(variancia)
 * ---------------------------------------------------------------------
 * O desvio padrao tem a MESMA UNIDADE dos dados originais (a variancia
 * fica em unidades ao quadrado, por isso tiramos a raiz).
 * ===================================================================== */
float calcularDesvioPadrao(float variancia)
{
    /* sqrt vem da math.h. Precisa de -lm ao compilar.                       */
    return (float)sqrt((double)variancia);
    /* Cast para double na entrada do sqrt para evitar perda de precisao,
     * e cast de volta para float na saida para manter o tipo consistente.  */
}

/* =====================================================================
 * 8) calcularMinimo: percorre o vetor e acha o MENOR valor.
 * ---------------------------------------------------------------------
 * Inicializamos com o primeiro elemento (v[0]) e comparamos com os
 * demais. Se achar algum menor, atualiza.
 * ===================================================================== */
float calcularMinimo(float v[], int n)
{
    float min = v[0];  /* comeca assumindo que o primeiro e o menor        */
    int i;

    for (i = 1; i < n; i++) {  /* comeca do 1 pois v[0] ja e o min atual   */
        if (v[i] < min) {
            min = v[i];  /* achou um menor: atualiza                        */
        }
    }

    return min;
}

/* =====================================================================
 * 9) calcularMaximo: percorre o vetor e acha o MAIOR valor.
 *    Logica espelhada da funcao de minimo.
 * ===================================================================== */
float calcularMaximo(float v[], int n)
{
    float max = v[0];  /* comeca assumindo que o primeiro e o maior        */
    int i;

    for (i = 1; i < n; i++) {
        if (v[i] > max) {
            max = v[i];  /* achou um maior: atualiza                        */
        }
    }

    return max;
}

/* =====================================================================
 * 10) calcularTudo: chama todas as funcoes e preenche a struct Resultados.
 * ---------------------------------------------------------------------
 * Mostra como uma struct agrupa varios resultados relacionados.
 * Recebe o vetor ORIGINAL (nao ordenado) e a quantidade n.
 * Internamente cria uma COPIA ordenada para a mediana, sem destruir
 * a ordem original (boa pratica: nao surpreenda o usuario reordenando
 * os dados dele sem avisar).
 * ===================================================================== */
Resultados calcularTudo(float v[], int n)
{
    Resultados r;          /* variavel do tipo struct (ja com alias)        */
    float copia[MAX_N];   /* copia do vetor para ordenar sem mexer no original */
    int i;

    /* Copia o vetor original para a copia. Nao existe strcpy para float,
     * entao copiamos elemento por elemento com um loop.                    */
    for (i = 0; i < n; i++) {
        copia[i] = v[i];
    }

    /* Ordena a COPIA, nao o original. Assim o vetor original do main()
     * continua na ordem em que o usuario digitou.                           */
    ordenarVetor(copia, n);

    /* Preenche cada campo da struct chamando a funcao correspondente.       */
    r.media        = calcularMedia(v, n);
    r.mediana      = calcularMediana(copia, n);   /* usa copia ordenada     */
    r.moda         = calcularModa(v, n);
    r.variancia    = calcularVariancia(v, n, r.media);
    r.desvioPadrao = calcularDesvioPadrao(r.variancia);
    r.minimo       = calcularMinimo(v, n);
    r.maximo       = calcularMaximo(v, n);

    return r;  /* devolve a struct inteira preenchida                        */
}

/* =====================================================================
 * 11) mostrarResultados: imprime todos os campos da struct Resultados.
 *     Separada da logica de calculo para deixar o codigo mais limpo
 *     (principio de modularizacao: cada funcao faz uma coisa).
 * ===================================================================== */
void mostrarResultados(Resultados r, int n)
{
    printf("\n========== RESULTADOS ESTATISTICOS ==========\n");
    printf("Quantidade de valores (N) : %d\n", n);
    printf("Media aritmetica          : %.4f\n", r.media);
    printf("Mediana                   : %.4f\n", r.mediana);
    printf("Moda                      : %.4f\n", r.moda);
    printf("Variancia (amostral, N-1) : %.4f\n", r.variancia);
    printf("Desvio padrao             : %.4f\n", r.desvioPadrao);
    printf("Valor minimo              : %.4f\n", r.minimo);
    printf("Valor maximo              : %.4f\n", r.maximo);
    printf("=============================================\n");
}

/* =====================================================================
 * 12) mostrarVetor: imprime o vetor na tela (util para conferir a entrada).
 * ===================================================================== */
void mostrarVetor(float v[], int n)
{
    int i;
    printf("\nValores digitados: [ ");
    for (i = 0; i < n; i++) {
        printf("%.2f", v[i]);
        if (i < n - 1) {
            printf(", ");  /* virgula entre os valores, menos no ultimo    */
        }
    }
    printf(" ]\n");
}

/* =====================================================================
 * 13) limparTela: atalho para system("cls"). Encapsulado para reaproveitar.
 * ===================================================================== */
void limparTela(void)
{
    system("cls");  /* limpa a tela no Windows (cmd). No Linux seria "clear" */
}

/* =====================================================================
 * 14) pausar: atalho para system("pause"). Espera o usuario teclar algo.
 * ===================================================================== */
void pausar(void)
{
    system("pause");  /* mostra "Pressione qualquer tecla..." no Windows     */
}

/* =====================================================================
 * FUNCAO PRINCIPAL (main)
 * ---------------------------------------------------------------------
 * Estrutura classica do professor: menu do-while + switch.
 * O do-while garante que o menu aparece PELO MENOS uma vez.
 * O switch decide o que fazer conforme a opcao escolhida.
 * ===================================================================== */
int main(void)
{
    float valores[MAX_N];  /* vetor principal com os dados do usuario      */
    int   n = 0;           /* quantidade de valores efetivamente lidos     */
    int   jaLeu = 0;       /* flag: 1 se o usuario ja digitou os valores   */
    char  opcao;           /* opcao do menu (lida com getche)              */
    Resultados res;        /* struct para guardar todos os resultados       */

    do {
        limparTela();

        /* Cabecalho do menu                                                  */
        printf("=============================================\n");
        printf("   CALCULOS ESTATISTICOS EM VETOR (GB)       \n");
        printf("=============================================\n");
        printf("  1) Digitar os valores\n");
        printf("  2) Ver valores digitados\n");
        printf("  3) Calcular e mostrar TODAS as estatisticas\n");
        printf("  4) Mostrar apenas a MEDIA\n");
        printf("  5) Mostrar apenas a MEDIANA\n");
        printf("  6) Mostrar apenas a MODA\n");
        printf("  7) Mostrar MINIMO e MAXIMO\n");
        printf("  0) Sair\n");
        printf("=============================================\n");
        printf("Escolha uma opcao: ");

        /* getche le um caractere e ja mostra na tela (eco). Nao precisa Enter.
         * Retorna int, por isso o cast para char.                              */
        opcao = (char)getche();
        printf("\n");

        switch (opcao) {

        case '1':
            /* Le os valores do usuario. Se ja tinha lido antes, sobrescreve. */
            n = lerValores(valores, MAX_N);
            jaLeu = 1;  /* marca que agora temos dados validos               */
            printf("\nValores armazenados com sucesso!\n");
            pausar();
            break;

        case '2':
            /* Antes de mostrar, confere se o usuario ja digitou algo.        */
            if (jaLeu == 0) {
                printf("Voce ainda nao digitou valores! Use a opcao 1.\n");
            } else {
                mostrarVetor(valores, n);
            }
            pausar();
            break;

        case '3':
            /* Calcula tudo de uma vez e mostra.                              */
            if (jaLeu == 0) {
                printf("Voce ainda nao digitou valores! Use a opcao 1.\n");
            } else {
                res = calcularTudo(valores, n);
                mostrarResultados(res, n);
            }
            pausar();
            break;

        case '4':
            /* So a media. Calcula sob demanda para nao desperdicar se o
             * usuario so quer esse numero.                                   */
            if (jaLeu == 0) {
                printf("Voce ainda nao digitou valores! Use a opcao 1.\n");
            } else {
                float m = calcularMedia(valores, n);
                printf("\nMedia aritmetica = %.4f\n", m);
            }
            pausar();
            break;

        case '5':
            /* So a mediana. Precisa ordenar uma copia primeiro.              */
            if (jaLeu == 0) {
                printf("Voce ainda nao digitou valores! Use a opcao 1.\n");
            } else {
                float copia[MAX_N];
                int i;
                float med;  /* declarada no inicio do bloco (C89)            */
                /* Copia o vetor original para nao destruir a ordem digitada. */
                for (i = 0; i < n; i++) {
                    copia[i] = valores[i];
                }
                ordenarVetor(copia, n);
                med = calcularMediana(copia, n);
                printf("\nMediana = %.4f\n", med);
            }
            pausar();
            break;

        case '6':
            /* So a moda.                                                    */
            if (jaLeu == 0) {
                printf("Voce ainda nao digitou valores! Use a opcao 1.\n");
            } else {
                float moda = calcularModa(valores, n);
                printf("\nModa = %.4f\n", moda);
                printf("(Se todos os valores forem diferentes, a moda\n");
                printf(" sera apenas o primeiro valor digitado.)\n");
            }
            pausar();
            break;

        case '7':
            /* Minimo e maximo juntos.                                      */
            if (jaLeu == 0) {
                printf("Voce ainda nao digitou valores! Use a opcao 1.\n");
            } else {
                float mn = calcularMinimo(valores, n);
                float mx = calcularMaximo(valores, n);
                printf("\nMinimo = %.4f\n", mn);
                printf("Maximo = %.4f\n", mx);
            }
            pausar();
            break;

        case '0':
            printf("Encerrando o programa. Ate logo!\n");
            break;

        default:
            /* Qualquer outra tecla: opcao invalida.                         */
            printf("Opcao invalida! Escolha entre 0 e 7.\n");
            pausar();
            break;
        }

    } while (opcao != '0');  /* repete o menu ate digitar 0 (sair)          */

    return 0;  /* fim normal do programa                                     */
}
