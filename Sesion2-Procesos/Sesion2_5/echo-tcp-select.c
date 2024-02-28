// Fichero: echo-tcp-udp-select.c
#include <stdio.h>       // printf()
#include <stdlib.h>      // exit()
#include <sys/socket.h>  // socket(), bind(), listen(), recv(), send(), etc
#include <arpa/inet.h>   // sockaddr_in
#include <errno.h>       // perror()
#include <sys/select.h>  // select() y fd_set
#include <unistd.h>      // close()

#define MAXCLIENTES 10
#define CIERTO 1
#define FALSO 0
#define NO_HAY_POS_LIBRE -1


int clientes[MAXCLIENTES];

void InicializarArrayClientes(int *lista, int num)
{
    int i;
    for (i = 0; i < num; i++) lista[i]=0; //*(lista+i)=0
}

int HayPosicionesLibres(int *lista, int num)
{
    int resultado = FALSO;
    int i;

    for(i = 0; i < num; i++)
    {
        if(lista[i]==0)
        {
            resultado = CIERTO;
            break;
        }
    }
    return resultado;
}

int BuscarPosLibre(int *lista, int num)
{
    int resultado = NO_HAY_POS_LIBRE;
    int i;

    for(i = 0; i < num; i++)
    {
        if(lista[i] == 0)
        {
            resultado = i;
            break;
        }
    }
    return resultado;
}

int CrearSocketTCP(int puerto)
{
    int s;
    struct sockaddr_in dir;
    int r;

    s = socket(PF_INET, SOCK_STREAM, 0);
    if (s==-1) {
        perror("En socket TCP");
        exit(1);
    }
    dir.sin_family = AF_INET;
    dir.sin_port   = htons(puerto);
    dir.sin_addr.s_addr = htonl(INADDR_ANY);
    r = bind(s, (struct sockaddr *) &dir, sizeof(dir));
    if (r==-1) {
        perror("En bind TCP");
        exit(1);
    }
    r = listen(s, SOMAXCONN);
    if (r==-1) {
        perror("En listen");
        exit(1);
    }
    return s;
}

int dar_servicio_TCP(int s)
{
    // Lee datos del socket s y si lee distinto de cero, envia eco
    // Retorna el numero de datos leidos

   char buffer[100];
   int leidos;

   leidos = recv(s, buffer, 100, 0);
   if (leidos>0)
       send(s, buffer, leidos, 0);
   return leidos;
}

int buscar_maximo(int tcp, int *lista, int num)
{
    int resultado = tcp;
    int i;

    for(i=0; i<num; i++)
    {
        if( lista[i] > resultado)
        {
            resultado = lista[i];
        }
    }
}

int main(int argc, char * argv[])
{
    int puerto;
    int s_tcp; // sockets "de escucha"
    int s_datos;      // Para la conexion TCP
    fd_set conjunto;  // Para select
    int    maximo;    // Para select
    int pos, i;

    if (argc<2) {
        printf("Uso: %s puerto\n", argv[0]);
        exit(0);
    }

    puerto = atoi(argv[1]);
    s_tcp = CrearSocketTCP(puerto);
    s_datos = 0;  // De momento no tenemos cliente

    InicializarArrayClientes(clientes, MAXCLIENTES);

    while (1) {  // Bucle infinito del servidor

        // Vaciar conjunto de descriptores a vigilar
        FD_ZERO(&conjunto);

        if (HayPosicionesLibres(clientes, MAXCLIENTES))
        {
            FD_SET(s_tcp, &conjunto);
        }

        for (i = 0; i < MAXCLIENTES; i++)
        {
            if (clientes[i] != 0)
                FD_SET(clientes[i], &conjunto);
            
        }

        // Si hay cliente meto el de datos, si no meto el de escucha
        if (s_datos!=0)
        {
            FD_SET(s_datos,&conjunto);
        }
        else 
        {
            FD_SET(s_tcp,&conjunto);
        }
        maximo = buscar_maximo(s_tcp, clientes, MAXCLIENTES);

        // Esperar a que ocurra "algo"
        select(maximo + 1, &conjunto, NULL, NULL, NULL);
        printf("Ha ocurrido algo\n");

        // Averiguar que ocurrió
        if (HayPosicionesLibres(clientes, MAXCLIENTES) && FD_ISSET(s_tcp,&conjunto) )// ---A rellenar ------------------------
        {
            printf("Ha llegado un cliente al socket TCP\n");
            s_datos = accept(s_tcp, NULL, NULL);
            pos = BuscarPosLibre(clientes, MAXCLIENTES);
            if (pos != NO_HAY_POS_LIBRE)
                clientes[pos] = s_datos;
        }

        for (i = 0; i < MAXCLIENTES; i++)
        {
            if (clientes[i] != 0 && FD_ISSET(clientes[i],&conjunto) )// ---A rellenar ------------------------
            {
                int n;
                printf("Han llegado datos por la conexión TCP\n");
                n = dar_servicio_TCP(clientes[i]);
                if (n==0) {
                    printf("El cliente TCP ha desconectado\n");
                    // ---A rellenar ------------------------
                    close(clientes[i]);
                    clientes[i] = 0;
                }
            }
        }
    } // del while(1)
    return 0;   // Nunca se ejecuta, pero para que el compilador no proteste
} // de main()