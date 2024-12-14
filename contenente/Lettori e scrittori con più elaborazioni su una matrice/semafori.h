#ifndef _SEMAFORI_H_
#define _SEMAFORI_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// Funzioni per la gestione dei semafori
int Wait_Sem(int id_sem, int numsem);    // Operazione di wait sul semaforo
int Signal_Sem(int id_sem, int numsem);  // Operazione di signal sul semaforo

#endif // _SEMAFORI_H_