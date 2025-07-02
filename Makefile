# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Wextra -I.

# Directory structure
CLIENT_SRC = client/src
CLIENT_INC = client/include
SERVER_SRC = server/src
SERVER_INC = server/include
BIN_DIR = bin

# Targets
CLIENT_TARGET = $(BIN_DIR)/client_app
SERVER_TARGET = $(BIN_DIR)/server_app

# Object files
CLIENT_OBJS = $(CLIENT_SRC)/main.o $(CLIENT_SRC)/client.o
SERVER_OBJS = $(SERVER_SRC)/main.o $(SERVER_SRC)/server.o

# Build client
client: CFLAGS += -I$(CLIENT_INC)
client: $(BIN_DIR) $(CLIENT_OBJS)
	$(CC) -o $(CLIENT_TARGET) $(CLIENT_OBJS)

# Build server
server: CFLAGS += -I$(SERVER_INC)
server: $(BIN_DIR) $(SERVER_OBJS)
	$(CC) -o $(SERVER_TARGET) $(SERVER_OBJS)

# Create bin directory if not exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Pattern rule for .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(CLIENT_OBJS) $(SERVER_OBJS)
	rm -rf $(BIN_DIR)
