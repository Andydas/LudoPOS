#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct data {
    int socketKlient;
    char * buffer;
    int n;
} DATA;

void* citaj(void * param) {

    DATA* data = (DATA*) param;
    char buff[1];
    sprintf(buff, "%d", data->socketKlient )   ;
    // const char *buff = socketKlient      ;
    data->n = write(data->socketKlient, buff, strlen(buff) + 1)      ;

    printf("Csilo socket: %d\n", data->socketKlient)      ;
    if (data->socketKlient < 0) {
        perror("Chyba pri accepte" )      ;
        //return 3       ;
    }

    int maxSpravy = 2;


    bool koniec = false;
    while (!koniec) {
        bzero(data->buffer, 256);
        data->n = read(data->socketKlient, data->buffer, 255);
        if (data->n < 0) {
            perror("Nepodarilo sa nacitat zo socketu")           ;
            //return 4;
        }
        printf("Sprava od klienta: %s\n", data->buffer);

        if (data->buffer[0] == '0')
            koniec = true;

        const char *msg = "Dostal som tvoju spravu Andydas :)"  ;
        data->n = write(data->socketKlient, msg, strlen(msg) + 1);
        if (data->n < 0) {
            perror("Nepodarilo sa zapisat")      ;
            //return 5;
        }
    }
}

int main(int argc, char * argv[])
{
    int pocetUsers = 1;
    int socketServer;
    int socketKlient;
    socklen_t cli_len;
    struct sockaddr_in serverAdresa;
    struct sockaddr_in klientAdresa;
    int n;
    char buffer[256];

    if (argc < 2)
    {
        fprintf(stderr,"Zly argument pre port %s \n", argv[0]);
        return 1;
    }

    bzero((char*)&serverAdresa, sizeof(serverAdresa));
    serverAdresa.sin_family = AF_INET;
    serverAdresa.sin_addr.s_addr = INADDR_ANY;
    serverAdresa.sin_port = htons(atoi(argv[1]));

    socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServer < 0)
    {
        perror("Chyba pri vytvarani socketu");
        return 1;
    }

    if (bind(socketServer, (struct sockaddr*)&serverAdresa, sizeof(serverAdresa)) < 0)
    {
        perror("Chyba pri bindovani adresy socketu");
        return 2;
    }

    listen(socketServer, 5)   ;
    cli_len = sizeof(klientAdresa)    ;


    pthread_t thread[5];
    while (pocetUsers < 5)    {

        int socketKlient = accept(socketServer, (struct sockaddr *) &klientAdresa, &cli_len)       ;
        DATA pomData;
        pomData.socketKlient = socketKlient;
        pomData.buffer = buffer;
        pomData.n = n;


        pthread_create(&thread[pocetUsers], NULL, &citaj, (void*)&pomData);

        pocetUsers++;



    }

    close(socketKlient);
    close(socketServer);

    return 0;
}