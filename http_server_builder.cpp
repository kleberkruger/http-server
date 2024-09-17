#include "http_server_builder.h"
#include <filesystem>
#include <yaml-cpp/yaml.h>

void HttpServerBuilder::addAndParseArgs(argparse::ArgumentParser &program, int argc, char *argv[],
                                        bool use_config_file) {
    if (use_config_file) {
        program.add_argument("-c", "--config")
                .help("sets a config file")
                .default_value("../config/httpserver.yaml")
                .required();
    }

    auto &group = program.add_mutually_exclusive_group();
    group.add_argument("-s", "--sync")
            .help("runs the server on sync mode")
            .nargs(0, 1)
            .scan<'i', uint32_t>();
    group.add_argument("-f", "--fork")
            .help("runs the server on fork mode")
            .nargs(0, 2)
            .scan<'i', uint32_t>();
    group.add_argument("-t", "--thread")
            .help("runs the server on thread mode")
            .nargs(0, 2)
            .scan<'i', uint32_t>();
    group.add_argument("-a", "--async")
            .help("runs the server on async mode")
            .nargs(0, 2)
            .scan<'i', uint32_t>();

    program.add_argument("--name")
            .help("sets server name");

    program.add_argument("-p", "--port")
            .help("sets port")
            .scan<'i', uint16_t>();

    program.add_argument("--home")
            .help("sets home address");

    program.add_argument("--path")
            .help("sets the path of <root> <cgi-bin> directories")
            .nargs(1, 2);

    program.add_argument("--log")
            .help("sets a log file")
            .nargs(0, 1);

    program.add_argument("--no-keep-alive")
            .help("disables keep-alive connections")
            .flag();

    program.add_argument("--verbose")
            .help("shows output verbosity")
            .flag();

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << program;
        exit(EXIT_FAILURE);
    }
}

HttpServerBuilder &HttpServerBuilder::byCommandLineArgs(int argc, char *argv[], bool use_config_file) {
    argparse::ArgumentParser program("HTTP Server", HttpServer::VERSION);
    addAndParseArgs(program, argc, argv, use_config_file);

    bool saveConfig = false;
    if (use_config_file && program.is_used("config")) {
        auto config_file = program.get("config");
        if (!std::filesystem::exists(config_file)) {
            saveConfig = true;
        }
        byConfigFile(config_file);
    }

    std::string mode_in_use = program.is_used("sync") ? "sync" :
                              program.is_used("fork") ? "fork" :
                              program.is_used("thread") ? "thread" :
                              program.is_used("async") ? "async" :
                              "";
    if (!mode_in_use.empty()) {
        _mode = HttpServerMode::toEnum(mode_in_use);
        if (_mode == HttpServerMode::Type::SYNC) {
            _max_simultaneous_connections = 1;
        }
        auto args = program.get<std::vector<uint32_t>>(mode_in_use);
        if (!args.empty()) {
            if (_mode == HttpServerMode::Type::SYNC) {
                _max_pending_connections = args[0];
            } else {
                _max_simultaneous_connections = args[0];
            }
            if (args.size() > 1) {
                _max_pending_connections = args[1];
            }
        }
    }

    if (program.is_used("name")) {
        _name = program.get("name");
    }
    if (program.is_used("port")) {
        _port = program.get<uint16_t>("port");
    }
    if (program.is_used("home")) {
        _homepage = program.get("home");
    }
    if (program.is_used("path")) {
        auto args = program.get<std::vector<std::string>>("path");
        if (!args.empty()) {
            _root_dir = args[0];
            if (args.size() > 1) {
                _cgi_bin_dir = args[1];
            }
        }
    }
    if (program.is_used("no-keep-alive")) {
        _keep_alive = false;
    }
    if (program.is_used("log")) {
        auto args = program.get<std::vector<std::string>>("log");
        _log_file = !args.empty() ? args[0] : "log.txt";
        std::cout << "Log: [" << _log_file << "]\n";
    }
    if (program.is_used("verbose")) {
        _verbose = true;
    }

    if (saveConfig) {
        saveConfigFile(program.get("config"));
    }

    return *this;
}

HttpServerBuilder &HttpServerBuilder::byConfigFile(std::string_view config_file) {
    try {
        YAML::Node config = YAML::LoadFile(config_file.data());

        const auto &server = config["server"];
        _name = server["name"].as<std::string>();
        _port = server["port"].as<uint16_t>();
        _homepage = server["homepage"].as<std::string>();
        _mode = HttpServerMode::toEnum(server["mode"].as<std::string>());
        _keep_alive = server["keep_alive"].as<bool>();
        _max_simultaneous_connections = server["max_simultaneous_connections"].as<uint32_t>();
        _max_pending_connections = server["max_pending_connections"].as<uint32_t>();

        const auto &resource = config["resource"];
        _cgi_bin_dir = resource["cgi_bin"].as<std::string>();
        _root_dir = resource["root_dir"].as<std::string>();

        // Ler e exibir o "mapping"
        std::cout << "Mapping:\n";
        for (const auto &item: config["mapping"]) {
            const auto key = item.first.as<std::string>();
            if (item.second.IsSequence()) {
                std::cout << "  " << key << ":\n";
                for (const auto &value: item.second) {
                    std::cout << "    - " << value.as<std::string>() << "\n";
                }
            } else {
                std::cout << "  " << key << ": " << item.second.as<std::string>() << "\n";
            }
        }

    } catch (const YAML::Exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return *this;
}

HttpServer HttpServerBuilder::build() const {
    std::cout << "Server:\n";
    std::cout << "  name: " << _name << "\n";
    std::cout << "  port: " << _port << "\n";
    std::cout << "  homepage: " << _homepage << "\n";
    std::cout << "  mode: " << HttpServerMode::toString(_mode) << "\n";
    std::cout << "  keep-alive: " << (_keep_alive ? "true" : "false") << "\n";
    std::cout << "  max simultaneous connections: " << _max_simultaneous_connections << "\n";
    std::cout << "  max pending connections: " << _max_pending_connections << "\n";
    std::cout << "Resource:\n";
    std::cout << "  cgi-bin: " << _cgi_bin_dir << "\n";
    std::cout << "  www: " << _root_dir << "\n";

    return HttpServer();
}

void HttpServerBuilder::saveConfigFile(std::string_view config_file) {
    std::cout << "Salvando arquivo de configuração: " << config_file << std::endl;

    std::cout << "Server:\n";
    std::cout << "  name: " << _name << "\n";
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
