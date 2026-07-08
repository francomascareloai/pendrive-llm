/*
=====================================================================
   31_recursao_varios.c
---------------------------------------------------------------------
   O QUE FAZ: Demonstra RECURSAO em C com 8 exemplos classicos:
     1) Fatorial
     2) Fibonacci
     3) Somatorio de 1 a N
     4) Potencia (x elevado a n)
     5) Imprimir vetor recursivamente
     6) Inverter string recursivamente
     7) Torre de Hanoi (simples)
     8) MDC pelo algoritmo de Euclides
   Cada funcao tem o CASO BASE e o CASO RECURSIVO comentados.
   Menu do-while + switch no estilo do professor.
---------------------------------------------------------------------
   TOPICO: Recursao (funcoes que chamam a si mesmas)
   PALAVRAS-CHAVE Ctrl-F: recursao, fatorial, fibonacci, somatorio,
     potencia, imprimir vetor recursivo, inverter string recursivo,
     torre de hanoi, mdc, euclides, caso base, caso recursivo,
     pilha de chamadas, stack, conio.h, menu do-while switch.
=====================================================================
*/

/* Inclui bibliotecas padrao de I/O e a conio.h do professor */
#include <stdio.h>   /* printf, scanf, fgets, puts  */
#include <string.h>  /* strlen, strcspn            */
#include <conio.h>   /* getche, getch (estilo professor) */
#include <stdlib.h>  /* system, atoi                  */

/* -----------------------------------------------------------------
   PROTOTIPOS de todas as funcoes recursivas e auxiliares.
   Declarar antes do main() e boa pratica: o compilador conhece
   as assinaturas e nao reclama com -Wall -Wextra.
   ----------------------------------------------------------------- */
long fatorial(int n);                 /* 1) Fatorial recursivo        */
long fibonacci(int n);                /* 2) Fibonacci recursivo       */
long somatorio(int n);               /* 3) Somatorio 1..N recursivo  */
double potencia(double base, int exp);/* 4) Potencia recursiva        */
void imprimirVetor(int v[], int tam, int pos); /* 5) Vetor recursivo */
void inverterString(char str[], int ini, int fim); /* 6) Inverte str */
void hanoi(int n, char origem, char aux, char destino); /* 7) Hanoi  */
int mdc(int a, int b);               /* 8) MDC de Euclides recursivo */

/* Funcoes de menu / utilidades */
void limparBuffer(void);
void cabecalho(void);

/*
=====================================================================
   FUNCAO 1: FATORIAL RECURSIVO
---------------------------------------------------------------------
   Definicao matematica:
       0! = 1                      <- CASO BASE
       n! = n * (n-1)!  para n>0   <- CASO RECURSIVO
   A funcao chama a si mesma com n-1 ate chegar em 0 (caso base).
   Sem o caso base a recursao seria infinita (estouraria a pilha).
=====================================================================
*/
long fatorial(int n)
{
    /* CASO BASE: se n e 0 ou 1, o fatorial e 1.
       Isso PARA a recursao. Sem isso, a funcao se chamaria
       indefinidamente ate dar "stack overflow". */
    if (n <= 1) {
        return 1L;  /* 1L = literal long, evita warning de int p/ long */
    }

    /* CASO RECURSIVO: n! = n * (n-1)!
       A funcao chama a SI MESMA com n-1.
       Cada chamada empilha um novo "n" na pilha de chamadas.
       Quando o caso base e atingido, a pilha comeca a "desempilhar". */
    return (long)n * fatorial(n - 1);
}

