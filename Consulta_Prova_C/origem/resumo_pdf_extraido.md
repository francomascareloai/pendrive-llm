                                        TRABALHO DE REVISÃO DO GA

Segue o desafio para aplicar os conhecimentos do GA.

DESAFIO - aplicar matrizes e vetores:

                    Implemente uma solução para o Desafio Proposto em Linguagem C.

Uma cidade da Serra Gaúcha está enfrentando um problema muito sério com infecções e internações por
Dengue , e a cidade necessita gerenciar o número de pessoas internadas por Dengue nos últimos 6 meses
nos 5 principais hospitais da cidade. Estas informações serão importantes para a tomada de decisão do
grupo de contingência sobre o aumento ou diminuição de leitos da cidade.



Exemplo de Cadastro da Matriz:




Desenvolva um programa com as seguintes funcionalidades, tudo modularizado:



a)    Defina a matriz 6 x 5 no main       para armazenar os dados solicitados para o problema e realize a
leitura da matriz em um novo módulo.



b)    Defina um vetor Internados de 6 posições de inteiro no main, inicialize o vetor com zero. Desenvolva
um novo módulo que recebe o vetor, a matriz e as variáveis necessárias para calcular e armazenar neste
vetor os totais somados de todos os internados de todos os hospitais em cada mês.




Posição Internados[0] -> armazena a soma de todos os internados do mês 1

Posição Internados[1] -> armazena a soma de todos os internados do mês 2

....... e assim todos os meses
c) Defina um vetor Pacientes de 5 posições de inteiros no main, inicialize o vetor com zero. Desenvolva
um novo módulo que recebe o vetor, a matriz e as variáveis necessárias para calcular e armazenar neste
vetor Paciente [5] os totais somados de todos os internados de todos os meses por hospital.

Exemplo:




Posição Pacientes[0] -> armazena a soma de todos os internados do Hospital 1

Posição Pacientes[1] -> armazena a soma de todos os internados do Hospital 2

....... e assim todos os hospitais



Calcule também nesse módulo o valor que os hospitais irão receber, conforme abaixo:

O Setor Público vai auxiliar os hospitais em R$, dependendo da quantidade de pacientes de Dengue que o
hospital recebeu nos seis meses.



Para até 50 internados – receberá 1000,00 por cada pacientes

Acima de 50 até 100 internados – receberá 1200,00 por cada paciente

Acima de 100 internados – receberá 1500,00 por cada paciente



Calcule e Imprima os valores em R$ que cada hospital vai receber durante os 6 meses.



Deverá gerar o seguinte relatório com os totais em R$ para cada hospital:

Hospital 0 – 321000,00

Hospital 1 – 111600,00

Hospital 2 – 235500,00

Hospital 3 – 48000,00

Hospital 4 – 26000,00



d) Desenvolva um novo módulo , envie os parâmetros necessários, calcule e imprima a média de
pacientes de toda a matriz. (Somar todos os elementos e dividir pelo total e elementos) . A média calculada
deve ser retornada para o main e impressa no main.
