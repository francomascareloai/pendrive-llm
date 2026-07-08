/*
 * ============================================================================
 * ARQUIVO: 19_crud_binario_struct.c
 *
 * O QUE FAZ: CRUD (Create-Read-Update) de funcionarios em arquivo BINARIO
 *            usando struct. Menu com opcoes A-F.
 *
 * TOPICO COBERTO: Arquivos binarios + struct + fwrite/fread + padrao
 *                 temp.dat + rename para alteracao.
 *
 * PALAVRAS-CHAVE (Ctrl-F): crud, binario, struct, fwrite, fread, fopen,
 *   fclose, wb, rb, ab, append, rename, remove, temp.dat, funcionario.dat,
 *   arquivo binario, duplicidade, alterar salario, relatorio, fseek nao usado,
 *   typedef struct, fgets, strcspn, fflush stdin, menu switch, getche, conio
 *
 * MENU:
 *   A - Criar arquivo (wb)         -> cria/trunca funcionario.dat
 *   B - Cadastrar (rb + ab)        -> checa duplicidade, depois append
 *   C - Alterar Salario (rb+wb)    -> padrao temp.dat + rename
 *   D - Relatorio (rb)             -> le e imprime todos
 *   F - Sair
 *
 * COMPILAR: gcc -Wall -Wextra 19_crud_binario_struct.c -o programa
 *           (conio.h funciona no MinGW/Windows)
 *
 * BUGS CORRIGIDOS EM RELACAO AO CODIGO ORIGINAL DO PROFESSOR:
 *   1. gets() removido -> trocado por fgets + strcspn (gets foi excluido do C)
 *   2. printf "%03    |" (faltava 'd') -> corrigido para "%03d |"
 *   3. Adicionado default no switch e mensagens de feedback
 *   4. Adicionada verificacao de arquivo vazio no relatorio
 *   5. fflush(stdin) mantido (estilo do professor) com comentario explicando
 *      que eh indefinido no padrao C mas funciona no Windows
 *   6. fread no while com == 1 (forma correta, sem feof)
 * ============================================================================
 */

#include <stdio.h>    // fopen, fclose, fread, fwrite, printf, scanf, fgets
#include <stdlib.h>   // system, remove, rename
#include <conio.h>    // getche (le tecla com eco) - Windows/MinGW
#include <string.h>  // strcspn (para remover o \n do fgets)
#include <ctype.h>    // toupper (converte letra para maiuscula)

#define max 80  // limite generico (nao usado neste programa, mas estilo do professor)

/* -------------------------------------------------------------------------- *
 * STRUCT Funcionario
 *
 * CORRECAO DE BUG #1: O professor as vezes escrevia
 *   "typedef struct nome { ... };"  (SEM alias)
 * o que NAO compila em C puro. A forma correta eh:
 *   typedef struct { ... } Nome;  (struct anonima + alias no final)
 *
 * Assim podemos declarar "Funcionario func;" diretamente, sem "struct".
 * -------------------------------------------------------------------------- */
typedef struct {
    int   codigo;    /* codigo unico do funcionario (chave primaria) */
    char  nome[30];  /* nome do funcionario (29 chars + '\0')        */
    float salario;   /* salario atual do funcionario                 */
} Funcionario;       /* alias: agora "Funcionario" eh um tipo        */


/* ========================================================================== *
 * FUNCAO: opcao_menu
 * Limpa a tela, mostra o menu e retorna a tecla pressionada (maiuscula).
 * getche() le 1 caractere do teclado COM eco (mostra na tela).
 * toupper() converte para maiuscula -> nao precisa testar 'a' e 'A'.
 * ========================================================================== */
char opcao_menu(void) {
    system("cls");  /* limpa a tela (Windows) */
    printf("(A) Iniciar o arquivo\n");
    printf("(B) Cadastrar\n");
    printf("(C) Alterar Salario\n");
    printf("(D) Relatorio geral\n");
    printf("(F) Sair\n");
    return (char)toupper(getche());  /* le tecla, converte p/ maiuscula */
}


/* ========================================================================== *
 * FUNCAO: cria_arquivo  (opcao A)
 *
 * Cria (ou ZERA) o arquivo funcionario.dat.
 * fopen com "wb" = Write Binary:
 *   - Se o arquivo NAO existe: cria um novo (vazio).
 *   - Se o arquivo JA existe: TRUNCA (apaga todo o conteudo anterior).
 *
 * CUIDADO: chamar esta opcao depois de cadastrar apaga tudo!
 * ========================================================================== */
