/*
 ============================================================================
 ARQUIVO    : 30_ordenacao_busca_avancada.c
 DESCRICAO  : Implementa os 3 algoritmos de ordenacao simples (bubble sort,
              selection sort, insertion sort) e 2 algoritmos de busca
              (busca linear e busca binaria), todos com contador de
              comparacoes para analise de desempenho didatica.
              Inclui menu interativo no estilo do professor (conio.h,
              system cls/pause, do-while + switch).
 TOPICO     : Ordenacao e Busca Avancada
 PALAVRAS-CHAVE (Ctrl+F para encontrar secoes):
   bubble sort | selection sort | insertion sort | busca linear
   busca binaria | comparacoes | ordenacao | troca | swap | vetor
   menu | do-while | switch | conio | modularizacao | ponteiro
   complexidade | O(n^2) | O(log n)
 ============================================================================
 */

/* ---- BIBLIOTECAS ---- */
#include <stdio.h>   /* printf, scanf - entrada e saida padrao do C */
#include <stdlib.h>  /* system - para system("cls") e system("pause") */
#include <conio.h>   /* getch, getche - leitura de tecla individual (Windows/MinGW) */

/* Tamanho maximo do vetor.
   Usar #define facilita a manutencao: se precisar de mais espaco,
   basta alterar este valor em um unico lugar, sem mexer no codigo. */
#define TAM 100

/* -----------------------------------------------------------------------
   Variavel global: contador de comparacoes.
   Usamos uma global para nao poluir a assinatura das funcoes de
   ordenacao e busca, mantendo o foco no algoritmo em si.
   E resetada (zerada) antes de cada chamada de ordenacao ou busca.
   ----------------------------------------------------------------------- */
int comparacoes = 0;

/* =======================================================================
   PROTOTIPOS DAS FUNCOES
   Sao declarados antes do main() para que o compilador conheca as
   funcoes antes de elas serem chamadas. Em C, isso e obrigatorio se
   a funcao for definida apos o ponto onde e chamada.
   ======================================================================= */
void menu(void);
void lerVetor(int v[], int *n);
void mostrarVetor(int v[], int n);
void copiarVetor(int origem[], int destino[], int n);
void bubbleSort(int v[], int n);
void selectionSort(int v[], int n);
void insertionSort(int v[], int n);
int  buscaLinear(int v[], int n, int chave);
int  buscaBinaria(int v[], int n, int chave);
void compararOrdenacoes(int v[], int n);
void pausar(void);

/* =======================================================================
   FUNCAO: main
   Ponto de entrada do programa. Controla o menu principal com
   do-while + switch, no estilo do professor.
   ======================================================================= */
