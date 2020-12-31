
#ifndef SERVER_FUNKCIE_H
#define SERVER_FUNKCIE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>


int pocetUsers;
int userNaRade;
int sockfd;
int cisloPortu;
socklen_t cli_len;
struct sockaddr_in serverAdresa;
struct sockaddr_in klientAdresa;
int n;
char buffer[256];

typedef struct data {
    int socketKlient;
    char * buffer;
    int n;
    int ID;
} DATA;

void* citajVstupy( void* param);
void odosliUserNaRade();

#endif //SERVER_FUNKCIE_H
