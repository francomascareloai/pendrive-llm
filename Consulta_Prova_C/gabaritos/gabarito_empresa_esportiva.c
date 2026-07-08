/*
 * ============================================================================
 * GABARITO - EMPRESA ESPORTIVA (Material Esportivo)
 * ============================================================================
 * O QUE FAZ: Sistema de cadastro de filiais e funcionarios (gerente +
 *            vendedores) de uma empresa de material esportivo. Permite
 *            cadastrar e consultar filiais e funcionarios, com varias
 *            validacoes. Tudo em memoria (vetores de struct, sem arquivo).
 *
 * TOPICO: struct, strings, strcmp, funcoes/modularizacao, vetores de struct
 *
 * PALAVRAS-CHAVE (Ctrl+F): struct, typedef struct, fgets, strcspn, strcmp,
 *   pegadinha strcmp, if(strcmp), ==0, fflush stdin, conio.h, getche,
 *   system cls, system pause, menu switch do-while, modularizacao, funcao,
 *   validacao, filial duplicada, filial lotada, funcionario duplicado,
 *   filial nao cadastrada, filial sem vagas, filial ja tem gerente,
 *   filial ja tem 4 vendedores, cadastro vazio, gerente, vendedor,
 *   total salario, total geral
 *
 * COMPILACAO: gcc -Wall -Wextra gabarito_empresa_esportiva.c -o empresa
 * ============================================================================
 */

#include <stdio.h>   // printf, scanf, fgets, etc.
#include <stdlib.h>  // system (cls, pause)
#include <string.h>  // strcmp, strcspn, strcpy
#include <conio.h>   // getche, getch (estilo do professor - Windows/MinGW)

/* ========================================================================== */
/* CONSTANTES DO SISTEMA                                                      */
/* ========================================================================== */

#define MAX_FILIAIS 4            // Empresa tem no maximo 4 filiais
#define MAX_FUNC_POR_FILIAL 5    // 1 gerente + 4 vendedores = 5 por filial
#define MAX_VENDEDORES 4         // Limite de vendedores por filial
#define MAX_GERENTES 1            // So pode haver 1 gerente por filial
#define TOTAL_FUNCIONARIOS (MAX_FILIAIS * MAX_FUNC_POR_FILIAL)  // 4*5 = 20

/* ========================================================================== */
/* DEFINICAO DAS STRUCTS (typedef struct correto - C puro, NAO C++)           */
/*                                                                            */
/* PEGADINHA DE PROVA: typedef struct { ... } Nome;                            */
/*  - "typedef struct" cria um APELIDO para o tipo, evitando escrever          */
/*    "struct Nome" toda vez. Basta usar "Nome".                               */
/*  - Errado (C++): so "struct Nome { ... };" sem typedef (funciona em C++    */
/*    mas NAO e o estilo do professor de C).                                  */
/* ========================================================================== */

// Struct da Filial: identifica a loja pelo numero e nome
typedef struct {
    int numero;      // Numero identificador da filial (ex: 1, 2, 3, 4)
    char nome[50];   // Nome da filial (ex: "Filial Centro")
} Filial;

// Struct do Funcionario: dados de cada trabalhador vinculado a uma filial
typedef struct {
    int numero_filial;  // FK: a qual filial esse funcionario pertence
    int codigo;         // Codigo unico do funcionario (chave primaria)
    char nome[50];      // Nome do funcionario
    char cargo[20];     // Cargo: "Gerente" ou "Vendedor"
    float salario;      // Salario do funcionario (R$)
} Funcionario;

/* ========================================================================== */
/* VARIAVEIS GLOBAIS (estilo do professor - vetores em memoria)              */
/*                                                                            */
/* O professor usa variaveis globais para simplificar o codigo didatico:      */
/* assim nao precisa passar os vetores por parametro em toda funcao.         */
/* Em projetos reais, prefira passar por parametro ou usar ponteiros.        */
/* ========================================================================== */

Filial filiais[MAX_FILIAIS];                  // Vetor de filiais (max 4)
Funcionario funcionarios[TOTAL_FUNCIONARIOS];  // Vetor de funcionarios (max 20)
int qtdFiliais = 0;       // Contador de filiais cadastradas
int qtdFuncionarios = 0;  // Contador de funcionarios cadastrados

/* ========================================================================== */
/* PROTOTIPOS DAS FUNCOES (modularizacao)                                    */
/* Declarar os prototipos antes do main() permite que o main chame qualquer  */
/* funcao mesmo que ela seja definida depois no arquivo.                     */
/* ========================================================================== */

