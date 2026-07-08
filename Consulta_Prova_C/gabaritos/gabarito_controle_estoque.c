/*
 ============================================================================
 GABARITO: CONTROLE DE ESTOQUE
 ============================================================================
 O QUE FAZ: Sistema completo de controle de estoque em arquivo binario.
            Cadastro, listagem, busca por codigo, alteracao de preco,
            entrada/saida de estoque, alerta de minimo, relatorio de valor
            total e exclusao de produtos. Tudo modularizado em funcoes.
 TOPICO: struct, arquivos binarios, funcoes/modularizacao, strings, menu
 PALAVRAS-CHAVE Ctrl-F:
   - typedef struct
   - arquivo binario
   - fopen wb ab rb rb+
   - fread == 1
   - fwrite
   - fseek SEEK_CUR
   - fgets strcspn
   - fflush stdin
   - menu do-while switch
   - getche
   - system cls pause
   - excluir registro
   - rename remove
   - relatorio estoque
   - entrada saida estoque
 ============================================================================
*/

#include <stdio.h>   // printf, scanf, fopen, fread, fwrite, fclose, fseek
#include <stdlib.h>  // system, remove, rename
#include <string.h>  // strcspn (remover '\n' do fgets)
#include <conio.h>   // getche, getch (nao portavel, mas funciona no Windows)

// -------------------------------------------------------------------------
// DEFINICAO DA STRUCT COM ALIAS (typedef struct)
// Usar typedef permite declarar "Produto p;" em vez de "struct Produto p;"
// Isso deixa o codigo mais limpo e e o padrao do professor.
// -------------------------------------------------------------------------
typedef struct {
    int   codigo;      // codigo unico do produto (chave primaria)
    char  nome[50];    // nome do produto (string de ate 49 chars + '\0')
    float preco;       // preco unitario do produto
    int   quantidade;  // quantidade atual em estoque
    int   minimo;      // estoque minimo (para alerta de reposicao)
} Produto;

// Nome do arquivo binario usado em todo o programa.
// Usar #define facilita manutencao: se quiser mudar o nome, muda em 1 lugar.
#define ARQUIVO "produtos.dat"

// -------------------------------------------------------------------------
// PROTOTIPOS DAS FUNCOES
// Declarados antes do main para o compilador conhecer as assinaturas.
// Assim o main pode chamar qualquer funcao abaixo dele.
// -------------------------------------------------------------------------
void cadastrar(void);
void listar(void);
void buscarPorCodigo(void);
void alterarPreco(void);
void registrarMovimento(int ehEntrada);
void alertarMinimo(void);
void relatorioValorTotal(void);
void excluir(void);
void limparBufferStdin(void);
void lerString(char *destino, int tamanho);

// =========================================================================
// FUNCAO PRINCIPAL
// =========================================================================
int main(void)
{
    char opcao;  // guarda a opcao digitada pelo usuario no menu

    // Menu do-while: executa pelo menos 1 vez, repete ate digitar '0'
    do {
        system("cls");  // limpa a tela (comando do Windows)

        printf("===== CONTROLE DE ESTOQUE =====\n");
        printf("1. Cadastrar produto\n");
        printf("2. Listar produtos\n");
        printf("3. Buscar por codigo\n");
        printf("4. Alterar preco\n");
        printf("5. Registrar entrada de estoque\n");
        printf("6. Registrar saida de estoque\n");
        printf("7. Alertar produtos abaixo do minimo\n");
        printf("8. Relatorio: valor total do estoque\n");
        printf("9. Excluir produto\n");
        printf("0. Sair\n");
        printf("===============================\n");
        printf("Opcao: ");

        // getche() le 1 caractere do teclado COM eco (mostra na tela).
        // Retorna o caractere digitado. Nao precisa dar Enter.
        opcao = getche();
        printf("\n\n");

        // switch direciona para a funcao correspondente
        switch (opcao) {
            case '1': cadastrar();              break;
            case '2': listar();                break;
            case '3': buscarPorCodigo();       break;
            case '4': alterarPreco();          break;
            case '5': registrarMovimento(1);   break;  // 1 = entrada
            case '6': registrarMovimento(0);   break;  // 0 = saida
            case '7': alertarMinimo();         break;
            case '8': relatorioValorTotal();   break;
            case '9': excluir();               break;
            case '0': printf("Saindo do sistema...\n"); break;
            default:  printf("Opcao invalida! Tente novamente.\n");
        }

        // Pausa para o usuario ler o resultado antes de limpar a tela.
        // Nao pausa se escolheu sair (opcao '0').
        if (opcao != '0') {
            system("pause");  // "Pressione qualquer tecla para continuar..."
        }
    } while (opcao != '0');  // repete enquanto nao escolher sair

    return 0;  // retorno do main (0 = sucesso)
}

