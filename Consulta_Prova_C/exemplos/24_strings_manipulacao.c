/*
 ============================================================================
 ARQUIVO  : 24_strings_manipulacao.c
 OBJETIVO : Demonstrar as principais operacoes de manipulacao de strings em C.
            Material de CONSULTA para a prova do Grau B (Algoritmos II).
 TOPICO   : Strings (vetor de char terminado em '\0') + bibliotecas string.h e ctype.h.
 PALAVRAS-CHAVE (Ctrl+F):
    fgets            - leitura segura de string (substitui o perigoso gets)
    strcspn          - achar posicao de um caractere (usado para remover o \n)
    strlen           - tamanho da string (quantos chars validos antes do \0)
    strcpy           - copiar string de uma variavel para outra
    strcat           - concatenar (juntar) duas strings
    strcmp           - comparar duas strings (RETORNA 0 se iguais)
    PEGADINHA strcmp - if(strcmp(a,b)) vs if(strcmp(a,b)==0)  <-- cai na prova!
    toupper/tolower  - converter para maiuscula/minuscula
    isalpha/isdigit  - testar se caractere e letra ou digito
    vogais           - contar vogais manualmente (sem funcao pronta)
    inverter string  - inverter manualmente com dois ponteiros/indices
    strstr           - busca parcial (procurar substring dentro de string)
    strtok           - dividir string por um separador (ex: CSV por virgula)
    sprintf          - montar string formatada (printf para dentro de string)
    atoi/atof        - converter string para int / float
    menu switch      - estilo do professor com do-while + switch
    fflush(stdin)    - limpar buffer do teclado (estilo Windows/MinGW)
 ============================================================================
*/

/* ---- BIBLIOTECAS ---- */
#include <stdio.h>   /* fgets, printf, sprintf, puts, gets (NUNCA usar gets!) */
#include <stdlib.h>  /* atoi, atof, system */
#include <string.h>  /* strlen, strcpy, strcat, strcmp, strcspn, strstr, strtok */
#include <ctype.h>   /* toupper, tolower, isalpha, isdigit, isupper, islower */
#include <conio.h>   /* getche, getch (estilo do professor - so existe no Windows) */

/*
  NOTA IMPORTANTE sobre conio.h:
  A conio.h NAO e padrao ANSI C. So existe no Windows (MinGW/Dev-Cpp/Code::Blocks).
  Por isso o professor compila sem -std=c11 (que seria mais restritivo).
  Comando de compilacao que funciona:
      gcc -Wall -Wextra 24_strings_manipulacao.c -o programa
*/

/* =========================================================================
   PROTOTIPOS DAS FUNCOES (declaradas antes do main para o compilador conhecer)
   ========================================================================= */
void limpar_tela(void);
void pausar(void);
void limpar_buffer_teclado(void);

void demo_fgets_strcspn(void);
void demo_strlen_strcpy_strcat_strcmp(void);
void demo_ctype(void);
int  contar_vogais(const char s[]);
void demo_contar_vogais(void);
void inverter_string(char s[]);
void demo_inverter_string(void);
void demo_strstr(void);
void demo_strtok_csv(void);
void demo_sprintf(void);
void demo_atoi_atof(void);

/* =========================================================================
   FUNCOES AUXILIARES DE TELA (estilo do professor)
   ========================================================================= */

/* Limpa a tela do console. No Windows usa "cls", no Linux seria "clear". */
void limpar_tela(void) {
    system("cls");  /* system() executa um comando do sistema operacional */
}

/* Pausa a execucao ate o usuario pressionar uma tecla. */
void pausar(void) {
    printf("\nPressione qualquer tecla para continuar...");
    getch();  /* getch() le 1 caractere sem ecoar na tela e sem precisar Enter */
}

/*
   limpar_buffer_teclado:
   Limpa o que ficou "sobrando" no buffer do teclado (o '\n' do Enter).
   O professor usa fflush(stdin), mas isso NAO e garantido pelo padrao C
   (so fflush(stdout) e). Em Windows/MinGW funciona na pratica.
   Aqui mostramos a forma PORTAVEL (laco que descarta chars ate o \n)
   e deixamos o fflush(stdin) comentado como o estilo do professor.
*/
void limpar_buffer_teclado(void) {
    int c;
    /* fflush(stdin);  <-- estilo do professor (funciona no Windows, nao portavel) */
    /* Forma portavel e correta: ler e descartar ate achar \n ou EOF */
    while ((c = getchar()) != '\n' && c != EOF) {
        /* corpo vazio: so queremos descartar os caracteres */
    }
}

