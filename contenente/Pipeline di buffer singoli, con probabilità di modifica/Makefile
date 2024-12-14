all: main

main: main.o buffer.o semafori.o Produttore.o Consumatore.o Iniettore.o
	gcc -o main main.o buffer.o semafori.o Produttore.o Consumatore.o Iniettore.o

main.o: main.c buffer.h semafori.h
	gcc -c -o main.o main.c

Produttore.o: Produttore.c buffer.h semafori.h
	gcc -c -o Produttore.o Produttore.c

Consumatore.o: Consumatore.c buffer.h semafori.h
	gcc -c -o Consumatore.o Consumatore.c

Iniettore.o: Iniettore.c buffer.h semafori.h
	gcc -c -o Iniettore.o Iniettore.c

buffer.o: buffer.c buffer.h
	gcc -c -o buffer.o buffer.c

semafori.o: semafori.c semafori.h
	gcc -c -o semafori.o semafori.c

clean:
	rm -f *.o
	rm -f main