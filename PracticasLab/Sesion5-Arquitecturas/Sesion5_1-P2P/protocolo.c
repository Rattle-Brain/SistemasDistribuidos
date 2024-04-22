#include "protocolo.h"

// Este fichero implementa algunas funciones que simplifican la creación
// de sockets pasivos o activos, y también varias funciones específicas
// para el protocolo entre cliente y servidor chat, que permiten enviar
// y recibir 1 byte de información, un short, una cadena de longitud dada
// o una estructura sockaddr_in que contiene IP y puerto de un contacto


int CrearSocketServidorTCP(int puerto)
{
    // Crea un socket pasivo en el puerto dado y lo retorna
    // Aborta la ejecución del programa en caso de error

    int ret;
    int sock;
    struct sockaddr_in dir;

    ret = socket(PF_INET, SOCK_STREAM, 0);
    if (ret == -1)
    {
        perror("Al crear socket");
        exit(-1);
    }
    sock = ret;
    if (puerto != 0)
    {
        dir.sin_family = AF_INET;
        dir.sin_port = htons(puerto);
        dir.sin_addr.s_addr = htonl(INADDR_ANY);
        ret = bind(sock, (struct sockaddr *)  &dir, sizeof(dir));
        if (ret == -1)
        {
            perror("Al asignar direccion");
            close(sock);
            exit(-1);
        }
    }
    ret = listen(sock, SOMAXCONN);
    if (ret == -1)
    {
        perror("Al poner en modo escucha");
        close(sock);
        exit(-1);
    }
    return sock;
}

int AceptarConexion(int sock)
{
    // Acepta una conexión en el socket pasivo, o aborta la ejecución en caso de error
    int ret;

    ret = accept(sock, NULL, NULL);
    if (ret == -1)
    {
        perror("Al aceptar");
        exit(1);
    }
    return ret;
}

int CrearSocketDatosTCP()
{
    // Crea y retorna un socket de datos
    // Aborta la ejecución en caso de error
    int sock;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Al crear socket");
        exit(-1);
    }
    return sock;
}

int Conectar(int sock, char *ip, int puerto)
{
    // Intenta una conexión a la IP y puerto dados (la IP se espera como una
    // cadena de caracteres, ej: "127.0.0.1")
    // Aborta la ejecución del programa en caso de error

    struct sockaddr_in dir_serv;
    int result;

    dir_serv.sin_family = AF_INET;
    dir_serv.sin_port   = htons(puerto);
    result = inet_aton(ip, &dir_serv.sin_addr);
    if (result == 0)
    {
        perror("Al convertir IP");
        exit(1);
    }

    result = connect(sock, (struct sockaddr *)&dir_serv, sizeof(dir_serv));
    if (result == -1)
    {
        perror("Al conectar");
        exit(1);
    }
    return result;
}

// ===========================================================================
// Transmisión de datos en binario
//
// Las siguientes funciones implementan el envío o recepción a través
// de un socket TCP que reciben como parámetro, de diferentes tipos de datos
// usados por el protocolo.

char readByte(int s)
{
    // Devuelve 1 byte leido del socket, o aborta la ejecución en caso de error
    char byte;
    int leidos;
    leidos = recv(s, &byte, 1, 0);
    if (leidos != 1)
    {
        perror("Al leer byte");
        exit(1);
    }
    return byte;
}

short int readShort(int s)
{
    // Devuelve 1 short leido del socket, o aborta la ejecución en caso de error
    // Espera el short en formato de red y lo convierte a la endianity del host
    short int dato;
    int leidos;
    leidos = recv(s, &dato, 2, 0);
    if (leidos != 2)
    {
        perror("Al leer short");
        exit(1);
    }
    return ntohs(dato);
}

void readString(int s, char *buff, int len)
{
    // Lee una cadena de longitud dada y la deja en la dirección especificada en s
    // Es responsabilidad del programa que llama el asegurar de que en la dirección
    // apuntada hay espacio suficiente para albergar los bytes recibidos
    int leidos;
    leidos = recv(s, buff, len, 0);
    if (leidos != len)
    {
        perror("Al leer string");
        exit(1);
    }
}

void readEndpoint(int s, struct sockaddr_in *endpoint)
{
    // Lee una IP y puerto y los deja en la estructura sockaddr_in que recibe
    // como parámetro. La IP y puerto se esperan en el formato de red usado
    // por el protocolo TCP IP
    int leidos;
    leidos = recv(s, &(endpoint->sin_addr.s_addr), 4, 0);
    if (leidos != 4)
    {
        perror("Al leer ip");
        exit(1);
    }
    leidos = recv(s, &(endpoint->sin_port), 2, 0);
    if (leidos != 2)
    {
        perror("Al leer puerto");
        exit(1);
    }
}

void sendByte(int s, char byte)
{
    int enviados;
    enviados = send(s, &byte, 1, 0);
    if (enviados != 1)
    {
        perror("Al enviar byte");
        exit(1);
    }
}

void sendShort(int s, short int dato)
{
    int enviados;
    short int dato_ok = htons(dato);
    enviados = send(s, &dato_ok, 2, 0);
    if (enviados != 2)
    {
        perror("Al enviar short");
        exit(1);
    }
}

void sendString(int s, char *str)
{
    int enviados;
    enviados = send(s, str, strlen(str), 0);
    if (enviados != strlen(str))
    {
        perror("Al enviar cadena");
        exit(1);
    }
}

void sendEndpoint(int s, struct sockaddr_in *endpoint)
{
    int enviados;
    enviados = send(s, &(endpoint->sin_addr), 4, 0);
    if (enviados != 4)
    {
        perror("Al enviar ip");
        exit(1);
    }
    enviados = send(s, &(endpoint->sin_port), 2, 0);
    if (enviados != 2)
    {
        perror("Al enviar puerto");
        exit(1);
    }
}
