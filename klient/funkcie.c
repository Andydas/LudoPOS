#include "funkcie.h"
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void komunikacia(void* param) {
    DATA* data = (DATA*) param;
    bool koniec = false;

    char buffCitanie[265];
    char buffZapisovanie[265];
    bzero(buffCitanie, 256);
    bzero(buffZapisovanie, 256);



    //posielanie sprav
    while (!koniec) {

        //citanie spravy od servera
        //bzero(buffCitanie, 256);
        int uspech = read(data->sock, buffCitanie, 255);
        if (uspech < 0) {
            perror("Chyba pri citani zo socketu\n");//kontrola co precitalo
        }

        //zapisem si co som precital
        data->ID = buffCitanie[0];
        data->ktoJeNaRade = buffCitanie[4];
        printf("Od servera som zistil tieto informacie:\n");
        printf("Moje ideeecko je: %d\n", data->ID);
        printf("Naposledy hral id: %d\n", buffCitanie[1]);
        printf("Posuval panacika %d o %d policok.\n", buffCitanie[3], buffCitanie[2]);
        printf("Teraz je na rade: %d\n", data->ktoJeNaRade);

        //kym nie som na rade tak cakam
        while (data->ID != data->ktoJeNaRade){
            //citanie spravy od servera
            //bzero(buffCitanie, 256);
            int uspech = read(data->sock, buffCitanie, 255);
            if (uspech < 0) {
                perror("Chyba pri citani zo socketu\n");//kontrola co precitalo
            }

            //zapisem si co som precital
            data->ID = buffCitanie[0];
            data->ktoJeNaRade = buffCitanie[4];
            printf("Od servera som zistil tieto informacie:\n");
            printf("Moje id je: %d\n", data->ID);
            printf("Naposledy hral id: %d\n", buffCitanie[1]);
            printf("Posuval panacika %d o %d policok.\n", buffCitanie[2], buffCitanie[3]);
            printf("Teraz je na rade: %d\n", data->ktoJeNaRade);
        }

        printf("Teraz si na rade, tvoj vstup bude odoslany na server.\n");

        //hadzem koockou a zapisujem to serveru
        bool okVstup = false;
        while (!okVstup){
            printf("O kolko posuvas?: ");
            int hodeneCislo;
            scanf("%d", &hodeneCislo);
            bzero(buffZapisovanie, 256);

            if (hodeneCislo > 0 && hodeneCislo < 7){
                if (hodeneCislo == 0){
                    koniec = true;
                    buffZapisovanie[10] = 1;
                }

                //zapisem moje ID
                buffZapisovanie[0] = data->ID;
                //zapisem hod kockou
                buffZapisovanie[1] = hodeneCislo;
                //zapisem ktorym panacikom posuvam
                buffZapisovanie[2] = data->ID;

                data->n = write(data->sock, buffZapisovanie, 255);
                if (data->n < 0){
                    perror("Chyba pri zapisovani do socketu");
                    //return 5;
                }
                printf("Uspesne zapisane do socketu!\n");
                okVstup = true;
            } else {
                printf("Neplany vstup.\n");
            }
        }

    }

}

void zapisuj(int id, int hod, int panacik) {

}

char *citaj() {
    return NULL;
}
