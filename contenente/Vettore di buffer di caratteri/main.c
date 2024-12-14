#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "buffer.h"

void* produttore(void* arg) {
    BufChar* b = (BufChar*)arg;
    // TBD:
    // 1. Implementa un ciclo che produce 3 volte
        // 2. Ad ogni iterazione:
    //    - Prepara una stringa di caratteri
    //    - Chiamata a produci()
    //    - Aggiungi sleep(1)
    for(int i = 0 ; i<3; i++){
        char caratteri[] = "ABC"; 
        printf("Produttore: inserisco %s\n", caratteri);
        produci(b,caratteri,3); 
        sleep(1);
    }
    return NULL;
}

void* consumatore(void* arg) {
    BufChar* b = (BufChar*)arg;
    // TBD:
    // 1. Implementa un ciclo che consuma 2 volte
    // 2. Ad ogni iterazione:
    //    - Prepara un buffer per ricevere i caratteri
    //    - Chiamata a consuma()
    //    - Stampa i caratteri letti
    //    - Aggiungi sleep(2)
    for(int i = 0 ; i<2;i++){
        char caratteri[DIM]; 
        int letti = consuma(b,caratteri); 
        caratteri[letti]='\0'; 
        printf("Consumatore: ho letto %s\n", caratteri);
        sleep(2); 
    }
    return NULL;
}

int main() {
    BufChar b;
    pthread_t prod_thread, cons_thread;
    
    // TBD:
    // 1. Inizializza la struttura buffer
    // 2. Crea thread produttore
    // 3. Crea thread consumatore
    // 4. Attendi terminazione dei thread
    // 5. Distruggi la struttura buffer
    inizializza(&b); 
    pthread_create(&prod_thread,NULL,produttore,&b);
    pthread_create(&cons_thread,NULL,consumatore,&b); 

    pthread_join(prod_thread,NULL); 
    pthread_join(cons_thread,NULL); 
    distruggi(&b); 
    
    return 0;
}