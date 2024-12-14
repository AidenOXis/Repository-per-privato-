#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Definizione costanti
#define NUM_THREADS 7
#define ITER_THREAD1 3
#define ITER_THREAD2_3 5
#define ITER_THREAD4_7 10

// Struttura dati BufferLS
typedef struct {
    int buffer;
    pthread_mutex_t mutex;
    pthread_cond_t cv_lettori;
    pthread_cond_t cv_scrittori;
    int num_lettori;
    int num_scrittori;
    int num_cv_lettori;
    int num_cv_scrittori;
} BufferLS;

// Dichiarazione funzioni
void inizializza(BufferLS* b);
void rimuovi(BufferLS* b);
void* thread1(void* arg);
void* thread2_3(void* arg);
void* thread4_7(void* arg);

#endif