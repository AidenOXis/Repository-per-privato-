#ifndef HEADER_H
#define HEADER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_SCRITTORI 4
#define NUM_LETTORI 2
#define NUM_ITERAZIONI 5

// TBD: Definire struttura thread_args per passaggio parametri
struct  thread_args
{
    struct monitor* m; 
    int id; //id dello scrittore
};

// TBD: Definire struttura monitor con:
// - buffer condivisi
// - mutex e condition variables
// - contatori per gestione lettori/scrittori
struct monitor {
    int buffer1; 
    int buffer2; 
    
    pthread_mutex_t mutex; 
    pthread_cond_t cv_scrittore; 
    pthread_cond_t cv_lettore; 

    int num_lettori; //lettori attivi
    int num_scrittori; //scrittori attivi
    int num_cv_lettori; //lettori in attesa
    int num_cv_scrittori; //scrittori in attesa
    

};

// Prototipi delle funzioni
void inizializza(struct monitor* m);
void rimuovi(struct monitor* m);
void* scrittore(void* arg);
void* lettore_media(void* arg);
void* lettore_max(void* arg);

#endif