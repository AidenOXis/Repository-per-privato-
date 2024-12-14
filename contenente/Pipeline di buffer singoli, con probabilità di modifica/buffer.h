#ifndef BUFFER_H
#define BUFFER_H



#define SPAZIO_DISPONIBILE 0
#define MESSAGGIO_DISPONIBILE 1

// Struttura del messaggio
typedef struct {
    char stringa[256];
    int intero;
} messaggio;

// Struttura del buffer condiviso
struct shared_memory {
    messaggio buffer[2];
    int stato_buffer[2];  // 0: libero, 1: occupato
};

// Funzioni per la gestione del buffer
void inizializza_buffer(struct shared_memory* shm);
int trova_buffer_libero(struct shared_memory* shm);
int trova_buffer_occupato(struct shared_memory* shm);
void scrivi_messaggio(struct shared_memory* shm, int buffer_index, const char* stringa, int intero);
messaggio leggi_messaggio(struct shared_memory* shm, int buffer_index);

#endif