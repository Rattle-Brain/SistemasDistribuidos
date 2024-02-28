#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    int sock_dat, recibidos;
    char buffer[50];
    char * date;
    unsigned long int numero;
    unsigned long int seventy_years = 2208988800;

    struct sockaddr_in d_serv;
    socklen_t ldir = sizeof(d_serv);

    if (argc < 2)
    {
        printf("Forma de uso: %s IP\n", argv[0]);
    }

    sock_dat = socket(PF_INET, SOCK_DGRAM, 0);
    d_serv.sin_family = AF_INET;
    d_serv.sin_addr.s_addr = inet_addr(argv[1]);
    d_serv.sin_port = htons(37);

    // Enviar datagrama vacio
    sendto(sock_dat, buffer, 0, 0, (struct sockaddr *) &d_serv, ldir);

    // recibir contenido del servidor time
    recibidos = recvfrom(sock_dat, &numero, 4, 0,
        (struct sockaddr *) &d_serv, &ldir);

    numero = ntohl(numero);
    numero = numero - seventy_years;
    printf("Valor entero: %lu\n", numero);
    date = ctime((time_t *) &numero);
    printf("Fecha actual: %s", date);
}