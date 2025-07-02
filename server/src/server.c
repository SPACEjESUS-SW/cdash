#include "server.h"

int start_server(const int port_no) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creating failed");
        return -1;
    }

    int opt = 1;
    struct sockaddr_in server_addr;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    server_addr.sin_port = htons(port_no);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        return -1;
    }

    printf("Server listening on port %d\n", port_no);
    return server_fd;
}

void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(client_fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Recieved: %s\n", buffer);
    }

    return NULL;
}

void accept_clients(int server_fd) {
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));

        *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        if (*client_fd < 0) {
            perror("Accept failed");
            free(client_fd);
            continue;
        }

        printf("Accepted connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, client_fd) != 0) {
            perror("Failed to create thread");
            close(*client_fd);
            free(client_fd);
        } else {
            pthread_detach(tid); // Optional: no need to join
        }
    }
}
