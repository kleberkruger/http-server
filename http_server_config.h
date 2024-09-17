#ifndef HTTP_SERVER_HTTP_SERVER_CONFIG_H
#define HTTP_SERVER_HTTP_SERVER_CONFIG_H

#include <cstdint>
#include <unordered_map>
#include <any>
#include "http_server_mode.h"

class HttpServerConfig {
public:

//    explicit HttpServerConfig(std::string_view config_file);

    explicit HttpServerConfig(std::string_view config_file = "",
                              const std::unordered_map<std::string, std::any> &args = {});

    virtual ~HttpServerConfig();

    void print() const;

    [[nodiscard]] const std::string &getName() const { return _name; }

    void setName(const std::string &name) { _name = name; }

    [[nodiscard]] uint16_t getPort() const { return _port; }

    void setPort(uint16_t port) { _port = port; }

    [[nodiscard]] const std::string &getCgiBinDir() const { return _cgi_bin_dir; }

    void setCgiBinDir(const std::string &cgiBinDir) { _cgi_bin_dir = cgiBinDir; }

    [[nodiscard]] const std::string &getRootDir() const { return _root_dir; }

    void setRootDir(const std::string &rootDir) { _root_dir = rootDir; }

    [[nodiscard]] const std::string &getHomepage() const { return _homepage; }

    void setHomepage(const std::string &homepage) { _homepage = homepage; }

    [[nodiscard]] HttpServerMode::Type getMode() const { return _mode; }

    void setMode(HttpServerMode::Type mode) { _mode = mode; }

    [[nodiscard]] uint32_t getMaxSimultaneousConnections() const {
        return _max_simultaneous_connections;
    }

    void setMaxSimultaneousConnections(uint32_t maxSimultaneousConnections) {
        _max_simultaneous_connections = maxSimultaneousConnections;
    }

    [[nodiscard]] uint32_t getMaxPendingConnections() const {
        return _max_pending_connections;
    }

    void setMaxPendingConnections(uint32_t maxPendingConnections) {
        _max_pending_connections = maxPendingConnections;
    }

    [[nodiscard]] bool isKeepAlive() const { return _keep_alive; }

    void setKeepAlive(bool keepAlive) { _keep_alive = keepAlive; }

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

    std::string _name;
    uint16_t _port;
    std::string _cgi_bin_dir;
    std::string _root_dir;
    std::string _homepage;
    HttpServerMode::Type _mode;
    uint32_t _max_simultaneous_connections;
    uint32_t _max_pending_connections;
    bool _keep_alive;

    std::unordered_map<std::string, std::string> route_mapping;
};


#endif //HTTP_SERVER_HTTP_SERVER_CONFIG_H
