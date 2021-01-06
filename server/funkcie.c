#include "funkcie.h"

void* komunikacia(void * param) {
    DATA* data = (DATA*) param;
    //bool koniec = false;
    bool vyhodil = false;
    int ktoHral = 0;
    int hodKockou = 0;
    int ktoraFigurka = 0;
    int aktualnaPozicka = 0;
    int rezignacia = 0;
    int prvy = 0;
    int druhy = 0;
    data->koniecHodnota = 0;
    data->zahral = false;
    char buffCitanie[256];
    *data->userNaRade = 1;

    while (!(*data->koniecHry)) {
        vyhodil = false;
        if (pocetUsers >= 2) {
                zapis(data, ktoHral, hodKockou, ktoraFigurka);
        }

        bzero(buffCitanie, 256);
        data->n = read(*data->userNaRade + 3, buffCitanie, 255);
        if (data->n < 0) {
            perror("Nepodarilo sa nacitat zo socketu");
        }
        printf("Sprava od hraca %d:\n", buffCitanie[0]);
        ktoHral = buffCitanie[0];
        hodKockou = buffCitanie[1];
        ktoraFigurka = buffCitanie[2];
        rezignacia = buffCitanie[10];

        *data->koniecHry= rezignaciaF(ktoHral, hodKockou, ktoraFigurka, rezignacia, data);

        aktualnaPozicka = logikaHryF(ktoHral, hodKockou, ktoraFigurka, data, prvy, druhy);

        //kontrola vyhodenia panacika
        vyhodil = vyhodenieF(ktoHral, ktoraFigurka, aktualnaPozicka, data);


        if (hodKockou != 6) {
            if (!vyhodil) {
                if (pocetUsers == ktoHral ) {                           //&& data->onlineUsers[0] == 1
                        *data->userNaRade = 1;
                        if (!(*data->koniecHry))
                        printf("Na rade bude %d a online %d \n", *data->userNaRade, data->onlineUsers[*data->userNaRade - 1]);
                } else  {                                               //data->onlineUsers[1] == 1
                        *data->userNaRade = 2;
                        if (!(*data->koniecHry))
                            printf("Na rade bude %d a online %d \n", *data->userNaRade, data->onlineUsers[*data->userNaRade - 1]);
                }
            }
        }
    }

    close(data->socketKlient);
}

void zapis(DATA *data, int kto, int hod, int fig) {

    char buffZapisovanie[256];
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
        buffZapisovanie[10] = data->koniecHodnota;
        buffZapisovanie[11] = kto;
        buffZapisovanie[12] = hod;

        if (!data->zahral)
        {
            buffZapisovanie[13] = 0;
        } else if (kto == 1 && data->zahral)
        {
            buffZapisovanie[13] = fig;
        } else if (kto == 2 && data->zahral){
            buffZapisovanie[13] = fig + 4;
        }


        data->n = write(i + 3, buffZapisovanie, 255);
        if (data->n < 0) {
            perror("Nepodarilo sa nacitat do socketu");
        }
    }
}

bool rezignaciaF(int kto, int hod, int fig, int rez, DATA *data) {

    if (kto == 1 && rez == 1)
    {
        data->onlineUsers[kto - 1] = 0;
        data->koniecHodnota = 2;
        printf("Hrac 2 vyhrava, hrac 1 sa odpojil \n");
        zapis(data, kto, hod, fig);
        return true;
    }
    else if (kto == 2 && rez == 1)
    {
        data->onlineUsers[kto - 1] = 0;
        data->koniecHodnota = 1;
        printf("Hrac 1 vyhrava, hrac 2 sa odpojil \n");
        zapis(data, kto, hod, fig);
        return true;
    } else {
        return false;
    }
}

