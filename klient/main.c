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

#define POCET_HRACOV 2
#define POCET_PANACIKOV 8
#define VELKOST_BUFFER 256
#define VElKOST_HRACEJ_PLOCHY 44

    bool koniecHry;
    int ID;
    int ktoJeNaRade;
    int sock;
    int n;
    int vyherca;
    int poziciePanacikov[POCET_PANACIKOV];
    char buffCitanie[VELKOST_BUFFER];
    char buffZapisovanie[VELKOST_BUFFER];



int hodKockou() {
    return 1+rand()%6;
}

void precitajServerData(){
    int uspech = read(sock, buffCitanie, VELKOST_BUFFER - 1);
    if (uspech < 0) {
        perror("Chyba pri citani zo socketu\n");//kontrola co precitalo
    }
    //zapisem si co som precital
    ID = buffCitanie[0];
    ktoJeNaRade = buffCitanie[1];
    vyherca = buffCitanie[5];
    //zapis pozicii panacikov do pola
    for (int i = 0; i < POCET_PANACIKOV; i++) {
        poziciePanacikov[i] = buffCitanie[i+2];
    }
    vyherca = buffCitanie[10];
    //kontrolny vypis komunikacie
    printf("Moje idcko je: %d\n", ID);
    printf("Na rade je: %d\n", ktoJeNaRade);
    if (ID == 1) {
        for (int i = 0; i < 4; i++) {
            printf("Panacik %d (moj) je na pozicii: %d\n", i+1, poziciePanacikov[i]);
        }
        for (int i = 4; i < 8; i++) {
            printf("Panacik %d (protihracov) je na pozicii: %d\n", i+1, poziciePanacikov[i]);
        }
    } else if (ID == 2){
        for (int i = 0; i < 4; i++) {
            printf("Panacik %d (protihracov) je na pozicii: %d\n", i+1, poziciePanacikov[i]);
        }
        for (int i = 4; i < 8; i++) {
            printf("Panacik %d (moj) je na pozicii: %d\n", i+1, poziciePanacikov[i]);
        }
    }
}

void zapisServerData(int id, int hod, int panacik, int rezignacia){

    bzero(buffZapisovanie, VELKOST_BUFFER);
    //zapisem moje ID
    buffZapisovanie[0] = id;
    //zapisem hod kockou
    buffZapisovanie[1] = hod;
    //zapisem ktorym panacikom posuvam
    buffZapisovanie[2] = panacik;
    //rezignacia
    buffZapisovanie[10] = rezignacia;

    n = write(sock, buffZapisovanie, VELKOST_BUFFER - 1);
    if (n < 0){
        perror("Chyba pri zapisovani do socketu");
        //return 5;
    }
    printf("Uspesne zapisane do socketu!\n");
}

bool mozeHybatPanacikom(int hod){
    if (ID == 1){
        for (int i = 0; i < 4; i++) {
            //mam niekoho von z domceka? ak ano mam sa este kam posunut s danym hodom?
            if ((poziciePanacikov[i] > 0) && (poziciePanacikov[i] + hod <= VElKOST_HRACEJ_PLOCHY)){
                return true;
            }
        }
        if (hod == 6){
            return true;
        }
    } else  if (ID == 2){
        for (int i = 4; i < 8; i++) {
            if ((poziciePanacikov[i] > 0) && (poziciePanacikov[i] + hod <= VElKOST_HRACEJ_PLOCHY)){
                return true;
            }
        }
        if (hod == 6){
            return true;
        }
    }
    return false;
}

