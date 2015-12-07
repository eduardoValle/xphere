#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libs/mongoose.h"
#include "libs/sqlite3.h"
#include "includes/rotas.h"

//#include "resources.h"
//#include "post_request.h"
//#include "db/database.h"
//#include "network/ap.h"
//#include "network/iptables.h"
//#include "network/dhcp.h"

/**
 * @param conn connection struct
 * @param ev event
 * @return 
 */
static int ev_handler(struct mg_connection *conn, enum mg_event evento) {

    int rota;
    switch (evento) {
        case MG_AUTH:
            return MG_MORE;
        case MG_REQUEST:
            fprintf(stdout, "Processando... %s\n", conn->uri);
            rota = pega_rota(conn->uri, conn);
            if (rota == 404) {
                return MG_FALSE;
            } else if (!rota) {
                return MG_FALSE;
            }
            return MG_TRUE;
        default:
            return MG_FALSE;
    }
}

int main(void) {
    struct mg_server *server;

    //Start network configurations
    //setup_ap();
    //setup_dhcp();
    //setup_iptables();

    // Create and configure the server
    server = mg_create_server(NULL, ev_handler);
    mg_set_option(server, "document_root", ".");
    mg_set_option(server, "listening_port", "8080");

    limparRotas();
    init_resources_table();

    criar_tabela();

    //registra a rota do log do cliente
    add_route("/login", handle_post_request);

    // Serve request. Hit Ctrl-C to terminate the program
    printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
    for (;;) {
        mg_poll_server(server, 1000);
    }

    // Cleanup, and free server instance
    mg_destroy_server(&server);
    return 0;
}