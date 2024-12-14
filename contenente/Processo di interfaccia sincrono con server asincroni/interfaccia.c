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
        printf("Uso: %s <num_server>\n", argv[0]);
        exit(1);
    }

    // TBD: Apertura code messaggi
    int coda_request = msgget(ftok(".", 'a'), 0644);
    if(coda_request == -1) {
        perror("Errore apertura coda request");
        exit(1);
    }

    int coda_richieste = msgget(ftok(".", 'c'), 0644);
    if(coda_richieste == -1) {
        perror("Errore apertura coda richieste");
        exit(1);
    }

    int coda_ok = msgget(ftok(".", 'b'), 0644);
    if(coda_ok == -1) {
        perror("Errore apertura coda ok");
        exit(1);
    }

    // TBD: Apertura code server
    int num_server = atoi(argv[1]);
    int code_server[num_server];
    for(int i = 0; i < num_server; i++) {
        code_server[i] = msgget(ftok(".", 'd'+i), 0644);
        if(code_server[i] == -1) {
            perror("Errore apertura coda server");
            exit(1);
        }
    }

    // TBD: Preparazione messaggi
    sync_msg msg_request;
    sync_msg msg_ok;
    request_msg msg_richiesta;
    int current_server = 0;  // Per round-robin
    int clients_serviti = 0;

    printf("[Interfaccia] Avvio elaborazione richieste\n");

    // TBD: Ciclo principale
    while(clients_serviti < NUM_CLIENTS) {
        // Attesa request-to-send sincrona
        ret = msgrcv(coda_request, &msg_request, sizeof(sync_msg) - sizeof(long), REQUEST_TO_SEND, 0);
        if(ret == -1) {
            perror("Errore ricezione request-to-send");
            exit(1);
        }

        // Invio ok-to-send
        msg_ok.type = msg_request.pid_client;
        msg_ok.pid_client = msg_request.pid_client;
        ret = msgsnd(coda_ok, &msg_ok, sizeof(sync_msg) - sizeof(long), 0);
        if(ret == -1) {
            perror("Errore invio ok-to-send");
            exit(1);
        }

        // Gestione 5 richieste del client
        for(int i = 0; i < RICHIESTE_PER_CLIENT; i++) {
            // Ricezione richiesta
            ret = msgrcv(coda_richieste, &msg_richiesta, sizeof(request_msg) - sizeof(long), RICHIESTA, 0);
            if(ret == -1) {
                perror("Errore ricezione richiesta");
                exit(1);
            }

            // Inoltro al server (round-robin)
            msg_richiesta.type = RICHIESTA;
            ret = msgsnd(code_server[current_server], &msg_richiesta, sizeof(request_msg) - sizeof(long), 0);
            if(ret == -1) {
                perror("Errore inoltro richiesta a server");
                exit(1);
            }

            printf("[Interfaccia] Richiesta da client %d inoltrata a server %d\n", 
                   msg_richiesta.pid_client, current_server);

            // Aggiornamento round-robin
            current_server = (current_server + 1) % num_server;
        }
        
        clients_serviti++;
        printf("[Interfaccia] Serviti %d client su %d\n", clients_serviti, NUM_CLIENTS);
    }

    printf("[Interfaccia] Terminazione normale\n");
    return 0;
}