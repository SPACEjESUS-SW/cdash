#include "client.h"

#define EXIT_MSG "-exit-"

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

    // Connect to the server
    int sockfd = connect_to_server(ip_addr, port_no);
    if (sockfd < 0) {
        fprintf(stderr, "Connection failed. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    int running = 1;
    char *buffer = NULL;
    size_t buffer_size = 0;

    while (running) {
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
