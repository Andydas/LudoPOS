#include "funkcie.h"


#define MAX_POCET_HRACOV 2

int main(int argc, char * argv[])
{
    bool koniecHry = false;
    pocetUsers = 0;
    userNaRade = 0;
    koniecHodnota = 0;
    int poleFigurok[8] = {0};
    int polePomocne[4] = {0};
    int onlineUsers[2] = {0};
    bool zahral = false;

    if (argc < 2)
    {
        fprintf(stderr,"Zly argument pre port %s \n", argv[0]);
        return 1;
    }

    cisloPortu = atoi(argv[1]);
    bzero((char*)&serverAdresa, sizeof(serverAdresa));
    serverAdresa.sin_family = AF_INET;
    serverAdresa.sin_addr.s_addr = INADDR_ANY;
    serverAdresa.sin_port = htons(cisloPortu);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Chyba pri vytvarani socketu");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serverAdresa, sizeof(serverAdresa)) < 0)
    {
        perror("Chyba pri bindovani adresy socketu");
        return 2;
    }

    listen(sockfd, 5);
    cli_len = sizeof(klientAdresa);

    DATA poleData[MAX_POCET_HRACOV];
    pthread_t threads[MAX_POCET_HRACOV];
    int userIDs[MAX_POCET_HRACOV];

    for (int i = 0; i < MAX_POCET_HRACOV; i++) {
        userIDs[i] = 0;
    }

    while (pocetUsers < MAX_POCET_HRACOV){
        int socketKlient = accept(sockfd, (struct sockaddr *) &klientAdresa, &cli_len);
        if (socketKlient < 0) {
            perror("Chyba pri accepte" );
            return 3;
        }

        onlineUSers[pocetUsers] = 1;
        bzero(buffer,256);

        DATA pomData;


        pomData.socketKlient = socketKlient;
        pomData.poleFigurok = poleFigurok;
        pomData.pomocnePole = polePomocne;
        pomData.onlineUsers = onlineUSers;
        pomData.ID = pocetUsers +1;
        pomData.n = n;
        pomData.userNaRade = &userNaRade;
        poleData->koniecHodnota = koniecHodnota;
        pomData.zahral = &zahral;
        pomData.koniecHry = &koniecHry;
        poleData[pocetUsers] = pomData;
        userIDs[pocetUsers] = socketKlient;

        pthread_create(&threads[pocetUsers], NULL, &komunikacia, &poleData[pocetUsers]);
        pocetUsers++;
    }
    close(sockfd);
    for(int i = 0; i < MAX_POCET_HRACOV; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}