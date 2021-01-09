#include "funkcie.h"

  /*
  Metoda vlakna v ktorej dochadza k vymene informacii a logike po hracovom tahu.
  */
void* komunikacia(void * param) {
    DATA* data = (DATA*) param;
    bool vyhodil = false;
    bool zaciatok = true;
    int ktoHral = 0;
    int hodKockou = 0;
    int ktoraFigurka = 0;
    int aktualnaPozicka = 0;
    int rezignacia = 0;
    data->vitaz = 0;
    data->zahral = false;
    char buffCitanie[BUFF_SIZE];
    *data->userNaRade = HRAC_1;

    while (*data->koniecHry != 1) {

        //Informacia pre klientov len na zaciatku hry.
        if (pocetUsers >= MAX_POCET_HRACOV && zaciatok && data->socketKlient == SOCKET_ID_1) {
            zapis(data, ktoHral, hodKockou, ktoraFigurka);
            zaciatok = false;
        } else if (pocetUsers < MAX_POCET_HRACOV && zaciatok){
            continue;
        }

        //Zamknutie vlakna.
        pthread_mutex_lock(data->mut);

        //Ak su napojeny vsetci hraci hra pokracuje ak nie caka sa na dalsich hracov.
        if (pocetUsers >= MAX_POCET_HRACOV) {

            //Kontrola ci uz hra skoncila.
            if (*data->koniecHry ==  1){
                break;
            }

            //Podmienka pre vlakna, aby vedeli kedy maju cakat na signal.
            if (*data->userNaRade != data->socketKlient - 3 && data->socketKlient == SOCKET_ID_1)
            {
                pthread_cond_wait(data->prve, data->mut);
            } else if (*data->userNaRade != data->socketKlient - 3 && data->socketKlient == SOCKET_ID_2)
            {
                pthread_cond_wait(data->druhe, data->mut);
            }

            vyhodil = false;

            //Kontrola ci uz hra skoncila.
            if (*data->koniecHry == 1){
                break;
            }
        } else {
            if (*data->userNaRade != data->socketKlient - 3 && data->socketKlient == SOCKET_ID_1)
            {
                pthread_cond_signal(data->druhe);
            } else if (*data->userNaRade != data->socketKlient - 3 && data->socketKlient == SOCKET_ID_2)
            {
                pthread_cond_signal(data->prve);
            }
            pthread_mutex_unlock(data->mut);
            continue;
        }

        //Vynulovanie a nasledne citanie buffra zo socketu od klienta.
        bzero(buffCitanie, BUFF_SIZE);
        data->n = read(*data->userNaRade + 3, buffCitanie, BUFF_SIZE - 1);
        if (data->n < 0) {
            perror("Nepodarilo sa nacitat zo socketu");
            break;
        }
        ktoHral = buffCitanie[0];
        hodKockou = buffCitanie[1];
        ktoraFigurka = buffCitanie[2];
        rezignacia = buffCitanie[10];

        //Metoda rezignaciaF kontroluje a vracia ci niekto z hracov nerezignoval.
        *data->koniecHry = rezignaciaF(ktoHral, hodKockou, ktoraFigurka, rezignacia, data);

        //Ak hra este neskoncila a nikto nerezignoval pokracuje sa dalej.
        if (*data->koniecHry != 1)
        {
            //Vykonavanie logiky hry so zistenim ktora figurka zmenila poziciu.
            aktualnaPozicka = logikaHryF(ktoHral, hodKockou, ktoraFigurka, data);

            //Ak este jeden z hracov nema vsetkych hracov v domceku pokracuje sa dalej.
            if (*data->koniecHry != 1) {

                //Kontorla ci nedoslo k vyhodeniu nejakej figurky z hracieho pola.
                vyhodil = vyhodenieF(ktoHral, ktoraFigurka, aktualnaPozicka, data);

                //Ak hrac hodil 6 alebo vyhodil panacika moze hadzat znova.
                if (hodKockou != 6) {
                    if (!vyhodil) {
                        if (pocetUsers == ktoHral) {
                            *data->userNaRade = HRAC_1;
                        } else {
                            *data->userNaRade = HRAC_2;
                        }
                    }
                }
            } else {
                zapis(data, ktoHral, hodKockou, ktoraFigurka);
                break;
            }

            //Zapis klientom ake zmeny v hre nastali.
            zapis(data, ktoHral, hodKockou, ktoraFigurka);

        } else {
            break;
        }

        //Podmienka pre vlakna, ak jedno skoncilo aby poslalo signal druhemu ze je na rade.
        if (*data->userNaRade != data->socketKlient - 3 && data->socketKlient == SOCKET_ID_1)
        {
            pthread_cond_signal(data->druhe);
        } else if (*data->userNaRade != data->socketKlient - 3 && data->socketKlient == SOCKET_ID_2)
        {
            pthread_cond_signal(data->prve);
        }
        //Odomknutie mutexu.
        pthread_mutex_unlock(data->mut);
    }

    //Odoslanie signalu a odomknutie mutexu.
    ukonci(data);
    printf("KONCI VLAKNO %d\n", data->socketKlient - 3);
    return NULL;
}

  /*
  Metoda ktora zapise do buffra vsetky potrebne informacie o hracovom tahu.
  */
void zapis(DATA *data, int kto, int hod, int fig) {
    char buffZapisovanie[BUFF_SIZE];
    bzero(buffZapisovanie, BUFF_SIZE);
    for (int i = 1; i <= pocetUsers; i++) {
        printf("ZAPISUJEM PRE %d HRACA , SOM %d. VLAKNO\n", i, data->socketKlient-3);
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
        buffZapisovanie[10] = data->vitaz;
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

        data->n = write(i + 3, buffZapisovanie, BUFF_SIZE - 1);
        if (data->n < 0) {
            perror("Nepodarilo sa nacitat do socketu");
        }
    }
}

  /*
  Metoda ktora kontroluje ci jeden z hracov nerezignoval.
  */
