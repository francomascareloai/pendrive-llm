# 02 - CODIGO: ARQUIVOS, STRUCT, STRINGS, LEITURA E COLA RAPIDA

Documento de referencia de codigo para a parte PRATICA da prova.
Todas as construcoes de C do GB, com exemplos comentados.

INDICE DESTE ARQUIVO:
  PARTE 1 - Arquivos em C (fopen, fwrite/fread, fseek, CRUD)
  PARTE 2 - Struct avancado (typedef, ponteiro ->, aninhada, ordenar)
  PARTE 3 - Strings avancado (string.h, ctype.h, strtok, strstr, strcmp)
  PARTE 4 - Combinacoes (struct+arquivo, struct+string, recursao)
  PARTE 5 - Leitura de dados (TODAS as formas, scanf/fgets/sscanf/scanset)
  PARTE 6 - Cola rapida (snippets prontos para copiar)


#############################################################
# PARTE 1 - ARQUIVOS EM C
#############################################################

# ARQUIVOS EM C — GUIA EXAUSTIVO PARA O GRAU B (GB)

Palavras-chave para Ctrl+F: arquivo, fopen, fclose, fwrite, fread, fprintf, fscanf, fgets, fputs, fseek, SEEK_SET, SEEK_CUR, SEEK_END, rewind, feof, binario, texto, struct em arquivo, CRUD, remover, rename, temp, acesso direto, sizeof, FILE, NULL, fputc, fgetc, perror, remove, ftell, r, w, a, r+, w+, a+, rb, wb, ab, r+b, w+b, a+b, truncar, append, persistir, registro, byte offset.

---

## 1. VISAO GERAL — O QUE E UM ARQUIVO EM C

Um **arquivo** e uma colecao de dados armazenada em um dispositivo (geralmente disco rigido), que **persiste** apos o programa terminar. Variaveis comuns (int, float, vetores) morrem quando o programa fecha; arquivos nao. E por isso que usamos arquivos: para **guardar dados entre execucoes** (cadastro de clientes, notas de alunos, configuracoes, etc).

Em C, o acesso a arquivo e feito por meio de um **ponteiro especial** chamado `FILE *` (lê-se: "ponteiro para FILE"). O tipo `FILE` e uma `struct` definida dentro de `<stdio.h>` que guarda informacoes internas sobre o arquivo aberto: onde esta o cursor (posicao atual de leitura/escrita), se o arquivo esta aberto para leitura ou escrita, se houve erro, se chegou no fim (EOF), etc. Voce NAO precisa saber os campos internos de `FILE` — so precisa saber que **toda operacao recebe o `FILE *`** como argumento.

```c
#include <stdio.h>   // <-- OBRIGATORIO: contem fopen, fclose, fprintf, etc.

int main(void)
{
    FILE *arq;        // declara um PONTEIRO para arquivo (ainda nao aponta pra nada)
    // arq agora e "lixo" — so vamos usa-lo depois de fopen()
    return 0;
}
```

Fluxo PADRAO de qualquer programa que mexe com arquivo:

1. **Declarar** `FILE *arq;`
2. **Abrir** com `fopen` e **testar se deu NULL** (erro!).
3. **Usar** o arquivo (ler/escrever com fprintf, fread, etc.).
4. **Fechar** com `fclose(arq);` — libera o recurso e grava o que ficou em buffer.

Esquecer o `fclose` e um dos erros mais comuns em prova: o arquivo pode ficar **incompleto** (dados nao gravados) porque C usa **buffer** — ele so escreve no disco de fato quando o buffer enche ou quando voce fecha o arquivo.

---

## 2. TABELA DE TODOS OS MODOS DE ABERTURA (fopen)

A funcao `fopen(nome, modo)` retorna um `FILE *` ou `NULL` (se falhou). O **modo** e uma string que diz o que voce pode fazer com o arquivo. Decorar essa tabela e OBRIGATORIO para a prova teorica.

### 2.1 Modos TEXTO (sem o "b")

| Modo | Leitura? | Escrita? | Append? | Cria se nao existir? | Trunca (zera) se existir? | Posicao inicial |
|------|----------|----------|---------|----------------------|---------------------------|-----------------|
| "r"  | SIM | nao | nao | NAO (falha) | nao | inicio |
| "w"  | nao | SIM | nao | SIM | SIM (zera tudo!) | inicio |
| "a"  | nao | SIM | SIM (sempre no fim) | SIM | nao (preserva) | fim |
| "r+" | SIM | SIM | nao | NAO (falha) | nao | inicio |
| "w+" | SIM | SIM | nao | SIM | SIM (zera tudo!) | inicio |
| "a+" | SIM | SIM | SIM (escrita so no fim) | SIM | nao (preserva) | fim (leitura pode voltar com rewind) |

### 2.2 Modos BINARIO (com o "b") — mesmas regras, mas sem traducao de caracteres

| Modo | Leitura? | Escrita? | Append? | Cria? | Trunca? | Posicao inicial |
|------|----------|----------|---------|-------|----------|-----------------|
| "rb"  | SIM | nao | nao | NAO | nao | inicio |
| "wb"  | nao | SIM | nao | SIM | SIM | inicio |
| "ab"  | nao | SIM | SIM | SIM | nao | fim |
| "r+b" (ou "rb+") | SIM | SIM | nao | NAO | nao | inicio |
| "w+b" (ou "wb+") | SIM | SIM | nao | SIM | SIM | inicio |
| "a+b" (ou "ab+") | SIM | SIM | SIM | SIM | nao | fim |

### 2.3 Diferenca crucial: TEXTO vs BINARIO (cai em prova teorica!)

- **Modo texto ("r","w",...):** o sistema faz **traducao** de caracteres. No Windows, o `\n` (newline) vira a dupla `\r\n` (CRLF) ao gravar, e o `\r\n` vira `\n` ao ler. Isso pode **corromper dados binarios** (um int com byte 0x0A seria alterado!). Por isso NUNCA grave struct/int/float em modo texto.
- **Modo binario ("rb","wb",...):** os bytes sao gravados **exatamente como estao na memoria**, sem traducao. Use SEMPRE para gravar struct, int, float, vetores.

Regra de ouro:
- **Texto humano** (relatorios, CSV, configs legiveis) → modo texto.
- **Dados estruturados** (struct, int, float, vetores numericos) → modo binario.

### 2.4 Detalhes que caem em pegadinha

- **"w" sempre trunca**: se o arquivo ja existe, ele e **zerado** no momento do fopen. Cuidado para nao abrir "w" quando so queria ler!
- **"a" (append) so escreve no fim**: mesmo que voce use fseek, a escrita em modo "a"/"a+" **sempre vai para o final** do arquivo. A leitura (em "a+") pode ser em qualquer posicao com fseek.
- **"r" falha se o arquivo nao existe**: fopen retorna NULL. Por isso SEMPRE teste `if(arq==NULL)`.
- **"r+" nao cria**: se o arquivo nao existe, retorna NULL. Para criar, use "w+".
- **"w+" zera o arquivo mesmo que voce so queira ler depois**: cuidado!

---

## 3. FUNCOES ESSENCIAIS DE <stdio.h> PARA ARQUIVOS

Lista completa com assinatura e explicacao. Decore as assinaturas.

### 3.1 Abrir e fechar

```c
FILE *fopen(const char *nome, const char *modo);
// Retorna FILE* ou NULL (erro). Ex: fopen("dados.txt","r")

int fclose(FILE *arq);
// Fecha o arquivo, grava o buffer. Retorna 0 se ok, EOF se erro.
// SEMPRE chame fclose quando terminar de usar o arquivo!
```

### 3.2 Escrita/leitura em TEXTO (caractere por caractere ou linha)

```c
int fputc(int ch, FILE *arq);          // escreve 1 caractere. Retorna o char ou EOF.
int fgetc(FILE *arq);                  // le 1 caractere. Retorna o int do char ou EOF (fim).

int fputs(const char *str, FILE *arq); // escreve a string (sem \0). Retorna >=0 ou EOF.
char *fgets(char *s, int n, FILE *arq);// le ate n-1 chars ou ate \n. Retorna s ou NULL (fim/erro).
                                       // IMPORTANTE: fgets MANTEM o \n na string (se couber).

int fprintf(FILE *arq, const char *fmt, ...); // printf mas no arquivo. Retorna qtd de itens.
int fscanf(FILE *arq, const char *fmt, ...);  // scanf mas do arquivo. Retorna qtd de itens lidos ou EOF.
```

### 3.3 Escrita/leitura em BINARIO (blocos de bytes)

```c
size_t fwrite(const void *ptr, size_t tam, size_t qtd, FILE *arq);
// Grava qtd elementos de tam bytes cada, a partir do endereco ptr.
// Retorna quantos elementos foram gravados de fato (deve ser == qtd se ok).
// Ex: fwrite(&x, sizeof(int), 1, arq);  // grava 1 int

size_t fread(void *ptr, size_t tam, size_t qtd, FILE *arq);
// Le qtd elementos de tam bytes cada, para o endereco ptr.
// Retorna quantos elementos foram lidos de fato.
// Ex: fread(&x, sizeof(int), 1, arq);    // le 1 int
```

**Parametros do fwrite/fread (DECORE!):**
1. `ptr` → **endereco** da variavel/struct (use `&` para variavel simples, so o nome para vetor).
2. `tam` → **tamanho de UM elemento** em bytes → use `sizeof(tipo)` ou `sizeof(struct)`.
3. `qtd` → **quantos elementos** ler/gravar (geralmente 1 para struct unica, ou N para vetor).
4. `arq` → o `FILE *`.

### 3.4 Controle de posicao e estado

```c
int feof(FILE *arq);          // Retorna !=0 se o cursor passou do fim (EOF). CUIDADO: veja secao 8!
int fseek(FILE *arq, long offset, int origem);  // Move o cursor. origem: SEEK_SET/SEEK_CUR/SEEK_END.
long ftell(FILE *arq);        // Retorna a posicao atual do cursor (em bytes) ou -1L se erro.
void rewind(FILE *arq);      // Volta o cursor para o inicio (equivale a fseek(arq,0,SEEK_SET)).
```

Origens do `fseek`:
- `SEEK_SET` → inicio do arquivo (offset a partir do byte 0).
- `SEEK_CUR` → posicao atual do cursor.
- `SEEK_END` → fim do arquivo (offset geralmente negativo para voltar).

### 3.5 Gerenciamento de arquivos e erros

```c
int remove(const char *nome);        // Apaga o arquivo do disco. Retorna 0 se ok, -1 se erro.
int rename(const char *velho, const char *novo); // Renomeia/move. Retorna 0 ou -1.
void perror(const char *msg);        // Imprime msg + descricao do erro do sistema (errno).
                                     // Ex: perror("Erro ao abrir"); -> "Erro ao abrir: No such file..."
```

---

## 4. PADRAO SEGREDO — SEMPRE testar if(arq==NULL) apos fopen

Este e o padrao **numero 1** que o professor procura em qualquer codigo de arquivo. Se voce esquecer, perde ponto. Se aparecer em analise de codigo, e o bug.

```c
FILE *arq = fopen("dados.txt", "r");
if (arq == NULL)          // <-- SEMPRE! fopen pode falhar (arquivo nao existe, sem permissao, disco cheio)
{
    printf("Erro: nao consegui abrir o arquivo.\n");
    // perror("fopen");  // opcional: mostra a causa real (errno)
    return 1;             // sai do programa (ou retorna erro da funcao)
}
// ... usa o arquivo ...
fclose(arq);
```

Por que testar? Porque `fopen` devolve `NULL` quando falha (arquivo nao existe no modo "r", sem permissao, caminho errado, disco cheio no modo "w"). Se voce usar um `NULL` em `fprintf`/`fread`, o programa **crasha** (segmentation fault / access violation). Em prova, codigo sem esse teste esta **errado por definicao**.

---

## 5. ARQUIVO TEXTO — exemplo completo

Cenario: cadastrar nomes em um arquivo texto, um por linha, e depois listar.

```c
#include <stdio.h>
#include <string.h>

// Programa: cadastra e lista nomes em arquivo texto.
// Topico: arquivo texto, fprintf, fgets, NULL test, fclose.
// Compila: gcc -Wall -Wextra texto_nomes.c -o texto_nomes

int main(void)
{
    FILE *arq;
    char nome[50];
    int opcao;

    do
    {
        printf("\n1 - Cadastrar nome\n");
        printf("2 - Listar nomes\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        while (getchar() != '\n'); // limpa o \n deixado pelo scanf no buffer do teclado

        if (opcao == 1)
        {
            // MODO "a" (append): cria se nao existir, escreve no fim, NAO trunca.
            arq = fopen("nomes.txt", "a");
            if (arq == NULL)            // <-- teste OBRIGATORIO
            {
                printf("Erro ao abrir para gravacao.\n");
                continue;
            }
            printf("Digite o nome: ");
            fgets(nome, sizeof(nome), stdin);   // le do teclado (com espaco)
            nome[strcspn(nome, "\n")] = '\0';   // remove o \n que o fgets mantem
            fprintf(arq, "%s\n", nome);        // grava no arquivo + quebra de linha
            fclose(arq);                        // <-- NAO esqueca!
        }
        else if (opcao == 2)
        {
            arq = fopen("nomes.txt", "r");
            if (arq == NULL)            // <-- arquivo pode nao existir ainda
            {
                printf("Arquivo nao encontrado.\n");
                continue;
            }
            printf("\n--- Lista de nomes ---\n");
            // fgets le linha por linha. Retorna NULL quando chega no fim.
            while (fgets(nome, sizeof(nome), arq) != NULL)
            {
                printf("- %s", nome);   // o \n ja vem dentro de nome
            }
            fclose(arq);
        }
    } while (opcao != 0);

    return 0;
}
```

Pontos do exemplo acima que caem em prova:
- `fgets` retorna `NULL` no fim do arquivo → use no `while`. **Nao use `!feof` com fgets** (veja secao 8).
- `fgets` **mantem o `\n`** na string lida. Por isso o `printf("- %s", nome)` nao precisa de `\n`.
- `while (getchar() != '\n');` limpa o buffer do teclado apos `scanf("%d")` — truque do professor para evitar que o proximo `fgets` leia o `\n` restante e pule a leitura.
- `nome[strcspn(nome, "\n")] = '\0';` remove o `\n` antes de gravar (opcional, mas limpo).

---

## 6. ARQUIVO BINARIO — fwrite/fread com sizeof

### 6.1 Gravando um int simples

```c
#include <stdio.h>

// Grava 3 inteiros em arquivo binario e depois le de volta.
// Topico: fwrite, fread, sizeof(int), modo "wb"/"rb".
int main(void)
{
    FILE *arq;
    int numeros[3] = {10, 20, 30};
    int x, i;

    // --- GRAVACAO ---
    arq = fopen("numeros.bin", "wb");   // "wb": cria/zera, binario
    if (arq == NULL) { printf("Erro\n"); return 1; }

    // fwrite(endereco, tamanho_de_um, quantidade, arq)
    fwrite(numeros, sizeof(int), 3, arq);  // grava os 3 int de uma vez (vetor)
    fclose(arq);

    // --- LEITURA ---
    arq = fopen("numeros.bin", "rb");   // "rb": leitura binaria
    if (arq == NULL) { printf("Erro\n"); return 1; }

    // Le um por um para mostrar o padrao:
    while (fread(&x, sizeof(int), 1, arq) == 1)   // le 1 int por vez
    {
        printf("Li: %d\n", x);
    }
    fclose(arq);
    return 0;
}
```

Detalhe do `&`: `numeros` ja e endereco (vetor), por isso nao tem `&`. `x` e variavel simples, por isso `&x`.

### 6.2 Gravando uma STRUCT (o caso mais comum em prova)

```c
#include <stdio.h>

typedef struct {
    int    matricula;
    char   nome[40];
    float  nota;
} Aluno;

// Grava e le structs Aluno em arquivo binario.
// Topico: struct em arquivo, fwrite, fread, sizeof(struct).
int main(void)
{
    FILE *arq;
    Aluno a = {123, "Maria Silva", 8.5f};
    Aluno lida;

    // --- GRAVAR 1 struct ---
    arq = fopen("alunos.bin", "wb");
    if (arq == NULL) { printf("Erro\n"); return 1; }
    // &a porque a struct e variavel simples (precisa do endereco)
    // sizeof(Aluno) porque queremos o tamanho de UMA struct
    // 1 porque queremos gravar UMA struct
    fwrite(&a, sizeof(Aluno), 1, arq);
    fclose(arq);

    // --- LER 1 struct ---
    arq = fopen("alunos.bin", "rb");
    if (arq == NULL) { printf("Erro\n"); return 1; }
    fread(&lida, sizeof(Aluno), 1, arq);   // le 1 struct para dentro de 'lida'
    printf("Mat: %d  Nome: %s  Nota: %.2f\n", lida.matricula, lida.nome, lida.nota);
    fclose(arq);
    return 0;
}
```

Por que `sizeof(Aluno)` e nao `sizeof(Aluno*)`? Porque queremos o **tamanho da struct inteira** (matricula + nome + nota + possivel padding), nao o tamanho de um ponteiro. Sempre use `sizeof(TipoDaStruct)` ou `sizeof(struct)`.

---

## 7. ACESSO DIRETO com fseek — SEEK_SET, SEEK_CUR, SEEK_END

Acesso direto = pular para uma posicao **exata** do arquivo sem ler tudo antes. So funciona bem em **arquivo binario** com registros de **tamanho fixo** (struct). Em arquivo texto, o tamanho das linhas varia, entao o calculo de byte offset nao funciona.

### 7.1 As tres origens

```c
fseek(arq, offset, SEEK_SET);  // offset a partir do INICIO do arquivo (mais usado)
fseek(arq, offset, SEEK_CUR);  // offset a partir da POSICAO ATUAL
fseek(arq, offset, SEEK_END);  // offset a partir do FIM (offset geralmente negativo)
```

### 7.2 Calculo de byte offset para pular para o registro N

Se cada registro ocupa `sizeof(struct)` bytes, entao:
- Registro 0 → byte 0
- Registro 1 → byte `1 * sizeof(struct)`
- Registro N → byte `N * sizeof(struct)`

```c
typedef struct {
    int   matricula;
    char  nome[40];
    float nota;
} Aluno;

// Le direto o registro de indice N (0-based) do arquivo binario.
void lerRegistroN(const char *caminho, int N)
{
    FILE *arq = fopen(caminho, "rb");
    if (arq == NULL) { printf("Erro\n"); return; }

    // Pula para o byte N * sizeof(Aluno) a partir do inicio.
    // SEEK_SET = origem no inicio do arquivo.
    long offset = (long)N * sizeof(Aluno);
    fseek(arq, offset, SEEK_SET);

    Aluno a;
    // Le 1 struct a partir da posicao atual (para onde o fseek apontou).
    if (fread(&a, sizeof(Aluno), 1, arq) == 1)
    {
        printf("Registro %d: %d %s %.2f\n", N, a.matricula, a.nome, a.nota);
    }
    else
    {
        printf("Registro %d nao existe (fora do arquivo).\n", N);
    }
    fclose(arq);
}
```

Atencao ao **cast** `(long)N`: se `sizeof` retorna `size_t` e N e int, multiplicar pode dar overflow em arquivos grandes. Cast para `long` evita warning. Em prova, o importante e a formula `N * sizeof(struct)`.

### 7.3 Usos classicos do fseek

- **Ir para o fim para contar o tamanho do arquivo** (veja secao 11).
- **Ir para um registro especifico** (acesso direto por indice).
- **Voltar para o inicio** com `fseek(arq, 0, SEEK_SET)` ou `rewind(arq)` (sao equivalentes).
- **Andar para tras a partir do fim**: `fseek(arq, -sizeof(struct), SEEK_END)` le o ULTIMO registro.

---

## 8. LEITURA CORRETA — while(fread==1) vs while(!feof) — PONTO DE PROVA

Este e um dos topicos MAIS cobertos em analise de codigo. Decore o seguinte:

### 8.1 O PADRAO CORRETO (use sempre este)

```c
Aluno a;
while (fread(&a, sizeof(Aluno), 1, arq) == 1)
{
    printf("%s\n", a.nome);
}
```

Por que e correto? Porque `fread` retorna **quantos elementos foram lidos de fato**. Quando ele tenta ler e ja esta no fim do arquivo, ele le **0 elementos** e retorna 0 — entao o loop para **antes** de processar lixo. O `fread` so retorna 1 quando realmente leu uma struct completa.

O mesmo vale para `fscanf` e `fgets`:
```c
while (fscanf(arq, "%d", &x) == 1) { ... }      // para quando fscanf nao consegue ler
while (fgets(linha, 80, arq) != NULL) { ... }   // para quando fgets nao le nada
```

### 8.2 O PADRAO BUGADO (nao use — mas reconheca em prova)

```c
// BUG CLASSICO — NAO FAZER ISSO:
while (!feof(arq))
{
    fread(&a, sizeof(Aluno), 1, arq);
    printf("%s\n", a.nome);   // imprime o ULTIMO registro DUAS VEZES (ou lixo)
}
```

Por que da errado? Porque `feof` so retorna **verdadeiro DEPOIS** que uma leitura tentou passar do fim. Ou seja:
1. O `fread` le o ultimo registro valido → `feof` ainda e **falso** (a leitura funcionou).
2. O loop roda de novo → `fread` tenta ler e **falha** (nao le nada, `a` continua com o valor anterior).
3. O `printf` imprime o **ultimo registro de novo** (ou lixo nao inicializado).
4. So agora `feof` vira verdadeiro e o loop para.

Resultado: o ultimo registro aparece **duas vezes**, ou aparece **lixo** no final. Em prova, se voce vir `while(!feof(arq))` seguido de `fread` dentro do loop, marque como **ERRO**.

### 8.3 Outra variante bugada (fread antes do loop)

```c
// TAMBM BUGADO:
fread(&a, sizeof(Aluno), 1, arq);          // le o primeiro fora do loop
while (!feof(arq))
{
    printf("%s\n", a.nome);
    fread(&a, sizeof(Aluno), 1, arq);      // le o proximo
}
```

Mesmo problema: o `feof` so acende depois da leitura falha, entao o ultimo `printf` imprime o penultimo de novo ou lixo. **Evite.**

### 8.4 Resumo para a prova

| Padrao | Correto? | Por que |
|--------|----------|---------|
| `while(fread(&a,sizeof,1,arq)==1)` | SIM | fread retorna 0 quando falha; loop para antes de processar lixo |
| `while(fscanf(arq,"%d",&x)==1)` | SIM | mesma logica |
| `while(fgets(s,80,arq)!=NULL)` | SIM | fgets retorna NULL no fim |
| `while(!feof(arq)){ fread(...); processa; }` | NAO | feof so acende depois da leitura falha; ultimo item duplicado/lixo |
| `fread(...); while(!feof){processa; fread;}` | NAO | mesmo bug |

---

## 9. CRUD em arquivo TEXTO — padrao temp + rename

CRUD = Create (criar/cadastrar), Read (listar), Update (alterar), Delete (excluir). Em arquivo texto, **nao da para alterar uma linha no meio** sem reescrever tudo. Por isso o padrao e: **abrir o original para leitura, abrir um temporario para escrita, copiar tudo mudando o que precisa, fechar os dois, remover o original e renomear o temporario**.

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// CRUD em arquivo texto: cadastrar, listar, alterar, excluir.
// Topico: CRUD texto, temp+rename, fgets, fprintf, remove, rename.
// Compila: gcc -Wall -Wextra crud_texto.c -o crud_texto

void cadastrar(void)
{
    FILE *arq = fopen("dados.txt", "a");   // append: nao zera, cria se faltar
    if (arq == NULL) { printf("Erro\n"); return; }
    char nome[50];
    int  idade;
    printf("Nome: ");  scanf(" %49[^\n]", nome);   // le ate \n (com espacos)
    printf("Idade: "); scanf("%d", &idade);
    fprintf(arq, "%s|%d\n", nome, idade);          // formato: nome|idade
    fclose(arq);
}

void listar(void)
{
    FILE *arq = fopen("dados.txt", "r");
    if (arq == NULL) { printf("Arquivo vazio ou inexistente.\n"); return; }
    char linha[100];
    printf("\n--- Cadastros ---\n");
    while (fgets(linha, sizeof(linha), arq) != NULL)   // PADRAO CORRETO
    {
        printf("%s", linha);   // linha ja vem com \n
    }
    fclose(arq);
}

void alterar(void)
{
    char busca[50];
    printf("Nome para alterar: "); scanf(" %49[^\n]", busca);

    FILE *orig = fopen("dados.txt", "r");
    if (orig == NULL) { printf("Sem arquivo.\n"); return; }
    FILE *tmp = fopen("temp.txt", "w");          // temporario em modo escrita (zera)
    if (tmp == NULL) { fclose(orig); printf("Erro temp\n"); return; }

    char linha[100];
    int achou = 0;
    while (fgets(linha, sizeof(linha), orig) != NULL)
    {
        char nome[50];
        int  idade;
        // sscanf extrai nome e idade da linha (formato nome|idade)
        if (sscanf(linha, "%49[^|]|%d", nome, &idade) == 2)
        {
            if (strcmp(nome, busca) == 0)        // achou o registro
            {
                printf("Nova idade: "); scanf("%d", &idade);
                fprintf(tmp, "%s|%d\n", nome, idade);   // grava alterado
                achou = 1;
            }
            else
            {
                fprintf(tmp, "%s", linha);        // copia igual (ja tem \n)
            }
        }
    }
    fclose(orig);
    fclose(tmp);

    if (achou)
    {
        remove("dados.txt");          // apaga o original
        rename("temp.txt", "dados.txt");  // temporario vira o original
        printf("Alterado.\n");
    }
    else
    {
        remove("temp.txt");           // nao achou: descarta o temp
        printf("Nao encontrado.\n");
    }
}

void excluir(void)
{
    char busca[50];
    printf("Nome para excluir: "); scanf(" %49[^\n]", busca);

    FILE *orig = fopen("dados.txt", "r");
    if (orig == NULL) { printf("Sem arquivo.\n"); return; }
    FILE *tmp = fopen("temp.txt", "w");
    if (tmp == NULL) { fclose(orig); printf("Erro temp\n"); return; }

    char linha[100];
    int achou = 0;
    while (fgets(linha, sizeof(linha), orig) != NULL)
    {
        char nome[50]; int idade;
        if (sscanf(linha, "%49[^|]|%d", nome, &idade) == 2)
        {
            if (strcmp(nome, busca) == 0)
            {
                achou = 1;            // NAO copia para o temp → some
            }
            else
            {
                fprintf(tmp, "%s", linha);   // copia os outros
            }
        }
    }
    fclose(orig);
    fclose(tmp);

    if (achou)
    {
        remove("dados.txt");
        rename("temp.txt", "dados.txt");
        printf("Excluido.\n");
    }
    else
    {
        remove("temp.txt");
        printf("Nao encontrado.\n");
    }
}

int main(void)
{
    int op;
    do {
        printf("\n1-Cad 2-Lista 3-Alt 4-Exc 0-Sai: ");
        scanf("%d", &op);
        switch (op) {
            case 1: cadastrar(); break;
            case 2: listar();   break;
            case 3: alterar();  break;
            case 4: excluir();   break;
        }
    } while (op != 0);
    return 0;
}
```

### 9.1 O padrao temp + rename (DECORE este bloco)

```c
// 1. Abre original para LEITURA ("r") e temporario para ESCRITA ("w")
FILE *orig = fopen("dados.txt", "r");
FILE *tmp  = fopen("temp.txt",  "w");

// 2. Copia registro por registro, mudando (alterar) ou pulando (excluir) o desejado
while (fgets(...) != NULL) { ... fprintf(tmp, ...); }

// 3. Fecha os DOIS antes de remover/renomear (Windows bloqueia arquivos abertos!)
fclose(orig);
fclose(tmp);

