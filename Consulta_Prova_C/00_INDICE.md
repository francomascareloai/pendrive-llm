# 00 - INDICE MASTER - Consulta de Prova C (GA e GB)

Material de consulta para prova de C. Abra no VS Code, use Ctrl+P para
abrir arquivos e Ctrl+F para buscar dentro.

=========================================================
ESTRUTURA DA PASTA (6 arquivos .md + codigos)
=========================================================

DOCUMENTOS (abra pelo numero conforme o que precisar):

  00_INDICE.md          <- VOCE ESTA AQUI. Indice, busca por palavra-chave, checklist, cobertura.
  01_TEORIA.md          <- Prova teorica: assercoes com PORQUE, pegadinhas, casos extremos.
  02_CODIGO.md          <- Codigo: arquivos, struct, strings, leitura de dados, cola rapida.
  03_EXERCICIOS.md      <- Exercicios e desafios com gabarito (do simples ao complexo).
  04_FORMULAS.md        <- Todas as formulas e calculos matematicos.
  05_GA_COMPLETO.md     <- Material completo do GA (mega guia + guia unico).
  06_TOPICOS_EXTRAS.md  <- Topicos que faltavam: passagem por referencia, enum, union, bitwise, argc/argv, qsort, rand, matriz dinamica, static, precedencia, macros.

PASTAS:
  exemplos\   -> 35 programas .c completos (01 a 35)
  gabaritos\  -> 7 gabaritos .c (dengue, empresa esportiva, etc)

=========================================================
QUAL ARQUIVO ABRIR PARA CADA TIPO DE QUESTAO?
========================================================

- Questao teorica / multipla escolha / assercao com PORQUE -> 01_TEORIA.md
- Pegadinha (= vs ==, if(strcmp), recursao printf)        -> 01_TEORIA.md
- Escrever codigo de arquivo / struct / string             -> 02_CODIGO.md
- Nao lembra como ler dados (scanf/fgets/scanset)          -> 02_CODIGO.md
- Precisa de snippet pronto pra copiar                      -> 02_CODIGO.md (secao Cola Rapida)
- Quer ver exercicio resolvido parecido                    -> 03_EXERCICIOS.md
- Precisa de formula matematica                            -> 04_FORMULAS.md
- Questao do GA (vetor, matriz, calculo basico)            -> 05_GA_COMPLETO.md
- Passagem por referencia / ponteiro / troca              -> 06_TOPICOS_EXTRAS.md (Parte 1)
- Enum, union, bitwise, argc/argv                           -> 06_TOPICOS_EXTRAS.md (Parte 2)
- qsort, rand, matriz dinamica, macros, static             -> 06_TOPICOS_EXTRAS.md (Parte 3)

=========================================================
COMO USAR DURANTE A PROVA
========================================================

- Enunciado fala em "total": procure soma, acumulador.
- "quantidade": contador.
- "maior/menor": inicialize com primeiro e compare.
- "cadastro/aluno/produto": struct.
- "nome/frase/senha": string, fgets, strcmp.
- "arquivo/salvar/persistir": fopen, fwrite, fread.
- "alterar/excluir registro": temp + rename.
- "recursao/recursiva": caso base + caso recursivo.
- Prova teorica (assercoes PORQUE): 01_TEORIA.md.


#########################################################
# SECAO 1: PALAVRAS-CHAVE PARA CTRL+F
#########################################################

PALAVRAS-CHAVE PARA CTRL+F
==========================

Use Ctrl+F por estes termos. Lista dividida por topico.

Basico (GA):
mega guia
tudo comentado
scanf
printf
int
float
char
double
media
media ponderada
porcentagem
desconto
aumento
regra de tres
IMC
temperatura
salario
comissao
juros
juros simples
juros composto
area
perimetro
bhaskara
fatorial
fibonacci
PA
PG
primo
MDC
MMC
velocidade
distancia
consumo
triangulo
pitagoras
segundos
aprovado
reprovado
par
impar
modulo
%

