#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Definicao da estrutura do contato
struct Contato {
    char nome[50];
    char numero[15];
    char email[50];
    struct Contato* proximo;
};

// Funcao de hash com dobramento
int hash(const char* chave) {
    unsigned int valor = 0;
    int i;
    int tamanho = strlen(chave);

    for (i = 0; i < tamanho; i++) {
        valor += chave[i];
    }

    return valor % 32;
}

// Inicializa a tabela hash
struct Contato* tabela[32];

// Funcao para adicionar um contato a tabela hash

void adicionarContato(const char* nome, const char* numero, const char* email) {
    struct Contato* novoContato = (struct Contato*)malloc(sizeof(struct Contato));
    if (novoContato == NULL) {
        printf("Erro ao alocar memoria para o novo contato.\n");
        return;
    }

    strcpy(novoContato->nome, nome);
    strcpy(novoContato->numero, numero);
    strcpy(novoContato->email, email);
    novoContato->proximo = NULL;

    int indice = hash(nome);

    if (tabela[indice] == NULL) {
        tabela[indice] = novoContato;
    } else {
        // Tratamento de colisoes por encadeamento
        struct Contato* atual = tabela[indice];
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoContato;
    }
}

// Funcao para listar todos os contatos na tabela hash
void listarContatos() {
    printf("Lista de Contatos:\n");
    for (int i = 0; i < 32; i++) {
        struct Contato* atual = tabela[i];
        while (atual != NULL) {
            printf("Nome: %s\n", atual->nome);
            printf("Numero: %s\n", atual->numero);
            printf("Email: %s\n", atual->email);
            printf("\n");
            atual = atual->proximo;
        }
    }
}

// Funcao para buscar um contato na tabela hash
void buscarContato(const char* nome) {
    int indice = hash(nome);
    struct Contato* atual = tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            printf("Nome: %s\n", atual->nome);
            printf("Numero: %s\n", atual->numero);
            printf("Email: %s\n", atual->email);
            return;
        }
        atual = atual->proximo;
    }

    printf("Nenhum contato encontrado com o nome '%s'.\n", nome);
}

// Funcao para exportar contatos para um arquivo
void exportarContatos(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para exportacao.\n");
        return;
    }

    for (int i = 0; i < 32; i++) {
        struct Contato* atual = tabela[i];
        while (atual != NULL) {
            fprintf(arquivo, "Nome: %s\n", atual->nome);
            fprintf(arquivo, "Numero: %s\n", atual->numero);
            fprintf(arquivo, "Email: %s\n", atual->email);
            fprintf(arquivo, "\n");
            atual = atual->proximo;
        }
    }

    fclose(arquivo);
    printf("Contatos exportados com sucesso para '%s'.\n", nomeArquivo);
}

// Funcao para remover um contato da tabela hash
void removerContato(const char* nome) {
    int indice = hash(nome);

    struct Contato* atual = tabela[indice];
    struct Contato* anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            if (anterior == NULL) {
                tabela[indice] = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("Contato '%s' removido com sucesso.\n", nome);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    printf("Nenhum contato encontrado com o nome '%s'. Nada foi removido.\n", nome);
}

int main() {
    // Inicializa a tabela hash
    for (int i = 0; i < 32; i++) {
        tabela[i] = NULL;
    }

    int escolha;
    char nome[50], numero[15], email[50], nomeArquivo[50];

    while (1) {
        printf("\nEscolha a operacao:\n");
        printf("1. Adicionar Contato\n");
        printf("2. Listar Contatos\n");
        printf("3. Buscar Contato\n");
        printf("4. Exportar Contatos\n");
        printf("5. Remover Contato\n");
        printf("6. Sair\n");
        printf("Digite o numero da operacao desejada: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                printf("Digite o nome: ");
                scanf("%s", nome);
                printf("Digite o numero: ");
                scanf("%s", numero);
                printf("Digite o email: ");
                scanf("%s", email);
                adicionarContato(nome, numero, email);
                break;
            case 2:
                listarContatos();
                break;
            case 3:
                printf("Digite o nome a ser buscado: ");
                scanf("%s", nome);
                buscarContato(nome);
                break;
            case 4:
                printf("Digite o nome do arquivo para exportacao: ");
                scanf("%s", nomeArquivo);
                exportarContatos(nomeArquivo);
                break;
            case 5:
                printf("Digite o nome a ser removido: ");
                scanf("%s", nome);
                removerContato(nome);
                break;
            case 6:
                exit(0);
            default:
                printf("Escolha invalida.\n");
        }
    }

    return 0;
}