// 4. Remove o original e renomeia o temporario
remove("dados.txt");
rename("temp.txt", "dados.txt");
```

**Detalhe que cai em prova:** no Windows, **nao da para remover ou renomear um arquivo que esta aberto**. Por isso e OBRIGATORIO fechar ambos (`fclose`) antes de `remove`/`rename`. Se esquecer, o `remove` falha silenciosamente.

---

## 10. CRUD em arquivo BINARIO com struct

Mesma logica do CRUD texto, mas usando `fwrite`/`fread` e `sizeof(struct)`. A vantagem do binario e que **acesso direto** (fseek) permite **alterar um registro no lugar** sem reescrever o arquivo todo — mas so se o registro tiver o **mesmo tamanho** (struct de tamanho fixo). Para **excluir**, ainda precisamos do padrao temp+rename, porque nao da para "apagar um buraco" no meio do arquivo.

```c
#include <stdio.h>
#include <string.h>

typedef struct {
    int   matricula;
    char  nome[40];
    float nota;
} Aluno;

// CRUD binario: cadastrar, listar, alterar (no lugar), excluir (temp+rename).
// Topico: struct em arquivo, fwrite, fread, fseek, acesso direto, temp+rename.

void cadastrar(void)
{
    FILE *arq = fopen("alunos.bin", "ab");   // append binario: nao zera, cria se faltar
    if (arq == NULL) { printf("Erro\n"); return; }
    Aluno a;
    printf("Matricula: "); scanf("%d", &a.matricula);
    printf("Nome: ");      scanf(" %39[^\n]", a.nome);
    printf("Nota: ");     scanf("%f", &a.nota);
    fwrite(&a, sizeof(Aluno), 1, arq);       // grava 1 struct no fim
    fclose(arq);
}

void listar(void)
{
    FILE *arq = fopen("alunos.bin", "rb");
    if (arq == NULL) { printf("Sem arquivo.\n"); return; }
    Aluno a;
    printf("\n--- Alunos ---\n");
    while (fread(&a, sizeof(Aluno), 1, arq) == 1)   // PADRAO CORRETO
    {
        printf("%d  %s  %.2f\n", a.matricula, a.nome, a.nota);
    }
    fclose(arq);
}

// ALTERAR NO LUGAR — vantagem do binario com tamanho fixo!
void alterar(int matBusca)
{
    FILE *arq = fopen("alunos.bin", "r+b");   // r+b: le e escreve, nao zera
    if (arq == NULL) { printf("Erro\n"); return; }
    Aluno a;
    int achou = 0;
    while (fread(&a, sizeof(Aluno), 1, arq) == 1)
    {
        if (a.matricula == matBusca)
        {
            printf("Nova nota: "); scanf("%f", &a.nota);
            // Volta o cursor em 1 struct para reescrever o registro que acabamos de ler.
            // fread avancou o cursor; precisamos voltar para sobrescrever.
            fseek(arq, -(long)sizeof(Aluno), SEEK_CUR);
            fwrite(&a, sizeof(Aluno), 1, arq);   // sobrescreve no lugar
            achou = 1;
            break;   // achou, nao precisa continuar
        }
    }
    fclose(arq);
    printf(achou ? "Alterado.\n" : "Nao encontrado.\n");
}

// EXCLUIR — nao da para apagar no lugar; usa temp+rename.
void excluir(int matBusca)
{
    FILE *orig = fopen("alunos.bin", "rb");
    if (orig == NULL) { printf("Sem arquivo.\n"); return; }
    FILE *tmp = fopen("temp.bin", "wb");
    if (tmp == NULL) { fclose(orig); printf("Erro temp\n"); return; }

    Aluno a;
    int achou = 0;
    while (fread(&a, sizeof(Aluno), 1, orig) == 1)
    {
        if (a.matricula == matBusca)
        {
            achou = 1;   // pula — nao escreve no temp
        }
        else
        {
            fwrite(&a, sizeof(Aluno), 1, tmp);   // copia os outros
        }
    }
    fclose(orig);
    fclose(tmp);

    if (achou)
    {
        remove("alunos.bin");
        rename("temp.bin", "alunos.bin");
        printf("Excluido.\n");
    }
    else
    {
        remove("temp.bin");
        printf("Nao encontrado.\n");
    }
}

int main(void)
{
    int op, mat;
    do {
        printf("\n1-Cad 2-Lista 3-Alt 4-Exc 0-Sai: ");
        scanf("%d", &op);
        switch (op) {
            case 1: cadastrar(); break;
            case 2: listar();    break;
            case 3: printf("Matricula: "); scanf("%d", &mat); alterar(mat); break;
            case 4: printf("Matricula: "); scanf("%d", &mat); excluir(mat);  break;
        }
    } while (op != 0);
    return 0;
}
```

### 10.1 Truque do fseek negativo para alterar no lugar

Depois de um `fread` que leu 1 struct, o cursor avancou `sizeof(Aluno)` bytes. Para **reescrever aquele mesmo registro**, voltamos o cursor com:

```c
fseek(arq, -(long)sizeof(Aluno), SEEK_CUR);   // volta 1 struct a partir da posicao atual
fwrite(&a, sizeof(Aluno), 1, arq);            // sobrescreve
```

Isso so funciona em modo `"r+b"` (leitura e escrita) e com registros de **tamanho fixo**. Em arquivo texto, as linhas tem tamanhos diferentes, entao nao da para voltar um numero exato de bytes.

---

## 11. CONTAR REGISTROS de um arquivo binario

Truque classico: ir para o fim com `fseek(..., SEEK_END)`, ler a posicao com `ftell` (que da o tamanho total em bytes), e dividir por `sizeof(struct)`.

```c
// Retorna quantos registros (structs) existem no arquivo binario.
long contarRegistros(const char *caminho)
{
    FILE *arq = fopen(caminho, "rb");
    if (arq == NULL) return 0;          // arquivo nao existe → 0 registros

    fseek(arq, 0, SEEK_END);            // vai para o fim do arquivo
    long tamanhoBytes = ftell(arq);     // ftell da a posicao atual = tamanho total em bytes
    fclose(arq);

    // numero de registros = tamanho total / tamanho de um registro
    return tamanhoBytes / sizeof(Aluno);
}
```

Explicacao passo a passo:
1. `fopen("rb")` — abre para leitura binaria. Se falhar (NULL), retorna 0.
2. `fseek(arq, 0, SEEK_END)` — move o cursor para o **fim** do arquivo.
3. `ftell(arq)` — como o cursor esta no fim, retorna o **numero total de bytes** do arquivo.
4. `tamanhoBytes / sizeof(Aluno)` — como cada struct ocupa exatamente `sizeof(Aluno)` bytes, a divisao da o numero de registros.

Atencao: isso so funciona em **arquivo binario** com registros de **tamanho fixo**. Em arquivo texto, o tamanho das linhas varia, entao nao faz sentido dividir.

---

## 12. ERROS COMUNS EM ARQUIVO (topico de prova teorica)

Lista dos erros que aparecem em analise de codigo e assercoes. Decore cada um.

### 12.1 Esquecer o fclose

```c
// ERRO: arquivo nunca e fechado
FILE *arq = fopen("x.txt", "w");
fprintf(arq, "teste");
// faltou fclose(arq);  → dados podem nao ser gravados (buffer nao fluiu)
```

**Por que e erro:** C usa **buffer** — a escrita so vai para o disco quando o buffer enche ou quando voce fecha o arquivo. Sem `fclose`, os ultimos dados podem **ficar presos no buffer** e nunca serem gravados. Alem disso, o arquivo fica **aberto/travado** (no Windows, outros programas nao conseguem apaga-lo).

### 12.2 Nao testar NULL apos fopen

```c
// ERRO: se o arquivo nao existe no modo "r", fopen retorna NULL
FILE *arq = fopen("x.txt", "r");   // pode ser NULL!
fscanf(arq, "%d", &x);            // CRASH: usar NULL como FILE*
```

**Por que e erro:** `fopen` em modo `"r"` retorna `NULL` se o arquivo nao existe. Usar `NULL` em `fscanf`/`fread`/`fprintf` causa **crash** (access violation). Sempre: `if (arq == NULL) { ...trata erro... }`.

### 12.3 Usar feof errado (o bug da secao 8)

```c
// ERRO: ultimo registro impresso duas vezes (ou lixo)
while (!feof(arq)) {
    fread(&a, sizeof(a), 1, arq);
    printf("%s", a.nome);
}
```

**Por que e erro:** `feof` so retorna verdadeiro **depois** que uma leitura falhou. O loop processa um item a mais (o ultimo valido de novo, ou lixo). Use `while (fread(...) == 1)`.

### 12.4 Confundir modos de abertura

- Abrir `"w"` quando queria ler → **zera o arquivo** sem avisar! Perde todos os dados.
- Abrir `"r"` quando queria criar → retorna `NULL` (nao cria).
- Abrir `"a"` e tentar escrever no meio com `fseek` → **nao funciona**: em modo append, a escrita **sempre vai para o fim**, mesmo com fseek.
- Abrir `"r"` e tentar escrever → falha (modo so leitura).
- Abrir `"w"` e tentar ler → o arquivo foi zerado, nao tem nada para ler.

### 12.5 Nao posicionar o cursor antes de ler/escrever

```c
// ERRO: depois de ler tudo, o cursor esta no fim; nova leitura nao le nada
while (fread(&a, sizeof(a), 1, arq) == 1) { ... }
fread(&a, sizeof(a), 1, arq);   // ja esta no fim → le 0, nao faz nada
// Para ler de novo, precisa voltar:
rewind(arq);   // ou fseek(arq, 0, SEEK_SET);
```

**Por que e erro:** apos um loop de leitura, o cursor fica no **fim** do arquivo. Qualquer leitura nova retorna 0/EOF. Para reler, use `rewind` ou `fseek(arq, 0, SEEK_SET)`.

### 12.6 Usar modo texto para dados binarios

```c
// ERRO: gravar struct em modo texto corrompe os dados
FILE *arq = fopen("alunos.txt", "w");   // modo texto!
fwrite(&a, sizeof(Aluno), 1, arq);       // os bytes 0x0A viram \r\n → corrompe
```

**Por que e erro:** o modo texto traduz `\n` para `\r\n` no Windows. Se um byte da struct for `0x0A` (que e `\n`), ele vira dois bytes (`\r\n`), **corrompendo** a struct. Use sempre `"wb"`/`"rb"` para struct/int/float.

### 12.7 Esquecer o & no fread/fwrite de variavel simples

```c
int x;
fread(x, sizeof(int), 1, arq);     // ERRO: x nao e endereco → crash
fread(&x, sizeof(int), 1, arq);     // CORRETO: &x e o endereco de x
```

Excecao: **vetores e structs ja sao enderecos** quando passados por nome:
```c
int v[10];
fread(v, sizeof(int), 10, arq);     // correto: v ja e endereco (nao precisa de &)
Aluno a;
fread(&a, sizeof(Aluno), 1, arq);   // &a: struct simples precisa do &
```

### 12.8 Abrir o mesmo arquivo duas vezes sem fechar

```c
// ERRO: no Windows, o arquivo ja esta travado pelo primeiro fopen
FILE *a1 = fopen("x.txt", "r");
FILE *a2 = fopen("x.txt", "w");   // pode falhar ou travar
```

Sempre feche um antes de abrir outro para o mesmo caminho, ou use o mesmo `FILE *`.

### 12.9 Nao fechar o temporario antes de remove/rename

```c
// ERRO no Windows: arquivo aberto nao pode ser removido/renomeado
FILE *tmp = fopen("temp.txt", "w");
// ... escreve ...
remove("dados.txt");          // pode falhar se dados.txt estiver aberto
rename("temp.txt", "dados.txt"); // pode falhar se temp.txt estiver aberto
// Faltou fclose(tmp); antes
```

### 12.10 Misturar leitura e escrita sem fflush/rewind entre elas

Em modo `"r+"`/`"w+"`/`"a+"`, ao alternar entre leitura e escrita, deve-se fazer um `fflush` ou `fseek` entre elas. Senao o comportamento e indefinido (undefined behavior). Truque seguro: sempre posicione o cursor com `fseek` antes de alternar.

---

## 13. RESUMO RAPIDO PARA DECORAR NA VESPERA

- **Toda fopen** → testar `if (arq == NULL)`.
- **Toda fopen** → tem `fclose` correspondente.
- **Texto** → `fprintf`/`fscanf`/`fgets`/`fputs`/`fgetc`/`fputc`. Modos `r w a r+ w+ a+`.
- **Binario** → `fwrite`/`fread` com `sizeof`. Modos `rb wb ab r+b w+b a+b`.
- **Leitura correta** → `while (fread(...) == 1)` ou `while (fgets(...) != NULL)`. **Nunca** `while (!feof)`.
- **Acesso direto** → `fseek(arq, N * sizeof(struct), SEEK_SET)`.
- **Contar registros** → `fseek END` + `ftell` / `sizeof(struct)`.
- **Alterar texto** → temp + rename (nao da para alterar no lugar).
- **Alterar binario** → `r+b` + `fseek` negativo + `fwrite` no lugar (se tamanho fixo).
- **Excluir** → sempre temp + rename (texto ou binario).
- **Windows** → fechar arquivos antes de `remove`/`rename`.
- **struct** → `fwrite(&a, sizeof(Aluno), 1, arq)` (com `&`).
- **vetor** → `fwrite(v, sizeof(int), N, arq)` (sem `&`).

---

Fim do documento. Use Ctrl+F com as palavras-chave do topo para encontrar qualquer topico rapidamente durante a prova.


#############################################################
# PARTE 2 - STRUCT AVANCADO
#############################################################

# STRUCT AVANCADO - GUIA COMPLETO PARA O GB

> Material de consulta para a prova do Grau B (GB) de Algoritmos e Programacao II (Uniftec).
> Linguagem: C puro (gcc no Windows / MinGW).
> Aluno: Franco.
> Compila com: `gcc -Wall -Wextra arquivo.c -o programa`

---

## INDICE (use Ctrl+F com as palavras-chave marcadas entre colchetes)

1. [struct] O que e struct e VANTAGEM de usar (questao teorica confirmada)
2. [typedef] Sintaxe correta do typedef struct em C puro (e o bug do typedef sem alias)
3. [ponto] Declarar variavel e acessar campos com ponto (.)
4. [parametro struct] Struct como parametro de funcao (por valor vs por referencia)
5. [vetor de struct] Vetor de struct: cadastro, ler, imprimir, buscar, ordenar
6. [seta] [->] [ponteiro struct] Ponteiro para struct e operador SETA (->)
7. [struct aninhada] Struct aninhada (Endereco dentro de Pessoa)
8. [retorno struct] Struct como retorno de funcao
9. [ordenar struct] Ordenar vetor de struct por campo numerico e por STRING (strcmp)
10. [buscar struct] Buscar struct por nome (strcmp) e por codigo (==)
11. [media struct] Calcular media/soma de um campo no vetor de struct
12. [maior struct] Encontrar maior/menor por um campo (mais velho, maior salario)
13. [erros comuns] Erros comuns: == em string, esquecer &, ponto vs seta, typedef sem alias

---

## 1. [struct] O QUE E STRUCT E VANTAGEM DE USAR (TEORICO CONFIRMADO)

### O que e

Struct (estrutura) e um tipo de dado composto que permite agrupar varias variaveis,
possivelmente de tipos DIFERENTES, sob um unico nome. Cada variavel interna e chamada
de "campo" (ou membro).

Exemplo: para representar uma Pessoa, precisamos do nome (string), idade (int) e
altura (float). Sem struct, teriamos tres variaveis avulsas:

```c
char nome[50];
int  idade;
float altura;
```

O problema: se quisermos cadastrar 30 pessoas, teriamos que criar 3 vetores
separados (nome[30][50], idade[30], altura[30]) e ficar sincronizando os indices.
Facil errar. Com struct, agrupamos:

```c
typedef struct {
    char  nome[50];
    int   idade;
    float altura;
} Pessoa;
```

Agora uma Pessoa contem os tres dados juntos. Um vetor `Pessoa lista[30];` guarda
tudo de forma organizada: `lista[i].nome`, `lista[i].idade`, `lista[i].altura`
pertencem a mesma pessoa de indice i.

### VANTAGEM (isto cai na teorica)

A grande vantagem do struct e AGRUPAR DADOS RELACIONADOS DE TIPOS DIFERENTES em uma
so unidade logica. Isso traz:

- **Organizacao**: dados que pertencem ao mesmo "objeto" ficam juntos, nao espalhados.
- **Reuso**: passa-se a struct inteira para uma funcao, em vez de varios argumentos.
  Ex: `imprimir(Pessoa p)` em vez de `imprimir(char n[], int i, float a)`.
- **Legibilidade**: o codigo reflete o mundo real (uma Pessoa, um Produto, um Carro).
- **Manutencao**: adicionar um campo (ex: CPF) nao quebra as chamadas de funcao.
- **Vetores de registros**: um unico `Pessoa lista[30]` substitui 3 vetores paralelos.

> ATENCAO (pegadinha de prova): struct NAO serve para "repetir comandos" nem para
> "criar funcoes". Struct guarda DADOS, nao codigo. Repetir blocos e papel de
> funcao/loop. Outra pegadinha: struct agrupa tipos DIFERENTES; se todos os campos
> fossem do mesmo tipo, daria para usar vetor - a forca do struct e justamente
> misturar int, float, char[] no mesmo registro.

### Diferenca entre struct e vetor (memorize)

| Caracteristica  | Vetor (array)              | Struct                          |
|-----------------|----------------------------|---------------------------------|
| Tipos dos dados | Todos IGUAIS              | Podem ser DIFERENTES            |
| Acesso          | Por INDICE: v[i]          | Por NOME do campo: p.idade      |
| Tamanho         | Definido em tempo de exec. | Fixo pelos campos declarados    |
| Uso tipico      | Lista de valores homogeneos| Um "registro" de uma entidade   |

---

## 2. [typedef] SINTAXE CORRETA DO typedef struct EM C PURO

### Forma CORRETA (use esta)

```c
// Cria um NOVO TIPO chamado "Pessoa" que e uma struct com esses campos.
// O typedef no inicio + o nome no final fazem "Pessoa" ser um alias direto.
typedef struct {
    char  nome[50];
    int   idade;
    float altura;
} Pessoa;   // <-- nome do tipo. SEM ponto-e-virgula faltando aqui!
```

Depois disso, declaramos variaveis sem a palavra `struct`:

```c
Pessoa p1;          // variavel simples
Pessoa lista[30];   // vetor de 30 pessoas
Pessoa *ptr;        // ponteiro para Pessoa
```

### Forma TAMBEM correta (com tag, estilo antigo)

```c
// Aqui a struct tem uma "tag" (nomePessoa) e tambem um alias (Pessoa).
// Em C puro os dois estilos funcionam. O alias (typedef) e o que usamos no dia a dia.
typedef struct nomePessoa {
    char  nome[50];
    int   idade;
} Pessoa;
```

### BUG CLASSICO: typedef struct SEM alias (NAO compila em C puro)

```c
// ERRADO em C puro (so funciona em C++):
typedef struct {
    char nome[50];
    int  idade;
};   // <-- faltou o nome do alias no final!
```

Em C puro isso gera erro / warning, porque o typedef precisa de um nome para o alias.
Sem o alias, voce declararia a variavel assim: `struct { ... } p1;` (struct anonima),
o que e ruim e nao reutilizavel.

> RESUMO para prova: `typedef struct { campos; } NomeTipo;` e o padrao seguro.
> O nome do tipo vai no FINAL, antes do ponto-e-virgula.

### Outro bug: esquecer o typedef e tentar usar so o nome

```c
struct Pessoa {        // sem typedef - "Pessoa" aqui e a TAG, nao o tipo
    char nome[50];
    int  idade;
};

Pessoa p1;             // ERRADO em C puro (Pessoa nao e tipo, e tag)
struct Pessoa p1;      // CERTO - precisa do "struct" na frente
```

Por isso usamos `typedef`: para nao ter que escrever `struct` toda hora.

---

## 3. [ponto] DECLARAR VARIAVEL E ACESSAR CAMPOS COM PONTO (.)

Depois de definir o tipo, declaramos variaveis e acessamos cada campo com o
operador PONTO (`.`):

```c
#include <stdio.h>
#include <string.h>

typedef struct {
    char  nome[50];
    int   idade;
    float altura;
} Pessoa;

int main(void) {
    Pessoa p1;                       // declara uma variavel do tipo Pessoa

    // Acesso por PONTO: variavel.campo
    strcpy(p1.nome, "Franco");       // string: use strcpy, NUNCA p1.nome = "..."
    p1.idade  = 20;                  // int: atribuicao direta
    p1.altura = 1.75;                // float: atribuicao direta

    printf("Nome: %s\n", p1.nome);   // %s para string
    printf("Idade: %d\n", p1.idade); // %d para int
    printf("Altura: %.2f\n", p1.altura); // %.2f para float com 2 casas

    return 0;
}
```

> PEGADINHA: para campos do tipo string (char[]), voce NAO pode fazer
> `p1.nome = "Franco";`. Strings em C sao vetores; use `strcpy(p1.nome, "Franco");`.
> Atribuicao com `=` so funciona para tipos primitivos (int, float, char).

---

## 4. [parametro struct] STRUCT COMO PARAMETRO DE FUNCAO (valor vs referencia)

### Por VALOR (copia - a funcao nao altera o original)

Quando passamos a struct por valor, a funcao recebe uma COPIA. Alteracoes feitas
dentro da funcao NAO afetam a variavel original do main.

```c
void aniversario(Pessoa p) {   // recebe COPIA
    p.idade++;                  // altera a COPIA, o original nao muda
}

// no main:
aniversario(p1);               // p1.idade continua o mesmo!
```

Use por valor quando a funcao so precisa LER os dados (ex: imprimir).

### Por REFERENCIA (ponteiro - a funcao altera o original)

Para ALTERAR a struct original, passamos o ENDERECO (&) e usamos o operador SETA (->)
dentro da funcao.

```c
void aniversario(Pessoa *p) {  // recebe PONTEIRO (endereco)
    p->idade++;                 // seta: acessa campo apontado por p
}

// no main:
aniversario(&p1);              // passa o endereco de p1 -> p1.idade muda de verdade
```

> REGRA DE OURO: se a funcao vai MODIFICAR a struct, passe por ponteiro (& e ->).
> Se so vai LER, pode passar por valor (mais simples, mas faz copia - cuidado com
> structs grandes, que ficam lentas ao copiar).

### Exemplo completo: imprimir (valor) vs alterar (referencia)

```c
#include <stdio.h>
#include <string.h>

typedef struct {
    char  nome[50];
    int   idade;
    float nota;
} Aluno;

// Por VALOR: so le. Recebe copia, nao altera o original.
void imprimir(Aluno a) {
    printf("Nome: %s | Idade: %d | Nota: %.2f\n", a.nome, a.idade, a.nota);
}

// Por REFERENCIA: altera o original. Recebe endereco, usa ->.
void aumentarNota(Aluno *a, float bonus) {
    a->nota += bonus;            // seta: modifica o campo nota do original
}

int main(void) {
    Aluno al;
    strcpy(al.nome, "Franco");
    al.idade = 20;
    al.nota  = 7.0;

    imprimir(al);                // passa por valor (copia)
    aumentarNota(&al, 1.5);      // passa endereco (&) -> altera al
    imprimir(al);                // agora nota vale 8.5

    return 0;
}
```

---

## 5. [vetor de struct] VETOR DE STRUCT: cadastro, ler, imprimir, buscar, ordenar

O uso mais classico em prova: um vetor de struct para cadastrar N registros.
Cada posicao do vetor e uma struct completa.

```c
#include <stdio.h>
#include <string.h>

#define MAX 3     // numero de alunos (pequeno para teste)

typedef struct {
    int   matricula;
    char  nome[50];
    float nota;
} Aluno;

// Le um vetor de Aluno do teclado.
void lerAlunos(Aluno v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("\n--- Aluno %d ---\n", i + 1);
        printf("Matricula: ");
        scanf("%d", &v[i].matricula);     // & porque matricula e int
        printf("Nome: ");
        scanf(" %[^\n]", v[i].nome);      // espaco+%[^\n] le nome com espacos
        printf("Nota: ");
        scanf("%f", &v[i].nota);          // & porque nota e float
    }
}

// Imprime o vetor inteiro.
void imprimirAlunos(Aluno v[], int n) {
    int i;
    printf("\nLista de alunos:\n");
    for (i = 0; i < n; i++) {
        printf("%d | %s | %.2f\n",
               v[i].matricula, v[i].nome, v[i].nota);
    }
}

int main(void) {
    Aluno turma[MAX];          // vetor de MAX alunos
    lerAlunos(turma, MAX);     // passa o vetor (ja e endereco)
    imprimirAlunos(turma, MAX);
    return 0;
}
```

> DETALHE importante: quando passamos um VETOR de struct para uma funcao, nao
> usamos `&`. O nome do vetor ja e o endereco do primeiro elemento:
> `lerAlunos(turma, MAX)` - sem &. Dentro da funcao, `v[i].campo` acessa direto
> (nao precisa de ->, porque v[i] e uma struct, nao um ponteiro).

---

## 6. [seta] [->] [ponteiro struct] PONTEIRO PARA STRUCT E OPERADOR SETA (->)

Este e um dos pontos MAIS cobrados. Decor a regra:

- **PONTO (`.`)**: quando voce tem a STRUCT DIRETA (variavel ou v[i]).
- **SETA (`->`)**: quando voce tem um PONTEIRO para struct.

### Comparacao lado a lado

```c
Pessoa p1;        // variavel direta
Pessoa *ptr;      // ponteiro
ptr = &p1;        // ptr aponta para p1

// Acesso ao campo "idade":
p1.idade;         // PONTO: variavel direta
ptr->idade;       // SETA: ponteiro
(*ptr).idade;     // equivalente a ptr->idade (raramente usado)
```

> A seta `->` e um atalho para `(*ptr).campo`. Os dois sao a mesma coisa,
> mas `->` e mais legivel e e o que cai na prova.

### Quando usar cada um (DECORE ISSO)

| Situacao                              | Operador | Exemplo        |
|---------------------------------------|----------|----------------|
| Variavel simples: `Pessoa p;`         | ponto    | `p.idade`      |
| Vetor por indice: `v[i]`              | ponto    | `v[i].idade`   |
| Ponteiro: `Pessoa *p;`                | seta     | `p->idade`     |
| Funcao recebe ponteiro: `f(Pessoa *p)`| seta     | `p->idade`     |

### Exemplo: funcao que recebe struct por ponteiro e altera

```c
#include <stdio.h>
#include <string.h>

typedef struct {
    char  nome[50];
    int   idade;
} Pessoa;

// Recebe PONTEIRO -> usa SETA para alterar o original.
void fazerAniversario(Pessoa *p) {
    p->idade++;                       // seta: modifica idade do original
    printf("%s fez aniversario! Agora tem %d anos.\n", p->nome, p->idade);
}

int main(void) {
    Pessoa p1;
    strcpy(p1.nome, "Franco");
    p1.idade = 20;

    fazerAniversario(&p1);           // passa ENDERECO (&)
    printf("No main: %d\n", p1.idade); // 21 - alterou de verdade
    return 0;
}
```

### Erro classico: usar ponto quando deveria ser seta

```c
void erro(Pessoa *p) {
    p.idade = 10;     // ERRADO! p e ponteiro, nao struct direta.
    p->idade = 10;    // CERTO.
}
```

---

## 7. [struct aninhada] STRUCT ANINHADA (struct dentro de struct)

Uma struct pode ter como campo OUTRA struct. Exemplo classico: Endereco dentro
de Pessoa.

```c
#include <stdio.h>
#include <string.h>

typedef struct {
    char rua[50];
    char cidade[30];
    int  cep;
} Endereco;                       // primeiro definimos Endereco

