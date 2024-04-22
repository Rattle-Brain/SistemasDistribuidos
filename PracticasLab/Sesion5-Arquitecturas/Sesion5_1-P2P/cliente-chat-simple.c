// Este cliente implementa la comunicacion entre peers por el protocolo UDP
// Usa select para multiplexar su atención entre el teclado y el socket p2p

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAX_TAM_LINEA   500
#define MAX_TAM_MENSAJE 500
#define MAX_TAM_NICK    10

char *nick;                 // Para enviarlo en cada mensaje

// Estas funciones están implementadas después de main()
void leer_y_procesar_teclado(int socketUDP);
void recibir_y_mostrar_mensaje(int socketUDP);

int main(int argc, char *argv[])
{
    int socketUDP; int puerto;  // Para el socket entre peers
    struct sockaddr_in dirUDP;
    int teclado = 0;        // Descriptor de la entrada estandar

    // Para el select
    fd_set escucha;
    int max;
    int resultado;

    // Las siguientes variables contendrán la IP y puerto del peer
    // al que enviar los mensajes
    char *ip_peer = NULL;
    int  puerto_peer = 0;

    /***** Comprobación de argumentos *****/
    if (argc != 3)
    {
        printf("Uso: %s puerto nick\n", argv[0]);
        exit(0);
    }
    nick = argv[2];
    // Truncar nick si es necesario
    if (strlen(nick) >= MAX_TAM_NICK - 1)
        nick[MAX_TAM_NICK] = 0;
    puerto = atoi(argv[1]);

    /***** Inicialización del socket ****/
    // Creamos el socket UDP en el puerto que se le pasa por linea de comandos
    socketUDP = socket(PF_INET, SOCK_DGRAM, 0);
    if (socketUDP == -1)
    {
        perror("Al crear el socket UDP");
        exit(1);
    }
    dirUDP.sin_family = AF_INET;
    dirUDP.sin_addr.s_addr = htonl(INADDR_ANY);
    dirUDP.sin_port = htons(puerto);
    resultado = bind(socketUDP, (struct sockaddr *) &dirUDP, sizeof(struct sockaddr_in));
    if (resultado == -1)
    {
        perror("Al hacer bind");
        exit(1);
    }

    printf("Usa el comando /CHAT <ip> <puerto> para indicar el destino de tus mensajes\n");
    printf("El texto que escribas después se enviará a ese contacto\n\n");
    printf("En cualquier momento puedes poner /CHAT <ip> <puerto> de nuevo\n");
    printf("para cambiar a un nuevo contacto.\n\n");

    // Y entramos en el bucle de espera
    printf("%s> ", nick);
    while (1)
    {
        // Imprimir un prompt para invitar a escribir
        fflush(stdout);        // Para que lo saque ya, pues no finaliza con \n

        // Esperar en select a que el usuario escriba algo, o a que algo llegue
        // por el socket UDP

        /***************************************************/
        /* A RELLENAR                                      */
        /* Llamada a select, previa inicialización         */
        /* de las variables necesarias                     */
        /***************************************************/

        // Al salir es que algo ha ocurrido
        if (FD_ISSET(socketUDP, &escucha))
        {
            recibir_y_mostrar_mensaje(socketUDP);
        }

        if (FD_ISSET(teclado, &escucha))
        {
            leer_y_procesar_teclado(socketUDP);
        }
    }
}

void recibir_y_mostrar_mensaje(int socketUDP)
{
    char buff[MAX_TAM_MENSAJE];
    int recibidos;

    recibidos = recvfrom(socketUDP, buff, MAX_TAM_MENSAJE, 0, NULL, NULL);
    if (recibidos == -1) return; // Ignoramos silenciosamente errores
    buff[recibidos] = 0;
    printf("\n**|%s|\n", buff);
}

void leer_y_procesar_teclado(int socketUDP)
{
    char linea[MAX_TAM_LINEA];
    char cmd[MAX_TAM_LINEA];
    static char ip_destino[25] = "No asignada";
    static int  puerto_destino = 0;
    static struct sockaddr_in dir_destino;
    char *mensaje_a_enviar;
    int i;

    // Leer la línea
    fgets(linea, MAX_TAM_LINEA - MAX_TAM_NICK, stdin);
    // Eliminarle el retorno de carro o espacios al final
    for (i = strlen(linea) - 1; i >= 0; i--)
    {
        if (isspace(linea[i]))
            linea[i] = 0;
        else
            break;
    }
    // Sacar un nuevo prompt
    printf("%s> ", nick);

    // Ver si es un comando
    if (linea[0] == '/')
    {
        // Comienza por /, en ese caso hay que procesarlo. 
        // Lo más fácil es leer sus contenidos con sscanf
        sscanf(linea, "%s", cmd);

        /*******************************************************/
        /* A RELLENAR                                          */
        /* En función del valor encontrado en cmd, realizamos  */
        /* la acción apropiada                                 */
        /*******************************************************/
    }
    else      // Si la linea no comienza por /, es un mensaje a enviar al peer
    {
        // Hay que enviarlo a la ip y puertos antes asignados, 
        // pero antes comprobamos si efectivamente se asignaron
        // antes estos valores
        if (strcmp(ip_destino, "No asignada") == 0)
        {
            printf("ERROR: antes de enviar mensaje debes usar el comando\n");
            printf("/CHAT <ip> <puerto>\n");
            return;
        }
        // Si todo va bien, enviamos el mensaje, 
        // precedido por el nick del usuario

        /*******************************************************/
        /* A RELLENAR                                          */
        /* Crear un buffer con el mensaje a enviar y enviarlo  */
        /*******************************************************/
    }
}
