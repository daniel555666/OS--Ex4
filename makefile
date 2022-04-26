.PHONY: all run clean
G++ = gcc
FLAGS= 
HEADERS = 
all: server client

server: server.o
	$(G++) $< -o server -lpthread

client: client.o
	$(G++) $< -o client

%.o: %.cpp
	$(G++) -c $< -o $@

clean:
	rm -f *.o server client