/**
######################################################################################
##  Este codigo no te lo dan, solo te dan el ejecutable para que lo uses cuando lo  ##
##  necesites a lo largo de la practica 5.1, pero gracias a mi gran genialidad      ##
##  (y a ghidra) ahora tu tambien lo tienes.                                        ##
##                                                                                  ##
##  No es que lo necesites, porque el ejercicio "cliente-chat-simple.c" ya esta     ##
##  resuelto y funcionando bien, pero yo si que lo necesitaba porque tenia la       ##
##  corazonada que esto iba a solucionar un problema con el select que no era       ##
##  capaz a resolver.                                                               ##
##                                                                                  ##
##  Si por lo que sea el "cliente-chat-simple.c" se resolvio en clase,              ##
##  no tenia forma de saberlo, no pise una en todo el semestre :)                   ##
##                                                                                  ##
##  En todo caso, de nada                                                           ##
##                                                                                  ##
##  PD:                                                                             ##
##    Despues de haber hecho casi todo me di cuenta de que esto era un              ##
##    ejercicio que te pedian hacer y para el cual te daban el guion en             ##
##    el campus virtual. Podria haberlo hecho basandome en el anterior y            ##
##    pegandome con ello, pero ya que te daban el ejecutable, era más facil         ##
##    hacer ingenieria inversa.                                                     ##
##                                                                                  ##
##    Resulta que al final si que te venia bien. De Nada :))                        ##
######################################################################################
*/

#include "cliente-con-servidor.h"
#include "protocolo.h"

// User info
char *nick;
char *server_ip_addr;
int server_port;

int main(int argc, char *argv[]) {

    // Infrastructure data
    int socketUDP, resultado;
    size_t nick_len;
    fd_set escucha;
    struct sockaddr_in dirUDP;

    // Si el numero de argumentos es incorrecto, mostramos uso
    if (argc != 4) {
        imprimir_uso(argv[0]);
    }

    // Asignamos los valores
    nick = argv[1];
    server_ip_addr = argv[2];
    server_port = atoi(argv[3]);

    // Si el nick es muy largo lo cortamos
    nick_len = strlen(nick);
    if (nick_len > MAX_TAM_NICK) {
        nick[MAX_TAM_NICK] = '\0';
    }

    // Creamos un socket udp para la comunicacion
    socketUDP = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketUDP == -1) {
        perror("Al crear el socket UDP");
        exit(1);
    }

    // Inicializamos lo que necesita el socket para bind
    dirUDP.sin_family = AF_INET;
    dirUDP.sin_port = htons(0);
    dirUDP.sin_addr.s_addr = htonl(INADDR_ANY);

    // Hacemos bind y vemos si falla o no
    resultado = bind(socketUDP, (struct sockaddr *) &dirUDP, sizeof(dirUDP));
    if (resultado == -1)
    {
        perror("Al hacer bind");
        exit(1);
    }

    socklen_t dirUDP_len = sizeof(dirUDP);
    resultado = getsockname(socketUDP, (struct sockaddr *)&dirUDP, &dirUDP_len);
    if (resultado == -1) {
        perror("Al llamar a getsockname");
        exit(EXIT_FAILURE);
    }

    // Mensajes de uso y bienvenida al usuario
    printf("Mi IP es %s\n", inet_ntoa(dirUDP.sin_addr));
    printf("Mi puerto UDP es %d\n", ntohs(dirUDP.sin_port));

    resultado = join_new_user(nick, ntohs(dirUDP.sin_port));
    if (resultado != 0) {
        fwrite("ERROR: el nick ya estaba en uso. Prueba otro",1,0x2c,stderr);
        exit(2);
    }

    printf("Usa el comando /CHAT <nick> para indicar el destino de tus mensajes\n");
    printf("En cualquier momento puedes poner /CHAT <nick> de nuevo\n");
    printf("para cambiar a un nuevo contacto.\n");

    printf("%s> ", nick);

    while (1) {
        fflush(stdout);

        FD_ZERO(&escucha);
        FD_SET(socketUDP, &escucha);
        FD_SET(STDIN_FILENO, &escucha);

        select(socketUDP + 1, &escucha, NULL, NULL, NULL);

        if (FD_ISSET(socketUDP, &escucha)) {
            recibir_y_mostrar_mensaje(socketUDP);
            printf("%s> ", nick);
        }

        if (FD_ISSET(STDIN_FILENO, &escucha)) {
            leer_y_procesar_teclado(socketUDP);
            printf("%s> ", nick);
        }
    }
    return 0;
}

