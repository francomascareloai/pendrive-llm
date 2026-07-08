/* =====================================================================
   ARQUIVO: 27_validacao_entrada_robusta.c
   O QUE FAZ: Demonstra funcoes robustas de leitura/validacao de dados
              digitados pelo usuario. Cada funcao repete ate o dado ser
              valido, mostrando como verificar o retorno do scanf e como
              limpar o buffer do teclado corretamente.
   TOPICO:    Validacao de entrada, scanf, limpeza de buffer, fgets, CPF.
   PALAVRAS-CHAVE (Ctrl-F):
      validacao entrada | scanf retorno | limpar buffer | fflush stdin
      ler inteiro | ler float 0 a 10 | ler string so letras | ler CPF 11 digitos
      menu do-while switch | fgets strcspn | isdigit isalpha | conio getche
   COMPILA:   gcc -Wall -Wextra 27_validacao_entrada_robusta.c -o validacao.exe
   ===================================================================== */

/* ---- Bibliotecas padrao ---- */
#include <stdio.h>   /* printf, scanf, fgets, fflush, stdin, stdout        */
#include <stdlib.h>  /* system, EXIT_SUCCESS                                */
#include <string.h>  /* strlen, strcspn (procura caractere na string)      */
#include <ctype.h>   /* isalpha, isdigit, isspace (classificacao de char)   */
#include <conio.h>   /* getche, getch (le 1 tecla sem Enter) - Windows/MinGW */

/* ---------------------------------------------------------------------
   Cabecalho do programa: so apresenta o menu e chama as funcoes.
   Toda a logica de validacao fica em funcoes separadas (modularizacao),
   para o main() ficar curto e legivel. Isso cai na prova: o professor
   quer ver funcoes pequenas e bem definidas, nao um main gigante.
   --------------------------------------------------------------------- */

/* =====================================================================
   FUNCAO: limpar_buffer_teclado
   OBJETIVO: Descartar todos os caracteres que sobraram no buffer do
            teclado apos uma leitura com scanf. Se nao limpar, o proximo
            scanf/fgets le lixo (ex: o '\n' que sobrou do Enter).
   DETALHE: fflush(stdin) NAO e portavel segundo o padrao C (comportamento
            indefinido em outros sistemas), MAS funciona no Windows com
            o MinGW/MSVC. Por isso usamos aqui com este aviso. Em codigo
            portavel, o correto e o loop com getchar() mostrado abaixo.
   ===================================================================== */
void limpar_buffer_teclado(void)
{
    int c; /* variavel para receber cada caractere descartado */

    /* getchar() le 1 caractere por vez do buffer.
       O loop roda enquanto o caractere lido for DIFERENTE de '\n'
       (Enter) e diferente de EOF (fim de fluxo).
       Assim consumimos tudo ate o Enter, limpando o buffer. */
    c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar(); /* le proximo caractere e continua descartando */
    }
}

/* =====================================================================
   FUNCAO: ler_inteiro_valido
   OBJETIVO: Ler um numero inteiro digitado pelo usuario, repetindo ate
            que o scanf consiga ler de fato um inteiro (retorno == 1).
   RETORNO: o inteiro valido digitado.
   EXPLICAÇÃO DO RETORNO DO scanf:
      scanf retorna a QUANTIDADE de itens que conseguiu converter com
      sucesso. Para "%d", se o usuario digita "42", retorna 1 (ok).
      Se digita "abc", retorna 0 (nao converteu nada). Se digita Ctrl+Z
      (EOF), retorna -1. Por isso testamos == 1.
   ===================================================================== */
int ler_inteiro_valido(const char *mensagem)
{
    int numero;   /* onde vamos guardar o inteiro lido            */
    int ok;      /* vai receber o retorno do scanf (1, 0 ou -1)  */

    do
    {
        printf("%s", mensagem); /* exibe o prompt ao usuario       */
        fflush(stdout);         /* garante que o texto apareceu ANTES
                                   da leitura (stdout com \n as vezes
                                   fica em buffer). Boa pratica.    */

        /* scanf tenta ler um inteiro. Guardamos o retorno em 'ok'. */
        ok = scanf("%d", &numero);

        /* SEMPRE limpar o buffer depois de scanf, independente de
           ter dado certo ou errado. Se o usuario digitou "abc\n",
           o scanf nao consumiu nada e o '\n' (e o "abc") ficam la. */
        limpar_buffer_teclado();

        /* Se scanf nao converteu 1 item, avisa e repete o loop. */
        if (ok != 1)
        {
            printf("  -> Entrada invalida! Digite apenas numeros.\n");
        }
    } while (ok != 1); /* repete ate conseguir ler 1 inteiro com sucesso */

    return numero; /* devolve o inteiro validado */
}

