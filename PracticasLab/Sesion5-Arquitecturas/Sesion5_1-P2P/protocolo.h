#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define JOIN_CMD  0x01
#define LEAVE_CMD 0x02
#define QUERY_CMD 0x03
#define JOIN_ERR  0x81
#define LEAVE_ERR 0x82
#define QUERY_ERR 0x83
#define CMD_ERR   0x80
#define CMD_OK    0x00

int CrearSocketServidorTCP(int puerto);
int AceptarConexion(int sock);
int CrearSocketDatosTCP(void);
int Conectar(int sock, char *ip, int puerto);
char readByte(int s);
short int readShort(int s);
void readString(int s, char *buff, int len);
void readEndpoint(int s, struct sockaddr_in *endpoint);

void sendByte(int s, char byte);
void sendShort(int s, short int dato);
void sendString(int s, char *buff);
void sendEndpoint(int s, struct sockaddr_in *endpoint);