/* =========================================================================
   (1) fgets + strcspn para remover o '\n'
   ========================================================================= */
void demo_fgets_strcspn(void) {
    char nome[100];  /* vetor de 100 chars; posicao 99 reserva o '\0' final */

    printf("=== (1) fgets + strcspn (remover o \\n) ===\n\n");

    /*
       POR QUE fgets e NAO gets?
       gets(nome) nao recebe o tamanho do vetor, entao se o usuario digitar
       mais caracteres do que cabe, ele escreve ALEM do vetor -> estouro de buffer
       (buffer overflow), bug classico de seguranca. gets foi removido do C11.
       fgets ja recebe o tamanho maximo, entao NAO estoura o vetor.
    */
    printf("Digite seu nome completo: ");

    /*
       fgets(destino, tamanho, origem)
       - destino: onde guardar a string lida
       - tamanho: MAX de chars que cabem (incluindo o '\0')
       - origem: stdin = teclado
       O fgets SEMPRE guarda o '\n' do Enter na string (se houver espaco).
       Por isso precisamos remover o '\n' depois.
    */
    fgets(nome, sizeof(nome), stdin);

    /*
       strcspn(nome, "\n") retorna o INDICE do primeiro '\n' encontrado.
       Se a string for "Franco\n", strcspn retorna 6 (indice do '\n').
       Entao nome[6] = '\0' corta a string ali, removendo o '\n'.
       Truque classico de prova: nome[strcspn(nome,"\n")] = '\0';
       Se nao houver '\n' (usuario digitou exatamente ate o limite),
       strcspn retorna o indice do '\0' final, e nome[...] = '\0' nao faz mal.
    */
    nome[strcspn(nome, "\n")] = '\0';

    printf("Ola, [%s]! (o \\n foi removido)\n", nome);
    pausar();
}

/* =========================================================================
   (2) strlen, strcpy, strcat, strcmp (com a PEGADINHA do if)
   ========================================================================= */
void demo_strlen_strcpy_strcat_strcmp(void) {
    char origem[50] = "Franco";   /* ja inicializada com a string "Franco" */
    char destino[50];             /* vazia: contem lixo ate atribuirmos algo */
    char a[50] = "abc";
    char b[50] = "abc";
    char c[50] = "abd";

    printf("=== (2) strlen / strcpy / strcat / strcmp ===\n\n");

    /* --- strlen: tamanho da string (NAO conta o '\0') --- */
    /* strlen percorre ate achar '\0' e retorna quantos chars validos ha. */
    printf("strlen(\"%s\") = %zu\n", origem, strlen(origem));
    /* %zu e o especificador correto para size_t (tipo retornado por strlen).
       Em provas antigas o professor aceita %d, mas %zu e o correto. */

    /* --- strcpy: COPIAR string --- */
    /* strcpy(destino, origem): copia byte a byte INCLUSIVE o '\0'.
       NAO se faz destino = origem (isso so copia o endereco do ponteiro,
       nao o conteudo). Para copiar o CONTEUDO usa-se strcpy. */
    strcpy(destino, origem);
    printf("strcpy: destino agora = [%s]\n", destino);

    /* --- strcat: CONCATENAR (juntar) --- */
    /* strcat(destino, origem): cola a string origem no FIM de destino.
       destino precisa ter espaco suficiente senao estoura o vetor. */
    strcat(destino, " da Silva");  /* cola " da Silva" no fim de destino */
    printf("strcat: destino apos colar ' da Silva' = [%s]\n", destino);

    /* --- strcmp: COMPARAR --- */
    //  strcmp(s1, s2) retorna:
    //    < 0  se s1 < s2  (s1 vem antes no dicionario)
    //    == 0 se s1 == s2 (IGUAIS)
    //    > 0  se s1 > s2  (s1 vem depois no dicionario)
    //
    //  >>>>>> PEGADINHA CLASSICA DE PROVA <<<<<<
    //  Para testar se duas strings sao IGUAIS, o correto e:
    //
    //       if (strcmp(s1, s2) == 0)   { ... iguais ... }
    //
    //  O ERRO que cai em prova e escrever:
    //
    //       if (strcmp(s1, s2))        { ... iguais ... }   <-- ERRADO!
    //
    //  Por que e erro? Porque strcmp retorna 0 quando sao IGUAIS,
    //  e em C o valor 0 significa FALSO. Entao if(strcmp(...)) so entra
    //  no if quando as strings sao DIFERENTES (retorno != 0 = verdadeiro).
    //  Ou seja, o programador achou que estava testando igualdade,
    //  mas na verdade testou DIFERENCA.
    //
    //  Outro erro comum: comparar com == diretamente:
    //       if (s1 == s2)   <-- ERRADO! Compara enderecos de memoria, nao conteudo.
    //  Isso so da verdadeiro se os dois ponteiros apontarem para o MESMO lugar,
    //  nao se o CONTEUDO for igual. Para comparar CONTEUDO, sempre use strcmp.
    //
    //  NOTA: usamos comentarios // (linha) aqui para evitar o problema de
    //  comentarios /* */ aninhados, que o C NAO suporta (o primeiro */
    //  fecha todo o bloco). Isso e um erro classico de compilacao.
    printf("\nstrcmp(\"%s\",\"%s\") = %d\n", a, b, strcmp(a, b)); // 0: iguais
    printf("strcmp(\"%s\",\"%s\") = %d\n", a, c, strcmp(a, c)); // negativo: a<c

    /* Forma CORRETA de testar igualdade de strings: */
    if (strcmp(a, b) == 0) {
        printf("CORRETO: a e b sao iguais (usamos == 0)\n");
    }

    /* Demonstracao do erro (comentado para nao confundir, mas explicado): */
    /*
       if (strcmp(a, b)) { ... }   <-- entraria aqui? NAO, porque sao iguais
                                      e strcmp retorna 0 = FALSO. Nao entraria.
                                      O programador que queria "entrar se iguais"
                                      teria um bug silencioso.
    */
    if (strcmp(a, b)) {
        printf("Isso NAO imprime, porque strcmp==0 (FALSO) quando iguais.\n");
    } else {
        printf("Entrou no ELSE: confirma que strcmp retornou 0 (iguais).\n");
    }

    pausar();
}

