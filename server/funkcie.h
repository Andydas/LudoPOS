
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
int koniecHodnota;
int sockfd;
int cisloPortu;
socklen_t cli_len;
struct sockaddr_in serverAdresa;
struct sockaddr_in klientAdresa;
int n;
char buffer[256];
int onlineUSers[5];

typedef struct data {
    volatile int * onlineUsers;
    volatile int * poleFigurok;
    volatile int * pomocnePole;
    int socketKlient;
    int n;
    int ID;
    int * userNaRade;
    int koniecHodnota;
} DATA;

void* komunikacia(void* param);
void zapis(DATA *data);
bool rezignaciaF(int kto, int rez, DATA * data);
int logikaHryF(int kto, int hod, int fig, DATA * data, int prvy, int druhy);
bool vyhodenieF(int kto, int fig, int akt, DATA * data);

#endif //SERVER_FUNKCIE_H