void citajVstupKonzola(){
    bool okVstup = false;
    while (!okVstup){
        printf("Co si zelas spravit?\n");
        printf("1 - hod kockou\n");
        printf("0 - vzdaj sa \n");
        int vstupKonzola;
        scanf("%d", &vstupKonzola);

        if (vstupKonzola == 1 || vstupKonzola == 0){
            //kontrolujem ci rezignoval
            if (vstupKonzola == 0){
                zapisServerData(ID, 0, ID*4, 1);
                koniecHry = true;
            } else {
                int hod = hodKockou();
                //moze hybat??????????????????????????????????????????????????????????????
                if (mozeHybatPanacikom(hod)){
                    bool okVstupPanacik = false;
                    while (!okVstupPanacik){
                        int vstupPanacik;
                        printf("Ktorym panacikom sa chces posunut?\n");
                        if (ID == 1) {
                            for (int i = 1; i < 5; i++) {
                                printf("%d - panacik\n", i);
                            }
                            scanf("%d", &vstupPanacik);
                            if (vstupPanacik > 0 && vstupPanacik < 5){
                                zapisServerData(ID, hod, vstupPanacik, 0);
                                okVstupPanacik = true;
                            } else {
                                printf("Neplany vstup, zadaj panacika od 1 po 4.\n");
                            }
                        } else if (ID == 2){
                            for (int i = 5; i < 9; i++) {
                                printf("%d - panacik\n", i);
                            }
                            scanf("%d", &vstupPanacik);
                            if (vstupPanacik > 4 && vstupPanacik < 9){
                                zapisServerData(ID, hod, vstupPanacik, 0);
                                okVstupPanacik = true;
                            } else {
                                printf("Neplany vstup, zadaj panacika od 1 po 4.\n");
                            }
                        }
                    }
                //ak sa nemoze hybat tak
                } else {
                    zapisServerData(ID, 0, ID*4, 0);
                    printf("S danym hodom sa nemozes posunut, tak priste pane kolego....");
                }
            }
            okVstup = true;
        } else {
            printf("Neplany vstup, zadaj 1 alebo 0.\n");
        }
    }
}

bool skontrolujVyhercu(){
    if (vyherca != 0){
        if (vyherca == ID){
            printf("Hra konci, VYHRAVAS\n");
            return true;
        } else {
            printf("Hra konci, PREHRAVAS\n");
            return true;
        }
    }
    return false;
}

void komunikacia() {

    bzero(buffCitanie, VELKOST_BUFFER);
    bzero(buffZapisovanie, VELKOST_BUFFER);

    //posielanie sprav
    while (!koniecHry) {

        //citanie spravy od servera
        precitajServerData();
        //kontrola konca  hry
        if (skontrolujVyhercu())
            break;


        //kym nie som na rade tak cakam
        while (ID != ktoJeNaRade){
            //citanie spravy od servera
            precitajServerData();
            //kontrola konca  hry
            if (skontrolujVyhercu())
                koniecHry = true;

        }

        if (koniecHry)
            break;

        printf("Teraz si na rade, tvoj vstup bude odoslany na server.\n");

        //hadzem koockou a zapisujem to serveru
        citajVstupKonzola();
    }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    int sock;
    int n;
    struct sockaddr_in serverAdresa;
    struct hostent* server;

    //kontrolujem pocet argumentov
    if (argc < 3)
    {
        fprintf(stderr,"Malo argumentov!\n", argv[0]);
        return 1;
    }

    //ziskam meno servera
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Zvoleny host neexistuje!\n");
        return 2;
    }
    printf("Pripojene na zvoleny host!\n");

    //urcenie adresy
    bzero((char*)&serverAdresa, sizeof(serverAdresa));
    serverAdresa.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
                (char*)&serverAdresa.sin_addr.s_addr,
                server->h_length
    );
    serverAdresa.sin_port = htons(atoi(argv[2]));

    //vytvorim tcp socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Chyba pri vytvarani socketu!");
        return 3;
    }
    printf("Socket uspesne vytvoreny!\n");

    //pripojim sa na server
    if(connect(sock, (struct sockaddr*)&serverAdresa, sizeof(serverAdresa)) < 0)
    {
        perror("Chyba pri pripajani sa do socketu!");
        return 4;
    }
    printf("Socket uspesne pripojeny!\n");

    //vytvorim datovu strukturu
    koniecHry = false;
    ID = -1;
    n = n;
    sock = sock;
    vyherca = 0;
    for (int i = 0; i < POCET_PANACIKOV; i++) {
        poziciePanacikov[i] = 0;
    }

    //zacnem komunikaciu
    komunikacia();

    //pthread_t thread;
    //pthread_create(&thread, NULL, &komunikacia, (void*) &pomData);

    //pthread_join(thread, NULL);

    close(sock);

    return 0;
}


