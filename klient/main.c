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
#define POCET_POLICOK 45
#define ID_CIEL 100
#define VELKOST_HRACIEHO_POLA 100

typedef struct data{
    bool koniecHry;
    int ID;
    int ktoJeNaRade;
    int sock;
    int n;
    int vyherca;
    int poziciePanacikov[POCET_PANACIKOV];
    char buffCitanie[VELKOST_BUFFER];
    char buffZapisovanie[VELKOST_BUFFER];
    char hraciePole[VELKOST_HRACIEHO_POLA];
} DATA;


int hodKockou() {
    return 1+rand()%6;
}

/*void zobrazHraciePole(DATA * data){
    //prvy riadok
    system("clear");
    printf("_________________________\n");
    printf("|       |%c| %c |%c|       |\n",data->hraciePole[19],data->hraciePole[20],data->hraciePole[21]);
    printf("| CIEL  |%c||%c||%c|  %c %c  |\n",data->hraciePole[18],data->hraciePole[46],data->hraciePole[22],data->hraciePole[54],data->hraciePole[55]);
    printf("|  2: %c |%c||%c||%c|  %c %c  |\n",data->hraciePole[52],data->hraciePole[17],data->hraciePole[47],data->hraciePole[23],data->hraciePole[56],data->hraciePole[57]);
    printf("|_______|%c||%c||%c|_______|\n",data->hraciePole[16],data->hraciePole[48],data->hraciePole[24]);
    printf("|%c|%c|%c|%c|%c||%c||%c|%c|%c|%c|%c|\n",data->hraciePole[11],data->hraciePole[12],data->hraciePole[13],data->hraciePole[14],data->hraciePole[15],data->hraciePole[49],data->hraciePole[25],data->hraciePole[26],data->hraciePole[27],data->hraciePole[28],data->hraciePole[29]);
    printf("|%c|___________________|%c|\n",data->hraciePole[10],data->hraciePole[30]);
    printf("|%c|%c|%c|%c|%c||%c||%c|%c|%c|%c|%c|\n",data->hraciePole[9],data->hraciePole[8],data->hraciePole[7],data->hraciePole[6],data->hraciePole[5],data->hraciePole[44],data->hraciePole[35],data->hraciePole[34],data->hraciePole[33],data->hraciePole[32],data->hraciePole[31]);
    printf("|       |%c||%c||%c|       |\n",data->hraciePole[4],data->hraciePole[43],data->hraciePole[36]);
    printf("|  %c %c  |%c||%c||%c| CIEL  |\n",data->hraciePole[50],data->hraciePole[51],data->hraciePole[3],data->hraciePole[42],data->hraciePole[37]);
    printf("|  %c %c  |%c||%c||%c|  1: %c |\n",data->hraciePole[52],data->hraciePole[53],data->hraciePole[2],data->hraciePole[41],data->hraciePole[38],data->hraciePole[51]);
    printf(      "|_______|%c| %c |%c|_______|\n",data->hraciePole[1],data->hraciePole[40],data->hraciePole[39]);
}*/

void vypisNaKonzolu(DATA * data){
    //kontrolny vypis komunikacie
    system("clear");
    printf("*************** HRA LUDO ***************\n");
    printf("Moje idcko je: %d\n", data->ID);
    printf("Na rade je: %d\n", data->ktoJeNaRade);
    if (data->ID == 1) {
        for (int i = 0; i < 4; i++) {
            printf("Panacik %d (moj) je na pozicii: %d\n", i+1, data->poziciePanacikov[i]);
        }
        for (int i = 4; i < 8; i++) {
            printf("Panacik %d (protihracov) je na pozicii: %d\n", i+1, data->poziciePanacikov[i]);
        }
    } else if (data->ID == 2){
        for (int i = 0; i < 4; i++) {
            printf("Panacik %d (protihracov) je na pozicii: %d\n", i+1, data->poziciePanacikov[i]);
        }
        for (int i = 4; i < 8; i++) {
            printf("Panacik %d (moj) je na pozicii: %d\n", i+1, data->poziciePanacikov[i]);
        }
    }
}

void zapisDoHraciehoPola(DATA* data){
    for (int i = 0; i < POCET_PANACIKOV; i++) {
        if (data->poziciePanacikov[i] == 0){
            switch (i+1) {
                case 1:
                    data->hraciePole[50] = i+1+48;
                case 2:
                    data->hraciePole[51] = i+1+48;
                case 3:
                    data->hraciePole[52] = i+1+48;
                case 4:
                    data->hraciePole[53] = i+1+48;
                case 5:
                    data->hraciePole[54] = i+1+48;
                case 6:
                    data->hraciePole[55] = i+1+48;
                case 7:
                    data->hraciePole[56] = i+1+48;
                case 8:
                    data->hraciePole[57] = i+1+48;
            }
        } else {
            switch (i+1) {
                case 1:
                    data->hraciePole[50] = 95;
                case 2:
                    data->hraciePole[51] = 95;
                case 3:
                    data->hraciePole[52] = 95;
                case 4:
                    data->hraciePole[53] = 95;
                case 5:
                    data->hraciePole[54] = 95;
                case 6:
                    data->hraciePole[55] = 95;
                case 7:
                    data->hraciePole[56] = 95;
                case 8:
                    data->hraciePole[57] = 95;
            }

            for (int j = 0; j < 59; j++) {
                if (data->poziciePanacikov[i] == j){
                    data->hraciePole[j] = i+1+49;
                } else {
                    data->hraciePole[j] = 95;
                }
            }
        }

    }
}

