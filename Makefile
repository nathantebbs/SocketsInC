CC     = gcc
CFLAGS = -Wall -Wextra -g

# Directories
SERVER_DIR = server
CLIENT_DIR = client

# Targets
SERVER = $(SERVER_DIR)/server
CLIENT = $(CLIENT_DIR)/client

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $<

$(CLIENT): $(CLIENT_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(SERVER) $(CLIENT)
