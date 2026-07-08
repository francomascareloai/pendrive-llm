/*
 ============================================================================
 ARQUIVO : 29_manipulacao_string_avancada.c
 DESCRICAO: Manipulacao avancada de strings em C puro. Reune varias funcoes
            uteis para prova: contar palavras, contar vogais/consoantes,
            inverter string, converter maiuscula/minuscula, remover espacos,
            verificar palindromo (ignorando espacos e maiusculas), contar
            ocorrencias de substring e substituir caractere. Tudo via menu
            interativo no estilo do professor.
 TOPICO   : Strings, vetores de char, ponteiros, funcoes, modularizacao,
            ctype.h, string.h
 PALAVRAS-CHAVE Ctrl-F: string, strlen, fgets, strcspn, strstr, toupper,
            tolower, isalpha, isspace, palindromo, inverter, vogais,
            consoantes, palavras, substring, substituir, remover espacos,
            menu, do-while, switch, conio, getche, getch, fflush stdin,
            ponteiro por referencia, in-place
 COMPILAR : gcc -Wall -Wextra 29_manipulacao_string_avancada.c -o string.exe
 ============================================================================
*/

#include <stdio.h>   /* printf, scanf, fgets, puts  */
#include <stdlib.h>  /* system                     */
#include <string.h>  /* strlen, strcspn, strstr   */
#include <ctype.h>   /* toupper, tolower, isalpha, isspace */
#include <conio.h>   /* getche, getch - especifico do Windows/MinGW */

/* Tamanho maximo das strings usadas no programa.
   Definir como constante facilita manutencao: se precisar maior,
   basta alterar aqui em um unico lugar. */
#define TAM 100

/* ----------------------------------------------------------------------- */
/* PROTOTIPOS DAS FUNCOES                                                  */
/* Declarar antes do main() para que o compilador conheca as assinaturas.  */
/* ----------------------------------------------------------------------- */
int  contarPalavras(const char *s);
void contarVogaisConsoantes(const char *s, int *vogais, int *consoantes);
void inverterString(char *s);
void paraMaiuscula(char *s);
void paraMinuscula(char *s);
void removerEspacos(char *s);
int  ehPalindromo(const char *s);
int  contarSubstring(const char *texto, const char *sub);
void substituirCaractere(char *s, char antigo, char novo);

/* ======================================================================= */
/* FUNCAO: contarPalavras                                                  */
/* Recebe uma string e retorna quantas palavras ela tem.                   */
/* Estrategia: usa uma flag "dentro" que indica se estamos dentro de uma   */
/* palavra. Toda vez que passamos de "fora" para "dentro", incrementamos.  */
/* isspace cobre espaco, tab, \n, etc.                                     */
/* ======================================================================= */
int contarPalavras(const char *s)
{
    int cont = 0;     /* contador de palavras            */
    int dentro = 0;   /* 0 = fora de palavra, 1 = dentro */
    int i;            /* indice do loop                   */

    /* Percorre a string ate o terminador \0 */
    for (i = 0; s[i] != '\0'; i++) {
        if (!isspace((unsigned char)s[i])) {
            /* Nao e espaco: estamos dentro de uma palavra */
            if (dentro == 0) {
                /* Estava fora, agora entrou: nova palavra! */
                cont++;
                dentro = 1;
            }
            /* Se ja estava dentro, nao faz nada (continua na mesma palavra) */
        } else {
            /* E espaco: sai da palavra */
            dentro = 0;
        }
    }
    return cont;
}

/* ======================================================================= */
/* FUNCAO: contarVogaisConsoantes                                          */
/* Recebe uma string e devolve, POR REFERENCIA, o numero de vogais e de   */
/* consoantes. Usamos ponteiros (int *) para que a funcao possa alterar    */
/* as variaveis do main() sem precisar de retorno.                         */
/* ======================================================================= */
void contarVogaisConsoantes(const char *s, int *vogais, int *consoantes)
{
    int v = 0;  /* contador local de vogais     */
    int c = 0;  /* contador local de consoantes */
    int i;      /* indice do loop               */
    char ch;    /* caractere convertido para minuscula */

    for (i = 0; s[i] != '\0'; i++) {
        /* isalpha retorna verdadeiro apenas para letras (A-Z, a-z).
           Cast para unsigned char evita comportamento indefinido quando
           o char e assinado e tem valor negativo (acentos, por exemplo). */
        if (isalpha((unsigned char)s[i])) {
            /* Converte para minuscula para simplificar a comparacao:
               so precisamos testar 'a','e','i','o','u'. */
            ch = (char)tolower((unsigned char)s[i]);
            if (ch == 'a' || ch == 'e' || ch == 'i' ||
                ch == 'o' || ch == 'u') {
                v++;  /* e vogal */
            } else {
                c++;  /* e letra mas nao e vogal -> consoante */
            }
        }
        /* Se nao for letra (numeros, espacos, pontuacao), ignora */
    }

    /* Devolve os resultados atraves dos ponteiros recebidos.
       *vogais = v significa: "o conteudo apontado por vogais recebe v". */
    *vogais = v;
    *consoantes = c;
}