Condicoes:
if
else
else if
switch
case
break
default
menu
opcao
do while

Repeticao:
for
while
contador
acumulador
soma
sentinela

Vetor:
vetor
array
ler vetor
imprimir vetor
soma vetor
media vetor
maior vetor
menor vetor
contar pares
busca
buscar
posicao
ordenar
ordenacao
crescente
troca
aux

Matriz:
matriz
linha
coluna
for dentro de for
soma matriz
media matriz
soma linha
soma coluna
maior matriz
menor matriz
diagonal principal
diagonal secundaria
matriz quadrada

Funcoes:
funcao
void
return
parametro
modularizacao
vetor parametro
matriz parametro
prototipo

String (GA):
string
char
fgets
strlen
strcmp
strcpy
strcat
strcspn
remover enter
comparar string
senha
frase
palavra

Struct (GA):
struct
typedef
registro
cadastro
aluno
pessoa
produto
vetor de struct
buscar struct
listar aprovados

Ponteiros:
ponteiro
endereco
alterar valor
&

Desafio dengue:
dengue
internados
hospitais
meses
matriz 6x5
total por mes
total por hospital
pacientes
repasse
1000
1200
1500
media dengue

=========================================================
ARQUIVOS EM C (GB) - ver ARQUIVOS_PROVA_GB.md
=========================================================
arquivo
FILE
fopen
fclose
modo abertura
r
w
a
r+
w+
a+
rb
wb
ab
binario
texto
fprintf
fscanf
fputc
fgetc
fputs
fgets
fwrite
fread
sizeof
feof
fseek
SEEK_SET
SEEK_CUR
SEEK_END
rewind
ftell
remove
rename
perror
NULL
acesso direto
byte offset
temp
temporario
CRUD
criar
cadastrar
listar
alterar
excluir
contar registros
struct em arquivo

=========================================================
STRUCT AVANCADO (GB) - ver STRUCT_AVANCADO_GB.md
=========================================================
struct avancado
typedef struct
alias
ponto
seta
->
ponteiro struct
struct parametro
struct retorno
struct aninhada
ordenar struct
ordenar por nome
strcmp struct
buscar struct
media struct
maior struct
mais velho
mais novo
classificacao

=========================================================
STRINGS AVANCADO (GB) - ver STRINGS_AVANCADO_GB.md
=========================================================
string avancado
\0
terminador
scanf %s
fgets
strcspn
limpar buffer
fflush stdin
strlen
strcpy
strncpy
strcat
strncat
strcmp
strncmp
strchr
strrchr
strstr
busca parcial
strtok
delimitador
CSV
sprintf
ctype
isalpha
isdigit
isalnum
isspace
toupper
tolower
isupper
islower
atoi
atof
atol
vogais
inverter
palindromo
contar palavras
validar
validar CPF
validar email
char vs string

=========================================================
COMBINACOES (GB) - ver COMBINACOES_GB.md
=========================================================
struct arquivo
struct arquivo binario
struct arquivo texto
fwrite struct
fread struct
strtok CSV
struct string
busca parcial nome
menu CRUD
temp rename
alterar registro
excluir registro
matriz vetor funcoes
recursao
caso base
caso recursivo
fatorial recursivo
fibonacci recursivo
imprimir recursivo
printf ordem
decrescente
crescente
pilha de chamadas

=========================================================
TEORIA / PROVA OBJETIVA (GB) - ver TEORIA_PROVA_GB.md
=========================================================
teoria
multipla escolha
assercao
PORQUE
proposicao
justifica
analise codigo
prever saida
pegadinha
recursao
caso base
printf antes
printf depois
strcmp retorno
strcmp == 0
if strcmp
struct vantagem
agrupar dados
ponteiro
& vs *
->
= vs ==
esquecer &
comparar string ==
feof errado
estouro buffer
esquecer break
divisao inteira