void imprimir_uso(char *prog_name) {
    printf("Uso: %s nick ip_servidor puerto_servidor\n", prog_name);
    exit(0);
}

/**
    Esta funcion es la misma que la del archivo cliente-chat-simple.c

    ¿Para que voy a hacer ingenieria inversa de algo que ya existe y
    sabemos que hace lo mismo?
*/
void recibir_y_mostrar_mensaje(int socketUDP)
{
    char buff[MAX_TAM_LINEA];
    int recibidos;

    recibidos = recvfrom(socketUDP, buff, MAX_TAM_LINEA, 0, NULL, NULL);
    if (recibidos == -1) return; // Ignoramos silenciosamente errores
    buff[recibidos] = 0;
   printf("\n\033[90m**|%s|\033[0m\n", buff);
}

/**
    Lee y procesa el teclado de forma parecida a la del ejercicio anterior, aunque no igual.
*/
void leer_y_procesar_teclado(int socketUDP)
{
    char linea[MAX_TAM_LINEA];
    char cmd[MAX_TAM_LINEA];
    static char ip_destino[IP_LEN] = "No asignada";
    static int  puerto_destino = 0;
    static struct sockaddr_in dir_destino;
    char *mensaje;
    int i;

    int resultado, socket_send;

    // Leer la línea
    fgets(linea, MAX_TAM_LINEA - MAX_TAM_NICK, stdin);

    // Eliminarle el retorno de carro o espacios al final
    for (i = strlen(linea) - 1; i >= 0; i--)
    {
        if (isspace(linea[i]))  // isspace está en <ctype.h>, es una macro
            linea[i] = 0;
        else
            break;
    }

    // Ver si es un comando
    if (linea[0] == '/')
    {
        // Comienza por /, en ese caso hay que procesarlo. 
        // Lo más fácil es leer sus contenidos con sscanf
        sscanf(linea, "%s", cmd);
        if (strcasecmp(cmd, "/CHAT") == 0)
        {
            // podria haberlo puesto al inicio de la func, pero no quiero
            char peer_nick[MAX_TAM_NICK];
            sscanf(linea,"%*s %s", peer_nick);

            // Encontramos los datos del peer por nombre
            resultado = find_peer_by_nick(peer_nick, &dir_destino);
            
            // en caso afirmativo, guay
            if (resultado == 0) {
                dir_destino.sin_family = AF_INET;
                puerto_destino = ntohs(dir_destino.sin_port);
                sscanf(inet_ntoa(dir_destino.sin_addr), "%s", ip_destino);
                
                printf("Se han cambiado la IP y el puerto de destino a: %s:%d\n", ip_destino, puerto_destino);
            }
            else {  // Si no, pues no se ha encontrado el peer
                printf("Peer not found\n");
            }
        }
        else if (strcasecmp(cmd, "/QUIT") == 0){
            // Salimos del server borrando nuestras huellas
            leave_server(nick);
            exit(0);
        }
        else {
            printf("ERROR: comando no reconocido\n");
        }
    }
    else      // Esta parte es igual que el ejercicio anterior
    {
        // Hay que enviarlo a la ip y puertos antes asignados, 
        // pero antes comprobamos si efectivamente se asignaron
        // antes estos valores
        if (strcmp(ip_destino, "No asignada") == 0)
        {
            printf("ERROR: antes de enviar mensaje debes usar el comando\n");
            printf("/CHAT <nick>\n");
            return;
        }
        // Si todo va bien, enviamos el mensaje, 
        // precedido por el nick del usuario

        // Crear el mensaje a enviar (incluyendo el nick)
        mensaje = (char *)malloc((strlen(linea) + strlen(nick)) * sizeof(char));
        sprintf(mensaje, "%s> %s", nick, linea);

        // Enviar el mensaje al peer
        int bytes_enviados = sendto(socketUDP, mensaje, strlen(mensaje),
                    0, (struct sockaddr *)&dir_destino, sizeof(dir_destino));
        if (bytes_enviados == -1)
        {
            perror("Error al enviar mensaje");
            return;
        }

        // Liberar memoria del mensaje
        free(mensaje);
    }
}

