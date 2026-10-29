SISTEMA DE ESTOQUE - TAREFA 5

Arquivos-fonte:
- Main.c: menu principal e persistencia em arquivos
- produto.c / produto.h: cadastro, listagem, pesquisa, alteracao de estoque e remocao logica
- categoria.c: cadastro e listagem de categorias
- util.c / util.h: validacao de entradas

Como compilar (GCC):
gcc -std=c11 -Wall -Wextra -pedantic Main.c produto.c categoria.c util.c -o sistema_estoque

Execute o programa no mesmo diretorio em que deseja manter categorias.txt e produtos.txt.
Os arquivos de dados sao criados/atualizados ao escolher a opcao 7 - Sair.
O carregamento aceita o formato antigo de produtos.txt (Tarefa 4, 7 campos) e o novo formato (8 campos, incluindo ativo).