// =========================================================================
// FUNCAO AUXILIAR: limparBufferStdin
// -------------------------------------------------------------------------
// Apos usar scanf, o caractere '\n' (Enter) fica no buffer de entrada.
// Se nao limpar, o proximo fgets le esse '\n' e retorna string vazia.
//
// fflush(stdin) NAO e portavel (o padrao C diz que o comportamento e
// indefinido para streams de entrada), MAS funciona no Windows com MinGW.
// O professor usa essa abordagem, entao mantemos o estilo.
// =========================================================================
void limparBufferStdin(void)
{
    // fflush(stdin) limpa o buffer de entrada no Windows.
    // NAO funciona em Linux/Mac, mas a prova e no Windows (MinGW).
    fflush(stdin);
}

// =========================================================================
// FUNCAO AUXILIAR: lerString
// -------------------------------------------------------------------------
// Le uma string de forma segura usando fgets (NAO usa gets, que e perigoso
// e foi removido do padrao C11 por causar buffer overflow).
//
// fgets le no maximo (tamanho-1) caracteres, evitando estouro de buffer.
// O problema do fgets e que ele inclui o '\n' no final da string.
// Usamos strcspn para encontrar a posicao do '\n' e substitui-lo por '\0'.
//
// strcspn(str, "\n") retorna o indice do primeiro caractere em str que
// esta no conjunto "\n". Se nao encontrar, retorna o tamanho da string.
// =========================================================================
void lerString(char *destino, int tamanho)
{
    // fgets(destino, tamanho, stdin): le do teclado ate (tamanho-1) chars ou Enter
    fgets(destino, tamanho, stdin);

    // strcspn encontra onde esta o '\n' e substitui por '\0' (fim de string)
    destino[strcspn(destino, "\n")] = '\0';
}

// =========================================================================
// 1. CADASTRAR PRODUTO
// -------------------------------------------------------------------------
// Abre o arquivo em modo "ab" (append binary) para ADICIONAR ao final.
// Se abrissemos em "wb", apagaria todo o conteudo anterior a cada cadastro!
// =========================================================================
void cadastrar(void)
{
    Produto p;   // variavel local para armazenar os dados do produto
    FILE *f;     // ponteiro para o arquivo

    printf("--- CADASTRO DE PRODUTO ---\n");

    // Leitura do codigo (inteiro)
    printf("Codigo: ");
    scanf("%d", &p.codigo);
    limparBufferStdin();  // limpa o '\n' deixado pelo scanf

    // Leitura do nome (string) - usa fgets+strcspn, NUNCA gets
    printf("Nome: ");
    lerString(p.nome, 50);

    // Leitura do preco (float)
    printf("Preco: ");
    scanf("%f", &p.preco);
    limparBufferStdin();

    // Leitura da quantidade inicial (inteiro)
    printf("Quantidade inicial: ");
    scanf("%d", &p.quantidade);
    limparBufferStdin();

    // Leitura do estoque minimo (inteiro)
    printf("Estoque minimo: ");
    scanf("%d", &p.minimo);
    limparBufferStdin();

    // "ab" = append binary: abre para escrita no final do arquivo.
    // Se o arquivo nao existir, ele e criado.
    f = fopen(ARQUIVO, "ab");
    if (f == NULL) {
        // Sempre verificar se fopen retornou NULL (erro ao abrir)
        printf("Erro ao abrir o arquivo para escrita!\n");
        return;
    }

    // fwrite grava 1 bloco de sizeof(Produto) bytes a partir do endereco &p.
    // Parametros: &p = endereco dos dados, sizeof(Produto) = tamanho do bloco,
    //             1 = quantidade de blocos, f = arquivo.
    fwrite(&p, sizeof(Produto), 1, f);
    fclose(f);  // sempre fechar o arquivo apos usar

    printf("\nProduto cadastrado com sucesso!\n");
}

