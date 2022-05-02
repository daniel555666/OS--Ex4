.PHONY: all run clean
CC = gcc
FLAGS= 
HEADERS = 
all: server client

server: server.o
	$(CC) $< -o server -lpthread

client: client.o
	$(CC) $< -o client

%.o: %.cpp
	$(CC) -c $< -o $@

clean:
	rm -f *.o server client