void precitajServerData(DATA* data){
    int uspech = read(data->sock, data->buffCitanie, VELKOST_BUFFER - 1);
    if (uspech < 0) {
        perror("Chyba pri citani zo socketu\n");//kontrola co precitalo
    }
    //zapisem si co som precital
    data->ID = data->buffCitanie[0];
    data->ktoJeNaRade = data->buffCitanie[1];
    //zapis pozicii panacikov do pola
    for (int i = 0; i < POCET_PANACIKOV; i++) {
        data->poziciePanacikov[i] = data->buffCitanie[i+2];
    }
    data->vyherca = data->buffCitanie[10];
    zapisDoHraciehoPola(data);
}

void zapisServerData(DATA* data, int id, int hod, int panacik, int rezignacia){

    bzero(data->buffZapisovanie, VELKOST_BUFFER);
    //zapisem moje ID
    data->buffZapisovanie[0] = id;
    //zapisem hod kockou
    data->buffZapisovanie[1] = hod;
    //zapisem ktorym panacikom posuvam
    data->buffZapisovanie[2] = panacik;
    //rezignacia
    data->buffZapisovanie[10] = rezignacia;

    data->n = write(data->sock, data->buffZapisovanie, VELKOST_BUFFER - 1);
    if (data->n < 0){
        perror("Chyba pri zapisovani do socketu");
        //return 5;
    }
}

bool mozeHybatPanacikom(DATA* data, int hod){
    bool vsetciDomcek = true;
    if (data->ID == 1){
        //su vsetci moji panacikovia v domceku? ak ano mozem sa hybat len po hode 6
        for (int i = 0; i < 4; i++) {
            if (data->poziciePanacikov[i] > 0) {
                vsetciDomcek = false;
                break;
            }
        }
        if (vsetciDomcek && hod == 6) {
            return true;
        } else if (vsetciDomcek && hod != 6){
            return false;
        }


        //mam niekoho von z domceka? ak ano mam sa este kam posunut s danym hodom?
        for (int i = 0; i < 4; i++) {
            if ((!vsetciDomcek) && (data->poziciePanacikov[i] != ID_CIEL) && (data->poziciePanacikov[i] + hod <= POCET_POLICOK)){
                return true;
            }
        }
    } else  if (data->ID == 2){
        //su vsetci moji panacikovia v domceku? ak ano mozem sa hybat len po hode 6
        for (int i = 4; i < 8; i++) {
            if (data->poziciePanacikov[i] > 0) {
                vsetciDomcek = false;
                break;
            }
        }
        if (vsetciDomcek && hod == 6) {
            return true;
        } else if (vsetciDomcek && hod != 6){
            return false;
        }

        //mam niekoho von z domceka? ak ano mam sa este kam posunut s danym hodom?
        for (int i = 4; i < 8; i++) {
            if ((!vsetciDomcek) && (data->poziciePanacikov[i] >= 21) && (data->poziciePanacikov[i] + hod <= 50)){
                return true;
            } else if ((data->poziciePanacikov[i] + hod <= 25) && (data->poziciePanacikov[i] != 0)){
                return true;
            }
        }
    }
    return false;
}

bool mozeHybatKonkretnymPanacikom(DATA* data, int hod, int panacik){

    bool jeVDomceku = true;
    //je v domceku? ak ano mozem sa hybat len po hode 6
    if (data->poziciePanacikov[panacik-1] > 0) {
        jeVDomceku = false;
    }
    if (jeVDomceku && hod == 6) {
        return true;
    } else if (jeVDomceku && hod != 6){
        return false;
    }

    //je v cieli? ak ano tak sa uz nemoze hybat
    if (data->poziciePanacikov[panacik-1] == ID_CIEL) {
        return false;
    }

    //je von z domceka? ak ano mam sa este kam posunut s danym hodom?
    if (data->ID == 1) {
        if ((!jeVDomceku) && (data->poziciePanacikov[panacik - 1] + hod <= POCET_POLICOK)){
            return true;
        }
    }
    if (data->ID == 2) {
        if ((!jeVDomceku) && (data->poziciePanacikov[panacik-1] >= 21) && (data->poziciePanacikov[panacik-1] + hod <= 50)){
            return true;
        } else if ((data->poziciePanacikov[panacik-1] + hod <= 25 )){
            return true;
        }
    }
    return false;
}

