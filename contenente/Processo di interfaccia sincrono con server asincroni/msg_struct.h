#ifndef MSG_STRUCT_H
#define MSG_STRUCT_H

#define NUM_CLIENTS 5
#define NUM_SERVERS 3
#define RICHIESTE_PER_CLIENT 5

// Tipi di messaggi
#define REQUEST_TO_SEND 1
#define OK_TO_SEND 2
#define RICHIESTA 3
#define RISPOSTA 4

// Struttura per messaggi REQUEST-TO-SEND e OK-TO-SEND
typedef struct {
    long type;
    pid_t pid_client;
} sync_msg;

// Struttura per messaggi di richiesta
typedef struct {
    long type;
    pid_t pid_client;
    int valore;
} request_msg;

// Struttura per messaggi di risposta
typedef struct {
    long type;
    pid_t pid_server;
    int valore;
} response_msg;

#endif