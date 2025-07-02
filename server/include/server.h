#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "logger.h"

#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

typedef struct {
    int fd;
    logger_t* log_file;
} arg_t;

/**
 * @brief Starts the server
 * 
 * @param port_no (const int) The port to listen on
 * @param log_file (logger_t*) Log file struct
 * 
 * @return file descriptor of the server
 */
int start_server(const int, logger_t*);

/**
 * @brief handles client
 * 
 * @param arg (void*) a struct of all relavent arguments, typecasted to void*
 */
void* handle_client(void*);

/**
 * @brief Accepts connection and spawns threads
 * 
 * @param server_fd (const int) file descriptor of the server
 */
void accept_clients(const int, logger_t*);

#endif