#include "banco.h"

// Variaveis de conexao
sqlite3 *conn;
sqlite3_stmt *stmt;
int msg;

// Armazenar mensagem de erro
char *errMsg = 0;

// Variaveis comandos SQL
char *sql_update;
char *sql_delete;
char *sql_query;

int connectar_banco() {
    msg = sqlite3_open("banco.db", &conn);
    if (msg == SQLITE_OK) {
        char sql_foreign = "PRAGMA foreign_keys = ON;";
        msg = sqlite3_exec(conn, sql_foreign, 0, 0, 0);
        if (msg != SQLITE_OK) {
            return 1;
        } else {
            fprintf(stdout, "Erro ao conectar ao banco, erro ao tentar ativar foreign_keys\n");
            return 0;
        }
    }
    fprintf(stdout, "Erro ao conectar ao banco, SQLITE_OK false\n");
    return 0;
}

/**
 * Esse método cria as tabelas de clientes e log_usuários caso elas não existam.
 * @return 
 */
int criar_tabelas() {

    char sql_cliente = "CREATE TABLE IF NOT EXISTS clientes (id INTEGER PRIMARY KEY ASC NOT NULL, id_cliente INTEGER, nome_cliente VARCHAR(100), localizacao VARCHAR[255], email VARCHAR[50], aniversario VARCHAR[12], link VARCHAR[255]);";
    char sql_log_usuario = "CREATE TABLE IF NOT EXISTS log_usuario (id INTEGER NOT NULL PRIMARY KEY, created_at DATETIME NOT NULL, fk_id INTEGER NOT NULL, FOREIGN KEY (fk_id) REFERENCES cliente(id));";

    int resposta = connectar_banco();
    if (resposta != SQLITE_OK) {

        fprintf(stdout, "Não foi possivel conectar ao banco de dados: %s\n", sqlite3_errmsg(conn));
        close_db();
        return 0;
    } else {

        msg = sqlite3_exec(conn, sql_cliente, callback, 0, 0);
        if (msg != SQLITE_OK) {
            fprintf(stdout, "Não foi posível criar a tabela clientes: %s\n", sqlite3_errmsg(conn));
            close_db();
            return 0;
        } else {
            msg = sqlite3_exec(conn, sql_log_usuario, callback, 0, 0);
            if (msg != SQLITE_OK) {
                fprintf(stdout, "Não foi posível criar a tabela log_usuário: %s\n", sqlite3_errmsg(conn));
                close_db();
                return 0;
            } else {
                close_db();
                return 1;
            }
        }
    }
}

/**
 * Seleciona o cliente pelo id passado nos parâmetros.
 * @param id_cliente id do cliente no banco de dados.
 * @return 
 */
int cliente_by_id(int id_cliente) {

    char sql = "SELECT * FROM clientes WHERE id_cliente = ?;";
    if (connect_db() == 0) {
        sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, id_cliente, strlen(id_cliente), 0);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int aux = atoi((char*) sqlite3_column_text(stmt, 0));
            sqlite3_finalize(stmt);
            close_db();
            return aux;
        } else return -1;
    } else {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(conn));
        return -1;
    }
}

int inserir(cliente c) {

    char sql_insert = "INSERT INTO clientes VALUES(?, ?, ?, ?, ?, ?, ?, ?);";
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
    sql_update = "UPDATE clientes SET nome_cliente = ?, localizacao = ?, email = ?, aniversario = ?, link = ? WHERE id_cliente = ?;";

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
    sql_delete = "DELETE FROM clientes WHERE id_cliente = ?;";
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

int listar() {
    sql_query = "SELECT * FROM clientes;";
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
