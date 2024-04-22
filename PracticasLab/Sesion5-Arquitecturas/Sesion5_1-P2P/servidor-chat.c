#include "protocolo.h"
#include <string.h>


// Estructura que almacena la pareja (nick, endpoint), que relaciona un nick
// con su correspondiente IP y puerto
struct Cliente
{
    char *nick;
    struct sockaddr_in *endpoint;
};
typedef struct Cliente Cliente;


// La implementacion de la lista de clientes es muy ineficiente. Lo correcto
// sería usar una tabla hash o un diccionario, pero el C estandar no incluye
// este tipo de datos, y se sale de los objetivos de la asignatura implementar
// este tipo y todas sus operaciones, de modo que se opta por una solucion
// mucho mas simple aunque inadecuada: un array de tamaño fijo que contiene
// punteros a las estructuras cliente, y busquedas ineficientes recorriendo
// el array completo.

#define MAX_CLIENTS 200
Cliente *clientes[MAX_CLIENTS];


void init_clientes()
// Inicializa con NULL el array de clientes
{
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) clientes[i] = NULL;
}

int inserta_cliente(Cliente *c)
// Busca un lugar vacío en el array y mete allí un nuevo puntero a cliente
{
    int i;
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        if (clientes[i] == NULL)
        {
            clientes[i] = c;
            return 0;
        }
    }
    return -1;
}

int borra_cliente(int i)
// Accede al elemento [i] del array, obtiene el puntero allí almacenado, libera
// las estructuras de memoria señaladas por ese puntero, y pone a NULL ese
// elemento del array para marcarlo como libre
{
    Cliente *c;
    if (clientes[i] == NULL) return -1;
    c = clientes[i];
    clientes[i] = NULL;
    free(c->nick);
    free(c->endpoint);
    free(c);
    return 0;
}

int busca_cliente(char *nick)
// Recorre el array y compara el nick de las entradas no nulas con el nick dado
// Devuelve el índice de la primera coincidencia, o -1 si no hay ninguna
{
    int i;
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        if ((clientes[i] != NULL) && (strcmp(clientes[i]->nick, nick) == 0))
            return i;
    }
    return -1;
}


// -------------------------------------------------------------------
// Estas funciones son las usadas por el protocolo cuando un cliente
// se registra ante el servidor, o se desregistra o pregunta por otro
// cliente

int registra(char *nick, struct sockaddr_in *endpoint)
// Un cliente nuevo hace JOIN, sus datos se almacenan en la lista
// Retorna 0 si todo va bien, -1 si hay un error, lo que casi
// siempre significa que el nick ya estaba registrado. También
// puede devolver -1 si no hay memoria para el nuevo cliente o si
// todas las posiciones del array están ocupadas, pero estos casos
// no ocurrirán normalmente durante las sesiones prácticas.
{
    Cliente *c;

    // Si el nick ya está registrado, retornar error
    if (busca_cliente(nick) != -1) return -1;

    // Hacer memoria para el nuevo cliente y su información
    // retornando error si hay algún problema.
    c = malloc(sizeof(Cliente));
    if (c == NULL) return -1;
    c->nick = malloc(strlen(nick));
    if (c->nick == NULL)
    {
        free(c);
        return -1;
    }
    c->endpoint = malloc(sizeof(struct sockaddr_in));
    if (c->endpoint == NULL)
    {
        free(c->nick);
        free(c);
        return -1;
    }

    // Copiar los parametros recibidos en el nuevo cliente
    strcpy(c->nick, nick);
    memcpy(c->endpoint, endpoint, sizeof(struct sockaddr_in));

    // DEBUG: mostrar lo que vamos a guardar
    printf("Insertando al cliente p2p  %s@%s:%d\n",
           c->nick,
           inet_ntoa(c->endpoint->sin_addr),
           ntohs(c->endpoint->sin_port));

    // E insertar el cliente en la lista
    if (inserta_cliente(c) == -1)
    {
        free(c->nick); free(c->endpoint); free(c);
        return -1;
    }
    // Todo OK
    return 0;
}

int desregistra(char *nick)
// Un cliente hace LEAVE. Sus datos se borran de la lista
// Devuelve -1 si no se encuentra el nick, 0 en caso contrario
{
    int c = busca_cliente(nick);
    if (c == -1)
        return -1;
    return borra_cliente(c);
}


