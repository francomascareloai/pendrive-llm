/*
============================================================================
 ARQUIVO: 25_crud_binario_completo.c
 TOPICO:   Arquivo BINARIO + struct + CRUD completo + modularizacao
 OBJETIVO: Reunir NUM SO PROGRAMA todos os padroes de arquivo binario que
           caem na prova: criar (wb), cadastrar com fwrite e checagem de
           duplicidade (ab+rb), listar com fread==1 (rb), buscar por codigo
           com fseek acesso direto (rb), alterar copiando para temporario
           (rb + wb temp + rename), excluir copiando e pulando o excluido
           (rb + wb temp + rename), contar registros com fseek END + ftell
           dividido por sizeof(struct). Tudo modularizado em funcoes.
 PALAVRAS-CHAVE (Ctrl+F): arquivo binario, fwrite, fread, fopen, wb, ab, rb,
                          rb+, wb, fseek, SEEK_SET, SEEK_END, ftell, rewind,
                          sizeof, struct, typedef struct, fread==1, feof,
                          duplicidade, acesso direto, arquivo temporario,
                          rename, remove, fclose, CRUD, modularizacao,
                          fgets, strcspn, fflush(stdin), conio.h, menu switch
 COMPILA:  gcc -Wall -Wextra 25_crud_binario_completo.c -o programa
============================================================================
*/

#include <stdio.h>    /* fopen, fclose, fwrite, fread, fseek, ftell, rename   */
#include <stdlib.h>   /* system, exit, EXIT_FAILURE, EXIT_SUCCESS, remove     */
#include <string.h>   /* strcspn (tirar \n do fgets) - NAO usar gets()        */
#include <conio.h>    /* getche, getch - estilo do professor (Windows/MinGW)  */

/* -------------------------------------------------------------------------
   DEFINICAO DA STRUCT (typedef struct correto, C puro).
   O typedef permite declarar variaveis escrevendo so "Cliente x;" em vez
   de "struct Cliente x;". Isso cai em prova: saber diferenciar struct com
   e sem typedef.
   A struct TEM QUE estar definida ANTES das funcoes que a usam, por isso
   fica aqui no topo, logo apos os includes.
   ------------------------------------------------------------------------- */
typedef struct
{
    int  codigo;       /* chave primaria do cliente (usada na busca direta)   */
    char nome[50];     /* nome do cliente (string com espaco para 49 + '\0') */
    int  idade;        /* idade do cliente (inteiro)                         */
} Cliente;             /* novo nome do tipo: agora "Cliente" existe           */

/* -------------------------------------------------------------------------
   PROTOTIPOS DAS FUNCOES (declaracao antes do main para o compilador conhecer
   as assinaturas). Modularizacao é topico de prova: saber separar em funcoes
   e usar prototipos. Cada funcao faz UMA coisa so (boa pratica).
   ------------------------------------------------------------------------- */
void criarArquivo(void);          /* opcao 1: cria/zeria o arquivo (wb)      */
void cadastrar(void);             /* opcao 2: adiciona cliente (ab+rb)       */
void listarTodos(void);           /* opcao 3: lista registros (rb, fread==1)  */
void buscarPorCodigo(void);       /* opcao 4: busca direta com fseek (rb)    */
void alterar(void);              /* opcao 5: altera via temp+rename (rb+wb)  */
void excluir(void);             /* opcao 6: exclui via temp+rename (rb+wb)  */
void contarRegistros(void);     /* opcao 7: conta com fseek END+ftell       */
int  codigoExiste(int cod);     /* auxiliar: verifica duplicidade de codigo */
void limparBufferEntrada(void); /* auxiliar: limpa stdin de forma portatil  */

/* Nome do arquivo binario usado em TODO o programa. Como é uma constante
   (#define), se um dia quiser mudar o nome do arquivo, muda em UM lugar so. */
#define NOME_ARQUIVO "clientes.dat"

