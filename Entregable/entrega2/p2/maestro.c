// Implementación de un maestro de recursos para
// coordinacion distribuida
#include <sys/socket.h> // socket, send, recv...
#include <netinet/in.h> // sockaddr_in, htons, htonl, etc.
#include <arpa/inet.h>  // inet_addr
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>  //prototipos de funciones para mutex

#define MAX_LINEA 80 // Maxima longitud esperada para una lectura

//Estructura que se le va a pasar a cada hilo que atiende una petición
typedef struct {
    int puerto;
    pthread_mutex_t * lista;
    int tamlista;
}
petinfo;

// Esta función crea el socket de escucha, le asigna el número de
// puerto y le pone en modo "listen". Retorna el socket creado.
// El código de la función está más abajo (después del main())
int InicializarSocketEscucha(int puerto);

// Esta función hace una lectura del socket de datos.  Si recibe cero,
// retorna. En otro caso, lee del socket, interpreta el mensaje,
// hace la operacion correspondiente sobre la posicion del array de mutex
// y envia confirmacion
void OperarMutex(petinfo * pet, int socket_datos, int * lista, int listalen);

// Esta funcion inicializa cada posicion del array de mutex

void InicializaMutex(pthread_mutex_t * lista, int tam);

//Procesa una nueva peticion de acceso a un recurso
void * procesaPeticion(petinfo * p);

// Programa principal.
// Crea un socket de escucha y queda a la espera de un cliente.
// Cuando llega una nueva peticion, genera un nuevo hilo
// que realmente es el que atiende la peticion
int main(int argc, char * argv[]) {
    int sockEscucha; // Socket por el que se esperan clientes
    int sockDatos; // Socket por el que se reciben datos de los
    // clientes y se envía la respuesta
    struct sockaddr_in dir_cliente;
    int long_dir_cliente = sizeof(dir_cliente);
    //puerto de escucha de peticiones
    int puerto;
    //numero de mutex que protegen los recursos, uno por cada recursos
    int num_mutex;
    //puntero a la dirección base del array dinámico de mutex
    pthread_mutex_t * mutex;
    //objeto de datos de hilo donde se almacena el id de cada hilo lanzado
    pthread_t tid;
    petinfo * info;

    //comprobación del número de agumentos
    if (argc < 3) {
        fprintf(stderr, "Forma de uso: %s puerto num_mutex\n", argv[0]);
        exit(1);
    }
    /*** lectura de los argumentos y almacenamiento
         en las variables oportunas ***/
    puerto = atoi(argv[1]);
    if ((puerto < 0) || (puerto > 65535)) {
        fprintf(stderr, "Forma de uso: numero de puerto no válido.\n");
        exit(2);
    }
    num_mutex = atoi(argv[2]);
    if (num_mutex < 0) {
        fprintf(stderr, "Forma de uso: el numero de mutex debe ser positivo.\n");
        exit(3);
    }
    /*********************************************************************/
    // Informar al usuario del número de puerto que se usará
    printf("Master  recursos: usando el puerto = % d\n", puerto);

    // Eliminar el buffer de printf, para que los printfs tengan efecto
    // inmediato y sea más fácil depurar el programa
    setvbuf(stdout, NULL, _IONBF, 0);

    //reservamos espacio para los mutex
    mutex = (pthread_mutex_t * ) malloc(sizeof(pthread_mutex_t) * num_mutex);
    if (mutex == NULL) {
        fprintf(stderr, "No se pudo reservar espacio para los mutex.\n");
        exit(4);
    }
    //inicializamos los mutex que protegen a los recursos
    InicializaMutex(mutex, num_mutex);
    //se crea e inicializa el socket de escucha
    sockEscucha = InicializarSocketEscucha(puerto);
    // Bucle infinito, se repite para cada cliente que llegue
    printf("Esperando por clientes...\n");
    while (1) {
        // Esperar a que llegue un cliente
        // se reserva espacio para almacenar los datos que se le pasarán
        // al hilo que procese la siguiente petición
        info = (petinfo * ) malloc(sizeof(petinfo));
        //nos bloqueamos en el accept hasta que llega la siguiente petición
        sockDatos = accept(sockEscucha, (struct sockaddr * ) & dir_cliente,
            (socklen_t * ) & long_dir_cliente);
        //almacenamos en los campos de la estructura a la que apunta info
        //los valores que le vamos a pasar al hilo que procesa la nueva
        //petición
        info -> puerto = sockDatos;
        info -> lista = mutex;
        info -> tamlista = num_mutex;
        //se lanza el hilo que procesa la nueva petición
        pthread_create( & tid, NULL, (void * ) procesaPeticion, (void * ) info);
        //se convierte el hilo en independiente para que de esa manera
        //tan pronto termine de procesar la siguiente petición finalice
        pthread_detach(tid);

    }
}

