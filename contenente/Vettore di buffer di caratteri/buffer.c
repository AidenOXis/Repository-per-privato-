
#include <stdio.h>
#include <string.h>
#include "buffer.h"

void inizializza(BufChar* b) {
    // TBD: 
    // 1. Inizializza occupati a 0
    // 2. Inizializza il mutex
    // 3. Inizializza le condition variables
    b->occupati = 0 ; 
    pthread_mutex_init(&b->mutex,NULL);
    pthread_cond_init(&b->cv_cons,NULL);
    pthread_cond_init(&b->cv_prod,NULL);
}

void produci(BufChar* b, char* caratteri, int num_char) {
    // TBD:
    // 1. Acquisisci il mutex
    // 2. Verifica se c'è spazio disponibile nel buffer
    //    - Se non c'è spazio, attendi sulla condition variable appropriata
    // 3. Copia i caratteri nel buffer
    // 4. Aggiorna il contatore occupati
    // 5. Segnala ai consumatori che ci sono nuovi dati
    // 6. Rilascia il mutex
    pthread_mutex_lock(&b->mutex);
    while(b->occupati + num_char > DIM){
        pthread_cond_wait(&b->cv_prod,&b->mutex);
    }
    strncpy(b->buffer + b->occupati, caratteri, num_char);
    b->occupati += num_char;

    pthread_cond_signal(&b->cv_cons);
    pthread_mutex_unlock(&b->mutex); 
}

int consuma(BufChar* b, char* caratteri) {
    // TBD:
    // 1. Acquisisci il mutex
    pthread_mutex_lock(&b->mutex); 
    // 2. Verifica se ci sono caratteri da leggere
    //    - Se non ci sono caratteri, attendi sulla condition variable appropriata
    while(b->occupati == 0 ){
        pthread_cond_wait(&b->cv_cons,&b->mutex); 
    }
    // 3. Copia i caratteri dal buffer
    strncpy(caratteri, b->buffer, b->occupati);
    
    // 4. Aggiorna il contatore occupati
    int chars_letti = b->occupati;
    b->occupati = 0;
    // 5. Segnala ai produttori che c'è nuovo spazio disponibile
    pthread_cond_signal(&b->cv_prod);
    // 6. Rilascia il mutex
    pthread_mutex_unlock(&b->mutex); 
    // 7. Ritorna il numero di caratteri letti
    return chars_letti; 
    return 0; // placeholder
}

void distruggi(BufChar* b) {
    // TBD:
    // 1. Distruggi il mutex
    // 2. Distruggi le condition variables
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->cv_cons);
    pthread_cond_destroy(&b->cv_prod);
}