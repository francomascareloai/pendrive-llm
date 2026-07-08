#include <stdio.h>
#define MAX_NOMES 50
struct Aluno {
    char nome[MAX_NOMES];
    int matricula;
    float nota;
};

void listar(struct Aluno aluno[], int n);
int buscarPorMatrícula(struct Aluno aluno[], int n, int matr);
void salvarArquivo(struct Aluno aluno[], int n, char filename);
char carregarArquivo(struct Aluno aluno[], int n, char filename);

int main() {
    struct Aluno alunos[MAX_NOMES];
    int numAlunos = 0;
    
    while (1) {
        printf("Sistema de cadastro de alunos\n");
        printf("1 - Cadastrar\n2 - Listar\n3 - Buscar por matrícula\n4 - Salvar em arquivo binario\n5 - Carregar do arquivo\n0 - Sair\n");
        
        int opc;
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                cadastrar(alunos, numAlunos);
                break;
            case 2:
                listar(alunos, numAlunos);
                break;
            case 3:
                buscarPorMatrícula(alunos, numAlunos, 0);
                break;
            case 4:
                salvarArquivo(alunos, numAlunos);
                break;
            case 5:
                carregarArquivo(alunos, numAlunos);
                break;
            default:
                printf("Opção inválida!\n");
        }

        printf("\nTotal de alunos cadastrados: %d\n", numAlunos);
    }
    
    return 0;
}
