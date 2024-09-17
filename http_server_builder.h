#ifndef HTTP_SERVER_HTTP_SERVER_BUILDER_H
#define HTTP_SERVER_HTTP_SERVER_BUILDER_H

#include <cstdint>
#include <argparse/argparse.hpp>
#include "http_server_mode.h"
#include "http_server.h"

class HttpServerBuilder {
public:

    explicit HttpServerBuilder() : _name(DEFAULT_NAME),
                                   _port(DEFAULT_PORT),
                                   _cgi_bin_dir(DEFAULT_CGI_DIR),
                                   _root_dir(DEFAULT_ROOT_DIR),
                                   _homepage(DEFAULT_HOMEPAGE),
                                   _mode(DEFAULT_MODE),
                                   _max_simultaneous_connections(DEFAULT_MAX_SIMULTANEOUS_CONNECTIONS),
                                   _max_pending_connections(DEFAULT_MAX_PENDING_CONNECTIONS),
                                   _keep_alive(DEFAULT_KEEP_ALIVE),
                                   _verbose(DEFAULT_VERBOSE) {}

    ~HttpServerBuilder() = default;

    HttpServerBuilder &setName(const std::string &name) {
        _name = name;
        return *this;
    }

    HttpServerBuilder &setPort(const uint16_t port) {
        _port = port;
        return *this;
    }

    HttpServerBuilder &setCGIDirectory(const std::string &cgi_bin_dir) {
        _cgi_bin_dir = cgi_bin_dir;
        return *this;
    }

    HttpServerBuilder &setRootDirectory(const std::string &root_dir) {
        _root_dir = root_dir;
        return *this;
    }

    HttpServerBuilder &setPaths(const std::string &root_dir, const std::string &cgi_bin_dir) {
        _root_dir = root_dir;
        _cgi_bin_dir = cgi_bin_dir;
        return *this;
    }

    HttpServerBuilder &setHomepage(const std::string &homepage) {
        _homepage = homepage;
        return *this;
    }

    HttpServerBuilder &setMode(const HttpServerMode::Type mode) {
        _mode = mode;
        return *this;
    }

    HttpServerBuilder &setMaxSimultaneousConnections(const uint32_t max_simultaneous_connections) {
        _max_simultaneous_connections = max_simultaneous_connections;
        return *this;
    }

    HttpServerBuilder &setMaxPendingConnections(const uint32_t max_pending_connections) {
        _max_pending_connections = max_pending_connections;
        return *this;
    }

    HttpServerBuilder &setModeAndMaxConnections(const HttpServerMode::Type mode,
                                                const uint32_t max_simultaneous_connections,
                                                const uint32_t max_pending_connections) {
        _mode = mode;
        _max_simultaneous_connections = max_simultaneous_connections;
        _max_pending_connections = max_pending_connections;

        return *this;
    }

    HttpServerBuilder &setKeepAlive(const bool keep_alive) {
        _keep_alive = keep_alive;
        return *this;
    }

    HttpServerBuilder &setLogFile(const std::string &log_file) {
        _log_file = log_file;
        return *this;
    }

    HttpServerBuilder &setVerbose(const bool verbose) {
        _verbose = verbose;
        return *this;
    }

    HttpServerBuilder &byCommandLineArgs(int argc, char *argv[], bool use_config_file = true);

    HttpServerBuilder &byConfigFile(std::string_view config_file);

    HttpServer build() const;

private:

    static constexpr const char *DEFAULT_NAME = "SI-CPCX-UFMS";
    static constexpr uint16_t DEFAULT_PORT = 8080;
    static constexpr const char *DEFAULT_CGI_DIR = "../resources/cgi-bin";
    static constexpr const char *DEFAULT_ROOT_DIR = "../resources/www";
    static constexpr const char *DEFAULT_HOMEPAGE = "index.html";
    static constexpr HttpServerMode::Type DEFAULT_MODE = HttpServerMode::Type::FORK;
    static constexpr uint32_t DEFAULT_MAX_SIMULTANEOUS_CONNECTIONS = 100;
    static constexpr uint32_t DEFAULT_MAX_PENDING_CONNECTIONS = 100;
    static constexpr bool DEFAULT_KEEP_ALIVE = true;
    static constexpr bool DEFAULT_VERBOSE = false;

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

    void saveConfigFile(std::string_view config_file);

    static void addAndParseArgs(argparse::ArgumentParser &program, int argc, char *argv[], bool use_config_file = true);
};


#endif //HTTP_SERVER_HTTP_SERVER_BUILDER_H