// Intenta crear un nuevo usuario en el servidor
int join_new_user(char *nickname, int local_port)
{
    char byte_received;
    int sockTCP, result, return_value;
    size_t nick_len;
    
    nick_len = strlen(nickname);

    // Crea un nuevo socket de datos y se conecta
    sockTCP = CrearSocketDatosTCP();
    result = Conectar(sockTCP,server_ip_addr,server_port);

    // Comprobamos que no falla la conx
    if (result == -1) {
        puts("ERROR: no se puede contactar con el servidor");
        exit(1);
    }

    // Ni idea de que hace esto pero funciona. Asi lo decompilo ghidra
    // Yo solo hice limpieza. Si tienes curiosidad, la decompilacion
    // pone esto literalmente (despues de que yo renombrara los simbolos):

    /*
    sendByte(__fd,1);
    sendShort(__fd,(int)(short)sVar3);
    sendString(__fd,param_1);
    sendShort(__fd,(int)param_2);
    */

    sendByte(sockTCP,JOIN_CMD);
    sendShort(sockTCP,(short)nick_len);
    sendString(sockTCP,nickname);
    sendShort(sockTCP,(int)local_port);

    // Recibimos respuesta del server
    byte_received = readByte(sockTCP);
    close(sockTCP);

    // Esto es que fue bien
    if (byte_received == '\0') 
    {
        return_value = 0;
    }
    else    // Y esto es que en algun lado murio
    {
        return_value = 0xffffffff;
    }

    return return_value;
}


// Encuentra la IP y puerto del peer que se busca por el nick
int find_peer_by_nick(char *peer_nick, struct sockaddr_in* endpoint)
{
    char byte_received;
    int sockTCP;
    int result, return_value;
    size_t peer_nick_len;
    
    peer_nick_len = strlen(peer_nick);

    // Creamos el socket TCP y nos conectamos
    sockTCP = CrearSocketDatosTCP();
    result = Conectar(sockTCP,server_ip_addr,server_port);
    
    if (result == -1) {
        puts("ERROR: no se puede contactar con el servidor");
        exit(1);
    }

    // Por lo que entiendo esto es parte del protocolo
    // Supongo que es la forma de hacerle saber al
    // Servidor que es lo que queremos hacer con 
    // Esta peticion.
    sendByte(sockTCP, QUERY_CMD);
    sendShort(sockTCP, (short)peer_nick_len);
    sendString(sockTCP, peer_nick);

    // Recibimos respuesta
    byte_received = readByte(sockTCP);

    if (byte_received == '\0') {
        // Si hay un peer con ese nombre, entonces
        // se vuelcan los datos del sockaddr_in del
        // peer en un endpoint local (parametro)
        // para que nosotros sepamos a quen tenemos que enviar la comunicacion.
        readEndpoint(sockTCP, endpoint);
        return_value = 0;
    }
    else {
        return_value = 0xffffffff;
    }
    close(sockTCP);
    return return_value;
}


// Abandona el servidor borrando el nick para que 
// quede disponible de nuevo. Mismo drill que los otros.
int leave_server(char *nick)
{
    char byte_received;
    int sockTCP, return_value, result;
    size_t nick_len;
    
    nick_len = strlen(nick);
    sockTCP = CrearSocketDatosTCP();
    result = Conectar(sockTCP,server_ip_addr,server_port);

    if (result == -1) {
        puts("ERROR: no se puede contactar con el servidor");
        exit(1);
    }

    sendByte(sockTCP,LEAVE_CMD);
    sendShort(sockTCP,(short)nick_len);
    sendString(sockTCP,nick);

    byte_received = readByte(sockTCP);
    close(sockTCP);

    if (byte_received == '\0') {
        return_value = 0;
    }
    else {
        return_value = 0xffffffff;
    }

    return return_value;
}

// Santo dios como me quiero y que listo soy