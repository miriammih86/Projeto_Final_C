#ifndef UTIL_H
#define UTIL_H
#include <stddef.h>

int lerInteiroPositivo(const char *mensagem);
int lerInteiroNaoNegativo(const char *mensagem);
float lerPreco(const char *mensagem);
void lerTextoObrigatorio(const char *mensagem, char destino[], size_t tamanho);
char lerPrateleira(void);

#endif
