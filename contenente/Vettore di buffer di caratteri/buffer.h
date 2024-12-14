#include <pthread.h>
// Definizione della struttura del buffer
#define DIM 10 
typedef struct {
    char buffer[DIM];     // vettore di buffer, tipo "char" 
    int occupati;        // numero di caratteri occupati
    // TBD: Aggiungi qui le variabili per la sincronizzazione (mutex e condition variables)
    pthread_mutex_t mutex; 
    pthread_cond_t cv_prod; 
    pthread_cond_t cv_cons; 
} BufChar;

// Prototipi delle funzioni
void inizializza(BufChar* b);
void produci(BufChar* b, char* caratteri, int num_char);
int consuma(BufChar* b, char* caratteri);
void distruggi(BufChar* b);