/* -------------------------------------------------------------------------
   FUNCAO: limparBufferEntrada
   Limpa o buffer de entrada (stdin) descartando caracteres ate '\n' ou EOF.
   Esta é a forma PORTAVEL de limpar stdin (funciona em Windows, Linux, Mac).
   O professor costuma usar fflush(stdin), que funciona no Windows (MinGW)
   mas é "comportamento indefinido" pelo padrao C (nao funciona em Linux/Mac).
   Aqui usamos a versao portatil para evitar bugs, mas mantemos o comentario
   explicando o fflush(stdin) porque ele cai na prova.
   ------------------------------------------------------------------------- */
void limparBufferEntrada(void)
{
    int c;
    /* descarta caracteres ate achar o '\n' (enter) ou EOF (fim de entrada) */
    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* corpo vazio: so queremos descartar os caracteres */
    }
}

/* -------------------------------------------------------------------------
   FUNCAO: criarArquivo  (OPCAO 1)
   Cria o arquivo binario do zero no modo "wb" (write binary).
   MODO "wb": cria o arquivo se nao existir; se JÁ existir, TRUNCA (zera)
   todo o conteudo anterior. Por isso essa opcao é "destrutiva": apaga tudo.
   Cai em prova: saber que "wb" zera o arquivo existente.
   ------------------------------------------------------------------------- */
void criarArquivo(void)
{
    FILE *fp;   /* ponteiro para o arquivo que vamos abrir */

    /* fopen com "wb" cria/trunca o arquivo binario. Retorna NULL se falhar
       (ex.: pasta protegida contra escrita). Sempre testar NULL!          */
    fp = fopen(NOME_ARQUIVO, "wb");
    if (fp == NULL)
    {
        printf("Erro ao criar o arquivo %s\n", NOME_ARQUIVO);
        system("pause");          /* estilo do professor: pausa antes de voltar */
        return;                  /* retorna ao menu sem fazer mais nada         */
    }

    /* Como abrimos em "wb", o arquivo ja esta vazio. Nao precisamos escrever
       nada agora; so fechar para confirmar a criacao. O fclose descarrega
       o buffer interno e fecha o arquivo no sistema operacional.           */
    fclose(fp);

    printf("Arquivo '%s' criado com sucesso (modo wb).\n", NOME_ARQUIVO);
    printf("ATENCAO: se o arquivo ja existia, o conteudo anterior foi apagado.\n");
    system("pause");
}

/* -------------------------------------------------------------------------
   FUNCAO: codigoExiste  (AUXILIAR)
   Percorre o arquivo em modo leitura ("rb") procurando um registro cujo
   campo codigo seja igual ao parametro 'cod'. Retorna 1 (existe) ou 0 (nao).
   Usada na opcao Cadastrar para EVITAR codigos duplicados (chave primaria).
   Cai em prova: checagem de duplicidade percorrendo arquivo binario.
   ------------------------------------------------------------------------- */
int codigoExiste(int cod)
{
    FILE *fp;
    Cliente c;    /* variavel struct para receber cada registro lido */
    int achou = 0; /* flag: 0 = nao achou, 1 = achou */

    fp = fopen(NOME_ARQUIVO, "rb");   /* rb = read binary (so leitura) */
    if (fp == NULL)
    {
        /* Se o arquivo nem existe ainda, com certeza o codigo nao existe. */
        return 0;
    }

    /* PADRAO CORRETO de leitura em arquivo binario:
       while (fread(&registro, sizeof(registro), 1, fp) == 1)
       fread retorna o NUMERO DE BLOCOS lidos com sucesso. Como pedimos 1 bloco
       por vez, ele retorna 1 enquanto conseguir ler, e 0 quando chegar no fim
       (EOF). NUNCA use while(!feof(fp)) com fread ANTES do loop: feof so vira
       verdadeiro DEPOIS de uma leitura falha, o que faz o ultimo registro ser
       processado duas vezes. Esse é um erro classico de prova.               */
    while (fread(&c, sizeof(Cliente), 1, fp) == 1)
    {
        if (c.codigo == cod)   /* compara o codigo lido com o procurado */
        {
            achou = 1;          /* marca que achou                       */
            break;              /* nao precisa continuar lendo; sai do while */
        }
    }

    fclose(fp);    /* sempre fechar o arquivo apos usar */
    return achou;   /* devolve 1 se existe, 0 se nao     */
}

