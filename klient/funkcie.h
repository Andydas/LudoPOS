#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

//attr a struktury

#define POCET_PANACIKOV 8
#define HRAC1 1
#define HRAC2 2
#define VELKOST_BUFFER 256
#define POCET_POLICOK 45
#define INDEX_CIEL 100
#define INDEX_DOMCEK 0
#define VELKOST_HRACIEHO_POLA 100
#define PRAVIDLA 2
#define SKONCI 0
#define ASCII_NULA 48
#define PRAZDNE_POLICKO 95
#define DOMCEK1 50
#define DOMCEK2 51
#define DOMCEK3 52
#define DOMCEK4 53
#define DOMCEK5 54
#define DOMCEK6 55
#define DOMCEK7 56
#define DOMCEK8 57

typedef struct data{
    bool koniecHry;
    int ID;
    int ktoJeNaRade;
    int sock;
    int n;
    int vyherca;
    int ktoCitanie;
    int hodCitanie;
    int panacikCitanie;
    int poziciePanacikov[POCET_PANACIKOV];
    char buffCitanie[VELKOST_BUFFER];
    char buffZapisovanie[VELKOST_BUFFER];
    char hraciePole[VELKOST_HRACIEHO_POLA];
    int ciel1;
    int ciel2;
} DATA;

//funkcie
int hodKockou();
void zobrazHraciePole(DATA* data);
void vypisNaKonzolu(DATA* data);
void zapisDoHraciehoPola(DATA* data);
void precitajServerData(DATA* data);
void zapisServerData(DATA* data, int id, int hod, int pancik, int rezignacia);
bool mozeHybatPanacikom(DATA* data, int hod);
bool mozeHybatKonkretnymPanacikom(DATA* data, int hod, int panacik);
void citajVstupKonzola(DATA* data);
bool skontrolujVyhercu(DATA* data);
void komunikacia(DATA* data);
void zobrazPravidla();

#ifdef __cplusplus
}
#endif

#endif /* DEFINITIONS_H */