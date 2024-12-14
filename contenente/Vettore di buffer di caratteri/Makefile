all: main

main: buffer.o main.o
	gcc -pthread buffer.o main.o -o main

buffer.o: buffer.c
	gcc -pthread -c buffer.c

main.o: main.c
	gcc -pthread -c main.c

clean:
	rm -f *.o main