/* -------------------------------------------------------------------------
   FUNCAO: cadastrar  (OPCAO 2)
   Adiciona um novo cliente no FIM do arquivo binario usando modo "ab"
   (append binary). O modo "ab" so permite ESCREVER no fim do arquivo,
   preservando o conteudo anterior (nao zera como o "wb").
   Antes de gravar, verifica se o codigo ja existe (chave primaria unica).
   Cai em prova: fwrite + checagem de duplicidade + modo ab.
   ------------------------------------------------------------------------- */
void cadastrar(void)
{
    FILE *fp;
    Cliente c;    /* struct local que sera preenchida e gravada */

    /* --- Leitura dos dados do teclado --- */
    printf("\n--- CADASTRO DE CLIENTE ---\n");

    printf("Codigo: ");
    scanf("%d", &c.codigo);

    /* fflush(stdin): limpa o '\n' deixado no buffer pelo scanf anterior.
       O professor usa fflush(stdin) e funciona no Windows (MinGW).
       ATENCAO: pelo padrao C, fflush em fluxo de ENTRADA é comportamento
       indefinido (nao funciona em Linux/Mac). Para ser portatel usariamos
       a funcao limparBufferEntrada(), mas mantemos fflush(stdin) por
       fidelidade ao estilo do professor (é o que cai na prova).          */
    fflush(stdin);

    /* CHECAGEM DE DUPLICIDADE: antes de gravar, verifica se o codigo ja
       existe no arquivo. Se existir, aborta o cadastro (chave primaria). */
    if (codigoExiste(c.codigo))
    {
        printf("ERRO: ja existe um cliente com o codigo %d. Cadastro cancelado.\n",
               c.codigo);
        system("pause");
        return;   /* volta para o menu sem gravar */
    }

    /* Leitura do nome: NUNCA usar gets() (inseguro, removido do padrao C11).
       Usamos fgets, que le ate o limite do buffer ou ate o '\n'.
       fgets guarda o '\n' no final da string, por isso removemos depois. */
    printf("Nome: ");
    fgets(c.nome, sizeof(c.nome), stdin);

    /* strcspn devolve o indice do primeiro caractere que bate com o conjunto
       informado. Aqui procuramos '\n'. Substituimos por '\0' para remover o
       enter que o fgets guardou. Isso cai em prova (pegadinha do fgets). */
    c.nome[strcspn(c.nome, "\n")] = '\0';

    printf("Idade: ");
    scanf("%d", &c.idade);
    fflush(stdin);   /* limpa o '\n' deixado pelo scanf antes de voltar ao menu */

    /* --- Gravacao no arquivo binario --- */
    /* "ab" = append binary: abre para escrita no FIM do arquivo. Se o arquivo
       nao existe, ele é criado. Se existe, preserva o conteudo e posiciona o
       cursor no fim. Nao zera como o "wb".                                  */
    fp = fopen(NOME_ARQUIVO, "ab");
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita (ab).\n");
        system("pause");
        return;
    }

    /* fwrite grava UM bloco de sizeof(Cliente) bytes a partir do endereco &c.
       Parametros: fwrite(ponteiro_dados, tamanho_bloco, qtd_blocos, arquivo).
       Retorna o numero de blocos gravados. Se retornar != 1, deu erro.    */
    if (fwrite(&c, sizeof(Cliente), 1, fp) != 1)
    {
        printf("Erro ao gravar o registro no arquivo.\n");
    }
    else
    {
        printf("Cliente cadastrado com sucesso!\n");
    }

    fclose(fp);   /* descarrega o buffer e fecha o arquivo */
    system("pause");
}

/* -------------------------------------------------------------------------
   FUNCAO: listarTodos  (OPCAO 3)
   Lista todos os registros do arquivo binario em modo leitura ("rb").
   Usa o padrao CORRETO: while (fread(...) == 1). NUNCA usar while(!feof)
   com fread antes do laco (erro classico que duplica o ultimo registro).
   Cai em prova: leitura sequencial de arquivo binario.
   ------------------------------------------------------------------------- */
