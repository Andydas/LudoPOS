#ifndef DEFINITIONS_H
#define DEFINITIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#ifdef __cplusplus
extern "C" {
#endif

//attr

typedef struct data{
    int ID;
    int somNaRade;
    int ktoJeNaRade;
    int sock;
    int n;
    pthread_mutex_t * mut;
    pthread_cond_t * odosli;
} DATA;

//funkcie




void komunikacia (void* param);


#ifdef __cplusplus
}
#endif

#endif /* DEFINITIONS_H */