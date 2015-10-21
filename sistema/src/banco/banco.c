#include "banco.h"

// Variaveis de conexao
sqlite3 *conn;
sqlite3_stmt *stmt;
int msg;

// Armazenar mensagem de erro
char *errMsg = 0;

// Variaveis comandos SQL
char *sql_create;
char *sql_insert;
char *sql_update;
char *sql_delete;
char *sql_query_client;
char *sql_query;

int connectar_banco() {
    return msg = sqlite3_open("banco.db", &conn);
}

int criar_tabela() {
    sql_create = "CREATE TABLE IF NOT EXISTS cliente (id INTEGER PRIMARY KEY ASC NOT NULL, id_cliente INTEGER, nome_cliente VARCHAR(100), localizacao VARCHAR[255], email VARCHAR[50], aniversario VARCHAR[12], link VARCHAR[255]);";

    if (connect_db() != SQLITE_OK) {

        fprintf(stderr, "Conexão com o banco falhou!! %s\n", sqlite3_errmsg(conn));
        close_db();
        return 0;
    } else {

        msg = sqlite3_exec(conn, sql_create, callback, 0, &errMsg);

        if (msg != SQLITE_OK) {

            fprintf(stderr, "Não foi possivel executar comando!! %s\n", errMsg);
            close_db();
            return 0;
        } else {
            fprintf(stdout, "Conexão com o banco efetuada com sucesso!!\n");
        }
        close_db();
        return 1;
    }
}

int inserir(cliente c) {
    sql_insert = "INSERT INTO cliente VALUES(?, ?, ?, ?, ?, ?, ?, ?);";
    int r = connect_db();
    if (r != SQLITE_OK) {

        fprintf(stderr, "Erro ao conectar com o banco de dados!! %s\n", sqlite3_errmsg(conn));
        close_db();
        return 0;
    } else {

        msg = sqlite3_exec(conn, sql_insert, callback, 0, &errMsg);
        if (msg != SQLITE_OK) {

            fprintf(stderr, "Não foi possivel executar comando!! %s\n", errMsg);
            close_db();

            return 0;
        } else {
            sqlite3_bind_null(stmt, 1);
            sqlite3_bind_int(stmt, 2, c.id_cliente);
            sqlite3_bind_text(stmt, 3, c.nome_cliente, strlen(c.nome_cliente), 0);
            sqlite3_bind_text(stmt, 4, c.localizacao, strlen(c.localizacao), 0);
            sqlite3_bind_text(stmt, 5, c.email, strlen(c.email), 0);
            sqlite3_bind_text(stmt, 6, c.aniversario, strlen(c.aniversario), 0);
            sqlite3_bind_text(stmt, 7, c.link, strlen(c.link), 0);
        }
        close_db();
        return 1;
    }
}

int atualizar(cliente c) {
    sql_update = "UPDATE cliente SET nome_cliente = ?, localizacao = ?, email = ?, aniversario = ?, link = ? WHERE id_cliente = ?;";

    int r = connect_db();
    if (r != SQLITE_OK) {

        fprintf(stderr, "Erro ao conectar com o banco de dados!! %s\n", sqlite3_errmsg(conn));
        close_db();
        return 0;
    } else {

        msg = sqlite3_exec(conn, sql_update, callback, 0, &errMsg);
        if (msg != SQLITE_OK) {
            fprintf(stderr, "Não foi possivel executar comando!! %s\n", errMsg);
            close_db();
            return 0;
        } else {
            sqlite3_bind_text(stmt, 1, c.nome_cliente, strlen(c.nome_cliente), 0);
            sqlite3_bind_text(stmt, 2, c.localizacao, strlen(c.localizacao), 0);
            sqlite3_bind_text(stmt, 3, c.email, strlen(c.email), 0);
            sqlite3_bind_text(stmt, 4, c.aniversario, strlen(c.aniversario), 0);
            sqlite3_bind_text(stmt, 5, c.link, strlen(c.link), 0);
            sqlite3_bind_int(stmt, 6, c.id_cliente);
        }
        close_db();
        return 1;
    }
}

int deletar(int id_cliente) {
    sql_delete = "DELETE FROM cliente WHERE id_cliente = ?;";
    int r = connect_db();
    if (r != SQLITE_OK) {

        fprintf(stderr, "Erro ao conectar com o banco de dados!! %s\n", sqlite3_errmsg(conn));
        close_db();
        return 0;

    } else if (cliente_by_id(id_cliente)) {
        msg = sqlite3_prepare_v2(conn, sql_delete, -1, &stmt, 0);
        if (msg != SQLITE_OK) {
            fprintf(stderr, "Não foi possivel executar comando!! %s\n", errMsg);
            close_db();
            return 0;
        } else {
            sqlite3_bind_int(stmt, 1, id_cliente);
            return 1;
        }
        close_db();
    }
    return 1;
}

int cliente_by_id(int id_cliente) {
    sql_query_client = "SELECT * FROM cliente WHERE id_cliente = ?;";
    int r = connect_db();
    if (r != SQLITE_OK) {

        fprintf(stderr, "Erro ao conectar com o banco de dados!! %s\n", sqlite3_errmsg(conn));
        close_db();
        return 0;
    } else {

        r = sqlite3_prepare_v2(conn, sql_query_client, -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, id_cliente);

        int step = sqlite3_step(stmt);
        if (step == SQLITE_ROW) {
            return 1;
        }
        close_db();
        return 0;
    }
}

int listar() {
    sql_query = "SELECT * FROM cliente;";
    int r = connect_db();
    if (r != SQLITE_OK) {

        fprintf(stderr, "Erro ao conectar com o banco de dados!! %s\n", sqlite3_errmsg(conn));
        close_db();
        return 0;
    } else {

        msg = sqlite3_exec(conn, sql_query, callback, 0, &errMsg);
        if (msg != SQLITE_OK) {

            fprintf(stderr, "Não foi possivel executar comando!! %s\n", errMsg);
            close_db();
            return 0;
        }
        close_db();
        return 1;
    }
}

void close_db() {
    sqlite3_free(errMsg);
    sqlite3_close(conn);
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
