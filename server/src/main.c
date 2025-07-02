#include "server.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("USAGE: %s <port_no>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);

    int server_fd = start_server(port);

    if (server_fd < 0) {
        fprintf(stderr, "Server setup failed\n");
        return 1;
    }

    accept_clients(server_fd);

    close(server_fd);
    return 0;
}
