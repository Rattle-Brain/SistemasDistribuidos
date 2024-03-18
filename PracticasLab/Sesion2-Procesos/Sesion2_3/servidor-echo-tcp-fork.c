#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#define MSG_LEN 100

// Rutina de manejo de la interrucion.
void atrapa_SIGCHLD (int numsig)
{
    wait(0);    // limpiamos asi la entrada del subproceso hijo en la tabla de procesos
    return;
}

int main(int argc, char* argv[]) {

    int sock_pasivo, sock_datos, i;
    struct sockaddr_in d_local;
    pid_t pid;
    char mensaje[MSG_LEN];
    int leidos;

    if(argc < 2)
    {
        perror("Es necesario incluir al menos un puerto paara la conexión\n");
        perror("\tservidor-echo-tcp-basico [port]\n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    if(port < 1024 || port > 65536)
    {
        perror("El puerto está fuera de rango válido\n");
        exit(EXIT_FAILURE);
    }

    // Establecemos lo que vamos a hacer ante la llegada de la señal
    signal(SIGCHLD, atrapa_SIGCHLD);

    sock_pasivo = socket(PF_INET, SOCK_STREAM, 0);      // Comprobar valor devuelto por socket
    d_local.sin_family = AF_INET;
    d_local.sin_addr.s_addr = htonl(INADDR_ANY);
    d_local.sin_port = htons(port);

    (void)bind(sock_pasivo, (struct sockaddr *)&d_local, sizeof(d_local)); // Comprobar valor devuelto por bind
    listen(sock_pasivo, SOMAXCONN);     // Comprobar valor devuelto por listen

    while (1) { // Bucle infinito de atención a clientes
        printf("Soy el proceso padre (%d) antes del fork()\n", getpid());
        sock_datos = accept(sock_pasivo, 0, 0);
        if((pid = fork()) < 0) //fork falla -> tiene que atenderlo el padre pq el cli ya esta accepted
        {
            perror("Fallo en el fork, el cliente lo atiende el padre\n");
            while((leidos = read(sock_datos, mensaje, sizeof(mensaje))) > 0)
            {
                for(i = 0; i < leidos; i++)
                {
                    mensaje[i] =(char) toupper((int) mensaje[i]);
                }
                send(sock_datos, mensaje, leidos, 0);
            }
        }
        else if(pid == 0)// Retorno del fork en el padre.
        {
            printf("Soy el proceso hijo (%d) antes del bucle de lectura\n", getpid());
            while((leidos = read(sock_datos, mensaje, sizeof(mensaje))) > 0)
            {
                for(i = 0; i < leidos; i++)
                {
                    mensaje[i] =(char) toupper((int) mensaje[i]);
                }
                send(sock_datos, mensaje, leidos, 0);
            }
            printf("Soy el proceso hijo (%d) despues del bucle de lectura\n", getpid());
        }
        else
        {
            printf("Soy el proceso padre(%d) antes de cerrar el socket de datos\n", getpid());
            close(sock_datos);
        }
    }
}