/* =====================================================================
   FUNCAO: ler_float_faixa
   OBJETIVO: Ler um numero real (float) que esteja entre 'min' e 'max'
            (inclusive). Repete ate o valor ser valido E estar na faixa.
   PARAMETROS:
      mensagem - texto exibido ao usuario
      min, max - limites inferior e superior aceitos
   EXPLICAÇÃO: Combinamos duas validacoes:
      1) scanf retornou 1? (digitou um numero, nao texto)
      2) o numero esta entre min e max?
   ===================================================================== */
float ler_float_faixa(const char *mensagem, float min, float max)
{
    float valor; /* guarda o float lido                       */
    int ok;      /* retorno do scanf                           */

    do
    {
        printf("%s", mensagem);
        fflush(stdout);

        /* %f le um float. Se digitar "7.5", ok=1. Se "abc", ok=0. */
        ok = scanf("%f", &valor);

        /* limpa o buffer sempre (regra de ouro apos scanf). */
        limpar_buffer_teclado();

        if (ok != 1)
        {
            printf("  -> Digite um numero valido (ex: 7.5).\n");
        }
        else if (valor < min || valor > max)
        {
            /* numero valido, mas fora da faixa permitida */
            printf("  -> Valor fora da faixa! Digite entre %.1f e %.1f.\n",
                   min, max);
            ok = 0; /* forca repetir o loop, pois nao esta valido ainda */
        }
    } while (ok != 1); /* so sai quando for numero E estiver na faixa */

    return valor;
}

/* =====================================================================
   FUNCAO: ler_string_so_letras
   OBJETIVO: Ler uma string (via fgets) que contenha APENAS letras e
            espacos. Repete ate o usuario digitar algo valido.
   PARAMETROS:
      mensagem - texto exibido ao usuario
      destino  - vetor onde a string lida sera armazenada
      tamanho  - tamanho maximo do vetor (para nao estourar)
   EXPLICAÇÃO DO fgets:
      fgets(destino, tamanho, stdin) le ate (tamanho-1) caracteres ou
      ate encontrar '\n'. Diferente do gets (perigoso e proibido), o
      fgets respeita o tamanho do vetor, evitando overflow de buffer.
      O '\n' do Enter costuma entrar na string; removemos com strcspn.
   ===================================================================== */
void ler_string_so_letras(const char *mensagem, char *destino, int tamanho)
{
    int valida;   /* flag: 1 = string valida, 0 = invalida */
    int i;        /* contador para percorrer a string        */

    do
    {
        valida = 1; /* parte do principio que sera valida */

        printf("%s", mensagem);
        fflush(stdout);

        /* fgets le a linha inteira com seguranca (ate tamanho-1 chars). */
        if (fgets(destino, tamanho, stdin) == NULL)
        {
            /* Se retornou NULL, houve erro ou EOF sem dados. Repete. */
            valida = 0;
            continue; /* volta ao inicio do do-while */
        }

        /* strcspn retorna a posicao do primeiro '\n' na string.
           Colocamos '\0' nessa posicao, removendo o Enter que o fgets
           incluiu. Se nao houver '\n' (usuario digitou mais que o
           tamanho), strcspn retorna o fim da string e nao causa dano. */
        destino[strcspn(destino, "\n")] = '\0';

        /* Se o usuario digitou mais caracteres que o vetor comporta, o
           '\n' nao entrou na string e sobrou lixo no buffer. Limamos. */
        if (strlen(destino) == (size_t)(tamanho - 1))
        {
            /* Pode ter sobrado o Enter no buffer; limpamos por seguranca. */
            limpar_buffer_teclado();
        }

        /* Verifica caractere por caractere se so ha letras/espacos. */
        for (i = 0; destino[i] != '\0'; i++)
        {
            /* isalpha retorna verdadeiro se for letra.
               isspace retorna verdadeiro se for espaco/espaco.
               Se achar qualquer outra coisa (numero, simbolo), invalida. */
            if (!isalpha((unsigned char)destino[i]) &&
                !isspace((unsigned char)destino[i]))
            {
                valida = 0; /* achou caractere invalido */
                break;       /* nao precisa continuar o for */
            }
        }

        /* Se a string estiver vazia (so tinha Enter), tambem invalida. */
        if (destino[0] == '\0')
        {
            valida = 0;
        }

        if (!valida)
        {
            printf("  -> Invalido! Use apenas letras e espacos.\n");
        }
    } while (!valida); /* repete ate a string ser valida */
}