/* =========================================================================
   (3) ctype.h: toupper, tolower, isalpha, isdigit
   ========================================================================= */
void demo_ctype(void) {
    char letra = 'f';
    char str[] = "Franco 123";
    int i;

    printf("=== (3) ctype.h: toupper / tolower / isalpha / isdigit ===\n\n");

    /* toupper: devolve a letra em MAIUSCULA. NAO altera a original. */
    printf("toupper('%c') = '%c'\n", letra, toupper(letra));
    /* tolower: devolve em MINUSCULA. */
    printf("tolower('%c') = '%c'\n", 'F', tolower('F'));

    /*
       IMPORTANTE: toupper/tolower retornam o char convertido, NAO alteram
       a variavel. Para alterar, tem que atribuir:
            ch = toupper(ch);
       Fazer so toupper(ch); (sem atribuir) NAO muda nada.
    */

    /* Percorrendo a string caractere por caractere para classificar cada um. */
    printf("\nAnalisando cada caractere de \"%s\":\n", str);
    for (i = 0; str[i] != '\0'; i++) {  /* ate achar o terminador '\0' */
        printf("  [%c] ", str[i]);
        if (isalpha(str[i])) {        /* isalpha: e letra (A-Z ou a-z)? */
            printf("LETRA");
        } else if (isdigit(str[i])) { /* isdigit: e digito (0-9)? */
            printf("DIGITO");
        } else {
            printf("OUTRO (espaco/pontuacao)");
        }
        printf("\n");
    }

    pausar();
}

/* =========================================================================
   (4) Contar vogais manualmente (sem funcao pronta)
   ========================================================================= */
/*
   contar_vogais: conta quantas vogais (a,e,i,o,u) existem na string s.
   Recebe const char * para indicar que NAO vai modificar a string.
   Retorna int com a quantidade.
*/
int contar_vogais(const char s[]) {
    int i, cont = 0;

    /* Percorre ate o '\0' (fim da string) */
    for (i = 0; s[i] != '\0'; i++) {
        /* Converte para minuscula antes de comparar, assim nao precisamos
           testar 'A' e 'a' separadamente. toupper tambem funcionaria. */
        char ch = tolower(s[i]);
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            cont++;
        }
    }
    return cont;
}

void demo_contar_vogais(void) {
    char frase[100];

    printf("=== (4) Contar vogais manualmente ===\n\n");
    printf("Digite uma frase: ");
    fgets(frase, sizeof(frase), stdin);
    frase[strcspn(frase, "\n")] = '\0';  /* remove o \n (ja vimos no item 1) */

    printf("Numero de vogais: %d\n", contar_vogais(frase));
    pausar();
}