int logikaHryF(int kto, int hod, int fig, DATA *data, int prvy, int druhy) {
    if (kto == 1) {
        if (data->poleFigurok[fig - 1] == 0 && hod == 6) {
            data->poleFigurok[fig - 1] += 1;
            printf("Hrac 1  - hodil kockou (%d) vychadza z domceka a posuva figurkou (%d) \n", hod , fig);
            data->zahral = true;
        } else if (data->poleFigurok[fig - 1] == 0 && hod != 6) {
            printf("Hrac 1 - sa nepohol nikam \n");
            data->zahral = false;
        } else {
            if (data->poleFigurok[fig - 1] + hod <= 45) {
                if (data->poleFigurok[fig - 1] + hod == 45) {
                    data->poleFigurok[fig - 1] = 100;
                    printf("Hrac 1 - hodil kockou (%d) a figurka (%d) je na konci \n", hod, fig);
                    data->zahral = true;
                    for (int i = 0; i < 4; i++) {
                        if (data->poleFigurok[i] == 100){
                            prvy += 1;
                        }
                    }

                    printf("Hrac 1 - v cieli je %d. panacik\n", prvy);
                    if (prvy > 3)
                    {
                     data->koniecHodnota = 1;
                    }
                } else {
                    data->poleFigurok[fig - 1] += hod;
                    printf("Hrac 1 - hodil kockou (%d) a a posuva figurkou (%d) \n", hod, fig);
                    data->zahral = true;
                }
            } else {
                printf("Hrac 1 - preskocil si domcek, hodil si kockou (%d) \n", hod);
            }
        }
            return data->poleFigurok[fig - 1];

    } else if (kto == 2) {
        if (data->poleFigurok[(fig + 4) - 1] == 0 && hod == 6) {
            data->poleFigurok[(fig + 4) - 1] += 21;
            printf("Hrac 2  - hodil kockou (%d) vychadza z domceka a posuva figurkou (%d) \n", hod , fig + 4);
            data->pomocnePole[(fig) - 1] += 1;
            printf("Pomocne pole %d\n", data->pomocnePole[(fig) - 1]);
            data->zahral = true;
        } else if (data->poleFigurok[(fig + 4) - 1] == 0 && hod != 6) {
            printf("Hrac 2 - sa nepohol nikam \n");
            data->zahral = false;

        } else {
            if (data->pomocnePole[(fig) - 1] + hod <= 45) {
                if (data->pomocnePole[(fig) - 1] + hod == 45) {
                    data->pomocnePole[(fig) - 1] += hod;
                    //
                    data->poleFigurok[(fig + 4) - 1] = 100;
                    printf("Hrac 2 - hodil kockou (%d) a figurka (%d) je na konci \n", hod, fig + 4);
                    data->zahral = true;
                    for (int i = 4; i < 8; i++) {
                        if (data->poleFigurok[i] == 100){
                            druhy += 1;
                        }
                    }

                    printf("Hrac 2 - v cieli je %d. panacik\n", druhy);
                    if (druhy > 3)
                    {
                        data->koniecHodnota = 2;
                    }
                } else {
                    printf("Hrac 2 - hodil kockou (%d) a a posuva figurkou (%d) \n", hod, fig + 4);
                    data->pomocnePole[(fig) - 1] += hod; //40  //21
                    printf("Pomocne pole figurky %d je %d\n", fig, data->pomocnePole[(fig) - 1]);
                    data->poleFigurok[(fig + 4) - 1] += hod;   //20  //41
                    data->zahral = true;
                    if (data->pomocnePole[(fig) - 1] >= 21 && data->poleFigurok[(fig + 4) - 1] > 20 && data->pomocnePole[(fig) - 1] <= 40)          //ked prejde cez 40
                    {
                        data->poleFigurok[(fig + 4) - 1] -= 40;
                    }

                    if (data->pomocnePole[(fig) - 1] > 40)
                    {
                        data->poleFigurok[(fig + 4) - 1] = data->pomocnePole[(fig) - 1] + 5;
                    }
                }
            } else {
                printf("Hrac 2 - preskocil si domcek, hodil si kockou (%d) \n", hod);
            }
        }

            return data->poleFigurok[(fig + 4) - 1];

    }
}

bool vyhodenieF(int kto, int fig, int akt, DATA *data) {
    if (kto == 1) {
        for (int i = 0; i < 8; i++) {
            if (fig != i+1 && akt != 100 && akt != 0)
            {
                if (akt == data->poleFigurok[i])
                {
                    data->poleFigurok[i] = 0;
                    printf("Hrac 1 - vyhodil panacika, konkretne figurku %d \n", i+1);
                    return true;
                }
            }
        }
    } else if (kto == 2) {
        for (int i = 0; i < 8; i++) {
            if (fig+4 != i+1 && akt != 100 && akt != 0) {
                if (akt == data->poleFigurok[i]) {
                    data->poleFigurok[i] = 0;
                    printf("Hrac 2 - vyhodil panacika, konkretne figurku %d \n", i + 1);
                    return true;
                }
            }
        }
    } else {
        return false;
    }
}




