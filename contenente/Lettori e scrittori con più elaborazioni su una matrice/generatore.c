

#include "inizializzatore.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include "semafori.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Errore: numero errato di argomenti\n");
        exit(1);
    }

    srand(time(NULL));
    
    int shm_id = atoi(argv[1]);
    int sem_id = atoi(argv[2]);

    struct SharedData* shared_data = shmat(shm_id, NULL, 0);
    if(shared_data == (void*)-1) {
        perror("Errore attach memoria condivisa");
        exit(1);
    }

    printf("Generatore avviato\n");

    while (!shared_data->termina) {
        Wait_Sem(sem_id, MUTEX_SCRITTORE);
        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                shared_data->matrix[i][j] = rand() % 10;
            }
        }
        
        printf("Generatore: Nuova matrice generata\n");
        Signal_Sem(sem_id, MUTEX_SCRITTORE);
        sleep(3);
    }

    shmdt(shared_data);
    printf("Generatore terminato\n");
    return 0;
}