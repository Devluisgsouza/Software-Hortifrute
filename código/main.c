/// Software para gerenciamento de um Hortifrute

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_PRODUTOS 100
#define ARQUIVO "produtos.txt"
#define ADMIN_EMAIL "admin@example.com" // E-mail padrao do administrador
#define ADMIN_SENHA "senha123" // Senha padrao do administrador

typedef struct {
    char nome[50];
    float preco;
    int estoque;
} Produto;

Produto produtos[MAX_PRODUTOS];
int totalProdutos = 0;

// Funcao para carregar produtos do arquivo
void carregarProdutos() {
    FILE *file = fopen(ARQUIVO, "r");
    if (file) {
        while (fscanf(file, "%49s %f %d", produtos[totalProdutos].nome, &produtos[totalProdutos].preco, &produtos[totalProdutos].estoque) == 3) {
            totalProdutos++;
        }
        fclose(file);
    }
}

// Funcao para salvar produtos no arquivo
void salvarProdutos() {
    FILE *file = fopen(ARQUIVO, "w");
    for (int i = 0; i < totalProdutos; i++) {
        fprintf(file, "%s %.2f %d\n", produtos[i].nome, produtos[i].preco, produtos[i].estoque);
    }
    fclose(file);
}
// Funçao para Cadastrar Produtos
void cadastrarProduto() {
    char nome[50];
    float preco;
    int quantidade;

    printf("Digite o nome do produto: ");
    scanf("%49s", nome);

    printf("Digite o preço do produto: ");
    while (scanf("%f", &preco) != 1 || preco < 0) {
        while (getchar() != '\n'); // Limpar o buffer
    }

    printf("Digite a quantidade em estoque: ");
    while (scanf("%d", &quantidade) != 1 || quantidade < 0) {
        while (getchar() != '\n'); // Limpar o buffer
    }

// Verificar se o produto ja esta cadastrado
    for (int i = 0; i < totalProdutos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            produtos[i].estoque += quantidade; // Adiciona ao estoque existente
            printf("Produto já cadastrado. Estoque atualizado: %d\n", produtos[i].estoque);
            salvarProdutos(); // Salvar apos atualizacao
            return;
        }
    }

// Se nao encontrado, cadastrar novo produto
    Produto novoProduto;
    strcpy(novoProduto.nome, nome);
    novoProduto.preco = preco;
    novoProduto.estoque = quantidade;

    produtos[totalProdutos] = novoProduto;
    totalProdutos++;
    salvarProdutos(); // Salvar apos cadastro
    printf("Produto cadastrado com sucesso!\n");
}
// Função Exibir Estoque
void exibirEstoque() {
    printf("\n--- Estoque ---\n");
    for (int i = 0; i < totalProdutos; i++) {
        printf("Produto: %s | Preço: %.2f | Estoque: %d\n", produtos[i].nome, produtos[i].preco, produtos[i].estoque);
    }
    printf("----------------\n");
}
// Função Vender Produto
void venderproduto() {
    char nome[50];
    int quantidade;

    printf("Digite o nome do produto que deseja comprar: ");
    scanf("%49s", nome);
    printf("Digite a quantidade: ");
    while (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
        printf("Quantidade invalida. Tente novamente: ");
        while (getchar() != '\n'); // Limpar o buffer
    }

    for (int i = 0; i < totalProdutos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            if (produtos[i].estoque >= quantidade) {
                produtos[i].estoque -= quantidade;
                salvarProdutos(); // Salvar apos compra
                system("cls");
                printf("Compra realizada com sucesso! Produto: %s | Quantidade: %d\n | Preço: %.2f", produtos[i].nome, quantidade, produtos[i].preco*quantidade);
                return;
            } else {
                printf("Estoque insuficiente para o produto: %s. Estoque atual: %d\n", produtos[i].nome, produtos[i].estoque);
                return;
            }
        }
    }
    printf("Produto não encontrado!\n");
}
//Funçao para remover produto
void removerProduto() {
    char nome[50];
    printf("Digite o nome do produto que deseja remover: ");
    scanf("%49s", nome);

    for (int i = 0; i < totalProdutos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            for (int j = i; j < totalProdutos - 1; j++) {
                produtos[j] = produtos[j + 1];
            }
            totalProdutos--;
            salvarProdutos(); // Salvar apos remocao
            printf("Produto removido com sucesso!\n");
            return;
        }
    }
    printf("Produto não encontrado!\n");
}

int loginAdministrador() {
    char email[50];
    char senha[50];

    printf("Digite seu e-mail: ");
    scanf("%49s", email);
    printf("Digite sua senha: ");
    scanf("%49s", senha);

    return (strcmp(email, ADMIN_EMAIL) == 0 && strcmp(senha, ADMIN_SENHA) == 0);
}

int main() {
    setlocale(LC_ALL,"");
    carregarProdutos(); // Carregar produtos ao iniciar

    // Mensagem de boas-vindas
    printf("Bem vindo ao Hortifruti! \nVocê é gerente?\n ");
    printf("[1] - sim  [2] - não: ");
    int resposta;
    scanf("%d", &resposta);

    int adminLoggedIn = 0;

    if (resposta == 1) {
        printf("Login do Administrador\n");
        adminLoggedIn = loginAdministrador();
        if (!adminLoggedIn) {
            printf("Login falhou. Você nao terá acesso total.\n");
        }
    }

    int opcao;
    do {
        printf("\n--- Menu ---\n");
        if (adminLoggedIn) {
            printf("1. Cadastrar Produto\n");
            printf("2. Exibir Estoque\n");
            printf("3. Efetuar venda\n");
            printf("4. Remover Produto\n");
            printf("5. Sair\n");
        } else {
            printf("1. Efetuar venda\n");
            printf("2. Exibir Estoque\n");
            printf("3. Sair\n");
        }

        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (adminLoggedIn) {
                    cadastrarProduto();
                } else{
                    venderproduto();
                    opcao = 5;
                }
                break;
            case 3:
                if (adminLoggedIn){
                    venderproduto();
                    opcao = 5;
                }else {
                    printf("Volte sempre!\n");
                    opcao = 5; // Forcar a saida
                break;
            case 2:
                exibirEstoque();
                break;
            case 4:
                if (adminLoggedIn) {
                    removerProduto();
                }
                }
                break;
            case 5:
                if (adminLoggedIn) {
                    printf("Volte sempre!\n");
                    opcao = 5; // Forcar a saida
                }
                break;
            default:
                printf("Opção invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 5);

return(0);
}
