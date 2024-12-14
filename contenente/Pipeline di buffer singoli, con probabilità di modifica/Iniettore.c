#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>  // Per la funzione sleep
#include "buffer.h"
#include "semafori.h"

void iniettore(int sem_id, struct shared_memory* shm) {
    srand(time(NULL));
    int messaggi_processati = 0;
    const int TOTALE_MESSAGGI = 9;  // 3 produttori × 3 messaggi

    while(messaggi_processati < TOTALE_MESSAGGI) {
        Wait_Sem(sem_id, 1);  // Wait su MESSAGGIO_DISPONIBILE
        
        int buffer_index = trova_buffer_occupato(shm);
        if(buffer_index >= 0) {
            int prob = rand() % 100;
            
            if(prob < 50) {
                printf("Iniettore: nessuna modifica al messaggio\n");
            } else if(prob < 80) {
                shm->buffer[buffer_index].intero = rand() % 100;
                printf("Iniettore: modificato campo intero\n");
            } else {
                int pos = rand() % strlen(shm->buffer[buffer_index].stringa);
                shm->buffer[buffer_index].stringa[pos] = '\0';
                printf("Iniettore: inserito terminatore in posizione %d\n", pos);
            }
            
            Signal_Sem(sem_id, 1);  // Signal su MESSAGGIO_DISPONIBILE
            messaggi_processati++;
        }
    }
    printf("Iniettore: Terminato dopo aver processato %d messaggi.\n", TOTALE_MESSAGGI);
}