/* =====================================================================
   FUNCAO: ler_cpf
   OBJETIVO: Ler um CPF de exatamente 11 digitos numericos. Aceita ou
            nao o usuario digitar pontos/traço (ex: 123.456.789-09),
            mas valida se, apos remover a pontuacao, sobram 11 digitos.
   PARAMETROS:
      mensagem - texto exibido
      destino  - vetor (minimo 12 posicoes) onde o CPF limpo sera salvo
   EXPLICAÇÃO: Usamos getche() para ler tecla por tecla, permitindo
      formatar a entrada (mostrar pontos e traço na tela). getche le
      1 caractere e ja o ecoa na tela (diferente do getch que nao ecoa).
   ===================================================================== */
void ler_cpf(const char *mensagem, char *destino)
{
    int i;        /* contador de digitos armazenados */
    char tecla;   /* ultima tecla pressionada        */

    do
    {
        i = 0; /* zera o contador a cada tentativa */

        printf("%s", mensagem);
        fflush(stdout);

        /* Le ate 11 digitos validos. getche() le e ecoa o caractere. */
        while (i < 11)
        {
            tecla = getche(); /* le 1 tecla e mostra na tela */

            /* Se o usuario apertar Enter antes de terminar, cancela. */
            if (tecla == '\r') /* '\r' e o Enter no Windows */
            {
                printf("\n  -> CPF incompleto! Digite 11 digitos.\n");
                i = 0; /* recomeca */
                break;
            }

            /* isdigit: verdadeiro se for '0' a '9'. */
            if (isdigit((unsigned char)tecla))
            {
                destino[i] = tecla; /* guarda o digito */
                i++;
            }
            else
            {
                /* Ignora pontos, traços, espacos (nao conta nem guarda). */
                /* Apenas nao incrementa i, entao o usuario pode digitar
                   "123.456.789-09" que os pontos/traços sao descartados. */
            }
        }

        /* Se saiu do while com i==11, CPF completo foi lido. */
    } while (i != 11); /* repete se nao completou 11 digitos */

    destino[11] = '\0'; /* finaliza a string do CPF com '\0' */
    printf("\n");       /* quebra linha apos a digitacao do CPF */
}

/* =====================================================================
   FUNCAO: ler_opcao_menu
   OBJETIVO: Ler uma opcao de menu (char) e validar se esta entre as
            opcoes permitidas. Retorna o caractere valido escolhido.
   EXPLICAÇÃO: Usamos getch() para ler a tecla sem ecoar (menu tipico
      do professor: "Pressione a opcao desejada"). Depois validamos.
   ===================================================================== */
char ler_opcao_menu(const char *opcoes_validas)
{
    char tecla; /* tecla pressionada pelo usuario */

    do
    {
        printf("Digite sua opcao: ");
        fflush(stdout);

        tecla = getch(); /* le 1 tecla SEM ecoar na tela */
        printf("%c\n", tecla); /* ecoamos manualmente para o usuario ver */

        /* strchr procura 'tecla' dentro da string opcoes_validas.
           Se encontrar, retorna ponteiro != NULL (opcao valida).
           Se nao encontrar, retorna NULL (opcao invalida). */
        if (strchr(opcoes_validas, tecla) == NULL)
        {
            printf("  -> Opcao invalida! Tente: %s\n", opcoes_validas);
        }
    } while (strchr(opcoes_validas, tecla) == NULL);

    return tecla; /* devolve a opcao validada */
}

/* =====================================================================
   FUNCAO: menu_principal
   OBJETIVO: Mostrar o menu de opcoes na tela.
   ===================================================================== */
