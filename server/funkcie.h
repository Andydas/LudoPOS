
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

#define BUFF_SIZE 256
#define MAX_POCET_HRACOV 2
#define FIGURKY 8
#define POMOCNE_POLE 4
#define HRAC_1 1
#define HRAC_2 2
#define SOCKET_ID_1 4
#define SOCKET_ID_2 5
#define HRACIA_PLOCHA_STROP 45
#define CIEL_HODNOTA 100

int pocetUsers;
int userNaRade;
int koniecHodnota;
int sockfd;
int cisloPortu;
socklen_t cli_len;
struct sockaddr_in serverAdresa;
struct sockaddr_in klientAdresa;
int n;
char buffer[BUFF_SIZE];

typedef struct data {
    volatile int * poleFigurok;
    volatile int * pomocnePole;
    int socketKlient;
    int n;
    int * userNaRade;
    int vitaz;
    int * koniecHry;
    bool zahral;
    pthread_mutex_t * mut;
    pthread_cond_t * prve;
    pthread_cond_t * druhe;
} DATA;

void* komunikacia(void* param);
void zapis(DATA *data, int kto, int hod, int fig);
int rezignaciaF(int kto, int hod, int fig, int rez, DATA * data);
int logikaHryF(int kto, int hod, int fig, DATA * data);
bool vyhodenieF(int kto, int fig, int akt, DATA * data);
void ukonci(DATA *data);

#endif //SERVER_FUNKCIE_H