int main(void) {
    int vetor[TAM];   /* vetor original - preservado para nao perder a ordem digitada */
    int copia[TAM];   /* copia de trabalho - ordenacoes sao feitas aqui para preservar o original */
    int n = 0;        /* numero de elementos validos no vetor (0 = vazio) */
    int opcao;        /* opcao escolhida pelo usuario no menu */
    int chave;        /* valor a ser procurado nas buscas */
    int pos;          /* posicao encontrada pela busca (-1 = nao encontrado) */

    /* Loop do-while: executa o menu pelo menos uma vez e repete
       ate o usuario escolher a opcao 0 (sair). */
    do {
        menu();       /* exibe as opcoes do menu na tela (limpa com system("cls")) */

        /* fflush(stdin): limpa o buffer de entrada do teclado.
           ATENCAO: o padrao da linguagem C diz que fflush em stream
           de entrada e "comportamento indefinido" (undefined behavior).
           Porem, no Windows com MinGW (gcc), funciona na pratica e e
           amplamente usado em codigo didatico. Nao e portavel para Linux/Mac. */
        fflush(stdin);
        scanf("%d", &opcao);   /* le a opcao digitada pelo usuario */

        /* switch: direciona para a opcao escolhida.
           Cada case executa uma acao diferente. */
        switch(opcao) {

            case 1:
                /* Le o vetor do usuario.
                   Passa &n (endereco) para a funcao poder alterar
                   a quantidade de elementos lidos. */
                lerVetor(vetor, &n);
                break;

            case 2:
                /* Mostra o vetor atual na tela */
                if (n == 0) {
                    printf("\nVetor vazio! Leia o vetor primeiro (opcao 1).\n");
                } else {
                    printf("\nVetor atual (%d elementos):\n", n);
                    mostrarVetor(vetor, n);
                }
                break;

            case 3:
                /* BUBBLE SORT: ordena uma COPIA para nao perder o original.
                   Assim o usuario pode testar varios algoritmos sem
                   precisar redigitar os dados. */
                if (n == 0) {
                    printf("\nVetor vazio!\n");
                } else {
                    copiarVetor(vetor, copia, n);  /* faz copia de trabalho */
                    comparacoes = 0;                 /* zera contador antes de ordenar */
                    bubbleSort(copia, n);           /* ordena a copia */
                    printf("\n--- BUBBLE SORT ---\n");
                    printf("Comparacoes realizadas: %d\n", comparacoes);
                    printf("Vetor ordenado:\n");
                    mostrarVetor(copia, n);
                }
                break;

            case 4:
                /* SELECTION SORT */
                if (n == 0) {
                    printf("\nVetor vazio!\n");
                } else {
                    copiarVetor(vetor, copia, n);
                    comparacoes = 0;
                    selectionSort(copia, n);
                    printf("\n--- SELECTION SORT ---\n");
                    printf("Comparacoes realizadas: %d\n", comparacoes);
                    printf("Vetor ordenado:\n");
                    mostrarVetor(copia, n);
                }
                break;

            case 5:
                /* INSERTION SORT */
                if (n == 0) {
                    printf("\nVetor vazio!\n");
                } else {
                    copiarVetor(vetor, copia, n);
                    comparacoes = 0;
                    insertionSort(copia, n);
                    printf("\n--- INSERTION SORT ---\n");
                    printf("Comparacoes realizadas: %d\n", comparacoes);
                    printf("Vetor ordenado:\n");
                    mostrarVetor(copia, n);
                }
                break;

            case 6:
                /* COMPARAR OS 3: roda os 3 algoritmos em copias separadas
                   do mesmo vetor e mostra o numero de comparacoes de cada
                   um, lado a lado. Muito util para ver na pratica qual
                   faz menos trabalho. */
                if (n == 0) {
                    printf("\nVetor vazio!\n");
                } else {
                    compararOrdenacoes(vetor, n);
                }
                break;

            case 7:
                /* BUSCA LINEAR: funciona em vetor NAO ordenado.
                   Percorre do inicio ao fim, elemento por elemento. */
                if (n == 0) {
                    printf("\nVetor vazio!\n");
                } else {
                    printf("\nDigite o valor a procurar: ");
                    fflush(stdin);
                    scanf("%d", &chave);
                    comparacoes = 0;  /* zera antes de buscar */
                    pos = buscaLinear(vetor, n, chave);
                    printf("Comparacoes: %d\n", comparacoes);
                    if (pos != -1) {
                        printf("Valor %d encontrado na posicao %d.\n", chave, pos);
                    } else {
                        printf("Valor %d NAO encontrado.\n", chave);
                    }
                }
                break;

            case 8:
                /* BUSCA BINARIA: REQUER vetor ORDENADO!
                   Por isso, ordenamos uma copia antes de buscar.
                   A busca binaria so funciona em vetores ordenados;
                   se o vetor nao estiver ordenado, o resultado sera errado. */
                if (n == 0) {
                    printf("\nVetor vazio!\n");
                } else {
                    /* Primeiro: ordena uma copia do vetor */
                    copiarVetor(vetor, copia, n);
                    comparacoes = 0;
                    bubbleSort(copia, n);  /* ordena a copia */
                    printf("Vetor ordenado para a busca:\n");
                    mostrarVetor(copia, n);

                    /* Segundo: pede a chave e faz a busca binaria */
                    printf("\nDigite o valor a procurar: ");
                    fflush(stdin);
                    scanf("%d", &chave);

                    /* Zera o contador DE NOVO: as comparacoes da
                       ordenacao acima nao devem ser contadas aqui.
                       Queremos mostrar apenas as comparacoes da busca. */
                    comparacoes = 0;
                    pos = buscaBinaria(copia, n, chave);
                    printf("Comparacoes (apenas na busca): %d\n", comparacoes);
                    if (pos != -1) {
                        printf("Valor %d encontrado na posicao %d.\n", chave, pos);
                    } else {
                        printf("Valor %d NAO encontrado.\n", chave);
                    }
                }
                break;

            case 0:
                /* Sair do programa */
                printf("\nSaindo do programa... Ate logo!\n");
                break;

            default:
                /* Opcao invalida: o usuario digitou algo fora do range 0-8.
                   O default pega tudo que nao foi coberto pelos cases acima. */
                printf("\nOpcao invalida! Escolha entre 0 e 8.\n");
                break;
        }

        pausar();  /* system("pause") - espera o usuario pressionar uma tecla */

    } while (opcao != 0);  /* repete o menu ate o usuario escolher 0 (sair) */

    /* getch() (conio.h): espera o usuario pressionar qualquer tecla.
       Usado aqui para manter a janela do console aberta no final,
       comum em programas no Windows. */
    printf("\nPrograma finalizado. Pressione qualquer tecla para fechar...\n");
    getch();

    return 0;  /* retorno 0 indica ao sistema operacional que terminou OK */
}