void menu_principal(void)
{
    system("cls"); /* limpa a tela (Windows). Nao e portavel, mas ok aqui. */
    printf("=============================================\n");
    printf("   VALIDACAO DE ENTRADA ROBUSTA - Exemplos   \n");
    printf("=============================================\n");
    printf(" 1 - Ler inteiro valido\n");
    printf(" 2 - Ler nota (float 0 a 10)\n");
    printf(" 3 - Ler nome (so letras)\n");
    printf(" 4 - Ler CPF (11 digitos)\n");
    printf(" 5 - Ler opcao de menu valida\n");
    printf(" 0 - Sair\n");
    printf("=============================================\n");
}

/* =====================================================================
   FUNCAO: main
   OBJETIVO: Controlar o fluxo do programa via menu do-while + switch.
   ===================================================================== */
int main(void)
{
    char opcao;          /* opcao escolhida no menu                    */
    char opcoes[] = "012345"; /* string com todas as opcoes validas    */
    int  inteiro;        /* guarda inteiro lido                        */
    float nota;          /* guarda nota lida                           */
    char nome[100];      /* vetor para guardar o nome (ate 99 chars)    */
    char cpf[12];        /* vetor para CPF: 11 digitos + '\0' = 12      */

    /* do-while: o menu aparece pelo menos 1 vez e repete ate opcao '0'. */
    do
    {
        menu_principal(); /* exibe o menu */

        /* Le e valida a opcao em uma funcao separada (modularizacao). */
        opcao = ler_opcao_menu(opcoes);

        /* switch: direciona para a funcao conforme a opcao escolhida. */
        switch (opcao)
        {
            case '1':
                printf("\n--- Ler inteiro valido ---\n");
                inteiro = ler_inteiro_valido("Digite um inteiro: ");
                printf("  OK! Voce digitou: %d\n", inteiro);
                break;

            case '2':
                printf("\n--- Ler nota (0 a 10) ---\n");
                /* Passamos a faixa 0.0 a 10.0 para a funcao validar. */
                nota = ler_float_faixa("Digite a nota: ", 0.0f, 10.0f);
                printf("  OK! Nota valida: %.2f\n", nota);
                break;

            case '3':
                printf("\n--- Ler nome (so letras) ---\n");
                /* Passamos o vetor, seu tamanho e a mensagem. */
                ler_string_so_letras("Digite o nome: ", nome, (int)sizeof(nome));
                printf("  OK! Nome valido: %s\n", nome);
                break;

            case '4':
                printf("\n--- Ler CPF (11 digitos) ---\n");
                /* cpf tem 12 posicoes (11 digitos + '\0'). */
                ler_cpf("Digite o CPF (so numeros): ", cpf);
                printf("  OK! CPF valido: %s\n", cpf);
                break;

            case '5':
                printf("\n--- Ler opcao de menu valida ---\n");
                /* Exemplo: simular submenu com opcoes S/N. */
                {
                    char sub;
                    sub = ler_opcao_menu("SsNn");
                    printf("  OK! Opcao escolhida: %c\n", sub);
                }
                break;

            case '0':
                printf("\nSaindo do programa... Ate logo!\n");
                break;

            default:
                /* Nunca deve chegar aqui porque ler_opcao_menu ja valida,
                   mas e boa pratica deixar um default por seguranca. */
                printf("Opcao desconhecida.\n");
                break;
        }

        /* Pausa para o usuario ver o resultado antes de voltar ao menu.
           So pausa se nao for a opcao de saida. */
        if (opcao != '0')
        {
            system("pause"); /* "Pressione qualquer tecla..." (Windows) */
        }

    } while (opcao != '0'); /* repete ate escolher sair */

    return EXIT_SUCCESS; /* retorno padrao de sucesso (0) */
}

/* =====================================================================
   RESUMO DIDATICO (para revisar antes da prova):
   1) scanf retorna o numero de itens convertidos. Teste == 1 para %d/%f.
   2) SEMPRE limpe o buffer apos scanf (loop com getchar ate '\n').
   3) fflush(stdin) funciona no Windows mas NAO e portavel (evite em
      codigo multiplataforma; prefira o loop com getchar).
   4) Use fgets + strcspn para ler strings com seguranca (nunca gets).
   5) isalpha/isdigit/isspace (ctype.h) validam caractere por caractere.
   6) strchr verifica se um caractere esta num conjunto (validar menu).
   7) Modularize: cada tipo de leitura em sua propria funcao.
   ===================================================================== */