#########################################################
# SECAO 2: CHECKLIST ANTES DE ENTREGAR
#########################################################

CHECKLIST RAPIDO DE C
=====================

== BASICO (GA) ==

1. Vetor:
   int v[5];
   indices: 0, 1, 2, 3, 4

2. Matriz:
   int m[6][5];
   m[linha][coluna]

3. Soma:
   int soma = 0;
   soma += valor;

4. Media:
   media = soma / 5.0;

5. scanf:
   scanf("%d", &x);
   scanf("%f", &nota);
   scanf("%49s", nome);

6. String:
   #include <string.h>
   strcmp(a, b) == 0
   strcpy(destino, origem)
   strlen(texto)

7. Struct:
   typedef struct {
       char nome[50];
       int idade;
   } Pessoa;

8. Funcao com vetor:
   void f(int v[], int n)

9. Funcao com matriz:
   void f(int m[LINHAS][COLUNAS])

10. Erros comuns:
    = nao e ==
    esquecer ;
    esquecer &
    esquecer break
    acessar fora do limite

== ARQUIVOS (GB) ==

11. Abrir arquivo SEMPRE testar NULL:
    FILE *arq = fopen("x.dat", "rb");
    if (arq == NULL) { perror("erro"); return 1; }

12. Modos de abertura:
    "r"  ler texto        "rb" ler binario
    "w"  criar/truncar    "wb" criar binario
    "a"  append texto     "ab" append binario
    "r+" ler+escrever     "r+b" ler+escr binario

13. Escrever binario (struct):
    fwrite(&alu, sizeof(Aluno), 1, arq);

14. Ler binario CORRETO (nao usar feof!):
    while (fread(&alu, sizeof(Aluno), 1, arq) == 1) { ... }
    // BUG: while(!feof) com fread antes do loop le o ultimo 2x

15. Acesso direto (pular para registro N):
    fseek(arq, N * sizeof(Aluno), SEEK_SET);

16. Contar registros:
    fseek(arq, 0, SEEK_END);
    long tam = ftell(arq);
    int n = tam / sizeof(Aluno);

17. Alterar/Excluir registro: padrao temp+rename
    - abre original em "rb", temp em "wb"
    - copia todos, trocando/pulando o registro alvo
    - fclose ambos, remove original, rename temp

18. Fechar TODO arquivo aberto: fclose(arq);

== STRUCT AVANCADO (GB) ==

19. typedef struct CORRETO (C puro):
    typedef struct { int cod; char nome[50]; } Aluno;
    // ERRADO (nao compila em C): typedef struct aluno { ... };

20. Ponteiro para struct usa SETA (->):
    void alterar(Aluno *a) { a->media = 7.0; }  // seta
    Aluno a; a.media = 7.0;                       // ponto

21. Struct aninhada:
    typedef struct { char rua[50]; } Endereco;
    typedef struct { Endereco end; } Pessoa;
    p.end.rua

22. Ordenar struct por nome (usa strcmp):
    if (strcmp(v[j].nome, v[j+1].nome) > 0) { troca; }

== STRINGS AVANCADO (GB) ==

23. Ler frase com espaco:
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';  // tira o enter

24. strcmp - PEGADINHA DE PROVA:
    if (strcmp(a, b) == 0)  // IGUAIS (correto)
    if (strcmp(a, b))       // DIFERENTES (!=0 = verdadeiro)

25. Busca parcial (nome contem "sil"):
    if (strstr(nome, "sil") != NULL) { achou; }

26. Separar CSV com strtok:
    char *p = strtok(linha, ",");
    while (p != NULL) { ... p = strtok(NULL, ","); }
    atoi(p) / atof(p) para converter

27. ctype.h: toupper(c), tolower(c), isdigit(c), isalpha(c)

== RECURSAO (GB - QUESTAO 1 CONFIRMADA) ==

