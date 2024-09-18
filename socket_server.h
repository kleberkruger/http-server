#ifndef HTTP_SERVER_SOCKET_SERVER_H
#define HTTP_SERVER_SOCKET_SERVER_H

#include <cstdint>
#include "http_server_mode.h"

class SocketServer {
public:

    void start();

protected:

    SocketServer(uint16_t port, HttpServerMode::Type mode,
                 uint32_t max_simultaneous_connections, uint32_t max_pending_connections, bool keep_alive,
                 std::string_view log_file, bool verbose) : _port(port),
                                                            _mode(mode),
                                                            _max_simultaneous_connections(max_simultaneous_connections),
                                                            _max_pending_connections(max_pending_connections),
                                                            _keep_alive(keep_alive),
                                                            _log_file(log_file),
                                                            _verbose(verbose) {}

    [[nodiscard]] int createServerSocket() const;

    static std::string readRequest(int fd);

    static void sendResponse(int fd, std::string_view content);

    [[noreturn]] static void runOnBlockingMode(int server_fd);

    void runOnForkMode(int server_fd);

    void runOnThreadMode(int server_fd);

    void runOnNonBlockingMode(int server_fd);

    uint16_t _port;
    HttpServerMode::Type _mode;
    uint32_t _max_simultaneous_connections;
    uint32_t _max_pending_connections;
    bool _keep_alive;
    std::string_view _log_file;
    bool _verbose;
};


#endif //HTTP_SERVER_SOCKET_SERVER_H
