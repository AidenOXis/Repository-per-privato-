/**
 * Programma ELABORATORE: legge e raddoppia i valori della matrice
 */

#include "inizializzatore.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include "semafori.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Errore: numero errato di argomenti\n");
        exit(1);
    }

    int shm_id = atoi(argv[1]);
    int sem_id = atoi(argv[2]);

    struct SharedData* shared_data = shmat(shm_id, NULL, 0);
    if(shared_data == (void*)-1) {
        perror("Errore attach memoria condivisa");
        exit(1);
    }

    printf("Elaboratore avviato\n");

    while (!shared_data->termina) {
        Wait_Sem(sem_id, MUTEX_LETTORI);
        shared_data->num_lettori++;
        if (shared_data->num_lettori == 1) {
            Wait_Sem(sem_id, MUTEX_SCRITTORE);
        }
        Signal_Sem(sem_id, MUTEX_LETTORI);

        printf("\nElaboratore - Matrice doppia:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%2d ", shared_data->matrix[i][j] * 2);
            }
            printf("\n");
        }
        printf("\n");

        Wait_Sem(sem_id, MUTEX_LETTORI);
        shared_data->num_lettori--;
        if (shared_data->num_lettori == 0) {
            Signal_Sem(sem_id, MUTEX_SCRITTORE);
        }
        Signal_Sem(sem_id, MUTEX_LETTORI);

        sleep(1);
    }

    shmdt(shared_data);
    printf("Elaboratore terminato\n");
    return 0;
}