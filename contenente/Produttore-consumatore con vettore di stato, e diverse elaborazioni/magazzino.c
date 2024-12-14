#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "magazzino.h"

/* TBD: Implementare inizializza_magazzino */
void inizializza_magazzino(MonitorMagazzino* m) {
    pthread_mutex_init(&m->mutex, NULL);       // Inizializzare mutex
    pthread_cond_init(&m->spazio_disponibile, NULL);  // Inizializzare condition variables
    pthread_cond_init(&m->prod_disponibile, NULL);
    
    m->num_liberi = DIM;    // Inizializzare contatori
    m->num_occupati = 0;
    
    for(int i = 0; i < DIM; i++) {            // Inizializzare stati del buffer a LIBERO
        m->stato[i] = LIBERO;
    }
}

/* TBD: Implementare rimuovi_magazzino */
void rimuovi_magazzino(MonitorMagazzino* m) {
    pthread_mutex_destroy(&m->mutex);          // Deallocare mutex
    pthread_cond_destroy(&m->spazio_disponibile);  // Deallocare condition variables
    pthread_cond_destroy(&m->prod_disponibile);
    free(m);
}

/* TBD: Implementare inserisci_elettrodomestico */
void inserisci_elettrodomestico(MonitorMagazzino* m, int costo) {
    int i = 0;
    pthread_mutex_lock(&m->mutex);            // Lock del mutex
    
    if(m->num_liberi == 0) {                 // Se buffer pieno, wait su condition
        pthread_cond_wait(&m->spazio_disponibile, &m->mutex);
    }
    
    while(m->stato[i] != LIBERO) {          // Cercare posizione LIBERA nel buffer
        i++;
    }
    
    m->stato[i] = IN_USO;                   // Settare stato IN_USO
    m->vettore[i].tipo = 1;                 // Inserire tipo e costo
    m->vettore[i].costo = costo;
    
    printf("Thread %lu: Inserito elettrodomestico in posizione %d con costo %d\n",
           pthread_self(), i, costo);
    
    m->stato[i] = OCCUPATO;                 // Settare stato OCCUPATO
    m->num_liberi--;                        // Aggiornare contatori
    m->num_occupati++;
    
    pthread_cond_signal(&m->prod_disponibile);  // Signal su condition
    pthread_mutex_unlock(&m->mutex);            // Unlock del mutex
}

/* TBD: Implementare inserisci_arredo */
void inserisci_arredo(MonitorMagazzino* m, int costo) {
    int i = 0;
    pthread_mutex_lock(&m->mutex);
    
    while(m->num_liberi == 0) {
        pthread_cond_wait(&m->spazio_disponibile, &m->mutex);
    }
    
    while(i<DIM && m->stato[i] != LIBERO) {
        i++;
    }
    
    m->stato[i] = IN_USO;
    m->vettore[i].tipo = 2;                 // Tipo 2 per arredo
    m->vettore[i].costo = costo;
    
    printf("Thread %lu: Inserito arredo in posizione %d con costo %d\n",
           pthread_self(), i, costo);
    
    m->stato[i] = OCCUPATO;
    m->num_liberi--;
    m->num_occupati++;
    
    pthread_cond_signal(&m->prod_disponibile);
    pthread_mutex_unlock(&m->mutex);
}

/* TBD: Implementare preleva_prodotto */
prodotto preleva_prodotto(MonitorMagazzino* m) {
    int i = 0;
    prodotto p;
    pthread_mutex_lock(&m->mutex);            // Lock del mutex
    
    while(m->num_occupati == 0) {               // Se buffer vuoto, wait su condition
        pthread_cond_wait(&m->prod_disponibile, &m->mutex);
    }
    
    while(i<DIM,m->stato[i] != OCCUPATO) {         // Cercare posizione OCCUPATA nel buffer
        i++;
    }
    
    m->stato[i] = IN_USO;                    // Settare stato IN_USO
    p = m->vettore[i];                       // Leggere prodotto
    
    printf("Thread %lu: Prelevato prodotto tipo %d da posizione %d con costo %d\n",
           pthread_self(), p.tipo, i, p.costo);
    
    m->stato[i] = LIBERO;                    // Settare stato LIBERO
    m->num_liberi++;                         // Aggiornare contatori
    m->num_occupati--;
    
    pthread_cond_signal(&m->spazio_disponibile);  // Signal su condition
    pthread_mutex_unlock(&m->mutex);              // Unlock del mutex
    return p;                                     // Return prodotto
}

/* TBD: Implementare preleva_prodotto_scontato */
prodotto preleva_prodotto_scontato(MonitorMagazzino* m) {
    int i = 0;
    prodotto p;
    pthread_mutex_lock(&m->mutex);
    
    while(m->num_occupati == 0) {
        pthread_cond_wait(&m->prod_disponibile, &m->mutex);
    }
    
    while(i<DIM, m->stato[i] != OCCUPATO) {
        i++;
    }
    
    m->stato[i] = IN_USO;
    p = m->vettore[i];
    p.costo = p.costo * 0.9;  // Applica sconto 10%
    
    printf("Thread %lu: Prelevato prodotto scontato tipo %d da posizione %d con costo %d\n",
           pthread_self(), p.tipo, i, p.costo);
    
    m->stato[i] = LIBERO;
    m->num_liberi++;
    m->num_occupati--;
    
    pthread_cond_signal(&m->spazio_disponibile);
    pthread_mutex_unlock(&m->mutex);
    return p;
}