int busca(char *nick, struct sockaddr_in *resultado)
// Un cliente hace un query para buscar a otro (por su nick)
// Si se encuentra, la estructura resultado se rellena con la IP y puerto
// del cliente encontrado, y la función retorna 0
// Si no se encuentra, la función retorna -1 y el contanido de la
// la estructura resultado no se modifica
{
    Cliente *c;
    int i = busca_cliente(nick);
    if (i == -1)    // No ha sido encontrado
        return -1;

    // Si ha sido encontrado, copiamos la información a la dirección
    // indicada por la variable resultado
    c = clientes[i];
    memcpy(resultado, c->endpoint, sizeof(struct sockaddr_in));
    return 0;
}


// La función que lee de la red el comando enviado por el cliente y lo ejecuta
void ejecutar_comando(int sock)
{
    char tipo;
    short int nick_len;
    char *nick;
    struct sockaddr_in cliente;
    socklen_t tamanio = sizeof(struct sockaddr_in);
    int puerto;
    char respuesta;
    int result;

    // Leer el tipo de comando
    tipo = readByte(sock);

    // Leer el nick de quien lo envía
    nick_len = readShort(sock);
    nick = malloc(nick_len + 1);
    readString(sock, nick, nick_len);
    nick[nick_len] = 0;

    // Según el tipo leído, la acción prosigue de forma diferente
    switch (tipo)
    {
    case JOIN_CMD:
        // Obtener la IP desde la que viene el cliente
        getpeername(sock, (struct sockaddr *) &cliente, &tamanio);
        // DEBUG: imprimimos esa informacion
        printf("JOIN recibido desde %s:%d\n",
               inet_ntoa(cliente.sin_addr),
               ntohs(cliente.sin_port));

        // Obtener el puerto de su socket p2p, el cual debe transmitirnoslo
        // ahora como parte del protocolo, en forma de short
        puerto = readShort(sock);

        // Lo guardamos en la estructura de cliente anterior, sobreescribiendo
        // el puerto que venía allí, que no era el del socket p2p, sino el
        // del socket que usó para conectar con el servidor. La IP en cambio
        // ha de ser la misma, así que la dejamos
        cliente.sin_port = ntohs(puerto);

        // Probar a registrar el nick recibido
        result = registra(nick, &cliente);
        if (result == -1)
        {
            // El nick ya estaba registrado, u otros problemas
            respuesta = JOIN_ERR;
        }
        else
        {
            respuesta = CMD_OK; // OK
        }
        sendByte(sock, respuesta);
        break;
    case LEAVE_CMD:
        // Intentar desregistrar el nick recibido
        result = desregistra(nick);
        if (result == -1)
        {
            // El nick no estaba registrado
            respuesta = LEAVE_ERR;
        }
        else
        {
            respuesta = CMD_OK; // OK
        }
        sendByte(sock, respuesta);
        break;
    case QUERY_CMD:
        // Imprimimos mensaje de depuración
        printf("Solicitados datos del nick |%s|\n", nick);
        // Buscar el nick
        result = busca(nick, &cliente);
        if (result == -1)
        {
            // El nick no estaba registrado
            respuesta = QUERY_ERR;
            sendByte(sock, respuesta);
        }
        else
        {
            respuesta = CMD_OK;
            sendByte(sock, respuesta);
            sendEndpoint(sock, &cliente);
        }
        break;
    default:
        // Error de sintaxis
        respuesta = CMD_ERR;
        sendByte(sock, respuesta);
    }
    // Liberar la memoria ocupada por la cadena nick que ya no es necesaria
    // (al registrar el nick, la función encargada de ello ha sacado una copia
    // de esta cadena)
    free(nick);
}

// Programa principal
int main(int argc, char *argv[])
{
    int puerto = 0;
    int sockEscucha, sockDatos;

    // Comprobar el correcto uso de la línea de comandos
    if (argc < 2)
    {
        printf("Uso: %s puerto\n", argv[0]);
        exit(0);
    }
    puerto = atoi(argv[1]);

    sockEscucha = CrearSocketServidorTCP(puerto);

    // Bucle de atención a clientes
    while (1)
    {
        //Esperamos la llegada de nuevas conexiones
        printf("Esperando clientes...\n");
        sockDatos = AceptarConexion(sockEscucha);
        printf("Cliente nuevo\n");
        ejecutar_comando(sockDatos);
        close(sockDatos);
    }
}
