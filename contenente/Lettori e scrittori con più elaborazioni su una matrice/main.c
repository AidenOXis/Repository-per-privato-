/**
 * Programma principale: gestisce l'avvio e la terminazione dei processi
 */

#include "inizializzatore.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include "semafori.h"

// Funzione di supporto per convertire int in stringa
void int_to_string(int num, char* str) {
    int i = 0;
    int temp = num;
    
    // Gestisce il caso dello 0
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Converte ogni cifra
    while (temp > 0) {
        str[i++] = (temp % 10) + '0';
        temp = temp / 10;
    }
    str[i] = '\0';
    
    // Inverte la stringa
    int j;
    for (j = 0; j < i/2; j++) {
        char t = str[j];
        str[j] = str[i-1-j];
        str[i-1-j] = t;
    }
}

int main() {
    // STEP 1: Inizializzazione automatica delle risorse
    struct SharedIDs ids = inizializza();

    // STEP 2: Attach alla memoria condivisa
    struct SharedData* shared_data = (struct SharedData*)shmat(ids.shm_id, NULL, 0);
    if(shared_data == (void*)-1) {
        perror("Errore attach memoria condivisa");
        exit(1);
    }

    // STEP 3: Creazione processo GENERATORE
    if (fork() == 0) {
        char arg1[20], arg2[20];
        int_to_string(ids.shm_id, arg1);
        int_to_string(ids.sem_id, arg2);
        
        execl("./generatore", "generatore", arg1, arg2, NULL);
        perror("Errore execl generatore");
        exit(1);
    }

    // STEP 4: Creazione processo ELABORATORE
    if (fork() == 0) {
        char arg1[20], arg2[20];
        int_to_string(ids.shm_id, arg1);
        int_to_string(ids.sem_id, arg2);
        
        execl("./elaboratore", "elaboratore", arg1, arg2, NULL);
        perror("Errore execl elaboratore");
        exit(1);
    }

    // STEP 5: Creazione dei due processi ANALIZZATORE
    int i;
    for (i = 1; i <= 2; i++) {
        if (fork() == 0) {
            char arg1[20], arg2[20], arg3[5];
            int_to_string(ids.shm_id, arg1);
            int_to_string(ids.sem_id, arg2);
            int_to_string(i, arg3);
            
            execl("./analizzatore", "analizzatore", arg1, arg2, arg3, NULL);
            perror("Errore execl analizzatore");
            exit(1);
        }
    }

    // STEP 6: Attesa di 15 secondi
    sleep(15);

    // STEP 7: Avvio della terminazione
    shared_data->termina = 1;

    // STEP 8: Attesa terminazione di tutti i processi figli
    for (i = 0; i < 4; i++) {
        wait(NULL);
    }

    // STEP 9: Pulizia delle risorse
    shmdt(shared_data);
    shmctl(ids.shm_id, IPC_RMID, NULL);
    semctl(ids.sem_id, 0, IPC_RMID);

    return 0;
}