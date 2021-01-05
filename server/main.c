#include "funkcie.h"

int main(int argc, char * argv[])
{
    pocetUsers = 0;
    userNaRade = 0;
    koniecHodnota = 0;
    int poleFigurok[8] = {0};
    int polePomocne[4] = {0};
    int onlineUsers[3] = {0};
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

    DATA poleData[5];
    pthread_t threads[5];
    int userIDs[5];

    for (int i = 0; i < 5; i++) {
        userIDs[i] = 0;
    }

    while (pocetUsers < 4){
        int socketKlient = accept(sockfd, (struct sockaddr *) &klientAdresa, &cli_len);
        if (socketKlient < 0) {
            perror("Chyba pri accepte" );
            return 3;
        }
        pocetUsers++;
        onlineUSers[pocetUsers - 1] = 1;
        bzero(buffer,256);

        DATA pomData;


        pomData.socketKlient = socketKlient;
        pomData.poleFigurok = poleFigurok;
        pomData.pomocnePole = polePomocne;
        pomData.onlineUsers = onlineUSers;
        pomData.ID = pocetUsers;
        pomData.n = n;
        pomData.userNaRade = &userNaRade;
        poleData->koniecHodnota = koniecHodnota;
        pomData.zahral = &zahral;
        poleData[pocetUsers] = pomData;
        userIDs[pocetUsers] = socketKlient;

       pthread_create(&threads[pocetUsers], NULL, &komunikacia, &poleData[pocetUsers]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    close(sockfd);
    return 0;
}