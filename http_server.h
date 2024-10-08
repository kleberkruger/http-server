#ifndef HTTP_SERVER_HTTP_SERVER_H
#define HTTP_SERVER_HTTP_SERVER_H

#include <string>
#include "http_server_mode.h"


class HttpServer {
public:
    static constexpr const char *VERSION = "1.0";

//    HttpServer(std::string_view name, uint16_t port,
//               std::string_view cgi_bin_dir, std::string_view root_dir,
//               std::string_view homepage, HttpServerMode::Type mode,
//               uint32_t max_simultaneous_connections, uint32_t max_pending_connections,
//               bool keep_alive, std::string_view log_file, bool verbose);

    void start();

private:
    std::string _name;
    uint16_t _port;
    std::string _cgi_bin_dir;
    std::string _root_dir;
    std::string _homepage;
    HttpServerMode::Type _mode;
    uint32_t _max_simultaneous_connections;
    uint32_t _max_pending_connections;
    bool _keep_alive;
    std::string _log_file;
    bool _verbose;
};


#endif //HTTP_SERVER_HTTP_SERVER_H