/* ======================================================================= */
/* FUNCAO: inverterString                                                  */
/* Inverte a string IN-PLACE (modifica o proprio vetor, sem criar outro).  */
/* Estrategia: dois indices, um no inicio (i) e outro no fim (j).         */
/* Trocam-se os caracteres e os indices convergem para o centro.          */
/* ======================================================================= */
void inverterString(char *s)
{
    int n = (int)strlen(s);  /* comprimento da string        */
    int i, j;                /* i = esquerda, j = direita    */
    char tmp;                /* variavel temporaria p/ troca */

    /* i comeca em 0 (primeiro caractere), j em n-1 (ultimo caractere).
       O loop roda enquanto i < j, ou seja, ate se cruzarem no meio. */
    for (i = 0, j = n - 1; i < j; i++, j--) {
        tmp  = s[i];  /* guarda o caractere da esquerda  */
        s[i] = s[j];  /* esquerda recebe o da direita     */
        s[j] = tmp;   /* direita recebe o guardado        */
    }
    /* Ao final, a string esta invertida. Nao precisa de return. */
}

/* ======================================================================= */
/* FUNCAO: paraMaiuscula                                                   */
/* Converte todos os caracteres da string para maiusculo, in-place.        */
/* toupper converte apenas letras; outros caracteres nao sao alterados.   */
/* ======================================================================= */
void paraMaiuscula(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        /* Cast para unsigned char: boa pratica com funcoes de ctype.h
           para evitar comportamento indefinido com chars negativos. */
        s[i] = (char)toupper((unsigned char)s[i]);
    }
}

/* ======================================================================= */
/* FUNCAO: paraMinuscula                                                   */
/* Converte todos os caracteres da string para minusculo, in-place.        */
/* ======================================================================= */
void paraMinuscula(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
}

/* ======================================================================= */
/* FUNCAO: removerEspacos                                                  */
/* Remove TODOS os espacos da string, in-place.                            */
/* Estrategia: dois indices. i percorre tudo (leitura), j so avanca       */
/* quando copiamos um caractere que nao e espaco (escrita).               */
/* Isso "comprime" a string, pulando os espacos.                          */
/* ======================================================================= */
void removerEspacos(char *s)
{
    int i = 0;  /* indice de LEITURA  (percorre a string original) */
    int j = 0;  /* indice de ESCRITA  (monta a string sem espacos) */
    int n = (int)strlen(s);

    for (i = 0; i < n; i++) {
        if (s[i] != ' ') {
            /* Nao e espaco: copia para a posicao de escrita */
            s[j] = s[i];
            j++;  /* avanca apenas a escrita */
        }
        /* Se for espaco, j NAO avanca -> o espaco e descartado */
    }
    /* Fecha a string com \0 apos o ultimo caractere mantido.
       Isso e CRUCIAL: sem isso, o final antigo (com espacos) ficaria lixo. */
    s[j] = '\0';
}

/* ======================================================================= */
/* FUNCAO: ehPalindromo                                                    */
/* Verifica se a string e um palindromo, IGNORANDO espacos e diferencas   */
/* de maiuscula/minuscula.                                                 */
/* Ex: "A base do teto desaba" -> palindromo (letras: abasedotetodesaba)  */
/* Retorna 1 (verdadeiro) se for palindromo, 0 (falso) caso contrario.     */
/* ======================================================================= */
int ehPalindromo(const char *s)
{
    char limpa[TAM];  /* string so com letras, em minuscula */
    int i;            /* indice de leitura da string original */
    int j = 0;        /* indice de escrita da string limpa */
    int n;            /* comprimento da string limpa */

    /* Passo 1: construir uma versao "limpa" da string.
       Copiamos apenas letras (isalpha), convertidas para minuscula.
       Espacos, numeros e pontuacao sao ignorados. */
    for (i = 0; s[i] != '\0'; i++) {
        if (isalpha((unsigned char)s[i])) {
            limpa[j] = (char)tolower((unsigned char)s[i]);
            j++;
        }
    }
    limpa[j] = '\0';  /* fecha a string limpa */

    /* Passo 2: comparar os extremos em direcao ao centro.
       Se todas as comparacoes forem iguais, e palindromo. */
    n = j;  /* numero de letras na string limpa */
    for (i = 0, j = n - 1; i < j; i++, j--) {
        if (limpa[i] != limpa[j]) {
            return 0;  /* achou diferenca: NAO e palindromo */
        }
    }
    return 1;  /* todas as comparacoes bateram: E palindromo */
}