/* =======================================================================
   FUNCAO: menu
   Exibe as opcoes do menu na tela.
   Limpa a tela antes com system("cls") (Windows).
   ======================================================================= */
void menu(void) {
    /* system("cls"): limpa a tela no Windows.
       Equivalente a system("clear") no Linux. */
    system("cls");
    printf("========================================\n");
    printf("   ORDENACAO E BUSCA - MENU PRINCIPAL\n");
    printf("========================================\n");
    printf("  1 - Ler vetor\n");
    printf("  2 - Mostrar vetor\n");
    printf("  3 - Bubble Sort\n");
    printf("  4 - Selection Sort\n");
    printf("  5 - Insertion Sort\n");
    printf("  6 - Comparar os 3 (comparacoes)\n");
    printf("  7 - Busca Linear\n");
    printf("  8 - Busca Binaria (ordena antes)\n");
    printf("  0 - Sair\n");
    printf("========================================\n");
    printf("Escolha uma opcao: ");
}

/* =======================================================================
   FUNCAO: lerVetor
   Le os elementos do vetor digitados pelo usuario.
   Recebe o vetor v[] (sempre passado por referencia em C) e o
   endereco de n (&n) para poder modificar a quantidade de elementos.
   ======================================================================= */
void lerVetor(int v[], int *n) {
    int i;  /* contador do loop for */

    printf("\nQuantos elementos deseja digitar (max %d)? ", TAM);
    fflush(stdin);
    scanf("%d", n);   /* le direto em n (n ja e ponteiro, nao precisa de &) */

    /* Validacao: se o usuario digitar valor negativo, ajusta para 0 */
    if (*n < 0) {
        *n = 0;
        printf("Valor invalido. Vetor vazio.\n");
        return;
    }
    /* Validacao: se passar do maximo, trunca para TAM */
    if (*n > TAM) {
        printf("Valor maior que o maximo (%d). Usando %d.\n", *n, TAM);
        *n = TAM;
    }

    /* Le cada elemento do vetor, um por vez */
    printf("Digite os %d elementos:\n", *n);
    for (i = 0; i < *n; i++) {
        printf("  v[%d] = ", i);
        fflush(stdin);
        scanf("%d", &v[i]);   /* le o elemento e armazena na posicao i */
    }
    printf("Vetor lido com sucesso!\n");
}