//================================================================
// Código de las funciones auxiliares
int InicializarSocketEscucha(int puerto) {
    int ret;
    struct sockaddr_in dir;
    int socket_escucha;

    // Crear el socket
    socket_escucha = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_escucha == -1) {
        perror("Al crear socket");
        exit(1);
    }
    // Asignarle el numero de puerto
    dir.sin_family = AF_INET;
    dir.sin_port = htons(puerto);
    dir.sin_addr.s_addr = htonl(INADDR_ANY);
    ret = bind(socket_escucha, (struct sockaddr * ) & dir, sizeof(dir));
    if (ret == -1) {
        perror("Al asignar direccion");
        close(socket_escucha);
        exit(1);
    }
    // Ponerlo en modo escucha
    ret = listen(socket_escucha, SOMAXCONN);
    if (ret == -1) {
        perror("Al poner en modo escucha");
        close(socket_escucha);
        exit(1);
    }
    // El socket ya está preparado para hacer accept()
    return socket_escucha;
}

//================================================================

//funcion a ejecutar por cada hilo que procesa una petición de acceso
//a un recurso
void * procesaPeticion(petinfo * pet) {
    int leidos, escritos;
    char buffer[MAX_LINEA];
    char * ptr;
    char *palillo1, *palillo2;  // FASE 3: Modificacion del protocolo
    int poslista_1, poslista_2; // FASE 3: Modificacion del protocolo
    char valido = 1;

    // Recibir mensaje
    leidos = read(pet -> puerto, buffer, MAX_LINEA);
    if (leidos == 0) 
    {
        return NULL;
    }

    // FASE 3: Inicializacion de variables
    ptr = malloc(sizeof(char));
    palillo1 = malloc(sizeof(char));
    palillo2 = malloc(sizeof(char));


    //interpretar el mensaje
    sscanf(buffer, "%s %s %s", ptr, palillo1, palillo2);    // FASE 3: scnaf de las variables

    //ptr = strtok(buffer, " \t");
    //printf("%s\n", ptr);
    switch ( * ptr) {
    case 'l':
    case 'L':
        //ptr = strtok(NULL, " ");
        // ptr2 = strtok(NULL, " \t");
        poslista_1 = atoi(palillo1);    // FASE 3: asignacion de los palillos
        poslista_2 = atoi(palillo2);    // FASE 3: asignacion de los palillos
        
        if (((poslista_1 < 0) || (poslista_1 > (pet -> tamlista - 1))) ||
            ((poslista_2 < 0) || (poslista_2 > (pet -> tamlista - 1))))
            valido = 0;
        else{
            //esperar a tener el mutex que protege al recurso solicitado
            pthread_mutex_lock( & pet -> lista[poslista_1]);
            pthread_mutex_lock( & pet -> lista[poslista_2]);
        }
        //en este punto ya se garantiza que tenemos el acceso exclusivo
        //a dicho recurso
        break;
    case 'u':
    case 'U':
        //ptr = strtok(NULL, " ");
        // ptr2 = strtok(NULL, " \t");
        poslista_1 = atoi(palillo1);    // FASE 3: liberacion de los palillos
        poslista_2 = atoi(palillo2);    // FASE 3: liberacion de los palillos

        if (((poslista_1 < 0) || (poslista_1 > (pet -> tamlista - 1))) ||
            ((poslista_2 < 0) || (poslista_2 > (pet -> tamlista - 1))))
            valido = 0;
        else{
            //desbloquear el mutex que protege al recurso que se libera
            pthread_mutex_unlock( & pet -> lista[poslista_1]);
            pthread_mutex_unlock( & pet -> lista[poslista_2]);
        }
        break;
    default:
        valido = 0;
    }
    //componer el mensaje de respuesta
    if (valido)
        sprintf(buffer, "1");
    else
        sprintf(buffer, "0");

    // Devolver el mensaje
    escritos = strlen(buffer);
    write(pet -> puerto, buffer, escritos);
    //cerrar socket de respuesta
    close(pet -> puerto);
    //liberar la memoria donde se pasaron los argumentos
    free(pet);

    return NULL;
}

//inicialización de los mutex que protegen a los recursos
void InicializaMutex(pthread_mutex_t * lista, int tam) {
    register int i;

    for (i = 0; i < tam; i++)
        pthread_mutex_init( & lista[i], NULL);
}