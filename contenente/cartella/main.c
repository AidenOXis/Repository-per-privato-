#include "header.h"
#include <time.h>

int main() {
    // TBD: Inizializzare generatore numeri casuali
    srand(time(NULL)); 

    // TBD: Creare e inizializzare monitor
    struct monitor* m=malloc(sizeof(struct monitor)); 
    inizializza(m);
    pthread_t scrittori[NUM_SCRITTORI]; 
    pthread_t lettori[NUM_LETTORI]; 

     // Creazione attributi thread
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    // TBD: Creare thread scrittori con argomenti appropriati
    for(int i=0; i<NUM_SCRITTORI;i++){
        struct thread_args* args = malloc(sizeof(struct thread_args));
        args->m = m;
        args->id = i;
        pthread_create(&scrittori[i], &attr, scrittore, args);
    }
    // TBD: Creare thread lettori
    pthread_create(&lettori[0], &attr, lettore_media, m);
    pthread_create(&lettori[1], &attr, lettore_max, m);
    
    // TBD: Join dei thread
    for(int i = 0; i < NUM_SCRITTORI; i++) {
        pthread_join(scrittori[i], NULL);
    }
    for(int i = 0; i < NUM_LETTORI; i++) {
        pthread_join(lettori[i], NULL);
    }
    
    // TBD: Liberare risorse
    rimuovi(m); 
    free(m);
    return 0;
}