typedef struct {
    char     nome[50];
    int      idade;
    Endereco endereco;            // campo do tipo Endereco (struct aninhada)
} Pessoa;                         // depois Pessoa, que contem um Endereco
```

### Acesso: encadeie os pontos (p.endereco.rua)

```c
int main(void) {
    Pessoa p1;

    strcpy(p1.nome, "Franco");
    p1.idade = 20;

    // Acesso ao campo aninhado: ponto em CADA nivel.
    strcpy(p1.endereco.rua, "Rua das Flores");
    strcpy(p1.endereco.cidade, "Caxias do Sul");
    p1.endereco.cep = 95000000;

    printf("%s mora em %s, %s - CEP %d\n",
           p1.nome,
           p1.endereco.rua,
           p1.endereco.cidade,
           p1.endereco.cep);

    return 0;
}
```

> REGRA: para struct aninhada, basta encadear pontos: `p.endereco.rua`.
> Se fosse ponteiro externo: `p->endereco.rua` (seta no primeiro nivel, ponto
> nos demais, porque endereco e struct direta dentro de Pessoa).

---

## 8. [retorno struct] STRUCT COMO RETORNO DE FUNCAO

Em C, uma funcao pode RETORNAR uma struct inteira. Isso e util para funcoes que
"constroem" um registro.

```c
#include <stdio.h>
#include <string.h>

typedef struct {
    char  nome[50];
    int   idade;
    float nota;
} Aluno;

// Funcao que cria e RETORNA um Aluno.
Aluno criarAluno(char nome[], int idade, float nota) {
    Aluno a;
    strcpy(a.nome, nome);     // string: strcpy
    a.idade = idade;          // int: =
    a.nota  = nota;           // float: =
    return a;                 // retorna a struct inteira
}

int main(void) {
    Aluno a1;
    a1 = criarAluno("Franco", 20, 8.5);   // recebe a struct retornada
    printf("%s | %d | %.2f\n", a1.nome, a1.idade, a1.nota);
    return 0;
}
```

> OBS: retornar struct por valor faz uma COPIA. Para structs grandes, e mais
> eficiente passar por ponteiro e preencher no lugar (`void criarAluno(Aluno *a, ...)`).
> Mas para prova, o retorno direto e valido e mais simples de ler.

---

## 9. [ordenar struct] ORDENAR VETOR DE STRUCT POR CAMPO

### 9a. Ordenar por campo NUMERICO (bubble sort por nota)

```c
// Bubble sort crescente pela NOTA.
void ordenarPorNota(Aluno v[], int n) {
    int i, j;
    Aluno aux;                 // auxiliar do tipo Aluno (nao do tipo do campo!)
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (v[j].nota > v[j + 1].nota) {   // compara o CAMPO nota
                aux     = v[j];               // troca a STRUCT INTEIRA
                v[j]    = v[j + 1];
                v[j+1]  = aux;
            }
        }
    }
}
```

> DETALHE que cai: o `aux` deve ser do tipo da STRUCT (`Aluno aux`), nao do tipo
> do campo. Trocamos o registro inteiro de posicao, nao so a nota. Se trocar so
> a nota, os nomes ficam trocados (bug classico).

### 9b. Ordenar por campo STRING (usando strcmp) - [strcmp struct]

Para ordenar por nome (string), a comparacao muda: usamos `strcmp`.

```c
#include <string.h>

// Bubble sort crescente pelo NOME (alfabetico).
void ordenarPorNome(Aluno v[], int n) {
    int i, j;
    Aluno aux;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            // strcmp < 0  -> v[j] vem ANTES de v[j+1] (ja em ordem)
            // strcmp > 0  -> v[j] vem DEPOIS  de v[j+1] (trocar)
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                aux     = v[j];
                v[j]    = v[j + 1];
                v[j+1]  = aux;
            }
        }
    }
}
```

> RELEMBRE o strcmp (pegadinha confirmada na prova):
> - `strcmp(a, b) == 0`  -> strings IGUAIS
> - `strcmp(a, b) < 0`   -> a vem ANTES de b (a < b alfabeticamente)
> - `strcmp(a, b) > 0`   -> a vem DEPOIS de b (a > b alfabeticamente)
>
> NUNCA compare strings com `==` (isso compara ENDERECOS, nao conteudo).
> O correto e `if (strcmp(a, b) == 0)`, e NAO `if (strcmp(a, b))` - este ultimo
> e VERDADEIRO quando as strings sao DIFERENTES (o oposto do que se quer!).

---

## 10. [buscar struct] BUSCAR STRUCT POR NOME (strcmp) E POR CODIGO (==)

### 10a. Buscar por codigo (campo int) - comparacao com ==

```c
// Retorna o INDICE do aluno com a matricula buscada, ou -1 se nao achar.
int buscarPorMatricula(Aluno v[], int n, int matBuscada) {
    int i;
    for (i = 0; i < n; i++) {
        if (v[i].matricula == matBuscada) {   // int: == funciona
            return i;                          // achou: retorna indice
        }
    }
    return -1;                                 // nao achou
}
```

### 10b. Buscar por nome (campo string) - comparacao com strcmp

```c
// Retorna o INDICE do aluno com o nome buscado, ou -1 se nao achar.
int buscarPorNome(Aluno v[], int n, char nomeBuscado[]) {
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(v[i].nome, nomeBuscado) == 0) {  // string: strcmp == 0
            return i;
        }
    }
    return -1;
}
```

> PEGADINHA: `if (v[i].nome == nomeBuscado)` esta ERRADO - compara enderecos de
> memoria, nunca vai dar igual. Use `strcmp(...) == 0`.

### Uso no main

```c
int pos = buscarPorNome(turma, MAX, "Franco");
if (pos != -1) {
    printf("Achou na posicao %d: nota %.2f\n", pos, turma[pos].nota);
} else {
    printf("Nao encontrado.\n");
}
```

---

## 11. [media struct] CALCULAR MEDIA / SOMA DE UM CAMPO NO VETOR DE STRUCT

```c
// Soma todas as notas e retorna a media.
float mediaNotas(Aluno v[], int n) {
    int i;
    float soma = 0.0;
    for (i = 0; i < n; i++) {
        soma += v[i].nota;        // acumula o campo nota de cada aluno
    }
    return soma / n;              // media = soma / quantidade
}

// No main:
float m = mediaNotas(turma, MAX);
printf("Media da turma: %.2f\n", m);
```

> CUIDADO com divisao por zero: se `n == 0`, `soma / n` da problema. Em codigo
> de prova, geralmente n > 0, mas vale proteger:
> `if (n == 0) return 0.0;` antes de dividir.

---

## 12. [maior struct] ENCONTRAR MAIOR / MENOR POR UM CAMPO

### 12a. Maior nota (ou maior salario, mais velho, etc.)

```c
// Retorna o INDICE do aluno com a MAIOR nota.
int indiceMaiorNota(Aluno v[], int n) {
    int i, idxMaior = 0;          // comeca assumindo que o primeiro e o maior
    for (i = 1; i < n; i++) {     // comeca do 1 (o 0 ja e o maior por enquanto)
        if (v[i].nota > v[idxMaior].nota) {
            idxMaior = i;          // atualiza o indice do maior
        }
    }
    return idxMaior;
}
```

### 12b. Menor nota (ou mais novo, menor salario)

```c
int indiceMenorNota(Aluno v[], int n) {
    int i, idxMenor = 0;
    for (i = 1; i < n; i++) {
        if (v[i].nota < v[idxMenor].nota) {   // so inverte > para <
            idxMenor = i;
        }
    }
    return idxMenor;
}
```

### 12c. Mais velho (campo idade) - mesma logica, so muda o campo

```c
int indiceMaisVelho(Pessoa v[], int n) {
    int i, idx = 0;
    for (i = 1; i < n; i++) {
        if (v[i].idade > v[idx].idade) {   // compara o campo idade
            idx = i;
        }
    }
    return idx;
}
```

> PADRAO: retornar o INDICE e melhor que retornar o valor, porque com o indice
> voce consegue acessar TODOS os campos daquele registro (nome, nota, etc.).

---

## 13. [erros comuns] ERROS COMUNS (caem em analise de codigo)

### Erro 1: Comparar string com ==

```c
if (v[i].nome == "Franco") ...        // ERRADO (compara enderecos)
if (strcmp(v[i].nome, "Franco") == 0) // CERTO
```

### Erro 2: Esquecer o & ao passar struct por referencia

```c
aniversario(p1);      // ERRADO: a funcao espera ponteiro, falta o &
aniversario(&p1);     // CERTO
```

### Erro 3: Usar ponto (.) quando a variavel e ponteiro

```c
void f(Pessoa *p) {
    p.idade = 10;      // ERRADO: p e ponteiro, deveria ser ->
    p->idade = 10;     // CERTO
}
```

### Erro 4: Usar seta (->) quando a variavel e struct direta

```c
Pessoa p1;
p1->idade = 10;       // ERRADO: p1 nao e ponteiro
p1.idade = 10;        // CERTO
```

### Erro 5: typedef struct sem alias

```c
typedef struct { int x; };   // ERRADO em C puro (falta o nome do alias)
typedef struct { int x; } Tipo; // CERTO
```

### Erro 6: Atribuir string com =

```c
p1.nome = "Franco";          // ERRADO (vetor nao recebe =)
strcpy(p1.nome, "Franco");    // CERTO
```

### Erro 7: Trocar so o campo na ordenacao (em vez da struct inteira)

```c
// ERRADO: troca so a nota, nomes ficam trocados.
aux_nota = v[j].nota;
v[j].nota = v[j+1].nota;
v[j+1].nota = aux_nota;

// CERTO: troca a STRUCT INTEIRA.
Aluno aux = v[j];
v[j]    = v[j+1];
v[j+1]  = aux;
```

### Erro 8: if(strcmp) em vez de if(strcmp == 0)

```c
if (strcmp(a, b)) ...        // VERDADEIRO quando sao DIFERENTES (oposto do esperado!)
if (strcmp(a, b) == 0) ...  // VERDADEIRO quando sao IGUAIS (o que se quer)
```

> Esta e a pegadinha CLASSICA de concurso. `if (strcmp(a,b))` executa o bloco
> quando as strings sao DIFERENTES, porque strcmp retorna nao-zero (verdadeiro)
> para strings diferentes. Para "entrar se forem iguais", use `== 0`.

### Erro 9: Esquecer o espaco antes de %[^\n] no scanf

```c
scanf("%[^\n]", v[i].nome);      // ERRADO: le o \n pendente e nao le nada
scanf(" %[^\n]", v[i].nome);     // CERTO: o espaco descarta o \n pendente
```

### Erro 10: Declarar aux do tipo errado na ordenacao

```c
int aux;            // ERRADO: precisa ser do tipo da struct
Aluno aux;          // CERTO: troca-se a struct inteira
```

---

## RESUMO FINAL (leia na hora da prova)

- struct agrupa dados RELACIONADOS de tipos DIFERENTES (vantagem teorica).
- Sintaxe: `typedef struct { campos; } NomeTipo;` - alias no final.
- Acesso: PONTO para struct direta (`p.campo`, `v[i].campo`); SETA para ponteiro (`p->campo`).
- Por valor: copia, nao altera original. Por ponteiro: altera original (use & e ->).
- Vetor de struct: `v[i].campo`, sem & ao passar o vetor para funcao.
- Struct aninhada: encadeie pontos (`p.endereco.rua`).
- Ordenar: troque a STRUCT INTEIRA (aux do tipo da struct), nao so o campo.
- Comparar string: SEMPRE `strcmp(...) == 0`, nunca `==`.
- Buscar por int: `==`. Buscar por string: `strcmp == 0`.
- Maior/menor: guarde o INDICE, nao o valor.
- typedef sem alias NAO compila em C puro.

---

## PALAVRAS-CHAVE PARA Ctrl+F

struct, typedef, ponto, seta, ->, ponteiro struct, vetor de struct,
struct aninhada, ordenar struct, strcmp struct, buscar struct, media struct,
maior struct, parametro struct, retorno struct, bubble sort struct,
pegadinha strcmp, pegadinha seta, pegadinha typedef, struct por referencia,
struct por valor, struct aninhada endereco, maior nota, mais velho, media notas.


#############################################################
# PARTE 3 - STRINGS AVANCADO
#############################################################

# STRINGS AVANCADO - Material de Consulta para Prova GB

> Materia: Algoritmos e Programacao II - Uniftec
> Linguagem: C pura (compila com gcc -Wall -Wextra no Windows/MinGW)
> Aluno: Franco
> Topico: STRINGS AVANCADO (uso exaustivo para prova pratica e teorica)

---

## PALAVRAS-CHAVE PARA Ctrl+F

string, char, fgets, scanf, strlen, strcpy, strncpy, strcat, strncat, strcmp, strncmp, strchr, strrchr, strstr, strtok, sprintf, ctype, toupper, tolower, isalpha, isdigit, isalnum, isspace, isupper, islower, atoi, atof, atol, vogais, inverter, palindromo, validar, busca parcial, CSV, terminador, \0, NULL, buffer, ponteiro, indice, maiuscula, minuscula, delimitador, substring, pontuacao.

---

## 1. O QUE E STRING EM C

Em C **NAO existe** um tipo "string" nativo como em Python, Java ou C#.
Uma string em C e simplesmente um **vetor (array) de char** terminado por um
caractere especial chamado **\0** (caractere nulo, valor ASCII 0).

O **\0** e o "terminador" da string. Ele indica onde a string acaba.
Todas as funcoes da biblioteca `string.h` (strlen, strcpy, strcmp, etc.)
param de ler quando encontram o \0. Se voce esquecer o \0, as funcoes
continuam lendo memoria ate encontrar um byte zero por acaso - isso e
comportamento indefinido (bug perigoso).

### Declaracao basica

```c
#include <stdio.h>

int main(void) {
    // Forma 1: vetor de char com tamanho fixo
    char nome[50];  // comporta 49 caracteres uteis + 1 para o \0

    // Forma 2: inicializar na declaracao (o \0 e colocado automaticamente)
    char frase[] = "Ola mundo";  // tamanho 10: 9 letras + \0

    // Forma 3: inicializar caractere por caractere (precisa colocar \0 manualmente)
    char s[4];
    s[0] = 'O';
    s[1] = 'l';
    s[2] = 'a';
    s[3] = '\0';  // OBRIGATORIO - sem isso a string esta "quebrada"

    printf("frase = %s\n", frase);   // Ola mundo
    printf("s     = %s\n", s);        // Ola
    printf("tamanho de frase[] = %zu bytes\n", sizeof(frase));  // 10

    return 0;
}
```

### Por que o \0 importa tanto?

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char a[10] = "abc";       // a = ['a','b','c','\0','?','?','?','?','?','?']
    char b[10] = "abcdef";    // b = ['a','b','c','d','e','f','\0','?','?','?']

    // strlen le ate achar \0 - nao le o vetor inteiro
    printf("strlen(a) = %zu\n", strlen(a));   // 3
    printf("strlen(b) = %zu\n", strlen(b));   // 6

    // sizeof le o tamanho TOTAL do vetor (nao importa o conteudo)
    printf("sizeof(a) = %zu\n", sizeof(a));  // 10
    printf("sizeof(b) = %zu\n", sizeof(b));  // 10

    return 0;
}
```

**Diferenca chave (cai em prova):**
- `strlen(s)` -> quantidade de caracteres ANTES do \0 (tamanho logico).
- `sizeof(s)` -> quantidade de bytes do vetor inteiro (tamanho fisico).

---

## 2. DECLARAR E LER STRINGS DO TECLADO

### 2.1 scanf com %s - le UMA palavra (sem espaco)

```c
#include <stdio.h>

int main(void) {
    char nome[50];

    printf("Digite seu primeiro nome: ");
    // %49s: le no maximo 49 caracteres (deixa 1 espaco para o \0)
    // Para no primeiro espaco, tab ou enter.
    scanf("%49s", nome);   // nome nao precisa de & (ja e endereco)

    printf("Ola, %s!\n", nome);
    return 0;
}
```

**Atencao:** `scanf("%s", nome)` SEM o numero entre % e s e PERIGOSO -
se o usuario digitar mais caracteres do que o vetor comporta, ocorre
estouro de buffer (buffer overflow). Sempre coloque o limite: `%49s`
para um vetor de 50.

### 2.2 fgets - le FRASE inteira (com espaco)

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char frase[100];

    printf("Digite uma frase: ");
    // fgets(endereco, tamanho_maximo, origem)
    // stdin = teclado. Le ate tamanho-1 caracteres ou ate o ENTER.
    fgets(frase, 100, stdin);

    // PROBLEMA: o fgets GUARDA o \n (enter) no final da string!
    // Ex: se digitar "Ola mundo\n", frase = "Ola mundo\n\0"
    printf("Voce digitou: %s", frase);  // ja tem o \n, nao precisa de \n no printf

    return 0;
}
```

### 2.3 Remover o \n do fgets com strcspn (TECNICA DO PROFESSOR)

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char frase[100];

    printf("Digite uma frase: ");
    fgets(frase, 100, stdin);

    // strcspn(frase, "\n") retorna o INDICE do primeiro \n encontrado.
    // Se nao achar, retorna o tamanho da string. Em ambos os casos,
    // colocar \0 naquela posicao remove o \n (ou nao faz nada se nao houver).
    frase[strcspn(frase, "\n")] = '\0';

    printf("Frase limpa: [%s]\n", frase);  // agora sem o \n
    return 0;
}
```

**Por que usar strcspn e nao `frase[strlen(frase)-1] = '\0'`?**
Porque se o usuario digitar so ENTER, o fgets guarda so `\n\0` e
`strlen-1` seria 0, apagando o \0 e quebrando a string. Com strcspn
e seguro em todos os casos.

### 2.4 PEGADINHA: misturar scanf e fgets (buffer com ENTER)

```c
#include <stdio.h>

int main(void) {
    int idade;
    char nome[50];

    printf("Idade: ");
    scanf("%d", &idade);   // usuario digita "20" e aperta ENTER
    // O scanf le o "20" mas DEIXA o "\n" no buffer do teclado!

    printf("Nome: ");
    fgets(nome, 50, stdin);  // o fgets le o "\n" que sobrou e acha que acabou!
    // RESULTADO: nome fica so com "\n\0" - o fgets nem espera o usuario digitar.

    printf("Idade=%d Nome=[%s]\n", idade, nome);
    return 0;
}
```

**Solucao 1: limpar o buffer depois do scanf**

```c
#include <stdio.h>

// Limpa o buffer do teclado ate encontrar \n ou EOF
void limpar_buffer(void) {
    int c;
    // fflush(stdin) NAO e portatil mas funciona no Windows/MinGW.
    // O loop abaixo e a forma portatil e correta.
    while ((c = getchar()) != '\n' && c != EOF) {
        // descarta cada caractere ate o enter
    }
}

int main(void) {
    int idade;
    char nome[50];

    printf("Idade: ");
    scanf("%d", &idade);
    limpar_buffer();   // <-- limpa o \n que sobrou

    printf("Nome: ");
    fgets(nome, 50, stdin);  // agora funciona certo

    printf("Idade=%d Nome=[%s]\n", idade, nome);
    return 0;
}
```

**Solucao 2 (estilo do professor, com fflush):**

```c
#include <stdio.h>

int main(void) {
    int idade;
    char nome[50];

    printf("Idade: ");
    scanf("%d", &idade);
    fflush(stdin);  // NAO e portatil (nao funciona em Linux), mas funciona no Windows

    printf("Nome: ");
    fgets(nome, 50, stdin);

    printf("Idade=%d Nome=[%s]\n", idade, nome);
    return 0;
}
```

> **Dica de prova:** se a questao mostrar scanf seguido de fgets e o fgets
> "pular" a leitura, o problema e o \n no buffer. A resposta certa e
> limpar o buffer entre os dois.

---

## 3. BIBLIOTECA string.h - FUNCOES PRINCIPAIS

Todas as funcoes abaixo estao em `<string.h>`. Lembre-se: elas param
de processar no \0.

### 3.1 strlen - tamanho da string

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char s[] = "programacao";
    size_t n = strlen(s);   // size_t e o tipo retornado por strlen
    printf("strlen = %zu\n", n);   // 11 (nao conta o \0)
    return 0;
}
```

### 3.2 strcpy - copiar string

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char origem[] = "algoritmos";
    char destino[50];

    strcpy(destino, origem);   // copia origem -> destino (incluindo o \0)
    printf("destino = %s\n", destino);   // algoritmos
    return 0;
}
```

**Cuidado:** o destino precisa ter espaco suficiente. Se a origem for
maior que o destino, ocorre estouro de buffer.

### 3.3 strncpy - copiar com limite (mais seguro)

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char origem[] = "algoritmos e programacao";
    char destino[10];

    strncpy(destino, origem, sizeof(destino) - 1);
    destino[sizeof(destino) - 1] = '\0';  // strncpy NAO coloca \0 se cortar!
    printf("destino = %s\n", destino);   // algoritmo (9 letras + \0)
    return 0;
}
```

> **Pegadinha de strncpy:** se a origem for maior que o limite, o strncpy
> NAO adiciona o \0 automaticamente. Por isso, sempre coloque
> `destino[n-1] = '\0'` depois.

### 3.4 strcat - concatenar (juntar no final)

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char s[50] = "Ola, ";   // precisa ter espaco sobrando
    char nome[] = "Franco!";

    strcat(s, nome);   // junta nome no final de s
    printf("%s\n", s);   // Ola, Franco!
    return 0;
}
```

### 3.5 strncat - concatenar com limite

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char s[20] = "Ola, ";
    char nome[] = "Franco da Silva Sauro";

    strncat(s, nome, 6);   // junta no maximo 6 caracteres de nome
    printf("%s\n", s);   // Ola, Franco
    return 0;
}
```

### 3.6 strcmp - comparar strings (ver secao 4 para detalhes)

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char a[] = "abc";
    char b[] = "abc";
    char c[] = "abd";

    printf("strcmp(a,b) = %d\n", strcmp(a, b));   // 0  (iguais)
    printf("strcmp(a,c) = %d\n", strcmp(a, c));   // <0 (a < c)
    printf("strcmp(c,a) = %d\n", strcmp(c, a));   // >0 (c > a)
    return 0;
}
```

### 3.7 strncmp - comparar so os primeiros N caracteres

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char a[] = "programacao";
    char b[] = "procurar";

    // Compara so os 3 primeiros caracteres
    printf("strncmp(a,b,3) = %d\n", strncmp(a, b, 3));   // 0 (iguais nos 3)
    return 0;
}
```

### 3.8 strchr - achar a PRIMEIRA ocorrencia de um caractere

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char s[] = "programacao";
    char *p = strchr(s, 'a');   // procura 'a' (primeira ocorrencia)

    if (p != NULL) {
        printf("Achou 'a' na posicao %ld\n", p - s);   // 5
        printf("Resto a partir do 'a': %s\n", p);       // acao
    } else {
        printf("Nao achou\n");
    }
    return 0;
}
```

> `strchr` retorna um PONTEIRO para o caractere encontrado, ou NULL se
> nao achar. Para descobrir o INDICE, fazemos `p - s` (aritmetica de
> ponteiros).

### 3.9 strrchr - achar a ULTIMA ocorrencia de um caractere

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char s[] = "programacao";
    char *p = strrchr(s, 'a');   // procura 'a' (ultima ocorrencia)

    if (p != NULL) {
        printf("Ultimo 'a' na posicao %ld\n", p - s);   // 8
        printf("Resto: %s\n", p);   // ao
    }
    return 0;
}
```

### 3.10 strstr - achar SUBSTRING (busca parcial) - MUITO IMPORTANTE

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char frase[] = "O rato roeu a roupa do rei de Roma";
    char *p = strstr(frase, "roupa");   // procura a palavra "roupa"

    if (p != NULL) {
        printf("Achou na posicao %ld\n", p - frase);
        printf("A partir dali: %s\n", p);   // roupa do rei de Roma
    } else {
        printf("Nao achou\n");
    }
    return 0;
}
```

> `strstr` e a funcao para BUSCA PARCIAL. Se voce quer saber se um nome
> "contem" um trecho (ex: todos os nomes que contem "sil"), usa strstr.
> Veja a secao 7 para o exemplo completo do projeto.

### 3.11 strtok - separar string por DELIMITADOR (modelo CSV)

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char linha[] = "10,Franco,1500.50";
    char *token;

    // PRIMEIRA chamada: passa a string original
    token = strtok(linha, ",");

    while (token != NULL) {
        printf("Token: [%s]\n", token);
        // PROXIMAS chamadas: passa NULL (continua de onde parou)
        token = strtok(NULL, ",");
    }
    return 0;
}
```

Saida:
```
Token: [10]
Token: [Franco]
Token: [1500.50]
```

> **Padrao do strtok (cai em prova):**
> 1. Primeira chamada: `strtok(string, delimitador)` - passa a string.
> 2. Proximas chamadas: `strtok(NULL, delimitador)` - passa NULL.
> 3. Retorna NULL quando nao ha mais tokens.
> 4. **MODIFICA a string original** colocando \0 no lugar dos delimitadores.

### 3.12 sprintf - montar string formatada (NAO imprime, guarda)

```c
#include <stdio.h>

int main(void) {
    char linha[100];
    int codigo = 10;
    char nome[] = "Franco";
    float salario = 1500.50;

    // sprintf funciona como printf, mas guarda o resultado em "linha"
    sprintf(linha, "%d,%s,%.2f", codigo, nome, salario);

    printf("Linha montada: %s\n", linha);   // 10,Franco,1500.50
    return 0;
}
```

> `sprintf` e util para montar linhas de CSV, mensagens de log, ou
> formatar dados antes de gravar em arquivo. Veja a secao 9.

---

## 4. strcmp EM DETALHE - QUESTAO TEORICA CONFIRMADA

### 4.1 O que strcmp retorna

```c
int strcmp(const char *s1, const char *s2);
```

- Retorna **0** se as strings forem **iguais**.
- Retorna **< 0** (negativo) se `s1` for **menor** que `s2` (vem antes na ordem alfabetica/ASCII).
- Retorna **> 0** (positivo) se `s1` for **maior** que `s2` (vem depois).

A comparacao e caractere por caractere usando o valor ASCII. Por isso
letras maiusculas vêm antes de minusculas ('A'=65, 'a'=97).

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    printf("strcmp(\"abc\",\"abc\") = %d\n", strcmp("abc", "abc"));   // 0
    printf("strcmp(\"abc\",\"abd\") = %d\n", strcmp("abc", "abd"));   // negativo
    printf("strcmp(\"abd\",\"abc\") = %d\n", strcmp("abd", "abc"));   // positivo
    printf("strcmp(\"ABC\",\"abc\") = %d\n", strcmp("ABC", "abc"));   // negativo (case-sensitive!)
    return 0;
}
```

### 4.2 A PEGADINHA CLASSICA (cai em prova de concurso)

Em C, **qualquer valor diferente de zero e VERDADEIRO**. So o zero e falso.

`strcmp(a, b)` retorna:
- 0 se iguais
- !=0 se diferentes

Entao:

```c
// ERRADO - logica invertida!
if (strcmp(a, b)) {
    // isso executa quando as strings SAO DIFERENTES (nao quando sao iguais!)
    printf("As strings sao iguais\n");   // MENTIRA - sao diferentes!
}
```

Por que? Porque `strcmp` retorna !=0 quando diferentes, e !=0 e verdadeiro
em C. Entao o `if` entra quando as strings DIFEREM.

```c
// CORRETO - testar igualdade com == 0
if (strcmp(a, b) == 0) {
    printf("As strings sao iguais\n");   // agora sim!
}
```

E para testar diferenca:

```c
// CORRETO - testar diferenca com != 0
if (strcmp(a, b) != 0) {
    printf("As strings sao diferentes\n");
}
```

