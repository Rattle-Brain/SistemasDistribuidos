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

    bind(sock_pasivo, (struct sockaddr *)&d_local, sizeof(d_local)); // Comprobar valor devuelto por bind
    listen(sock_pasivo, SOMAXCONN);     // Comprobar valor devuelto por listen

    // Creamos el pool de procesos con forks
    for (int i = 0; i < 3; i++)
    {
        if((pid = fork()) <0)       // Fallo del fork()
        {
            perror("Fallo del fork al general un proceso para el pool\n");
        } 
        else if(pid == 0)
        {
            // Si estamos en el retorno del fork en un hijo impedimos
            // que este siga ejecutando el mismo codigo que el padre
            // haciendo que salga del bucle con un break.
            break;
        }
    }

    while (1) { // Bucle infinito de atención a clientes
        printf("Soy el proceso (%d) antes del accept()\n", getpid());
        sock_datos = accept(sock_pasivo, 0, 0);
        printf("Soy el proceso (%d) antes del bucle de lectura\n", getpid());
        while((leidos = read(sock_datos, mensaje, sizeof(mensaje))) > 0)
        {
            for(i = 0; i < leidos; i++)
            {
                mensaje[i] =(char) toupper((int) mensaje[i]);
            }
            send(sock_datos, mensaje, leidos, 0);
        }
        printf("Soy el proceso (%d) despues del bucle de lectura\n", getpid());
        close(sock_datos);
        printf("Sou el proceso (%d) despues de cerrar el socket de datos\n", getpid());
    }
}