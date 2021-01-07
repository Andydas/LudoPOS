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


int hodKockou() {
    return 1+rand()%6;
}

void zobrazHraciePole(DATA * data){
    //prvy riadok
    system("clear");

    printf("*************** HRA LUDO ***************\n");
    printf("\t_________________________\n");
    printf("\t|       |%c| %c |%c|       |\n",data->hraciePole[19],data->hraciePole[20],data->hraciePole[21]);
    printf("\t| CIEL  |%c||%c||%c|  %c %c  |\n",data->hraciePole[18],data->hraciePole[46],data->hraciePole[22],data->hraciePole[54],data->hraciePole[55]);
    printf("\t| 2: %c  |%c||%c||%c|  %c %c  |\n",data->ciel2,data->hraciePole[17],data->hraciePole[47],data->hraciePole[23],data->hraciePole[56],data->hraciePole[57]);
    printf("\t|_______|%c||%c||%c|_______|\n",data->hraciePole[16],data->hraciePole[48],data->hraciePole[24]);
    printf("\t|%c|%c|%c|%c|%c||%c||%c|%c|%c|%c|%c|\n",data->hraciePole[11],data->hraciePole[12],data->hraciePole[13],data->hraciePole[14],data->hraciePole[15],data->hraciePole[49],data->hraciePole[25],data->hraciePole[26],data->hraciePole[27],data->hraciePole[28],data->hraciePole[29]);
    printf("\t|%c|___________________|%c|\n",data->hraciePole[10],data->hraciePole[30]);
    printf("\t|%c|%c|%c|%c|%c||%c||%c|%c|%c|%c|%c|\n",data->hraciePole[9],data->hraciePole[8],data->hraciePole[7],data->hraciePole[6],data->hraciePole[5],data->hraciePole[44],data->hraciePole[35],data->hraciePole[34],data->hraciePole[33],data->hraciePole[32],data->hraciePole[31]);
    printf("\t|       |%c||%c||%c|       |\n",data->hraciePole[4],data->hraciePole[43],data->hraciePole[36]);
    printf("\t|  %c %c  |%c||%c||%c| CIEL  |\n",data->hraciePole[50],data->hraciePole[51],data->hraciePole[3],data->hraciePole[42],data->hraciePole[37]);
    printf("\t|  %c %c  |%c||%c||%c| 1: %c  |\n",data->hraciePole[52],data->hraciePole[53],data->hraciePole[2],data->hraciePole[41],data->hraciePole[38],data->ciel1);
    printf("\t|_______|%c| %c |%c|_______|\n",data->hraciePole[1],data->hraciePole[40],data->hraciePole[39]);
    printf("\n");
}

void vypisNaKonzolu(DATA * data){
    //kontrolny vypis komunikacie
    printf("Moje idcko je: %d\n", data->ID);
    //printf("Na rade je: %d\n", data->ktoJeNaRade);
    //printf("Vyherca je: %d\n", data->vyherca);
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
    printf("----------------------- \n");
    printf("Minule kolo hral hrac %d \n", data->ktoCitanie);
    printf("Hodil %d \n", data->hodCitanie);
    if (data->panacikCitanie == -1){
        printf("Neposunul ziadneho panacika\n");
    } else {
        printf("Posunul panacika %d.\n",data->panacikCitanie);
    }
    printf("\n");
}

