all: main

main: magazzino.o main.o
	gcc -pthread magazzino.o main.o -o main

magazzino.o: magazzino.c
	gcc -pthread -c magazzino.c

main.o: main.c
	gcc -pthread -c main.c

clean:
	rm -f *.o main