/* =======================================================================
   FUNCAO: mostrarVetor
   Exibe os elementos do vetor na tela, um por linha, com seu indice.
   ======================================================================= */
void mostrarVetor(int v[], int n) {
    int i;  /* contador do loop */

    for (i = 0; i < n; i++) {
        printf("  v[%d] = %d\n", i, v[i]);  /* mostra indice e valor */
    }
}

/* =======================================================================
   FUNCAO: copiarVetor
   Copia todos os elementos de 'origem' para 'destino'.
   Usamos copias para preservar o vetor original do usuario, permitindo
   testar varios algoritmos de ordenacao sem precisar redigitar os dados.
   ======================================================================= */
void copiarVetor(int origem[], int destino[], int n) {
    int i;  /* contador do loop */

    for (i = 0; i < n; i++) {
        destino[i] = origem[i];  /* copia elemento por elemento */
    }
}

/* =======================================================================
   FUNCAO: bubbleSort
   -----------------------------------------------------------------------
   Ordena o vetor em ordem CRESCENTE usando o metodo Bubble Sort
   (bolha). Compara pares adjacentes e troca se estiverem fora de ordem.
   A cada passada, o maior elemento "borbulha" para o final.

   COMPLEXIDADE:
     - Pior caso:   O(n^2)  (vetor em ordem inversa)
     - Melhor caso: O(n)    (vetor ja ordenado, com a otimizacao do flag)
     - Comparacoes no pior caso: n*(n-1)/2

   COMO FUNCIONA:
     - Loop externo (i): controla quantas passadas sao feitas.
       Apos a i-esima passada, os i maiores elementos ja estao no final.
     - Loop interno (j): percorre comparando pares adjacentes.
       Compara v[j] com v[j+1]; se v[j] > v[j+1], troca.
     - OTIMIZACAO: se uma passada inteira nao fez nenhuma troca,
       o vetor ja esta ordenado e podemos parar (flag 'trocou').
   ======================================================================= */
void bubbleSort(int v[], int n) {
    int i, j;       /* i = passada externa, j = posicao interna */
    int aux;        /* variavel auxiliar para a troca (swap) */
    int trocou;     /* flag: 1 se houve troca nesta passada, 0 se nao houve */

    /* Loop externo: faz no maximo n-1 passadas.
       Apos cada passada, o maior elemento restante vai para o final. */
    for (i = 0; i < n - 1; i++) {
        trocou = 0;  /* assume que nao havera troca nesta passada */

        /* Loop interno: compara elementos adjacentes.
           O limite diminui com i (n-1-i) porque os ultimos i elementos
           ja estao ordenados (os maiores ja "subiram" para o final). */
        for (j = 0; j < n - 1 - i; j++) {
            comparacoes++;  /* conta cada comparacao feita */

            if (v[j] > v[j + 1]) {
                /* TROCA (swap) dos dois elementos adjacentes.
                   Usamos 'aux' para guardar temporariamente o valor
                   de v[j], pois ele sera sobrescrito na linha seguinte. */
                aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
                trocou = 1;  /* marca que houve troca nesta passada */
            }
        }

        /* OTIMIZACAO: se nenhuma troca ocorreu nesta passada,
           o vetor ja esta totalmente ordenado. Podemos parar mais cedo.
           Isso faz o bubble sort ser O(n) no melhor caso
           (quando o vetor ja esta ordenado). */
        if (trocou == 0) {
            break;  /* sai do loop externo: vetor ja ordenado */
        }
    }
}