/*
=====================================================================
   FUNCAO 2: FIBONACCI RECURSIVO
---------------------------------------------------------------------
   Sequencia: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...
   Definicao recursiva:
       fib(0) = 0                       <- CASO BASE 1
       fib(1) = 1                       <- CASO BASE 2
       fib(n) = fib(n-1) + fib(n-2)     <- CASO RECURSIVO
   OBS: a versao recursiva "pura" e didatica mas LENTA para n grande,
   pois recalcula os mesmos valores muitas vezes (complexidade
   exponencial ~O(1.6^n)). Para n>40 ja fica visivelmente lenta.
=====================================================================
*/
long fibonacci(int n)
{
    /* CASOS BASE: os dois primeiros termos da sequencia.
       Sem esses dois casos base a recursao nao teria onde parar. */
    if (n == 0) return 0L;   /* 1o termo */
    if (n == 1) return 1L;   /* 2o termo */

    /* CASO RECURSIVO: cada termo e a soma dos dois anteriores.
       Note que faz DUAS chamadas recursivas por execucao,
       o que cria uma "arvore" de chamadas (por isso e lento). */
    return fibonacci(n - 1) + fibonacci(n - 2);
}

/*
=====================================================================
   FUNCAO 3: SOMATORIO DE 1 ATE N (RECURSIVO)
---------------------------------------------------------------------
   somatorio(1) = 1                 <- CASO BASE
   somatorio(n) = n + somatorio(n-1) <- CASO RECURSIVO
   Exemplo: somatorio(4) = 4+3+2+1 = 10
=====================================================================
*/
long somatorio(int n)
{
    /* CASO BASE: somar de 1 ate 1 da 1.
       Tambem protege contra n<=0 retornando 0. */
    if (n <= 1) {
        return (n == 1) ? 1L : 0L;  /* operador ternario: se n==1 ->1, senao 0 */
    }

    /* CASO RECURSIVO: n + (soma de 1 ate n-1) */
    return (long)n + somatorio(n - 1);
}

/*
=====================================================================
   FUNCAO 4: POTENCIA RECURSIVA (x^n)
---------------------------------------------------------------------
   x^0 = 1                          <- CASO BASE (qualquer coisa^0 = 1)
   x^n = x * x^(n-1)  para n>0      <- CASO RECURSIVO
   Retorna double para suportar bases fracionarias.
=====================================================================
*/
double potencia(double base, int exp)
{
    /* CASO BASE: qualquer numero elevado a 0 e 1.
       Encerra a recursao. */
    if (exp == 0) {
        return 1.0;
    }

    /* CASO RECURSIVO: x^n = x * x^(n-1) */
    return base * potencia(base, exp - 1);
}

/*
=====================================================================
   FUNCAO 5: IMPRIMIR VETOR RECURSIVAMENTE
---------------------------------------------------------------------
   Recebe o vetor, o tamanho total e a posicao atual.
   Imprime v[pos] e chama a si mesma para pos+1 ate pos == tam.
   CASO BASE: pos == tam (chegou ao fim do vetor) -> para.
   CASO RECURSIVO: imprime v[pos] e chama para pos+1.
   Mostra como passar um indice como parametro "progressivo".
=====================================================================
*/
void imprimirVetor(int v[], int tam, int pos)
{
    /* CASO BASE: se a posicao atual chegou ao tamanho do vetor,
       ja percorremos tudo. Encerra a recursao. */
    if (pos >= tam) {
        return;  /* return sem valor em funcao void = apenas retorna */
    }

    /* Imprime o elemento da posicao atual.
       %d inteiro, \t tab para separar visualmente. */
    printf("%d\t", v[pos]);

    /* CASO RECURSIVO: avanca para a proxima posicao (pos+1).
       A chamada recursiva "anda" pelo vetor. */
    imprimirVetor(v, tam, pos + 1);
}

