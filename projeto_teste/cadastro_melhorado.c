#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nome[50];
    int matricula;
    float nota;
} Aluno;

Aluno alunos[50];
int count = 0;

void cadastrar(Aluno *aluno) {
    printf("Inserir nome: ");
    strcpy(aluno->nome, fgets(aluno->nome, sizeof(aluno->nome), stdin));
    
    printf("Inserir matricula: ");
    scanf("%d", &aluno->matricula);
    
    printf("Inserir nota: ");
    scanf("%f", &aluno->nota);
}

void listar() {
    if (count == 0) {
        printf("A lista está vazia.\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i+1, alunos[i].nome);
    }
}

void buscar_por_matricula(int matricula) {
    int pos = -1;
    for (int i = 0; i < count; i++) {
        if (alunos[i].matricula == matricula) {
            pos = i;
            break;
        }
    }
    
    if (pos != -1) {
        printf("Matrícula encontrada: %d. Nota: %.2f\n", pos+1, alunos[pos].nota);
    } else {
        printf("Matrícula inválida.\n");
    }
}

void salvar_binario(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Opção de arquivo inválida");
        return;
    }
    
    // Write binary data to file
    for (int i = 0; i < count; i++) {
        fwrite(&alunos[i].val, sizeof(int), 1, file);
    }
    
    fclose(file);
}

void carregar() {
    const char *filename = "student.dat";
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Opção de arquivo inválida");
        return;
    }
    
    // Read binary data from file
    for (int i = 0; i < count; i++) {
        fread(&alunos[i].val, sizeof(int), 1, file);
    }
    
    fclose(file);
}

void main_menu() {
    int choice;
    
    printf("\n--- Sistema de Cadastros ---\n");
    printf("1. Cadastrar\n");
    printf("2. Listar\n");
    printf("3. Buscar por Matrícula\n");
    printf("4. Salvar em Arquivo Binario\n");
    printf("5. Carregar do arquivo\n");
    printf("0. Sair\n");
    
    while (1) {
        printf("\n--- Menu ---\n");
        printf("Escolha a opção: ");
        
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("Opção inválida. Use 1-0.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                cadastrar(&alunos[count]);
                break;
            case 2:
                listar();
                break;
            case 3:
                buscar_por_matricula(matricula);
                break;
            case 4:
                salvar_binario("student.dat");
                break;
            case 5:
                carregar();
                break;
            default:
                printf("Opção inválida. Use 1-0.\n");
        }
    }
}

int main() {
    main_menu();
    return 0;
}