28. Toda recursao tem CASO BASE e CASO RECURSIVO:
    int fat(int n) {
        if (n <= 1) return 1;        // caso base
        return n * fat(n - 1);      // caso recursivo
    }

29. PEGADINHA da ordem do printf:
    void rec(int n) {
        if (n == 0) return;
        printf("%d ", n);   // ANTES -> decrescente (5 4 3 2 1)
        rec(n - 1);
    }
    // Se o printf for DEPOIS da chamada -> crescente (1 2 3 4 5)

== ANTES DE ENTREGAR ==

30. #include corretos? (stdio, string, ctype, stdlib, math)
31. int main(void) com return 0?
32. Toda variavel declarada?
33. Todo scanf com & (exceto string)?
34. Soma/contador inicializados em 0?
35. Vetor/matriz dentro do limite?
36. Funcoes declaradas antes do main ou com prototipo?
37. Strings comparadas com strcmp (nao ==)?
38. Arquivo: testou NULL? fechou todos?
39. Leitura de arquivo: fread==1 no while (nao feof)?
40. O codigo compila? (gcc -Wall -Wextra sem erros)
41. A saida responde exatamente o que o enunciado pediu?


#########################################################
# SECAO 3: COBERTURA E LACUNAS
#########################################################

COBERTURA E LACUNAS - PROVA DE C
=================================

Resumo direto:
- Para o GA: vetor, matriz, struct, string, funcoes, lacos, if/switch e calculos basicos - bem coberto.
- Para o GB (arquivos, struct avancado, strings avancado, recursao, teoria/assercoes) - AGORA COBERTO com 5 documentos novos + 14 codigos novos.
- O segredo e reconhecer o padrao: ler dados, acumular, contar, calcular media, comparar, buscar, ordenar, persistir em arquivo, recursao.
- Se a consulta for permitida, use Ctrl+F por palavra-chave (ver PALAVRAS_CHAVE_BUSCA.md).

=========================================================
NOVIDADES DO GB (adicionadas nesta revisao)
=========================================================

5 documentos novos:
- ARQUIVOS_PROVA_GB.md: fopen/fclose, todos os modos, fwrite/fread, fseek/SEEK_SET/CUR/END, CRUD texto e binario, padrao temp+rename, contar registros, bug do feof.
- STRUCT_AVANCADO_GB.md: typedef correto, ponteiro para struct (->), struct aninhada, ordenar por string, struct como parametro/retorno.
- STRINGS_AVANCADO_GB.md: string.h completa, ctype.h, strcmp (pegadinha), strtok/CSV, strstr/busca parcial, sprintf, atoi/atof, validacao, manipulacao manual.
- COMBINACOES_GB.md: struct+arquivo, struct+string, string+arquivo, menu+CRUD, recursao (pegadinha da ordem do printf).
- TEORIA_PROVA_GB.md: multipla escolha, analise de codigo, assercoes com PORQUE, recursao, strcmp, struct vantagem, ponteiros.

2 documentos adicionais (enriquecimento final):
- LEITURA_DADOS_PROVA_GB.md: TODAS as formas de ler dados (scanf, fgets, sscanf, fscanf, getchar, scanset %[^;], validacao, limpar buffer), das mais simples as mais complexas.
- BANCO_EXERCICIOS_GB.md: banco com 34 exercicios (enunciado + gabarito comentado) em 12 categorias, do simples ao complexo. TODOS os codigos compilam com gcc -Wall -Wextra.

FORMULAS_CALCULOS_PROVA_C.md enriquecido: agora vai do topico 1 ao 42 (adicionados conversao de base, media harmonica/geometrica, variancia/desvio, IR/INSS por faixa, regra de tres composta, volumes, media movel, raiz sem math.h, conversao de angulos).

