#include "server.h"

logger_t logger;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("USAGE: %s <port_no>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (init_logger("server.log", &logger) != 0) {
        perror("Failed to initialize logger.");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    int server_fd = start_server(port, &logger);

    if (server_fd < 0) {
        fprintf(stderr, "Server setup failed\n");
        return 1;
    }

    accept_clients(server_fd, &logger);

    close(server_fd);
    return 0;
}
