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
} DATA;


int hodKockou() {
    return 1+rand()%6;
}

void precitajServerData(DATA* data){
    int uspech = read(data->sock, data->buffCitanie, VELKOST_BUFFER - 1);
    if (uspech < 0) {
        perror("Chyba pri citani zo socketu\n");//kontrola co precitalo
    }
    //zapisem si co som precital
    data->ID = data->buffCitanie[0];
    data->ktoJeNaRade = data->buffCitanie[1];
    data->vyherca = data->buffCitanie[5];
    //zapis pozicii panacikov do pola
    for (int i = 0; i < POCET_PANACIKOV; i++) {
        data->poziciePanacikov[i] = data->buffCitanie[i+2];
    }
    data->vyherca = data->buffCitanie[10];
    //kontrolny vypis komunikacie
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
    printf("Uspesne zapisane do socketu!\n");
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
            if ((!vsetciDomcek) && (data->poziciePanacikov[i] + hod <= VElKOST_HRACEJ_PLOCHY)){
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
            if ((!vsetciDomcek) && (data->poziciePanacikov[i] + hod <= VElKOST_HRACEJ_PLOCHY)){
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

    //je von z domceka? ak ano mam sa este kam posunut s danym hodom?
    if ((!jeVDomceku) && (data->poziciePanacikov[panacik - 1] + hod <= VElKOST_HRACEJ_PLOCHY)){
        return true;
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
                zapisServerData(data, data->ID, 0, data->ID*4, 1);
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
                        printf("Ktorym panacikom sa chces posunut?\n");
                        if (data->ID == 1) {
                            for (int i = 1; i < 5; i++) {
                                if (mozeHybatKonkretnymPanacikom(data, hod, i))
                                    printf("%d - panacik\n", i);//SEM VYPIS IBA TYCH KTORYMI MOZES HADZAT
                            }
                            scanf("%d", &vstupPanacik);
                            if (vstupPanacik > 0 && vstupPanacik < 5){
                                zapisServerData(data, data->ID, hod, vstupPanacik, 0);
                                okVstupPanacik = true;
                            } else {
                                printf("Neplany vstup, zadaj panacika od 1 po 4.\n");
                            }
                        } else if (data->ID == 2){
                            for (int i = 5; i < 9; i++) {
                                if (mozeHybatKonkretnymPanacikom(data, hod, i))
                                    printf("%d - panacik\n", i);//SEM VYPIS IBA TYCH KTORYMI MOZES HADZAT
                            }
                            scanf("%d", &vstupPanacik);
                            if (vstupPanacik > 4 && vstupPanacik < 9){
                                zapisServerData(data, data->ID, hod, vstupPanacik - 4, 0);
                                okVstupPanacik = true;
                            } else {
                                printf("Neplany vstup, zadaj panacika od 1 po 4.\n");
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
        //kontrola konca  hry
        if (skontrolujVyhercu(data))
            break;


        //kym nie som na rade tak cakam
        while (data->ID != data->ktoJeNaRade){
            //citanie spravy od servera
            precitajServerData(data);
            //kontrola konca  hry
            if (skontrolujVyhercu(data))
                data->koniecHry = true;

        }

        if (data->koniecHry)
            break;

        printf("Teraz si na rade, tvoj vstup bude odoslany na server.\n");

        //hadzem koockou a zapisujem to serveru
        citajVstupKonzola(data);
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
    DATA pomData;
    pomData.koniecHry = false;
    pomData.ID = -1;
    pomData.n = n;
    pomData.sock = sock;
    pomData.vyherca = 0;
    for (int i = 0; i < POCET_PANACIKOV; i++) {
        pomData.poziciePanacikov[i] = 0;
    }

    //zacnem komunikaciu
    komunikacia(&pomData);

    //pthread_t thread;
    //pthread_create(&thread, NULL, &komunikacia, (void*) &pomData);

    //pthread_join(thread, NULL);

    close(sock);

    return 0;
}