4 documentos adicionais (enriquecimento do pendrive):
- PEGADINHAS_PROVA_GB.md: 46 pegadinhas de C (= vs ==, if(strcmp), recursao printf ordem, feof, divisao inteira, break, etc) com codigo errado/certo lado a lado + 6 questoes de analise com gabarito + checklist anti-pegadinhas.
- FORMULAS_EXTRAS_GB.md: 30 formulas novas (folha de pagamento, tabela price, depreciacao, fisica - MRU/MRUV/energia, combinatoria, estatistica completa, geometria, datas, numeros especiais - Armstrong/perfeito/capicua, crivo de Eratostenes).
- CASOS_EXTREMOS_ERROS_GB.md: 25 topicos de tratamento de erros e casos limite, cada um com codigo FRAGIL vs ROBUSTO lado a lado (validacao, divisao zero, NULL, overflow, imprecisao float, assert, perror, codigos de retorno).
- COLA_RAPIDA_GB.md: pagina densa de snippets prontos para copiar (esqueletos, leitura, vetores, matrizes, strings, struct, arquivos, recursao, tabelas de formatadores/modos/funcoes).

13 codigos novos adicionados no pendrive (TODOS compilam com gcc -Wall -Wextra):
- exemplos/26_pegadinhas_compiladas.c (demonstra pegadinhas na pratica)
- exemplos/27_validacao_entrada_robusta.c (validacao de entrada)
- exemplos/28_calculos_estatisticos.c (media/mediana/moda/variancia/desvio)
- exemplos/29_manipulacao_string_avancada.c (strings avancada com menu)
- exemplos/30_ordenacao_busca_avancada.c (bubble/selection/insertion + busca binaria)
- exemplos/31_recursao_varios.c (fatorial/fibonacci/somatorio/potencia/hanoi/MDC)
- exemplos/32_ponteiro_avancado.c (aritmetica de ponteiro, ponteiro funcao)
- exemplos/33_alocacao_dinamica.c (malloc/calloc/realloc/free)
- exemplos/34_arquivo_texto_processamento.c (processar arquivo texto)
- exemplos/35_struct_aninhada_data.c (struct Data dentro de Pessoa)
- gabaritos/gabarito_sistema_biblioteca.c (CRUD livros em arquivo binario)
- gabaritos/gabarito_controle_estoque.c (CRUD produtos em arquivo binario)
- gabaritos/gabarito_agenda_contatos.c (CRUD contatos em arquivo texto)

14 codigos novos (TODOS compilam com gcc -Wall -Wextra, sem erros nem warnings):
Exemplos do professor corrigidos e comentados:
- exemplos/15_arquivo_binario_nomes.c (fwrite/fread, menu getche)
- exemplos/16_arquivo_fseek_posicionamento.c (fseek SEEK_SET/CUR/END, rewind)
- exemplos/17_arquivo_acesso_direto.c (fseek com sizeof(int)*5)
- exemplos/18_crud_texto_strtok.c (funcionario.txt, strtok+atoi/atof, temp+rename)
- exemplos/19_crud_binario_struct.c (funcionario.dat, fwrite/fread de struct)
- exemplos/20_struct_vetor_5_funcionarios.c (leitura/imprimir/media)
- exemplos/21_struct_vetor_n_variavel.c (n variavel, MAX 10)
- exemplos/22_struct_produto_media.c (PRODUTO, media pvenda)

Gabaritos de enunciado de prova:
- gabaritos/gabarito_empresa_esportiva.c (filiais+funcionarios, 8 validacoes, menu modular)
- gabaritos/gabarito_mais_velho_novo.c (struct aluno, mais velho/novo)
- gabaritos/gabarito_media_ponderada_classificacao.c (media ponderada, classificacao A-R)

Exemplos de lacunas preenchidas:
- exemplos/23_ponteiro_struct.c (operador ->, struct por ponteiro, malloc)
- exemplos/24_strings_manipulacao.c (string.h, ctype.h, strtok, strstr, atoi/atof)
- exemplos/25_crud_binario_completo.c (CRUD completo: criar/cad/listar/buscar/alterar/excluir/contar)

=========================================================
COBERTURA POR TOPICO
=========================================================

