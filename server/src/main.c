#include "server.h"

logger_t logger;

volatile sig_atomic_t shutdown_requested = 0;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("USAGE: %s <port_no>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (init_logger("server.log", &logger) != 0) {
        perror("Failed to initialize logger.");
        exit(EXIT_FAILURE);
    }

    handle_signal(&logger);

    int port = atoi(argv[1]);
    int server_fd = start_server(port, &logger);

    if (server_fd < 0) {
        log_message(CRITICAL, "Server setup failed", &logger);
        close_logger(&logger);
        return 1;
    }

    accept_clients(server_fd, &logger);

    close(server_fd);
    log_message(INFO, "Server socket closed", &logger);

    close_logger(&logger);
    log_message(INFO, "Logger closed. Shutdown complete.", &logger);

    return 0;
}
