#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produto.h"
#include "util.h"

#define ARQ_CATEGORIAS "categorias.txt"
#define ARQ_PRODUTOS "produtos.txt"

static int carregarCategorias(Categoria categorias[])
{
    FILE *arquivo = fopen(ARQ_CATEGORIAS, "r");
    int total = 0, codigo;
    char nome[TAM_NOME_CATEGORIA];
    if (arquivo == NULL) return 0;
    while (total < MAX_CATEGORIAS && fscanf(arquivo, "%d;%29[^\n]\n", &codigo, nome) == 2) {
        if (codigo > 0 && nome[0] != '\0' && buscarCategoriaPorCodigo(categorias, total, codigo) == -1) {
            categorias[total].codigo = codigo;
            strcpy(categorias[total].nome, nome);
            total++;
        }
    }
    fclose(arquivo);
    return total;
}

/* Aceita tanto o formato antigo (7 campos) quanto o novo (8 campos). */
static int carregarProdutos(Produto produtos[], const Categoria categorias[], int totalCategorias)
{
    FILE *arquivo = fopen(ARQ_PRODUTOS, "r");
    int total = 0, codigo, quantidade, disponivel, codigoCategoria, ativo, campos;
    char nome[TAM_NOME_PRODUTO], prateleira, linha[256];
    float preco;
    if (arquivo == NULL) return 0;
    while (total < MAX_PRODUTOS && fgets(linha, sizeof(linha), arquivo)) {
        campos = sscanf(linha, "%d;%49[^;];%c;%f;%d;%d;%d;%d",
                        &codigo, nome, &prateleira, &preco, &quantidade,
                        &disponivel, &codigoCategoria, &ativo);
        if (campos == 7) {
            ativo = 1; /* Compatibilidade com arquivos da Tarefa 4. */
        } else if (campos != 8) {
            continue;
        }
        if (codigo > 0 && nome[0] != '\0' && preco >= 0 && quantidade >= 0 &&
            (ativo == 0 || ativo == 1) && buscarPorCodigo(produtos, total, codigo) == -1 &&
            buscarCategoriaPorCodigo(categorias, totalCategorias, codigoCategoria) >= 0) {
            produtos[total].codigo = codigo;
            strcpy(produtos[total].nome, nome);
            produtos[total].prateleira = prateleira;
            produtos[total].preco = preco;
            produtos[total].quantidade = quantidade;
            produtos[total].disponivel = quantidade > 0 ? 1 : 0;
            produtos[total].codigoCategoria = codigoCategoria;
            produtos[total].ativo = ativo;
            total++;
        }
    }
    fclose(arquivo);
    return total;
}

static int salvarCategorias(const Categoria categorias[], int total)
{
    FILE *arquivo = fopen(ARQ_CATEGORIAS, "w");
    int i;
    if (!arquivo) { printf("Erro ao salvar categorias.\n"); return 0; }
    for (i = 0; i < total; i++) fprintf(arquivo, "%d;%s\n", categorias[i].codigo, categorias[i].nome);
    if (fclose(arquivo) != 0) return 0;
    return 1;
}

static int salvarProdutos(const Produto produtos[], int total)
{
    FILE *arquivo = fopen(ARQ_PRODUTOS, "w");
    int i;
    if (!arquivo) { printf("Erro ao salvar produtos.\n"); return 0; }
    for (i = 0; i < total; i++)
        fprintf(arquivo, "%d;%s;%c;%.2f;%d;%d;%d;%d\n", produtos[i].codigo, produtos[i].nome,
                produtos[i].prateleira, produtos[i].preco, produtos[i].quantidade,
                produtos[i].disponivel, produtos[i].codigoCategoria, produtos[i].ativo);
    if (fclose(arquivo) != 0) return 0;
    return 1;
}

static void menuCategorias(Categoria categorias[], int *total)
{
    int opcao;
    do {
        printf("\n========== CATEGORIAS ==========\n1 - Cadastrar Categoria\n2 - Listar Categorias\n3 - Voltar\n");
        opcao = lerInteiroPositivo("Escolha uma opcao: ");
        switch (opcao) {
            case 1: *total = cadastrarCategoria(categorias, *total); break;
            case 2: listarCategorias(categorias, *total); break;
            case 3: break;
            default: printf("Opcao invalida. Escolha 1, 2 ou 3.\n");
        }
    } while (opcao != 3);
}

int main(void)
{
    Produto produtos[MAX_PRODUTOS];
    Categoria categorias[MAX_CATEGORIAS];
    int totalCategorias = carregarCategorias(categorias);
    int totalProdutos = carregarProdutos(produtos, categorias, totalCategorias);
    int opcao;

 
    do {
        printf("\n========== SISTEMA DE ESTOQUE ==========\n");
        printf("1 - Cadastrar Produto\n2 - Listar Produtos\n3 - Pesquisar Produto\n");
        printf("4 - Alterar Estoque de um Produto\n5 - Remover Produto\n6 - Categorias\n7 - Sair\n");
        opcao = lerInteiroPositivo("Escolha uma opcao: ");
        switch (opcao) {
            case 1: totalProdutos = cadastrarProduto(produtos, totalProdutos, categorias, totalCategorias); break;
            case 2: listarProdutos(produtos, totalProdutos, categorias, totalCategorias); break;
            case 3: pesquisarProduto(produtos, totalProdutos, categorias, totalCategorias); break;
            case 4: alterarEstoque(produtos, totalProdutos); break;
            case 5: removerProduto(produtos, totalProdutos); break;
            case 6: menuCategorias(categorias, &totalCategorias); break;
            case 7:
                if (salvarCategorias(categorias, totalCategorias) && salvarProdutos(produtos, totalProdutos))
                    printf("\nDados salvos com sucesso. Programa encerrado!\n");
                else
                    printf("\nHouve erro ao salvar os dados. Verifique a pasta do programa.\n");
                break;
            default: printf("Opcao invalida. Escolha de 1 a 7.\n");
        }
    } while (opcao != 7);
    return 0;
}

