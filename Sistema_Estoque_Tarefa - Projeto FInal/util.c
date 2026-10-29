#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "util.h"

static void limparQuebraLinha(char texto[])
{
    size_t n = strlen(texto);
    while (n > 0 && (texto[n - 1] == '\n' || texto[n - 1] == '\r')) {
        texto[n - 1] = '\0';
        n--;
    }
}

static void lerLinha(const char *mensagem, char destino[], size_t tamanho)
{
    int c;
    printf("%s", mensagem);
    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    if (strchr(destino, '\n') == NULL) {
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    limparQuebraLinha(destino);
}

static int lerInteiro(const char *mensagem, int *valor)
{
    char linha[100], *fim;
    long numero;
    lerLinha(mensagem, linha, sizeof(linha));
    if (linha[0] == '\0') return 0;
    errno = 0;
    numero = strtol(linha, &fim, 10);
    while (isspace((unsigned char)*fim)) fim++;
    if (errno != 0 || *fim != '\0' || fim == linha ||
        numero < -2147483647L - 1 || numero > 2147483647L) return 0;
    *valor = (int)numero;
    return 1;
}

int lerInteiroPositivo(const char *mensagem)
{
    int valor;
    while (!lerInteiro(mensagem, &valor) || valor <= 0)
        printf("Entrada invalida. Digite um numero inteiro positivo.\n");
    return valor;
}

int lerInteiroNaoNegativo(const char *mensagem)
{
    int valor;
    while (!lerInteiro(mensagem, &valor) || valor < 0)
        printf("Entrada invalida. Digite um inteiro maior ou igual a zero.\n");
    return valor;
}

float lerPreco(const char *mensagem)
{
    char linha[100], *fim;
    float valor;
    int i;
    while (1) {
        lerLinha(mensagem, linha, sizeof(linha));
        for (i = 0; linha[i] != '\0'; i++)
            if (linha[i] == ',') linha[i] = '.';
        errno = 0;
        valor = strtof(linha, &fim);
        while (isspace((unsigned char)*fim)) fim++;
        if (linha[0] != '\0' && *fim == '\0' && fim != linha &&
            errno == 0 && valor >= 0.0f) return valor;
        printf("Preco invalido. Digite um valor >= 0 (ex.: 12.50).\n");
    }
}

void lerTextoObrigatorio(const char *mensagem, char destino[], size_t tamanho)
{
    int i;
    while (1) {
        lerLinha(mensagem, destino, tamanho);
        for (i = 0; destino[i] != '\0'; i++)
            if (destino[i] == ';') destino[i] = ',';
        i = 0;
        while (destino[i] != '\0' && isspace((unsigned char)destino[i])) i++;
        if (destino[i] != '\0') return;
        printf("O campo nao pode ficar em branco.\n");
    }
}

char lerPrateleira(void)
{
    char texto[30], letra;
    while (1) {
        lerLinha("Digite a prateleira (A, B, C ou D): ", texto, sizeof(texto));
        if (strlen(texto) == 1) {
            letra = (char)toupper((unsigned char)texto[0]);
            if (letra >= 'A' && letra <= 'D') return letra;
        }
        printf("Prateleira invalida. Escolha A, B, C ou D.\n");
    }
}