void cria_arquivo(void) {
    FILE *arq;
    system("cls");
    arq = fopen("funcionario.dat", "wb");  /* wb = write binary (cria/trunca) */
    if (arq == NULL) {                      /* SEMPRE testar se fopen falhou */
        printf("Erro ao criar o arquivo!\n");
        return;                             /* sai da funcao, nao continua */
    }
    printf("Arquivo de funcionarios criado com sucesso.\n");
    fclose(arq);  /* fecha o arquivo (neste ponto esta vazio) */
}


/* ========================================================================== *
 * FUNCAO: cadastra_funcionario  (opcao B)
 *
 * Le os dados de UM funcionario, verifica se o codigo ja existe (duplicidade)
 * e, se nao existir, grava no final do arquivo (append).
 *
 * FLUXO:
 *   1. Abre funcionario.dat em "rb" (read binary) para checar duplicidade.
 *   2. Le o codigo digitado pelo usuario.
 *   3. Percorre o arquivo lendo struct por struct com fread.
 *   4. Se achar o mesmo codigo -> aborta (nao cadastra duplicata).
 *   5. Fecha o arquivo (modo leitura).
 *   6. Le nome e salario.
 *   7. Reabre em "ab" (append binary) e fwrite no final.
 *
 * PORQUE abrir 2 vezes (rb depois ab)?
 *   - "rb" so permite ler. "ab" so permite escrever no final.
 *   - Nao da para checar duplicidade e append ao mesmo tempo com um fopen so.
 *   - Por isso abrimos 2 vezes: 1x para ler (rb), 1x para append (ab).
 * ========================================================================== */
void cadastra_funcionario(void) {
    FILE *arq;
    Funcionario func;  /* struct que sera preenchida e gravada */
    Funcionario tmp;   /* struct temporaria para ler cada registro do arquivo */

    system("cls");

    /* --- PASSO 1: abrir para leitura (rb) e checar duplicidade --- */
    arq = fopen("funcionario.dat", "rb");  /* rb = read binary */
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        printf("Dica: use a opcao (A) para criar o arquivo primeiro.\n");
        return;
    }

    printf("Digite os dados do funcionario:\n");
    printf("Codigo: ");
    scanf("%d", &func.codigo);  /* le inteiro -> SEMPRE com & (endereco) */

    /* --- PASSO 3: percorre o arquivo procurando codigo duplicado --- */
    /*
     * CORRECAO DE BUG #2: O professor usava "while(!feof(arq))" com fread
     * ANTES do loop. Isso fazia o ULTIMO registro ser lido 2x (imprimia lixo).
     *
     * A forma CORRETA eh: while(fread(...) == 1) { ... }
     *   - fread retorna o numero de itens lidos (1 se leu, 0 se fim).
     *   - Assim o loop so continua se realmente leu um registro.
     *   - NAO precisa de feof nem de fread antes do while.
     */
    while (fread(&tmp, sizeof(Funcionario), 1, arq) == 1) {
        if (tmp.codigo == func.codigo) {
            /* codigo eh int -> pode comparar com == (strings usariam strcmp) */
            printf("Funcionario ja cadastrado!\n");
            fclose(arq);
            return;  /* nao cadastra duplicata */
        }
    }
    fclose(arq);  /* fecha o modo leitura */

    /* --- PASSO 6: ler os dados restantes (nome e salario) --- */
    printf("Nome: ");
    fflush(stdin);  /*
                      * Limpa o '\n' que o scanf deixou no buffer de entrada.
                      *
                      * CUIDADO: fflush(stdin) eh INDEFINIDO no padrao C!
                      * So funciona no Windows (MSVC e MinGW). Em Linux/Mac
                      * nao limpa nada. Mantido aqui porque eh o estilo do
                      * professor e a prova usa Windows.
                      * A forma portatvel seria uma funcao limparBuffer()
                      * com while((c=getchar())!='\n' && c!=EOF);
                      */

    /*
     * CORRECAO DE BUG #3: gets() foi REMOVIDO do C (a partir do C11).
     * Nao existe mais. Usamos fgets + strcspn:
     *   - fgets le ate n-1 caracteres ou ate achar '\n' (inclui o '\n').
     *   - strcspn encontra a posicao do '\n' e trocamos por '\0'.
     */
    fgets(func.nome, sizeof(func.nome), stdin);  /* le o nome (max 29 chars) */
    func.nome[strcspn(func.nome, "\n")] = '\0';  /* remove o '\n' do final */

    printf("Salario inicial: ");
    scanf("%f", &func.salario);  /* float tambem leva & */

    /* --- PASSO 7: reabrir em "ab" (append binary) e gravar --- */
    arq = fopen("funcionario.dat", "ab");  /* ab = append binary (escreve no final) */
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    /*
     * fwrite grava 1 struct inteira (em binario) no arquivo.
     *   &func          = endereco dos dados a gravar
     *   sizeof(...)    = tamanho em bytes de 1 Funcionario
     *   1              = quantidade de itens a gravar
     *   arq            = arquivo onde gravar
     */
    fwrite(&func, sizeof(Funcionario), 1, arq);
    printf("Funcionario cadastrado com sucesso!\n");
    fclose(arq);
}


