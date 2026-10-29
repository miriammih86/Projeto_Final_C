#ifndef PRODUTO_H
#define PRODUTO_H

#include <stddef.h>

#define MAX_PRODUTOS 50
#define TAM_NOME_PRODUTO 50
#define MAX_CATEGORIAS 20
#define TAM_NOME_CATEGORIA 30

typedef struct {
    int codigo;
    char nome[TAM_NOME_CATEGORIA];
} Categoria;

typedef struct {
    int codigo;
    char nome[TAM_NOME_PRODUTO];
    char prateleira;
    float preco;
    int quantidade;
    int disponivel;
    int codigoCategoria;
    int ativo;
} Produto;

int buscarPorCodigo(const Produto produtos[], int total, int codigo);
int buscarPorNome(const Produto produtos[], int total, const char nome[]);
int buscarCategoriaPorCodigo(const Categoria categorias[], int total, int codigo);
int cadastrarCategoria(Categoria categorias[], int total);
void listarCategorias(const Categoria categorias[], int total);
int cadastrarProduto(Produto produtos[], int total, const Categoria categorias[], int totalCategorias);
void listarProdutos(const Produto produtos[], int total, const Categoria categorias[], int totalCategorias);
void pesquisarProduto(const Produto produtos[], int total, const Categoria categorias[], int totalCategorias);
void alterarEstoque(Produto produtos[], int total);
void removerProduto(Produto produtos[], int total);
void exibirProduto(const Produto *produto, const Categoria categorias[], int totalCategorias);

int lerInteiroPositivo(const char *mensagem);
int lerInteiroNaoNegativo(const char *mensagem);
float lerPreco(const char *mensagem);
void lerTextoObrigatorio(const char *mensagem, char destino[], size_t tamanho);
char lerPrateleira(void);

#endif
