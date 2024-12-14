#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg_struct.h"

int main() {
    // Creazione code di messaggi
    int coda_request = msgget(ftok(".", 'a'), IPC_CREAT | 0664);
    int coda_ok = msgget(ftok(".", 'b'), IPC_CREAT | 0664);
    int coda_messaggi = msgget(ftok(".", 'c'), IPC_CREAT | 0664);
    
    if(coda_request < 0 || coda_ok < 0 || coda_messaggi < 0) {
        perror("Errore creazione code");
        exit(1);
    }

    // Avvio server (una sola volta)
    pid_t pid_server = fork();
    if(pid_server < 0) {
        perror("Errore fork server");
        exit(1);
    }
    if(pid_server == 0) {
        execl("./server", "server", NULL);
        perror("Errore execl server");
        exit(1);
    }

    // Breve pausa per assicurarsi che il server sia pronto
    sleep(1);

    // Avvio dei 3 client
    for(int i = 0; i < NUM_CLIENTS; i++) {
        pid_t pid_client = fork();
        if(pid_client < 0) {
            perror("Errore fork client");
            exit(1);
        }
        if(pid_client == 0) {
            execl("./client", "client", NULL);
            perror("Errore execl client");
            exit(1);
        }
    }

    // Attesa terminazione di tutti i processi
    for(int i = 0; i < NUM_CLIENTS + 1; i++) {
        wait(NULL);
    }

    // Rimozione code
    msgctl(coda_request, IPC_RMID, NULL);
    msgctl(coda_ok, IPC_RMID, NULL);
    msgctl(coda_messaggi, IPC_RMID, NULL);

    printf("[Main] Terminazione normale\n");
    return 0;
}