void zapisDoHraciehoPola(DATA* data){
    for (int j = 0; j < 59; j++) {
        data->hraciePole[j] = 95;
    }
    for (int i = 0; i < POCET_PANACIKOV; i++) {
        if (data->poziciePanacikov[i] == 0){
            switch (i) {
                case 0:
                    data->hraciePole[50] = i+1+48;
                    break;
                case 1:
                    data->hraciePole[51] = i+1+48;
                    break;
                case 2:
                    data->hraciePole[52] = i+1+48;
                    break;
                case 3:
                    data->hraciePole[53] = i+1+48;
                    break;
                case 4:
                    data->hraciePole[54] = i+1+48;
                    break;
                case 5:
                    data->hraciePole[55] = i+1+48;
                    break;
                case 6:
                    data->hraciePole[56] = i+1+48;
                    break;
                case 7:
                    data->hraciePole[57] = i+1+48;
                    break;
            }
        } else {

            switch (i) {
                case 0:
                    data->hraciePole[50] = 95;
                    break;
                case 1:
                    data->hraciePole[51] = 95;
                    break;
                case 2:
                    data->hraciePole[52] = 95;
                    break;
                case 3:
                    data->hraciePole[53] = 95;
                    break;
                case 4:
                    data->hraciePole[54] = 95;
                    break;
                case 5:
                    data->hraciePole[55] = 95;
                    break;
                case 6:
                    data->hraciePole[56] = 95;
                    break;
                case 7:
                    data->hraciePole[57] = 95;
                    break;
            }
            for (int j = 0; j < 59; j++) {
                if (data->poziciePanacikov[i] == j){
                    data->hraciePole[j] = i+1+48;
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
    data->ktoCitanie = data->buffCitanie[11];
    data->hodCitanie = data->buffCitanie[12];
    data->panacikCitanie = data->buffCitanie[13];
    //zapis pozicii panacikov do pola
    data->ciel1=48; //cislo 0 v ascii
    data->ciel2=48; //cislo 0 v ascii
    for (int i = 0; i < POCET_PANACIKOV; i++) {
        data->poziciePanacikov[i] = data->buffCitanie[i+2];
        if (data->poziciePanacikov[i] == 100){
            if (i < 4){
                data->ciel1++;
            } else {
                data->ciel2++;
            }
        }
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
    bool niektoDoma;
    if (data->ID == 1){
        niektoDoma = false;
        //su vsetci moji panacikovia v domceku? ak ano mozem sa hybat len po hode 6
        for (int i = 0; i < 4; i++) {
            if (data->poziciePanacikov[i] > 0 && data->poziciePanacikov[i] < 100) {
                vsetciDomcek = false;
                break;
            } if (data->poziciePanacikov[i] == 0) {
                niektoDoma = true;
            }
        }
        if (vsetciDomcek && hod == 6) {
            return true;
        } else if (vsetciDomcek && hod != 6){
            return false;
        }
        if (niektoDoma && hod ==6){
            return true;
        }


        //mam niekoho von z domceka? ak ano mam sa este kam posunut s danym hodom?
        for (int i = 0; i < 4; i++) {
            if ((!vsetciDomcek) && (data->poziciePanacikov[i] != ID_CIEL) && (data->poziciePanacikov[i] + hod <= POCET_POLICOK)){
                return true;
            }
        }
    } else  if (data->ID == 2){
        niektoDoma = false;
        //su vsetci moji panacikovia v domceku? ak ano mozem sa hybat len po hode 6
        for (int i = 4; i < 8; i++) {
            if (data->poziciePanacikov[i] > 0 && data->poziciePanacikov[i] < 100) {
                vsetciDomcek = false;
                break;
            } if (data->poziciePanacikov[i] == 0) {
                niektoDoma = true;
            }
        }
        if (vsetciDomcek && hod == 6) {
            return true;
        } else if (vsetciDomcek && hod != 6){
            return false;
        }
        if (niektoDoma && hod ==6){
            return true;
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

//kontrola, ci sa moze hybat konkretnym panacikom
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

//citanie vstupu od uzivatela a nasledne zapisovanie serveru
void citajVstupKonzola(DATA* data){
    bool okVstup = false;
    while (!okVstup){
        printf("1 - hod kockou\n");
        printf("0 - vzdaj sa \n");
        printf("HRAC %d, co si zelas spravit?: ",data->ID);
        int vstupKonzolaChar;
        int vstupKonzola;
        do
            vstupKonzolaChar = getchar();
        while (isspace(vstupKonzolaChar));
        vstupKonzola = vstupKonzolaChar - 48;

        //if (vstupKonzola == 1 || vstupKonzola == 0){
        if (vstupKonzola < 7 && vstupKonzola > -1){
            //kontrolujem ci rezignoval
            if (vstupKonzola == 0){
                zapisServerData(data, data->ID, 7, 1, 1);
                //printf("Vzdal si sa PREHRAVAS\n");
                //data->koniecHry = true;
            } else {
                //int hod = hodKockou();
                int hod = vstupKonzola;
                printf("Hodil si kockou, hodil si: %d\n", hod);
                //moze hybat??????????????????????????????????????????????????????????????
                if (mozeHybatPanacikom(data, hod)){
                    bool okVstupPanacik = false;
                    while (!okVstupPanacik){
                        char vstupPanacikChar;
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
                            do
                                vstupPanacikChar = getchar();
                            while (isspace(vstupPanacikChar));
                            vstupPanacik = vstupPanacikChar - 48;
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
                            do
                                vstupPanacikChar = getchar();
                            while (isspace(vstupPanacikChar));
                            vstupPanacik = vstupPanacikChar - 48;
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
        zobrazHraciePole(data);
        vypisNaKonzolu(data);
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
            zobrazHraciePole(data);
            vypisNaKonzolu(data);
            //kontrola konca  hry
            if (skontrolujVyhercu(data)){
                data->koniecHry = true;
                break;
            }
        }

        if (data->koniecHry){
            break;
        }


        printf("Teraz si na rade, tvoj vstup bude odoslany na server.\n");

        //uz som na rade tak hadzem kockou a zapisujem to serveru
        citajVstupKonzola(data);

    }
}

void zobrazPravidla(){
    printf("\n");
    printf("************** PRAVIDLA HRY LUDO ***************\n");
    printf("Hra je pre 2 hracov\n");
    printf("Kym ma hrac vsetkych panacikov v domceku, musi hodit 6 aby nejakeho postavil na hraciu plochu\n");
    printf("Na jednom policku sa v jednom momente moze nachadzat len jeden panacik\n");
    printf("Ak chce hrac postavit panacika na policko, kde uz je nejaky iny panacik, tohto panacika vyhodi, ci uz je jeho alebo nie\n");
    printf("Hra sa konci ked sa jeden z hracov vzda alebo ma jeden z hracov vsetkych panacikov v cieli\n");
    printf("\n");
}


int main(int argc, char *argv[])
{    bool vstupOK = false;
    char vstupChar;
    int vstup;
    while (!vstupOK) {
        printf("VITAJ V APLIKACII LUDO\n");
        printf("1 - Pripoj na server\n");
        printf("2 - Zobraz pravidla\n");
        printf("0 - Vypni aplikaciu\n");
        printf("Co chces urobit?: ");
        do
            vstupChar = getchar();
        while (isspace(vstupChar));
        vstup = vstupChar - 48;
        if (vstup >= 0 && vstup <= 2){
            if (vstup == 2) {
                zobrazPravidla();
                vstupOK = false;
            } else if (vstup == 0) {
                return 0;
            } else {
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

        }
        vstupOK = false;
        printf("Neplatny vstupChar\n");
    }

}


