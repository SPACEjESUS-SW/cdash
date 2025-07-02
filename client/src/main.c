#include "client.h"

#define EXIT_MSG "-exit-"

int sockfd = -1;            // global for signal handler
char *buffer = NULL;
volatile sig_atomic_t shutdown_requested = 0;

void sigint_handler(int signum) {
    (void)signum;  // suppress unused warning
    shutdown_requested = 1;
    printf("\n[INFO] SIGINT received. Disconnecting from server...\n");
    if (buffer) free(buffer);
    if (sockfd != -1) teardown(sockfd);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("USAGE: %s <ip_addr> <port_no>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* ip_addr = argv[1];
    int port_no = atoi(argv[2]);

    if (strcmp(ip_addr, "localhost") == 0) {
        ip_addr = "127.0.0.1";
    }

    // Register SIGINT handler
    signal(SIGINT, sigint_handler);

    sockfd = connect_to_server(ip_addr, port_no);
    if (sockfd < 0) {
        fprintf(stderr, "Connection failed. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    int running = 1;
    size_t buffer_size = 0;

    while (running && !shutdown_requested) {
        printf("\n>> ");
        ssize_t chars = getline(&buffer, &buffer_size, stdin);
        if (chars < 0) {
            perror("Failed to read line");
            break;
        }

        // Strip newline if present
        if (buffer[chars - 1] == '\n') {
            buffer[chars - 1] = '\0';
            chars--;
        }

        if (send_message(buffer, chars, sockfd) < 0) {
            perror("Failed to send message");
            break;
        }

        if (strcmp(buffer, EXIT_MSG) == 0) {
            running = 0;
        }
    }

    free(buffer);
    teardown(sockfd);
    return 0;
}
