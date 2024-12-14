#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include "buffer.h"
#include "semafori.h"

void produttore(int sem_id, struct shared_memory* shm, int id_prod) {
    // Prepara il messaggio base
    char msg[256] = "MSG";
    // Converti l'ID in carattere aggiungendo '0' (in ASCII)
    msg[3] = id_prod + '0';  // Converte id_prod (1,2,3) in carattere ('1','2','3')
    msg[4] = '\0';           // Terminatore stringa
    
    // Ogni produttore produce 3 messaggi
    for(int i = 0; i < 3; i++) {
        // Attende che ci sia spazio disponibile
        Wait_Sem(sem_id, SPAZIO_DISPONIBILE);  // Wait su SPAZIO_DISPONIBILE
        
        // Trova un buffer libero e scrive il messaggio
        int buffer_index = trova_buffer_libero(shm);
        scrivi_messaggio(shm, buffer_index, msg, 4);  // 4 è la lunghezza della stringa
        
        printf("Produttore %d: scritto messaggio %s nel buffer %d\n", 
               id_prod, msg, buffer_index);
        
        // Segnala che c'è un nuovo messaggio disponibile
        Signal_Sem(sem_id, MESSAGGIO_DISPONIBILE);  // Signal su MESSAGGIO_DISPONIBILE
        
        sleep(1);  // Pausa tra una produzione e l'altra
    }
}