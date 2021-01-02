#include "funkcie.h"

void* komunikacia(void * param) {
    DATA* data = (DATA*) param;
    bool koniec = false;
    bool vyhodil = false;
    int ktoHral = 0;
    int hodKockou = 0;
    int ktoraFigurka = 0;
    int aktualnaPozicka = 0;
    char buffZapisovanie[256];
    char buffCitanie[256];
    *data->userNaRade = 1;

    while (!koniec) {

            if (pocetUsers >= 2){
                for (int i = 1; i <= pocetUsers; i++) {

                    buffZapisovanie[0] = i;
                    buffZapisovanie[1] = *data->userNaRade;

                    buffZapisovanie[2] = data->poleFigurok[0];       //1
                    buffZapisovanie[3] = data->poleFigurok[1];       //2
                    buffZapisovanie[4] = data->poleFigurok[2];       //3
                    buffZapisovanie[5] = data->poleFigurok[3];       //4
                    buffZapisovanie[6] = data->poleFigurok[4];       //5
                    buffZapisovanie[7] = data->poleFigurok[5];       //6
                    buffZapisovanie[8] = data->poleFigurok[6];       //7
                    buffZapisovanie[9] = data->poleFigurok[7];       //8

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


            data->poleFigurok[ktoraFigurka * ktoHral] += hodKockou;
            aktualnaPozicka = data->poleFigurok[ktoraFigurka * ktoHral];

            if (aktualnaPozicka > 44)
            {
                data->poleFigurok[ktoraFigurka * ktoHral] -= hodKockou;
            } else if (aktualnaPozicka > 40 && aktualnaPozicka < 44)
            {
                data->poleFigurok[ktoraFigurka * ktoHral] = 100;
            }

            for (int i = 1; i <= 8; i++) {
                if (ktoraFigurka * ktoHral <= 4 && i >= 4)
                {
                    if (data->poleFigurok[i-1] == aktualnaPozicka){
                        data->poleFigurok[i-1] = 0;
                        vyhodil = true;
                    }
                }  else if (ktoraFigurka * ktoHral > 4 && i < 4) {
                    if (data->poleFigurok[i-1] == aktualnaPozicka){
                        data->poleFigurok[i-1] = 0;
                        vyhodil = true;
                    }
                }
            }

            if (hodKockou != 6 || !vyhodil) {
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


        if (buffCitanie[10] == '1')
        {
            koniec = true;
            data->onlineUsers[ktoHral - 1] = 0;
        }
    }

    close(data->socketKlient);
}




