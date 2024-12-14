all: main generatore elaboratore analizzatore

main: main.o inizializzatore.o semafori.o
	gcc -o main main.o inizializzatore.o semafori.o

generatore: generatore.o semafori.o
	gcc -o generatore generatore.o semafori.o

elaboratore: elaboratore.o semafori.o
	gcc -o elaboratore elaboratore.o semafori.o

analizzatore: analizzatore.o semafori.o
	gcc -o analizzatore analizzatore.o semafori.o

main.o: main.c inizializzatore.h semafori.h
	gcc -c -o main.o main.c

inizializzatore.o: inizializzatore.c inizializzatore.h semafori.h
	gcc -c -o inizializzatore.o inizializzatore.c

generatore.o: generatore.c inizializzatore.h semafori.h
	gcc -c -o generatore.o generatore.c

elaboratore.o: elaboratore.c inizializzatore.h semafori.h
	gcc -c -o elaboratore.o elaboratore.c

analizzatore.o: analizzatore.c inizializzatore.h semafori.h
	gcc -c -o analizzatore.o analizzatore.c

semafori.o: semafori.c semafori.h
	gcc -c -o semafori.o semafori.c

clean:
	rm -f *.o
	rm -f main generatore elaboratore analizzatore