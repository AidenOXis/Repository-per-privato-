#include "buffer.h"
#include <string.h>

void inizializza_buffer(struct shared_memory* shm) {
    shm->stato_buffer[0] = 0;
    shm->stato_buffer[1] = 0;
}

int trova_buffer_libero(struct shared_memory* shm) {
    for(int i = 0; i < 2; i++) {
        if(shm->stato_buffer[i] == 0) 
        return i;
    }
    return -1;
}

int trova_buffer_occupato(struct shared_memory* shm) {
    for(int i = 0; i < 2; i++) {
        if(shm->stato_buffer[i] == 1) 
        return i;
    }
    return -1;
}

void scrivi_messaggio(struct shared_memory* shm, int buffer_index, const char* stringa, int intero) {
    strcpy(shm->buffer[buffer_index].stringa, stringa);
    shm->buffer[buffer_index].intero = intero;
    shm->stato_buffer[buffer_index] = 1;
}

messaggio leggi_messaggio(struct shared_memory* shm, int buffer_index) {
    shm->stato_buffer[buffer_index] = 0;
    return shm->buffer[buffer_index];
}