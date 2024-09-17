#include "http_server_builder.h"

/**
* Função principal que criar um HttpServer usando os argumentos de inicialização do programa.
 * Para verificar os possíveis parâmetros, execute "./http-server --help"
 *
 * @param argc número de argumentos de inicialização
 * @param argv argumentos de inicialização
 * @return O em caso de execução sem erros
 */
int main(int argc, char *argv[]) {

    // Cria um servidor HTTP lendo os parâmetros de inicialização
    HttpServer server = HttpServerBuilder()
            .byCommandLineArgs(argc, argv)
            .build();

    // Inicializa a execução do servidor HTTP
    server.start();

    return EXIT_SUCCESS;
}
