#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MSG_LEN 128

int main(int argc, char** argv)
{
    int sock, port, i;
    struct sockaddr_in d_local, d_cliente;
    socklen_t ldir = sizeof(d_cliente);
    char mensaje[MSG_LEN];
    int leidos;

    if(argc < 2)
    {
        perror("Es necesario incluir al menos un puerto paara la conexión\n");
        perror("\tservidor-echo-udp-basico [port]\n");
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);

    if(port < 1024 || port > 65536)
    {
        perror("Puerto fuera de rango. [1024, 65536]\n");
        exit(EXIT_FAILURE);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("Error al crear el socket.\n");
        exit(EXIT_FAILURE);
    }

    d_local.sin_family = AF_INET;
    d_local.sin_addr.s_addr = htonl(INADDR_ANY);
    d_local.sin_port = htons(port);
    printf("Listening in port: %d\n", port);

    if(bind(sock, (struct sockaddr*) &d_local, sizeof(d_local)) < 0)
    {
        perror("Error en el bind\n");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        printf("Bucle de lectura:\n");
        while((leidos = recvfrom(sock, mensaje, sizeof(mensaje), 0, (struct sockaddr*) &d_cliente, &ldir)) > 0)
        {
            for(i = 0; i < leidos; i++)
            {
                mensaje[i] =(char) toupper((int) mensaje[i]);
            }
            sendto(sock, mensaje, strlen(mensaje), 0, (struct sockaddr*) &d_cliente, ldir); 
        }
    }

    exit(EXIT_SUCCESS);
}