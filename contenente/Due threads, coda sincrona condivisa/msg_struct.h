#ifndef MSG_STRUCT_H
#define MSG_STRUCT_H

#include <sys/types.h>

#define REQUEST_TO_SEND 1
#define OK_TO_SEND 2

typedef struct {
    long type;           // Deve essere 1 (REQUEST_TO_SEND)
    pid_t pid_client;    // PID del client che invia la richiesta
} sync_msg;

typedef struct {
    long type;           // Deve essere il PID del client
    pid_t pid_client;
} sync_msg_ok;

typedef struct {
    long type;           // Deve essere il PID del client
    pid_t pid_client;
    int val1;
    int val2;
} message_t;

#define NUM_MESSAGGI 4     // Messaggi per client
#define NUM_CLIENTS 3      // Numero di client
#define MSG_PER_THREAD 6   // Messaggi da elaborare per thread

#endif