Coberto em matematica (GA):
- operacoes basicas, resto, porcentagem, desconto, aumento
- media simples e ponderada, regra de tres, IMC, temperatura
- salario, comissao, bonus e faixas, juros simples e composto
- tempo, area, perimetro, triangulo, Pitagoras, potencia, raiz, Bhaskara
- fatorial, Fibonacci, PA, PG, MDC, MMC, primo
- minimo, maximo, soma, media em vetor, porcentagem de ocorrencias
- matriz: soma total, media, soma por linha/coluna, diagonais

Coberto em programacao (GA):
- entrada/saida, if/else/switch, for/while/do while, menu
- vetor: ler, imprimir, somar, buscar, ordenar, maior, menor, contar
- matriz: ler, imprimir, somar, diagonais, transposta
- funcoes: retorno, void, parametros, vetor/matriz como parametro
- string: scanf, fgets, strlen, strcmp, strcpy, strcat
- struct: cadastro, vetor de struct, buscar/listar
- ponteiro basico: alterar valor por endereco

Coberto em arquivos (GB):
- fopen/fclose, todos os modos (r/w/a/r+/w+/a+ e binarios)
- fprintf/fscanf, fgets/fputs, fgetc/fputc
- fwrite/fread de int e de struct
- fseek (SEEK_SET/CUR/END), ftell, rewind
- acesso direto com sizeof
- CRUD em texto (strtok) e em binario (struct)
- padrao temp+rename para alterar/excluir
- contar registros
- bug do while(!feof) e a forma correta (fread==1)

Coberto em struct avancado (GB):
- typedef correto (alias)
- ponteiro para struct, operador ->
- struct aninhada
- struct como parametro e retorno
- ordenar por campo numerico e por string (strcmp)
- buscar por nome e por codigo
- achar maior/menor por campo

Coberto em strings avancado (GB):
- string.h: strlen, strcpy, strncpy, strcat, strncat, strcmp, strncmp, strchr, strrchr, strstr, strtok, sprintf
- ctype.h: isalpha, isdigit, isalnum, isspace, toupper, tolower, isupper, islower
- strcmp e a pegadinha if(strcmp) vs if(strcmp==0)
- busca parcial com strstr
- strtok para CSV com atoi/atof
- manipulacao manual: vogais, inverter, palindromo, contar palavras
- validacao: CPF, e-mail, so digitos

Coberto em recursao (GB):
- caso base e caso recursivo
- fatorial recursivo, fibonacci recursivo
- imprimir N ate 1 recursivo
- PEGADINHA da ordem do printf (antes=decrescente, depois=crescente)

Coberto em teoria (GB):
- multipla escolha, analise de codigo, assercoes com PORQUE
- recursao (prever saida)
- strcmp (retorno e pegadinha)
- struct (vantagem de usar)
- ponteiros (& vs *, ->)
- erros comuns para identificar (= vs ==, esquecer &, comparar string com ==, feof, estouro de buffer, esquecer break, divisao inteira)

=========================================================
O QUE AINDA PODE APARECER (fuga do foco)
=========================================================
- alocacao dinamica: malloc, calloc, realloc, free (coberto no 23_ponteiro_struct.c basico)
- aritmetica de ponteiros avancada
- enum, union
- multiplicacao de matrizes
- selection sort, insertion sort (so bubble sort esta detalhado)
- busca binaria
- arquivos com fsetpos/fgetpos

Minha leitura:
- O material esta forte para o GA e agora tambem para o GB.
- O ponto mais provavel de faltar nao e "formula", e sim variacao de enunciado. Por isso o banco por palavras-chave.
- Quando bater um enunciado estranho, procure pelo padrao: contador, acumulador, vetor, matriz, struct, string, funcao, faixa, media, busca, ordenacao, arquivo, recursao.
- Para a parte teorica, treine o formato de assercoes com PORQUE (ver TEORIA_PROVA_GB.md).
