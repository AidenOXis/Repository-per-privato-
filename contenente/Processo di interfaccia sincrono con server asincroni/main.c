#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg_struct.h"

int main() {
    pid_t pid;

    // TBD: Creazione code messaggi
    int coda_request = msgget(ftok(".", 'a'), IPC_CREAT | 0644);
    int coda_ok = msgget(ftok(".", 'b'), IPC_CREAT | 0644);
    int coda_richieste = msgget(ftok(".", 'c'), IPC_CREAT | 0644);
    
    // TBD: Array code server
    int code_server[NUM_SERVERS];
    for(int i = 0; i < NUM_SERVERS; i++) {
        code_server[i] = msgget(ftok(".", 'd'+i), IPC_CREAT | 0644);
    }

    // TBD: Creazione interfaccia
    pid = fork();
    if(pid == 0) {
        char num_server_str[10];
        sprintf(num_server_str, "%d", NUM_SERVERS);
        execl("./interfaccia", "interfaccia", num_server_str, NULL);
        exit(1);
    }

    // TBD: Creazione server
    for(int i = 0; i < NUM_SERVERS; i++) {
        pid = fork();
        if(pid == 0) {
            char server_id[10];
            sprintf(server_id, "%d", i);
            execl("./server", "server", server_id, NULL);
            exit(1);
        }
    }

    // TBD: Creazione client
    for(int i = 0; i < NUM_CLIENTS; i++) {
        pid = fork();
        if(pid == 0) {
            execl("./client", "client", NULL);
            exit(1);
        }
    }

    // TBD: Attesa terminazione
    for(int i = 0; i < (NUM_CLIENTS + NUM_SERVERS + 1); i++) {
        wait(NULL);
    }

    printf("[Main] Pulizia code messaggi...\n");

    // TBD: Rimozione code
    msgctl(coda_request, IPC_RMID, NULL);
    msgctl(coda_ok, IPC_RMID, NULL);
    msgctl(coda_richieste, IPC_RMID, NULL);
    for(int i = 0; i < NUM_SERVERS; i++) {
        msgctl(code_server[i], IPC_RMID, NULL);
    }

    printf("[Main] Terminazione normale\n");
    return 0;
}