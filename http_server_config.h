#ifndef HTTP_SERVER_HTTP_SERVER_CONFIG_H
#define HTTP_SERVER_HTTP_SERVER_CONFIG_H

#include <string>
#include <optional>
#include "http_server_mode.h"


struct HttpServerConfig {

    std::optional<std::string> name = "SI-CPCX-UFMS";
    std::optional<uint16_t> port = 8080;
    std::optional<std::string> cgi_bin_dir = "../resources/cgi-bin";
    std::optional<std::string> root_dir = "../resources/www";
    std::optional<std::string> homepage = "index.html";
    std::optional<HttpServerMode::Type> mode = HttpServerMode::Type::FORK;
    std::optional<uint32_t> max_simultaneous_connections = 100;
    std::optional<uint32_t> max_pending_connections = 100;
    std::optional<bool> keep_alive = true;
    std::optional<std::string> log_file = std::nullopt;
    std::optional<bool> verbose = false;
};


#endif //HTTP_SERVER_HTTP_SERVER_CONFIG_H