/* =========================================================================
   (5) Inverter string manualmente (dois indices, um no inicio um no fim)
   ========================================================================= */
/*
   inverter_string: inverte a string s no proprio vetor (in-place).
   Estrategia: dois indices, i no inicio (0) e j no fim (strlen-1).
   Trocamos s[i] com s[j], andando i para frente e j para tras,
   ate se encontrarem no meio.
*/
void inverter_string(char s[]) {
    int i = 0;                       /* indice que comeca no inicio */
    int j = (int)strlen(s) - 1;      /* indice que comeca no ultimo char valido */
    char tmp;                        /* variavel auxiliar para a troca */

    while (i < j) {                  /* enquanto nao se cruzaram no meio */
        tmp  = s[i];                 /* guarda o char da frente */
        s[i] = s[j];                 /* frente recebe o char de tras */
        s[j] = tmp;                  /* tras recebe o char guardado */
        i++;                         /* avanca a frente */
        j--;                         /* recua o tras */
    }
    /* Ao final, a string esta invertida no MESMO vetor. */
}

void demo_inverter_string(void) {
    char palavra[100];

    printf("=== (5) Inverter string manualmente ===\n\n");
    printf("Digite uma palavra: ");
    fgets(palavra, sizeof(palavra), stdin);
    palavra[strcspn(palavra, "\n")] = '\0';

    printf("Original : [%s]\n", palavra);
    inverter_string(palavra);
    printf("Invertida: [%s]\n", palavra);
    pausar();
}

/* =========================================================================
   (6) strstr: busca parcial (procurar substring dentro de string)
   ========================================================================= */
