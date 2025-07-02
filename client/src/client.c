#include "client.h"

int connect_to_server(const char* ip_addr, const int port_no) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creating failed.");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);

    if (inet_pton(AF_INET, ip_addr, &server_addr.sin_addr) <= 0) {
        perror("Invalid Address");
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int send_message(const char* message, const size_t length, const int sockfd) {
    ssize_t bytes_sent = send(sockfd, message, length, 0);
    if (bytes_sent < 0) {
        perror("Send failed");
        return -1;
    }

    return 0;
}

void teardown(const int sockfd) {
    if (sockfd >= 0)
        close(sockfd);
}