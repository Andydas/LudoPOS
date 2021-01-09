#include "funkcie.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

/*
 * Simuluje hod kockou tak, ze generuje nahodne cislo od 1 po 6
 */
int hodKockou() {
    return 1+rand()%6;
}

/*
 * Vypise na konzolu "graficke" znazornenie hracieho pola
 */
void zobrazHraciePole(DATA * data){
    //zobrazovanie hracieho pola

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

/*
 * Vypis informacii, ktore klient precita od servera
 */
void vypisNaKonzolu(DATA * data){
    //kontrolny vypis komunikacie
    printf("Moje idcko je: %d\n", data->ID);
    //printf("Na rade je: %d\n", data->ktoJeNaRade);
    //printf("Vyherca je: %d\n", data->vyherca);
    /*if (data->ID == 1) {
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
    }*/

    //vypis potrebnych informacii
    printf("----------------------- \n");
    printf("Minule kolo hral hrac %d \n", data->ktoCitanie);
    if (data->hodCitanie == 7) {
        printf("Vzdal sa\n");
    } else {
        printf("Hodil %d \n", data->hodCitanie);
    }

    if (data->panacikCitanie == -1){
        printf("Neposunul ziadneho panacika\n");
    } else if (data->panacikCitanie == 0 ){
        printf("\n");
    } else {
        printf("Posunul panacika %d.\n",data->panacikCitanie);
    }
    printf("\n");
}

/*
 * Zapis znakov do hracieho pola, ktore sa nasledne zobrazuju v poli
 */
void zapisDoHraciehoPola(DATA* data){
    //pociatocne naplnenie pola podtrznikmi
    for (int j = 0; j < VELKOST_HRACIEHO_POLA; j++) {
        data->hraciePole[j] = PRAZDNE_POLICKO;
    }
    for (int i = 0; i < POCET_PANACIKOV; i++) {
        //ak je panacik v domceku
        if (data->poziciePanacikov[i] == 0){
            switch (i) {
                case 0:
                    data->hraciePole[DOMCEK1] = i+1+ASCII_NULA;
                    break;
                case 1:
                    data->hraciePole[DOMCEK2] = i+1+ASCII_NULA;
                    break;
                case 2:
                    data->hraciePole[DOMCEK3] = i+1+ASCII_NULA;
                    break;
                case 3:
                    data->hraciePole[DOMCEK4] = i+1+ASCII_NULA;
                    break;
                case 4:
                    data->hraciePole[DOMCEK5] = i+1+ASCII_NULA;
                    break;
                case 5:
                    data->hraciePole[DOMCEK6] = i+1+ASCII_NULA;
                    break;
                case 6:
                    data->hraciePole[DOMCEK7] = i+1+ASCII_NULA;
                    break;
                case 7:
                    data->hraciePole[DOMCEK8] = i+1+ASCII_NULA;
                    break;
            }
        } else {
            //zvysok su prazdne policka
            switch (i) {
                case 0:
                    data->hraciePole[DOMCEK1] = PRAZDNE_POLICKO;
                    break;
                case 1:
                    data->hraciePole[DOMCEK2] = PRAZDNE_POLICKO;
                    break;
                case 2:
                    data->hraciePole[DOMCEK3] = PRAZDNE_POLICKO;
                    break;
                case 3:
                    data->hraciePole[DOMCEK4] = PRAZDNE_POLICKO;
                    break;
                case 4:
                    data->hraciePole[DOMCEK5] = PRAZDNE_POLICKO;
                    break;
                case 5:
                    data->hraciePole[DOMCEK6] = PRAZDNE_POLICKO;
                    break;
                case 6:
                    data->hraciePole[DOMCEK7] = PRAZDNE_POLICKO;
                    break;
                case 7:
                    data->hraciePole[DOMCEK8] = PRAZDNE_POLICKO;
                    break;
            }
            for (int j = 0; j < VELKOST_HRACIEHO_POLA; j++) {
                if (data->poziciePanacikov[i] == j){
                    data->hraciePole[j] = i+1+ASCII_NULA;
                }
            }
        }
    }
}

/*
 * Citanie dat zo servera do buffra
 */
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
    data->ciel1=ASCII_NULA; //cislo 0 v ascii
    data->ciel2=ASCII_NULA; //cislo 0 v ascii
    for (int i = 0; i < POCET_PANACIKOV; i++) {
        data->poziciePanacikov[i] = data->buffCitanie[i+2];
        if (data->poziciePanacikov[i] == VELKOST_HRACIEHO_POLA){
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

/*
 * Zapis dat odosielanych na server do buffra a samotne odoslanie na server
 */
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

/*
 * Kontrola, ci klient moze s danym hodom hybat nejakym panacikom, vracia true ak moze, false ak nie
 */
bool mozeHybatPanacikom(DATA* data, int hod){
    bool vsetciDomcek = true;
    bool niektoDoma;
    if (data->ID == HRAC1){
        niektoDoma = false;
        //su vsetci moji panacikovia v domceku? ak ano mozem sa hybat len po hode 6
        for (int i = 0; i < 4; i++) {
            if (data->poziciePanacikov[i] > INDEX_DOMCEK && data->poziciePanacikov[i] < INDEX_CIEL) {
                vsetciDomcek = false;
                break;
            }
            if (data->poziciePanacikov[i] == INDEX_DOMCEK) {
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
            if ((!vsetciDomcek) && (data->poziciePanacikov[i] != INDEX_CIEL) && (data->poziciePanacikov[i] + hod <= POCET_POLICOK) && (data->poziciePanacikov[i] != INDEX_DOMCEK)){
                return true;
            }
        }
        return false;
    } else  if (data->ID == HRAC2){
        niektoDoma = false;
        //su vsetci moji panacikovia v domceku? ak ano mozem sa hybat len po hode 6
        for (int i = 4; i < 8; i++) {
            if (data->poziciePanacikov[i] > INDEX_DOMCEK && data->poziciePanacikov[i] < INDEX_CIEL) {
                vsetciDomcek = false;
                break;
            }
            if (data->poziciePanacikov[i] == INDEX_DOMCEK) {
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
            if ((!vsetciDomcek) && (data->poziciePanacikov[i] != INDEX_CIEL) && (data->poziciePanacikov[i] + hod <= POCET_POLICOK+5) && (data->poziciePanacikov[i] != INDEX_DOMCEK)){
                return true;
            } else if ((data->poziciePanacikov[i] + hod <= POCET_POLICOK-20) && (data->poziciePanacikov[i] != INDEX_DOMCEK)){
                return true;
            }
        }
        return false;
    }
    return false;
}

/*
 * Kontrola, ci moze klient posunut konkretnym panacikom, vracia true ak moze, false ak nie
 */
bool mozeHybatKonkretnymPanacikom(DATA* data, int hod, int panacik){

    bool jeVDomceku = true;
    //je v domceku? ak ano mozem sa hybat len po hode 6
    if (data->poziciePanacikov[panacik-1] > INDEX_DOMCEK) {
        jeVDomceku = false;
    }
    if (jeVDomceku && hod == 6) {
        return true;
    } else if (jeVDomceku && hod != 6){
        return false;
    }

    //je v cieli? ak ano tak sa uz nemoze hybat
    if (data->poziciePanacikov[panacik-1] == INDEX_CIEL) {
        return false;
    }

    //je von z domceka? ak ano mam sa este kam posunut s danym hodom?
    if (data->ID == HRAC1) {
        if ((!jeVDomceku) && (data->poziciePanacikov[panacik - 1] + hod <= POCET_POLICOK)){
            return true;
        }
    }
    if (data->ID == HRAC2) {
        if ((!jeVDomceku) && (data->poziciePanacikov[panacik-1] >= 21) && (data->poziciePanacikov[panacik-1] + hod <= POCET_POLICOK+50)){
            return true;
        } else if ((data->poziciePanacikov[panacik-1] + hod <= POCET_POLICOK - 20)){
            return true;
        }
    }
    return false;
}

/*
 * Citanie vstupu od uzivatela a nasledne zapisovanie tahu na server
 */
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
        vstupKonzola = vstupKonzolaChar - ASCII_NULA;

        if (vstupKonzola == 1 || vstupKonzola == 0){
            //kontrolujem ci rezignoval
            if (vstupKonzola == 0){
                zapisServerData(data, data->ID, 7, 1, 1);
            } else {
                int hod = hodKockou();
                printf("Hodil si kockou, hodil si: %d\n", hod);
                //moze hybat??????????????????????????????????????????????????????????????
                if (mozeHybatPanacikom(data, hod)){
                    bool okVstupPanacik = false;
                    while (!okVstupPanacik){
                        char vstupPanacikChar;
                        int vstupPanacik;
                        int vstupy[4] = {0};
                        printf("Ktorym panacikom sa chces posunut?\n");
                        if (data->ID == HRAC1) {
                            //vypis iba tych ktorymi sa da hybat
                            for (int i = 1; i < 5; i++) {
                                if (mozeHybatKonkretnymPanacikom(data, hod, i)){
                                    printf("%d - panacik %d\n", i, i);
                                    vstupy[i-1]=i;
                                }
                            }
                            do
                                vstupPanacikChar = getchar();
                            while (isspace(vstupPanacikChar));
                            vstupPanacik = vstupPanacikChar - ASCII_NULA;
                            //dovol precitat iba takeho panacika, ktorym sa da hybat
                            for (int i = 0; i < 4; i++) {
                                if (vstupy[i] == vstupPanacik) {
                                    okVstupPanacik = true;
                                    break;
                                }
                            }
                            //samotny zapis dat na server
                            if (okVstupPanacik) {
                                zapisServerData(data, data->ID, hod, vstupPanacik, 0);
                            } else {
                                printf("Neplany vstup, zadaj panacika s ktorym mozes hybat.\n");
                            }
                        } else if (data->ID == HRAC2){
                            //vypis iba tych ktorymi sa da hybat
                            for (int i = 5; i < 9; i++) {
                                if (mozeHybatKonkretnymPanacikom(data, hod, i)){
                                    printf("%d - panacik %d\n", i, i);
                                    vstupy[i-5]=i;
                                }
                            }
                            do
                                vstupPanacikChar = getchar();
                            while (isspace(vstupPanacikChar));
                            vstupPanacik = vstupPanacikChar - ASCII_NULA;
                            //dovol precitat iba takeho panacika, ktorym sa da hybat
                            for (int i = 0; i < 4; i++) {
                                if (vstupy[i] == vstupPanacik) {
                                    okVstupPanacik = true;
                                    break;
                                }
                            }
                            //samotny zapis dat na server
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

/*
 * Kontroluje, ci server poslal informaciu o vyhercovi hry, vypise ci klient vyhral alebo prehral. Vracia true ak je koniec hry, false ak nie
 */
bool skontrolujVyhercu(DATA * data){
    if (data->vyherca != 0){
        //ak som vyherca ja
        if (data->vyherca == data->ID){
            printf("Hra konci, VYHRAVAS\n");
            return true;
        //ak je vyherca niekto iny
        } else {
            printf("Hra konci, PREHRAVAS\n");
            return true;
        }
    }
    return false;
}

/*
 * Zabezpecuje komunikaciu medzi klientom a serverom az pokial neskonci hra
 */
void komunikacia(DATA* data) {

    bzero(data->buffCitanie, VELKOST_BUFFER);
    bzero(data->buffZapisovanie, VELKOST_BUFFER);

    //cyklus vymeny informacii (posielanie sprav)
    while (!data->koniecHry) {

        //citanie spravy od servera
        precitajServerData(data);
        printf("Moje ID je: %d\n", data->ID);
        zobrazHraciePole(data);
        vypisNaKonzolu(data);
        //kontrola konca hry z precitanych informacii
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

        //ak je koniec hry tak prestavam vymienat informacie
        if (data->koniecHry){
            break;
        }


        printf("Teraz si na rade, tvoj vstup bude odoslany na server.\n");

        //uz som na rade tak hadzem kockou a zapisujem to serveru
        citajVstupKonzola(data);

    }
}
/*
 * Zobrazenie pravidiel hry
 */
void zobrazPravidla(){
    //vypis pravidiel hry
    printf("\n");
    printf("************** PRAVIDLA HRY LUDO ***************\n");
    printf("Hra je pre 2 hracov\n");
    printf("Kym ma hrac vsetkych panacikov v domceku, musi hodit 6 aby nejakeho postavil na hraciu plochu\n");
    printf("Na jednom policku sa v jednom momente moze nachadzat len jeden panacik\n");
    printf("Ak chce hrac postavit panacika na policko, kde uz je nejaky iny panacik, tohto panacika vyhodi, ci uz je jeho alebo nie\n");
    printf("Hra sa konci ked sa jeden z hracov vzda alebo ma jeden z hracov vsetkych panacikov v cieli\n");
    printf("\n");
}