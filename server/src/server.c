#include "server.h"

logger_t* global_logger = NULL;

// Actual signal handler function
static void sigint_handler(int sig) {
    (void)sig; // unused
    shutdown_requested = 1;
    if (global_logger) {
        log_message(INFO, "SIGINT received. Shutting down server...", global_logger);
    }
}

// Register signal handler and store logger reference
void handle_signal(logger_t* logger) {
    global_logger = logger;
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
}

int start_server(const int port_no, logger_t* logger) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
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

    char msg[128];
    snprintf(msg, sizeof(msg), "Server listening on port %d", port_no);
    log_message(INFO, msg, logger);

    return server_fd;
}

void* handle_client(void* arg) {
    arg_t* client_arg = (arg_t*)arg;
    int client_fd = client_arg->fd;
    logger_t* logger = client_arg->log_file;
    free(arg);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    pthread_t tid = pthread_self();
    char log_prefix[128];
    snprintf(log_prefix, sizeof(log_prefix),
             "Received a message | Thread ID: %lu | ",
             (unsigned long)tid);

    while ((bytes_read = read(client_fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';

        printf("Received: %s\n", buffer);

        char message[2048];
        snprintf(message, sizeof(message), "%s%d bytes: %s",
                 log_prefix, (int)bytes_read, buffer);

        log_message(INFO, message, logger);

        // Optionally echo back
        send(client_fd, buffer, bytes_read, 0);
    }

    close(client_fd);
    return NULL;
}

void accept_clients(int server_fd, logger_t* logger) {
    while (!shutdown_requested) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);

        if (shutdown_requested) {
            close(client_fd);
            break;
        }

        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        char msg[128];
        snprintf(msg, sizeof(msg), "Accepted connection from %s:%d",
                 inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        log_message(INFO, msg, logger);

        arg_t* args = malloc(sizeof(arg_t));
        args->fd = client_fd;
        args->log_file = logger;

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, args) != 0) {
            perror("Failed to create thread");
            close(client_fd);
            free(args);
        } else {
            pthread_detach(tid);
        }
    }

    log_message(INFO, "Stopped accepting clients.", logger);
}
