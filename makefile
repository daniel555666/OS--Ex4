.PHONY: all run clean
CC = gcc
FLAGS= 
HEADERS = 
all: server client test

server: server.o
	$(CC) $< -o server -lpthread

client: client.o
	$(CC) $< -o client
test: autoClient test.o
	$(CC) test.o -o test
autoClient: autoClient.o
	$(CC) $< -o autoClient
%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o server client test autoClient