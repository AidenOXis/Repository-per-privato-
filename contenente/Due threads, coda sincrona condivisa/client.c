#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include "msg_struct.h"



int main() {
    int coda_request = msgget(ftok(".", 'a'), 0664);
    int coda_ok = msgget(ftok(".", 'b'), 0664);
    int coda_messaggi = msgget(ftok(".", 'c'), 0664);

    if(coda_request < 0 || coda_ok < 0 || coda_messaggi < 0) {
        perror("Errore apertura code");
        exit(1);
    }

    pid_t pid = getpid();
    srand(time(NULL) ^ pid);

    for(int i = 0; i < 4; i++) {
        // Invio request-to-send
        sync_msg request;
        request.type = 1;  // Tipo fisso per request
        request.pid_client = pid;

        if(msgsnd(coda_request, &request, sizeof(sync_msg) - sizeof(long), 0) < 0) {
            perror("Errore invio request");
            exit(1);
        }

        // Attesa ok-to-send
        sync_msg_ok ok;
        if(msgrcv(coda_ok, &ok, sizeof(sync_msg_ok) - sizeof(long), OK_TO_SEND, 0) < 0) {
            perror("Errore ricezione ok");
            exit(1);
        }

        // Invio messaggio con i valori
        message_t msg;
        msg.type = pid;
        msg.pid_client = pid;
        msg.val1 = rand() % 11;
        msg.val2 = rand() % 11;

        if(msgsnd(coda_messaggi, &msg, sizeof(message_t) - sizeof(long), 0) < 0) {
            perror("Errore invio messaggio");
            exit(1);
        }

        printf("[Client %d] Inviato messaggio %d: val1=%d, val2=%d\n", pid, i+1, msg.val1, msg.val2);
    }

    printf("[Client %d] Terminato\n", pid);
    return 0;
}