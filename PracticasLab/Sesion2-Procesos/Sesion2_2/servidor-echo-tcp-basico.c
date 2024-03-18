#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MSG_LEN 100

int main(int argc, char* argv[]) {

    int sock_pasivo, sock_datos;
    struct sockaddr_in d_local;
    char mensaje[MSG_LEN];
    int leidos;

    

    int port = atoi(argv[1]);
    if(port < 1024 || port > 65536)
    {
        perror("El puerto está fuera de rango válido\n");
        exit(EXIT_FAILURE);
    }

    sock_pasivo = socket(PF_INET, SOCK_STREAM, 0);      // Comprobar valor devuelto por socket
    d_local.sin_family = AF_INET;
    d_local.sin_addr.s_addr = htonl(INADDR_ANY);
    d_local.sin_port = htons(port);

    (void)bind(sock_pasivo, (struct sockaddr *)&d_local, sizeof(d_local)); // Comprobar valor devuelto por bind
    listen(sock_pasivo, SOMAXCONN);     // Comprobar valor devuelto por listen

    while (1) { // Bucle infinito de atención a clientes
        sock_datos = accept(sock_pasivo, 0, 0);
        while((leidos = read(sock_datos, mensaje, sizeof(mensaje))) > 0)
        {
            send(sock_datos, mensaje, leidos, 0);
        }
        send(sock_datos, mensaje, strlen(mensaje), 0);
        close(sock_datos);
    }
}