// =========================================================================
// 2. LISTAR TODOS OS PRODUTOS
// -------------------------------------------------------------------------
// Abre em "rb" (read binary) e percorre todos os registros com fread.
//
// IMPORTANTE: usar "while (fread(...) == 1)" e NAO "while (!feof(f))".
// O feof() so retorna verdadeiro DEPOIS de uma tentativa de leitura que
// falhou (chegou no fim). Se usar while(!feof) com fread DENTRO do loop,
// o ultimo registro seria processado 2 vezes (uma valida, uma com lixo).
// Com "fread == 1", so processamos quando a leitura foi bem-sucedida.
// =========================================================================
void listar(void)
{
    Produto p;
    FILE *f;
    int count = 0;  // contador de produtos listados

    f = fopen(ARQUIVO, "rb");  // "rb" = read binary (so leitura)
    if (f == NULL) {
        printf("Nenhum produto cadastrado (arquivo nao encontrado).\n");
        return;
    }

    printf("--- LISTA DE PRODUTOS ---\n");
    // Cabecalho da tabela com formatacao alinhada
    // %-6s = string alinhada a esquerda com 6 espacos
    // %10s = string alinhada a direita com 10 espacos
    printf("%-6s %-25s %10s %8s %8s\n",
           "COD", "NOME", "PRECO", "QTDE", "MIN");
    printf("----------------------------------------------------------\n");

    // fread retorna o numero de blocos lidos com sucesso.
    // Se leu 1 bloco, ha dados validos para processar.
    // Se chegou no fim do arquivo, retorna 0 e o loop termina.
    while (fread(&p, sizeof(Produto), 1, f) == 1) {
        printf("%-6d %-25s %10.2f %8d %8d\n",
               p.codigo, p.nome, p.preco, p.quantidade, p.minimo);
        count++;
    }

    fclose(f);

    if (count == 0) {
        printf("Nenhum produto cadastrado.\n");
    } else {
        printf("\nTotal de produtos: %d\n", count);
    }
}

