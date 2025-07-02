#ifndef CLIENT_H
#define CLIENT_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * @brief Connects to the server at the given IP address through the given port number
 * 
 * @param ip_addr (const char*) IP address of the server
 * 
 * @param port_no (const int) port number to use
 * 
 * @return (int) file descriptor of the socket connection, -1 on failure
 */
int connect_to_server(const char*, const int);

/**
 * @brief Sends the message to the server through the socket connection
 * 
 * @param message (const char*) A string of message
 * 
 * @param length (const size_t) length of the message
 * 
 * @param sockfd (const int) file descriptor of the socket connection
 * 
 * @return -1 on failure, 0 on success 
 */
int send_message(const char*, const size_t, const int);

/**
 * @brief Gracefully disconnects from the server
 * 
 * @param sockfd (const int) file descriptor of the socket connection
 */
void teardown(const int);

#endif