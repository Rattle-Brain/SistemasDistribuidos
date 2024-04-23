#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bits/posix2_lim.h>

#define IP_LEN 16
#define MAX_TAM_NICK 10
#define MAX_TAM_LINEA 512

void imprimir_uso(char *prog_name);
void recibir_y_mostrar_mensaje(int socketUDP);
void leer_y_procesar_teclado(int socketUDP);
int join_new_user(char *nick, int local_port);
int find_peer_by_nick(char *peer_nick, struct sockaddr_in* endpoint);
int leave_server(char* nick);