/* ======================================================================= */
/* FUNCAO: contarSubstring                                                 */
/* Conta quantas vezes a substring "sub" aparece dentro de "texto".       */
/* Usa strstr() em um loop: strstr retorna um ponteiro para a primeira     */
/* ocorrencia, ou NULL se nao encontrar. A cada encontro, avancamos o      */
/* ponteiro para continuar a busca a partir dali.                         */
/* ======================================================================= */
int contarSubstring(const char *texto, const char *sub)
{
    int cont = 0;              /* contador de ocorrencias */
    const char *p = texto;     /* ponteiro que percorre o texto */
    int lenSub = (int)strlen(sub);

    /* Se a substring e vazia, nao ha o que procurar. */
    if (lenSub == 0) {
        return 0;
    }

    /* strstr(p, sub) procura "sub" a partir da posicao apontada por p.
       Retorna ponteiro para a ocorrencia, ou NULL se nao achar. */
    while ((p = strstr(p, sub)) != NULL) {
        cont++;         /* achou uma ocorrencia! */
        p += lenSub;    /* avanca o ponteiro para depois da ocorrencia
                           encontrada, evitando contar a mesma posicao. */
    }
    return cont;
}

/* ======================================================================= */
/* FUNCAO: substituirCaractere                                             */
/* Substitui TODAS as ocorrencias do caractere "antigo" por "novo",       */
/* modificando a string in-place.                                          */
/* ======================================================================= */
void substituirCaractere(char *s, char antigo, char novo)
{
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == antigo) {
            s[i] = novo;  /* substitui este caractere */
        }
    }
}

