#include "funkcie.h"
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

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
            return 5;
        }
        printf("Uspesne zapisane do socketu!\n");

        if (data->buffer[0] == '0')
            koniec = true;

        bzero(data->buffer,256);
        data->n = read(data->sock, data->buffer, 255);
        if (data->n < 0)
        {
            perror("Chyba pri citani zo socketu");
            return 6;
        }
        printf("Uspesne citanie do socketu!\n");
        printf("%s\n",data->buffer);
    }

}