### 4.3 Exemplo completo do erro comum vs correto

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char senha_digitada[] = "1234";
    char senha_correta[] = "1234";

    // ERRO COMUM (logica invertida):
    if (strcmp(senha_digitada, senha_correta)) {
        // strcmp retorna 0 (iguais) -> 0 e FALSO -> NAO entra aqui
        // strcmp retorna !=0 (diferentes) -> entra aqui
        printf("ERRO: isso so roda se forem DIFERENTES\n");
    }

    // CORRETO:
    if (strcmp(senha_digitada, senha_correta) == 0) {
        printf("OK: senha correta (iguais)\n");
    } else {
        printf("Erro: senha incorreta (diferentes)\n");
    }

    return 0;
}
```

### 4.4 Formato de questao de prova (assercao com PORQUE)

> **Assercao I:** A expressao `if (strcmp(s1, s2))` executa o bloco do `if`
> quando as strings `s1` e `s2` sao diferentes.
> **PORQUE**
> **Assercao II:** A funcao `strcmp` retorna 0 quando as strings sao iguais e
> um valor diferente de zero quando sao diferentes, e em C qualquer valor
> diferente de zero e considerado verdadeiro.

Resposta: **As duas assercoes sao verdadeiras, e a II e uma justificativa
correta da I.** A assercao I esta correta porque strcmp retorna !=0 quando
diferem, e !=0 e verdadeiro em C, fazendo o bloco do `if` executar.

---

## 5. BIBLIOTECA ctype.h - CLASSIFICACAO E CONVERSAO DE CARACTERES

A biblioteca `<ctype.h>` tem funcoes que analisam ou transformam UM
caractere por vez. Todas recebem um `int` (o caractere) e retornam `int`
(0 = falso, !=0 = verdadeiro).

### 5.1 isalpha - e letra?

```c
#include <stdio.h>
#include <ctype.h>

int main(void) {
    char c = 'A';
    if (isalpha(c)) {        // verdadeiro se for letra (A-Z, a-z)
        printf("%c e letra\n", c);
    }
    return 0;
}
```

### 5.2 isdigit - e digito (0-9)?

```c
#include <stdio.h>
#include <ctype.h>

int main(void) {
    char c = '7';
    if (isdigit(c)) {        // verdadeiro se for '0' a '9'
        printf("%c e digito\n", c);
    }
    return 0;
}
```

### 5.3 isalnum - e letra OU digito?

```c
#include <stdio.h>
#include <ctype.h>

int main(void) {
    char c = 'a';
    if (isalnum(c)) {        // verdadeiro se for letra ou digito
        printf("%c e alfanumerico\n", c);
    }
    return 0;
}
```

### 5.4 isspace - e espaco/tab/enter?

```c
#include <stdio.h>
#include <ctype.h>

int main(void) {
    char c = ' ';
    if (isspace(c)) {        // verdadeiro para ' ', '\t', '\n', '\r', '\f', '\v'
        printf("e espaco em branco\n");
    }
    return 0;
}
```

### 5.5 isupper / islower - e maiuscula / minuscula?

```c
#include <stdio.h>
#include <ctype.h>

int main(void) {
    char c = 'Z';
    if (isupper(c)) {        // verdadeiro se for maiuscula (A-Z)
        printf("%c e maiuscula\n", c);
    }
    if (islower(c)) {        // falso aqui
        printf("%c e minuscula\n", c);
    }
    return 0;
}
```

### 5.6 toupper / tolower - converter maiuscula/minuscula

```c
#include <stdio.h>
#include <ctype.h>

int main(void) {
    char c = 'a';
    char maiuscula = (char) toupper(c);   // 'A'
    char minuscula = (char) tolower('Z');  // 'z'

    printf("toupper('a') = %c\n", maiuscula);   // A
    printf("tolower('Z') = %c\n", minuscula);  // z
    return 0;
}
```

> **Atencao:** `toupper` e `tolower` retornam `int`. Em C, e comum fazer
> cast para `char` ao atribuir, embora nao seja obrigatorio. Se o
> caractere ja for maiuscula, `toupper` retorna ele mesmo (nao quebra).

### 5.7 Exemplo: converter frase inteira para maiuscula

```c
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(void) {
    char frase[] = "Ola Mundo 123";
    int i;

    for (i = 0; i < (int) strlen(frase); i++) {
        frase[i] = (char) toupper((unsigned char) frase[i]);
    }
    printf("%s\n", frase);   // OLA MUNDO 123
    return 0;
}
```

> **Dica:** passar `unsigned char` para as funcoes do ctype evita bugs
> com caracteres acentuados/negativos. Em prova, geralmente nao precisa,
> mas e boa pratica.

---

## 6. MANIPULACAO MANUAL DE STRING (sem biblioteca)

Esta secao e MUITO cobrada em prova pratica. O professor quer ver se voce
entende como a string funciona "por dentro", sem usar string.h.

### 6.1 Contar caracteres ate o \0 (strlen manual)

```c
#include <stdio.h>

// Implementacao manual do strlen
int meu_strlen(const char *s) {
    int i = 0;
    while (s[i] != '\0') {   // conta ate achar o terminador
        i++;
    }
    return i;
}

int main(void) {
    char s[] = "programacao";
    printf("Tamanho = %d\n", meu_strlen(s));   // 11
    return 0;
}
```

### 6.2 Contar vogais (sem ctype)

```c
#include <stdio.h>

int contar_vogais(const char *s) {
    int i, cont = 0;
    for (i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            cont++;
        }
    }
    return cont;
}

int main(void) {
    char s[] = "Algoritmos II";
    printf("Vogais: %d\n", contar_vogais(s));   // 5 (A, o, i, o, I)
    return 0;
}
```

### 6.3 Contar vogais (com ctype - mais elegante)

```c
#include <stdio.h>
#include <ctype.h>

int contar_vogais(const char *s) {
    int i, cont = 0;
    for (i = 0; s[i] != '\0'; i++) {
        char c = (char) tolower((unsigned char) s[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            cont++;
        }
    }
    return cont;
}

int main(void) {
    char s[] = "Algoritmos II";
    printf("Vogais: %d\n", contar_vogais(s));   // 5
    return 0;
}
```

### 6.4 Inverter string (com 2 indices)

```c
#include <stdio.h>
#include <string.h>

void inverter(char *s) {
    int i = 0;                    // inicio
    int j = (int) strlen(s) - 1; // fim

    while (i < j) {
        // troca s[i] com s[j]
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

int main(void) {
    char s[] = "Franco";
    inverter(s);
    printf("Invertida: %s\n", s);   // ocnarF
    return 0;
}
```

### 6.5 Inverter string (com 2 ponteiros - alternativa)

```c
#include <stdio.h>
#include <string.h>

void inverter_ptr(char *s) {
    char *inicio = s;
    char *fim = s + strlen(s) - 1;   // aponta para o ultimo caractere

    while (inicio < fim) {
        char tmp = *inicio;
        *inicio = *fim;
        *fim = tmp;
        inicio++;
        fim--;
    }
}

int main(void) {
    char s[] = "roma";
    inverter_ptr(s);
    printf("%s\n", s);   // amor
    return 0;
}
```

### 6.6 Contar palavras (separadas por espaco)

```c
#include <stdio.h>
#include <ctype.h>

int contar_palavras(const char *s) {
    int i = 0, cont = 0;
    int dentro_palavra = 0;   // 0 = fora de palavra, 1 = dentro

    while (s[i] != '\0') {
        if (isspace((unsigned char) s[i])) {
            dentro_palavra = 0;   // achou espaco -> saiu da palavra
        } else {
            if (dentro_palavra == 0) {
                cont++;            // comecou uma nova palavra
                dentro_palavra = 1;
            }
        }
        i++;
    }
    return cont;
}

int main(void) {
    char s[] = "O rato roeu a roupa";
    printf("Palavras: %d\n", contar_palavras(s));   // 5
    return 0;
}
```

> Logica: conta +1 toda vez que passa de "espaco" para "caractere".
> Assim funciona mesmo com varios espacos seguidos.

### 6.7 Converter para maiuscula manualmente

```c
#include <stdio.h>

void para_maiuscula(char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            // 'a' - 'A' = 32 na tabela ASCII
            // entao s[i] - 32 converte minuscula em maiuscula
            s[i] = (char) (s[i] - ('a' - 'A'));
        }
    }
}

int main(void) {
    char s[] = "Franco 123";
    para_maiuscula(s);
    printf("%s\n", s);   // FRANCO 123
    return 0;
}
```

> **Por que `s[i] - 32`?** Na tabela ASCII, 'A'=65 e 'a'=97. A diferenca
> e 32. Entao para converter minuscula -> maiuscula, subtrai 32.
> Para maiuscula -> minuscula, soma 32. (Mas usar toupper/tolower e mais seguro.)

### 6.8 Verificar palindromo

Palindromo = palavra que se le igual de tras pra frente (ex: "ovo", "ana").

```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Retorna 1 se for palindromo, 0 se nao for
int eh_palindromo(const char *s) {
    int i = 0;
    int j = (int) strlen(s) - 1;

    while (i < j) {
        // Ignora espacos (opcional - depende do enunciado)
        while (i < j && s[i] == ' ') i++;
        while (i < j && s[j] == ' ') j--;

        // Compara em maiuscula para ser case-insensitive
        char ci = (char) toupper((unsigned char) s[i]);
        char cj = (char) toupper((unsigned char) s[j]);

        if (ci != cj) {
            return 0;   // achou diferenca -> nao e palindromo
        }
        i++;
        j--;
    }
    return 1;   // passou por tudo sem achar diferenca
}

int main(void) {
    char palavra[][20] = {"ovo", "ana", "franco", "Ovo", "A nutria"};
    int i;

    for (i = 0; i < 5; i++) {
        if (eh_palindromo(palavra[i])) {
            printf("\"%s\" e palindromo\n", palavra[i]);
        } else {
            printf("\"%s\" NAO e palindromo\n", palavra[i]);
        }
    }
    return 0;
}
```

Saida:
```
"ovo" e palindromo
"ana" e palindromo
"franco" NAO e palindromo
"Ovo" e palindromo
"A nutria" NAO e palindromo
```

---

## 7. BUSCA PARCIAL DE NOME COM strstr (MODELO PROJETO FUTEBOL)

Cenario: voce tem um vetor de structs com jogadores e quer achar todos
os nomes que CONTENM um trecho digitado pelo usuario (ex: "sil" acha
"Silva", "Silvio", "Silas").

```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Exemplo: busca parcial em vetor de structs (modelo projeto futebol)

typedef struct {
    int codigo;
    char nome[50];
    char posicao[20];
    float salario;
} Jogador;

// Funcao que converte string para minuscula (para busca case-insensitive)
void para_minuscula(char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        s[i] = (char) tolower((unsigned char) s[i]);
    }
}

int main(void) {
    Jogador time[5] = {
        {1, "Silva Santos", "Goleiro", 5000.00},
        {2, "Joao Silvio", "Zagueiro", 4000.00},
        {3, "Pedro Souza", "Meia", 6000.00},
        {4, "Marcos Silas", "Atacante", 7000.00},
        {5, "Ana Paula", "Tecnica", 8000.00}
    };

    char busca[50];
    printf("Digite parte do nome para buscar: ");
    fgets(busca, 50, stdin);
    busca[strcspn(busca, "\n")] = '\0';   // remove o \n do fgets

    // Converte a busca para minuscula
    para_minuscula(busca);

    printf("\nResultados encontrados:\n");
    int i, achou = 0;
    for (i = 0; i < 5; i++) {
        // Copia o nome para nao destruir o original
        char nome_minus[50];
        strcpy(nome_minus, time[i].nome);
        para_minuscula(nome_minus);

        // strstr retorna NULL se nao achar, ou ponteiro se achar
        if (strstr(nome_minus, busca) != NULL) {
            printf("Codigo %d: %s (%s) - R$ %.2f\n",
                   time[i].codigo, time[i].nome, time[i].posicao, time[i].salario);
            achou = 1;
        }
    }

    if (!achou) {
        printf("Nenhum jogador encontrado com \"%s\".\n", busca);
    }

    return 0;
}
```

> **Pontos-chave da busca parcial:**
> 1. Usa `strstr(nome, busca)` - retorna NULL se nao achar.
> 2. Para busca case-insensitive (ignorar maiuscula/minuscula), converte
>    AMBAS as strings para minuscula antes de comparar.
> 3. Nao destrua o original - copie para uma variavel auxiliar.

---

## 8. strtok PARA SEPARAR CSV (MODELO funcionario.txt DO PROFESSOR)

Cenario: ler uma linha de arquivo CSV no formato
`codigo,nome,salario` e separar os campos, convertendo os numeros.

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>   // atoi, atof

typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

int main(void) {
    // Linha simulando o que viria do arquivo funcionario.txt
    char linha[] = "10,Franco da Silva,1500.50";

    Funcionario f;
    char *token;

    // 1o token: codigo (converte para int com atoi)
    token = strtok(linha, ",");
    if (token != NULL) {
        f.codigo = atoi(token);
    }

    // 2o token: nome (copia para a struct)
    token = strtok(NULL, ",");
    if (token != NULL) {
        strncpy(f.nome, token, sizeof(f.nome) - 1);
        f.nome[sizeof(f.nome) - 1] = '\0';   // garante o \0
    }

    // 3o token: salario (converte para float com atof)
    token = strtok(NULL, ",");
    if (token != NULL) {
        f.salario = (float) atof(token);
    }

    printf("Codigo: %d\n", f.codigo);
    printf("Nome: %s\n", f.nome);
    printf("Salario: %.2f\n", f.salario);

    return 0;
}
```

Saida:
```
Codigo: 10
Nome: Franco da Silva
Salario: 1500.50
```

### 8.1 Lendo varias linhas de um arquivo CSV

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

int main(void) {
    // Cria um arquivo de teste (na pratica ele ja existiria)
    FILE *arq = fopen("funcionario.txt", "w");
    if (arq == NULL) {
        printf("Erro ao criar arquivo\n");
        return 1;
    }
    fprintf(arq, "1,Franco,1500.50\n");
    fprintf(arq, "2,Maria,2000.00\n");
    fprintf(arq, "3,Joao,1800.75\n");
    fclose(arq);

    // Agora le o arquivo linha por linha
    arq = fopen("funcionario.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    char linha[200];
    Funcionario func[100];
    int qtd = 0;

    // fgets le uma linha inteira do arquivo
    while (fgets(linha, sizeof(linha), arq) != NULL && qtd < 100) {
        // Remove o \n do final da linha
        linha[strcspn(linha, "\n")] = '\0';

        char *token;

        token = strtok(linha, ",");
        if (token != NULL) func[qtd].codigo = atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(func[qtd].nome, token, sizeof(func[qtd].nome) - 1);
            func[qtd].nome[sizeof(func[qtd].nome) - 1] = '\0';
        }

        token = strtok(NULL, ",");
        if (token != NULL) func[qtd].salario = (float) atof(token);

        qtd++;
    }
    fclose(arq);

    // Lista todos os funcionarios lidos
    printf("Total lido: %d\n", qtd);
    int i;
    for (i = 0; i < qtd; i++) {
        printf("%d | %s | R$ %.2f\n",
               func[i].codigo, func[i].nome, func[i].salario);
    }

    return 0;
}
```

> **Fluxo padrao de leitura de CSV (DECORE ISSO):**
> 1. `fgets` le uma linha do arquivo.
> 2. Remove o `\n` com `strcspn`.
> 3. `strtok(linha, ",")` para o primeiro campo.
> 4. `strtok(NULL, ",")` para os proximos.
> 5. `atoi` / `atof` para converter numeros.
> 6. Repete ate `fgets` retornar NULL (fim do arquivo).

---

## 9. sprintf PARA MONTAR STRING FORMATADA

### 9.1 Montar linha CSV a partir de variaveis

```c
#include <stdio.h>

typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

int main(void) {
    Funcionario f = {10, "Franco", 1500.50};
    char linha[200];

    // sprintf monta a string no formato CSV
    sprintf(linha, "%d,%s,%.2f", f.codigo, f.nome, f.salario);

    printf("Linha para gravar no arquivo: %s\n", linha);
    // Saida: 10,Franco,1500.50

    return 0;
}
```

### 9.2 Gravar em arquivo usando sprintf + fprintf

```c
#include <stdio.h>

typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Funcionario;

int main(void) {
    Funcionario f = {10, "Franco", 1500.50};
    char linha[200];

    // Monta a linha com sprintf
    sprintf(linha, "%d,%s,%.2f", f.codigo, f.nome, f.salario);

    // Grava no arquivo
    FILE *arq = fopen("saida.txt", "a");   // "a" = append (adiciona no final)
    if (arq == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }
    fprintf(arq, "%s\n", linha);
    fclose(arq);

    printf("Linha gravada com sucesso!\n");
    return 0;
}
```

> **Diferenca sprintf vs fprintf:**
> - `sprintf(string, ...)` -> guarda o resultado em uma string.
> - `fprintf(arquivo, ...)` -> escreve direto no arquivo.
> - `printf(...)` -> escreve na tela (stdout).
> Todos usam os mesmos especificadores de formato (%d, %s, %f, etc.).

### 9.3 Montar mensagem de erro / log

```c
#include <stdio.h>

int main(void) {
    char msg[100];
    int linha_erro = 42;

    sprintf(msg, "Erro na linha %d do arquivo funcionario.txt", linha_erro);
    printf("%s\n", msg);
    // Erro na linha 42 do arquivo funcionario.txt

    return 0;
}
```

---

## 10. atoi, atof, atol - CONVERTER STRING PARA NUMERO

Estas funcoes estao em `<stdlib.h>`.

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char s_int[] = "123";
    char s_float[] = "1500.50";
    char s_long[] = "999999999";
    char s_invalida[] = "abc";

    int    i = atoi(s_int);        // 123
    float  f = (float) atof(s_float);   // 1500.50
    long   l = atol(s_long);       // 999999999
    int    x = atoi(s_invalida);  // 0 (nao conseguiu converter -> retorna 0)

    printf("int    = %d\n", i);    // 123
    printf("float  = %.2f\n", f);  // 1500.50
    printf("long   = %ld\n", l);   // 999999999
    printf("invalida = %d\n", x);  // 0

    return 0;
}
```

> **Atencao (cai em prova):**
> - `atoi` converte para `int`.
> - `atof` converte para `double` (por isso o cast para float se precisar).
> - `atol` converte para `long`.
> - Se a string NAO for um numero valido, retornam **0** (nao dao erro).
> - Elas param de converter no primeiro caractere nao-numerico.
>   Ex: `atoi("123abc")` retorna 123.

---

## 11. VALIDACAO DE ENTRADA

### 11.1 Validar CPF (tamanho 11 e so digitos)

```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Retorna 1 se o CPF for valido (11 digitos), 0 caso contrario
int validar_cpf(const char *cpf) {
    // 1. Verifica o tamanho
    if (strlen(cpf) != 11) {
        return 0;
    }

    // 2. Verifica se todos os caracteres sao digitos
    int i;
    for (i = 0; i < 11; i++) {
        if (!isdigit((unsigned char) cpf[i])) {
            return 0;
        }
    }

    return 1;   // passou nas duas verificacoes
}

int main(void) {
    char cpf[20];

    printf("Digite o CPF (so numeros): ");
    scanf("%14s", cpf);

    if (validar_cpf(cpf)) {
        printf("CPF valido: %s\n", cpf);
    } else {
        printf("CPF invalido! Deve ter 11 digitos numericos.\n");
    }

    return 0;
}
```

### 11.2 Validar e-mail (tem @ e ponto)

```c
#include <stdio.h>
#include <string.h>

// Retorna 1 se o e-mail for valido (tem @ e .), 0 caso contrario
int validar_email(const char *email) {
    // Procura o @
    char *arroba = strchr(email, '@');
    if (arroba == NULL) {
        return 0;   // nao tem @
    }

    // Procura o ponto DEPOIS do @
    char *ponto = strchr(arroba, '.');
    if (ponto == NULL) {
        return 0;   // nao tem ponto depois do @
    }

    // Verifica se o ponto nao e o ultimo caractere
    if (ponto[1] == '\0') {
        return 0;   // ponto no final
    }

    return 1;
}

int main(void) {
    char email[100];

    printf("Digite o e-mail: ");
    fgets(email, 100, stdin);
    email[strcspn(email, "\n")] = '\0';

    if (validar_email(email)) {
        printf("E-mail valido: %s\n", email);
    } else {
        printf("E-mail invalido! Deve conter @ e ponto.\n");
    }

    return 0;
}
```

### 11.3 Validar que so tem digitos (usando isdigit em loop)

```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Retorna 1 se a string so tiver digitos, 0 caso contrario
int so_digitos(const char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (!isdigit((unsigned char) s[i])) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char entrada[50];

    printf("Digite um numero: ");
    scanf("%49s", entrada);

    if (so_digitos(entrada)) {
        printf("OK, so tem digitos: %s\n", entrada);
    } else {
        printf("Erro: contem caracteres nao numericos.\n");
    }

    return 0;
}
```

### 11.4 Validar e converter com verificacao completa

```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Le um numero inteiro valido do teclado (repete ate digitar certo)
int ler_inteiro(const char *mensagem) {
    char entrada[50];
    int valido, valor, i;

    do {
        printf("%s", mensagem);
        scanf("%49s", entrada);

        valido = 1;
        // Permite sinal de menos no inicio
        int inicio = (entrada[0] == '-') ? 1 : 0;
        if (inicio == 1 && entrada[1] == '\0') {
            valido = 0;   // so o sinal de menos, sem digitos
        }
        for (i = inicio; entrada[i] != '\0'; i++) {
            if (!isdigit((unsigned char) entrada[i])) {
                valido = 0;
                break;
            }
        }

        if (!valido) {
            printf("Entrada invalida! Digite apenas numeros.\n");
        }
    } while (!valido);

    valor = atoi(entrada);
    return valor;
}

int main(void) {
    int idade = ler_inteiro("Digite sua idade: ");
    printf("Idade registrada: %d\n", idade);
    return 0;
}
```

---

## 12. ERROS COMUNS (TOPICO DE PROVA TEORICA)

### 12.1 Comparar strings com ==

```c
// ERRADO - compara ENDERECOS de memoria, nao conteudo!
if (nome == "admin") { ... }   // SEMPRE falso (enderecos diferentes)

// CORRETO - usa strcmp
if (strcmp(nome, "admin") == 0) { ... }
```

> Em C, `==` entre vetores/strings compara o endereco de memoria, nao o
> conteudo. Sempre use `strcmp` para comparar conteudo de strings.

### 12.2 Esquecer o \0

```c
// ERRADO - sem o \0, funcoes como strlen/printf leem alem do vetor
char s[3];
s[0] = 'a';
s[1] = 'b';
s[2] = 'c';
// falta o s[3] = '\0' - e pior: o vetor so tem 3 posicoes!

// CORRETO
char s[4];
s[0] = 'a';
s[1] = 'b';
s[2] = 'c';
s[3] = '\0';
```

### 12.3 Estouro de buffer com gets (PROIBIDO)

```c
// ERRADO - gets nao tem limite de leitura, e PERIGOSO
// Alem disso, gets foi removido do padrao C11.
gets(nome);   // NUNCA USE!

// CORRETO - use fgets com limite
fgets(nome, 50, stdin);
```

> `gets` le sem limite, entao se o usuario digitar mais que o tamanho do
> vetor, ele sobrescreve memoria. Por isso foi removido do C. Use sempre
> `fgets`.

### 12.4 Nao remover o \n do fgets

```c
// PROBLEMA: o fgets guarda o \n
fgets(nome, 50, stdin);
// nome = "Franco\n\0"
// Se comparar com strcmp(nome, "Franco") -> NAO sao iguais (tem o \n)!

// SOLUCAO: remover o \n
nome[strcspn(nome, "\n")] = '\0';
```

> **Cai em prova:** se o codigo usa fgets e depois strcmp com uma string
> literal, e provavel que de "diferente" por causa do \n. Sempre remova.

### 12.5 Confundir char ('a') com string ("a")

```c
char c = 'a';     // char: UM caractere, aspas simples
char s[] = "a";   // string: vetor de char, aspas duplas (tem \0 no fim)

// 'a'  -> 1 byte (97)
// "a"  -> 2 bytes (97, 0) - tem o \0!

// ERRADO: atribuir string a char
char x = "a";     // ERRADO - "a" e um ponteiro, nao um char

// ERRADO: comparar char com string
if (c == "a") { ... }   // nao compila / logica errada
if (c == 'a') { ... }   // CORRETO para char
```

### 12.6 Nao alocar espaco suficiente

```c
// ERRADO: vetor muito pequeno para o conteudo
char s[3] = "programacao";   // estouro de buffer!

// CORRETO: tamanho suficiente
char s[50] = "programacao";

// ERRADO: strcpy sem verificar tamanho
char destino[3];
strcpy(destino, "programacao");   // estouro!

// CORRETO: usar strncpy com limite
char destino[3];
strncpy(destino, "programacao", 2);
destino[2] = '\0';
```

### 12.7 Usar scanf("%s") sem limite

```c
// ERRADO: sem limite, se o usuario digitar muito, estoura o buffer
char nome[10];
scanf("%s", nome);   // se digitar 20 caracteres -> estouro!

// CORRETO: colocar o limite (tamanho - 1)
scanf("%9s", nome);   // le no maximo 9 caracteres (+ \0)
```

### 12.8 Resumo dos erros para prova teorica

| Erro | Problema | Solucao |
|------|----------|---------|
| `if (s1 == s2)` | compara enderecos | `strcmp(s1, s2) == 0` |
| Esquecer `\0` | funcoes leem alem | sempre terminar com `\0` |
| `gets(s)` | sem limite, perigoso | `fgets(s, n, stdin)` |
| Nao tirar `\n` do fgets | strcmp da diferente | `s[strcspn(s,"\n")]='\0'` |
| `'a'` vs `"a"` | tipos diferentes | char usa `'`, string usa `"` |
| `scanf("%s", s)` sem limite | estouro de buffer | `scanf("%49s", s)` |
| `strncpy` sem `\0` | string nao termina | `dest[n-1]='\0'` depois |

---

## 13. EXEMPLO COMPLETO: MENU DE MANIPULACAO DE STRINGS

Codigo completo que junta varios conceitos. Estilo do professor
(com conio.h, system, fflush).

```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// fflush(stdin) NAO e portatil mas funciona no Windows/MinGW.
// Em Linux/Mac seria necessario limpar o buffer com loop de getchar.
void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // descarta caracteres
    }
}

// Conta vogais usando ctype
int contar_vogais(const char *s) {
    int i, cont = 0;
    for (i = 0; s[i] != '\0'; i++) {
        char c = (char) tolower((unsigned char) s[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            cont++;
        }
    }
    return cont;
}

// Inverte a string no proprio vetor
void inverter(char *s) {
    int i = 0, j = (int) strlen(s) - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

// Converte para maiuscula
void para_maiuscula(char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        s[i] = (char) toupper((unsigned char) s[i]);
    }
}

// Verifica se e palindromo (case-insensitive, ignora espacos)
int eh_palindromo(const char *s) {
    int i = 0, j = (int) strlen(s) - 1;
    while (i < j) {
        while (i < j && s[i] == ' ') i++;
        while (i < j && s[j] == ' ') j--;
        char ci = (char) toupper((unsigned char) s[i]);
        char cj = (char) toupper((unsigned char) s[j]);
        if (ci != cj) return 0;
        i++;
        j--;
    }
    return 1;
}

int main(void) {
    char texto[200];
    int opcao;

    do {
        // system("cls") limpa a tela no Windows (NAO e portatil para Linux)
        system("cls");
        printf("=== MENU DE STRINGS ===\n");
        printf("1 - Digitar texto\n");
        printf("2 - Contar vogais\n");
        printf("3 - Inverter texto\n");
        printf("4 - Converter para maiuscula\n");
        printf("5 - Verificar palindromo\n");
        printf("6 - Contar palavras\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                printf("Digite o texto: ");
                fgets(texto, sizeof(texto), stdin);
                texto[strcspn(texto, "\n")] = '\0';
                printf("Texto armazenado: %s\n", texto);
                break;
            case 2:
                printf("Vogais: %d\n", contar_vogais(texto));
                break;
            case 3:
                inverter(texto);
                printf("Invertido: %s\n", texto);
                break;
            case 4:
                para_maiuscula(texto);
                printf("Maiuscula: %s\n", texto);
                break;
            case 5:
                if (eh_palindromo(texto)) {
                    printf("\"%s\" e palindromo!\n", texto);
                } else {
                    printf("\"%s\" NAO e palindromo.\n", texto);
                }
                break;
            case 6: {
                int i = 0, cont = 0, dentro = 0;
                while (texto[i] != '\0') {
                    if (isspace((unsigned char) texto[i])) {
                        dentro = 0;
                    } else if (!dentro) {
                        cont++;
                        dentro = 1;
                    }
                    i++;
                }
                printf("Palavras: %d\n", cont);
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }
    } while (opcao != 0);

    return 0;
}
```

