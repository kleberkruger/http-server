#include <iostream>
#include <filesystem>
#include <argparse/argparse.hpp>
#include "http_server.h"
#include "http_server_config.h"
#include "http_server_builder.h"

//class HttpServerApp {
//public:
//
//    explicit HttpServerApp() : program("HTTP Server", HttpServer::VERSION) {
//        configureArgs();
//    }
//
//    virtual ~HttpServerApp() = default;
//
//    int start(int argc, char *argv[]) {
//        try {
//            program.parse_args(argc, argv);
//        }
//        catch (const std::exception &err) {
//            std::cerr << err.what() << std::endl;
//            std::cerr << program;
//            exit(EXIT_FAILURE);
//        }
//
//        return EXIT_SUCCESS;
//    }
//
//private:
//    argparse::ArgumentParser program;
//
//    void configureArgs() {
//        program.add_argument("-c", "--config")
//                .help("sets a config file")
//                .default_value("../config/httpserver.yaml")
//                .required();
//
//        auto &group = program.add_mutually_exclusive_group();
//        group.add_argument("-s", "--sync")
//                .help("runs the server on sync mode")
//                .nargs(0, 1)
//                .scan<'i', uint32_t>();
//        group.add_argument("-f", "--fork")
//                .help("runs the server on fork mode")
//                .nargs(0, 2)
//                .scan<'i', uint32_t>();
//        group.add_argument("-t", "--thread")
//                .help("runs the server on thread mode")
//                .nargs(0, 2)
//                .scan<'i', uint32_t>();
//        group.add_argument("-a", "--async")
//                .help("runs the server on async mode")
//                .nargs(0, 2)
//                .scan<'i', uint32_t>();
//
//        program.add_argument("--name")
//                .help("sets server name");
//
//        program.add_argument("-p", "--port")
//                .help("sets port")
//                .scan<'i', uint32_t>();
//
//        program.add_argument("--home")
//                .help("sets home address");
//
//        program.add_argument("--path")
//                .help("sets the path of <root> <cgi-bin> directories")
//                .nargs(1, 2);
//
//        program.add_argument("--log")
//                .help("sets a log file")
//                .nargs(argparse::nargs_pattern::optional);
//
//        program.add_argument("--no-keep-alive")
//                .help("disables keep-alive connections")
//                .flag();
//
//        program.add_argument("--verbose")
//                .help("shows output verbosity")
//                .flag();
//    }
//};

void add_args(argparse::ArgumentParser &program) {
    program.add_argument("-c", "--config")
            .help("sets a config file")
            .default_value("../config/httpserver.yaml")
            .required();

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
            .nargs(argparse::nargs_pattern::optional);

    program.add_argument("--no-keep-alive")
            .help("disables keep-alive connections")
            .flag();

    program.add_argument("--verbose")
            .help("shows output verbosity")
            .flag();
}

void update_args(argparse::ArgumentParser &program, HttpServerConfig &config) {
    std::string_view mode = program.is_used("sync") ? "sync" :
                            program.is_used("fork") ? "fork" :
                            program.is_used("thread") ? "thread" :
                            program.is_used("async") ? "async" : "";

    if (mode == "sync") {
        auto args = program.get<std::vector<uint32_t>>("sync");
        config.setMode(HttpServerMode::Type::SYNC);
        if (!args.empty()) {
            config.setMaxSimultaneousConnections(args[0]);
        }
        std::cout << "Modo Síncrono com " << config.getMaxPendingConnections() << " conexões pendentes" << std::endl;

    } else if (mode == "fork" || mode == "thread" || mode == "async") {
        HttpServerMode::Type modeType = HttpServerMode::toEnum(mode);
        config.setMode(modeType);
        auto args = program.get<std::vector<uint32_t>>(mode);
        if (!args.empty()) {
            config.setMaxSimultaneousConnections(args[0]);
            if (args.size() > 1) {
                config.setMaxPendingConnections(args[1]);
            }
        }
        std::cout << "Modo " << mode << " com " << config.getMaxSimultaneousConnections() << " conexões simultâneas e "
                  << config.getMaxPendingConnections() << " pendentes" << std::endl;
    }

    if (program.is_used("name")) {
        config.setName(program.get("name"));
    }
    if (program.is_used("port")) {
        config.setPort(program.get<uint16_t>("port"));
    }
    if (program.is_used("home")) {
        config.setHomepage(program.get("home"));
    }
    if (program.is_used("path")) {
        auto args = program.get<std::vector<std::string>>(mode);
        if (!args.empty()) {
            config.setRootDir(args[0]);
            if (args.size() > 1) {
                config.setCgiBinDir(args[1]);
            }
        }
    }
    if (program.is_used("no-keep-alive")) {
        config.setKeepAlive(false);
    }
}

int main(int argc, char *argv[]) {
////    return HttpServerApp().start(argc, argv);
//
//    argparse::ArgumentParser program("HTTP Server", HttpServer::VERSION);
//    add_args(program);
//
//    try {
//        program.parse_args(argc, argv);
//    }
//    catch (const std::exception &err) {
//        std::cerr << err.what() << std::endl;
//        std::cerr << program;
//
//        return EXIT_FAILURE;
//    }
//
//    auto config_file = program.get("config");
////    if (std::filesystem::exists(config_file)) {
////        std::cout << "Carregando arquivo de configuração..." << std::endl;
////        HttpServerConfig config(config_file);
////        std::cout << "Atualizando configuração..." << std::endl;
////        update_args(program, config);
////        config.print();
////    } else {
////        HttpServerConfig config;
////        update_args(program, config);
////        std::cout << "Gravar este arquivo de configurações em " << config_file << std::endl;
////        config.print();
////    }
//
//    HttpServerConfig config(std::filesystem::exists(config_file) ? config_file : "");
//    update_args(program, config);
//    config.print();
//
//    if (!std::filesystem::exists(config_file)) {
//        std::cout << "Gravar este arquivo de configurações em " << config_file << std::endl;
//    }
//
//    std::string log_file;
//    if (program.is_used("log")) {
//        log_file = program.get("log").empty() ? "log.txt" : program.get("log");
//    }
//    bool is_verbose = program.is_used("verbose");

    HttpServer server = HttpServerBuilder()
            .byCommandLineArgs(argc, argv)
            .build();

    server.start();

    return EXIT_SUCCESS;
}