void listarTodos(void)
{
    FILE *fp;
    Cliente c;   /* struct que recebera cada registro lido */

    fp = fopen(NOME_ARQUIVO, "rb");   /* rb = read binary (so leitura) */
    if (fp == NULL)
    {
        printf("Arquivo '%s' nao encontrado. Use a opcao 1 para cria-lo.\n",
               NOME_ARQUIVO);
        system("pause");
        return;
    }

    printf("\n--- LISTA DE CLIENTES ---\n");
    printf("%-8s %-30s %s\n", "CODIGO", "NOME", "IDADE");
    printf("----------------------------------------------\n");

    /* PADRAO CORRETO: fread retorna 1 enquanto conseguir ler um bloco.
       Quando chegar no fim do arquivo, retorna 0 e o while termina.
     NAO usar while(!feof(fp)) porque feof so fica verdadeiro DEPOIS de uma
       leitura que ja falhou, fazendo o loop rodar uma vez a mais e repetir
       o ultimo registro. */
    while (fread(&c, sizeof(Cliente), 1, fp) == 1)
    {
        /* imprime o registro lido. %-8s alinha a esquerda com 8 espacos. */
        printf("%-8d %-30s %d\n", c.codigo, c.nome, c.idade);
    }

    fclose(fp);
    system("pause");
}

/* -------------------------------------------------------------------------
   FUNCAO: buscarPorCodigo  (OPCAO 4)
   Busca um cliente pelo codigo usando fseek para ACESSO DIRETO.
   Acesso direto significa pular direto para a posicao do registro sem
   percorrer todos os anteriores. Isso so funciona em arquivo binario porque
   todos os registros tem o MESMO tamanho (sizeof(Cliente)).
   Cai em prova: fseek + SEEK_SET para acesso direto por calculo de offset.
   ------------------------------------------------------------------------- */
void buscarPorCodigo(void)
{
    FILE *fp;
    Cliente c;
    int codBusca, achou = 0;

    printf("\n--- BUSCA POR CODIGO ---\n");
    printf("Digite o codigo do cliente: ");
    scanf("%d", &codBusca);
    fflush(stdin);

    fp = fopen(NOME_ARQUIVO, "rb");
    if (fp == NULL)
    {
        printf("Arquivo nao encontrado.\n");
        system("pause");
        return;
    }

    /* --- ACESSO DIRETO com fseek ---
       fseek move o cursor do arquivo para uma posicao calculada.
       Parametros: fseek(arquivo, offset_em_bytes, origem).
       Origens possiveis:
         SEEK_SET -> a partir do INICIO do arquivo (mais usado)
         SEEK_CUR -> a partir da posicao ATUAL do cursor
         SEEK_END -> a partir do FIM do arquivo (usado para contar registros)
       Aqui fazemos busca SEQUENCIAL com fseek posicionando no inicio de cada
       registro, para mostrar o calculo de offset. O offset do registro N é:
         N * sizeof(Cliente)
       porque cada registro ocupa exatamente sizeof(Cliente) bytes.          */
    rewind(fp);   /* garante que o cursor esta no inicio (posicao 0) */

    /* Percorremos registro por registro. Para cada um, posicionamos o cursor
       com fseek e lemos com fread. Isso demonstra o calculo de offset.     */
    long offset = 0;   /* posicao em bytes do registro atual */
    while (fread(&c, sizeof(Cliente), 1, fp) == 1)
    {
        if (c.codigo == codBusca)
        {
            printf("\nCliente encontrado:\n");
            printf("Codigo: %d\n", c.codigo);
            printf("Nome:   %s\n", c.nome);
            printf("Idade:  %d\n", c.idade);
            achou = 1;
            break;   /* achou, nao precisa continuar procurando */
        }
        /* atualiza o offset para o proximo registro (avanca sizeof bytes) */
        offset += sizeof(Cliente);
        /* posiciona o cursor no inicio do proximo registro */
        fseek(fp, offset, SEEK_SET);
    }

    if (!achou)
    {
        printf("Cliente com codigo %d nao encontrado.\n", codBusca);
    }

    fclose(fp);
    system("pause");
}

