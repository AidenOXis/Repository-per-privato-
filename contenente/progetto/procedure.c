#include "header.h"

void inizializza(BufferLS* b) {
    // TBD: Inizializza tutte le variabili a 0
    b->buffer = 0;
    b->num_lettori = 0;
    b->num_scrittori = 0;
    b->num_cv_lettori = 0;
    b->num_cv_scrittori = 0;
    
    // TBD: Inizializza mutex e condition variables
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cv_lettori, NULL);
    pthread_cond_init(&b->cv_scrittori, NULL);
}

void rimuovi(BufferLS* b) {
    // TBD: Dealloca mutex e condition variables
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->cv_lettori);
    pthread_cond_destroy(&b->cv_scrittori);
}

void* thread1(void* arg) {
    BufferLS* b = (BufferLS*)arg;
    
    for(int i = 0; i < ITER_THREAD1; i++) {
        // TBD: Genera valore casuale da scrivere
        int valore = rand() % 10;
        
        pthread_mutex_lock(&b->mutex);
        
        // TBD: Attendi se ci sono lettori o scrittori attivi
        // num_cv_scrittori dentro il while -> possibile starvation
        while(b->num_lettori > 0 || b->num_scrittori > 0) {
            b->num_cv_scrittori++;
            pthread_cond_wait(&b->cv_scrittori, &b->mutex);
            b->num_cv_scrittori--;
        }
        
        // TBD: Scrivi il valore
        b->num_scrittori++;
        b->buffer = valore;
        printf("Thread 1: scritto %d\n", valore);
        b->num_scrittori--;
        
        // TBD: Sveglia tutti i lettori in attesa
        pthread_cond_broadcast(&b->cv_lettori);
        
        pthread_mutex_unlock(&b->mutex);
        sleep(3);
    }
    return NULL;
}

void* thread2_3(void* arg) {
    BufferLS* b = (BufferLS*)arg;
    
    for(int i = 0; i < ITER_THREAD2_3; i++) {
        int val_letto;
        
        // LETTURA
        pthread_mutex_lock(&b->mutex);
        
        // TBD: num_cv_lettori fuori dal while -> priorità ai lettori
        b->num_cv_lettori++;
        while(b->num_scrittori > 0) {
            pthread_cond_wait(&b->cv_lettori, &b->mutex);
        }
        b->num_cv_lettori--;
        
        // TBD: Leggi il valore
        b->num_lettori++;
        val_letto = b->buffer;
        printf("Thread 2-3: letto %d\n", val_letto);
        b->num_lettori--;
        
        // TBD: Se sono l'ultimo lettore, sveglia uno scrittore
        if(b->num_lettori == 0 && b->num_cv_scrittori > 0) {
            pthread_cond_signal(&b->cv_scrittori);
        }
        
        pthread_mutex_unlock(&b->mutex);
        sleep(2);
        
        // SCRITTURA
        pthread_mutex_lock(&b->mutex);
        
        // TBD: Attendi se ci sono lettori o scrittori attivi
        // num_cv_scrittori dentro il while -> possibile starvation
        while(b->num_lettori > 0 || b->num_scrittori > 0) {
            b->num_cv_scrittori++;
            pthread_cond_wait(&b->cv_scrittori, &b->mutex);
            b->num_cv_scrittori--;
        }
        
        // TBD: Scrivi il valore letto
        b->num_scrittori++;
        b->buffer = val_letto;
        printf("Thread 2-3: scritto %d\n", val_letto);
        b->num_scrittori--;
        
        // TBD: Sveglia tutti i lettori in attesa
        pthread_cond_broadcast(&b->cv_lettori);
        
        pthread_mutex_unlock(&b->mutex);
        sleep(2);
    }
    return NULL;
}

void* thread4_7(void* arg) {
    BufferLS* b = (BufferLS*)arg;
    
    for(int i = 0; i < ITER_THREAD4_7; i++) {
        pthread_mutex_lock(&b->mutex);
        
        // TBD: num_cv_lettori fuori dal while -> priorità ai lettori
        b->num_cv_lettori++;
        while(b->num_scrittori > 0) {
            pthread_cond_wait(&b->cv_lettori, &b->mutex);
        }
        b->num_cv_lettori--;
        
        // TBD: Leggi il valore
        b->num_lettori++;
        printf("Thread 4-7: letto %d\n", b->buffer);
        b->num_lettori--;
        
        // TBD: Se sono l'ultimo lettore, sveglia uno scrittore
        if(b->num_lettori == 0 && b->num_cv_scrittori > 0) {
            pthread_cond_signal(&b->cv_scrittori);
        }
        
        pthread_mutex_unlock(&b->mutex);
        sleep(1);
    }
    return NULL;
}