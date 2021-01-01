#include "funkcie.h"

void* citajVstupy(void * param) {
    DATA* data = (DATA*) param;
    bool koniec = false;
    bzero(data->buffer, 256);

    //odoslanie ID klientovi
    char buff[256];
    sprintf(buff, "%d", data->ID );
    //zapisovanie ID
    data->n = write(data->socketKlient, buff, strlen(buff) + 1);
    if (data->n < 0) {
        perror("Nepodarilo sa nacitat do socketu");
    }

    while (!koniec) {

        //odosielanie klientom kto je na rade
        odosliUserNaRade();

        //citanie spravy od klienta
        bzero(data->buffer, 256);
        data->n = read(data->socketKlient, data->buffer, 255);
        if (data->n < 0) {
            perror("Nepodarilo sa nacitat zo socketu");
        }
        printf("Sprava od klienta %d: %s\n", data->socketKlient, data->buffer);

        //kontrola ci sa ma socket uzavriet
        if (data->buffer[0] == '0')
            koniec = true;

        const char *msg = "Dostal som tvoju spravu Andydas :)";
        bzero(data->buffer, 256);

        data->n = write(data->socketKlient, msg, strlen(msg) + 1);
        if (data->n < 0) {
            perror("Nepodarilo sa zapisat");
        }
        if (data->buffer[0] == '0')
            pocetUsers--;


        //posunie na rad dalsieho usera
        if (pocetUsers == userNaRade)
            userNaRade = 1;
        else
            userNaRade++;

    }

    close(data->socketKlient);
}

void odosliUserNaRade() {
    char uspech[256];
    char neuspech[256];
    sprintf(uspech, "%d", 1 );
    sprintf(neuspech, "%d", 0 );

    for (int i = 1; i <= pocetUsers; i++) {
        if (userNaRade == i) {
            write(i+3, uspech, strlen(uspech) + 1);
        } else {
            write(i+3, neuspech, strlen(neuspech) + 1);
        }
    }

}