/* -------------------------------------------------------------------------
   FUNCAO: alterar  (OPCAO 5)
   Altera um registro existente. Como alterar "no lugar" com rb+ exige cuidado
   com o posicionamento do cursor, o padrao mais seguro (e que cai em prova)
   é copiar todo o arquivo para um temporario, trocando o registro alterado,
   e depois renomear o temporario para o nome original.
   Padrao: abrir original em "rb", abrir temp em "wb", copiar registro a
   registro, no registro desejado escrever os novos dados, fechar ambos,
   remove() o original e rename() o temp para o nome original.
   ------------------------------------------------------------------------- */
void alterar(void)
{
    FILE *fpOrig, *fpTemp;   /* original (leitura) e temporario (escrita) */
    Cliente c;
    int codBusca, achou = 0;

    printf("\n--- ALTERACAO DE CLIENTE ---\n");
    printf("Digite o codigo do cliente a alterar: ");
    scanf("%d", &codBusca);
    fflush(stdin);

    /* Abre o original em "rb" (so leitura) */
    fpOrig = fopen(NOME_ARQUIVO, "rb");
    if (fpOrig == NULL)
    {
        printf("Arquivo nao encontrado.\n");
        system("pause");
        return;
    }

    /* Abre um temporario em "wb" (write binary). O nome "temp.tmp" é
       arbitrario; sera renomeado depois. "wb" cria/trunca o temp. */
    fpTemp = fopen("temp.tmp", "wb");
    if (fpTemp == NULL)
    {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(fpOrig);
        system("pause");
        return;
    }

    /* Copia registro por registro do original para o temporario.
       Quando achar o registro com o codigo buscado, pede os novos dados
       e escreve a versao alterada no temporario (em vez da original). */
    while (fread(&c, sizeof(Cliente), 1, fpOrig) == 1)
    {
        if (c.codigo == codBusca && !achou)
        {
            /* Achou o registro. Mostra os dados atuais e pede os novos. */
            printf("\nCliente encontrado:\n");
            printf("Nome atual: %s\n", c.nome);
            printf("Idade atual: %d\n", c.idade);

            printf("\n--- NOVOS DADOS ---\n");
            /* Mantem o MESMO codigo (nao deixa trocar a chave primaria) */
            printf("Novo nome: ");
            fflush(stdin);
            fgets(c.nome, sizeof(c.nome), stdin);
            c.nome[strcspn(c.nome, "\n")] = '\0';   /* tira o '\n' do fgets */

            printf("Nova idade: ");
            scanf("%d", &c.idade);
            fflush(stdin);

            achou = 1;   /* marca que fez a alteracao */
        }

        /* Escreve o registro (alterado ou nao) no temporario.
           Se foi o alterado, escreve a versao nova; senao, copia igual. */
        fwrite(&c, sizeof(Cliente), 1, fpTemp);
    }

    /* Fecha AMBOS os arquivos antes de remover/renomear.
       No Windows, nao é possivel renomear/remover um arquivo aberto. */
    fclose(fpOrig);
    fclose(fpTemp);

    if (achou)
    {
        /* remove apaga o arquivo original do disco. */
        remove(NOME_ARQUIVO);
        /* rename muda o nome do temporario para o nome original.
           Assim o "temp.tmp" vira "clientes.dat" com o conteudo alterado. */
        if (rename("temp.tmp", NOME_ARQUIVO) == 0)
        {
            printf("Cliente alterado com sucesso!\n");
        }
        else
        {
            printf("Erro ao renomear o arquivo temporario.\n");
        }
    }
    else
    {
        /* Se nao achou o codigo, apaga o temporario (que é igual ao original)
           para nao deixar lixo no disco. */
        remove("temp.tmp");
        printf("Cliente com codigo %d nao encontrado.\n", codBusca);
    }

    system("pause");
}

/* -------------------------------------------------------------------------
   FUNCAO: excluir  (OPCAO 6)
   Exclui um registro. A exclusao em arquivo binario é LOGICA via copia:
   copia todos os registros para um temporario, EXCETO o que tem o codigo
   buscado, e depois renomeia o temporario para o nome original.
   O registro excluido simplesmente nao é copiado, entao some do arquivo.
   Padrao identico ao alterar, mas pulando o registro alvo em vez de troca-lo.
   ------------------------------------------------------------------------- */
