#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    int socketServer;
    int n;
    struct sockaddr_in serverAdresa;
    struct hostent* server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr,"Malo argumentov!\n", argv[0]);
        return 1;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Zvoleny host neexistuje!\n");
        return 2;
    }

    bzero((char*)&serverAdresa, sizeof(serverAdresa));
    serverAdresa.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serverAdresa.sin_addr.s_addr,
            server->h_length
    ); //janko je fesak
    serverAdresa.sin_port = htons(atoi(argv[2]));

    socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServer < 0)
    {
        perror("Chyba pri vytvarani socketu!");
        return 3;
    }

    if(connect(socketServer, (struct sockaddr*)&serverAdresa, sizeof(serverAdresa)) < 0)
    {
        perror("Chyba pri pripajani sa do socketu!");
        return 4;
    }

    bool koniec = false;

    while (koniec == false) {
        printf("Napis spravu: ");

        bzero(buffer,256);
        fgets(buffer, 255, stdin);

        n = write(socketServer, buffer, strlen(buffer));
        if (n < 0)
        {
            perror("Chyba pri zapisovani do socketu");
            return 5;
        }

        if (buffer[0] == '0')
            koniec = true;

        bzero(buffer,256);
        n = read(socketServer, buffer, 255);
        if (n < 0)
        {
            perror("Chyba pri citani zo socketu");
            return 6;
        }

    }

    printf("%s\n",buffer);
    close(socketServer);

    return 0;
}