#ifndef HTTP_SERVER_HTTP_SERVER_MODE_H
#define HTTP_SERVER_HTTP_SERVER_MODE_H

#include <string>
#include <ranges>
#include <iostream>


class HttpServerMode {
public:
    enum class Type {
        SYNC,
        FORK,
        THREAD,
        ASYNC
    };

    static std::string toString(Type mode) {
        switch (mode) {
            case Type::SYNC:
                return "sync";
            case Type::FORK:
                return "fork";
            case Type::THREAD:
                return "thread";
            case Type::ASYNC:
                return "async";
        }
    }

    static Type toEnum(std::string_view mode) {
        std::ranges::transform_view(mode, [](unsigned char c) { return std::tolower(c); });

        if (mode == "sync") return Type::SYNC;
        if (mode == "fork") return Type::FORK;
        if (mode == "thread") return Type::THREAD;
        if (mode == "async") return Type::ASYNC;

        throw std::invalid_argument("invalid mode");
    }
};


#endif //HTTP_SERVER_HTTP_SERVER_MODE_H
