#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
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

    listen(socketServer, 5);
    cli_len = sizeof(klientAdresa);

    socketKlient = accept(socketServer, (struct sockaddr*)&klientAdresa, &cli_len);
    if (socketKlient < 0)
    {
        perror("Chyba pri accepte");
        return 3;
    }

    int maxSpravy = 100;

    for (int i = 0; i < maxSpravy; i++) {
        bzero(buffer,256);
        n = read(socketKlient, buffer, 255);
        if (n < 0)
        {
            perror("Nepodarilo sa nacitat zo socketu");
            return 4;
        }
        printf("Sprava od klienta: %s\n", buffer);

        const char* msg = "Dostal som tvoju spravu Andydas :)";
        n = write(socketKlient, msg, strlen(msg) + 1);
        if (n < 0)
        {
            perror("Nepodarilo sa zapisat");
            return 5;
        }
    }

    close(socketKlient);
    close(socketServer);

    return 0;
}
