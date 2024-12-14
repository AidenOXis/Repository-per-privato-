#include "header.h"

int main() {
    BufferLS buffer;
    pthread_t threads[NUM_THREADS];
    
    // TBD: Inizializzare buffer
    inizializza(&buffer);
    
    // TBD: Creare thread
    pthread_create(&threads[0], NULL, thread1, (void*)&buffer);
    for(int i = 1; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread2_3, (void*)&buffer);
    }
    for(int i = 3; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread4_7, (void*)&buffer);
    }
    
    // TBD: Join thread
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // TBD: Deallocare buffer
    rimuovi(&buffer);
    
    return 0;
}