/* =======================================================================
   FUNCAO: selectionSort
   -----------------------------------------------------------------------
   Ordena o vetor em ordem CRESCENTE usando Selection Sort (selecao).
   A cada iteracao, encontra o MENOR elemento da parte nao ordenada
   e o coloca na posicao correta (troca com a posicao atual).

   COMPLEXIDADE:
     - Todos os casos: O(n^2)
     - Comparacoes: SEMPRE n*(n-1)/2, independente da ordem inicial.
       (O algoritmo sempre percorre todo o restante para achar o menor.)
     - Trocas: no maximo n-1 (uma por iteracao do loop externo).
       Isso e uma vantagem sobre o bubble sort, que faz mais trocas.

   COMO FUNCIONA:
     - Loop externo (i): marca a posicao que queremos preencher com
       o menor elemento restante.
     - Loop interno (j): procura o indice do menor elemento de i+1 ate n-1.
     - Apos achar o menor, troca com a posicao i.
   ======================================================================= */
void selectionSort(int v[], int n) {
    int i, j;       /* i = posicao a preencher, j = varredura para achar menor */
    int menor;      /* INDICE (nao o valor) do menor elemento encontrado */
    int aux;        /* variavel auxiliar para a troca */

    /* Loop externo: preenche posicao por posicao, da esquerda para direita.
       Vai ate n-2 (i < n-1) porque a ultima posicao nao precisa ser
       preenchida: quando todas as outras estao certas, a ultima
       automaticamente tambem esta. */
    for (i = 0; i < n - 1; i++) {
        menor = i;  /* assume que o menor esta na posicao i (candidato inicial) */

        /* Loop interno: procura o menor elemento no restante do vetor.
           Comeca em i+1 porque v[i] ja e o candidato atual a menor. */
        for (j = i + 1; j < n; j++) {
            comparacoes++;  /* conta cada comparacao */

            if (v[j] < v[menor]) {
                menor = j;  /* atualiza o indice do menor encontrado */
            }
        }

        /* Se encontrou um menor em posicao diferente de i, faz a troca.
           Se menor == i, o menor ja estava na posicao certa: nao troca.
           Note: mesmo se menor == i, as comparacoes ja foram contadas
           no loop interno. Por isso o selection sempre faz n*(n-1)/2. */
        if (menor != i) {
            aux = v[i];
            v[i] = v[menor];
            v[menor] = aux;
        }
    }
}

/* =======================================================================
   FUNCAO: insertionSort
   -----------------------------------------------------------------------
   Ordena o vetor em ordem CRESCENTE usando Insertion Sort (insercao).
   Funciona como organizar cartas na mao: pega cada carta e a insere
   na posicao correta entre as cartas ja ordenadas a sua esquerda.

   COMPLEXIDADE:
     - Pior caso:   O(n^2)  (vetor em ordem inversa/decrescente)
     - Melhor caso: O(n)    (vetor ja ordenado)
     - Comparacoes variam conforme a ordem inicial do vetor.

   COMO FUNCIONA:
     - O primeiro elemento (v[0]) ja e considerado "ordenado" (sozinho).
     - Para cada elemento a partir de v[1]:
       1. Guarda o elemento em 'chave' (tira do vetor temporariamente).
       2. Desloca todos os elementos maiores que 'chave' uma posicao
          para a direita (abrindo espaco).
       3. Insere 'chave' na posicao aberta.
   ======================================================================= */
void insertionSort(int v[], int n) {
    int i, j;       /* i = elemento a inserir, j = deslocamento */
    int chave;      /* elemento sendo inserido (guardado fora do vetor) */

    /* Comeca em i=1 porque v[0] sozinho ja esta "ordenado".
       Para cada elemento de v[1] ate v[n-1], insere-o na posicao correta. */
    for (i = 1; i < n; i++) {
        chave = v[i];  /* guarda o elemento a ser inserido */
        j = i - 1;     /* comeca a comparar com o elemento imediatamente a esquerda */

        /* Desloca elementos maiores que 'chave' uma posicao para a direita.
           O loop while continua enquanto:
           (a) j >= 0 (ainda ha elementos a esquerda para comparar)
           (b) v[j] > chave (o elemento e maior, precisa deslocar) */
        while (j >= 0) {
            comparacoes++;  /* conta cada comparacao de v[j] com chave */

            if (v[j] > chave) {
                v[j + 1] = v[j];  /* desloca v[j] uma posicao para a direita */
                j--;               /* move para o proximo elemento a esquerda */
            } else {
                /* v[j] <= chave: encontramos a posicao correta.
                   O elemento v[j] ja e menor ou igual, entao 'chave'
                   deve ir logo apos ele (na posicao j+1). Paramos. */
                break;
            }
        }

        /* Insere a chave na posicao aberta (j+1).
           Se j == -1 (chave e o menor de todos), j+1 == 0: vai para o inicio.
           Se o break ocorreu em j, j+1 e a posicao logo apos o ultimo
           elemento que era <= chave. */
        v[j + 1] = chave;
    }
}

