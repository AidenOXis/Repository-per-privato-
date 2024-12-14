#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "msg_struct.h"



int coda_request;
int coda_ok;
int coda_messaggi;

void* server_thread(void* arg) {
    int thread_id = *(int*)arg;
    int msg_elaborati = 0;
    sync_msg request;
    sync_msg_ok ok;
    message_t msg;

    printf("[Thread %d] Avviato\n", thread_id);

    while(msg_elaborati < MSG_PER_THREAD) {
        // Ricezione request-to-send
        if(msgrcv(coda_request, &request, sizeof(sync_msg) - sizeof(long), 1, 0) < 0) {
            perror("Errore ricezione request");
            pthread_exit(NULL);
        }

        printf("[Thread %d] Ricevuta request da client %d\n", thread_id, request.pid_client);

        // Invio ok-to-send
        ok.type = OK_TO_SEND;
        ok.pid_client = request.pid_client;
        if(msgsnd(coda_ok, &ok, sizeof(sync_msg_ok) - sizeof(long), 0) < 0) {
            perror("Errore invio ok");
            pthread_exit(NULL);
        }

        // Ricezione messaggio
        if(msgrcv(coda_messaggi, &msg, sizeof(message_t) - sizeof(long), request.pid_client, 0) < 0) {
            perror("Errore ricezione messaggio");
            pthread_exit(NULL);
        }

        printf("[Thread %d] Ricevuto da client %d: val1=%d, val2=%d, somma=%d\n",
               thread_id, msg.pid_client, msg.val1, msg.val2, msg.val1 + msg.val2);

        msg_elaborati++;
    }

    printf("[Thread %d] Terminato dopo %d messaggi\n", thread_id, msg_elaborati);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[2];
    int thread_ids[2] = {0, 1};

    // Apertura code
    coda_request = msgget(ftok(".", 'a'), 0664);
    coda_ok = msgget(ftok(".", 'b'), 0664);
    coda_messaggi = msgget(ftok(".", 'c'), 0664);

    if(coda_request < 0 || coda_ok < 0 || coda_messaggi < 0) {
        perror("Errore apertura code");
        exit(1);
    }

    printf("[Server] Avvio thread...\n");

    // Creazione thread
    for(int i = 0; i < 2; i++) {
        if(pthread_create(&threads[i], NULL, server_thread, &thread_ids[i]) != 0) {
            perror("Errore creazione thread");
            exit(1);
        }
    }

    // Attesa terminazione thread
    for(int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("[Server] Terminazione normale\n");
    return 0;
}