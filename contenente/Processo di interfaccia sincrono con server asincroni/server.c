#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg_struct.h"

int main(int argc, char *argv[]) {
    int ret;

    if(argc != 2) {
        printf("Uso: %s <id_server>\n", argv[0]);
        exit(1);
    }

    // TBD: Recupero ID server
    int server_id = atoi(argv[1]);

    // TBD: Apertura coda messaggi del server
    int coda_server = msgget(ftok(".", 'd'+server_id), 0644);
    if(coda_server == -1) {
        perror("Errore apertura coda server");
        exit(1);
    }

    // TBD: Preparazione messaggi
    request_msg msg_richiesta;
    pid_t pid = getpid();
    int richieste_gestite = 0;
    int richieste_totali = (NUM_CLIENTS * RICHIESTE_PER_CLIENT) / NUM_SERVERS;

    printf("[Server %d] Avvio elaborazione richieste\n", server_id);

    // TBD: Ciclo principale
    while(richieste_gestite < richieste_totali) {
        // Ricezione richiesta
        ret = msgrcv(coda_server, &msg_richiesta, sizeof(request_msg) - sizeof(long), RICHIESTA, 0);
        if(ret == -1) {
            perror("Errore ricezione richiesta");
            exit(1);
        }

        // Elaborazione e stampa
        printf("[Server %d] Ricevuta richiesta da client %d con valore %d\n", 
               server_id, msg_richiesta.pid_client, msg_richiesta.valore);
        
        // Simula elaborazione
        sleep(1);
        
        richieste_gestite++;
        printf("[Server %d] Gestite %d richieste su %d\n", 
               server_id, richieste_gestite, richieste_totali);
    }

    printf("[Server %d] Terminazione normale\n", server_id);
    return 0;
}