/* =======================================================================
   FUNCAO: buscaLinear
   -----------------------------------------------------------------------
   Procura 'chave' no vetor percorrendo elemento por elemento,
   do inicio ao fim. Retorna o indice da primeira ocorrencia
   encontrada, ou -1 se nao encontrar.

   VANTAGEM: funciona em vetor NAO ordenado.
   DESVANTAGEM: lenta para vetores grandes.

   COMPLEXIDADE:
     - Pior caso:   O(n)  (chave nao existe ou esta no final)
     - Melhor caso: O(1)  (chave esta na primeira posicao)
     - Comparacoes no pior caso: n
   ======================================================================= */
int buscaLinear(int v[], int n, int chave) {
    int i;  /* contador do loop */

    /* Percorre o vetor do inicio (0) ao fim (n-1) */
    for (i = 0; i < n; i++) {
        comparacoes++;  /* conta cada comparacao */

        if (v[i] == chave) {
            return i;  /* encontrou! retorna o indice da posicao */
        }
    }
    /* Se chegou aqui, percorreu todo o vetor e nao encontrou a chave */
    return -1;  /* -1 indica "nao encontrado" (indice invalido) */
}

/* =======================================================================
   FUNCAO: buscaBinaria
   -----------------------------------------------------------------------
   Procura 'chave' em um vetor ORDENADO dividindo o espaco de busca
   pela metade a cada iteracao.

   PRE-REQUISITO: o vetor DEVE estar ordenado (ordem crescente).
   Se o vetor nao estiver ordenado, o resultado sera imprevisivel/errado.

   VANTAGEM: muito mais rapida que a linear para vetores grandes.
   DESVANTAGEM: so funciona em vetores ordenados.

   COMPLEXIDADE:
     - Pior e medio caso: O(log n)  (logaritmo na base 2)
     - Melhor caso: O(1)  (chave esta no meio na primeira tentativa)
     - Comparacoes no pior caso: aproximadamente log2(n)

   COMO FUNCIONA:
     - Mantem dois limites: esq (esquerda) e dir (direita) do intervalo.
     - Calcula meio = (esq + dir) / 2.
     - Se v[meio] == chave: encontrou!
     - Se v[meio] < chave: a chave esta na metade DIREITA.
       Move esq para meio+1 (descarta a metade esquerda).
     - Se v[meio] > chave: a chave esta na metade ESQUERDA.
       Move dir para meio-1 (descarta a metade direita).
     - Repete enquanto esq <= dir. Se esq > dir, o intervalo ficou
       vazio e a chave nao existe no vetor.
   ======================================================================= */
