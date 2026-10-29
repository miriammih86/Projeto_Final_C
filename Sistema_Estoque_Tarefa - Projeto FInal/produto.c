#include <stdio.h>
#include <string.h>
#include "produto.h"
#include "util.h"


static int codigoJaExiste(const Produto produtos[], int total, int codigo)
{
    int i;

    for (i = 0; i < total; i++) {
        if (produtos[i].codigo == codigo && produtos[i].ativo == 1) {
            return 1;
        }
    }

    return 0;
}


int buscarPorCodigo(const Produto produtos[], int total, int codigo)
{
    int i;
    for (i = 0; i < total; i++)
        if (produtos[i].codigo == codigo && produtos[i].ativo == 1) return i;
    return -1;
}

int buscarPorNome(const Produto produtos[], int total, const char nome[])
{
    int i;
    for (i = 0; i < total; i++)
        if (produtos[i].ativo == 1 && strcmp(produtos[i].nome, nome) == 0) return i;
    return -1;
}

void exibirProduto(const Produto *produto, const Categoria categorias[], int totalCategorias)
{
    int pos = buscarCategoriaPorCodigo(categorias, totalCategorias, produto->codigoCategoria);
    printf("\n========== DADOS DO PRODUTO ==========\n");
    printf("Codigo: %d\nNome: %s\nPrateleira: %c\nPreco unitario: R$ %.2f\nQuantidade em estoque: %d\n",
           produto->codigo, produto->nome, produto->prateleira, produto->preco, produto->quantidade);
    printf("Categoria: %s\n", pos >= 0 ? categorias[pos].nome : "Categoria nao encontrada");
    printf("Disponivel para venda: %s\n======================================\n",
           produto->disponivel ? "Sim" : "Nao");
}

int cadastrarProduto(Produto produtos[], int total, const Categoria categorias[], int totalCategorias)
{
    Produto novo;
    int codigoCategoria;
    if (total >= MAX_PRODUTOS) {
        printf("\nA lista esta cheia! Limite de %d produtos.\n", MAX_PRODUTOS);
        return total;
    }
    if (totalCategorias == 0) {
        printf("\nCadastre pelo menos uma categoria antes de cadastrar um produto.\n");
        return total;
    }
    printf("\n========== CADASTRO DE PRODUTO ==========\n");
    while (1) {
        novo.codigo = lerInteiroPositivo("Digite o codigo do produto: ");
        if (!codigoJaExiste(produtos, total, novo.codigo)) break;
        printf("Esse codigo ja esta cadastrado (inclusive em produtos inativos). Digite outro.\n");
    }
    lerTextoObrigatorio("Digite o nome do produto: ", novo.nome, sizeof(novo.nome));
    printf("\nCategorias disponiveis:\n");
    listarCategorias(categorias, totalCategorias);
    while (1) {
        codigoCategoria = lerInteiroPositivo("Digite o codigo da categoria do produto: ");
        if (buscarCategoriaPorCodigo(categorias, totalCategorias, codigoCategoria) >= 0) break;
        printf("Categoria inexistente. Escolha um codigo da lista.\n");
    }
    novo.codigoCategoria = codigoCategoria;
    novo.prateleira = lerPrateleira();
    novo.preco = lerPreco("Digite o preco unitario: R$ ");
    novo.quantidade = lerInteiroNaoNegativo("Digite a quantidade em estoque: ");
    novo.disponivel = novo.quantidade > 0 ? 1 : 0;
    novo.ativo = 1;
    produtos[total] = novo;
    printf("\nProduto cadastrado com sucesso!\n");
    exibirProduto(&novo, categorias, totalCategorias);
    return total + 1;
}

void listarProdutos(const Produto produtos[], int total, const Categoria categorias[], int totalCategorias)
{
    int i, encontrados = 0;
    printf("\n========== LISTA DE PRODUTOS ==========\n");
    for (i = 0; i < total; i++) {
        if (produtos[i].ativo != 1) continue;
        exibirProduto(&produtos[i], categorias, totalCategorias);
        encontrados++;
    }
    if (!encontrados) printf("Nenhum produto ativo cadastrado.\n");
}

void pesquisarProduto(const Produto produtos[], int total, const Categoria categorias[], int totalCategorias)
{
    int opcao, codigo, posicao;
    char nome[TAM_NOME_PRODUTO];
    printf("\n========== PESQUISAR PRODUTO ==========\n1 - Por codigo\n2 - Por nome exato\n");
    do {
        opcao = lerInteiroPositivo("Escolha uma opcao: ");
        if (opcao != 1 && opcao != 2) printf("Opcao invalida. Digite 1 ou 2.\n");
    } while (opcao != 1 && opcao != 2);
    if (opcao == 1) {
        codigo = lerInteiroPositivo("Digite o codigo do produto: ");
        posicao = buscarPorCodigo(produtos, total, codigo);
        if (posicao >= 0) exibirProduto(&produtos[posicao], categorias, totalCategorias);
        else printf("\nNenhum produto ativo encontrado com esse codigo.\n");
    } else {
        lerTextoObrigatorio("Digite o nome do produto: ", nome, sizeof(nome));
        posicao = buscarPorNome(produtos, total, nome);
        if (posicao >= 0) exibirProduto(&produtos[posicao], categorias, totalCategorias);
        else printf("\nNenhum produto ativo encontrado com esse nome exato.\n");
    }
}

void alterarEstoque(Produto produtos[], int total)
{
    int codigo, posicao, tipo, quantidade;
    printf("\n========== ALTERAR ESTOQUE ==========\n");
    codigo = lerInteiroPositivo("Digite o codigo do produto: ");
    posicao = buscarPorCodigo(produtos, total, codigo);
    if (posicao < 0) {
        printf("Produto nao encontrado ou inativo. Nenhuma alteracao realizada.\n");
        return;
    }
    printf("Produto: %s | Estoque atual: %d\n", produtos[posicao].nome, produtos[posicao].quantidade);
    do {
        tipo = lerInteiroPositivo("1 - Entrada | 2 - Saida: ");
        if (tipo != 1 && tipo != 2) printf("Opcao invalida. Digite 1 para entrada ou 2 para saida.\n");
    } while (tipo != 1 && tipo != 2);
    quantidade = lerInteiroPositivo("Digite a quantidade: ");
    if (tipo == 1) {
        if (quantidade > 2147483647 - produtos[posicao].quantidade) {
            printf("Quantidade excede o limite permitido. Estoque nao alterado.\n");
            return;
        }
        produtos[posicao].quantidade += quantidade;
    } else {
        if (quantidade > produtos[posicao].quantidade) {
            printf("Saida nao realizada: quantidade solicitada maior que o estoque atual.\n");
            return;
        }
        produtos[posicao].quantidade -= quantidade;
    }
    produtos[posicao].disponivel = produtos[posicao].quantidade > 0 ? 1 : 0;
    printf("Estoque atualizado com sucesso. Novo estoque: %d\n", produtos[posicao].quantidade);
}

void removerProduto(Produto produtos[], int total)
{
    int codigo, posicao;
    printf("\n========== REMOVER PRODUTO ==========\n");
    codigo = lerInteiroPositivo("Digite o codigo do produto: ");
    posicao = buscarPorCodigo(produtos, total, codigo);
    if (posicao < 0) {
        printf("Produto nao encontrado ou ja inativo.\n");
        return;
    }
    produtos[posicao].ativo = 0;
    printf("Produto '%s' removido(a) com sucesso.\n", produtos[posicao].nome);
}