int rezignaciaF(int kto, int hod, int fig, int rez, DATA *data) {

    if (kto == HRAC_1 && rez == 1)
    {
        data->vitaz = HRAC_2;
        printf("Hrac 2 vyhrava, hrac 1 sa odpojil \n");
        zapis(data, kto, hod, fig);
        return 1;
    }
    else if (kto == HRAC_2 && rez == 1)
    {
        data->vitaz = HRAC_1;
        printf("Hrac 1 vyhrava, hrac 2 sa odpojil \n");
        zapis(data, kto, hod, fig);
        return 1;
    } else {
        return 0;
    }
}

  /*
  Metoda ktora spravuje logiku hracovho tahu spolu s kontrolou ci uz ma vsetky figurky v domceku.
  */
int logikaHryF(int kto, int hod, int fig, DATA *data) {
    int prvyCiel = 0;
    int druhyCiel = 0;
    if (kto == HRAC_1) {
        if (data->poleFigurok[fig - 1] == 0 && hod == 6) {
            data->poleFigurok[fig - 1] = 1;
            data->zahral = true;
        } else if (data->poleFigurok[fig - 1] == 0 && hod != 6) {
            data->zahral = false;
        } else {
            if (data->poleFigurok[fig - 1] + hod <= HRACIA_PLOCHA_STROP) {
                if (data->poleFigurok[fig - 1] + hod == HRACIA_PLOCHA_STROP) {
                    data->poleFigurok[fig - 1] = CIEL_HODNOTA;
                    data->zahral = true;
                    for (int i = 0; i < 4; i++) {
                        if (data->poleFigurok[i] == CIEL_HODNOTA){
                            prvyCiel += 1;
                        }
                    }
                    if (prvyCiel > 3)
                    {
                        data->vitaz = HRAC_1;
                        *data->koniecHry = 1;
                    }
                } else {
                    data->poleFigurok[fig - 1] += hod;
                    data->zahral = true;
                }
            } else {
                data->zahral = false;
            }
        }
        return data->poleFigurok[fig - 1];
    } else if (kto == HRAC_2) {
        if (data->poleFigurok[(fig + 4) - 1] == 0 && hod == 6) {
            data->poleFigurok[(fig + 4) - 1] = 21;
            data->pomocnePole[(fig) - 1] = 1;
            data->zahral = true;
        } else if (data->poleFigurok[(fig + 4) - 1] == 0 && hod != 6) {
            data->zahral = false;
        } else {
            if (data->pomocnePole[(fig) - 1] + hod <= HRACIA_PLOCHA_STROP) {
                if (data->pomocnePole[(fig) - 1] + hod == HRACIA_PLOCHA_STROP) {
                    data->pomocnePole[(fig) - 1] += hod;
                    data->poleFigurok[(fig + 4) - 1] = CIEL_HODNOTA;
                    data->zahral = true;
                    for (int i = 4; i < 8; i++) {
                        if (data->poleFigurok[i] == CIEL_HODNOTA){
                            druhyCiel += 1;
                        }
                    }
                    if (druhyCiel > 3)
                    {
                        data->vitaz = HRAC_2;
                        *data->koniecHry = 1;
                    }
                } else {
                    data->pomocnePole[(fig) - 1] += hod;
                    data->poleFigurok[(fig + 4) - 1] += hod;
                    data->zahral = true;
                    if (data->pomocnePole[(fig) - 1] >= 21 && data->poleFigurok[(fig + 4) - 1] > 20 && data->pomocnePole[(fig) - 1] <= 40)
                    {
                        data->poleFigurok[(fig + 4) - 1] -= 40;
                    }

                    if (data->pomocnePole[(fig) - 1] > 40)
                    {
                        data->poleFigurok[(fig + 4) - 1] = data->pomocnePole[(fig) - 1] + 5;
                    }
                }
            } else {
                data->zahral = false;
            }
        }
        return data->poleFigurok[(fig + 4) - 1];
    }
}


  /*
  Metoda ktora kontroluje ci nenastalo vyhodenie figurky ak ano vratenie figurky na start.
  */
bool vyhodenieF(int kto, int fig, int akt, DATA *data) {
    if (kto == HRAC_1) {
        for (int i = 0; i < 8; i++) {
            if (fig != i+1 && akt != CIEL_HODNOTA && akt != 0)
            {
                if (akt == data->poleFigurok[i])
                {
                    //printf("Hrac %d vyhodil s panacikom %d na pozicii %d panacika %d na pozicii %d\n", kto, fig, data->poleFigurok[fig-1], i+1, data->poleFigurok[i]);
                    data->poleFigurok[i] = 0;
                    return true;
                }
            }
        }
    } else if (kto == HRAC_2) {
        for (int i = 0; i < 8; i++) {
            if (fig+4 != i+1 && akt != CIEL_HODNOTA && akt != 0) {
                if (akt == data->poleFigurok[i]) {
                    //printf("Hrac %d vyhodil s panacikom %d na pozicii %d panacika %d na pozicii %d\n", kto, fig, data->poleFigurok[fig+4-1], i+1, data->poleFigurok[i]);
                    data->poleFigurok[i] = 0;
                    return true;
                }
            }
        }
    } else {
        return false;
    }
}

  /*
  Metoda ktora na konci hry odosle signal druhemu vlaknu a odomkne mutex.
  */
void ukonci(DATA *data) {
    if (data->socketKlient == SOCKET_ID_1) {
        pthread_cond_signal(data->druhe);
    } else {
        pthread_cond_signal(data->prve);
    }
    pthread_mutex_unlock(data->mut);
}






