#include "funkcie.h"

void* citajVstupy(void * param) {
    DATA* data = (DATA*) param;
    bool koniec = false;
    //bool zaciatok = true;
    int ktoHral = 0;
    int hodKockou = 0;
    int ktoraFigurka = 0;
    //int predchadzajuci = 0;
    char buffZapisovanie[256];
    char buffCitanie[256];
    data->userNaRade++;

    //odoslanie ID klientovi
    //sprintf(buff[0], "%d", data->ID );

    //zapisovanie ID
    /*data->n = write(data->socketKlient, buff, strlen(buff) + 1);
    if (data->n < 0) {
        perror("Nepodarilo sa nacitat do socketu");
    }
    printf("Pocet uzivatrrelov %d \n", pocetUsers);*/

    while (!koniec) {

        //printf("Pocet uzivatrrelov %d \n", pocetUsers);
        if (pocetUsers <= 1) {
            for (int i = 1; i <= pocetUsers; i++) {

                buffZapisovanie[0] = i;

                buffZapisovanie[1] = 0;

                buffZapisovanie[2] = 0;

                buffZapisovanie[3] = 0;

                buffZapisovanie[4] = 1;
            }

            data->n = write(data->socketKlient, buffZapisovanie, strlen(buffZapisovanie) + 1);
            if (data->n < 0) {
                perror("Nepodarilo sa nacitat do socketu");
            }

        } else {

            //odoslanie vsetko klientovi
            for (int i = 1; i <= pocetUsers; i++) {

                    buffZapisovanie[0] = i;

                    buffZapisovanie[1] = ktoHral;

                    buffZapisovanie[2] = hodKockou;

                    buffZapisovanie[3] = ktoraFigurka;

                    buffZapisovanie[4] = 1;

                data->n = write(data->socketKlient, buffZapisovanie, strlen(buffZapisovanie) + 1);
                if (data->n < 0) {
                    perror("Nepodarilo sa nacitat do socketu");
                }
            }



            //citanie spravy od klienta
            bzero(buffCitanie, 256);
            data->n = read(data->socketKlient, buffCitanie, 255);
            if (data->n < 0) {
                perror("Nepodarilo sa nacitat zo socketu");
            }
            printf("Sprava od klienta %d: %s\n", data->socketKlient, buffCitanie);
            ktoHral = buffCitanie[0];
            hodKockou = buffCitanie[1];
            ktoraFigurka = buffCitanie[2];

            if (buffCitanie[10] == '1')
            {
                koniec = true;
                data->onlineUsers[ktoHral - 1] = 0;
            }

            if (pocetUsers == ktoHral)
            {
                *data->userNaRade = 1;
                printf("Na rade bude %d a online %d \n", data->userNaRade, data->userNaRade[*data->userNaRade - 1]);
            } else
            {
                data->userNaRade++;
                printf("Na rade bude %d a online %d \n", data->userNaRade, data->userNaRade[*data->userNaRade - 1]);
            }
        }



        //odosielanie klientom kto je na rade
        //odosliUserNaRade();

        //citanie spravy od klienta
       /* bzero(buff, 256);
        data->n = read(data->socketKlient, buff, 255);
        if (data->n < 0) {
            perror("Nepodarilo sa nacitat zo socketu");
        }
        printf("Sprava od klienta %d: %s\n", data->socketKlient, buff);*/

        //kontrola ci sa ma socket uzavriet
        /*if (buff[0] == '0')
            koniec = true;*/

        /*const char *msg = "Dostal som tvoju spravu Andydas :)";
        bzero(buff, 256);

        data->n = write(data->socketKlient, msg, strlen(msg) + 1);
        if (data->n < 0) {
            perror("Nepodarilo sa zapisat");
        }*/



        //posunie na rad dalsieho usera
        /*if (pocetUsers == userNaRade)
        {
            userNaRade = 1;
            printf("Na rade bude %d a online %d \n", userNaRade, onlineUSers[userNaRade - 1]);
        } else
        {
            userNaRade++;
            printf("Na rade bude %d a online %d \n", userNaRade, onlineUSers[userNaRade - 1]);
        }*/

    }

    close(data->socketKlient);
}

void odosliUserNaRade() {
    char uspech[256];
    char neuspech[256];

    uspech[3] = (char)userNaRade;

    //sprintf(neuspech, "%d", 0 );

    for (int i = 1; i <= pocetUsers; i++) {
        write(i+3, uspech, strlen(uspech) + 1);
    }

    /*for (int i = 1; i <= pocetUsers; i++) {
        if (onlineUSers[i-1] == 1){
            if (userNaRade == i) {
                write(i+3, uspech, strlen(uspech) + 1);
            } else {
                write(i+3, neuspech, strlen(neuspech) + 1);
            }
        }
    }*/

}