void cadastrarFilial(void);
void cadastrarFuncionario(void);
void consultarFiliais(void);
void consultarFuncionarios(void);
int buscarFilial(int numero);
int buscarFuncionario(int codigo);
int contarFuncionariosPorFilial(int numeroFilial);
int contarGerentesPorFilial(int numeroFilial);
int contarVendedoresPorFilial(int numeroFilial);
float totalSalarioPorFilial(int numeroFilial);

/* ========================================================================== */
/* FUNCAO PRINCIPAL (main)                                                    */
/* Menu com switch dentro de do-while - estilo do professor.                 */
/* ========================================================================== */

int main(void) {
    int opcao;  // Opcao do menu escolhida pelo usuario

    do {
        system("cls");  // Limpa a tela (Windows) - estilo do professor
        printf("=== EMPRESA ESPORTIVA - SISTEMA DE CADASTRO ===\n\n");
        printf("1 - Cadastrar Filial\n");
        printf("2 - Cadastrar Funcionario\n");
        printf("3 - Consultar Filiais\n");
        printf("4 - Consultar Funcionarios\n");
        printf("5 - Sair\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        /*
         * fflush(stdin): limpa o buffer do teclado apos o scanf.
         *
         * ATENCAO (cai na teorica): fflush(stdin) NAO e definido pelo padrao C
         * para streams de ENTRADA (so e definido para saida/fflush(stdout)).
         * Porem, no Windows com MinGW (gcc) funciona na pratica, e e o estilo
         * que o professor usa. Em Linux/Mac, fflush(stdin) nao faz nada.
         * A alternativa portavel seria: while ((c = getchar()) != '\n' && c != EOF);
         */
        fflush(stdin);

        switch (opcao) {
            case 1:
                cadastrarFilial();   // Modulo 1
                break;
            case 2:
                cadastrarFuncionario();  // Modulo 2
                break;
            case 3:
                consultarFiliais();     // Modulo 3
                break;
            case 4:
                consultarFuncionarios(); // Modulo 4
                break;
            case 5:
                printf("\nSaindo do sistema... Ate logo!\n");
                break;
            default:
                // Opcao invalida: avisa e espera uma tecla
                printf("\nOpcao invalida! Pressione qualquer tecla...\n");
                getche();  // Pausa ate pressionar uma tecla (conio.h)
                break;
        }
    } while (opcao != 5);  // Repete o menu ate escolher Sair (opcao 5)

    return 0;  // Fim do programa com sucesso
}

/* ========================================================================== */
/* FUNCOES AUXILIARES (modularizacao - reaproveitamento de codigo)           */
/* Estas funcoes sao usadas pelos modulos de cadastro e consulta para evitar */
/* repeticao de logica.                                                       */
/* ========================================================================== */

/*
 * FUNCAO: buscarFilial
 * Procura uma filial pelo numero no vetor de filiais.
 * Retorna o INDICE no vetor se encontrar, ou -1 se nao existir.
 * Usada para validar: filial duplicada (cadastro) e filial existente.
 */
int buscarFilial(int numero) {
    int i;  // Declarada no inicio (estilo C classico, evita warning)
    for (i = 0; i < qtdFiliais; i++) {
        if (filiais[i].numero == numero) {
            return i;  // Encontrou: retorna o indice
        }
    }
    return -1;  // Percorreu tudo e nao encontrou
}

/*
 * FUNCAO: buscarFuncionario
 * Procura um funcionario pelo codigo (busca GLOBAL, em todas as filiais).
 * Retorna o indice ou -1 se nao encontrar.
 * Usada para validar: funcionario duplicado.
 */
int buscarFuncionario(int codigo) {
    int i;
    for (i = 0; i < qtdFuncionarios; i++) {
        if (funcionarios[i].codigo == codigo) {
            return i;  // Encontrou: codigo ja existe
        }
    }
    return -1;  // Codigo nao cadastrado
}

/*
 * FUNCAO: contarFuncionariosPorFilial
 * Conta quantos funcionarios (gerente + vendedores) estao vinculados a uma
 * filial especifica. Usada para validar "filial sem vagas" (max 5).
 */
int contarFuncionariosPorFilial(int numeroFilial) {
    int i;
    int contador = 0;
    for (i = 0; i < qtdFuncionarios; i++) {
        if (funcionarios[i].numero_filial == numeroFilial) {
            contador++;  // Pertence a filial procurada
        }
    }
    return contador;
}

/*
 * FUNCAO: contarGerentesPorFilial
 * Conta quantos gerentes uma filial ja tem. Maximo permitido: 1.
 *
 * PEGADINHA DE PROVA (strcmp):
 *   strcmp(a, b) retorna 0 (ZERO) quando as strings sao IGUAIS.
 *   Errado: if (strcmp(a, b))        -> entra quando sao DIFERENTES (!=0 e verdadeiro)
 *   Certo: if (strcmp(a, b) == 0)    -> entra quando sao IGUAIS
 *   Tambem certo: if (!strcmp(a, b)) -> !0 = 1 (verdadeiro) quando iguais
 */
int contarGerentesPorFilial(int numeroFilial) {
    int i;
    int contador = 0;
    for (i = 0; i < qtdFuncionarios; i++) {
        if (funcionarios[i].numero_filial == numeroFilial &&
            strcmp(funcionarios[i].cargo, "Gerente") == 0) {
            // strcmp == 0 significa que o cargo E "Gerente" (strings iguais)
            contador++;
        }
    }
    return contador;
}

/*
 * FUNCAO: contarVendedoresPorFilial
 * Conta quantos vendedores uma filial ja tem. Maximo permitido: 4.
 */
int contarVendedoresPorFilial(int numeroFilial) {
    int i;
    int contador = 0;
    for (i = 0; i < qtdFuncionarios; i++) {
        if (funcionarios[i].numero_filial == numeroFilial &&
            strcmp(funcionarios[i].cargo, "Vendedor") == 0) {
            // Mesma logica: strcmp == 0 quando strings sao iguais
            contador++;
        }
    }
    return contador;
}

/*
 * FUNCAO: totalSalarioPorFilial
 * Soma o salario de todos os funcionarios de uma filial.
 * Retorna o valor total (float) para exibir na consulta.
 */
float totalSalarioPorFilial(int numeroFilial) {
    int i;
    float total = 0.0;  // Acumulador: comeca em zero
    for (i = 0; i < qtdFuncionarios; i++) {
        if (funcionarios[i].numero_filial == numeroFilial) {
            total += funcionarios[i].salario;  // Soma o salario
        }
    }
    return total;
}

/* ========================================================================== */
/* MODULO 1: cadastrarFilial                                                 */
/* Cadastra uma nova filial no vetor, com validacoes.                         */
/* Validacoes: filial lotada (>4 filiais) e filial duplicada (numero repetido)*/
/* ========================================================================== */

void cadastrarFilial(void) {
    system("cls");  // Limpa a tela
    printf("=== CADASTRAR FILIAL ===\n\n");

    /*
     * VALIDACAO 1: "filial lotada (>4)"
     * Se ja existem 4 filiais cadastradas (MAX_FILIAIS), nao pode cadastrar
     * a 5a. O ">4" significa: tentar ultrapassar o limite de 4 filiais.
     */
    if (qtdFiliais >= MAX_FILIAIS) {
        printf("ERRO: Limite de filiais atingido (%d)! Nao e possivel cadastrar mais.\n",
               MAX_FILIAIS);
        printf("\nPressione qualquer tecla para voltar...\n");
        getche();  // Pausa (conio.h)
        return;    // Sai da funcao sem cadastrar (return em void nao retorna valor)
    }

    Filial novaFilial;  // Variavel LOCAL do tipo struct Filial (typedef)

    printf("Digite o numero da filial: ");
    scanf("%d", &novaFilial.numero);  // Le o numero (int)
    fflush(stdin);  // Limpa o \n deixado pelo scanf no buffer

    /*
     * VALIDACAO 2: "filial duplicada"
     * Verifica se ja existe uma filial com o mesmo numero.
     * buscarFilial retorna -1 se nao encontrar (pode cadastrar).
     * Se retornar diferente de -1, ja existe (nao pode cadastrar).
     */
    if (buscarFilial(novaFilial.numero) != -1) {
        printf("\nERRO: Filial duplicada! Ja existe uma filial com o numero %d.\n",
               novaFilial.numero);
        printf("\nPressione qualquer tecla para voltar...\n");
        getche();
        return;
    }

    printf("Digite o nome da filial: ");
    /*
     * fgets: le a string com SEGURANCA (limite de caracteres definido).
     * NUNCA usar gets() - foi removido do padrao C11 por causar buffer overflow
     * (nao verifica o tamanho do destino). O professor pode pedir fgets na prova.
     *
     * Parametros: fgets(destino, tamanho_maximo, origem)
     *   - destino: novaFilial.nome (o vetor de char)
     *   - tamanho_maximo: sizeof(novaFilial.nome) = 50 (tamanho do vetor)
     *   - origem: stdin (teclado)
     */
    fgets(novaFilial.nome, sizeof(novaFilial.nome), stdin);

    /*
     * strcspn: encontra a posicao do caractere '\n' (Enter) na string.
     * O fgets captura o Enter como '\n' no final da string. Precisamos
     * substituir o '\n' por '\0' (fim de string) para a string ficar correta.
     *
     * Exemplo: "Filial Centro\n" -> strcspn encontra o \n na posicao 13 ->
     *          substitui por \0 -> "Filial Centro"
     *
     * PEGADINHA: se nao fizer isso, o nome fica com um \n no final e pode
     * causar problemas na exibicao e nas comparacoes com strcmp.
     */
    novaFilial.nome[strcspn(novaFilial.nome, "\n")] = '\0';

    // Salva no vetor global e incrementa o contador
    filiais[qtdFiliais] = novaFilial;  // Copia a struct inteira para o vetor
    qtdFiliais++;                      // Incrementa o contador de filiais

    printf("\nFilial cadastrada com sucesso!\n");
    printf("Numero: %d | Nome: %s\n", novaFilial.numero, novaFilial.nome);
    printf("\nPressione qualquer tecla para voltar...\n");
    getche();
}

/* ========================================================================== */
/* MODULO 2: cadastrarFuncionario                                            */
/* Cadastra um novo funcionario no vetor, com varias validacoes.             */
/* ========================================================================== */

void cadastrarFuncionario(void) {
    system("cls");
    printf("=== CADASTRAR FUNCIONARIO ===\n\n");

    /*
     * VALIDACAO 1: "cadastro vazio"
     * Nao faz sentido cadastrar funcionario se nenhuma filial existe.
     * O funcionario precisa estar vinculado a uma filial.
     */
    if (qtdFiliais == 0) {
        printf("ERRO: Nenhuma filial cadastrada! Cadastre uma filial primeiro.\n");
        printf("\nPressione qualquer tecla para voltar...\n");
        getche();
        return;
    }

    Funcionario novoFunc;  // Variavel LOCAL do tipo struct Funcionario

    // Lista as filiais disponiveis para o usuario saber quais existem
    printf("Filiais disponiveis:\n");
    int i;
    for (i = 0; i < qtdFiliais; i++) {
        printf("  Filial %d - %s\n", filiais[i].numero, filiais[i].nome);
    }
    printf("\n");

    printf("Digite o numero da filial para o funcionario: ");
    scanf("%d", &novoFunc.numero_filial);  // Le o numero da filial (FK)
    fflush(stdin);

    /*
     * VALIDACAO 2: "filial nao cadastrada"
     * O numero digitado nao corresponde a nenhuma filial existente.
     * buscarFilial retorna -1 quando nao encontra.
     */
    if (buscarFilial(novoFunc.numero_filial) == -1) {
        printf("\nERRO: Filial nao cadastrada! O numero %d nao existe.\n",
               novoFunc.numero_filial);
        printf("\nPressione qualquer tecla para voltar...\n");
        getche();
        return;
    }

    /*
     * VALIDACAO 3: "filial sem vagas (>5)"
     * A filial suporta no maximo 5 funcionarios (1 gerente + 4 vendedores).
     * Se ja tem 5, nao cabe mais ninguem. O ">5" significa tentar passar de 5.
     */
    int qtdNaFilial = contarFuncionariosPorFilial(novoFunc.numero_filial);
    if (qtdNaFilial >= MAX_FUNC_POR_FILIAL) {
        printf("\nERRO: Filial sem vagas! A filial %d ja tem %d funcionarios (maximo %d).\n",
               novoFunc.numero_filial, qtdNaFilial, MAX_FUNC_POR_FILIAL);
        printf("\nPressione qualquer tecla para voltar...\n");
        getche();
        return;
    }

    printf("Digite o codigo do funcionario: ");
    scanf("%d", &novoFunc.codigo);  // Le o codigo (int)
    fflush(stdin);

    /*
     * VALIDACAO 4: "funcionario duplicado"
     * O codigo do funcionario deve ser unico em toda a empresa (busca global).
     * buscarFuncionario retorna -1 se o codigo nao existe (pode cadastrar).
     */
    if (buscarFuncionario(novoFunc.codigo) != -1) {
        printf("\nERRO: Funcionario duplicado! Ja existe um funcionario com o codigo %d.\n",
               novoFunc.codigo);
        printf("\nPressione qualquer tecla para voltar...\n");
        getche();
        return;
    }

    printf("Digite o nome do funcionario: ");
    fgets(novoFunc.nome, sizeof(novoFunc.nome), stdin);  // Le o nome com seguranca
    novoFunc.nome[strcspn(novoFunc.nome, "\n")] = '\0';  // Remove o \n do final

    printf("Digite o cargo (Gerente ou Vendedor): ");
    fgets(novoFunc.cargo, sizeof(novoFunc.cargo), stdin);  // Le o cargo
    novoFunc.cargo[strcspn(novoFunc.cargo, "\n")] = '\0';  // Remove o \n

    /*
     * VALIDACAO 5: "filial ja tem gerente"
     * Se o cargo for "Gerente", verifica se a filial ja tem um gerente.
     * So e permitido 1 gerente por filial (MAX_GERENTES = 1).
     *
     * PEGADINHA DE PROVA (strcmp):
     *   strcmp(str1, str2) == 0  -> strings IGUAIS (entre no if)
     *   strcmp(str1, str2)       -> valor != 0 se DIFERENTES (nao entre)
     *   if (strcmp(a,b))        -> ERRADO! Entra quando sao DIFERENTES.
     *   if (strcmp(a,b) == 0)   -> CERTO! Entra quando sao IGUAIS.
     */
    if (strcmp(novoFunc.cargo, "Gerente") == 0) {
        if (contarGerentesPorFilial(novoFunc.numero_filial) >= MAX_GERENTES) {
            printf("\nERRO: A filial %d ja tem um gerente! So e permitido %d gerente por filial.\n",
                   novoFunc.numero_filial, MAX_GERENTES);
            printf("\nPressione qualquer tecla para voltar...\n");
            getche();
            return;
        }
    }

    /*
     * VALIDACAO 6: "filial ja tem 4 vendedores"
     * Se o cargo for "Vendedor", verifica se a filial ja tem 4 vendedores.
     * So sao permitidos 4 vendedores por filial (MAX_VENDEDORES = 4).
     */
    if (strcmp(novoFunc.cargo, "Vendedor") == 0) {
        if (contarVendedoresPorFilial(novoFunc.numero_filial) >= MAX_VENDEDORES) {
            printf("\nERRO: A filial %d ja tem %d vendedores! Limite atingido.\n",
                   novoFunc.numero_filial, MAX_VENDEDORES);
            printf("\nPressione qualquer tecla para voltar...\n");
            getche();
            return;
        }
    }

    printf("Digite o salario: ");
    scanf("%f", &novoFunc.salario);  // Le o salario (float)
    fflush(stdin);

    // Salva no vetor global e incrementa o contador
    funcionarios[qtdFuncionarios] = novoFunc;  // Copia a struct inteira
    qtdFuncionarios++;

    printf("\nFuncionario cadastrado com sucesso!\n");
    printf("Codigo: %d | Nome: %s | Cargo: %s | Salario: R$ %.2f | Filial: %d\n",
           novoFunc.codigo, novoFunc.nome, novoFunc.cargo,
           novoFunc.salario, novoFunc.numero_filial);
    printf("\nPressione qualquer tecla para voltar...\n");
    getche();
}

/* ========================================================================== */
/* MODULO 3: consultarFiliais                                                */
/* Lista todas as filiais cadastradas (numero e nome).                       */
/* ========================================================================== */

void consultarFiliais(void) {
    system("cls");
    printf("=== CONSULTA DE FILIAIS ===\n\n");

    /*
     * VALIDACAO: "cadastro vazio"
     * Se nao ha filiais cadastradas, nao ha o que consultar.
     */
    if (qtdFiliais == 0) {
        printf("Nenhuma filial cadastrada!\n");
        printf("\nPressione qualquer tecla para voltar...\n");
        getche();
        return;
    }

    printf("Total de filiais: %d\n\n", qtdFiliais);
    printf("%-10s %s\n", "NUMERO", "NOME");  // %-10s: alinhado a esquerda, 10 espacos
    printf("-----------------------------------------------\n");

    int i;
    for (i = 0; i < qtdFiliais; i++) {
        // Acessa os campos da struct com o operador ponto (.)
        printf("%-10d %s\n", filiais[i].numero, filiais[i].nome);
    }

    printf("\nPressione qualquer tecla para voltar...\n");
    getche();
}

/* ========================================================================== */
/* MODULO 4: consultarFuncionarios                                           */
/* Para cada filial, mostra: gerente + vendedores, total de salario por      */
/* filial e total geral de salarios de toda a empresa.                       */
/* ========================================================================== */

void consultarFuncionarios(void) {
    system("cls");
    printf("=== CONSULTA DE FUNCIONARIOS ===\n\n");

    /*
     * VALIDACAO: "cadastro vazio"
     * Se nao ha funcionarios cadastrados, nao ha o que consultar.
     */
    if (qtdFuncionarios == 0) {
        printf("Nenhum funcionario cadastrado!\n");
        printf("\nPressione qualquer tecla para voltar...\n");
        getche();
        return;
    }

    float totalGeral = 0.0;  // Acumulador do total de salarios de TODAS as filiais
    int i, j;                // Variaveis de loop declaradas no inicio (estilo C)

    /*
     * Percorre cada filial cadastrada e mostra seus funcionarios.
     * Para cada filial, faz dois loops internos: um para o gerente e outro
     * para os vendedores. Assim a saida fica organizada por cargo.
     */
    for (i = 0; i < qtdFiliais; i++) {
        int numFilial = filiais[i].numero;  // Numero da filial atual

        printf("--- FILIAL %d: %s ---\n", numFilial, filiais[i].nome);

        // Mostra o GERENTE da filial (se houver)
        printf("  Gerente: ");
        int temGerente = 0;  // Flag: 0 = nao tem, 1 = tem
        for (j = 0; j < qtdFuncionarios; j++) {
            if (funcionarios[j].numero_filial == numFilial &&
                strcmp(funcionarios[j].cargo, "Gerente") == 0) {
                // strcmp == 0: cargo E "Gerente" (strings iguais)
                printf("%s (Cod: %d) - Salario: R$ %.2f\n",
                       funcionarios[j].nome, funcionarios[j].codigo,
                       funcionarios[j].salario);
                temGerente = 1;  // Marca que encontrou gerente
            }
        }
        if (!temGerente) {
            // Se percorreu todos e nao achou gerente, avisa
            printf("Nenhum gerente cadastrado\n");
        }

        // Mostra os VENDEDORES da filial
        printf("  Vendedores:\n");
        int temVendedor = 0;  // Flag: 0 = nao tem, 1 = tem
        for (j = 0; j < qtdFuncionarios; j++) {
            if (funcionarios[j].numero_filial == numFilial &&
                strcmp(funcionarios[j].cargo, "Vendedor") == 0) {
                printf("    - %s (Cod: %d) - Salario: R$ %.2f\n",
                       funcionarios[j].nome, funcionarios[j].codigo,
                       funcionarios[j].salario);
                temVendedor = 1;
            }
        }
        if (!temVendedor) {
            printf("    Nenhum vendedor cadastrado\n");
        }

        // Total de salario da filial atual (usa funcao auxiliar)
        float totalFilial = totalSalarioPorFilial(numFilial);
        printf("  TOTAL SALARIO FILIAL: R$ %.2f\n\n", totalFilial);

        totalGeral += totalFilial;  // Acumula no total geral da empresa
    }

    // Mostra o total geral (soma de todas as filiais)
    printf("========================================\n");
    printf("TOTAL GERAL DE SALARIOS: R$ %.2f\n", totalGeral);
    printf("========================================\n");

    printf("\nPressione qualquer tecla para voltar...\n");
    getche();
}

/*
 * ============================================================================
 * RESUMO DAS VALIDACOES IMPLEMENTADAS (para revisao antes da prova):
 * ============================================================================
 * 1. filial lotada (>4)        -> qtdFiliais >= MAX_FILIAIS (4)
 * 2. filial duplicada          -> buscarFilial(numero) != -1
 * 3. cadastro vazio            -> qtdFiliais == 0 ou qtdFuncionarios == 0
 * 4. filial nao cadastrada     -> buscarFilial(numero) == -1
 * 5. filial sem vagas (>5)     -> contarFuncionariosPorFilial >= 5
 * 6. funcionario duplicado     -> buscarFuncionario(codigo) != -1
 * 7. filial ja tem gerente     -> contarGerentesPorFilial >= 1
 * 8. filial ja tem 4 vendedores-> contarVendedoresPorFilial >= 4
 * ============================================================================
 */
