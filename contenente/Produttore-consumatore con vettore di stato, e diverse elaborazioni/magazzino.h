#ifndef MAGAZZINO_H
#define MAGAZZINO_H

#include <pthread.h>

/* TBD: Definire le costanti necessarie */
#define DIM 5 
#define LIBERO 0 
#define IN_USO 1
#define OCCUPATO 2

/* TBD: Definire la struttura prodotto */
typedef struct {
    int tipo;    // tipo (1 elettrodomestico, 2 arredo)
    int costo;   // costo
} prodotto;

/* TBD: Definire la struttura MonitorMagazzino */
typedef struct {
    prodotto vettore[DIM];  // vettore di prodotti
    int stato[DIM];         // array degli stati
    pthread_mutex_t mutex;  // mutex
    pthread_cond_t spazio_disponibile;  // condition variables
    pthread_cond_t prod_disponibile; 
    int num_liberi;     // contatore posizioni libere
    int num_occupati;   // contatore posizioni occupate
} MonitorMagazzino;

/* TBD: Dichiarare i prototipi delle funzioni */
void inizializza_magazzino(MonitorMagazzino* m);
void rimuovi_magazzino(MonitorMagazzino* m);
void inserisci_elettrodomestico(MonitorMagazzino* m, int costo);
void inserisci_arredo(MonitorMagazzino* m, int costo);
prodotto preleva_prodotto(MonitorMagazzino* m);
prodotto preleva_prodotto_scontato(MonitorMagazzino* m);

#endif