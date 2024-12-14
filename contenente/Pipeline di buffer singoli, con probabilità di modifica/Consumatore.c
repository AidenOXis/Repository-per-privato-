#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include "buffer.h"
#include "semafori.h"

void consumatore(int sem_id, struct shared_memory* shm) {
    int messaggi_letti = 0;
    const int TOTALE_MESSAGGI = 9;  // 3 produttori × 3 messaggi

    while(messaggi_letti < TOTALE_MESSAGGI) {
        Wait_Sem(sem_id, 1);  // Wait su MESSAGGIO_DISPONIBILE
        
        int buffer_index = trova_buffer_occupato(shm);
        if(buffer_index >= 0) {
            messaggio msg = leggi_messaggio(shm, buffer_index);
            
            if(strlen(msg.stringa) == msg.intero) {
                printf("Consumatore: Messaggio valido letto: %s\n", msg.stringa);
            } else {
                printf("Consumatore: Errore di integrità nel messaggio\n");
            }
            
            Signal_Sem(sem_id, 0);  // Signal su SPAZIO_DISPONIBILE
            messaggi_letti++;
        }
    }
    printf("Consumatore: Terminato dopo aver letto %d messaggi.\n", TOTALE_MESSAGGI);
}