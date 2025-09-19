CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = server client
OBJS = payload.o

all: $(TARGETS)

server: server.c $(OBJS)
	$(CC) $(CFLAGS) server.c $(OBJS) -o server

client: client.c $(OBJS)
	$(CC) $(CFLAGS) client.c $(OBJS) -o client

payload.o: payload.c payload.h
	$(CC) $(CFLAGS) -c payload.c -o payload.o

clean:
	rm -f $(TARGETS) *.o
