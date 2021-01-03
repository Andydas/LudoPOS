#include "funkcie.h"

void* komunikacia(void * param) {
    DATA* data = (DATA*) param;
    bool koniec = false;
    bool vyhodil = false;
    bool hrac1 = false;
    int ktoHral = 0;
    int hodKockou = 0;
    int ktoraFigurka = 0;
    int aktualnaPozicka = 0;
    char buffZapisovanie[256];
    char buffCitanie[256];
    *data->userNaRade = 1;

    while (!koniec) {
        vyhodil = false;
        if (pocetUsers >= 2) {
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

                data->n = write(i + 3, buffZapisovanie, 255);
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
        printf("Sprava od klienta %d: %d\n", buffCitanie[0], buffCitanie[1]);
        ktoHral = buffCitanie[0];
        hodKockou = buffCitanie[1];
        ktoraFigurka = buffCitanie[2];

        if (ktoHral == 1) {
            hrac1 = true;
        } else {
            hrac1 = false;
        }

        if (hrac1) {
            if (data->poleFigurok[ktoraFigurka - 1] == 0 && hodKockou == 6) {
                data->poleFigurok[ktoraFigurka - 1] += 1;
                printf("Som vo vetve Hrac 1 vypiskolko hodil %d a figurka %d \n", hodKockou , ktoraFigurka);
            } else if (data->poleFigurok[ktoraFigurka - 1] == 0 && hodKockou != 6) {
                printf("Klient je ratard \n");
            } else {
                if (data->poleFigurok[ktoraFigurka - 1] + hodKockou <= 45) {
                    if (data->poleFigurok[ktoraFigurka - 1] + hodKockou == 45) {
                        data->poleFigurok[ktoraFigurka - 1] = 100;
                        printf("Figurka c. %d je na konci \n", ktoraFigurka);
                    } else {
                        data->poleFigurok[ktoraFigurka - 1] += hodKockou;
                        printf("Gajdy\n");
                    }
                } else {
                    printf("Preskocil si domcek 1 \n");
                }
            }
            if (data->poleFigurok[(ktoraFigurka) - 1] > 40 || data->poleFigurok[(ktoraFigurka) - 1] == 0)
            {
                aktualnaPozicka = 200;
            } else {
                aktualnaPozicka = data->poleFigurok[ktoraFigurka - 1];          //34
            }
        } else {
            if (data->poleFigurok[(ktoraFigurka + 4) - 1] == 0 && hodKockou == 6) {
                data->poleFigurok[(ktoraFigurka + 4) - 1] += 21;
                printf("Som vo vetve Hrac 2 vypis kolkko hodil %d s ktorym pnakom %d \n", hodKockou, ktoraFigurka + 4);
                data->pomocnePole[(ktoraFigurka) - 1] += 1;
                printf("Pomocne pole %d\n", data->pomocnePole[(ktoraFigurka) - 1]);
            } else if (data->poleFigurok[(ktoraFigurka + 4) - 1] == 0 && hodKockou != 6) {
                printf("Klient je ratard \n");
            } else {
                if (data->pomocnePole[(ktoraFigurka) - 1] + hodKockou <= 45) {
                    if (data->pomocnePole[(ktoraFigurka) - 1] + hodKockou == 45) {
                        data->pomocnePole[(ktoraFigurka) - 1] += hodKockou;
                        //
                        data->poleFigurok[(ktoraFigurka + 4) - 1] = 100;
                        printf("Figurka c. %d je na konci \n", ktoraFigurka);
                    } else {
                        data->pomocnePole[(ktoraFigurka) - 1] += hodKockou;         //43
                        printf("Pomocne pole figurky %d je %d\n", ktoraFigurka, data->pomocnePole[(ktoraFigurka) - 1]);
                        data->poleFigurok[(ktoraFigurka + 4) - 1] += hodKockou;      //23
                        //
                        if (data->pomocnePole[(ktoraFigurka) - 1] > 20 && data->poleFigurok[(ktoraFigurka + 4) - 1] > 21 && data->pomocnePole[(ktoraFigurka) - 1] <= 40)
                        {
                            data->poleFigurok[(ktoraFigurka + 4) - 1] -= 40;
                        }

                        if (data->pomocnePole[(ktoraFigurka) - 1] > 40)
                        {
                            data->poleFigurok[(ktoraFigurka + 4) - 1] = data->pomocnePole[(ktoraFigurka) - 1] + 5;
                        }
                    }
                } else {
                    printf("Preskocil si domcek 2 \n");
                }
            }
            if (data->pomocnePole[(ktoraFigurka + 4) - 1] > 45 || data->pomocnePole[(ktoraFigurka + 4) - 1] == 0)
            {
                aktualnaPozicka = 200;
            } else {
                aktualnaPozicka = data->poleFigurok[(ktoraFigurka + 4) - 1];
            }
        }

        //kontrola vyhodenia panacika
        if (hrac1) {
            for (int i = 4; i < 8; i++) {
                if (aktualnaPozicka == data->poleFigurok[i])
                {
                    data->poleFigurok[i] = 0;
                    printf("Vyhodil si panacika hracovi 2, konkretne figurku %d \n", i+1);
                    vyhodil = true;
                    break;
                }
            }
        } else {
            for (int i = 0; i < 4; i++) {
                if (aktualnaPozicka == data->poleFigurok[i])
                {
                    data->poleFigurok[i] = 0;
                    printf("Vyhodil si panacika hracovi 1, konkretne figurku %d \n", i+1);
                    vyhodil = true;
                    break;
                }
            }
        }

            /*if (aktualnaPozicka > 44)
            {
                data->poleFigurok[ktoraFigurka * ktoHral] -= hodKockou;
            } else if (aktualnaPozicka > 40 && aktualnaPozicka < 44)
            {
                data->poleFigurok[ktoraFigurka * ktoHral] = 100;
            }*/

            /*for (int i = 1; i <= 8; i++) {
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
            }*/


            if (hodKockou != 6) {
                if (!vyhodil) {
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
            }



        if (buffCitanie[10] == '1')
        {
            koniec = true;
            data->onlineUsers[ktoHral - 1] = 0;
        }
    }

    close(data->socketKlient);
}




