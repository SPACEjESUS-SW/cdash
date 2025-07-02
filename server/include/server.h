#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

/**
 * @brief Starts the server
 * 
 * @param port_no (const int) The port to listen on
 * 
 * @return file descriptor of the server
 */
int start_server(const int);

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
void accept_clients(const int);

#endif