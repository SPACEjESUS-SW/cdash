# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Wextra -I. -Iclient/include -Iserver/include

# Directory structure
CLIENT_SRC = client/src
CLIENT_INC = client/include
SERVER_SRC = server/src
SERVER_INC = server/include
BIN_DIR = bin

# Object files in bin/
CLIENT_OBJS = $(BIN_DIR)/main_client.o $(BIN_DIR)/client.o
SERVER_OBJS = $(BIN_DIR)/main_server.o $(BIN_DIR)/server.o $(BIN_DIR)/logger.o

# Executable targets
CLIENT_TARGET = $(BIN_DIR)/client_app
SERVER_TARGET = $(BIN_DIR)/server_app

# Default target
all: client server

# Build client
client: $(CLIENT_TARGET)

# Build server
server: $(SERVER_TARGET)

# Build client binary
$(CLIENT_TARGET): $(BIN_DIR) $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS)

# Build server binary
$(SERVER_TARGET): $(BIN_DIR) $(SERVER_OBJS)
	$(CC) -o $@ $(SERVER_OBJS)

# Create bin directory if needed
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compilation rules
$(BIN_DIR)/main_client.o: $(CLIENT_SRC)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/client.o: $(CLIENT_SRC)/client.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/main_server.o: $(SERVER_SRC)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/server.o: $(SERVER_SRC)/server.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/logger.o: $(SERVER_SRC)/logger.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BIN_DIR)
