#include <stdio.h>
#include "produto.h"
#include "util.h"

int buscarCategoriaPorCodigo(const Categoria categorias[], int total, int codigo)
{
    int i;
    for (i = 0; i < total; i++)
        if (categorias[i].codigo == codigo) return i;
    return -1;
}

int cadastrarCategoria(Categoria categorias[], int total)
{
    Categoria nova;
    if (total >= MAX_CATEGORIAS) {
        printf("\nLimite de %d categorias atingido.\n", MAX_CATEGORIAS);
        return total;
    }
    printf("\n========== CADASTRO DE CATEGORIA ==========\n");
    while (1) {
        nova.codigo = lerInteiroPositivo("Digite o codigo da categoria: ");
        if (buscarCategoriaPorCodigo(categorias, total, nova.codigo) == -1) break;
        printf("Esse codigo ja existe. Digite outro.\n");
    }
    lerTextoObrigatorio("Digite o nome da categoria: ", nova.nome, sizeof(nova.nome));
    categorias[total] = nova;
    printf("\nCategoria cadastrada com sucesso!\n");
    return total + 1;
}

void listarCategorias(const Categoria categorias[], int total)
{
    int i;
    if (total == 0) {
        printf("\nNenhuma categoria cadastrada.\n");
        return;
    }
    printf("\n========== CATEGORIAS CADASTRADAS ==========\n");
    for (i = 0; i < total; i++)
        printf("Codigo: %d | Nome: %s\n", categorias[i].codigo, categorias[i].nome);
}