// =========================================================================
// 3. BUSCAR PRODUTO POR CODIGO
// -------------------------------------------------------------------------
// Percorre o arquivo comparando o codigo de cada registro com o buscado.
// Usa uma flag (encontrou) para saber se achou ou nao.
// =========================================================================
void buscarPorCodigo(void)
{
    Produto p;
    FILE *f;
    int codigoBusca;
    int encontrou = 0;  // flag: 0 = nao encontrou, 1 = encontrou

    printf("--- BUSCA POR CODIGO ---\n");
    printf("Digite o codigo: ");
    scanf("%d", &codigoBusca);
    limparBufferStdin();

    f = fopen(ARQUIVO, "rb");
    if (f == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    while (fread(&p, sizeof(Produto), 1, f) == 1) {
        if (p.codigo == codigoBusca) {
            // Achou! Mostra os dados do produto
            printf("\nProduto encontrado:\n");
            printf("  Codigo:     %d\n", p.codigo);
            printf("  Nome:       %s\n", p.nome);
            printf("  Preco:      %.2f\n", p.preco);
            printf("  Quantidade: %d\n", p.quantidade);
            printf("  Minimo:     %d\n", p.minimo);
            encontrou = 1;
            break;  // ja achou, nao precisa continuar procurando
        }
    }

    fclose(f);

    if (!encontrou) {
        printf("Produto com codigo %d nao encontrado.\n", codigoBusca);
    }
}

// =========================================================================
// 4. ALTERAR PRECO
// -------------------------------------------------------------------------
// Abre em "rb+" (read+write binary) para poder LER e ESCREVER no arquivo.
// Apos encontrar o registro, usa fseek para voltar o ponteiro do arquivo
// e sobrescrever o registro com o preco atualizado.
//
// DETALHE DO fseek: apos o fread, o ponteiro do arquivo avancou 1 registro.
// Para sobrescrever o registro que acabamos de ler, precisamos VOLTAR
// sizeof(Produto) bytes a partir da posicao atual (SEEK_CUR).
// O cast para (long) e necessario porque fseek espera um long int.
// =========================================================================
void alterarPreco(void)
{
    Produto p;
    FILE *f;
    int codigoBusca;
    int encontrou = 0;
    float novoPreco;

    printf("--- ALTERAR PRECO ---\n");
    printf("Digite o codigo do produto: ");
    scanf("%d", &codigoBusca);
    limparBufferStdin();

    // "rb+" = read binary update: permite leitura E escrita sem truncar.
    // Se usassemos "wb+", apagaria o arquivo existente!
    f = fopen(ARQUIVO, "rb+");
    if (f == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    while (fread(&p, sizeof(Produto), 1, f) == 1) {
        if (p.codigo == codigoBusca) {
            printf("Preco atual: %.2f\n", p.preco);
            printf("Novo preco: ");
            scanf("%f", &novoPreco);
            limparBufferStdin();

            p.preco = novoPreco;  // atualiza o preco na struct em memoria

            // Voltar o ponteiro do arquivo para o inicio do registro atual.
            // SEEK_CUR = a partir da posicao atual.
            // Voltamos sizeof(Produto) bytes (o tamanho de 1 registro).
            fseek(f, -(long)sizeof(Produto), SEEK_CUR);

            // Agora sobrescrevemos o registro com o preco atualizado.
            // O padrao C exige uma chamada a funcao de posicionamento (fseek)
            // entre uma leitura (fread) e uma escrita (fwrite) no modo update.
            fwrite(&p, sizeof(Produto), 1, f);

            encontrou = 1;
            printf("\nPreco alterado com sucesso!\n");
            break;
        }
    }

    fclose(f);

    if (!encontrou) {
        printf("Produto nao encontrado.\n");
    }
}

// =========================================================================
// 5 e 6. REGISTRAR ENTRADA OU SAIDA DE ESTOQUE
// -------------------------------------------------------------------------
// Recebe o parametro ehEntrada:
//   1 = entrada (soma quantidade ao estoque)
//   0 = saida  (subtrai quantidade do estoque)
//
// Mesma logica do alterarPreco: busca o registro, modifica a quantidade
// e sobrescreve com fseek + fwrite.
//
// Validacao importante: na saida, nao permitir que o estoque fique negativo.
// =========================================================================
void registrarMovimento(int ehEntrada)
{
    Produto p;
    FILE *f;
    int codigoBusca;
    int encontrou = 0;
    int qtd;

    // Mensagem diferente dependendo se e entrada ou saida
    if (ehEntrada) {
        printf("--- ENTRADA DE ESTOQUE ---\n");
    } else {
        printf("--- SAIDA DE ESTOQUE ---\n");
    }

    printf("Digite o codigo do produto: ");
    scanf("%d", &codigoBusca);
    limparBufferStdin();

    f = fopen(ARQUIVO, "rb+");  // leitura + escrita
    if (f == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    while (fread(&p, sizeof(Produto), 1, f) == 1) {
        if (p.codigo == codigoBusca) {
            printf("Produto: %s\n", p.nome);
            printf("Quantidade atual: %d\n", p.quantidade);

            // Operador ternario: escolhe a palavra conforme o tipo de operacao
            printf("Quantidade a %s: ", ehEntrada ? "entrar" : "sair");
            scanf("%d", &qtd);
            limparBufferStdin();

            // Validacao: saida nao pode ser maior que o estoque atual
            if (!ehEntrada && qtd > p.quantidade) {
                printf("Erro: quantidade de saida maior que estoque atual!\n");
                fclose(f);  // fecha antes de retornar
                return;
            }

            // Atualiza a quantidade conforme o tipo de operacao
            if (ehEntrada) {
                p.quantidade += qtd;  // entrada: soma
            } else {
                p.quantidade -= qtd;  // saida: subtrai
            }

            // Voltar o ponteiro para o inicio do registro atual
            fseek(f, -(long)sizeof(Produto), SEEK_CUR);

            // Sobrescrever o registro com a quantidade atualizada
            fwrite(&p, sizeof(Produto), 1, f);

            encontrou = 1;
            printf("\nEstoque atualizado: %d\n", p.quantidade);
            break;
        }
    }

    fclose(f);

    if (!encontrou) {
        printf("Produto nao encontrado.\n");
    }
}

// =========================================================================
// 7. ALERTAR PRODUTOS ABAIXO DO MINIMO
// -------------------------------------------------------------------------
// Percorre o arquivo e exibe os produtos cuja quantidade e menor que o
// minimo definido. Util para saber quais produtos precisam de reposicao.
// =========================================================================
void alertarMinimo(void)
{
    Produto p;
    FILE *f;
    int count = 0;  // contador de produtos abaixo do minimo

    printf("--- ALERTA: PRODUTOS ABAIXO DO MINIMO ---\n");

    f = fopen(ARQUIVO, "rb");
    if (f == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    while (fread(&p, sizeof(Produto), 1, f) == 1) {
        // Compara quantidade atual com o minimo definido
        if (p.quantidade < p.minimo) {
            printf("COD %-6d | %-25s | Qtde: %4d | Min: %4d  *** REPOR ***\n",
                   p.codigo, p.nome, p.quantidade, p.minimo);
            count++;
        }
    }

    fclose(f);

    if (count == 0) {
        printf("Nenhum produto abaixo do minimo. Estoque OK!\n");
    } else {
        printf("\n%d produto(s) precisam de reposicao.\n", count);
    }
}

// =========================================================================
// 8. RELATORIO: VALOR TOTAL DO ESTOQUE
// -------------------------------------------------------------------------
// Soma preco * quantidade de todos os produtos cadastrados.
// Mostra o subtotal de cada produto e o valor total no final.
// =========================================================================
void relatorioValorTotal(void)
{
    Produto p;
    FILE *f;
    float valorTotal = 0.0f;  // acumulador do valor total
    int count = 0;            // contador de produtos

    printf("--- RELATORIO: VALOR TOTAL DO ESTOQUE ---\n");

    f = fopen(ARQUIVO, "rb");
    if (f == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("%-6s %-25s %10s %8s %12s\n",
           "COD", "NOME", "PRECO", "QTDE", "SUBTOTAL");
    printf("------------------------------------------------------------\n");

    while (fread(&p, sizeof(Produto), 1, f) == 1) {
        // Subtotal = preco unitario * quantidade em estoque
        float subtotal = p.preco * (float)p.quantidade;

        printf("%-6d %-25s %10.2f %8d %12.2f\n",
               p.codigo, p.nome, p.preco, p.quantidade, subtotal);

        valorTotal += subtotal;  // acumula no total geral
        count++;
    }

    fclose(f);

    printf("------------------------------------------------------------\n");
    printf("TOTAL DE ITENS: %d\n", count);
    printf("VALOR TOTAL DO ESTOQUE: R$ %.2f\n", valorTotal);
}

// =========================================================================
// 9. EXCLUIR PRODUTO
// -------------------------------------------------------------------------
// Nao e possivel excluir um registro do meio de um arquivo binario
// diretamente. A estrategia padrao e:
//
// 1. Abrir o arquivo original para leitura ("rb")
// 2. Criar um arquivo temporario para escrita ("wb")
// 3. Copiar todos os registros EXCETO o que sera excluido
// 4. Fechar ambos os arquivos
// 5. Remover o arquivo original (remove)
// 6. Renomear o temporario para o nome original (rename)
//
// Se o produto nao for encontrado, o temporario e descartado.
// =========================================================================
void excluir(void)
{
    Produto p;
    FILE *fOrigem;   // arquivo original (leitura)
    FILE *fTemp;     // arquivo temporario (escrita)
    int codigoBusca;
    int encontrou = 0;

    printf("--- EXCLUIR PRODUTO ---\n");
    printf("Digite o codigo do produto a excluir: ");
    scanf("%d", &codigoBusca);
    limparBufferStdin();

    fOrigem = fopen(ARQUIVO, "rb");  // abre original para leitura
    if (fOrigem == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    // "wb" cria (ou sobrescreve) o arquivo temporario
    fTemp = fopen("temp.dat", "wb");
    if (fTemp == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(fOrigem);
        return;
    }

    // Percorre todos os registros do arquivo original
    while (fread(&p, sizeof(Produto), 1, fOrigem) == 1) {
        if (p.codigo == codigoBusca) {
            // Achou o produto a excluir: NAO copia para o temporario.
            // Apenas marca que encontrou.
            encontrou = 1;
        } else {
            // Nao e o produto excluido: copia normalmente para o temporario.
            fwrite(&p, sizeof(Produto), 1, fTemp);
        }
    }

    fclose(fOrigem);
    fclose(fTemp);

    if (encontrou) {
        // remove() apaga o arquivo original do disco
        remove(ARQUIVO);
        // rename() renomeia o temporario para o nome original
        rename("temp.dat", ARQUIVO);
        printf("Produto excluido com sucesso!\n");
    } else {
        // Nao encontrou: remove o temporario (que e uma copia identica)
        remove("temp.dat");
        printf("Produto nao encontrado.\n");
    }
}
