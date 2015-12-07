#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "../libs/sqlite3.h"

#ifndef __DATABASE__
#define __DATABASE__

typedef struct {
    int id;
    int id_cliente;
    char nome_cliente[100];
    char localizacao[100];
    char email[50];
    char aniversario[12];
    char link[255];
} cliente;

int connectar_db();
int criar_tabelas();
int inserir(cliente c);
int atualizar(cliente c);
int deletar(int id_cliente);
int cliente_by_id(int id_cliente);
int listar();
void close_db();
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
#endif