void excluir(void)
{
    FILE *fpOrig, *fpTemp;
    Cliente c;
    int codBusca, achou = 0;

    printf("\n--- EXCLUSAO DE CLIENTE ---\n");
    printf("Digite o codigo do cliente a excluir: ");
    scanf("%d", &codBusca);
    fflush(stdin);

    fpOrig = fopen(NOME_ARQUIVO, "rb");
    if (fpOrig == NULL)
    {
        printf("Arquivo nao encontrado.\n");
        system("pause");
        return;
    }

    fpTemp = fopen("temp.tmp", "wb");
    if (fpTemp == NULL)
    {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(fpOrig);
        system("pause");
        return;
    }

    /* Copia todos os registros, PULANDO o que tem o codigo buscado. */
    while (fread(&c, sizeof(Cliente), 1, fpOrig) == 1)
    {
        if (c.codigo == codBusca)
        {
            /* Achou o registro a excluir: NAO escreve no temporario.
               So marca que achou e continua o loop (pula a fwrite abaixo). */
            achou = 1;
            continue;   /* pula para o proximo registro sem gravar este */
        }

        /* Escreve no temporario todos os registros que NAO sao o excluido. */
        fwrite(&c, sizeof(Cliente), 1, fpTemp);
    }

    fclose(fpOrig);
    fclose(fpTemp);

    if (achou)
    {
        remove(NOME_ARQUIVO);   /* apaga o original */
        if (rename("temp.tmp", NOME_ARQUIVO) == 0)
        {
            printf("Cliente excluido com sucesso!\n");
        }
        else
        {
            printf("Erro ao renomear o arquivo temporario.\n");
        }
    }
    else
    {
        remove("temp.tmp");   /* nao achou: descarta o temporario */
        printf("Cliente com codigo %d nao encontrado.\n", codBusca);
    }

    system("pause");
}

/* -------------------------------------------------------------------------
   FUNCAO: contarRegistros  (OPCAO 7)
   Conta quantos registros existem no arquivo usando fseek + ftell.
   Tecnica: mover o cursor para o FIM do arquivo (SEEK_END), perguntar ao
   ftell qual a posicao em bytes (que é o tamanho total do arquivo), e
   dividir pelo tamanho de um registro (sizeof(Cliente)).
   Cai em prova: calcular quantidade de registros sem percorrer o arquivo.
   ------------------------------------------------------------------------- */
void contarRegistros(void)
{
    FILE *fp;
    long tamanhoBytes;     /* tamanho total do arquivo em bytes */
    long qtdRegistros;     /* numero de registros (tamanho / sizeof) */

    fp = fopen(NOME_ARQUIVO, "rb");
    if (fp == NULL)
    {
        printf("Arquivo nao encontrado. Crie-o primeiro (opcao 1).\n");
        system("pause");
        return;
    }

    /* fseek com SEEK_END move o cursor para o FIM do arquivo.
       O segundo parametro (0) é o offset a partir da origem; como queremos
       exatamente o fim, offset = 0. */
    fseek(fp, 0L, SEEK_END);

    /* ftell devolve a posicao ATUAL do cursor em bytes. Como movemos para o
       fim, ftell devolve o TAMANHO TOTAL do arquivo em bytes. */
    tamanhoBytes = ftell(fp);

    /* Cada registro ocupa sizeof(Cliente) bytes. Dividindo o tamanho total
       pelo tamanho de um registro, obtemos a quantidade de registros.
       É uma divisao inteira (long / size_t -> long). Se o arquivo estiver
       vazio, tamanhoBytes = 0 e qtdRegistros = 0. */
    qtdRegistros = tamanhoBytes / (long) sizeof(Cliente);

    printf("\n--- CONTAGEM DE REGISTROS ---\n");
    printf("Arquivo: %s\n", NOME_ARQUIVO);
    printf("Tamanho total: %ld bytes\n", tamanhoBytes);
    printf("Tamanho de cada registro (sizeof Cliente): %lu bytes\n",
           (unsigned long) sizeof(Cliente));
    printf("Quantidade de registros: %ld\n", qtdRegistros);

    fclose(fp);
    system("pause");
}