---

## 14. RESUMO RAPIDO PARA PROVA

### Funcoes de string.h (decore)

| Funcao | O que faz | Retorna |
|--------|-----------|---------|
| `strlen(s)` | tamanho (ate \0) | size_t |
| `strcpy(d, o)` | copia o -> d | ponteiro para d |
| `strncpy(d, o, n)` | copia n chars | ponteiro para d |
| `strcat(d, o)` | concatena o no fim de d | ponteiro para d |
| `strncat(d, o, n)` | concatena n chars | ponteiro para d |
| `strcmp(a, b)` | compara | 0=igual, <0, >0 |
| `strncmp(a, b, n)` | compara n chars | 0=igual, <0, >0 |
| `strchr(s, c)` | acha 1o caractere c | ponteiro ou NULL |
| `strrchr(s, c)` | acha ultimo caractere c | ponteiro ou NULL |
| `strstr(s, sub)` | acha substring | ponteiro ou NULL |
| `strtok(s, delim)` | separa por delimitador | token ou NULL |
| `sprintf(d, fmt, ...)` | monta string formatada | int (qtd chars) |

### Funcoes de ctype.h

| Funcao | O que faz |
|--------|-----------|
| `isalpha(c)` | e letra? |
| `isdigit(c)` | e digito? |
| `isalnum(c)` | e letra ou digito? |
| `isspace(c)` | e espaco/tab/enter? |
| `isupper(c)` | e maiuscula? |
| `islower(c)` | e minuscula? |
| `toupper(c)` | converte para maiuscula |
| `tolower(c)` | converte para minuscula |

### Funcoes de stdlib.h (conversao)

| Funcao | Converte para |
|--------|---------------|
| `atoi(s)` | int |
| `atof(s)` | double |
| `atol(s)` | long |

### Padrao strtok (DECORE)

```c
token = strtok(str, ",");      // 1a chamada: passa a string
while (token != NULL) {
    // usa token
    token = strtok(NULL, ","); // proximas: passa NULL
}
```

### Padrao fgets + remover \n (DECORE)

```c
fgets(s, sizeof(s), stdin);
s[strcspn(s, "\n")] = '\0';
```

### Pegadinha strcmp (DECORE)

```c
// ERRADO (logica invertida - entra quando DIFERENTES):
if (strcmp(a, b)) { ... }

// CORRETO para IGUALDADE:
if (strcmp(a, b) == 0) { ... }

// CORRETO para DIFERENCA:
if (strcmp(a, b) != 0) { ... }
```

---

## 15. CHECKLIST FINAL ANTES DA PROVA

- [ ] Sei que string e vetor de char terminado por \0.
- [ ] Sei a diferenca entre strlen (logico) e sizeof (fisico).
- [ ] Sei ler com scanf("%49s") e fgets, e remover o \n com strcspn.
- [ ] Sei que misturar scanf e fgets deixa \n no buffer - preciso limpar.
- [ ] Sei todas as funcoes de string.h da tabela.
- [ ] Sei a pegadinha do strcmp: if(strcmp(a,b)) e VERDADEIRO quando DIFEREM.
- [ ] Sei as funcoes de ctype.h (isalpha, isdigit, toupper, tolower...).
- [ ] Sei fazer manipulacao manual: contar vogais, inverter, palindromo.
- [ ] Sei fazer busca parcial com strstr (projeto futebol).
- [ ] Sei o padrao do strtok (1a chamada com string, proximas com NULL).
- [ ] Sei usar sprintf para montar CSV.
- [ ] Sei converter string -> numero com atoi/atof.
- [ ] Sei validar CPF, e-mail, e verificar se so tem digitos.
- [ ] Sei os erros comuns: ==, esquecer \0, gets, nao tirar \n, char vs string.

---

> **FIM DO MATERIAL DE STRINGS AVANCADO.**
> Bom estudo, Franco! Decore as tabelas e o padrao do strtok/fgets.
> A pegadinha do strcmp e a questao teorica mais garantida - nao erre!


#############################################################
# PARTE 4 - COMBINACOES (topicos que se juntam na prova)
#############################################################

# COMBINACOES DE TOPICOS - GRAU B (GB) - Algoritmos e Programacao II

Material de CONSULTA para a prova do Grau B.
Aluno: Franco - Uniftec - Linguagem C (gcc MinGW no Windows).
Compila com: gcc -Wall -Wextra

Palavras-chave para Ctrl+F:
struct arquivo ; CRUD ; fwrite struct ; fread struct ; strtok CSV ;
struct string ; busca parcial ; strstr ; menu CRUD ; recursao ;
caso base ; fibonacci recursivo ; fatorial recursivo ; printf ordem ;
temp rename ; alterar ; excluir ; funcionario.dat ; funcionario.txt ;
fgets strcspn ; bubble sort strcmp ; matriz vetor funcoes ; dengue.

====================================================================
COMO USAR ESTE DOCUMENTO
====================================================================
Cada combinacao de topicos tem:
  (a) ENUNCIADO de exemplo (como cai na prova)
  (b) GABARITO de codigo C super comentado
  (c) EXPLICACAO do raciocinio (como traduzir enunciado em codigo)
  (d) PADROES - palavras do enunciado que revelam o que usar

Traducao enunciado -> codigo (DECORE isso):
  "cadastrar / gravar / persistir"  -> struct + fwrite (bin) ou fprintf (texto)
  "listar / mostrar / exibir"       -> fread no while (bin) ou fgets (texto)
  "buscar por codigo"               -> fread + if(codigo == buscado)
  "buscar por nome"                 -> strcmp (exato) ou strstr (parcial)
  "alterar / editar"                -> copia para temp, troca campo, rename
  "excluir / remover"               -> copia para temp pulando o excluido, rename
  "media / soma / total"            -> acumulador em loop
  "maior / menor"                   -> variavel + if em loop
  "ordenar por nome"                -> bubble sort com strcmp
  "recursao / recursiva"            -> caso base + caso recursivo
  "contar vogais / palavras"        -> fgets + loop char a char
  "menu / opcao"                    -> do-while + switch

====================================================================
PADRAO DO PROFESSOR (estilo conio.h)
====================================================================
- conio.h para getche() e getch()
- system("cls") para limpar tela
- system("pause") para pausar
- fflush(stdin) com comentario // limpa buffer do teclado
- menu do-while + switch
- fgets + strcspn para ler strings (NAO usa gets())

Modelo de cabecalho padrao:

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <conio.h>

    // fflush(stdin) - limpa buffer do teclado (Windows/MinGW)

====================================================================
INDICE DAS COMBINACOES
====================================================================
1. STRUCT + ARQUIVO BINARIO (CRUD completo) - funcionario.dat
2. STRUCT + ARQUIVO TEXTO (CSV) - funcionario.txt
3. STRUCT + STRING (strcmp, strstr, bubble sort)
4. STRING + ARQUIVO (vogais, palavras, busca)
5. STRUCT + VETOR + FUNCOES (media, maior, menor)
6. MENU + STRUCT + ARQUIVO (CRUD modularizado)
7. MATRIZ + VETOR + FUNCOES (modelo dengue) - resumo
8. RECURSAO (QUESTAO 1 confirmada) - fatorial, fibonacci, imprimir N..1
9. PADROES do enunciado (resumo de traducao)

====================================================================
1. STRUCT + ARQUIVO BINARIO (altissima probabilidade)
   Palavras-chave: struct arquivo, CRUD, fwrite struct, fread struct,
   funcionario.dat, temp rename, alterar, excluir
====================================================================

ENUNCIADO DE EXEMPLO:
  Faca um programa em C que gerencie funcionarios cadastrados em um
  arquivo binario "funcionario.dat". Cada funcionario tem: codigo (int),
  nome (string de 50), salario (float). O programa deve ter funcoes para:
    (a) cadastrar (gravar com fwrite no final do arquivo)
    (b) listar todos (ler com fread no while)
    (c) buscar por codigo
    (d) alterar salario de um funcionario pelo codigo
    (e) excluir funcionario pelo codigo
  Use menu do-while+switch. NAO use gets() nem while(!feof).

GABARITO DE CODIGO:

----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// definicao da struct - SEMPRE com typedef
typedef struct {
    int    codigo;
    char   nome[50];
    float  salario;
} Funcionario;

// prototipos das funcoes (CRUD)
void cadastrar(void);
void listar(void);
void buscar(void);
void alterar(void);
void excluir(void);
void limparBuffer(void);

int main(void) {
    int op;
    do {
        system("cls");
        printf("=== MENU FUNCIONARIO (arquivo binario) ===\n");
        printf("1 - Cadastrar\n");
        printf("2 - Listar todos\n");
        printf("3 - Buscar por codigo\n");
        printf("4 - Alterar salario\n");
        printf("5 - Excluir\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer(); // limpa o \n deixado pelo scanf

        switch(op) {
            case 1: cadastrar(); break;
            case 2: listar();    break;
            case 3: buscar();    break;
            case 4: alterar();   break;
            case 5: excluir();   break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
        system("pause");
    } while(op != 0);
    return 0;
}

// funcao auxiliar: limpa o buffer do teclado
// uso: depois de scanf, antes de fgets
void limparBuffer(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF) { }
}

// (a) CADASTRAR - abre com "ab" (append binary) e fwrite
//     "ab" adiciona no final sem apagar o que ja existe
void cadastrar(void) {
    Funcionario f;
    FILE *fp;

    printf("--- Cadastro ---\n");
    printf("Codigo: ");   scanf("%d", &f.codigo);
    limparBuffer();
    printf("Nome: ");
    fgets(f.nome, sizeof(f.nome), stdin);
    f.nome[strcspn(f.nome, "\n")] = '\0'; // tira o \n do final
    printf("Salario: "); scanf("%f", &f.salario);
    limparBuffer();

    // abre em modo "ab" = append binary (grava no final)
    fp = fopen("funcionario.dat", "ab");
    if(fp == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }
    // fwrite grava a struct inteira de uma vez
    // parametros: &endereco, tamanho de 1 elemento, qtd de elementos, arquivo
    fwrite(&f, sizeof(Funcionario), 1, fp);
    fclose(fp);
    printf("Cadastrado com sucesso!\n");
}

// (b) LISTAR - abre com "rb" (read binary) e fread no while
//     PADRAO CORRETO: while(fread(...) == 1)
//     NAO usar while(!feof(fp)) - da bug do ultimo registro duplicado
void listar(void) {
    Funcionario f;
    FILE *fp;

    fp = fopen("funcionario.dat", "rb");
    if(fp == NULL) {
        printf("Arquivo nao existe ou vazio.\n");
        return;
    }
    printf("\n--- Lista de Funcionarios ---\n");
    printf("COD  NOME                                          SALARIO\n");
    // fread retorna a qtd de elementos lidos. ==1 significa que leu 1 struct
    while(fread(&f, sizeof(Funcionario), 1, fp) == 1) {
        printf("%-4d %-45s %.2f\n", f.codigo, f.nome, f.salario);
    }
    fclose(fp);
}

// (c) BUSCAR por codigo - le todos, compara com strcmp NAO (codigo e int)
//     para int usa == ; strcmp so para string
void buscar(void) {
    Funcionario f;
    FILE *fp;
    int codBusca, achou = 0;

    printf("Codigo para buscar: ");
    scanf("%d", &codBusca);
    limparBuffer();

    fp = fopen("funcionario.dat", "rb");
    if(fp == NULL) {
        printf("Arquivo nao existe.\n");
        return;
    }
    while(fread(&f, sizeof(Funcionario), 1, fp) == 1) {
        if(f.codigo == codBusca) {  // comparacao de int com ==
            printf("Achado: %d | %s | %.2f\n", f.codigo, f.nome, f.salario);
            achou = 1;
            break;  // ja achou, nao precisa continuar
        }
    }
    fclose(fp);
    if(!achou) printf("Codigo nao encontrado.\n");
}

// (d) ALTERAR salario - tecnica do arquivo temporario
//     1. abre original "rb" e temp "wb"
//     2. le cada registro do original
//        - se codigo == buscado: grava versao MODIFICADA no temp
//        - senao: grava o registro original no temp (copia identica)
//     3. fecha ambos, remove original, renomeia temp -> original
void alterar(void) {
    Funcionario f;
    FILE *orig, *temp;
    int codBusca, achou = 0;
    float novoSal;

    printf("Codigo para alterar: ");
    scanf("%d", &codBusca);
    limparBuffer();

    orig = fopen("funcionario.dat", "rb");
    if(orig == NULL) {
        printf("Arquivo nao existe.\n");
        return;
    }
    temp = fopen("temp.dat", "wb");  // temporario em write binary

    while(fread(&f, sizeof(Funcionario), 1, orig) == 1) {
        if(f.codigo == codBusca) {
            printf("Salario atual: %.2f\n", f.salario);
            printf("Novo salario: ");
            scanf("%f", &novoSal);
            limparBuffer();
            f.salario = novoSal;  // modifica o campo
            achou = 1;
        }
        // grava no temp (modificado ou copia identica)
        fwrite(&f, sizeof(Funcionario), 1, temp);
    }
    fclose(orig);
    fclose(temp);

    if(achou) {
        // troca o arquivo: remove original e renomeia temp
        remove("funcionario.dat");
        rename("temp.dat", "funcionario.dat");
        printf("Alterado com sucesso!\n");
    } else {
        remove("temp.dat");  // nao achou, descarta temp
        printf("Codigo nao encontrado.\n");
    }
}

// (e) EXCLUIR - mesma tecnica do temp, mas PULA o excluido
//     1. abre original "rb" e temp "wb"
//     2. le cada registro
//        - se codigo == buscado: NAO grava no temp (pula)
//        - senao: grava copia identica no temp
//     3. fecha, remove original, rename temp -> original
void excluir(void) {
    Funcionario f;
    FILE *orig, *temp;
    int codBusca, achou = 0;

    printf("Codigo para excluir: ");
    scanf("%d", &codBusca);
    limparBuffer();

    orig = fopen("funcionario.dat", "rb");
    if(orig == NULL) {
        printf("Arquivo nao existe.\n");
        return;
    }
    temp = fopen("temp.dat", "wb");

    while(fread(&f, sizeof(Funcionario), 1, orig) == 1) {
        if(f.codigo == codBusca) {
            achou = 1;       // marca que achou
            continue;        // PULA o fwrite - nao copia este registro
        }
        fwrite(&f, sizeof(Funcionario), 1, temp);  // copia os outros
    }
    fclose(orig);
    fclose(temp);

    if(achou) {
        remove("funcionario.dat");
        rename("temp.dat", "funcionario.dat");
        printf("Excluido com sucesso!\n");
    } else {
        remove("temp.dat");
        printf("Codigo nao encontrado.\n");
    }
}
----------------------------------------------------------

EXPLICACAO DO RACIOCINIO:

1. Por que typedef struct? Em C puro (NAO C++), sem typedef voce teria
   que escrever "struct Funcionario" toda hora. typedef cria apelido.

2. Por que "ab" no cadastrar e "rb" no listar?
   - "ab" = append binary: adiciona no final, NAO apaga o arquivo.
   - "wb" = write binary: APAGA o arquivo e comeca do zero (usado no temp).
   - "rb" = read binary: so leitura.

3. Por que fread == 1 e nao while(!feof)?
   O feof so retorna verdadeiro DEPOIS de uma leitura falha. Com fread,
   quando a ultima leitura falha (fim do arquivo), o registro anterior
   fica "preso" na variavel e e processado de novo. Resultado: o ultimo
   registro aparece DUPLICADO. O padrao correto e ler e testar o retorno
   do fread no proprio while: fread(&f, sizeof(...), 1, fp) == 1.

4. Por que limparBuffer depois de scanf?
   O scanf("%d") le o numero mas deixa o \n (Enter) no buffer. Se depois
   vier um fgets, ele le esse \n e retorna string vazia. A funcao
   limparBuffer descarta tudo ate o \n.

5. Por que fgets + strcspn para nome?
   gets() e perigoso (nao tem limite de tamanho) e foi removido do C11.
   fgets le ate o tamanho maximo ou ate o Enter. Mas fgets GUARDA o \n.
   strcspn(f.nome, "\n") acha a posicao do \n e colocamos '\0' ali,
   removendo o Enter do final da string.

6. Tecnica do temp+rename (ALTERAR e EXCLUIR):
   Em C nao da para "editar no meio" de um arquivo binario de forma facil
   quando o tamanho muda. Para EXCLUIR (que muda o tamanho do arquivo),
   o padrao e:
     - copiar todos os registros para um arquivo temporario
     - pular (NAO copiar) o que quer excluir
     - apagar o original
     - renomear o temporario para o nome do original
   Para ALTERAR e a mesma coisa, mas em vez de pular, grava a versao
   modificada do registro.

PADROES DO ENUNCIADO (decorar):
  "cadastrar em arquivo"     -> struct + fopen "ab" + fwrite
  "listar / exibir"           -> fopen "rb" + while(fread==1) + printf
  "buscar por codigo"         -> fopen "rb" + while(fread==1) + if(cod==buscado)
  "alterar / editar campo"    -> temp "wb" + copia + modifica + remove+rename
  "excluir / remover"         -> temp "wb" + copia pulando + remove+rename

====================================================================
2. STRUCT + ARQUIVO TEXTO (CSV)
   Palavras-chave: strtok CSV, funcionario.txt, fprintf struct,
   fgets strtok atoi atof
====================================================================

ENUNCIADO DE EXEMPLO:
  Faca um programa que grave funcionarios em um arquivo TEXTO
  "funcionario.txt" no formato CSV (valores separados por virgula):
    codigo,nome,salario
  Exemplo de linha: 101,Joao Silva,2500.50
  O programa deve: cadastrar (fprintf), listar (fgets + strtok),
  e buscar por codigo. Use atoi para converter codigo e atof para salario.

GABARITO DE CODIGO:

----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
    int    codigo;
    char   nome[50];
    float  salario;
} Funcionario;

void cadastrar(void);
void listar(void);
void buscar(void);
void limparBuffer(void);

int main(void) {
    int op;
    do {
        system("cls");
        printf("=== MENU FUNCIONARIO (arquivo texto CSV) ===\n");
        printf("1 - Cadastrar\n");
        printf("2 - Listar\n");
        printf("3 - Buscar por codigo\n");
        printf("0 - Sair\nOpcao: ");
        scanf("%d", &op);
        limparBuffer();
        switch(op) {
            case 1: cadastrar(); break;
            case 2: listar();    break;
            case 3: buscar();    break;
        }
        system("pause");
    } while(op != 0);
    return 0;
}

void limparBuffer(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF) { }
}

// CADASTRAR - fprintf grava como texto no formato CSV
// "a" = append (texto), adiciona no final
void cadastrar(void) {
    Funcionario f;
    FILE *fp;

    printf("Codigo: ");  scanf("%d", &f.codigo);
    limparBuffer();
    printf("Nome: ");
    fgets(f.nome, sizeof(f.nome), stdin);
    f.nome[strcspn(f.nome, "\n")] = '\0';
    printf("Salario: "); scanf("%f", &f.salario);
    limparBuffer();

    fp = fopen("funcionario.txt", "a");  // append texto
    if(fp == NULL) { printf("Erro!\n"); return; }
    // fprintf com virgula separando os campos
    fprintf(fp, "%d,%s,%.2f\n", f.codigo, f.nome, f.salario);
    fclose(fp);
    printf("Cadastrado!\n");
}

// LISTAR - fgets le cada linha, strtok quebra pela virgula
// atoi converte string -> int, atof converte string -> float
void listar(void) {
    char linha[200];
    FILE *fp;
    char *token;
    int codigo;
    char nome[50];
    float salario;

    fp = fopen("funcionario.txt", "r");  // read texto
    if(fp == NULL) { printf("Arquivo nao existe.\n"); return; }

    printf("\nCOD  NOME                                          SALARIO\n");
    // fgets le uma linha inteira (ate \n ou tamanho maximo)
    while(fgets(linha, sizeof(linha), fp) != NULL) {
        // strtok quebra a string pelo delimitador ","
        token = strtok(linha, ",");       // 1o token: codigo
        codigo = atoi(token);             // converte para int
        token = strtok(NULL, ",");         // 2o token: nome
        strcpy(nome, token);               // copia para a variavel
        token = strtok(NULL, ",");         // 3o token: salario
        salario = atof(token);             // converte para float
        printf("%-4d %-45s %.2f\n", codigo, nome, salario);
    }
    fclose(fp);
}

// BUSCAR por codigo - mesma leitura do listar, mas compara
void buscar(void) {
    char linha[200];
    FILE *fp;
    char *token;
    int codigo, codBusca, achou = 0;

    printf("Codigo para buscar: ");
    scanf("%d", &codBusca);
    limparBuffer();

    fp = fopen("funcionario.txt", "r");
    if(fp == NULL) { printf("Arquivo nao existe.\n"); return; }

    while(fgets(linha, sizeof(linha), fp) != NULL) {
        token = strtok(linha, ",");
        codigo = atoi(token);
        if(codigo == codBusca) {
            char nome[50]; float salario;
            token = strtok(NULL, ",");
            strcpy(nome, token);
            token = strtok(NULL, ",");
            salario = atof(token);
            printf("Achado: %d | %s | %.2f\n", codigo, nome, salario);
            achou = 1;
            break;
        }
    }
    fclose(fp);
    if(!achou) printf("Nao encontrado.\n");
}
----------------------------------------------------------

EXPLICACAO DO RACIOCINIO:

1. Diferenca binario x texto:
   - BINARIO (.dat): fwrite grava os bytes da struct. Nao da para ler no
     bloco de notas (fica ilegivel). Mais rapido e mantem a estrutura.
   - TEXTO (.txt): fprintf grava como texto legivel. Da para abrir no
     bloco de notas. Para ler de volta, usa fgets + strtok para quebrar.

2. strtok - como funciona:
   - strtok(string, delimitador) quebra a string na PRIMEIRA ocorrencia
     do delimitador e retorna o token anterior.
   - Depois, chama strtok(NULL, delimitador) para pegar o PROXIMO token.
   - O NULL diz "continua de onde parou".
   - Exemplo: "101,Joao Silva,2500.50"
       strtok(linha, ",")      -> "101"
       strtok(NULL, ",")       -> "Joao Silva"
       strtok(NULL, ",")       -> "2500.50"

3. atoi e atof:
   - atoi = "ASCII to Integer": converte "101" para 101 (int).
   - atof = "ASCII to Float": converte "2500.50" para 2500.50 (float).
   - Sao da <stdlib.h>.

4. Por que fgets e nao fscanf?
   fscanf com %s para de ler no espaco. Se o nome tem espaco ("Joao Silva"),
   fscanf le so "Joao". fgets le a linha inteira. Por isso o padrao e
   fgets + strtok para CSV.

PADROES DO ENUNCIADO:
  "arquivo texto / CSV / separado por virgula" -> fprintf + fgets + strtok
  "converter para inteiro"                     -> atoi
  "converter para float / real"                -> atof
  "gravar como texto"                           -> fprintf

====================================================================
3. STRUCT + STRING (strcmp, strstr, bubble sort)
   Palavras-chave: struct string, busca parcial, strstr, strcmp,
   bubble sort strcmp, fgets strcspn
====================================================================

ENUNCIADO DE EXEMPLO:
  Faca um programa que cadastre N funcionarios em um VETOR de struct
  (nao em arquivo). Cada funcionario tem nome (string de 50) e salario.
  Depois faca:
    (a) buscar funcionario pelo nome EXATO (strcmp)
    (b) buscar nomes que CONTEM um trecho, ex: "sil" (strstr - parcial)
    (c) ordenar o vetor por nome em ordem alfabetica (bubble sort + strcmp)

GABARITO DE CODIGO:

----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX 5   // qtd de funcionarios para teste

typedef struct {
    char  nome[50];
    float salario;
} Funcionario;

void lerVetor(Funcionario v[], int n);
void imprimir(Funcionario v[], int n);
void buscarExato(Funcionario v[], int n);
void buscarParcial(Funcionario v[], int n);
void ordenarPorNome(Funcionario v[], int n);
void limparBuffer(void);

int main(void) {
    Funcionario vet[MAX];
    int op;

    lerVetor(vet, MAX);

    do {
        system("cls");
        printf("=== STRUCT + STRING ===\n");
        printf("1 - Listar\n");
        printf("2 - Buscar nome exato (strcmp)\n");
        printf("3 - Buscar parcial (strstr)\n");
        printf("4 - Ordenar por nome (bubble sort)\n");
        printf("0 - Sair\nOpcao: ");
        scanf("%d", &op);
        limparBuffer();
        switch(op) {
            case 1: imprimir(vet, MAX); break;
            case 2: buscarExato(vet, MAX); break;
            case 3: buscarParcial(vet, MAX); break;
            case 4: ordenarPorNome(vet, MAX);
                    printf("Ordenado!\n"); break;
        }
        system("pause");
    } while(op != 0);
    return 0;
}

void limparBuffer(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF) { }
}

void lerVetor(Funcionario v[], int n) {
    int i;
    for(i = 0; i < n; i++) {
        printf("--- Funcionario %d ---\n", i+1);
        printf("Nome: ");
        fgets(v[i].nome, sizeof(v[i].nome), stdin);
        v[i].nome[strcspn(v[i].nome, "\n")] = '\0';
        printf("Salario: ");
        scanf("%f", &v[i].salario);
        limparBuffer();
    }
}

void imprimir(Funcionario v[], int n) {
    int i;
    printf("\nNOME                                          SALARIO\n");
    for(i = 0; i < n; i++) {
        printf("%-45s %.2f\n", v[i].nome, v[i].salario);
    }
}