/* ========================================================================== *
 * FUNCAO: altera_salario  (opcao C)
 *
 * Altera o salario de um funcionario identificado pelo codigo.
 *
 * USA O PADRAO temp.dat + rename (o padrao que CAI na prova):
 *   1. Abre funcionario.dat em "rb" (leitura).
 *   2. Cria temp.dat em "wb" (escrita, arquivo novo vazio).
 *   3. Le cada registro do original com fread.
 *      - Se o codigo bater: mostra o registro, pede novo salario, valida,
 *        atualiza func.salario.
 *      - Grava o registro (alterado OU copiado identico) em temp.dat com fwrite.
 *   4. Fecha ambos os arquivos.
 *   5. Se NAO achou: remove temp.dat (descarta a copia).
 *   6. Se achou: remove funcionario.dat e renomeia temp.dat -> funcionario.dat.
 *
 * PORQUE temp+rename e nao fseek + fwrite direto no arquivo?
 *   - fwrite direto no meio do arquivo exige fseek na posicao exata e que
 *     o registro tenha o MESMO tamanho (o que eh verdade aqui, mas eh facil
 *     errar o fseek).
 *   - O padrao temp+rename eh mais seguro: copia tudo, altera o que precisa,
 *     e troca o arquivo antigo pelo novo. Eh o padrao que o professor ensina.
 * ========================================================================== */
