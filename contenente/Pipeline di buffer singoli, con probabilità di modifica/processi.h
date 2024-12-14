#ifndef PROCESSI_H
#define PROCESSI_H

#include "buffer.h"

void produttore(int sem_id, struct shared_memory* shm, int id_prod);
void consumatore(int sem_id, struct shared_memory* shm);
void iniettore(int sem_id, struct shared_memory* shm);

#endif