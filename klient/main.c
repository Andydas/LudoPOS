#include "funkcie.h"

int main(int argc, char *argv[])
{    bool vstupOK = false;
    char vstupChar;
    int vstup;
    while (!vstupOK) {
        printf("VITAJ V APLIKACII LUDO\n");
        printf("1 - Pripoj na server\n");
        printf("2 - Zobraz pravidla\n");
        printf("0 - Vypni aplikaciu\n");
        printf("Co chces urobit?: ");
        do
            vstupChar = getchar();
        while (isspace(vstupChar));
        vstup = vstupChar - ASCII_NULA;
        if (vstup >= 0 && vstup <= 2){
            if (vstup == PRAVIDLA) {
                zobrazPravidla();
                vstupOK = false;
            } else if (vstup == SKONCI) {
                return 0;
            } else {
                DATA pomData;

                srand(time(NULL));
                int sock;
                int n;
                struct sockaddr_in serverAdresa;
                struct hostent* server;

                //kontrolujem pocet argumentov
                if (argc < 3)
                {
                    fprintf(stderr,"Malo argumentov!\n", argv[0]);
                    return 1;
                }

                //ziskam meno servera
                server = gethostbyname(argv[1]);
                if (server == NULL)
                {
                    fprintf(stderr, "Zvoleny host neexistuje!\n");
                    return 2;
                }

                //urcenie adresy
                bzero((char*)&serverAdresa, sizeof(serverAdresa));
                serverAdresa.sin_family = AF_INET;
                bcopy(
                        (char*)server->h_addr,
                        (char*)&serverAdresa.sin_addr.s_addr,
                        server->h_length
                );
                serverAdresa.sin_port = htons(atoi(argv[2]));

                //vytvorim tcp socket
                sock = socket(AF_INET, SOCK_STREAM, 0);
                if (sock < 0)
                {
                    perror("Chyba pri vytvarani socketu!");
                    return 3;
                }

                //pripojim sa na server
                if(connect(sock, (struct sockaddr*)&serverAdresa, sizeof(serverAdresa)) < 0)
                {
                    perror("Chyba pri pripajani sa do socketu!");
                    return 4;
                }
                printf("Uspesne pripojene na server, pockaj kym sa napoji potrebny pocet hracov!\n");


                //vytvorim datovu strukturu
                pomData.koniecHry = false;
                pomData.ID = -1;
                pomData.n = n;
                pomData.sock = sock;
                pomData.vyherca = 0;
                for (int i = 0; i < VELKOST_HRACIEHO_POLA; i++) {
                    pomData.hraciePole[i] = PRAZDNE_POLICKO;
                }

                //zacnem komunikaciu
                komunikacia(&pomData);

                //zatvorenie socketu
                close(sock);

                return 0;
            }

        }
        vstupOK = false;
        printf("Neplatny vstup, opakuj volbu\n");
    }

}


