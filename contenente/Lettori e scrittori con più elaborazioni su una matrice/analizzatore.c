/**
 * Programma ANALIZZATORE: calcola la media dei valori nella matrice
 */

#include "inizializzatore.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include "semafori.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Errore: numero errato di argomenti\n");
        exit(1);
    }

    int shm_id = atoi(argv[1]);
    int sem_id = atoi(argv[2]);
    int id = atoi(argv[3]);

    struct SharedData* shared_data = shmat(shm_id, NULL, 0);
    if(shared_data == (void*)-1) {
        perror("Errore attach memoria condivisa");
        exit(1);
    }

    printf("Analizzatore %d avviato\n", id);

    while (!shared_data->termina) {
        Wait_Sem(sem_id, MUTEX_LETTORI);
        shared_data->num_lettori++;
        if (shared_data->num_lettori == 1) {
            Wait_Sem(sem_id, MUTEX_SCRITTORE);
        }
        Signal_Sem(sem_id, MUTEX_LETTORI);

        int somma = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                somma += shared_data->matrix[i][j];
            }
        }
        printf("Analizzatore %d: Media = %.2f\n", id, (float)somma/(N*N));

        Wait_Sem(sem_id, MUTEX_LETTORI);
        shared_data->num_lettori--;
        if (shared_data->num_lettori == 0) {
            Signal_Sem(sem_id, MUTEX_SCRITTORE);
        }
        Signal_Sem(sem_id, MUTEX_LETTORI);

        sleep(2);
    }

    shmdt(shared_data);
    printf("Analizzatore %d terminato\n", id);
    return 0;
}