void citajVstupKonzola(DATA* data){
    bool okVstup = false;
    while (!okVstup){
        printf("Co si zelas spravit?\n");
        printf("1 - hod kockou\n");
        printf("0 - vzdaj sa \n");
        int vstupKonzola;
        scanf("%d", &vstupKonzola);

        if (vstupKonzola == 1 || vstupKonzola == 0){
        //if (vstupKonzola < 7 && vstupKonzola > -1){
            //kontrolujem ci rezignoval
            if (vstupKonzola == 0){
                zapisServerData(data, data->ID, 7, 1, 1);
                data->koniecHry = true;
            } else {
                int hod = hodKockou();
                //int hod = vstupKonzola;
                printf("Hodil si kockou, hodil si: %d\n", hod);
                //moze hybat??????????????????????????????????????????????????????????????
                if (mozeHybatPanacikom(data, hod)){
                    bool okVstupPanacik = false;
                    while (!okVstupPanacik){
                        int vstupPanacik;
                        int vstupy[4] = {0};
                        printf("Ktorym panacikom sa chces posunut?\n");
                        if (data->ID == 1) {
                            for (int i = 1; i < 5; i++) {
                                if (mozeHybatKonkretnymPanacikom(data, hod, i)){
                                    printf("%d - panacik %d\n", i, i);//SEM VYPIS IBA TYCH KTORYMI MOZES HADZAT
                                    vstupy[i-1]=i;
                                }
                            }
                            scanf("%d", &vstupPanacik);
                            for (int i = 0; i < 4; i++) {
                                if (vstupy[i] == vstupPanacik) {
                                    okVstupPanacik = true;
                                    break;
                                }
                            }
                            if (okVstupPanacik) {
                                zapisServerData(data, data->ID, hod, vstupPanacik, 0);
                            } else {
                                printf("Neplany vstup, zadaj panacika s ktorym mozes hybat.\n");
                            }
                        } else if (data->ID == 2){
                            for (int i = 5; i < 9; i++) {
                                if (mozeHybatKonkretnymPanacikom(data, hod, i)){
                                    printf("%d - panacik %d\n", i, i);//SEM VYPIS IBA TYCH KTORYMI MOZES HADZAT
                                    vstupy[i-5]=i;
                                }
                            }
                            scanf("%d", &vstupPanacik);
                            for (int i = 0; i < 4; i++) {
                                if (vstupy[i] == vstupPanacik) {
                                    okVstupPanacik = true;
                                    break;
                                }
                            }
                            if (okVstupPanacik) {
                                zapisServerData(data, data->ID, hod, vstupPanacik - 4, 0);
                            } else {
                                printf("Neplany vstup, zadaj panacika s ktorym mozes hybat.\n");
                            }
                        }
                    }
                //ak sa nemoze hybat tak
                } else {
                    zapisServerData(data, data->ID, hod, 1, 0);
                    printf("S danym hodom sa nemozes posunut, tak priste pane kolego....\n");
                }
            }
            okVstup = true;
        } else {
            printf("Neplany vstup, zadaj 1 alebo 0.\n");
        }
    }
}

bool skontrolujVyhercu(DATA * data){
    if (data->vyherca != 0){
        if (data->vyherca == data->ID){
            printf("Hra konci, VYHRAVAS\n");
            return true;
        } else {
            printf("Hra konci, PREHRAVAS\n");
            return true;
        }
    }
    return false;
}

void komunikacia(DATA* data) {

    bzero(data->buffCitanie, VELKOST_BUFFER);
    bzero(data->buffZapisovanie, VELKOST_BUFFER);

    //posielanie sprav
    while (!data->koniecHry) {

        //citanie spravy od servera
        precitajServerData(data);
        printf("Moje ID je: %d\n", data->ID);
        vypisNaKonzolu(data);
        //zobrazHraciePole(data);
        //kontrola konca  hry
        if (skontrolujVyhercu(data)){
            data->koniecHry = true;
            break;
        }



        //kym nie som na rade tak cakam
        while (data->ID != data->ktoJeNaRade){
            printf("Pockaj, kym budes na rade.\n");
            //citanie spravy od servera
            precitajServerData(data);
            vypisNaKonzolu(data);
            //zobrazHraciePole(data);
            //kontrola konca  hry
            if (skontrolujVyhercu(data)){
                data->koniecHry = true;
                break;
            }
        }

        if (data->koniecHry)
            break;

        printf("Teraz si na rade, tvoj vstup bude odoslany na server.\n");

        //uz som na rade tak hadzem kockou a zapisujem to serveru
        citajVstupKonzola(data);
    }
}


int main(int argc, char *argv[])
{
    DATA pomData;

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

    //pripojim sa na server
    if(connect(sock, (struct sockaddr*)&serverAdresa, sizeof(serverAdresa)) < 0)
    {
        perror("Chyba pri pripajani sa do socketu!");
        return 4;
    }
    printf("Uspesne pripojene na server, pockaj kym sa napoji potrebny pocet hracov!\n");


    //vytvorim datovu strukturu
    pomData.koniecHry = false;
    pomData.ID = -1;
    pomData.n = n;
    pomData.sock = sock;
    pomData.vyherca = 0;
    for (int i = 0; i < VELKOST_HRACIEHO_POLA; i++) {
        pomData.hraciePole[i] = 95;
    }


    //zacnem komunikaciu
    komunikacia(&pomData);

    close(sock);

    return 0;
}


