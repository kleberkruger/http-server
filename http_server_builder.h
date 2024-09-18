#ifndef HTTP_SERVER_HTTP_SERVER_BUILDER_H
#define HTTP_SERVER_HTTP_SERVER_BUILDER_H

#include <cstdint>
#include <expected>
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

    auto setName(std::string_view name) -> HttpServerBuilder & {
        _name = name;
        return *this;
    }

    auto setPort(const uint16_t port) -> HttpServerBuilder & {
        _port = port;
        return *this;
    }

    auto setCGIDirectory(const std::string &cgi_bin_dir) -> HttpServerBuilder & {
        _cgi_bin_dir = cgi_bin_dir;
        return *this;
    }

    auto setRootDirectory(const std::string &root_dir) -> HttpServerBuilder & {
        _root_dir = root_dir;
        return *this;
    }

    auto setPaths(const std::string &root_dir, const std::string &cgi_bin_dir) -> HttpServerBuilder & {
        _root_dir = root_dir;
        _cgi_bin_dir = cgi_bin_dir;
        return *this;
    }

    auto setHomepage(const std::string &homepage) -> HttpServerBuilder & {
        _homepage = homepage;
        return *this;
    }

    auto setMode(const HttpServerMode::Type mode) -> HttpServerBuilder & {
        _mode = mode;
        return *this;
    }

    auto setMaxSimultaneousConnections(const uint32_t max_simultaneous_connections) -> HttpServerBuilder & {
        _max_simultaneous_connections = max_simultaneous_connections;
        return *this;
    }

    auto setMaxPendingConnections(const uint32_t max_pending_connections) -> HttpServerBuilder & {
        _max_pending_connections = max_pending_connections;
        return *this;
    }

    auto setModeAndMaxConnections(const HttpServerMode::Type mode,
                                  const uint32_t max_simultaneous_connections,
                                  const uint32_t max_pending_connections) -> HttpServerBuilder & {
        _mode = mode;
        _max_simultaneous_connections = max_simultaneous_connections;
        _max_pending_connections = max_pending_connections;

        return *this;
    }

    auto setKeepAlive(const bool keep_alive) -> HttpServerBuilder & {
        _keep_alive = keep_alive;
        return *this;
    }

    auto setLogFile(const std::string &log_file) -> HttpServerBuilder & {
        _log_file = log_file;
        return *this;
    }

    auto setVerbose(const bool verbose) -> HttpServerBuilder & {
        _verbose = verbose;
        return *this;
    }

    auto byCommandLineArgs(int argc, char *argv[], bool use_config_file = true) -> HttpServerBuilder &;

    auto byConfigFile(std::string_view config_file) -> HttpServerBuilder &;

    [[nodiscard]] HttpServer build() const;

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
    uint32_t _max_simultaneous_connections; // talvez std::optional
    uint32_t _max_pending_connections;
    bool _keep_alive;
    std::optional<std::string> _log_file = std::nullopt;
    bool _verbose;

    void saveConfigFile(std::string_view config_file);

    static void addAndParseArgs(argparse::ArgumentParser &program, int argc, char *argv[], bool use_config_file = true);
};


#endif //HTTP_SERVER_HTTP_SERVER_BUILDER_H