/*
=====================================================================
   FUNCAO 6: INVERTER STRING RECURSIVAMENTE (in-place)
---------------------------------------------------------------------
   Troca str[ini] com str[fim] e chama para ini+1, fim-1.
   CASO BASE: ini >= fim (os indices se cruzaram) -> terminou.
   CASO RECURSIVO: troca os extremos e recursa para o "miolo".
   Exemplo: "roma" -> troca 'r'<->'a' -> "aomr" -> troca 'o'<->'m'
            -> "amor".
=====================================================================
*/
void inverterString(char str[], int ini, int fim)
{
    /* CASO BASE: quando o indice inicial alcancou ou ultrapassou
       o indice final, a string ja esta toda invertida. */
    if (ini >= fim) {
        return;  /* nada a fazer, encerra */
    }

    /* Troca os caracteres das posicoes ini e fim usando variavel aux.
       E a mesma logica da troca de dois valores com 3 copos. */
    char aux = str[ini];   /* guarda o caractere inicial   */
    str[ini] = str[fim];   /* coloca o final no inicio    */
    str[fim] = aux;        /* coloca o guardado no final  */

    /* CASO RECURSIVO: inverte o "miolo" da string,
       avancando ini e recuando fim (aproximam-se do centro). */
    inverterString(str, ini + 1, fim - 1);
}

/*
=====================================================================
   FUNCAO 7: TORRE DE HANOI (RECURSIVA)
---------------------------------------------------------------------
   Problema classico: mover N discos da torre ORIGEM para DESTINO,
   usando AUX como torre auxiliar, seguindo as regras:
     - mover 1 disco por vez;
     - nunca colocar disco maior sobre menor.
   CASO BASE: 1 disco -> basta mover direto de origem para destino.
   CASO RECURSIVO (n discos):
     1) mover n-1 discos de ORIGEM para AUX (usando DESTINO como aux)
     2) mover o disco maior de ORIGEM para DESTINO
     3) mover n-1 discos de AUX para DESTINO (usando ORIGEM como aux)
=====================================================================
*/
void hanoi(int n, char origem, char aux, char destino)
{
    /* CASO BASE: so ha 1 disco para mover.
       Basta mover da origem para o destino diretamente. */
    if (n == 1) {
        printf("Mover disco 1: %c -> %c\n", origem, destino);
        return;  /* encerra esta chamada recursiva */
    }

    /* PASSO 1: mover n-1 discos de ORIGEM para AUX.
       Note que DESTINO vira a torre auxiliar nesta chamada. */
    hanoi(n - 1, origem, destino, aux);

    /* PASSO 2: mover o disco maior (o n-esimo) de ORIGEM para DESTINO.
       Este disco fica "sozinho" embaixo apos o passo 1. */
    printf("Mover disco %d: %c -> %c\n", n, origem, destino);

    /* PASSO 3: mover os n-1 discos que estao em AUX para DESTINO.
       Agora ORIGEM e a torre auxiliar. */
    hanoi(n - 1, aux, origem, destino);
}

/*
=====================================================================
   FUNCAO 8: MDC PELO ALGORITMO DE EUCLIDES (RECURSIVO)
---------------------------------------------------------------------
   MDC(a, b) = maior divisor comum de a e b.
   Algoritmo de Euclides:
       MDC(a, 0) = a              <- CASO BASE
       MDC(a, b) = MDC(b, a%b)    <- CASO RECURSIVO
   Exemplo: MDC(48, 18)
       MDC(48,18) -> MDC(18, 48%18=12) -> MDC(12, 18%12=6)
       -> MDC(6, 12%6=0) -> CASO BASE: MDC=6
=====================================================================
*/
int mdc(int a, int b)
{
    /* CASO BASE: quando b chega a 0, o MDC e o valor de a.
       Isso acontece porque a%b vai diminuindo ate virar 0. */
    if (b == 0) {
        return a;
    }

    /* CASO RECURSIVO: MDC(a,b) = MDC(b, a%b).
       O operador % (modulo) da o resto da divisao de a por b.
       A cada chamada os numeros ficam menores, garantindo
       que b eventualmente chegue a 0 (caso base). */
    return mdc(b, a % b);
}

