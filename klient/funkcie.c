#include "funkcie.h"
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void hodKockou(void* param) {
    DATA* data = (DATA*) param;
    bool koniec = false;


    //zistenie mojho ID od serveru
    bzero(data->buffer,256);
    read(data->sock, data->buffer, 256);
    char* idecko = data->buffer;
    data->ID = atoi(idecko);
    printf("Moje ID je %d\n", data->ID);



    //posielanie sprav
    while (!koniec) {


        //klient caka kym je na rade, az potom posiela spravy
        bzero(data->buffer,256);
        read(data->sock, data->buffer, 256);
        char* somNaRade = data->buffer;
        data->somNaRade = atoi(somNaRade);
        //printf("Som na rade: %d", data->somNaRade);

        if (data->somNaRade == 1) {//klient moze posielat spravy
            printf("Napis spravu: ");

            bzero(data->buffer,256);
            fgets(data->buffer, 255, stdin);

            data->n = write(data->sock, data->buffer, strlen(data->buffer));
            if (data->n < 0)
            {
                perror("Chyba pri zapisovani do socketu");
                //return 5;
            }
            printf("Uspesne zapisane do socketu!\n");

            if (data->buffer[0] == '0')
                koniec = true;

            bzero(data->buffer,256);
            data->n = read(data->sock, data->buffer, 255);
            if (data->n < 0)
            {
                perror("Chyba pri citani zo socketu");
                //return 6;
            }
            printf("Uspesne citanie do socketu!\n");
            printf("%s\n",data->buffer);
        }



    }

}