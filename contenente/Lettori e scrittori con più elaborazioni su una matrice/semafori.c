#include <stdio.h>
#include <errno.h>
#include "semafori.h"

// Implementazione della Wait sul semaforo
int Wait_Sem(int id_sem, int numsem) {
    int err;
    struct sembuf sem_buf;

    // Prepara struttura sembuf
    sem_buf.sem_num = numsem;  // Numero del semaforo
    sem_buf.sem_flg = 0;       // Flag (0 per operazione bloccante)
    sem_buf.sem_op = -1;       // Operazione di decremento (wait)

    // Esegue operazione sul semaforo
    err = semop(id_sem, &sem_buf, 1);   // semaforo rosso

    if(err < 0) {
        perror("Errore WAIT");
    }

    return err;
}

// Implementazione della Signal sul semaforo
int Signal_Sem(int id_sem, int numsem) {
    int err;
    struct sembuf sem_buf;

    // Prepara struttura sembuf
    sem_buf.sem_num = numsem;  // Numero del semaforo
    sem_buf.sem_flg = 0;       // Flag (0 per operazione bloccante)
    sem_buf.sem_op = 1;        // Operazione di incremento (signal)

    // Esegue operazione sul semaforo
    err = semop(id_sem, &sem_buf, 1);   // semaforo verde

    if(err < 0) {
        perror("Errore SIGNAL");
    }

    return err;
}