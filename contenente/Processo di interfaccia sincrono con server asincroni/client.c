#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg_struct.h"

int main() {
    int ret;

    // TBD: Apertura code messaggi
    int coda_request = msgget(ftok(".", 'a'), 0644);
    if(coda_request == -1) {
        perror("Errore apertura coda request");
        exit(1);
    }

    int coda_ok = msgget(ftok(".", 'b'), 0644);
    if(coda_ok == -1) {
        perror("Errore apertura coda ok");
        exit(1);
    }

    int coda_richieste = msgget(ftok(".", 'c'), 0644);
    if(coda_richieste == -1) {
        perror("Errore apertura coda richieste");
        exit(1);
    }

    // TBD: Preparazione messaggi
    sync_msg msg_request;
    sync_msg msg_ok;
    request_msg msg_richiesta;

    pid_t pid = getpid();

    // TBD: Prima richiesta (sincrona)
    msg_request.type = REQUEST_TO_SEND;
    msg_request.pid_client = pid;
    ret = msgsnd(coda_request, &msg_request, sizeof(sync_msg) - sizeof(long), 0);
    if(ret == -1) {
        perror("Errore invio request-to-send");
        exit(1);
    }
    
    // TBD: Attesa OK
    ret = msgrcv(coda_ok, &msg_ok, sizeof(sync_msg) - sizeof(long), pid, 0);
    if(ret == -1) {
        perror("Errore ricezione ok-to-send");
        exit(1);
    }

    // TBD: Invio prima richiesta
    msg_richiesta.type = RICHIESTA;
    msg_richiesta.pid_client = pid;
    msg_richiesta.valore = rand() % 10;
    ret = msgsnd(coda_richieste, &msg_richiesta, sizeof(request_msg) - sizeof(long), 0);
    if(ret == -1) {
        perror("Errore invio prima richiesta");
        exit(1);
    }

    // TBD: Invio altre 4 richieste (asincrone)
    for(int i = 0; i < 4; i++) {
        msg_richiesta.type = RICHIESTA;
        msg_richiesta.pid_client = pid;
        msg_richiesta.valore = rand() % 10;
        ret = msgsnd(coda_richieste, &msg_richiesta, sizeof(request_msg) - sizeof(long), 0);
        if(ret == -1) {
            perror("Errore invio richiesta asincrona");
            exit(1);
        }
    }

    printf("[Client %d] Inviate tutte le richieste\n", pid);
    return 0;
}