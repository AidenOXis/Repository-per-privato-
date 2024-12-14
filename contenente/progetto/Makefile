all: main

main: procedure.o main.o
	gcc -pthread procedure.o main.o -o main

procedure.o: procedure.c
	gcc -pthread -c procedure.c

main.o: main.c
	gcc -pthread -c main.c

clean:
	rm -f *.o main