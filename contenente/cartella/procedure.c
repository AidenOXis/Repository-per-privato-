#include "header.h"

void inizializza(struct monitor* m) {
    // TBD: Inizializzare:
    m->buffer1 = 0; 
    m->buffer2 = 0; // - buffer
    
    m->num_lettori = 0; 
    m->num_scrittori = 0; 
    m->num_cv_scrittori = 0; 
    m->num_cv_lettori = 0; // - contatori
    
    pthread_mutex_init(&m->mutex, NULL); // - mutex
    pthread_cond_init(&m->cv_lettore, NULL); 
    pthread_cond_init(&m->cv_scrittore, NULL); // - condition variables
}

void rimuovi(struct monitor* m) {
    // TBD: Deallocare:
    pthread_mutex_destroy(&m->mutex); // - mutex
    pthread_cond_destroy(&m->cv_lettore);
    pthread_cond_destroy(&m->cv_scrittore); // - condition variables
}

void* scrittore(void* arg) {
    // TBD: Cast argomenti
    struct thread_args* args = (struct thread_args*)arg; 
    struct monitor* m = args->m; 
    int id = args->id;
    
    // TBD: Determinare quale buffer usare in base all'ID
    int buffer_id = (id < 2) ? 1 : 2;  // scrittori 0,1 -> buffer1, scrittori 2,3 -> buffer2
    
    for(int i = 0; i < NUM_ITERAZIONI; i++) {
        // TBD: Generare valore casuale tra 10-20
        int valore = rand() % 11 + 10;  // genera numero casuale tra 10 e 20
        
        // TBD: Gestire sincronizzazione pre-scrittura
        pthread_mutex_lock(&m->mutex);  // acquisizione mutex
        
        m->num_cv_scrittori++;  // incrementa prima del while
        while(m->num_lettori > 0 || m->num_scrittori > 0) {  // attendi se ci sono lettori o scrittori attivi
            pthread_cond_wait(&m->cv_scrittore, &m->mutex); 
        }
        m->num_cv_scrittori--;  // decrementa dopo il while
        
        m->num_scrittori++;  // incrementa contatore scrittori attivi
        
        // TBD: Scrittura nel buffer appropriato
        if(buffer_id == 1) {
            m->buffer1 = valore; 
        } else {
            m->buffer2 = valore; 
        }
        printf("Scrittore %d: scritto %d nel buffer %d\n", id, valore, buffer_id);
        
        // TBD: Gestire sincronizzazione post-scrittura
        m->num_scrittori--;  // decrementa contatore scrittori attivi
        
        if(m->num_cv_lettori > 0) {
            pthread_cond_signal(&m->cv_lettore);  // sveglia un lettore se ce ne sono in attesa
        } 
        
        pthread_mutex_unlock(&m->mutex);  // rilascio mutex
        
        // TBD: Sleep di 1 secondo
        sleep(1);  // attesa di 1 secondo prima della prossima iterazione
    }
    
    // TBD: Liberare memoria argomenti
    free(args);  // libera memoria allocata per gli argomenti
    return NULL;
}

void* lettore_media(void* arg) {
    // TBD: Cast argomenti
    struct monitor* m = (struct monitor*)arg;
    
    for(int i = 0; i < NUM_ITERAZIONI; i++) {
        // TBD: Gestire sincronizzazione pre-lettura
        int val1, val2; 
        
        pthread_mutex_lock(&m->mutex);  // acquisizione mutex
        
        m->num_cv_lettori++;  // incrementa prima del while
        while(m->num_scrittori > 0) {  // attendi se ci sono scrittori attivi
            pthread_cond_wait(&m->cv_lettore, &m->mutex); 
        }
        m->num_cv_lettori--;  // decrementa dopo il while
        
        m->num_lettori++;  // incrementa contatore lettori attivi
        
        // TBD: Leggere entrambi i buffer
        val1 = m->buffer1;  // leggi buffer1
        val2 = m->buffer2;  // leggi buffer2
        
        m->num_lettori--;  // decrementa contatore lettori attivi
        
        if(m->num_cv_scrittori > 0) {
            pthread_cond_signal(&m->cv_scrittore);  // sveglia uno scrittore se ce ne sono in attesa
        }
        
        pthread_mutex_unlock(&m->mutex);  // rilascio mutex
        
        // TBD: Calcolare e stampare media
        float media = (val1 + val2) / 2.0;
        printf("Lettore media: La media è %.2f\n", media);
        
        // TBD: Sleep di 2 secondi
        sleep(2);  // attesa di 2 secondi prima della prossima iterazione
    }
    return NULL;
}

void* lettore_max(void* arg) {
    // TBD: Cast argomenti
    struct monitor* m = (struct monitor*)arg;
    
    for(int i = 0; i < NUM_ITERAZIONI; i++) {
        // TBD: Gestire sincronizzazione pre-lettura
        int val1, val2; 
        
        pthread_mutex_lock(&m->mutex);  // acquisizione mutex
        
        m->num_cv_lettori++;  // incrementa prima del while
        while(m->num_scrittori > 0) {  // attendi se ci sono scrittori attivi
            pthread_cond_wait(&m->cv_lettore, &m->mutex); 
        }
        m->num_cv_lettori--;  // decrementa dopo il while
        
        m->num_lettori++;  // incrementa contatore lettori attivi
        
        // TBD: Leggere entrambi i buffer
        val1 = m->buffer1;  // leggi buffer1
        val2 = m->buffer2;  // leggi buffer2
        
        m->num_lettori--;  // decrementa contatore lettori attivi
        
        if(m->num_cv_scrittori > 0) {
            pthread_cond_signal(&m->cv_scrittore);  // sveglia uno scrittore se ce ne sono in attesa
        }
        
        pthread_mutex_unlock(&m->mutex);  // rilascio mutex
        
        // TBD: Calcolare e stampare massimo
        int max = (val1 > val2) ? val1 : val2;
        printf("Lettore max: Il massimo è %d\n", max);
        
        // TBD: Sleep di 2 secondi
        sleep(2);  // attesa di 2 secondi prima della prossima iterazione
    }
    return NULL;
}