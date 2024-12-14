#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "magazzino.h"

/* TBD: Definire costanti */
#define NUM_FORNITORI 2// Numero thread fornitori
#define NUM_NEGOZIANTI 1// Numero thread negozianti


/* TBD: Implementare funzione fornitore */
void* fornitore(void* arg) {
    // Cast del parametro
    MonitorMagazzino* m = (MonitorMagazzino*)arg; 
    
    // Per 3 volte:
    //   - Genera costo random
    //   - Inserisci elettrodomestico
    //   - Attendi 1 secondo
    for(int i = 0; i<3; i++){
        int costo = rand()%91 +10; 
        inserisci_elettrodomestico(m,costo);
        sleep(1);
    }
    
    // Per 2 volte:
    //   - Genera costo random
    //   - Inserisci arredo
    //   - Attendi 1 secondo
    for(int i = 0 ; i<2; i++){
        int costo = rand() % 91 + 10 ; 
        inserisci_arredo(m,costo); 
        sleep(1); 
    }
    
    return NULL;
}

/* TBD: Implementare funzione negoziante */
void* negoziante(void* arg) {
    // Cast del parametro
    MonitorMagazzino* m = (MonitorMagazzino*) arg; 
    
    // Per 5 volte:
    //   - Preleva prodotto normale
    for(int i = 0 ; i<5; i++){
        preleva_prodotto(m); 
    }
    
    // Per 5 volte:
    //   - Preleva prodotto scontato
    for(int i = 0 ; i<5; i++){
        preleva_prodotto_scontato(m); 
    }
    
    return NULL;
}

/* TBD: Implementare il main */
int main() {
    // Dichiarare variabili
    MonitorMagazzino magazzino; 
    pthread_t fornitori[NUM_FORNITORI]; 
    pthread_t negozianti[NUM_NEGOZIANTI];
    
    // Inizializzare generatore random
    srand(time(NULL)); 
    
    // Inizializzare magazzino
    inizializza_magazzino(&magazzino);
    
    // Creare thread fornitori
    for(int i = 0 ; i<NUM_FORNITORI; i++){
        pthread_create(&negozianti[i],NULL,fornitore,&magazzino); 
    }
    
    // Creare thread negozianti
        for(int i = 0; i < NUM_NEGOZIANTI; i++) {
        pthread_create(&negozianti[i], NULL, negoziante, &magazzino);
    }
    
    //attendi terminazione fornitoriS
    for(int i = 0; i < NUM_FORNITORI; i++) {
        pthread_join(fornitori[i], NULL);
    }
    
    // Attendi terminazione negozianti
    for(int i = 0; i < NUM_NEGOZIANTI; i++) {
        pthread_join(negozianti[i], NULL);
    }
    
    // Deallocare risorse
    rimuovi_magazzino(&magazzino); 
    
    return 0;
}