#include "rotas.h"

static route tabela_rotas[TAMANHO_TABELA_ROTAS];

static int route_to_fill = 0;

/* Registra uma nova rota, recebe o caminhho e a funcao */
void add_route(const char *path, int(*fn) (struct mg_connection *conn)) {
    strncpy(tabela_rotas[route_to_fill].prefix, path, TAMANHO_PREFIXO);
    tabela_rotas[route_to_fill].fn = fn;
    route_to_fill++;
}

/**
 * Insere um valor vazio no lugar de cada elemento da tabela de rotas.
 */
void limparRotas() {
    int i;
    for (i = 0; i < TAMANHO_TABELA_ROTAS; i++) {
        strcpy(tabela_rotas[i].prefix, "");
        tabela_rotas[i].fn = NULL;
    }
}

int (const char *path, struct mg_connection *conn) {
    int i;
    char prefix[TAMANHO_PREFIXO];
    route aux;

    if (strcmp("/", prefix) == 0) {
        strncpy(prefix, "/index.html", TAMANHO_PREFIXO);
    } else {
        strncpy(prefix, path, TAMANHO_PREFIXO);
    }

    for (i = 0; i < route_to_fill; i++) {
        aux = tabela_rotas[i];
        if (strcmp(aux.prefix, prefix) == 0) {
            return aux.fn(conn);
        }
    }
    return erro_404(conn);
}

int erro_404(struct mg_connection *conn) {
    
    int i;
    route aux;
    for (i = 0; i < route_to_fill; i++) {
        aux = tabela_rotas[i];
        if (strcmp(aux.prefix, "/404.html") == 0) {
            return aux.fn(conn);
        }
    }
    return 404;
}

void init_resources_table() {
    add_route("/404.html", _404_html);
    add_route("/index.html", _index_html);
    add_route("/admin/index.html", _admin_index_html);
    add_route("/admin/img/facebook.png", _admin_img_facebook_png);
    add_route("/admin/img/phone.png", _admin_img_phone_png);
    add_route("/admin/img/caroline.png", _admin_img_caroline_png);
    add_route("/admin/img/vinicius.png", _admin_img_vinicius_png);
    add_route("/admin/img/ivan.png", _admin_img_ivan_png);
    add_route("/admin/img/mail.png", _admin_img_mail_png);
    add_route("/admin/img/github.png", _admin_img_github_png);
    add_route("/admin/img/denis.png", _admin_img_denis_png);
    add_route("/admin/img/maily.png", _admin_img_maily_png);
    add_route("/admin/img/bg.jpg", _admin_img_bg_jpg);
    add_route("/admin/img/menubra.png", _admin_img_menubra_png);
    add_route("/admin/css/bootstrap.min.css", _admin_css_bootstrap_min_css);
    add_route("/admin/css/bootstrap.css", _admin_css_bootstrap_css);
    add_route("/admin/css/stylish-portfolio.css", _admin_css_stylish_portfolio_css);
    add_route("/admin/js/bootstrap.min.js", _admin_js_bootstrap_min_js);
    add_route("/admin/js/jquery.js", _admin_js_jquery_js);
    add_route("/admin/js/bootstrap.js", _admin_js_bootstrap_js);
    add_route("/img/poc.png", _img_poc_png);
    add_route("/img/facebook.png", _img_facebook_png);
    add_route("/img/portfolio-3.jpg", _img_portfolio_3_jpg);
    add_route("/img/phone.png", _img_phone_png);
    add_route("/img/404img.jpg", _img_404img_jpg);
    add_route("/img/mail.png", _img_mail_png);
    add_route("/img/portfolio-6.jpg", _img_portfolio_6_jpg);
    add_route("/img/portfolio-2.jpg", _img_portfolio_2_jpg);
    add_route("/img/bg.jpg", _img_bg_jpg);
    add_route("/img/portfolio-4.jpg", _img_portfolio_4_jpg);
    add_route("/img/dose.png", _img_dose_png);
    add_route("/img/b2.jpg", _img_b2_jpg);
    add_route("/img/portfolio-5.jpg", _img_portfolio_5_jpg);
    add_route("/img/wifi.png", _img_wifi_png);
    add_route("/img/b1.jpg", _img_b1_jpg);
    add_route("/img/portfolio-1.jpg", _img_portfolio_1_jpg);
    add_route("/img/pet.png", _img_pet_png);
    add_route("/img/menubra.png", _img_menubra_png);
    add_route("/img/cerv.png", _img_cerv_png);
    add_route("/css/bootstrap.min.css", _css_bootstrap_min_css);
    add_route("/css/bootstrap.css", _css_bootstrap_css);
    add_route("/css/stylish-portfolio.css", _css_stylish_portfolio_css);
    add_route("/js/bootstrap.min.js", _js_bootstrap_min_js);
    add_route("/js/jquery.js", _js_jquery_js);
    add_route("/js/app.js", _js_app_js);
    add_route("/js/bootstrap.js", _js_bootstrap_js);
    add_route("/js/image_loader.js", _js_image_loader_js);
}