void demo_strstr(void) {
    char texto[200];
    char busca[50];
    char *resultado;  /* ponteiro que vai guardar onde achou (ou NULL se nao achou) */

    printf("=== (6) strstr (busca parcial) ===\n\n");
    printf("Digite um texto: ");
    fgets(texto, sizeof(texto), stdin);
    texto[strcspn(texto, "\n")] = '\0';

    printf("Digite o trecho a procurar: ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = '\0';

    /*
       strstr(texto, busca):
       - Procura a string "busca" DENTRO da string "texto".
       - Se achar, retorna PONTEIRO para o inicio da 1a ocorrencia.
       - Se NAO achar, retorna NULL (ponteiro nulo).
       Por isso testamos: if (resultado != NULL) ou simplesmente if (resultado).
       Aqui if(resultado) esta CORRETO porque NULL == 0 == FALSO,
       e qualquer outro ponteiro == VERDADEIRO. (Diferente do strcmp!)
    */
    resultado = strstr(texto, busca);

    if (resultado != NULL) {
        /* resultado - texto da a posicao (indice) onde achou a substring.
           Aritmetica de ponteiros: subtrai o endereco inicial do endereco achado. */
        printf("Achou! na posicao %d\n", (int)(resultado - texto));
        printf("A partir dali: [%s]\n", resultado);
    } else {
        printf("Nao achou o trecho.\n");
    }
    pausar();
}

/* =========================================================================
   (7) strtok: dividir string por um separador (ex: CSV por virgula)
   ========================================================================= */
void demo_strtok_csv(void) {
    /*
       Exemplo de linha CSV (Comma-Separated Values):
       "Franco,20,9.5"
       Queremos separar em 3 campos: nome, idade, nota.
    */
    char linha[] = "Franco,20,9.5";
    char *campo;   /* ponteiro para cada pedaco que strtok vai devolver */

    printf("=== (7) strtok (separar CSV por virgula) ===\n\n");
    printf("Linha original: [%s]\n", linha);

    /*
       strtok(string, delimitadores):
       - 1a chamada: passa a string e o separador (",").
         Devolve ponteiro para o 1o campo e coloca '\0' no lugar da virgula.
       - Chamadas seguintes: passa NULL no lugar da string.
         strtok lembra internamente onde parou e continua dali.
       - Quando acaba, devolve NULL.
    */
    campo = strtok(linha, ",");  /* 1a chamada: passa a string */
    while (campo != NULL) {      /* enquanto houver campo */
        printf("Campo: [%s]\n", campo);
        campo = strtok(NULL, ",");  /* proximo campo: passa NULL */
    }

    /*
       CUIDADO com strtok:
       - Ele MODIFICA a string original (coloca '\0' no lugar dos separadores).
         Por isso "linha" nao pode ser const.
       - Ele usa uma variavel interna estatica para lembrar a posicao,
         o que o torna NAO thread-safe (nao use em threads concorrentes).
    */
    pausar();
}

/* =========================================================================
   (8) sprintf: montar string formatada (printf para dentro de uma string)
   ========================================================================= */
void demo_sprintf(void) {
    char buffer[200];   /* onde a string montada vai ficar */
    char nome[50] = "Franco";
    int idade = 20;
    float nota = 9.5f;

    printf("=== (8) sprintf (montar string) ===\n\n");

    /*
       sprintf(destino, formato, ...):
       - Igual ao printf, mas em vez de imprimir na tela,
         guarda o resultado DENTRO da string "destino".
       - Muito util para montar mensagens, linhas de log, registros de arquivo.
       - CUIDADO: destino precisa ter espaco suficiente para o resultado.
         Se o resultado for maior que o vetor, estoura o buffer.
         (C99 tem snprintf que recebe o tamanho maximo - mais seguro.)
    */
    sprintf(buffer, "Aluno: %s | Idade: %d | Nota: %.1f", nome, idade, nota);

    printf("String montada com sprintf:\n[%s]\n", buffer);
    pausar();
}

/* =========================================================================
   (9) atoi / atof: converter string para numero
   ========================================================================= */
void demo_atoi_atof(void) {
    char s_int[20] = "42";
    char s_float[20] = "3.14";
    char s_invalida[20] = "abc";

    printf("=== (9) atoi / atof (string -> numero) ===\n\n");

    /*
       atoi: ASCII to Integer. Converte string para int.
       - Para no primeiro caractere que nao e digito.
       - Se a string nao comeca com numero, retorna 0.
       atof: ASCII to Float. Converte string para double.
       - Aceita ponto decimal e sinal.
       Ambas estao em <stdlib.h>.
    */
    printf("atoi(\"%s\")  = %d\n", s_int, atoi(s_int));
    printf("atof(\"%s\") = %.2f\n", s_float, atof(s_float));
    printf("atoi(\"%s\")  = %d  (nao comeca com digito -> 0)\n",
           s_invalida, atoi(s_invalida));

    /*
       DICA de prova: para ler um NUMERO do teclado com validacao,
       o fluxo seguro e:
         1. ler como STRING com fgets
         2. converter com atoi/atof (ou strtol/strtod, mais robustas)
       Evita os problemas do scanf com %d quando o usuario digita letras.
    */
    pausar();
}

/* =========================================================================
   MENU PRINCIPAL (estilo do professor: do-while + switch)
   ========================================================================= */
int main(void) {
    int opcao;

    do {
        limpar_tela();
        printf("============================================\n");
        printf("   MANIPULACAO DE STRINGS - MENU DE DEMOS\n");
        printf("============================================\n");
        printf(" 1 - fgets + strcspn (remover \\n)\n");
        printf(" 2 - strlen / strcpy / strcat / strcmp\n");
        printf(" 3 - ctype: toupper/tolower/isalpha/isdigit\n");
        printf(" 4 - Contar vogais (manual)\n");
        printf(" 5 - Inverter string (manual)\n");
        printf(" 6 - strstr (busca parcial)\n");
        printf(" 7 - strtok (separar CSV)\n");
        printf(" 8 - sprintf (montar string)\n");
        printf(" 9 - atoi / atof (string -> numero)\n");
        printf(" 0 - Sair\n");
        printf("============================================\n");
        printf("Opcao: ");

        opcao = getche() - '0';  /* getche le 1 char e ecoa; subtrair '0' vira int */
        printf("\n\n");

        /*
           switch: seleciona qual bloco executar conforme a opcao.
           Cada case chama a funcao demo correspondente.
           O break evita "cair" no proximo case.
        */
        switch (opcao) {
            case 1: demo_fgets_strcspn();            break;
            case 2: demo_strlen_strcpy_strcat_strcmp(); break;
            case 3: demo_ctype();                    break;
            case 4: demo_contar_vogais();            break;
            case 5: demo_inverter_string();          break;
            case 6: demo_strstr();                   break;
            case 7: demo_strtok_csv();                break;
            case 8: demo_sprintf();                  break;
            case 9: demo_atoi_atof();                break;
            case 0:
                printf("Saindo... Boa prova!\n");
                break;
            default:
                printf("Opcao invalida! Escolha de 0 a 9.\n");
                pausar();
                break;
        }
    } while (opcao != 0);  /* repete o menu ate escolher 0 (sair) */

    return 0;
}
