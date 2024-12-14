#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>  // Per pid_t
#include <sys/wait.h>   // Per wait()
#include <unistd.h>
#include "buffer.h"
#include "semafori.h"
#include "processi.h"

int main() {
    key_t chiave_shm = ftok(".", 'k');
    key_t chiave_sem = ftok(".", 's');
    
    // Creazione memoria condivisa
    int shm_id = shmget(chiave_shm, sizeof(struct shared_memory), IPC_CREAT | 0664);
    if (shm_id < 0) {
        perror("Errore creazione shared memory");
        exit(1);
    }
    
    // Attach della memoria condivisa
    struct shared_memory* shm = (struct shared_memory*)shmat(shm_id, NULL, 0);
    inizializza_buffer(shm);
    
    // Creazione semafori
    int sem_id = semget(chiave_sem, 2, IPC_CREAT | 0664);
    if (sem_id < 0) {
        perror("Errore creazione semafori");
        exit(1);
    }

    // Inizializzazione semafori
    semctl(sem_id, SPAZIO_DISPONIBILE, SETVAL, 2);  // SPAZIO_DISPONIBILE = 2
    semctl(sem_id, MESSAGGIO_DISPONIBILE, SETVAL, 0);  // MESSAGGIO_DISPONIBILE = 0
    
    // Creazione processi
    pid_t pid;
    
    // Creazione 3 produttori
    for(int i = 0; i < 3; i++) {
        pid = fork();
        if(pid == 0) {
            produttore(sem_id, shm, i+1);
            exit(0);
        }
    }
    
    // Creazione iniettore
    pid = fork();
    if(pid == 0) {
        iniettore(sem_id, shm);
        exit(0);
    }
    
    // Creazione consumatore
    pid = fork();
    if(pid == 0) {
        consumatore(sem_id, shm);
        exit(0);
    }
    
    // Attesa terminazione processi figli
    for(int i = 0; i < 5; i++) {
        wait(NULL);
    }
    
    // Pulizia risorse
    shmdt(shm);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    
    printf("Programma terminato con successo.\n");
    return 0;
}