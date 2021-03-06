#include "funkcie.h"

int main(int argc, char * argv[])
{
    int koniecHry = 0;
    pocetUsers = 0;
    userNaRade = 0;
    koniecHodnota = 0;
    int poleFigurok[FIGURKY] = {0};
    int polePomocne[POMOCNE_POLE] = {0};
    bool zahral = false;
    DATA poleData[MAX_POCET_HRACOV];
    pthread_t threads[MAX_POCET_HRACOV];
    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t prve = PTHREAD_COND_INITIALIZER;
    pthread_cond_t druhe = PTHREAD_COND_INITIALIZER;

    //Kontrola ci sa v argumente nachadza port na ktorom bude server pocuvat.
    if (argc < 2)
    {
        fprintf(stderr,"Zly argument pre port %s \n", argv[0]);
        return 1;
    }

    //Nulovanie a inicializacia sietovej adresy.
    cisloPortu = atoi(argv[1]);
    bzero((char*)&serverAdresa, sizeof(serverAdresa));
    serverAdresa.sin_family = AF_INET;
    serverAdresa.sin_addr.s_addr = INADDR_ANY;
    serverAdresa.sin_port = htons(cisloPortu);

    //Vytvorenie socketu v danej domene.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Chyba pri vytvarani socketu");
        return 1;
    }

    //Priradenie sietovej adresy vytvorenemu socketu.
    if (bind(sockfd, (struct sockaddr*)&serverAdresa, sizeof(serverAdresa)) < 0)
    {
        perror("Chyba pri bindovani adresy socketu");
        return 2;
    }

    //Pripravenie socketu pre príjmanie spojení od klientov.
    listen(sockfd, 5);
    cli_len = sizeof(klientAdresa);

    //Prijem spojeni od klienta a naplnenie datovej struktury
    while (pocetUsers < MAX_POCET_HRACOV){
        int socketKlient = accept(sockfd, (struct sockaddr *) &klientAdresa, &cli_len);
        if (socketKlient < 0) {
            perror("Chyba pri accepte" );
            return 3;
        }
        bzero(buffer,BUFF_SIZE);
        DATA pomData;
        pomData.socketKlient = socketKlient;
        pomData.poleFigurok = poleFigurok;
        pomData.pomocnePole = polePomocne;
        pomData.n = n;
        pomData.userNaRade = &userNaRade;
        poleData->vitaz = koniecHodnota;
        pomData.zahral = &zahral;
        pomData.koniecHry = &koniecHry;
        pomData.mut = &mut;
        pomData.prve = &prve;
        pomData.druhe = &druhe;
        poleData[pocetUsers] = pomData;
        pthread_create(&threads[pocetUsers], NULL, &komunikacia, &poleData[pocetUsers]);
        pocetUsers++;
    }

    //Zatvorenie hlavneho socketu
    close(sockfd);
    for(int i = 0; i < MAX_POCET_HRACOV; i++) {
        pthread_join(threads[i], NULL);
    }

    //Zatvorenie socketov klientov
    printf("ZATRVARAM MAIN SOCKET 1\n");
    close(SOCKET_ID_1);
    printf("ZATRVARAM MAIN SOCKET 2\n");
    close(SOCKET_ID_2);

    //Zametanie po sebe
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&prve);
    pthread_cond_destroy(&druhe);

    return 0;
}