#include "inizializzatore.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "semafori.h"

struct SharedIDs inizializza() {
    struct SharedIDs ids;
    
    // Creazione memoria condivisa
    ids.shm_id = shmget(IPC_PRIVATE, sizeof(struct SharedData), IPC_CREAT | 0666);
    if(ids.shm_id < 0) {
        perror("Errore creazione memoria condivisa");
        exit(1);
    }

    // Attach memoria condivisa
    struct SharedData* shared_data = shmat(ids.shm_id, NULL, 0);
    if(shared_data == (void*)-1) {
        perror("Errore attach memoria condivisa");
        exit(1);
    }

    // Inizializzazione struttura
    shared_data->num_lettori = 0;
    shared_data->termina = 0;
    
    // Inizializzazione matrice a 0
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            shared_data->matrix[i][j] = 0;
        }
    }

    // Creazione semafori
    ids.sem_id = semget(IPC_PRIVATE, SYNCH, IPC_CREAT | 0666);
    if(ids.sem_id < 0) {
        perror("Errore creazione semafori");
        exit(1);
    }

    // Inizializzazione semafori
    semctl(ids.sem_id, MUTEX_LETTORI, SETVAL, 1);
    semctl(ids.sem_id, MUTEX_SCRITTORE, SETVAL, 1);

    // Detach
    shmdt(shared_data);

    return ids;
}