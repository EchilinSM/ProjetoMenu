#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 100
#define MAX_COMPRAS 100

#ifdef _WIN32
    #define limparTela() system("cls")
    #define pausar() system("pause")
#else
    #define limparTela() system("clear")
    #define pausar() system("read -p 'Pressione Enter para continuar...'")
#endif

struct Produto {
    char nome[50];
    float preco;
    int quantidade;
};

struct Compra {
    char nomeProduto[50];
    float precoUnitario;
    int quantidadeComprada;
};

struct Produto produtos[MAX_PRODUTOS];
int totalProdutos = 0;

struct Compra historico[MAX_COMPRAS];
int totalCompras = 0;

float saldoUsuario = 0.0;

void adicionarProduto() {
    limparTela();


    if (totalProdutos >= MAX_PRODUTOS) {
        printf("Erro: Limite de produtos atingido.\n");
        return;
    }


    printf("Nome do produto: ");
    getchar(); // Limpar buffer
    fgets(produtos[totalProdutos].nome, sizeof(produtos[totalProdutos].nome), stdin);
    produtos[totalProdutos].nome[strcspn(produtos[totalProdutos].nome, "\n")] = '\0';


    printf("Preço do produto: ");
    scanf("%f", &produtos[totalProdutos].preco);


    printf("Quantidade do produto: ");
    scanf("%d", &produtos[totalProdutos].quantidade);


    totalProdutos++;
    printf("Produto adicionado com sucesso!\n");
    pausar();
}

void adicionarCompraAoHistorico(char nome[], float preco, int quantidade){
    if(totalCompras >= MAX_COMPRAS){
        printf("Historico de compras cheio!\n");
        return;
    }

    strcpy(historico[totalCompras].nomeProduto, nome);
    historico[totalCompras].precoUnitario = preco;
    historico[totalCompras].quantidadeComprada = quantidade;
    totalCompras++;
}

void comprarProduto(){
    limparTela();

    if(totalProdutos == 0){
        printf("Nenhum produto disponivel para compra.\n");
        pausar();
        return;
    }

    if(saldoUsuario <= 0){
        printf("Seu saldo atual e R$ %.2f. Adicione saldo primeiro.\n", saldoUsuario);
        pausar();
        return; 
    }

    char continuar = 's';
    while((continuar == 's' || continuar == 'S')&& saldoUsuario > 0){
        limparTela();
        printf("Saldo atual: R$ %.2f", saldoUsuario);
        printf("\nProdutos disponiveis: \n\n");

        for(int i = 0; i < totalProdutos; i++){
            printf("%d. %s - R$ %.2f - Quantidade: %d\n", i + 1, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
        }

        int escolha;
        printf("\nDigite o numero do produto que deseja comprar: ");
        scanf("%d", &escolha);
        escolha--;

        if(escolha < 0 || escolha >= totalProdutos){
            printf("Opcao invalida.\n");
            pausar();
            continue;
        }

        if(produtos[escolha].quantidade == 0){
        printf("Produto esgotado. \n");
        }else{
            int qtdDesejada;

            printf("Digite a quantidade de produtos que deseja comprar: ");
            scanf("%d", &qtdDesejada);

            if(qtdDesejada <= 0){
                printf("Quantidade invalida.\n");
            } else if(qtdDesejada > produtos[escolha].quantidade){
                printf("Quantidade indisponivel em estoque.\n");
            }else{
                float totalCompra = produtos[escolha].preco * qtdDesejada;

                if(saldoUsuario < totalCompra){
                    printf("Saldo insuficiente para essa quantidade.Total: R$ %.2f", totalCompra);
                }else{
                    produtos[escolha].quantidade -= qtdDesejada;
                    saldoUsuario -= totalCompra;

                    adicionarCompraAoHistorico(produtos[escolha].nome, produtos[escolha].preco, qtdDesejada);

                    printf("Compra realizada com sucesso!\n");
                    printf("Saldo restante: R$ %.2f\n", saldoUsuario);
                }
            }
        }

        if(saldoUsuario > 0){
            printf("\nDeseja comprar outro produto? (s/n): ");
            getchar();//limpa o \n
            continuar = getchar();
        }else{
            printf("Saldo esgotado. Compra encerrada.\n");
            break;
        }
    }

    pausar();
}

void exibirHistoricoCompras(){
    limparTela();

    if(totalCompras == 0){
        printf("Nenhuma compra realizada ainda.\n");
    }else{
        printf("Historico de compras: \n\n");
        for(int i = 0; i < totalCompras; i++){
            printf("%d. %s - R$ %.2f- Quantidade: %d\n", i + 1, historico[i].nomeProduto, historico[i].precoUnitario, historico[i].quantidadeComprada);
        }
    }

    pausar();
}

void adicionarSaldo(){
    limparTela();
    float valor;
    printf("Digite o valor a adicionar ao saldo: R$");
    scanf("%f", &valor);

    if(valor > 0){
        saldoUsuario += valor;
        printf("Saldo atualizado: R$ %.2f\n", saldoUsuario);
    }else{
        printf("Valor invalido.\n");
    }

    pausar();
}

void exibirMenu() {
    printf("\tº======== Menu ========º\n");
    printf("\tº 1. Adicionar Produto º\n");
    printf("\tº 2. Comprar Produto   º\n");
    printf("\tº 3. Ver Historico     º\n");
    printf("\tº 4. Adicionar Saldo   º\n");
    printf("\tº 5. Sair              º\n");
    printf("\tº======================º\n");
    printf("Escolha uma opcao: ");
}


int main() {
    int opcao;


    do {
        limparTela();
        exibirMenu();


        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida.\n");
            while (getchar() != '\n');
            pausar();
            continue;
        }


        switch (opcao) {
            case 1:
                adicionarProduto();
                break;
            case 2:
                comprarProduto();
                break;
            case 3:
                exibirHistoricoCompras();
                break;
            case 4:
                adicionarSaldo();
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                pausar();
                break;
        }


    } while (opcao != 5);


    return 0;
}