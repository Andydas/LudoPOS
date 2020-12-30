#include "funkcie.h"

void* citajVstupy(void * param) {
    DATA* data = (DATA*) param;
    bool koniec = false;
    bzero(data->buffer, 256);

    //odoslanie ID klientovi
    char buff[256];
    sprintf(buff, "%d", data->ID );
    data->n = write(data->socketKlient, buff, strlen(buff) + 1);


    while (!koniec) {
        bzero(data->buffer, 256);
        data->n = read(data->socketKlient, data->buffer, 255);
        if (data->n < 0) {
            perror("Nepodarilo sa nacitat zo socketu");
            //return 4;
        }

        printf("Sprava od klienta %d: %s\n", data->ID, data->buffer);

        if (data->buffer[0] == '0')
            koniec = true;

        const char *msg = "Dostal som tvoju spravu Andydas :)"  ;
        bzero(data->buffer, 256);
        data->n = write(data->socketKlient, msg, strlen(msg) + 1);
        if (data->n < 0) {
            perror("Nepodarilo sa zapisat")      ;
            //return 5;
        }

    }
    pocetUsers--;
    close(data->socketKlient);
}