void altera_salario(void) {
    FILE *arq, *tmp;        /* arq = original (rb), tmp = temporario (wb) */
    Funcionario func;       /* registro lido do arquivo a cada iteracao */
    int   codigo;           /* codigo que o usuario quer alterar */
    float novo_salario;     /* novo salario digitado pelo usuario */
    int   achou = 0;        /* flag: 0 = nao achou, 1 = achou */

    system("cls");

    /* --- PASSO 1: abrir original para leitura --- */
    arq = fopen("funcionario.dat", "rb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    /* --- PASSO 2: criar arquivo temporario para escrita --- */
    tmp = fopen("temp.dat", "wb");  /* wb = cria/trunca temp.dat */
    if (tmp == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(arq);  /* nao esquecer de fechar o que ja estava aberto! */
        return;
    }

    printf("Digite o codigo do funcionario: ");
    scanf("%d", &codigo);

    /* --- PASSO 3: percorrer o arquivo original registro por registro --- */
    while (fread(&func, sizeof(Funcionario), 1, arq) == 1) {

        if (func.codigo == codigo) {  /* achou o funcionario procurado */
            achou = 1;  /* marca que encontrou (para nao remover temp.dat depois) */

            /*
             * CORRECAO DE BUG #4: O professor escrevia "%03    |" (faltava
             * o 'd' depois de %03). Sem o 'd', o printf nao sabe que eh
             * inteiro e imprime lixo ou erro.
             * Correto: "%03d" -> inteiro com 3 digitos, zeros a esquerda.
             */
            printf(" %03d | %30s | %.2f\n", func.codigo, func.nome, func.salario);

            printf("Digite o novo salario: ");
            scanf("%f", &novo_salario);

            if (novo_salario < 0) {  /* validacao: salario nao pode ser negativo */
                printf("Impossivel salario negativo.\n");
                fclose(arq);
                fclose(tmp);
                remove("temp.dat");  /* descarta o temporario (parcial) */
                return;              /* aborta a alteracao */
            }

            func.salario = novo_salario;  /* atualiza o salario na struct (em memoria) */
        }

        /*
         * Escreve o registro em temp.dat.
         * IMPORTANTE: isso executa SEMPRE, tenha ou nao alterado o registro.
         * Assim temp.dat recebe TODOS os registros (alterados e nao alterados).
         * Se colocassemos este fwrite DENTRO do if, so gravaria 1 registro!
         */
        fwrite(&func, sizeof(Funcionario), 1, tmp);
    }

    fclose(arq);
    fclose(tmp);

    /* --- PASSO 5/6: decidir o que fazer com temp.dat --- */
    if (!achou) {  /* percorreu tudo e nao encontrou o codigo */
        printf("Funcionario nao encontrado.\n");
        remove("temp.dat");  /* descarta temp.dat (copia identica do original) */
        return;
    }

    /*
     * Achou e alterou: troca o arquivo original pelo temporario.
     *   remove("funcionario.dat")  -> apaga o original
     *   rename("temp.dat", ...)     -> temp.dat passa a ser funcionario.dat
     *
     * PORQUE remover antes de renomear?
     *   No Windows, rename FALHA se o arquivo de destino ja existe.
     *   Por isso removemos funcionario.dat ANTES de renomear.
     */
    remove("funcionario.dat");
    rename("temp.dat", "funcionario.dat");
    printf("Salario alterado com sucesso!\n");
}


/* ========================================================================== *
 * FUNCAO: mostra_tudo  (opcao D)
 *
 * Le e imprime TODOS os funcionarios cadastrados (relatorio geral).
 * Simplesmente percorre o arquivo com fread ate o fim.
 * ========================================================================== */
void mostra_tudo(void) {
    FILE *arq;
    Funcionario func;
    int count = 0;  /* contador de registros impressos */

    system("cls");
    arq = fopen("funcionario.dat", "rb");  /* rb = read binary */
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("=== RELATORIO DE FUNCIONARIOS ===\n");
    printf(" Cod   | Nome                           | Salario\n");
    printf("-------|--------------------------------|----------\n");

    /*
     * Le registro por registro ate o fim do arquivo.
     * fread retorna 1 enquanto conseguir ler, 0 quando chegar ao fim.
     */
    while (fread(&func, sizeof(Funcionario), 1, arq) == 1) {
        /*
         * Formatos de printf:
         *   %03d  -> inteiro com 3 casas, zeros a esquerda (ex: 007)
         *   %-30s -> string em campo de 30 caracteres, alinhada a ESQUERDA
         *            (o sinal - alinha a esquerda; sem o - alinha a direita)
         *   %.2f  -> float com 2 casas decimais
         */
        printf(" %03d   | %-30s | %.2f\n", func.codigo, func.nome, func.salario);
        count++;
    }

    if (count == 0) {
        printf("(nenhum funcionario cadastrado)\n");
    } else {
        printf("-------|--------------------------------|----------\n");
        printf("Total: %d funcionario(s)\n", count);
    }

    fclose(arq);
}


/* ========================================================================== *
 * FUNCAO: main
 *
 * Loop principal com menu switch. So sai quando o usuario digita 'F'.
 *
 * CORRECAO DE BUG #5: main deve ter tipo de retorno int e return 0 no fim.
 *                      (O professor ja tinha, mas alguns codigos nao tem.)
 * ========================================================================== */
int main(void) {
    char op;  /* opcao do menu escolhida pelo usuario */

    do {
        op = opcao_menu();  /* mostra menu e le a tecla */

        switch (op) {
            case 'A': cria_arquivo();          break;
            case 'B': cadastra_funcionario();   break;
            case 'C': altera_salario();        break;
            case 'D': mostra_tudo();           break;
            case 'F': /* nada - vai sair do loop */ break;
            default:  printf("Opcao invalida!\n"); break;
        }

        printf("\n");
        getche();  /* pausa para o usuario ler o resultado antes de limpar a tela */

    } while (op != 'F');  /* repete enquanto nao escolher F (sair) */

    return 0;  /* retorno padrao do main (0 = sucesso) */
}
