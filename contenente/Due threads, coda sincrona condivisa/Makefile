all: server client main

server: server.c msg_struct.h
	gcc server.c -o server -pthread

client: client.c msg_struct.h
	gcc client.c -o client

main: main.c msg_struct.h
	gcc main.c -o main

clean:
	rm -f server client main