/* -------------------------------------------------------------------------
   FUNCAO: main
   Menu principal com switch dentro de do-while. Estilo do professor:
   system("cls") para limpar a tela, getche() para ler a opcao de uma tecla,
   system("pause") para pausar antes de voltar ao menu.
   ------------------------------------------------------------------------- */
int main(void)
{
    char opcao;   /* recebe a tecla digitada pelo usuario (getche le 1 char) */

    do
    {
        system("cls");   /* limpa a tela (Windows). Equivalente a 'clear' no Linux */

        printf("============================================\n");
        printf("   CRUD DE CLIENTES - ARQUIVO BINARIO\n");
        printf("============================================\n");
        printf(" 1 - Criar arquivo (wb)\n");
        printf(" 2 - Cadastrar cliente (ab + fwrite)\n");
        printf(" 3 - Listar todos (rb + fread==1)\n");
        printf(" 4 - Buscar por codigo (fseek acesso direto)\n");
        printf(" 5 - Alterar (rb + wb temp + rename)\n");
        printf(" 6 - Excluir (rb + wb temp + rename)\n");
        printf(" 7 - Contar registros (fseek END + ftell)\n");
        printf(" 0 - Sair\n");
        printf("============================================\n");
        printf("Opcao: ");

        /* getche le UM caractere do teclado e o ECOA (mostra na tela).
           Retorna o caractere digitado. Nao precisa dar Enter.
           getch faz o mesmo mas NAO ecoa (usado para senhas). */
        opcao = getche();

        /* switch direciona para a funcao correspondente a opcao escolhida.
           Cada case chama uma funcao modularizada (boa pratica de separacao). */
        switch (opcao)
        {
            case '1':
                criarArquivo();
                break;
            case '2':
                cadastrar();
                break;
            case '3':
                listarTodos();
                break;
            case '4':
                buscarPorCodigo();
                break;
            case '5':
                alterar();
                break;
            case '6':
                excluir();
                break;
            case '7':
                contarRegistros();
                break;
            case '0':
                printf("\nSaindo do programa... Ate logo!\n");
                break;
            default:
                /* opcao invalida: qualquer tecla fora de 0-7 cai aqui */
                printf("\nOpcao invalida! Escolha entre 0 e 7.\n");
                system("pause");
                break;
        }
    }
    while (opcao != '0');   /* repete o menu ate digitar 0 para sair */

    return 0;   /* fim normal do programa (EXIT_SUCCESS implicito) */
}

/*
============================================================================
 RESUMO DOS PADROES DE ARQUIVO BINARIO DEMONSTRADOS NESTE PROGRAMA:
 ----------------------------------------------------------------------------
 1. Criar/zerar arquivo ............ fopen("wb") ............ opcao 1
 2. Anexar registro no fim ......... fopen("ab") + fwrite ... opcao 2
 3. Leitura sequencial correta .... fopen("rb") + fread==1 . opcao 3
 4. Acesso direto por offset ....... fseek + SEEK_SET ....... opcao 4
 5. Alterar via temporario ......... fopen("rb")+"wb"+rename opcao 5
 6. Excluir via temporario ......... fopen("rb")+"wb"+rename opcao 6
 7. Contar registros sem percorrer . fseek SEEK_END + ftell  opcao 7
 ----------------------------------------------------------------------------
 PEGADINHAS DE PROVA COBERTAS:
 - NUNCA usar while(!feof(fp)) com fread antes do loop (duplica o ultimo).
   Usar while(fread(&r, sizeof(r), 1, fp) == 1).
 - NUNCA usar gets() (inseguro). Usar fgets + strcspn para tirar o '\n'.
 - Sempre testar if(fp==NULL) apos fopen.
 - fflush(stdin) funciona no Windows mas NAO é portatil (padrao C = indefinido).
 - typedef struct permite declarar "Cliente c;" em vez de "struct Cliente c;".
 - No Windows, fechar o arquivo ANTES de remove()/rename() (senao da erro).
 - sizeof(Cliente) é o tamanho FIXO de cada registro (base do acesso direto).
============================================================================
*/
