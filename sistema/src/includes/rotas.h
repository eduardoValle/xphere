#ifndef __ROTAS__
#define __ROTAS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/mongoose.h"

#define TAMANHO_TABELA_ROTAS 10000
#define TAMANHO_PREFIXO 1024

typedef struct route_t {
    char prefix[TAMANHO_PREFIXO];
    int(*fn) (struct mg_connection *conn);
} route;

/* Registra uma nova rota, recebe o caminhho e a funcao */
void add_route(const char *prefix, int(*fn) (struct mg_connection *conn));

/* Inicia a tabela de rotas limpando seus valores */
void init_routes_table();

/* Entrega a rota para o prefix pesquisado */
int pega_rota(const char *prefix, struct mg_connection *conn);

#endif