int buscaBinaria(int v[], int n, int chave) {
    int esq = 0;     /* limite esquerdo do intervalo de busca */
    int dir = n - 1; /* limite direito do intervalo de busca */
    int meio;        /* ponto central do intervalo atual */

    /* Continua enquanto o intervalo for valido (esq <= dir).
       Se esq > dir, o intervalo ficou vazio: a chave nao existe. */
    while (esq <= dir) {
        meio = (esq + dir) / 2;  /* calcula o indice do meio do intervalo */
        comparacoes++;           /* conta a comparacao com v[meio] */

        if (v[meio] == chave) {
            return meio;         /* encontrou! retorna a posicao */

        } else if (v[meio] < chave) {
            /* A chave e MAIOR que v[meio]: ela so pode estar na metade
               direita. Descartamos a metade esquerda movendo esq para
               meio+1 (meio ja foi verificado, nao precisa incluir). */
            esq = meio + 1;

        } else {
            /* A chave e MENOR que v[meio]: ela so pode estar na metade
               esquerda. Descartamos a metade direita movendo dir para
               meio-1 (meio ja foi verificado, nao precisa incluir). */
            dir = meio - 1;
        }
    }
    /* Se chegou aqui, o intervalo ficou vazio (esq > dir):
       a chave nao existe no vetor. */
    return -1;  /* -1 indica "nao encontrado" */
}

/* =======================================================================
   FUNCAO: compararOrdenacoes
   -----------------------------------------------------------------------
   Executa os 3 algoritmos de ordenacao em COPIAS separadas do mesmo
   vetor e exibe o numero de comparacoes de cada um, lado a lado.
   Isso permite ver na pratica qual algoritmo faz menos comparacoes
   para um dado vetor.

   OBSERVACOES DIDATICAS:
   - Bubble sort: comparacoes variam (para cedo se nao houver troca).
   - Selection sort: sempre faz n*(n-1)/2 comparacoes (numero fixo).
   - Insertion sort: comparacoes variam (depende da ordem inicial).
   ======================================================================= */
void compararOrdenacoes(int v[], int n) {
    int c1[TAM], c2[TAM], c3[TAM];  /* 3 copias independentes do vetor */
    int comp_bubble, comp_selection, comp_insertion;  /* totais de cada um */

    /* Faz 3 copias identicas do vetor original.
       Cada algoritmo vai ordenar a sua propria copia, de forma
       independente, sem afetar as outras. */
    copiarVetor(v, c1, n);
    copiarVetor(v, c2, n);
    copiarVetor(v, c3, n);

    /* --- Bubble Sort --- */
    comparacoes = 0;        /* zera antes de cada ordenacao */
    bubbleSort(c1, n);
    comp_bubble = comparacoes;  /* salva o total de comparacoes do bubble */

    /* --- Selection Sort --- */
    comparacoes = 0;
    selectionSort(c2, n);
    comp_selection = comparacoes;

    /* --- Insertion Sort --- */
    comparacoes = 0;
    insertionSort(c3, n);
    comp_insertion = comparacoes;

    /* Exibe o resultado comparativo em formato de tabela */
    printf("\n============================================\n");
    printf("   COMPARATIVO DE ORDENACAO (%d elementos)\n", n);
    printf("============================================\n");
    printf("  Bubble Sort    : %d comparacoes\n", comp_bubble);
    printf("  Selection Sort : %d comparacoes\n", comp_selection);
    printf("  Insertion Sort : %d comparacoes\n", comp_insertion);
    printf("============================================\n");

    /* Comentario didatico sobre o resultado esperado */
    printf("\nObservacao:\n");
    printf("  - Selection Sort SEMPRE faz n*(n-1)/2 = %d comparacoes.\n",
           n * (n - 1) / 2);
    printf("  - Bubble e Insertion variam conforme a ordem inicial do vetor.\n");
    printf("  - Se o vetor ja estava ordenado, Bubble e Insertion fazem menos.\n");
    printf("  - Se o vetor estava em ordem INVERSA, os 3 fazem quase o mesmo.\n");
}

/* =======================================================================
   FUNCAO: pausar
   Pausa a execucao ate o usuario pressionar uma tecla.
   Usa system("pause") que e especifico do Windows (exibe a mensagem
   "Pressione qualquer tecla para continuar...").
   ======================================================================= */
void pausar(void) {
    printf("\n");
    system("pause");  /* pausa no Windows - espera tecla do usuario */
}