// (a) BUSCA EXATA - strcmp
//     strcmp(a, b) retorna:
//       0   se a == b (strings iguais)
//       <0  se a < b (a vem antes de b)
//       >0  se a > b (a vem depois de b)
//     Para comparar se sao IGUAIS, usa if(strcmp(a,b) == 0)
void buscarExato(Funcionario v[], int n) {
    char busca[50];
    int i, achou = 0;
    printf("Nome para buscar (exato): ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = '\0';

    for(i = 0; i < n; i++) {
        if(strcmp(v[i].nome, busca) == 0) {
            printf("Achado: %s | %.2f\n", v[i].nome, v[i].salario);
            achou = 1;
            break;
        }
    }
    if(!achou) printf("Nome nao encontrado.\n");
}

// (b) BUSCA PARCIAL - strstr
//     strstr(texto, trecho) retorna:
//       - ponteiro para a 1a ocorrencia do trecho dentro do texto (achou)
//       - NULL se o trecho nao esta no texto (nao achou)
//     Para saber se achou: if(strstr(texto, trecho) != NULL)
//     IMPORTANTE: strstr e CASE SENSITIVE. "Sil" != "sil".
void buscarParcial(Funcionario v[], int n) {
    char trecho[50];
    int i, achou = 0;
    printf("Trecho para buscar (parcial): ");
    fgets(trecho, sizeof(trecho), stdin);
    trecho[strcspn(trecho, "\n")] = '\0';

    printf("Nomes que contem \"%s\":\n", trecho);
    for(i = 0; i < n; i++) {
        if(strstr(v[i].nome, trecho) != NULL) {
            printf("  %s | %.2f\n", v[i].nome, v[i].salario);
            achou = 1;
        }
    }
    if(!achou) printf("Nenhum nome contem o trecho.\n");
}

// (c) ORDENAR por nome - bubble sort com strcmp
//     Compara v[j] com v[j+1]. Se v[j] > v[j+1] (strcmp > 0), troca.
//     Troca de STRING se faz com strcpy (nao com =).
//     Troca de float se faz com = normal.
void ordenarPorNome(Funcionario v[], int n) {
    int i, j;
    Funcionario aux;
    for(i = 0; i < n-1; i++) {
        for(j = 0; j < n-1-i; j++) {
            // strcmp > 0 significa que v[j] vem DEPOIS de v[j+1]
            // ou seja, estao fora de ordem alfabetica -> troca
            if(strcmp(v[j].nome, v[j+1].nome) > 0) {
                // troca a struct inteira (copia todos os campos de uma vez)
                aux = v[j];
                v[j] = v[j+1];
                v[j+1] = aux;
            }
        }
    }
}
----------------------------------------------------------

EXPLICACAO DO RACIOCINIO:

1. strcmp - comparar strings:
   Em C, NAO pode usar == para comparar strings. == compara enderecos
   de memoria, nao o conteudo. Use sempre strcmp.
   - strcmp(a, b) == 0  -> strings iguais
   - strcmp(a, b) < 0   -> a vem antes de b (a < b alfabeticamente)
   - strcmp(a, b) > 0   -> a vem depois de b (a > b alfabeticamente)

2. strstr - buscar trecho dentro de string:
   - strstr(textoCompleto, trechoBuscado)
   - Retorna ponteiro se achou, NULL se nao achou.
   - CASE SENSITIVE: diferencia maiuscula/minuscula.
   - Para busca "case insensitive" teria que converter tudo para
     minuscula antes (tolower), mas geralmente a prova nao pede isso.

3. Bubble sort com strcmp:
   - O bubble sort compara pares adjacentes e troca se estao fora de ordem.
   - Para STRING, a comparacao e strcmp > 0 (fora de ordem alfabetica).
   - A TROCA de struct inteira pode ser feita com = (atribuicao de struct),
     porque C permite copiar struct com =. Isso copia TODOS os campos.
   - Se fosse so a string, teria que usar strcpy.

4. fgets + strcspn (sempre que ler string):
   - fgets le a linha incluindo o \n.
   - strcspn acha a posicao do \n.
   - Colocar '\0' nessa posicao remove o \n.

PADROES DO ENUNCIADO:
  "buscar por nome (exato)"   -> strcmp == 0
  "nomes que contem / parcial"-> strstr != NULL
  "ordenar por nome / alfabetica" -> bubble sort + strcmp > 0
  "comparar strings"          -> strcmp (NAO use ==)

====================================================================
4. STRING + ARQUIVO (vogais, palavras, busca)
   Palavras-chave: STRING ARQUIVO, fgets, vogais, palavras, strstr
====================================================================

ENUNCIADO DE EXEMPLO:
  Faca um programa que leia um arquivo texto "texto.txt" linha a linha
  com fgets e faca:
    (a) conte quantas vogais tem em cada linha
    (b) conte quantas palavras tem o arquivo inteiro
    (c) mostre as linhas que contem uma palavra informada pelo usuario

GABARITO DE CODIGO:

----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>   // para tolower

void contarVogais(void);
void contarPalavras(void);
void buscarPalavra(void);
void limparBuffer(void);

int main(void) {
    int op;
    do {
        system("cls");
        printf("=== STRING + ARQUIVO ===\n");
        printf("1 - Contar vogais de cada linha\n");
        printf("2 - Contar palavras do arquivo\n");
        printf("3 - Buscar linhas com palavra\n");
        printf("0 - Sair\nOpcao: ");
        scanf("%d", &op);
        limparBuffer();
        switch(op) {
            case 1: contarVogais(); break;
            case 2: contarPalavras(); break;
            case 3: buscarPalavra(); break;
        }
        system("pause");
    } while(op != 0);
    return 0;
}

void limparBuffer(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF) { }
}

// (a) CONTAR VOGAIS de cada linha
//     Para cada linha (fgets), percorre char a char e testa se e vogal.
//     Usa tolower para tratar maiuscula e minuscula igual.
void contarVogais(void) {
    char linha[200];
    FILE *fp;
    int i, vogais, numLinha = 0;

    fp = fopen("texto.txt", "r");
    if(fp == NULL) { printf("Arquivo nao existe.\n"); return; }

    while(fgets(linha, sizeof(linha), fp) != NULL) {
        numLinha++;
        vogais = 0;
        for(i = 0; linha[i] != '\0'; i++) {
            char c = tolower(linha[i]);  // converte para minuscula
            if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vogais++;
            }
        }
        printf("Linha %d: %d vogais\n", numLinha, vogais);
    }
    fclose(fp);
}

// (b) CONTAR PALAVRAS do arquivo
//     Truque: uma palavra comeca quando acha-se uma letra apos um espaco
//     ou inicio. Conta as transicoes de espaco -> letra.
void contarPalavras(void) {
    char linha[200];
    FILE *fp;
    int i, total = 0;
    int dentroPalavra = 0;  // 0 = fora de palavra, 1 = dentro

    fp = fopen("texto.txt", "r");
    if(fp == NULL) { printf("Arquivo nao existe.\n"); return; }

    while(fgets(linha, sizeof(linha), fp) != NULL) {
        for(i = 0; linha[i] != '\0'; i++) {
            if(linha[i] == ' ' || linha[i] == '\n' || linha[i] == '\t') {
                dentroPalavra = 0;  // espaco = fora de palavra
            } else {
                // se estava fora e achou letra, comecou uma palavra nova
                if(dentroPalavra == 0) {
                    total++;
                    dentroPalavra = 1;
                }
            }
        }
    }
    fclose(fp);
    printf("Total de palavras: %d\n", total);
}

// (c) BUSCAR LINHAS que contem uma palavra
//     strstr verifica se a palavra esta na linha
void buscarPalavra(void) {
    char linha[200], palavra[50];
    FILE *fp;
    int numLinha, achou = 0;

    printf("Palavra para buscar: ");
    fgets(palavra, sizeof(palavra), stdin);
    palavra[strcspn(palavra, "\n")] = '\0';

    fp = fopen("texto.txt", "r");
    if(fp == NULL) { printf("Arquivo nao existe.\n"); return; }

    numLinha = 0;
    while(fgets(linha, sizeof(linha), fp) != NULL) {
        numLinha++;
        // strstr != NULL significa que a palavra esta na linha
        if(strstr(linha, palavra) != NULL) {
            printf("Linha %d: %s", numLinha, linha);
            achou = 1;
        }
    }
    fclose(fp);
    if(!achou) printf("Palavra nao encontrada.\n");
}
----------------------------------------------------------

EXPLICACAO DO RACIOCINIO:

1. Ler arquivo texto linha a linha:
   O padrao e sempre: while(fgets(linha, tamanho, fp) != NULL).
   fgets retorna NULL quando chega no fim do arquivo.

2. Contar vogais:
   - Percorrer a string char a char com um for ate o '\0'.
   - tolower converte para minuscula, assim nao precisa testar A e a.
   - Comparar com cada vogal: a, e, i, o, u.

3. Contar palavras (truque da maquina de estados):
   - Mantem uma variavel "dentroPalavra" (0 ou 1).
   - Se achar espaco/\n/\t: dentroPalavra = 0 (saiu da palavra).
   - Se achar letra E estava fora (dentroPalavra == 0):
     comecou uma palavra nova -> conta +1 e dentroPalavra = 1.
   - Isso evita contar varios espacos seguidos como varias palavras.

4. Buscar palavra na linha:
   - strstr(linha, palavra) != NULL significa que achou.
   - Case sensitive: "Casa" != "casa".

PADROES DO ENUNCIADO:
  "contar vogais"              -> fgets + for char a char + tolower
  "contar palavras"            -> maquina de estados (dentroPalavra)
  "linhas que contem palavra"  -> fgets + strstr != NULL
  "ler arquivo texto"          -> while(fgets != NULL)

====================================================================
5. STRUCT + VETOR + FUNCOES (media, maior, menor)
   Palavras-chave: STRUCT VETOR FUNCOES, media, maior, menor, modular
====================================================================

ENUNCIADO DE EXEMPLO:
  Faca um programa modularizado que cadastre N alunos em um vetor de
  struct (matricula, nome, nota). Use funcoes separadas para:
    (a) leitura dos dados
    (b) impressao
    (c) calculo da media das notas
    (d) achar o aluno com MAIOR nota
    (e) achar o aluno com MENOR nota
  Passe o vetor e o tamanho n para cada funcao.

GABARITO DE CODIGO:

----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX 5

typedef struct {
    int   matricula;
    char  nome[50];
    float nota;
} Aluno;

// prototipos - sempre passamos o vetor e o tamanho n
void lerAlunos(Aluno v[], int n);
void imprimir(Aluno v[], int n);
float calcularMedia(Aluno v[], int n);
void acharMaior(Aluno v[], int n);
void acharMenor(Aluno v[], int n);
void limparBuffer(void);

int main(void) {
    Aluno vet[MAX];
    float media;

    lerAlunos(vet, MAX);
    imprimir(vet, MAX);

    media = calcularMedia(vet, MAX);
    printf("Media das notas: %.2f\n", media);

    acharMaior(vet, MAX);
    acharMenor(vet, MAX);

    system("pause");
    return 0;
}

void limparBuffer(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF) { }
}

// (a) LEITURA - passa o vetor por referencia (nome do vetor ja e endereco)
void lerAlunos(Aluno v[], int n) {
    int i;
    for(i = 0; i < n; i++) {
        printf("--- Aluno %d ---\n", i+1);
        printf("Matricula: "); scanf("%d", &v[i].matricula);
        limparBuffer();
        printf("Nome: ");
        fgets(v[i].nome, sizeof(v[i].nome), stdin);
        v[i].nome[strcspn(v[i].nome, "\n")] = '\0';
        printf("Nota: "); scanf("%f", &v[i].nota);
        limparBuffer();
    }
}

// (b) IMPRESSAO
void imprimir(Aluno v[], int n) {
    int i;
    printf("\nMAT  NOME                                          NOTA\n");
    for(i = 0; i < n; i++) {
        printf("%-4d %-45s %.2f\n", v[i].matricula, v[i].nome, v[i].nota);
    }
}

// (c) MEDIA - acumula a soma e divide por n
//     retorna float
float calcularMedia(Aluno v[], int n) {
    float soma = 0.0;
    int i;
    for(i = 0; i < n; i++) {
        soma += v[i].nota;   // acumula
    }
    return soma / n;         // media
}

// (d) MAIOR - mantem o indice do maior
//     comeca assumindo que o maior e o primeiro (indice 0)
void acharMaior(Aluno v[], int n) {
    int i, idxMaior = 0;
    for(i = 1; i < n; i++) {
        if(v[i].nota > v[idxMaior].nota) {
            idxMaior = i;    // atualiza o indice do maior
        }
    }
    printf("Maior nota: %s | %.2f\n",
           v[idxMaior].nome, v[idxMaior].nota);
}

// (e) MENOR - mesma logica, mas com <
void acharMenor(Aluno v[], int n) {
    int i, idxMenor = 0;
    for(i = 1; i < n; i++) {
        if(v[i].nota < v[idxMenor].nota) {
            idxMenor = i;
        }
    }
    printf("Menor nota: %s | %.2f\n",
           v[idxMenor].nome, v[idxMenor].nota);
}
----------------------------------------------------------

EXPLICACAO DO RACIOCINIO:

1. Modularizacao - por que dividir em funcoes?
   - Cada funcao faz UMA coisa (leitura, impressao, calculo).
   - Facilita testar e reusar.
   - O professor cobra modularizacao: ponto perdido se fizer tudo no main.

2. Passagem de vetor para funcao:
   - Em C, o nome do vetor ja e o endereco do primeiro elemento.
   - Por isso, alteracoes feitas no vetor DENTRO da funcao afetam o
     vetor original (passagem por referencia implicita).
   - Sintaxe: void funcao(Aluno v[], int n)
   - NAO precisa de & nem * para vetores.

3. Calcular media:
   - Acumulador: soma começa em 0, soma += nota em cada iteracao.
   - No final, divide soma por n.
   - CUIDADO: se n for 0, da divisao por zero. Na prova, geralmente
     n > 0 e garantido.

4. Achar maior/menor (padrao do indice):
   - Mantem o INDICE do maior (nao o valor).
   - Começa assumindo que o maior e o indice 0.
   - Compara cada um com o atual maior. Se achar maior, atualiza o indice.
   - No final, v[idxMaior] tem o aluno com maior nota.
   - Vantagem de guardar o indice: da para acessar TODOS os campos
     (nome, matricula, nota) do aluno achado.

PADROES DO ENUNCIADO:
  "media / soma"          -> acumulador em loop
  "maior / menor"         -> variavel indice + if em loop
  "modularizado / funcoes"-> dividir em funcoes com prototipo
  "vetor de struct"       -> typedef struct + passagem v[], n

====================================================================
6. MENU + STRUCT + ARQUIVO (CRUD modularizado)
   Palavras-chave: menu CRUD, do-while switch, modular
====================================================================

ENUNCIADO DE EXEMPLO:
  Faca um programa com MENU do-while+switch que gerencie produtos em
  arquivo binario "produtos.dat". Opcoes: 1-Cadastrar, 2-Listar,
  3-Buscar, 4-Alterar, 5-Excluir, 0-Sair. Cada opcao chama uma funcao.

GABARITO DE CODIGO (estrutura do menu - o CRUD ja foi visto na secao 1):

----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
    int   codigo;
    char  nome[50];
    float preco;
} Produto;

// prototipos
void cadastrar(void);
void listar(void);
void buscar(void);
void alterar(void);
void excluir(void);
void limparBuffer(void);

