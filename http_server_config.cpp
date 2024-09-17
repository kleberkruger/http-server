#include "http_server_config.h"
#include <iostream>
#include <yaml-cpp/yaml.h>

HttpServerConfig::HttpServerConfig(std::string_view config_file,
                                   const std::unordered_map<std::string, std::any> &args) :
        _name(DEFAULT_NAME),
        _port(DEFAULT_PORT),
        _cgi_bin_dir(DEFAULT_CGI_DIR),
        _root_dir(DEFAULT_ROOT_DIR),
        _homepage(DEFAULT_HOMEPAGE),
        _mode(DEFAULT_MODE),
        _max_simultaneous_connections(DEFAULT_MAX_SIMULTANEOUS_CONNECTIONS),
        _max_pending_connections(DEFAULT_MAX_PENDING_CONNECTIONS),
        _keep_alive(DEFAULT_KEEP_ALIVE) {

    if (!config_file.empty()) {

        try {
            YAML::Node config = YAML::LoadFile(config_file.data());

            const auto &server = config["server"];
            _name = server["name"].as<std::string>();
            _port = server["port"].as<uint16_t>();
            _homepage = server["homepage"].as<std::string>();
            _mode = HttpServerMode::toEnum(server["mode"].as<std::string>());
            _keep_alive = server["keep_alive"].as<bool>();
            _max_simultaneous_connections = _mode == HttpServerMode::Type::SYNC ? 1 :
                                            server["max_simultaneous_connections"].as<uint32_t>();
            _max_pending_connections = server["max_pending_connections"].as<uint32_t>();

            const auto &resource = config["resource"];
            _cgi_bin_dir = resource["cgi_bin"].as<std::string>();
            _root_dir = resource["root_dir"].as<std::string>();

//            std::cout << "Mapping:\n";
//            for (const auto &item: config["mapping"]) {
//                const auto key = item.first.as<std::string>();
//                if (item.second.IsSequence()) {
//                    std::cout << "  " << key << ":\n";
//                    for (const auto &value: item.second) {
//                        std::cout << "    - " << value.as<std::string>() << std::endl;
//                    }
//                } else {
//                    std::cout << "  " << key << ": " << item.second.as<std::string>() << std::endl;
//                }
//            }
        } catch (const YAML::Exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

HttpServerConfig::~HttpServerConfig() = default;

void HttpServerConfig::print() const {
    std::cout << "Info:\n";
    std::cout << "  name: " << _name << "\n";
    std::cout << "Server:\n";
    std::cout << "  port: " << _port << "\n";
    std::cout << "  homepage: " << _homepage << "\n";
    std::cout << "  mode: " << HttpServerMode::toString(_mode) << "\n";
    std::cout << "  keep-alive: " << (_keep_alive ? "true" : "false") << "\n";
    std::cout << "  max simultaneous connections: " << _max_simultaneous_connections << "\n";
    std::cout << "  max pending connections: " << _max_pending_connections << "\n";
    std::cout << "Resource:\n";
    std::cout << "  cgi-bin: " << _cgi_bin_dir << "\n";
    std::cout << "  www: " << _root_dir << "\n";
}
