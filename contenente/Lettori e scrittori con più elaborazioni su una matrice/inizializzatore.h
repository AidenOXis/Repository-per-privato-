#ifndef _INIZIALIZZATORE_H_
#define _INIZIALIZZATORE_H_

// Struttura per passare gli ID delle risorse condivise
struct SharedIDs {
    int shm_id;   // ID della memoria condivisa
    int sem_id;   // ID dei semafori
};

// Funzione che crea e inizializza le risorse condivise
struct SharedIDs inizializza();
// Definizioni condivise
#define N 5               // Dimensione della matrice NxN
#define SYNCH 2          // Numero totale di semafori
#define MUTEX_LETTORI 0   // Semaforo per mutua esclusione tra lettori
#define MUTEX_SCRITTORE 1 // Semaforo per mutua esclusione lettori-scrittore

// Struttura dati condivisa
struct SharedData {
    int matrix[N][N];    // Matrice di numeri interi NxN
    int num_lettori;     // Contatore dei lettori attivi
    int termina;         // Flag per terminazione
};

#endif