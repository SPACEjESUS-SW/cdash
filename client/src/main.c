#include "client.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>   // for usleep
#include <stdio.h>
#include <stdlib.h>

#define EXIT_MSG "-exit-"

int sockfd = -1;            // global for signal handler
char *buffer = NULL;
volatile sig_atomic_t shutdown_requested = 0;

void sigint_handler(int signum) {
    (void)signum;
    shutdown_requested = 1;
    printf("\n[INFO] SIGINT received. Disconnecting from server...\n");
    if (buffer) free(buffer);
    if (sockfd != -1) teardown(sockfd);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("USAGE: %s <ip_addr> <port_no> <bombard 0|1>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* ip_addr = argv[1];
    int port_no = atoi(argv[2]);
    int mode = atoi(argv[3]);

    if (strcmp(ip_addr, "localhost") == 0) {
        ip_addr = "127.0.0.1";
    }

    // Register SIGINT handler for graceful termination
    signal(SIGINT, sigint_handler);

    sockfd = connect_to_server(ip_addr, port_no);
    if (sockfd < 0) {
        fprintf(stderr, "Connection failed. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    if (mode == 0) {
        // Interactive mode
        int running = 1;
        size_t buffer_size = 0;

        while (running && !shutdown_requested) {
            printf("\n>> ");
            ssize_t chars = getline(&buffer, &buffer_size, stdin);
            if (chars < 0) {
                perror("Failed to read line");
                break;
            }

            // Strip newline
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
    } else {
        // Bombard mode
        const char* msg = "hi message";
        size_t msg_len = strlen(msg);

        printf("[INFO] Bombarding server with messages every 1ms...\n");

        while (!shutdown_requested) {
            if (send_message(msg, msg_len, sockfd) < 0) {
                perror("Failed to send message during bombard");
                break;
            }
            usleep(1000); // 1ms = 1000 microseconds
        }
    }

    free(buffer);
    teardown(sockfd);
    return 0;
}