int main(void) {
    int op;
    do {
        system("cls");   // limpa a tela (Windows)
        printf("===== MENU PRODUTOS =====\n");
        printf("1 - Cadastrar\n");
        printf("2 - Listar\n");
        printf("3 - Buscar por codigo\n");
        printf("4 - Alterar\n");
        printf("5 - Excluir\n");
        printf("0 - Sair\n");
        printf("=========================\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();   // tira o \n do buffer

        switch(op) {
            case 1:
                cadastrar();
                break;
            case 2:
                listar();
                break;
            case 3:
                buscar();
                break;
            case 4:
                alterar();
                break;
            case 5:
                excluir();
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
        // pausa para o usuario ver o resultado antes de limpar a tela
        system("pause");
    } while(op != 0);   // repete enquanto nao escolher sair

    return 0;
}

void limparBuffer(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF) { }
}

// As funcoes cadastrar/listar/buscar/alterar/excluir sao IGUAIS as da
// secao 1 (STRUCT + ARQUIVO BINARIO), so trocando Funcionario por Produto
// e "salario" por "preco". Consulte a secao 1 para o codigo completo.
void cadastrar(void) {
    Produto p;
    FILE *fp;
    printf("--- Cadastro ---\n");
    printf("Codigo: "); scanf("%d", &p.codigo); limparBuffer();
    printf("Nome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';
    printf("Preco: "); scanf("%f", &p.preco); limparBuffer();
    fp = fopen("produtos.dat", "ab");
    if(fp == NULL) { printf("Erro!\n"); return; }
    fwrite(&p, sizeof(Produto), 1, fp);
    fclose(fp);
    printf("Cadastrado!\n");
}

void listar(void) {
    Produto p;
    FILE *fp = fopen("produtos.dat", "rb");
    if(fp == NULL) { printf("Vazio.\n"); return; }
    while(fread(&p, sizeof(Produto), 1, fp) == 1) {
        printf("%d | %s | %.2f\n", p.codigo, p.nome, p.preco);
    }
    fclose(fp);
}

void buscar(void) {
    Produto p;
    FILE *fp;
    int cod, achou = 0;
    printf("Codigo: "); scanf("%d", &cod); limparBuffer();
    fp = fopen("produtos.dat", "rb");
    if(fp == NULL) { printf("Vazio.\n"); return; }
    while(fread(&p, sizeof(Produto), 1, fp) == 1) {
        if(p.codigo == cod) {
            printf("Achado: %d | %s | %.2f\n", p.codigo, p.nome, p.preco);
            achou = 1; break;
        }
    }
    fclose(fp);
    if(!achou) printf("Nao encontrado.\n");
}

void alterar(void) {
    Produto p;
    FILE *orig, *temp;
    int cod, achou = 0; float novo;
    printf("Codigo: "); scanf("%d", &cod); limparBuffer();
    orig = fopen("produtos.dat", "rb");
    if(orig == NULL) { printf("Vazio.\n"); return; }
    temp = fopen("temp.dat", "wb");
    while(fread(&p, sizeof(Produto), 1, orig) == 1) {
        if(p.codigo == cod) {
            printf("Novo preco: "); scanf("%f", &novo); limparBuffer();
            p.preco = novo;
            achou = 1;
        }
        fwrite(&p, sizeof(Produto), 1, temp);
    }
    fclose(orig); fclose(temp);
    if(achou) {
        remove("produtos.dat");
        rename("temp.dat", "produtos.dat");
        printf("Alterado!\n");
    } else {
        remove("temp.dat");
        printf("Nao encontrado.\n");
    }
}

void excluir(void) {
    Produto p;
    FILE *orig, *temp;
    int cod, achou = 0;
    printf("Codigo: "); scanf("%d", &cod); limparBuffer();
    orig = fopen("produtos.dat", "rb");
    if(orig == NULL) { printf("Vazio.\n"); return; }
    temp = fopen("temp.dat", "wb");
    while(fread(&p, sizeof(Produto), 1, orig) == 1) {
        if(p.codigo == cod) { achou = 1; continue; }
        fwrite(&p, sizeof(Produto), 1, temp);
    }
    fclose(orig); fclose(temp);
    if(achou) {
        remove("produtos.dat");
        rename("temp.dat", "produtos.dat");
        printf("Excluido!\n");
    } else {
        remove("temp.dat");
        printf("Nao encontrado.\n");
    }
}
----------------------------------------------------------

EXPLICACAO DO RACIOCINIO:

1. Estrutura do menu (DECORE este esqueleto):
   - do { ... } while(condicao);  -> executa pelo menos 1 vez, repete
   - system("cls") no inicio de cada iteracao para limpar a tela
   - scanf da opcao + limparBuffer
   - switch(op) com cada case chamando uma funcao
   - system("pause") no final para o usuario ver antes de limpar
   - Repete enquanto op != 0

2. Por que limparBuffer depois do scanf da opcao?
   Se a proxima funcao (ex: cadastrar) usa fgets para ler nome, o \n
   deixado pelo scanf seria lido pelo fgets como string vazia.

3. system("pause") no final de cada case:
   Mostra "Pressione qualquer tecla para continuar..." e espera. Assim
   o usuario ve o resultado antes do system("cls") limpar a tela.

PADROES DO ENUNCIADO:
  "menu / opcoes"            -> do-while + switch
  "limpar tela"             -> system("cls")
  "pausar"                  -> system("pause")
  "modular / funcoes"       -> cada case chama uma funcao

====================================================================
7. MATRIZ + VETOR + FUNCOES (modelo dengue) - RESUMO
   Palavras-chave: matriz vetor funcoes, dengue, totais, faixa
====================================================================

Este topico ja foi visto no GA. Resumo da conexao:

- Matriz de dados: linhas = bairros, colunas = semanas (ou similar).
- Vetor de totais por linha: soma de cada linha da matriz.
- Vetor de totais por coluna: soma de cada coluna da matriz.
- Funcoes separadas: leitura, impressao, somaLinha, somaColuna, faixa.

Esqueleto:

    void lerMatriz(int m[][COL], int lin, int col);
    void imprimir(int m[][COL], int lin, int col);
    void totaisLinha(int m[][COL], int lin, int col, int vet[]);
    void totaisColuna(int m[][COL], int lin, int col, int vet[]);
    int contarFaixa(int vet[], int n, int min, int max);

- totaisLinha: para cada linha i, soma todos os col j -> vet[i].
- totaisColuna: para cada coluna j, soma todos os lin i -> vet[j].
- contarFaixa: conta quantos elementos do vetor estao entre min e max.

PADRAO: "total por linha/coluna" -> dois for (um para linha, um para coluna)
        "faixa / entre X e Y"     -> if(v >= min && v <= max) conta++

====================================================================
8. RECURSAO (QUESTAO 1 CONFIRMADA da prova)
   Palavras-chave: recursao, caso base, fibonacci recursivo,
   fatorial recursivo, printf ordem, pilha de chamadas
====================================================================

CONCEITO CENTRAL:
  Recursao e quando uma funcao chama A SI MESMA.
  Toda funcao recursiva precisa de:
    (1) CASO BASE: condicao de parada (quando para de chamar a si mesma).
    (2) CASO RECURSIVO: a funcao chama a si mesma com um problema menor.

  Se faltar caso base, a funcao chama a si mesma para SEMPRE (stack overflow).

  A PEGADINHA DA ORDEM DO PRINTF (muito cobrada):
    - printf ANTES da chamada recursiva = imprime em ordem DECRESCENTE (ida).
    - printf DEPOIS da chamada recursiva = imprime em ordem CRESCENTE (volta).

  Isso acontece por causa da PILHA DE CHAMADAS:
    - Cada chamada recursiva empilha o estado da funcao.
    - Quando chega no caso base, comeca a DESEMPILHAR (voltar).
    - Se o printf esta DEPOIS da chamada, so executa na VOLTA (desempilhando),
      por isso imprime em ordem crescente.

--------------------------------------------------------------------
8.1 FATORIAL RECURSIVO
--------------------------------------------------------------------

ENUNCIADO: Faca uma funcao recursiva que calcule o fatorial de N.
  Fatorial: N! = N * (N-1) * (N-2) * ... * 1. Por definicao, 0! = 1.

GABARITO:

    // fatorial recursivo
    // caso base: 0! = 1
    // caso recursivo: n! = n * (n-1)!
    int fatorial(int n) {
        if(n == 0) {           // CASO BASE - para de chamar
            return 1;
        }
        return n * fatorial(n - 1);  // CASO RECURSIVO
    }

EXPLICACAO:
  - Caso base: n == 0 retorna 1 (sem chamar a si mesma).
  - Caso recursivo: n * fatorial(n-1).
  - Cada chamada reduz n em 1, entao sempre chega no caso base.

  Pilha para fatorial(4):
    fatorial(4) = 4 * fatorial(3)
                       fatorial(3) = 3 * fatorial(2)
                                          fatorial(2) = 2 * fatorial(1)
                                                             fatorial(1) = 1 * fatorial(0)
                                                                                fatorial(0) = 1  (caso base)
    Desempilhando: 1*1=1 -> 2*1=2 -> 3*2=6 -> 4*6=24
    Resultado: 24

--------------------------------------------------------------------
8.2 FIBONACCI RECURSIVO
--------------------------------------------------------------------

ENUNCIADO: Faca uma funcao recursiva que retorne o N-esimo termo de
  Fibonacci. Sequencia: 0, 1, 1, 2, 3, 5, 8, 13, 21, ...
  Regra: fib(n) = fib(n-1) + fib(n-2), com fib(0)=0 e fib(1)=1.

GABARITO:

    // fibonacci recursivo
    // caso base: fib(0) = 0, fib(1) = 1
    // caso recursivo: fib(n) = fib(n-1) + fib(n-2)
    int fibonacci(int n) {
        if(n == 0) return 0;   // CASO BASE 1
        if(n == 1) return 1;   // CASO BASE 2
        return fibonacci(n - 1) + fibonacci(n - 2);  // CASO RECURSIVO
    }

EXPLICACAO:
  - Fibonacci tem DOIS casos base (0 e 1) porque a recursao chama
    n-1 e n-2. Se so tivesse um caso base, daria erro.
  - Cada chamada gera DUAS chamadas (arvore binaria de recursao).
  - Por isso fibonacci recursivo e lento para N grande (exponencial).

  fib(5):
    fib(5) = fib(4) + fib(3)
           = (fib(3)+fib(2)) + (fib(2)+fib(1))
           = ... = 5

--------------------------------------------------------------------
8.3 IMPRIMIR DE N ATE 1 (a PEGADINHA da ordem do printf)
--------------------------------------------------------------------

ENUNCIADO: Faca uma funcao recursiva que imprima os numeros de N ate 1.

GABARITO - VERSAO DECRESCENTE (printf ANTES da chamada):

    // imprime de N ate 1 (DECRESCENTE)
    // caso base: n == 0 para
    // caso recursivo: imprime n, depois chama recursao(n-1)
    void recursao(int n) {
        if(n == 0) return;          // CASO BASE
        printf("%d ", n);           // imprime ANTES da chamada recursiva
        recursao(n - 1);            // CASO RECURSIVO
    }

  Para recursao(5) imprime: 5 4 3 2 1
  (ordem DECRESCENTE - imprime na IDA da pilha)

  Por que? O printf executa ANTES de chamar recursao(n-1). Entao:
    recursao(5): imprime 5, chama recursao(4)
    recursao(4): imprime 4, chama recursao(3)
    recursao(3): imprime 3, chama recursao(2)
    recursao(2): imprime 2, chama recursao(1)
    recursao(1): imprime 1, chama recursao(0)
    recursao(0): caso base, retorna

--------------------------------------------------------------------

GABARITO - VERSAO CRESCENTE (printf DEPOIS da chamada):

    // imprime de 1 ate N (CRESCENTE)
    // MESMA logica, so troca a ORDEM do printf
    void recursao(int n) {
        if(n == 0) return;          // CASO BASE (igual)
        recursao(n - 1);            // chama ANTES de imprimir
        printf("%d ", n);           // imprime DEPOIS da chamada recursiva
    }

  Para recursao(5) imprime: 1 2 3 4 5
  (ordem CRESCENTE - imprime na VOLTA da pilha)

  Por que? O printf so executa DEPOIS que recursao(n-1) retorna. Entao:
    recursao(5): chama recursao(4) e ESPERA
    recursao(4): chama recursao(3) e ESPERA
    recursao(3): chama recursao(2) e ESPERA
    recursao(2): chama recursao(1) e ESPERA
    recursao(1): chama recursao(0) e ESPERA
    recursao(0): caso base, retorna
    recursao(1): imprime 1, retorna
    recursao(2): imprime 2, retorna
    recursao(3): imprime 3, retorna
    recursao(4): imprime 4, retorna
    recursao(5): imprime 5, retorna
  Resultado: 1 2 3 4 5

  RESUMO DA PEGADINHA (DECORE):
    printf ANTES  -> DECRESCENTE (5 4 3 2 1) - imprime na IDA
    printf DEPOIS -> CRESCENTE  (1 2 3 4 5) - imprime na VOLTA

--------------------------------------------------------------------
8.4 PROGRAMA COMPLETO DE RECURSAO (para testar)
--------------------------------------------------------------------

    #include <stdio.h>
    #include <stdlib.h>

    int fatorial(int n) {
        if(n == 0) return 1;
        return n * fatorial(n - 1);
    }

    int fibonacci(int n) {
        if(n == 0) return 0;
        if(n == 1) return 1;
        return fibonacci(n - 1) + fibonacci(n - 2);
    }

    void decrescente(int n) {
        if(n == 0) return;
        printf("%d ", n);        // ANTES -> decrescente
        decrescente(n - 1);
    }

    void crescente(int n) {
        if(n == 0) return;
        crescente(n - 1);
        printf("%d ", n);        // DEPOIS -> crescente
    }

    int main(void) {
        printf("Fatorial de 5: %d\n", fatorial(5));   // 120
        printf("Fibonacci de 7: %d\n", fibonacci(7)); // 13
        printf("Decrescente(5): ");
        decrescente(5);   // 5 4 3 2 1
        printf("\nCrescente(5): ");
        crescente(5);      // 1 2 3 4 5
        printf("\n");
        return 0;
    }

EXPLICACAO DA PILHA DE CHAMADAS:
  - Cada chamada de funcao cria um "quadro" na pilha com suas variaveis.
  - A recursao empilha quadros ate o caso base.
  - Ao atingir o caso base, comeca a desempilhar (retornar).
  - O que esta DEPOIS da chamada recursiva so executa na volta.
  - Por isso a ordem do printf muda o resultado.

PADROES DO ENUNCIADO:
  "recursao / recursiva"          -> caso base + caso recursivo
  "fatorial"                      -> n * fatorial(n-1), caso base 0! = 1
  "fibonacci"                     -> fib(n-1) + fib(n-2), 2 casos base
  "imprimir de N ate 1"           -> printf antes = decrescente
  "imprimir de 1 ate N"           -> printf depois = crescente
  "caso base"                     -> condicao de parada (if return)

====================================================================
9. RESUMO DOS PADROES (traducao enunciado -> codigo)
====================================================================

PALAVRA NO ENUNCIADO        -> O QUE USAR
---------------------------------------------------------------
"cadastrar / gravar"        -> struct + fwrite (bin) ou fprintf (texto)
"persistir / salvar em arquivo" -> fopen + fwrite/fprintf
"listar / exibir / mostrar" -> fread no while (bin) ou fgets (texto)
"buscar por codigo"          -> fread + if(codigo == buscado)
"buscar por nome (exato)"    -> strcmp == 0
"buscar parcial / contem"    -> strstr != NULL
"alterar / editar"           -> temp + rename (copia e modifica)
"excluir / remover"          -> temp + rename (copia pulando)
"media / soma / total"       -> acumulador em loop
"maior / menor"              -> variavel indice + if em loop
"ordenar por nome"           -> bubble sort + strcmp
"ordenar por numero"         -> bubble sort + > ou <
"recursao / recursiva"       -> caso base + caso recursivo
"fatorial"                   -> n * fat(n-1), base 0! = 1
"fibonacci"                  -> fib(n-1) + fib(n-2), 2 bases
"imprimir de N ate 1"        -> printf antes da chamada
"imprimir de 1 ate N"        -> printf depois da chamada
"contar vogais"              -> fgets + for char a char + tolower
"contar palavras"            -> maquina de estados (dentroPalavra)
"menu / opcao"               -> do-while + switch
"limpar tela"                -> system("cls")
"pausar"                     -> system("pause")
"modular / funcoes"          -> dividir em funcoes com prototipo
"arquivo texto / CSV"        -> fprintf + fgets + strtok + atoi/atof
"arquivo binario"            -> fwrite + fread (while fread == 1)
"vetor de struct"            -> typedef struct + passagem v[], n
"matriz totais"              -> dois for (linha e coluna)
"faixa / entre X e Y"        -> if(v >= min && v <= max) conta++

====================================================================
CHECKLIST ANTES DE ENTREGAR A PROVA
====================================================================
[ ] typedef struct (NAO esquecer o typedef)
[ ] fgets + strcspn para ler strings (NAO usar gets)
[ ] limparBuffer depois de scanf antes de fgets
[ ] while(fread == 1) para ler binario (NAO while(!feof))
[ ] fclose no final de cada fopen
[ ] temp + rename para alterar e excluir
[ ] strcmp para comparar strings (NAO ==)
[ ] strstr para busca parcial
[ ] caso base na recursao (NAO esquecer)
[ ] prototipos das funcoes antes do main
[ ] system("pause") no menu para o usuario ver
[ ] comentarios explicando o codigo

FIM DO DOCUMENTO.


#############################################################
# PARTE 5 - LEITURA DE DADOS (todas as formas)
#############################################################

# GUIA DE LEITURA DE DADOS - TODAS AS FORMAS (GB)

Este documento reune TODAS as formas de ler dados em C, das mais simples
as mais complexas. Use Ctrl+F por: scanf, fgets, sscanf, fscanf, getchar,
scanset, %[^;], %[^#], formato, validacao, buffer, enter, CSV.

=========================================================
INDICE
=========================================================
1. Tabela de formatadores (cola rapida)
2. scanf - ler variavel simples
3. scanf - ler varios valores de uma vez
4. scanf - ler string (palavra, sem espaco)
5. scanf - ler char (espaco antes do %c)
6. fgets - ler frase com espaco
7. Remover o \n do fgets (strcspn)
8. Limpar o buffer (depois de scanf, antes de fgets)
9. getchar - ler um unico caractere
10. sscanf - ler de uma string (parsear)
11. fscanf - ler de arquivo
12. SCANSET %[^...] - ler ate um delimitador (CSV)
13. Ler linha inteira de arquivo com fgets
14. Ler struct de arquivo binario com fread
15. Ler com validacao (repetir ate valido)
16. Ler varios campos com scanf em loop
17. Pegadinhas e erros comuns

=========================================================
1. TABELA DE FORMATADORES (cola rapida)
=========================================================

| Tipo        | scanf  | printf  | Observacao                          |
|-------------|--------|---------|-------------------------------------|
| int         | %d     | %d      |                                     |
| long        | %ld    | %ld     |                                     |
| float       | %f     | %f      |                                     |
| double      | %lf    | %f      | scanf usa %lf, printf usa %f        |
| char        | %c     | %c      | espaco antes: " %c"                 |
| string      | %s     | %s      | nao usa & (ja e endereco)          |
| string seg. | %49s   | %s      | limita tamanho (49 + \0)           |
| unsigned    | %u     | %u      |                                     |
| hex         | %x     | %x      |                                     |
| scanset     | %[^;]  | -       | ler ate o ';' (nao para no espaco)  |

Regra de OURO do scanf:
- variavel SIMPLES (int, float, double, char): usa & (passa endereco)
  scanf("%d", &x);
- STRING (vetor de char): NAO usa & (o nome ja e endereco)
  scanf("%49s", nome);

=========================================================
2. scanf - ler variavel simples
=========================================================

Ler um inteiro. O & passa o endereco da variavel.

```c
int idade;
printf("Idade: ");
scanf("%d", &idade);   // & obrigatorio para int
```

Ler um float:

```c
float nota;
scanf("%f", &nota);    // & obrigatorio
```

Ler um double (CUIDADO: scanf usa %lf, printf usa %f):

```c
double salario;
scanf("%lf", &salario);   // %lf no scanf
printf("%.2f", salario);  // %f no printf (NAO use %lf aqui)
```

=========================================================
3. scanf - ler varios valores de uma vez
=========================================================

Da pra ler varios numeros separados por espaco ou enter.

```c
int a, b, c;
scanf("%d %d %d", &a, &b, &c);   // usuario digita: 10 20 30
```

Ler dois floats:

```c
float n1, n2;
scanf("%f %f", &n1, &n2);   // usuario digita: 7.5 8.0
```

Ler codigo, nome e salario (nome sem espaco):

```c
int codigo;
char nome[50];
float salario;
scanf("%d %s %f", &codigo, nome, &salario);
// usuario: 101 Joao 2500.50
// nome NAO pode ter espaco (so le "Joao")
```

=========================================================
4. scanf - ler string (palavra, sem espaco)
=========================================================

%s le ate o primeiro espaco. Para nomes sem espaco e util.
NAO usa & porque nome (vetor) ja e um endereco.

```c
char nome[50];
scanf("%49s", nome);   // limita a 49 caracteres + \0 (seguro)
```

CUIDADO: scanf("%s", nome) sem limite pode estourar o buffer.
Sempre coloque o tamanho: %49s para char nome[50].

=========================================================
5. scanf - ler char (espaco antes do %c)
=========================================================

Pegadinha classica: depois de um scanf anterior, o enter (\n) fica no
buffer. Se voce usar scanf("%c", &letra), ele le o enter e nao espera
o usuario. Solucao: coloque um ESPACO antes do %c.

```c
char letra;
scanf(" %c", &letra);   // espaco antes do %c ignora enter anterior
```

O espaco no formato diz ao scanf: "pule espacos e enters antes de ler".

=========================================================
6. fgets - ler frase com espaco
=========================================================

fgets le uma linha inteira, incluindo espacos. Para nomes completos,
enderecos, frases. Precisa de <stdio.h>.

```c
char nome[100];
printf("Nome completo: ");
fgets(nome, sizeof(nome), stdin);   // le ate enter ou 99 caracteres
```

Parametros do fgets:
- nome: onde guardar
- sizeof(nome): limite (nao passa do tamanho do vetor)
- stdin: de onde ler (teclado)

CUIDADO: fgets guarda o \n (enter) no fim da string. Tem que tirar.

=========================================================
7. Remover o \n do fgets (strcspn)
=========================================================

fgets coloca o \n no fim. Para tirar, use strcspn (precisa <string.h>).

```c
#include <string.h>

char nome[100];
fgets(nome, sizeof(nome), stdin);
nome[strcspn(nome, "\n")] = '\0';   // troca o \n por \0 (fim da string)
```

Como funciona:
- strcspn(nome, "\n") retorna a POSICAO do primeiro \n.
- Se nao tem \n, retorna o tamanho da string (coloca \0 no fim, sem dano).
- nome[posicao] = '\0' corta a string ali.

Forma alternativa (mais explicita):

```c
size_t tam = strlen(nome);
if (tam > 0 && nome[tam - 1] == '\n') {
    nome[tam - 1] = '\0';
}
```

=========================================================
8. Limpar o buffer (depois de scanf, antes de fgets)
=========================================================

PROBLEMA: se voce faz scanf e depois fgets, o fgets le o \n que sobrou
do scanf e "pula" a leitura da frase.

```c
int idade;
char nome[100];

scanf("%d", &idade);          // usuario digita 20 e aperta enter
                               // o enter (\n) fica no buffer!

fgets(nome, sizeof(nome), stdin);  // le o \n que sobrou -> nome fica vazio
```

SOLUCAO 1 - limpar o buffer com loop de getchar (PORTAVEL, correta):

```c
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // descarta caracteres ate achar enter ou fim
    }
}

// uso:
scanf("%d", &idade);
limparBuffer();   // tira o \n do buffer
fgets(nome, sizeof(nome), stdin);   // agora le certo
```

SOLUCAO 2 - fflush(stdin) (funciona no Windows/MinGW, NAO e portavel):

```c
scanf("%d", &idade);
fflush(stdin);   // limpa o buffer de entrada (so Windows)
fgets(nome, sizeof(nome), stdin);
```

O professor usa fflush(stdin). Funciona na prova (Windows), mas saiba
que nao e padrao C. A forma correta/portavel e o loop de getchar.

SOLUCAO 3 - espaco no scanf (so funciona para proximo scanf de %c ou %s):

```c
scanf("%d", &idade);
scanf(" %49s", nome);   // espaco pula o enter (mas %s nao le espaco)
```

=========================================================
9. getchar - ler um unico caractere
=========================================================

getchar le UM caractere do teclado. Retorna int (o codigo do caractere).

```c
int c = getchar();   // le um caractere
```

Usado para: pausar a tela (system("pause") alternativo), limpar buffer,
ler opcao de menu caractere por caractere.

Ler opcao de menu com getchar:

```c
printf("Continuar? (s/n): ");
int c = getchar();
if (c == 's' || c == 'S') {
    // continua
}
```

=========================================================
10. sscanf - ler de uma string (parsear)
=========================================================

sscanf le dados de uma STRING (nao do teclado). Serve para extrair
numeros e textos de uma string formatada. Muito util para processar
linhas de arquivo ou entradas complexas.

```c
char linha[] = "101 Joao 2500.50";
int codigo;
char nome[50];
float salario;

sscanf(linha, "%d %s %f", &codigo, nome, &salario);
// codigo = 101, nome = "Joao", salario = 2500.50
```

Exemplo: extrair dia, mes, ano de uma data:

```c
char data[] = "25/12/2025";
int dia, mes, ano;

sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
// dia = 25, mes = 12, ano = 2025
```

Exemplo: ler de uma linha CSV com sscanf + scanset (ver item 12):

```c
char linha[] = "101;Joao Silva;2500.50";
int codigo;
char nome[50];
float salario;

sscanf(linha, "%d;%49[^;];%f", &codigo, nome, &salario);
// codigo = 101, nome = "Joao Silva", salario = 2500.50
```

=========================================================
11. fscanf - ler de arquivo
=========================================================

fscanf le dados de um ARQUIVO (nao do teclado). Igual ao scanf mas
precisa do ponteiro do arquivo.

```c
FILE *arq = fopen("dados.txt", "r");
if (arq == NULL) { printf("erro"); return 1; }

int codigo;
char nome[50];
float salario;

// le enquanto conseguir ler os 3 campos
while (fscanf(arq, "%d %s %f", &codigo, nome, &salario) == 3) {
    printf("%d %s %.2f\n", codigo, nome, salario);
}

fclose(arq);
```

O == 3 verifica se leu os 3 campos. Se fscanf retornar menos que 3
(ou EOF), o loop para. NAO use feof para isso.

Ler de arquivo com fscanf e formato CSV (virgula):

```c
while (fscanf(arq, "%d,%49[^,],%f", &codigo, nome, &salario) == 3) {
    // codigo, nome (ate virgula), salario
}
```

=========================================================
12. SCANSET %[^...] - ler ate um delimitador (CSV)
=========================================================

O scanset e a forma de ler STRINGS COM ESPACO ate um delimitador.
E o recurso mais poderoso do scanf/sscanf/fscanf para ler dados.

Sintaxe: %[^\n] le tudo ate o enter. %[^;] le tudo ate o ponto-e-virgula.
%[^,] le tudo ate a virgula. %[^#] le tudo ate o #.

O ^ significa "negacao": le todos os caracteres QUE NAO sao os listados.

Exemplo: ler nome completo com espaco (ate o enter):

```c
char nome[100];
scanf(" %99[^\n]", nome);   // le tudo ate o enter (inclui espacos)
// o espaco antes do % ignora enter anterior
```

Exemplo: ler linha CSV com ponto-e-virgula (MODELO DO PROJETO FUTEBOL):

```c
// arquivo: 10;Joao Silva;25;atacante;7.5
int codigo;
char nome[50];
int idade;
char posicao[20];
float nota;

fscanf(arq, "%d;%49[^;];%d;%19[^;];%f",
       &codigo, nome, &idade, posicao, &nota);
// codigo = 10
// nome = "Joao Silva" (le ate o ;)
// idade = 25
// posicao = "atacante" (le ate o ;)
// nota = 7.5
```

Como funciona o %49[^;]:
- %49 limita a 49 caracteres (seguro)
- [^;] le todos os caracteres ate achar um ;
- o ; NAO e consumido sozinho, mas o proximo %d ou ; no formato cuida dele

Exemplo: ler data no formato dd/mm/aaaa:

```c
int dia, mes, ano;
scanf("%d/%d/%d", &dia, &mes, &ano);
// usuario: 25/12/2025
```

Exemplo: ler nome ate o # (delimitador customizado):

```c
char nome[100];
scanf(" %99[^#]", nome);   // le tudo ate o #
```

Exemplo completo: ler struct de arquivo CSV com sscanf:

```c
typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Func;

Func lerFuncDeLinha(char *linha) {
    Func f;
    // linha: "101;Joao Silva;2500.50"
    sscanf(linha, "%d;%49[^;];%f", &f.codigo, f.nome, &f.salario);
    return f;
}
```

=========================================================
13. Ler linha inteira de arquivo com fgets
=========================================================

Para processar arquivo texto linha por linha (relatorios, logs):

```c
FILE *arq = fopen("relatorio.txt", "r");
char linha[200];

// fgets retorna NULL quando chega no fim do arquivo
while (fgets(linha, sizeof(linha), arq) != NULL) {
    linha[strcspn(linha, "\n")] = '\0';   // tira o \n
    printf("%s\n", linha);
}

fclose(arq);
```

NAO use while(!feof(arq)) com fgets antes do loop. Use fgets direto
no while (retorna NULL no fim). E a forma correta.

=========================================================
14. Ler struct de arquivo binario com fread
=========================================================

Para arquivo binario (.dat, .bin), use fread para ler uma struct inteira
de uma vez.

```c
typedef struct {
    int codigo;
    char nome[50];
    float salario;
} Func;

FILE *arq = fopen("func.dat", "rb");
Func f;

// fread retorna o numero de registros lidos (1 se leu, 0 se fim)
while (fread(&f, sizeof(Func), 1, arq) == 1) {
    printf("%d %s %.2f\n", f.codigo, f.nome, f.salario);
}

fclose(arq);
```

Parametros do fread:
- &f: onde guardar (endereco da struct)
- sizeof(Func): tamanho de cada registro
- 1: quantos registros ler de uma vez
- arq: de onde ler

O == 1 verifica se leu 1 registro. NAO use while(!feof) com fread antes.

=========================================================
15. Ler com validacao (repetir ate valido)
=========================================================

Para garantir que o usuario digitou um valor valido (nota entre 0 e 10,
idade positiva, etc), use um while que repete ate estar certo.

Validar nota entre 0 e 10:

```c
float nota;
printf("Nota (0-10): ");
scanf("%f", &nota);

while (nota < 0 || nota > 10) {
    printf("Nota invalida. Digite novamente (0-10): ");
    scanf("%f", &nota);
}
```

Validar idade positiva:

```c
int idade;
scanf("%d", &idade);
while (idade < 0) {
    printf("Idade invalida. Digite novamente: ");
    scanf("%d", &idade);
}
```

Validar opcao de menu:

```c
int opcao;
scanf("%d", &opcao);
while (opcao < 1 || opcao > 5) {
    printf("Opcao invalida (1-5): ");
    scanf("%d", &opcao);
}
```

Validar que string so tem digitos (CPF, por exemplo):

```c
#include <ctype.h>

int soDigitos(char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) return 0;   // achou nao-digito
    }
    return 1;   // todos sao digitos
}
```

=========================================================
16. Ler varios campos com scanf em loop
=========================================================

Padrao para ler varios registros ate uma condicao de parada (sentinela):

```c
int codigo;
char nome[50];
float salario;

printf("Codigo (0 para sair): ");
scanf("%d", &codigo);

while (codigo != 0) {
    printf("Nome: ");
    scanf(" %49[^\n]", nome);   // le nome com espaco ate enter
    printf("Salario: ");
    scanf("%f", &salario);

    // ... processa o registro (gravar, calcular, etc)

    printf("Codigo (0 para sair): ");
    scanf("%d", &codigo);
}
```

=========================================================
17. PEGADINHAS E ERROS COMUNS
=========================================================

1. Esquecer o & no scanf de int/float/double/char:
   ERRADO: scanf("%d", x);
   CERTO:  scanf("%d", &x);

2. Colocar & em string no scanf:
   ERRADO: scanf("%s", &nome);
   CERTO:  scanf("%49s", nome);

3. Usar %f para double no scanf:
   ERRADO: scanf("%f", &salario_double);
   CERTO:  scanf("%lf", &salario_double);
   (no printf, double usa %f, NAO %lf)

4. fgets depois de scanf sem limpar buffer:
   O fgets le o \n que sobrou e pula a leitura.
   SOLUCAO: limparBuffer() ou fflush(stdin) entre eles.

5. scanf("%c") depois de outro scanf le o enter:
   ERRADO: scanf("%c", &letra);   // le o \n
   CERTO:  scanf(" %c", &letra);  // espaco ignora o enter

6. Nao limitar o tamanho da string no scanf:
   ERRADO: scanf("%s", nome);     // pode estourar o buffer
   CERTO:  scanf("%49s", nome);   // limita a 49

7. Usar while(!feof) com fread/fgets antes do loop:
   Le o ultimo registro duas vezes ou imprime lixo.
   CERTO: while(fread(...) == 1) ou while(fgets(...) != NULL)

8. Comparar string com ==:
   ERRADO: if (nome == "Ana")
   CERTO:  if (strcmp(nome, "Ana") == 0)

9. Nao tirar o \n do fgets:
   A string fica com enter no fim, quebra comparacoes e gravacao.
   SOLUCAO: nome[strcspn(nome, "\n")] = '\0';

10. Misturar gets() (removido do C):
    ERRADO: gets(nome);
    CERTO:  fgets(nome, sizeof(nome), stdin);

=========================================================
RESUMO: QUAL FORMA USAR?
=========================================================

- Ler numero simples: scanf("%d", &x)
- Ler palavra (sem espaco): scanf("%49s", nome)
- Ler frase (com espaco): fgets(nome, sizeof(nome), stdin)
- Ler char: scanf(" %c", &letra)
- Ler nome com espaco via scanf: scanf(" %49[^\n]", nome)
- Ler CSV com delimitador: scanf("%d;%49[^;];%f", ...)
- Ler de string: sscanf(linha, "%d %s %f", ...)
- Ler de arquivo texto: fscanf(arq, "%d %s %f", ...) ou fgets
- Ler de arquivo binario: fread(&f, sizeof(Func), 1, arq)
- Ler um caractere: getchar()
- Validar entrada: while com condicao de validade


#############################################################
# PARTE 6 - COLA RAPIDA (snippets prontos)
#############################################################

# COLA RAPIDA GB - Referencia densa de snippets C para a prova

> Material de CONSULTA para prova do Grau B (GB) de Algoritmos e Programacao II (Uniftec).
> Linguagem C pura, compila com `gcc -Wall -Wextra` no Windows (MinGW).
> Formato: snippet pronto + 1 linha de comentario. Denso, sem enrolacao.

---

## PALAVRAS-CHAVE Ctrl+F

```
ESQUELETO, LER_INT, LER_FLOAT, LER_DOUBLE, LER_CHAR, LER_STRING, LER_FRASE,
LIMPAR_BUFFER, IF_ELSE, ELSE_IF, SWITCH, TERNARIO, FOR, WHILE, DO_WHILE,
VETOR_LER, VETOR_IMPRIMIR, VETOR_SOMA, VETOR_MEDIA, VETOR_MAIOR, VETOR_MENOR,
VETOR_BUSCAR, VETOR_CONTAR, VETOR_BUBBLE, MATRIZ_LER, MATRIZ_IMPRIMIR,
MATRIZ_SOMA_TOTAL, MATRIZ_SOMA_LINHA, MATRIZ_SOMA_COLUNA, MATRIZ_DIAGONAL,
STRING_FGETS, STRLEN, STRCMP, STRCPY, STRCAT, STRTOK, STRSTR, SPRINTF, ATOI,
TOUPPER, TOLOWER, ISDIGIT, ISALPHA, STRUCT_TYPEDEF, STRUCT_ACESSAR,
STRUCT_VETOR, STRUCT_PONTEIRO, FUNCAO_RETORNO, FUNCAO_VOID, FUNCAO_VETOR,
FUNCAO_MATRIZ, PROTOTIPO, PONTEIRO_ALTERAR, PONTEIRO_TROCAR,
ARQ_TEXTO_ESCREVER, ARQ_TEXTO_LER, ARQ_TEXTO_APPEND, ARQ_FEOF,
ARQ_BIN_ESCREVER, ARQ_BIN_LER, ARQ_FSEEK, ARQ_FTELL, ARQ_CONTAR_REG,
CRUD_ALTERAR, CRUD_EXCLUIR, RECURS_FATORIAL, RECURS_FIBONACCI, RECURS_IMPRIMIR,
CALC_MEDIA, CALC_PORCENTAGEM, CALC_FATORIAL, CALC_PRIMO, CALC_MDC, CALC_BHASKARA,
MENU_SWITCH, VALIDACAO, TABELA_PRINTF, TABELA_FOPEN, TABELA_STRING, TABELA_CTYPE,
ERROS_COMUNS
```

---

## 1. ESQUELETO basico de programa

```c
/* ESQUELETO: includes + main padrao do professor */
#include <stdio.h>      /* printf, scanf, fgets, fopen, etc.            */
#include <stdlib.h>     /* system, malloc, atoi, atof, exit            */
#include <string.h>     /* strlen, strcmp, strcpy, strcat, strtok     */
#include <ctype.h>      /* toupper, tolower, isdigit, isalpha          */
#include <conio.h>      /* getche, getch (Windows/MinGW)               */

int main(void) {
    system("cls");      /* limpa tela no Windows                        */
    /* ... codigo ...  */
    system("pause");    /* "Pressione qualquer tecla..." no Windows    */
    return 0;           /* 0 = programa terminou sem erro              */
}
```

---

## 2. Ler int, float, double, char, string, frase

```c
/* LER_INT: le inteiro com scanf */
int n;
scanf("%d", &n);                 /* & obrigatorio para tipos escalares  */

/* LER_FLOAT: le float (4 bytes, ~6 digitos de precisao) */
float f;
scanf("%f", &f);

/* LER_DOUBLE: le double (8 bytes, ~15 digitos) - use %lf no scanf */
double d;
scanf("%lf", &d);                /* %lf no scanf; %f no printf           */

/* LER_CHAR: le um unico caractere (cuidado com \n pendente) */
char c;
scanf(" %c", &c);                /* espaco antes de %c ignora \n e espacos */

/* LER_STRING: le uma palavra (ate espaco) com scanf */
char nome[50];
scanf("%49s", nome);             /* sem & (nome ja e endereco); 49 limita */

/* LER_FRASE: le linha inteira com espacos usando fgets */
char frase[100];
fflush(stdin);                   /* nao portavel, mas funciona no Windows */
fgets(frase, sizeof(frase), stdin);
frase[strcspn(frase, "\n")] = '\0'; /* remove o \n que o fgets captura   */
```

---

## 3. LIMPAR_BUFFER (3 formas)

```c
/* LIMPAR_BUFFER forma 1: fflush(stdin) - simples, so Windows */
fflush(stdin);                   /* nao portavel mas funciona no Windows  */

/* LIMPAR_BUFFER forma 2: while com getchar - PORTAVEL, preferida em prova */
int ch;
while ((ch = getchar()) != '\n' && ch != EOF);  /* descarta ate \n ou EOF */

/* LIMPAR_BUFFER forma 3: scanf com %[^\n] descarta o resto da linha */
scanf("%*[^\n]");                /* descarta tudo ate \n                   */
scanf("%*c");                    /* descarta o proprio \n                 */
```

---

## 4. if/else, else if, switch, operador ternario

```c
/* IF_ELSE: decisao simples */
if (n > 0) {
    printf("positivo\n");
} else {
    printf("zero ou negativo\n");
}

/* ELSE_IF: cadeia de decisoes (ordem importa!) */
if (n >= 9)      printf("A\n");  /* conceito A                    */
else if (n >= 7) printf("B\n");  /* se n>=9 caiu no primeiro      */
else if (n >= 5) printf("C\n");
else             printf("D\n");

/* SWITCH: selecao multipla sobre int/char (NAO sobre float/string) */
switch (opcao) {
    case 1: printf("Incluir\n");  break;  /* break evita cair no proximo */
    case 2: printf("Listar\n");  break;
    case 3: printf("Sair\n");    break;
    default: printf("Invalido\n");        /* default = caso nenhum bateu */
}

/* TERNARIO: if/else em uma linha - use so em atribuicoes simples */
int maior = (a > b) ? a : b;     /* se a>b, maior=a; senao maior=b       */
```

---

## 5. for, while, do-while

```c
/* FOR: contador determinado (sabe quantas vezes repete) */
for (int i = 0; i < 10; i++) {   /* i++ = i = i+1 (pos-incremento)        */
    printf("%d ", i);            /* imprime 0 1 2 ... 9                   */
}

/* WHILE: repete ENQUANTO condicao verdadeira (pode nao executar nenhuma vez) */
int i = 0;
while (i < 10) {
    printf("%d ", i);
    i++;                         /* CUIDADO: esquecer i++ = loop infinito */
}

/* DO_WHILE: executa pelo menos 1 vez, depois testa - ideal para MENU */
int op;
do {
    printf("1-Incluir 2-Sair\n");
    scanf("%d", &op);
} while (op != 2);               /* ponto-e-virgula OBRIGATORIO no fim    */
```

---

## 6. Vetor: ler, imprimir, soma, media, maior, menor, buscar, contar, ordenar (bubble)

```c
/* VETOR_LER: le N valores para o vetor */
int v[100], n;
scanf("%d", &n);
for (int i = 0; i < n; i++) {
    scanf("%d", &v[i]);           /* indice comeca em 0, vai ate n-1       */
}

/* VETOR_IMPRIMIR: mostra todos os elementos */
for (int i = 0; i < n; i++) {
    printf("%d ", v[i]);
}
printf("\n");

/* VETOR_SOMA: acumula todos os elementos */
int soma = 0;
for (int i = 0; i < n; i++) {
    soma += v[i];                 /* soma = soma + v[i]                    */
}

/* VETOR_MEDIA: soma dividida pela quantidade */
float media = (float)soma / n;    /* cast para float antes da divisao       */
/* CUIDADO: soma/n com ambos int faz divisao inteira (trunca)             */

/* VETOR_MAIOR: acha o maior elemento */
int maior = v[0];                 /* inicializa com o PRIMEIRO, nao com 0  */
for (int i = 1; i < n; i++) {     /* comeca em 1 pois v[0] ja e o maior    */
    if (v[i] > maior) maior = v[i];
}

/* VETOR_MENOR: acha o menor elemento */
int menor = v[0];
for (int i = 1; i < n; i++) {
    if (v[i] < menor) menor = v[i];
}

/* VETOR_BUSCAR: procura um valor, retorna indice ou -1 */
int busca = 7, pos = -1;
for (int i = 0; i < n; i++) {
    if (v[i] == busca) { pos = i; break; }  /* break para na 1a ocorrencia */
}
/* pos == -1 significa "nao achou"                                       */

/* VETOR_CONTAR: conta quantas vezes um valor aparece */
int cont = 0;
for (int i = 0; i < n; i++) {
    if (v[i] == busca) cont++;
}

/* VETOR_BUBBLE: ordenacao bolha (crescente) - compara pares adjacentes */
for (int i = 0; i < n - 1; i++) {           /* n-1 passagens             */
    for (int j = 0; j < n - 1 - i; j++) {    /* -i pois os ultimos ja     */
        if (v[j] > v[j + 1]) {               /* estao ordenados           */
            int aux = v[j];                  /* troca usando variavel aux */
            v[j] = v[j + 1];
            v[j + 1] = aux;
        }
    }
}
/* Para DECRESCENTE, troque > por < na comparacao                         */
```

---

## 7. Matriz: ler, imprimir, soma total, soma linha, soma coluna, diagonal

```c
/* MATRIZ_LER: le matriz LxC */
int m[10][10], L, C;
scanf("%d %d", &L, &C);
for (int i = 0; i < L; i++) {            /* i = linha                    */
    for (int j = 0; j < C; j++) {        /* j = coluna                   */
        scanf("%d", &m[i][j]);
    }
}

/* MATRIZ_IMPRIMIR: mostra matriz formatada */
for (int i = 0; i < L; i++) {
    for (int j = 0; j < C; j++) {
        printf("%d\t", m[i][j]);         /* \t alinha em colunas         */
    }
    printf("\n");                        /* nova linha ao fim de cada linha */
}

/* MATRIZ_SOMA_TOTAL: soma todos os elementos */
int soma = 0;
for (int i = 0; i < L; i++)
    for (int j = 0; j < C; j++)
        soma += m[i][j];

/* MATRIZ_SOMA_LINHA: soma de uma linha especifica (linha k) */
int k = 2, somaLinha = 0;
for (int j = 0; j < C; j++)              /* percorre so as colunas       */
    somaLinha += m[k][j];

/* MATRIZ_SOMA_COLUNA: soma de uma coluna especifica (coluna k) */
int somaCol = 0;
for (int i = 0; i < L; i++)              /* percorre so as linhas        */
    somaCol += m[i][k];

/* MATRIZ_DIAGONAL: soma da diagonal principal (so vale p/ matriz quadrada L==C) */
int somaDiag = 0;
for (int i = 0; i < L; i++)
    somaDiag += m[i][i];                 /* diagonal principal: i == j   */

/* Diagonal SECUNDARIA: i + j == L-1, ou seja m[i][L-1-i] */
int somaDiagSec = 0;
for (int i = 0; i < L; i++)
    somaDiagSec += m[i][L - 1 - i];
```

---

## 8. String: fgets+strcspn, strlen, strcmp, strcpy, strcat, strtok, strstr, sprintf, atoi/atof

```c
/* STRING_FGETS: le frase com espacos e remove o \n */
char s[100];
fgets(s, sizeof(s), stdin);
s[strcspn(s, "\n")] = '\0';             /* strcspn acha posicao do \n   */

/* STRLEN: tamanho da string (sem contar o \0) */
size_t tam = strlen(s);                  /* "abc" -> 3                   */

/* STRCMP: compara strings. 0=iguais, >0=s1>s2, <0=s1<s2 */
if (strcmp(s1, s2) == 0)                /* NUNCA use s1==s2 (compara enderecos) */
    printf("iguais\n");

/* STRCPY: copia string de origem para destino */
strcpy(destino, origem);                 /* destino deve ter espaco suficiente */

/* STRCAT: concatena origem ao fim de destino */
strcat(destino, origem);                 /* destino cresce; verifique o tamanho */

/* STRTOK: divide string por delimitador (ex: virgula) */
char *token = strtok(s, ",");
while (token != NULL) {                  /* strtok retorna NULL quando acaba */
    printf("%s\n", token);
    token = strtok(NULL, ",");           /* 1a chamada passa a string; proximas passam NULL */
}

/* STRSTR: procura substring, retorna ponteiro ou NULL */
if (strstr(texto, "casa") != NULL)      /* achou "casa" dentro de texto */
    printf("encontrado\n");

/* SPRINTF: formata e guarda em string (em vez de imprimir) */
char buffer[100];
sprintf(buffer, "Nome=%s Idade=%d", nome, idade);

/* ATOI / ATOF: converte string para int / float */
int num = atoi("42");                    /* 42                           */
float f = atof("3.14");                  /* 3.14                         */
/* Se a string nao for numero, atoi retorna 0                       */
```

---

## 9. ctype: toupper, tolower, isdigit, isalpha

```c
/* TOUPPER: converte caractere para maiusculo */
char mai = toupper(c);                   /* 'a' -> 'A'                   */

/* TOLOWER: converte para minusculo */
char min = tolower(c);                   /* 'A' -> 'a'                   */

/* ISDIGIT: retorna nao-zero se c for digito 0-9 */
if (isdigit(c))                          /* '5' -> verdadeiro; 'a' -> falso */
    printf("digito\n");

/* ISALPHA: retorna nao-zero se c for letra A-Z a-z */
if (isalpha(c))                          /* 'a' -> verdadeiro; '5' -> falso */
    printf("letra\n");

/* Converter string inteira para maiusculo */
for (int i = 0; s[i] != '\0'; i++)        /* ate o \0 (fim da string)     */
    s[i] = toupper(s[i]);
```

---

## 10. Struct: typedef, declarar, acessar, vetor de struct, ponteiro ->

```c
/* STRUCT_TYPEDEF: define um tipo struct com alias (sem precisar de "struct" na declaracao) */
typedef struct {
    int    matricula;
    char   nome[50];
    float  nota;
} Aluno;                                 /* Aluno vira um tipo, como int */

/* Declarar e acessar */
Aluno a;
a.matricula = 123;                       /* acesso por ponto: variavel.campo */
strcpy(a.nome, "Maria");
a.nota = 8.5;

/* STRUCT_VETOR: vetor de structs (lista de alunos) */
Aluno turma[50];
turma[0].matricula = 1;                  /* vetor[i].campo                */
turma[0].nota = 7.0;

/* STRUCT_PONTEIRO: acesso por ponteiro usa -> (seta) em vez de ponto */
Aluno *p = &a;
p->matricula = 999;                      /* p->campo equivale a (*p).campo */
printf("%s\n", p->nome);
```

---

## 11. Funcao: com retorno, void, com vetor, com matriz, prototipo

```c
/* PROTOTIPO: declaracao antes do main (o professor exige) */
int soma(int a, int b);                  /* so assinatura, sem corpo     */
void mostrar(int v[], int n);

/* FUNCAO_RETORNO: funcao que retorna um valor */
int soma(int a, int b) {
    return a + b;                         /* devolve o resultado          */
}

/* FUNCAO_VOID: funcao que nao retorna nada (so executa) */
void saudacao(void) {                    /* void nos parametros = sem args */
    printf("Ola!\n");
}

/* FUNCAO_VETOR: passa vetor como ponteiro (alteracoes afetam o original) */
void dobrar(int v[], int n) {            /* v[] e tratado como ponteiro   */
    for (int i = 0; i < n; i++)
        v[i] *= 2;                       /* altera o vetor ORIGINAL       */
}
/* Chamada: dobrar(v, n); - sem & (vetor ja e endereco)                */

/* FUNCAO_MATRIZ: passa matriz - numero de colunas e OBRIGATORIO no parametro */
void mostrarMat(int m[][10], int L, int C) {  /* so a 1a dim pode ficar vazia */
    for (int i = 0; i < L; i++)
        for (int j = 0; j < C; j++)
            printf("%d ", m[i][j]);
}
```

---

## 12. Ponteiro: & e *, alterar valor, trocar valores

```c
/* PONTEIRO_ALTERAR: funcao que modifica variavel via ponteiro */
void triplicar(int *p) {                /* *p = conteudo apontado        */
    *p = *p * 3;                         /* altera a variavel original    */
}
/* Chamada: triplicar(&x); - & pega o endereco da variavel            */

/* PONTEIRO_TROCAR: troca dois valores usando ponteiros (classico de prova) */
void trocar(int *a, int *b) {
    int aux = *a;                        /* guarda valor de a            */
    *a = *b;                             /* a recebe valor de b          */
    *b = aux;                            /* b recebe o antigo de a       */
}
/* Chamada: trocar(&x, &y);                                            */

/* Conceito: & = "endereco de" (referencia); * = "conteudo de" (dereferencia) */
int x = 10;
int *p = &x;                             /* p guarda o endereco de x      */
printf("%d\n", *p);                      /* 10 (valor apontado)          */
printf("%p\n", (void*)p);                /* endereco de memoria          */
```

---

## 13. Arquivo texto: abrir w/r/a, fprintf, fscanf, fgets, feof correto

```c
/* ARQ_TEXTO_ESCREVER: abre em modo "w" (write - cria ou SOBRESCREVE) */
FILE *f = fopen("dados.txt", "w");       /* NULL se falhou (verifique!)  */
if (f == NULL) { printf("Erro\n"); return 1; }
fprintf(f, "%d %s\n", 42, "texto");       /* igual printf, mas no arquivo */
fclose(f);                               /* SEMPRE feche o arquivo        */

/* ARQ_TEXTO_LER: abre em modo "r" (read) */
FILE *f = fopen("dados.txt", "r");
if (f == NULL) { printf("Erro\n"); return 1; }
int num;
char palavra[50];
fscanf(f, "%d %49s", &num, palavra);     /* igual scanf, mas do arquivo  */
fclose(f);

/* ARQ_TEXTO_APPEND: abre em modo "a" (append - adiciona ao fim, nao apaga) */
FILE *f = fopen("log.txt", "a");
fprintf(f, "nova linha\n");
fclose(f);

/* ARQ_FEOF: ler TODAS as linhas com fgets (forma CORRETA) */
FILE *f = fopen("dados.txt", "r");
char linha[200];
while (fgets(linha, sizeof(linha), f) != NULL) {  /* NULL = fim/erro    */
    printf("%s", linha);                 /* fgets ja inclui o \n          */
}
fclose(f);
/* CUIDADO: NUNCA use while(!feof(f)) com fscanf/fgets antes do loop -
   feof so vira verdadeiro DEPOIS de uma leitura falhar, causando repeticao
   do ultimo registro. Use o retorno da propria funcao de leitura.       */
```

---

## 14. Arquivo binario: fwrite, fread, fseek, ftell, contar registros

```c
/* ARQ_BIN_ESCREVER: grava struct em arquivo binario com fwrite */
typedef struct { int id; char nome[30]; } Pessoa;
Pessoa p = {1, "Joao"};
FILE *f = fopen("dados.bin", "wb");       /* "wb" = write binary          */
fwrite(&p, sizeof(Pessoa), 1, f);         /* 1 registro de tamanho sizeof */
fclose(f);

/* ARQ_BIN_LER: le struct do arquivo binario com fread */
FILE *f = fopen("dados.bin", "rb");       /* "rb" = read binary           */
Pessoa p;
while (fread(&p, sizeof(Pessoa), 1, f) == 1) {  /* ==1 = leu 1 registro ok */
    printf("%d %s\n", p.id, p.nome);
}
fclose(f);
/* fread retorna o NUMERO de registros lidos. Comparar ==1 e a forma correta. */

/* ARQ_FSEEK: posiciona o cursor (offset, origem) - origens: SEEK_SET(inicio),
   SEEK_CUR (atual), SEEK_END (fim)                                       */
fseek(f, 0, SEEK_SET);                    /* vai para o inicio do arquivo  */
fseek(f, 0, SEEK_END);                    /* vai para o fim do arquivo    */

/* ARQ_FTELL: retorna posicao atual do cursor (em bytes) */
long pos = ftell(f);

/* ARQ_CONTAR_REG: conta quantos registros existem no arquivo binario */
fseek(f, 0, SEEK_END);                    /* vai para o fim                */
long tamBytes = ftell(f);                 /* tamanho total em bytes       */
int qtdRegistros = tamBytes / sizeof(Pessoa);  /* divide pelo tamanho do registro */
fseek(f, 0, SEEK_SET);                    /* volta para o inicio          */
```

---

## 15. CRUD padrao temp+rename (alterar e excluir)

```c
/* CRUD_ALTERAR: altera um registro copiando para arquivo temporario */
/* Estrategia: copia todos, trocando o registro desejado; depois renomeia */
FILE *orig = fopen("dados.bin", "rb");   /* original: leitura            */
FILE *tmp = fopen("temp.bin", "wb");     /* temporario: escrita          */
Pessoa p;
int idBusca = 3;
while (fread(&p, sizeof(Pessoa), 1, orig) == 1) {
    if (p.id == idBusca) {               /* achou o registro a alterar   */
        printf("Novo nome: ");
        fflush(stdin);
        fgets(p.nome, sizeof(p.nome), stdin);
        p.nome[strcspn(p.nome, "\n")] = '\0';
    }
    fwrite(&p, sizeof(Pessoa), 1, tmp);  /* grava (alterado ou nao) no tmp */
}
fclose(orig);
fclose(tmp);
remove("dados.bin");                     /* apaga o original             */
rename("temp.bin", "dados.bin");         /* temporario vira o novo original */

/* CRUD_EXCLUIR: exclui registro copiando todos EXCETO o desejado */
FILE *orig = fopen("dados.bin", "rb");
FILE *tmp = fopen("temp.bin", "wb");
Pessoa p;
int idExcluir = 3;
while (fread(&p, sizeof(Pessoa), 1, orig) == 1) {
    if (p.id != idExcluir) {             /* so copia se NAO for o excluido */
        fwrite(&p, sizeof(Pessoa), 1, tmp);
    }
}
fclose(orig);
fclose(tmp);
remove("dados.bin");
rename("temp.bin", "dados.bin");
/* PADRAO: sempre fecha ambos os arquivos ANTES de remove/rename         */
```

---

## 16. Recursao: fatorial, fibonacci, imprimir N ate 1

```c
/* RECURS_FATORIAL: n! = n * (n-1)!, caso base 0! = 1 */
int fatorial(int n) {
    if (n <= 1) return 1;                /* caso base (para a recursao)   */
    return n * fatorial(n - 1);          /* chamada recursiva             */
}
/* fatorial(4) = 4 * fatorial(3) = 4*3*2*1 = 24                          */

/* RECURS_FIBONACCI: fib(n) = fib(n-1) + fib(n-2), bases fib(0)=0 fib(1)=1 */
int fibonacci(int n) {
    if (n == 0) return 0;                /* caso base 1                   */
    if (n == 1) return 1;                /* caso base 2                   */
    return fibonacci(n - 1) + fibonacci(n - 2);  /* duas chamadas recursivas */
}
/* Sequencia: 0 1 1 2 3 5 8 13 21 34 ...                                */

/* RECURS_IMPRIMIR: imprime de N ate 1 recursivamente */
void imprimir(int n) {
    printf("%d ", n);                     /* imprime ANTES de chamar       */
    if (n > 1) imprimir(n - 1);          /* so chama se n > 1 (cuida do base) */
}
/* imprimir(5) -> 5 4 3 2 1                                              */
/* Para imprimir 1 ate N, inverta: chame recursao ANTES do printf        */
```

---

## 17. Calculos: media, porcentagem, fatorial, primo, MDC, bhaskara

```c
/* CALC_MEDIA: media aritmetica de 3 valores */
float media = (a + b + c) / 3.0f;         /* 3.0f força divisao float     */

/* CALC_PORCENTAGEM: x% de um valor */
float pct = (parte / total) * 100.0f;     /* cast para float antes de dividir */
/* CUIDADO: parte/total com ambos int trunca. Use cast ou multiplique por 100.0 */

/* CALC_FATORIAL: versao iterativa (mais eficiente que recursiva) */
int fat = 1;
for (int i = 2; i <= n; i++) fat *= i;    /* 5! = 1*2*3*4*5 = 120         */

/* CALC_PRIMO: verifica se n e primo (so divisivel por 1 e por ele mesmo) */
int ehPrimo(int n) {
    if (n < 2) return 0;                  /* 0 e 1 nao sao primos         */
    for (int i = 2; i * i <= n; i++) {    /* so ate raiz de n (i*i <= n)  */
        if (n % i == 0) return 0;         /* achou divisor -> nao e primo */
    }
    return 1;                             /* e primo                       */
}

/* CALC_MDC: Maximo Divisor Comum pelo algoritmo de Euclides */
int mdc(int a, int b) {
    while (b != 0) {
        int r = a % b;                    /* resto da divisao             */
        a = b;
        b = r;
    }
    return a;                             /* quando b==0, a e o MDC       */
}

/* CALC_BHASKARA: formula quadratica para ax^2 + bx + c = 0 */
float a, b, c;
scanf("%f %f %f", &a, &b, &c);
float delta = b * b - 4 * a * c;
if (delta < 0) {
    printf("Sem raizes reais\n");
} else if (delta == 0) {
    float x = -b / (2 * a);
    printf("Raiz unica: %.2f\n", x);
} else {
    float x1 = (-b + sqrt(delta)) / (2 * a);  /* precisa de #include <math.h> */
    float x2 = (-b - sqrt(delta)) / (2 * a);  /* compilar com -lm no gcc      */
    printf("x1=%.2f x2=%.2f\n", x1, x2);
}
```

---

## 18. Menu do-while+switch completo

```c
/* MENU_SWITCH: menu classico do-while + switch */
int op;
do {
    system("cls");
    printf("===== SISTEMA =====\n");
    printf("1 - Incluir\n");
    printf("2 - Listar\n");
    printf("3 - Alterar\n");
    printf("4 - Excluir\n");
    printf("5 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &op);
    switch (op) {
        case 1: /* chamar funcao incluir */ break;
        case 2: /* chamar funcao listar  */ break;
        case 3: /* chamar funcao alterar */ break;
        case 4: /* chamar funcao excluir */ break;
        case 5: printf("Saindo...\n"); break;
        default: printf("Opcao invalida!\n");
    }
    system("pause");                      /* espera tecla antes de limpar  */
} while (op != 5);                        /* repete ate escolher Sair (5)  */
```

---

## 19. Validacao de entrada (while ate valido)

```c
/* VALIDACAO: le nota entre 0 e 10, repetindo ate ser valida */
float nota;
do {
    printf("Nota (0-10): ");
    scanf("%f", &nota);
    if (nota < 0 || nota > 10)           /* || = OU logico               */
        printf("Invalida! Tente novamente.\n");
} while (nota < 0 || nota > 10);         /* repete enquanto invalida      */

/* VALIDACAO com while: le inteiro positivo */
int n;
printf("Digite n>0: ");
scanf("%d", &n);
while (n <= 0) {                         /* enquanto for invalido, repete */
    printf("Invalido! Digite n>0: ");
    scanf("%d", &n);
}

/* VALIDACAO de opcao de menu (char) */
char resp;
do {
    printf("Continuar? (S/N): ");
    fflush(stdin);
    scanf(" %c", &resp);
    resp = toupper(resp);                /* aceita s/S/n/N               */
} while (resp != 'S' && resp != 'N');    /* && = E logico                */
```

---

## 20. Tabela de formatadores scanf/printf

| Tipo        | scanf | printf | Observacao                                    |
|-------------|-------|--------|-----------------------------------------------|
| int         | %d    | %d     | precisa & no scanf                            |
| long        | %ld   | %ld    | inteiro longo                                 |
| float       | %f    | %f     | precisa & no scanf                            |
| double      | %lf   | %f     | %lf no scanf, %f no printf (CUIDADO!)         |
| char        | %c    | %c    | le 1 caractere; use " %c" para pular \n      |
| string      | %s    | %s    | sem & (ja e endereco); le ate espaco          |
| char[]      | %s    | %s    | mesmo que string                              |
| unsigned    | %u    | %u    | inteiro sem sinal                             |
| hex         | %x    | %x    | hexadecimal                                   |
| ponteiro    | %p    | %p    | endereco de memoria                           |
| %5d         | -     | %5d   | largura minima 5 (alinha direita)             |
| %-5d        | -     | %-5d  | alinha a esquerda                             |
| %.2f        | -     | %.2f  | 2 casas decimais                              |
| %05d        | -     | %05d  | preenche com zeros a esquerda                 |

---

## 21. Tabela de modos de fopen

| Modo | Texto/Binario | Acao                                    | Se arquivo existe        | Se NAO existe   |
|------|---------------|-----------------------------------------|--------------------------|-----------------|
| "r"  | texto         | leitura                                 | abre                     | ERRO (NULL)     |
| "w"  | texto         | escrita                                 | SOBRESCREVE (apaga)     | cria            |
| "a"  | texto         | append (adiciona ao fim)                | mantem, escreve no fim   | cria            |
| "r+" | texto         | leitura e escrita                       | abre                     | ERRO (NULL)     |
| "w+" | texto         | escrita e leitura                       | SOBRESCREVE              | cria            |
| "a+" | texto         | leitura e append                        | mantem, escreve no fim   | cria            |
| "rb" | binario       | leitura                                 | abre                     | ERRO (NULL)     |
| "wb" | binario       | escrita                                 | SOBRESCREVE              | cria            |
| "ab" | binario       | append                                  | mantem, escreve no fim   | cria            |

**Mnemonico:** r=read, w=write (apaga!), a=append (nao apaga).

---

## 22. Tabela de funcoes string.h

| Funcao            | Sintaxe                       | O que faz                                    |
|-------------------|-------------------------------|----------------------------------------------|
| strlen(s)         | size_t strlen(const char *s)  | tamanho da string (sem \0)                   |
| strcmp(s1,s2)     | int strcmp(const char*, ...)  | compara: 0=igual, >0=s1>s2, <0=s1<s2        |
| strcpy(dest,src)  | char* strcpy(char*, ...)      | copia src para dest                          |
| strcat(dest,src)  | char* strcat(char*, ...)      | concatena src ao fim de dest                 |
| strchr(s,c)       | char* strchr(const char*,int) | procura char c em s; retorna ptr ou NULL     |
| strstr(s,sub)     | char* strstr(const char*,...) | procura substring; retorna ptr ou NULL      |
| strtok(s,delim)   | char* strtok(char*, ...)      | divide s por delimitadores; NULL no fim      |
| sprintf(buf,fmt)  | int sprintf(char*, ...)      | formata e guarda em string (nao imprime)     |
| sscanf(s,fmt)     | int sscanf(const char*, ...)  | le de string em vez de teclado               |
| atoi(s)           | int atoi(const char*)         | converte string para int (0 se invalido)     |
| atof(s)           | double atof(const char*)      | converte string para double                  |
| strupr(s)         | char* strupr(char*)           | maiusculas (NAO padrao ANSI, Windows/MinGW)  |
| strlwr(s)         | char* strlwr(char*)           | minusculas (NAO padrao ANSI, Windows/MinGW)   |
| strrev(s)         | char* strrev(char*)           | inverte string (NAO padrao ANSI)              |

---

## 23. Tabela de funcoes ctype.h

| Funcao        | Retorna nao-zero (verdadeiro) se...        | Retorna 0 se...        |
|---------------|--------------------------------------------|------------------------|
| isdigit(c)    | c for digito 0-9                           | nao for digito         |
| isalpha(c)   | c for letra A-Z a-z                        | nao for letra          |
| isalnum(c)    | c for letra OU digito                      | nao for letra nem digito |
| isupper(c)    | c for letra maiuscula                      | nao for maiuscula      |
| islower(c)    | c for letra minuscula                      | nao for minuscula      |
| isspace(c)    | c for espaco, \t, \n, \r                   | nao for espaco         |
| toupper(c)    | (sempre) retorna c em maiusculo            | -                      |
| tolower(c)    | (sempre) retorna c em minusculo            | -                      |

**Atencao:** toupper/tolower retornam int; atribua a char ou use direto.

---

## 24. Erros comuns (lista rapida de 20)

```
ERROS_COMUNS - os 20 erros mais frequentes em prova de C:

1.  Esquecer & no scanf: scanf("%d", n);  -> ERRADO. Use scanf("%d", &n);
    Excecao: strings (char[]) nao levam & (ja sao endereco).

2.  Comparar strings com ==: if (s1 == s2) compara ENDERECOS, nao conteudo.
    Use strcmp(s1, s2) == 0.

3.  Usar %f para double no scanf: double exige %lf no scanf (no printf usa %f).

4.  Divisao inteira: 5/2 = 2 (trunca). Para 2.5 use (float)5/2 ou 5.0/2 ou 5/2.0.

5.  Esquecer break no switch: sem break, executa todos os cases seguintes (fall-through).

6.  Loop infinito: esquecer i++ no while/for. Sempre incremente o contador.

7.  Acesso fora do vetor: vetor de tamanho N tem indices 0 a N-1. v[N] e lixo/erro.

8.  fgets deixa \n na string: remova com s[strcspn(s,"\n")] = '\0'.

9.  \n pendente apos scanf de %d: proxima leitura de char/string pega o \n.
    Use fflush(stdin) ou " %c" (espaco antes de %c).

10. while(!feof(f)) com leitura antes do loop: repete o ultimo registro.
    Use while(fread(...)==1) ou while(fgets(...)!=NULL).

11. Nao verificar fopen==NULL: se o arquivo nao existe, da crash ao ler/escrever.

12. Esquecer fclose(f): dados podem nao ser gravados (ficam no buffer).

13. typedef struct sem alias: struct sem typedef exige "struct Nome" na declaracao.

14. Acessar struct por ponteiro com ponto: p.campo -> ERRADO. Use p->campo (seta).

15. Passar matriz sem numero de colunas: void f(int m[][]) -> ERRADO.
    A 1a dimensao pode ser vazia, as demais NAO: void f(int m[][10], int L).

16. Usar gets(): perigoso (sem limite de tamanho). Use fgets com sizeof.

17. Confundir = com ==: if (x = 5) atribui 5 a x e sempre e verdadeiro.
    Use == para comparar.

18. Modo "w" apaga o arquivo: fopen("dados.txt","w") SOBRESCREVE tudo.
    Para adicionar use "a" (append).

19. Esquecer prototipo de funcao: se a funcao fica depois do main e nao ha
    prototipo antes, o compilador assume int f() sem checar parametros.

20. Nao inicializar acumulador: soma=0, maior=v[0], cont=0 ANTES do loop.
    Esquecer isso usa lixo de memoria.
```

---

## DICAS RAPIDAS DE PROVA

```c
/* Ler N valores e achar media: cast antes de dividir! */
float media = (float)soma / n;           /* SEM cast: divisao inteira     */

/* Trocar sem variavel aux (truque, mas prefira com aux na prova) */
a = a + b; b = a - b; a = a - b;         /* so funciona com inteiros      */

/* Contar digitos de um inteiro */
int cont = 0, x = n;
if (x == 0) cont = 1;                    /* caso especial: 0 tem 1 digito */
while (x != 0) { x /= 10; cont++; }

/* Inverter numero (ex: 123 -> 321) */
int inv = 0, x = n;
while (x != 0) { inv = inv * 10 + x % 10; x /= 10; }

/* Verificar se ano e bissexto */
if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
    printf("bissexto\n");

/* Converter char digito para int: */
int digito = c - '0';                    /* '5' - '0' = 53 - 48 = 5      */

/* Converter int para string: */
char s[20]; sprintf(s, "%d", 123);       /* s = "123"                    */
```

---

**FIM DA COLA RAPIDA** - Use Ctrl+F com as palavras-chave do topo para encontrar
rapidamente o snippet necessario durante a prova.
