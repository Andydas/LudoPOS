#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "funkcie.h"



int main(int argc, char *argv[])
{
    int sock;
    int n;
    struct sockaddr_in serverAdresa;
    struct hostent* server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr,"Malo argumentov!\n", argv[0]);
        return 1;
    }
    printf("Dostatocny pocet argumentov!\n");

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Zvoleny host neexistuje!\n");
        return 2;
    }
    printf("Pripojene na zvoleny host!\n");

    bzero((char*)&serverAdresa, sizeof(serverAdresa));
    serverAdresa.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serverAdresa.sin_addr.s_addr,
            server->h_length
    );
    serverAdresa.sin_port = htons(atoi(argv[2]));

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Chyba pri vytvarani socketu!");
        return 3;
    }
    printf("Socket uspesne vytvoreny!\n");

    if(connect(sock, (struct sockaddr*)&serverAdresa, sizeof(serverAdresa)) < 0)
    {
        perror("Chyba pri pripajani sa do socketu!");
        return 4;
    }
    printf("Socket uspesne pripojeny!\n");



    DATA pomData;
    pomData.ID = -111;
    pomData.somNaRade = -1;
    pomData.buffer = buffer;
    pomData.n = n;
    pomData.sock = sock;

    hodKockou((void*)&pomData);

    //pthread_t thread;
    //pthread_create(&thread, NULL, &hodKockou, (void*) &pomData);

    //pthread_join(thread, NULL);

    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER ;



    close(sock);
    pthread_mutex_destroy(&mut);
    return 0;
}