/* ======================================================================= */
/* FUNCAO: main                                                            */
/* Menu interativo com do-while + switch, no estilo do professor.         */
/* ======================================================================= */
int main(void)
{
    char texto[TAM];        /* string principal manipulada pelo programa */
    char sub[TAM];          /* substring usada na opcao 9 */
    char c_antigo, c_novo;  /* caracteres para a opcao 10 (substituir) */
    int  opcao;             /* opcao escolhida pelo usuario no menu */
    int  vogais, consoantes; /* recebidos por referencia na opcao 3 */
    int  qtd;               /* resultado generico de contagens (opcoes 2 e 9) */

    /* Inicializa texto como string vazia.
       Sem isso, texto conteria lixo de memoria e as verificacoes
       de "texto vazio" nao funcionariam. */
    texto[0] = '\0';

    do {
        /* system("cls") limpa a tela no Windows.
           No Linux seria "clear". Usamos cls pois o professor usa Windows. */
        system("cls");

        /* Cabecalho do menu */
        puts("===== MANIPULACAO AVANCADA DE STRINGS =====");
        puts("1  - Digitar/alterar o texto");
        puts("2  - Contar palavras");
        puts("3  - Contar vogais e consoantes");
        puts("4  - Inverter string");
        puts("5  - Converter para MAIUSCULA");
        puts("6  - Converter para minuscula");
        puts("7  - Remover espacos");
        puts("8  - Verificar palindromo");
        puts("9  - Contar ocorrencias de substring");
        puts("10 - Substituir caractere");
        puts("0  - Sair");

        /* Mostra o texto atual para o usuario saber com o que esta trabalhando */
        printf("\nTexto atual: \"%s\"\n", texto);
        printf("Opcao: ");
        scanf("%d", &opcao);

        /* fflush(stdin): descarta o \n deixado pelo scanf no buffer de entrada.
           ATENCAO: fflush em fluxo de ENTRADA (stdin) NAO e portavel segundo
           o padrao C (o padrao so define fflush para fluxos de SAIDA).
           Porem, no Windows com MinGW, funciona perfeitamente.
           Alternativa portavel:  while (getchar() != '\n');
           O professor usa fflush(stdin), entao mantemos o estilo dele. */
        fflush(stdin);

        switch (opcao) {

            case 1:
                /* Opcao: digitar ou alterar o texto */
                printf("Digite o texto (ate %d caracteres): ", TAM - 1);
                /* fgets: forma SEGURA de ler strings.
                   Le no maximo TAM-1 caracteres (deixa espaco para o \0).
                   Diferente do gets() (perigoso e removido do padrao C11),
                   fgets respeita o limite do vetor, evitando buffer overflow. */
                fgets(texto, TAM, stdin);
                /* fgets inclui o \n no final se houver espaco no buffer.
                   strcspn(texto, "\n") retorna o indice da primeira ocorrencia
                   de \n. Substituimos por \0 para remove-lo da string. */
                texto[strcspn(texto, "\n")] = '\0';
                printf("Texto armazenado com sucesso!\n");
                break;

            case 2:
                /* Opcao: contar palavras */
                if (texto[0] == '\0') {
                    puts("Nenhum texto cadastrado. Use a opcao 1 primeiro.");
                    break;
                }
                qtd = contarPalavras(texto);
                printf("Numero de palavras: %d\n", qtd);
                break;

            case 3:
                /* Opcao: contar vogais e consoantes.
                   Passamos os ENDERECOS (&) de vogais e consoantes para que
                   a funcao possa modifica-los por referencia (ponteiros). */
                if (texto[0] == '\0') {
                    puts("Nenhum texto cadastrado. Use a opcao 1 primeiro.");
                    break;
                }
                contarVogaisConsoantes(texto, &vogais, &consoantes);
                printf("Vogais: %d\n", vogais);
                printf("Consoantes: %d\n", consoantes);
                break;

            case 4:
                /* Opcao: inverter a string (modifica o texto original!) */
                if (texto[0] == '\0') {
                    puts("Nenhum texto cadastrado. Use a opcao 1 primeiro.");
                    break;
                }
                inverterString(texto);
                printf("String invertida: \"%s\"\n", texto);
                break;

            case 5:
                /* Opcao: converter para maiuscula (modifica o texto!) */
                if (texto[0] == '\0') {
                    puts("Nenhum texto cadastrado. Use a opcao 1 primeiro.");
                    break;
                }
                paraMaiuscula(texto);
                printf("Em maiuscula: \"%s\"\n", texto);
                break;

            case 6:
                /* Opcao: converter para minuscula (modifica o texto!) */
                if (texto[0] == '\0') {
                    puts("Nenhum texto cadastrado. Use a opcao 1 primeiro.");
                    break;
                }
                paraMinuscula(texto);
                printf("Em minuscula: \"%s\"\n", texto);
                break;

            case 7:
                /* Opcao: remover todos os espacos (modifica o texto!) */
                if (texto[0] == '\0') {
                    puts("Nenhum texto cadastrado. Use a opcao 1 primeiro.");
                    break;
                }
                removerEspacos(texto);
                printf("Sem espacos: \"%s\"\n", texto);
                break;

            case 8:
                /* Opcao: verificar se e palindromo.
                   A funcao ignora espacos e maiusculas/minuscules,
                   entao "O lobo ama o bolo" sera detectado como palindromo. */
                if (texto[0] == '\0') {
                    puts("Nenhum texto cadastrado. Use a opcao 1 primeiro.");
                    break;
                }
                if (ehPalindromo(texto)) {
                    printf("\"%s\" E um palindromo!\n", texto);
                } else {
                    printf("\"%s\" NAO e um palindromo.\n", texto);
                }
                break;

            case 9:
                /* Opcao: contar quantas vezes uma substring aparece no texto */
                if (texto[0] == '\0') {
                    puts("Nenhum texto cadastrado. Use a opcao 1 primeiro.");
                    break;
                }
                printf("Digite a substring a procurar: ");
                fgets(sub, TAM, stdin);
                sub[strcspn(sub, "\n")] = '\0';  /* remove o \n */
                qtd = contarSubstring(texto, sub);
                printf("A substring \"%s\" aparece %d vez(es) no texto.\n",
                       sub, qtd);
                break;

            case 10:
                /* Opcao: substituir um caractere por outro no texto.
                   getche() le UM caractere do teclado COM eco (mostra na tela)
                   e nao precisa de Enter. Retorna o caractere como int. */
                if (texto[0] == '\0') {
                    puts("Nenhum texto cadastrado. Use a opcao 1 primeiro.");
                    break;
                }
                printf("Caractere a substituir: ");
                c_antigo = (char)getche();
                printf("\nNovo caractere: ");
                c_novo = (char)getche();
                /* Chama a funcao que percorre a string trocando todas as
                   ocorrencias de c_antigo por c_novo. */
                substituirCaractere(texto, c_antigo, c_novo);
                printf("\nResultado: \"%s\"\n", texto);
                break;

            case 0:
                /* Opcao: sair do programa */
                puts("Encerrando o programa. Ate logo!");
                break;

            default:
                /* Qualquer opcao fora do intervalo 0-10 */
                puts("Opcao invalida! Escolha um numero do menu.");
                break;
        }

        /* Pausa antes de voltar ao menu (exceto na opcao de sair).
           getch() le um caractere SEM eco e sem precisar de Enter.
           Ideal para "pressione qualquer tecla para continuar". */
        if (opcao != 0) {
            puts("\nPressione qualquer tecla para continuar...");
            getch();
        }

    } while (opcao != 0);  /* repete ate o usuario escolher 0 */

    return 0;  /* retorno 0 indica execucao bem-sucedida */
}
