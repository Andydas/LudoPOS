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
//#include "funkcie.h"

typedef struct data{
    int sock;
    char * buffer;
    int n;
} DATA;

void* hodKockou(void* param) {
    DATA* data = (DATA*) param;
    bool koniec = false;
    while (!koniec) {
        printf("Napis spravu: ");

        bzero(data->buffer,256);
        fgets(data->buffer, 255, stdin);

        data->n = write(data->sock, data->buffer, strlen(data->buffer));
        if (data->n < 0)
        {
            perror("Chyba pri zapisovani do socketu");
            //return 5;
        }
        printf("Uspesne zapisane do socketu!\n");

        if (data->buffer[0] == '0')
            koniec = true;

        bzero(data->buffer,256);
        data->n = read(data->sock, data->buffer, 255);
        if (data->n < 0)
        {
            perror("Chyba pri citani zo socketu");
            //return 6;
        }
        printf("Uspesne citanie do socketu!\n");
        printf("%s\n",data->buffer);
    }

}

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
    pomData.buffer = buffer;
    pomData.n = n;
    pomData.sock = sock;

    pthread_t thread;
    pthread_create(&thread, NULL, &hodKockou, (void*)&pomData);

    pthread_join(thread, NULL);

    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;



    close(sock);
    pthread_mutex_destroy(&mut);
    return 0;
}