/*
=====================================================================
   FUNCAO AUXILIAR: limparBuffer
---------------------------------------------------------------------
   Le e descarta caracteres restantes no buffer do teclado.
   Necessario apos scanf para evitar que o proximo fgets/scanf
   leia o "enter" deixado para tras.
=====================================================================
*/
void limparBuffer(void)
{
    int c;
    /* Le caractere por caractere ate achar '\n' ou EOF (fim de arquivo).
       O while consome tudo que esta "sobrando" no buffer. */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* corpo vazio: so queremos descartar */
    }
}

/*
=====================================================================
   FUNCAO AUXILIAR: cabecalho
---------------------------------------------------------------------
   Limpa a tela e imprime o titulo do programa.
   Usa system("cls") no estilo do professor (Windows/MinGW).
=====================================================================
*/
void cabecalho(void)
{
    system("cls");  /* limpa a tela (Windows). Nao e portavel p/ Linux. */
    printf("=============================================\n");
    printf("   RECURSAO - 8 EXEMPLOS CLASSICOS EM C\n");
    printf("=============================================\n\n");
}

/*
=====================================================================
   FUNCAO PRINCIPAL: menu do-while + switch
---------------------------------------------------------------------
   Estilo do professor: menu em loop ate o usuario escolher sair.
=====================================================================
*/
int main(void)
{
    int opcao;          /* guarda a opcao escolhida no menu */
    int n;              /* valor inteiro generico digitado pelo usuario */
    int i;              /* contador para ler vetor */
    int vetor[100];     /* vetor para o exemplo 5 */
    int tam;            /* tamanho logico do vetor */
    char texto[256];    /* string para o exemplo 6 */
    double base;        /* base da potencia (exemplo 4) */
    int exp;            /* expoente da potencia (exemplo 4) */
    int a, b;           /* valores para MDC (exemplo 8) */

    /* do-while: repete o menu ate o usuario escolher 0 (sair) */
    do {
        cabecalho();   /* limpa tela e mostra titulo */

        /* Imprime as opcoes do menu */
        printf("1) Fatorial recursivo\n");
        printf("2) Fibonacci recursivo\n");
        printf("3) Somatorio 1..N recursivo\n");
        printf("4) Potencia recursiva (x^n)\n");
        printf("5) Imprimir vetor recursivo\n");
        printf("6) Inverter string recursivo\n");
        printf("7) Torre de Hanoi\n");
        printf("8) MDC (Euclides) recursivo\n");
        printf("0) Sair\n\n");
        printf("Escolha uma opcao: ");

        /* Le a opcao. %d le inteiro. */
        scanf("%d", &opcao);

        /* limparBuffer(): descarta o "enter" deixado pelo scanf.
           Usa getchar() em loop, que e PORTAVEL (funciona em
           Windows, Linux e Mac). Antes usava-se fflush(stdin),
           mas aquele tem comportamento indefinido fora do Windows
           (padrao C so define fflush para saida, nao para entrada). */
        limparBuffer();

        /* switch: executa o bloco conforme a opcao escolhida */
        switch (opcao) {

        /* ----------------------------------------------------- */
        case 1: /* FATORIAL */
            cabecalho();
            printf("FATORIAL RECURSIVO\n\n");
            printf("Digite um numero inteiro nao negativo: ");
            scanf("%d", &n);
            limparBuffer();  /* portavel: descarta o "enter" do scanf (getchar) */

            /* Protege contra entrada negativa (nao existe fatorial
               de numero negativo nos inteiros). */
            if (n < 0) {
                printf("\nErro: fatorial nao definido para negativos.\n");
            } else {
                /* Chama a funcao recursiva e imprime o resultado.
                   %ld = long decimal (fatorial cresce rapido). */
                printf("\n%d! = %ld\n", n, fatorial(n));
            }
            break;

        /* ----------------------------------------------------- */
        case 2: /* FIBONACCI */
            cabecalho();
            printf("FIBONACCI RECURSIVO\n\n");
            printf("Digite a posicao (n >= 0): ");
            scanf("%d", &n);
            limparBuffer();  /* portavel: descarta o "enter" do scanf (getchar) */

            if (n < 0) {
                printf("\nErro: n deve ser >= 0.\n");
            } else {
                /* Aviso de lentidao para n grande (recursao pura). */
                if (n > 40) {
                    printf("\nAviso: n>40 pode demorar (recursao pura e lenta).\n");
                }
                printf("\nFibonacci(%d) = %ld\n", n, fibonacci(n));
            }
            break;

        /* ----------------------------------------------------- */
        case 3: /* SOMATORIO */
            cabecalho();
            printf("SOMATORIO 1..N RECURSIVO\n\n");
            printf("Digite N (>= 1): ");
            scanf("%d", &n);
            limparBuffer();  /* portavel: descarta o "enter" do scanf (getchar) */

            if (n < 1) {
                printf("\nErro: N deve ser >= 1.\n");
            } else {
                printf("\nSomatorio de 1 ate %d = %ld\n", n, somatorio(n));
            }
            break;

        /* ----------------------------------------------------- */
        case 4: /* POTENCIA */
            cabecalho();
            printf("POTENCIA RECURSIVA (x^n)\n\n");
            printf("Digite a base (real): ");
            scanf("%lf", &base);   /* %lf le double */
            limparBuffer();  /* portavel: descarta o "enter" do scanf (getchar) */
            printf("Digite o expoente (inteiro >= 0): ");
            scanf("%d", &exp);
            limparBuffer();  /* portavel: descarta o "enter" do scanf (getchar) */

            if (exp < 0) {
                /* Esta versao simples so trata expoente >= 0.
                   Para expoente negativo usaria 1/potencia(base,-exp). */
                printf("\nErro: expoente deve ser >= 0 nesta versao.\n");
            } else {
                /* %lf imprime double com 6 casas por padrao. */
                printf("\n%.2lf ^ %d = %lf\n", base, exp, potencia(base, exp));
            }
            break;

        /* ----------------------------------------------------- */
        case 5: /* IMPRIMIR VETOR */
            cabecalho();
            printf("IMPRIMIR VETOR RECURSIVAMENTE\n\n");
            printf("Quantos elementos (max 100)? ");
            scanf("%d", &tam);
            limparBuffer();  /* portavel: descarta o "enter" do scanf (getchar) */

            if (tam < 1 || tam > 100) {
                printf("\nErro: tamanho invalido (1 a 100).\n");
            } else {
                /* Le cada elemento do vetor com um for.
                   O for e iterativo (nao recursivo) so para leitura;
                   a IMPRESSAO e que sera recursiva. */
                for (i = 0; i < tam; i++) {
                    printf("vetor[%d] = ", i);
                    scanf("%d", &vetor[i]);
                    limparBuffer();  /* portavel: descarta o "enter" do scanf (getchar) */
                }
                /* Chama a funcao recursiva a partir da posicao 0. */
                printf("\nVetor impresso recursivamente:\n");
                imprimirVetor(vetor, tam, 0);
                printf("\n");  /* quebra de linha ao final */
            }
            break;

        /* ----------------------------------------------------- */
        case 6: /* INVERTER STRING */
            cabecalho();
            printf("INVERTER STRING RECURSIVAMENTE\n\n");
            printf("Digite uma palavra/frase: ");

            /* fgets le a linha inteira (incluindo espaços) com seguranca.
               Diferente do gets() (perigoso e removido do padrao C11),
               fgets recebe o tamanho maximo para evitar estouro de buffer.
               Parametros: destino, tamanho maximo, origem (stdin=teclado). */
            fgets(texto, sizeof(texto), stdin);

            /* fgets inclui o '\n' no final da string. Removemos trocando
               por '\0' (fim de string). strcspn retorna o indice do 1o
               '\n' encontrado (ou o tamanho se nao houver). */
            texto[strcspn(texto, "\n")] = '\0';

            /* So inverte se a string tiver ao menos 2 caracteres.
               strlen retorna o tamanho (sem contar o '\0'). */
            if (strlen(texto) >= 2) {
                /* Chama a recursao: ini=0, fim=ultima posicao valida.
                   strlen(texto)-1 e o indice do ultimo caractere. */
                inverterString(texto, 0, (int)strlen(texto) - 1);
            }
            /* Imprime a string ja invertida. */
            printf("\nString invertida: %s\n", texto);
            break;

        /* ----------------------------------------------------- */
        case 7: /* TORRE DE HANOI */
            cabecalho();
            printf("TORRE DE HANOI (RECURSIVA)\n\n");
            printf("Numero de discos: ");
            scanf("%d", &n);
            limparBuffer();  /* portavel: descarta o "enter" do scanf (getchar) */

            if (n < 1) {
                printf("\nErro: deve ter pelo menos 1 disco.\n");
            } else {
                /* Torres: A (origem), B (auxiliar), C (destino).
                   O numero de movimentos e 2^n - 1 (cresce muito rapido). */
                printf("\nMovimentos para %d disco(s):\n", n);
                hanoi(n, 'A', 'B', 'C');
                printf("\nTotal de movimentos: %d (2^%d - 1)\n",
                       (1 << n) - 1, n);  /* (1<<n) = 2^n via shift de bits */
            }
            break;

        /* ----------------------------------------------------- */
        case 8: /* MDC */
            cabecalho();
            printf("MDC PELO ALGORITMO DE EUCLIDES (RECURSIVO)\n\n");
            printf("Digite o primeiro numero (a): ");
            scanf("%d", &a);
            limparBuffer();  /* portavel: descarta o "enter" do scanf (getchar) */
            printf("Digite o segundo numero (b): ");
            scanf("%d", &b);
            limparBuffer();  /* portavel: descarta o "enter" do scanf (getchar) */

            /* Garante valores positivos pegando o valor absoluto.
               O MDC e definido para inteiros positivos. */
            if (a < 0) a = -a;
            if (b < 0) b = -b;

            /* Se ambos forem zero, MDC nao e definido. */
            if (a == 0 && b == 0) {
                printf("\nErro: MDC(0,0) nao definido.\n");
            } else {
                printf("\nMDC(%d, %d) = %d\n", a, b, mdc(a, b));
            }
            break;

        /* ----------------------------------------------------- */
        case 0: /* SAIR */
            cabecalho();
            printf("Encerrando o programa. Ate logo!\n");
            break;

        /* ----------------------------------------------------- */
        default:
            /* Opcao invalida: qualquer numero fora de 0..8 cai aqui. */
            printf("\nOpcao invalida! Escolha entre 0 e 8.\n");
            break;

        } /* fim do switch */

        /* Pausa antes de voltar ao menu (estilo do professor).
           system("pause") mostra "Pressione qualquer tecla...". */
        if (opcao != 0) {
            printf("\n");
            system("pause");
        }

    } while (opcao != 0);  /* repete enquanto nao escolher 0 (sair) */

    return 0;  /* fim do programa, retorno 0 = sucesso */
}

/*
=====================================================================
   RESUMO DIDATICO - COMO PENSAR RECURSAO:
---------------------------------------------------------------------
   1) Toda funcao recursiva PRECISA de um CASO BASE (condicao de
      parada). Sem ele, a recursao e infinita e estoura a pilha
      (stack overflow).
   2) O CASO RECURSIVO deve SEMPRE se aproximar do caso base,
      ou seja, os parametros devem "convergir" para a condicao de
      parada (ex: n-1, fim-1, a%b que diminui).
   3) Cada chamada recursiva cria um NOVO quadro na PILHA DE
      CHAMADAS (stack) com suas proprias variaveis locais.
   4) Quando o caso base e atingido, a pilha comeca a DESEMPILHAR,
      retornando os valores calculados de baixo para cima.
   5) Recursao e elegante mas pode ser LENTA e gastar muita memoria.
      Sempre que possivel, versao iterativa (loop) e mais eficiente.
=====================================================================
*/
