#include "funkcie.h"

void* citajVstupy(void * param) {
    DATA* data = (DATA*) param;
    bool koniec = false;
    int ktoHral = 0;
    int hodKockou = 0;
    int ktoraFigurka = 0;
    char buffZapisovanie[256];
    char buffCitanie[256];
    *data->userNaRade = 1;

    while (!koniec) {

            if (pocetUsers >= 2){
                for (int i = 1; i <= pocetUsers; i++) {

                    buffZapisovanie[0] = i;

                    buffZapisovanie[1] = ktoHral;

                    buffZapisovanie[2] = hodKockou;

                    buffZapisovanie[3] = ktoraFigurka;

                    buffZapisovanie[4] = *data->userNaRade;
                    printf("Na 4 indexe: %d \n", *data->userNaRade);

                    data->n = write(i+3, buffZapisovanie, 255);
                    if (data->n < 0) {
                        perror("Nepodarilo sa nacitat do socketu");
                    }
                }
            }

            bzero(buffCitanie, 256);
            data->n = read(*data->userNaRade + 3, buffCitanie, 255);
            if (data->n < 0) {
                perror("Nepodarilo sa nacitat zo socketu");
            }
            printf("Sprava od klienta %d: %d\n", data->socketKlient - 3, buffCitanie[1]);
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
                printf("Na rade bude %d a online %d \n", *data->userNaRade, data->onlineUsers[*data->userNaRade - 1]);
            } else
            {
                *data->userNaRade = 2;
                printf("Na rade bude %d a online %d \n", *data->userNaRade, data->onlineUsers[*data->userNaRade - 1]);
            }
    }

    close(data->socketKlient);
}


