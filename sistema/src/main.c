#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/mongoose.h"
#include "../libs/sqlite3.h"

#define tamanho_buffer 150000

int main(void) {
    struct mg_server *server;

    // Criando e configurando o servidor
    server = mg_create_server(NULL, ev_handler);
    mg_set_option(server, "document_root", ".");
    mg_set_option(server, "listening_port", "8080");

    // Serve request. Hit Ctrl-C to terminate the program
    printf("Iniciando na porta %s\n", mg_get_option(server, "listening_port"));
    for (;;) {
        mg_poll_server(server, 1000);
    }

    // Limpando e liberando o servidor
    mg_destroy_server(&server);

    return 0;
}