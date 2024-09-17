#include "socket_server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <format>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <csignal>
#include <sys/wait.h>

void SocketServer::start() {
    int server_fd = createServerSocket();
    std::cout << std::format("Server running on http://localhost:{}", _port) << std::endl;

    switch (_mode) {
        case HttpServerMode::Type::SYNC:
            runOnBlockingMode(server_fd);
            break;
        case HttpServerMode::Type::FORK:
            runOnForkMode(server_fd);
            break;
        case HttpServerMode::Type::THREAD:
            runOnThreadMode(server_fd);
            break;
        case HttpServerMode::Type::ASYNC:
            runOnNonBlockingMode(server_fd);
            break;
    }
}

int SocketServer::createServerSocket() const {
    int server_fd;
    struct sockaddr_in address{};
    int opt = 1;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == -1 ||
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1 ||
        bind(server_fd, (struct sockaddr *) &address, sizeof(address)) == -1 ||
        listen(server_fd, 3) == -1) {

        close(server_fd);
        throw std::runtime_error(strerror(errno));
    }

    return server_fd;
}

std::string SocketServer::readRequest(int fd) {
    int available = 0;

#ifdef POLLING
    int n = 0;
        do {
            std::cout << "Tentativa " << ++n << "..." << std::endl;
            if (ioctl(fd, FIONREAD, &available) == -1) {
                perror("ioctl");
                return "";
            }
            if (available == 0) usleep(500);
        } while (available == 0);
#else

    fd_set read_fd;
    FD_ZERO(&read_fd); // Limpa o conjunto de descritores
    FD_SET(fd, &read_fd);

    int ready = select(fd + 1, &read_fd, nullptr, nullptr, nullptr);
    if (ready == -1) {
        perror("select"); // Erro ao chamar select
        return "";
    }
    if (ready == 0) {
        std::cout << "Timeout: Nenhum dado disponível para leitura." << std::endl;
        return "";
    }

    if (ioctl(fd, FIONREAD, &available) == -1) {
        perror("ioctl");
        return "";
    }
#endif

    std::string buffer(available + 1, '\0');
    read(fd, buffer.data(), available);
    return buffer;
}

void SocketServer::sendResponse(int fd, std::string_view response) {
    send(fd, response.data(), response.size(), 0);
}

void SocketServer::runOnBlockingMode(int server_fd) {
    while (true) {
        struct sockaddr_in address{};
        socklen_t address_len = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr *) &address, &address_len);
        if (new_socket == -1) {
            perror("Erro ao aceitar conexão");
            continue;
        }

        printf("Conexão aceita de %s:%d | PID [%d] | socket [%d]\n", inet_ntoa(address.sin_addr),
               ntohs(address.sin_port),
               getpid(), new_socket);

        std::string request = readRequest(new_socket);
        sendResponse(new_socket, request);
        close(new_socket);
    }
}

void handle_sigchld(int sig) {
    (void) sig;
    pid_t pid;
    while ((pid = waitpid(-1, nullptr, WNOHANG)) > 0) {
        std::cout << "Seu filho << " << pid << " foi de X" << std::endl;
    }
    std::cout << "Menos uma pensão a pagar!!!" << std::endl;
}

void SocketServer::runOnForkMode(int server_fd) {
    struct sigaction sa{};
    sa.sa_handler = handle_sigchld;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCHLD, &sa, nullptr);

    while (true) {
        sockaddr_in address{};
        socklen_t address_len = sizeof(address);
        printf("Processo [%d] pronto na escuta de novas conexões...\n", getpid());
        int new_socket = accept(server_fd, (struct sockaddr *) &address, &address_len);
        if (new_socket == -1) {
            perror("Erro ao aceitar conexão");
            continue;
        }

        printf("Conexão aceita de %s:%d | PID [%d] | socket [%d]\n", inet_ntoa(address.sin_addr),
               ntohs(address.sin_port),
               getpid(), new_socket);

        pid_t pid = fork();
        if (pid == -1) {
            perror("Erro ao criar processo filho");
            close(new_socket);
        } else if (pid != 0) {
            close(new_socket);
        } else {
            close(server_fd);
            std::string request = readRequest(new_socket);
            sendResponse(new_socket, request);
            close(new_socket);
            exit(EXIT_SUCCESS);
        }
    }
}

void SocketServer::runOnThreadMode(int server_fd) {

}

void SocketServer::runOnNonBlockingMode(int server_fd) {

}
