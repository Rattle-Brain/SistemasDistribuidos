#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>
#include <semaphore.h>

// Archivos de cabecera para manipulación de sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>

#include "cola.h"
#include "util.h"

#define MAX_HILOS_WORK 10

struct param_hilo_aten
{
    int num_hilo;
    int s;
};

typedef struct param_hilo_aten param_hilo_aten;

// ====================================================================
// PROTOTIPOS FUNCIONES
// ====================================================================
static void handler(int signum);

// ====================================================================
// VARIABLES GLOBALES
// ====================================================================

// Cola donde se introducen las consultas que van llegando al servidor DNS
Cola cola_peticiones;

// Puerto en el que esperamos los mensajes
int puerto;

// Variable booleana que indica si el socket es orientado a conexión o no
unsigned char es_stream = CIERTO;

// Variable que almacena el numero de hilos de atencion de peticiones
int num_hilos_aten;

// Variable que almacena el numero de hilos trabajadores
int num_hilos_work;

// Puntero a la dirección de comienzo del array de datos de hilo
// de los hilos de atencion de peticiones
pthread_t *hilos_aten;

// Puntero a la dirección de comienzo del array de datos de hilo
// de los hilos trabajadores
pthread_t *hilos_work;

// Tamanio de la cola circular
int tam_cola;

// Variable global que almacena el nombre del fichero de registros
char *nomfrecords = NULL;

// Mutex de exclusión al fichero de salida
pthread_mutex_t mfsal;

// Puntero a FILE del fichero de salida
FILE *fpsal = NULL;

// nombre que se le da al fpsal para guardarlo en disco
char * fpsal_nombre;


// ====================================================================
// FUNCION handler de las señales recibidas por el proceso
// ====================================================================
static void handler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        // Finalización. Destruir mutex, semaforo, cola y liberar memoria

        destruir_cola(&cola_peticiones);

        free(hilos_aten);
        free(hilos_work);

        pthread_mutex_destroy(&mfsal);
        fclose(fpsal);

        exit(0);
    default:
        pthread_exit(NULL);
    }
}

void procesa_argumentos(int argc, char *argv[])
{
    if (argc != 8)
    {
        fprintf(stderr, "Forma de uso: %s {t|u} puerto fichero_registros tam_cola num_hilos_aten num_hilos_worker fich_log \n", argv[0]);
        exit(1);
    }
    // Verificación de los argumentos e inicialización de las correspondientes variables globales.
    // Puedes usar las funciones en util.h

    // A RELLENAR
    // Comprobamos el protocolo
    if(strcmp(argv[1], "u") == 0)
    {
        es_stream = FALSO;
    }
    else if (strcmp(argv[1], "t") != 0 && strcmp(argv[1], "u") != 0) 
    {
        perror("Protocolo invalido.\n\tt - TCP\n\tu - UDP\n");
        exit(EXIT_FAILURE);
    }

    nomfrecords = argv[3];

     // Validamos puerto
    if(valida_numero(argv[2]))
    {
        puerto = atoi(argv[2]);
        if(!puerto_en_rango(puerto))
        {
            perror("Puerto no valido. Fuera de rango\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        perror("Puerto no valido. No numerico\n");
        exit(EXIT_FAILURE);
    }


    if(valida_numero(argv[4]) && atoi(argv[4]) > 0)
    {
        tam_cola = atoi(argv[4]);
    }
    else
    {
        perror("Tamaño de cola inválido.\n");
    }

    // Comprobamos el numero de hilos de atencion
    if(valida_numero(argv[5]) && atoi(argv[5]) <= MAX_HILOS_WORK && atoi(argv[5]) >= 1)
    {
        num_hilos_aten = atoi(argv[5]);
    }
    else 
    {
        fprintf(stderr, "Numero de hilos de atencion no valido. MAX %d\n", MAX_HILOS_WORK);
        exit(EXIT_FAILURE);
    }

    // Comprobamos el numero de hilos worker
    if(valida_numero(argv[6]) && atoi(argv[6]) <= MAX_HILOS_WORK && atoi(argv[6]) >= 1)
    {
        num_hilos_work = atoi(argv[6]);
    }
    else 
    {
        fprintf(stderr, "Numero de hilos worker no valido. MAX %d\n", MAX_HILOS_WORK);
        exit(EXIT_FAILURE);
    }

    fpsal_nombre = argv[7];
}

// Función de utilidad para saber si la consulta DNS es del tipo
// que puede tener varios resultados
int es_multiresultado(char *tiporecord)
{
    if ((strcmp(tiporecord, "NS") == 0) || (strcmp(tiporecord, "MX") == 0))
        return CIERTO;
    else
        return FALSO;
}

// Función de utilidad para separar el mensaje recibido en la petición
// en sus distintos campos ()
// Los campos dominio y char son punteros que se devuelven por referencia
// el campo clave es un array que se rellena con la clave si existe
void procesa_mensaje_recibido(char *msg, char **dominio,
                              char **record, char *clave)
{
    char *token = NULL; // para extraer los tokens del mensaje
    char *loc = NULL;

    // extraemos los tokens del mensaje
    token = strtok_r(msg, ", \n", &loc);
    *dominio = strdup(token);
    token = strtok_r(NULL, ", \n", &loc);
    *record = strdup(token);

    if ((strcmp(*record, "NS") == 0) || (strcmp(*record, "MX") == 0))
    {
        // Se busca un registro de tipo NS o MX
        clave[0] = 0; // No hay clave de búsqueda
    }
    else
    {
        // se trata de un registro A, CNAME, PTR o AAAA
        // tenemos que leer la clave de búsqueda
        token = strtok_r(NULL, ", \n", &loc);
        strcpy(clave, token);
    }
}

// Función de utilidad para ver si los campos leidos de un registro
// coinciden con los buscados
int coinciden_campos(char *domleido, char *recordleido, char *claveleida,
                     char *dombuscado, char *recordbuscado, char *clavebuscada)
{
    // Simplemente se mira que las tres cadenas buscadas coincidan con las leidas
    if ((strcmp(dombuscado, domleido) == 0) && (strcmp(recordbuscado, recordleido) == 0) && (strcmp(clavebuscada, claveleida) == 0))
        return CIERTO;
    else
        return FALSO;
}

// ====================================================================
// Implementación de los hilos
// ====================================================================

void *Worker(int *id)
{
    int id_worker;                      // numero que identifica este hilo
    char pantalla[TAMPANTALLA];         // para mensajes de depuración
    dato_cola *pet;                     // datos de la peticion (sacados de la cola)
    char *dombuscado = NULL;            // dominio a buscar
    char *recordbuscado = NULL;         // tipo de registro a buscar
    char clavebusqueda[256];            // clave a buscar

    FILE *fp;                           // fichero de registros de donde leer                         
    char linea[TAMMSG];                 // linea leida del fichero de registros
    char claveleida[256];               // clave leida del fichero
    char *domleido = NULL;              // dominio leido del fichero
    char *recordleido = NULL;           // tipo de registro leido del fichero
    char *valorrecord = NULL;           // valor del registro leido del fichero
    char *token = NULL;                 // para extraer los tokens de la linea leida
    char *loc = NULL;                   // para extraer los tokens de la linea leida
    unsigned char primera;              // para saber si es el primer resultado de la consulta

    char ip_cliente[INET_ADDRSTRLEN];   // ip del cliente para el log
    time_t timeraw;                     // para obtener la fecha y hora
    char *fechahora = NULL;             // para escribirla en el log
    unsigned char puerto_cliente;       // puerto del cliente para el log
    char msg[TAMMSG];                   // mensaje de respuesta al cliente


    id_worker = *id;

    // Liberamos la memoria reservada para el identificador del
    // hilo trabajador
    free(id);

    // Información de depuración
    sprintf(pantalla, "Comienza el Worker %d\n", id_worker);
    log_debug(pantalla);

    // Codigo del worker
    while (1)
    {
        // Obtener de la cola la peticion a procesar, e imprimir
        // un mensaje de depuración mostrando el id del hilo y el mensaje
        // extraido de la cola
        // A RELLENAR
        pet = obtener_dato_cola(&cola_peticiones);
        sprintf(pantalla, "Hilo %d recibe peticion\n\tMensaje: %s\n", id_worker, pet->msg);
        log_debug(pantalla);

        fp = fopen(nomfrecords, "r");
        if (fp == NULL)
        {
            sprintf(pantalla, "Worker %d: No se pudo abrir el fichero %s\n", id_worker, nomfrecords);
            log_debug(pantalla);
        }
        else // aqui realizamos la búsqueda de la respuesta a la consulta DNS
        {
            // Separar el mensaje en sus constituyentes, con ayuda de la función
            // procesa_mensaje_recibido()
            // A RELLENAR
            procesa_mensaje_recibido(pet->msg, &dombuscado, &recordbuscado, clavebusqueda);

            primera = CIERTO;
            bzero(msg, TAMMSG);
            while (fgets(linea, TAMMSG, fp) != NULL)
            {
                linea[strlen(linea) - 1] = 0;
                token = strtok_r(linea, ", \n", &loc);
                domleido = strdup(token);
                token = strtok_r(NULL, ", \n", &loc);
                recordleido = strdup(token);
                if (es_multiresultado(recordleido))
                {
                    // En este caso no hay clave, solo el record
                    claveleida[0] = 0;
                    token = strtok_r(NULL, ", \n", &loc);
                    valorrecord = strdup(token);
                }
                else // En este caso hay clave y después record
                {
                    token = strtok_r(NULL, ", \n", &loc);
                    strcpy(claveleida, token);
                    token = strtok_r(NULL, ", \n", &loc);
                    valorrecord = strdup(token);
                }

                if (coinciden_campos(domleido, recordleido, claveleida,
                                     dombuscado, recordbuscado, clavebusqueda))
                {
                    // hemos encontrado un valor para responder a la consulta
                    // vamos añadiéndolo a la respuesta
                    if (primera) // si es el primer resultado de la consulta
                    {
                        sprintf(msg, "%s", valorrecord);
                        primera = FALSO;
                    }
                    else // no es el primer resultado
                    {
                        // Hay que añadirle al msg previo el nuevo campo
                        char aux[TAMMSG];
                        sprintf(aux, "%s:%s", msg, valorrecord);
                        strcpy(msg, aux);
                    }

                    // Si no se trata de un registro que pueda tener varios resultados
                    // salimos del bucle pues ya hemos encontrado un resultado
                    // A RELLENAR
                    if (!es_multiresultado(recordleido))
                    {
                        break;
                    }
                }
            }
            fclose(fp);

            // terminada la búsqueda lo que queda por hacer es escribir la línea
            // apropiada en el fichero de log, y enviar la respuesta al cliente

            // Recopilando los datos para el log. Primero la IP y puerto del cliente
            inet_ntop(AF_INET, &(pet->d_cliente.sin_addr.s_addr), ip_cliente, sizeof(ip_cliente));
            puerto_cliente = ntohs(pet->d_cliente.sin_port);
            
            // Después la fecha y hora
            timeraw = time(NULL);
            fechahora = ctime(&timeraw);
            fechahora[strlen(fechahora) - 1] = '\0';

            // Escribimos la línea en el fichero de log (con exclusión mutua entre workers)
            // A RELLENAR
            pthread_mutex_lock(&mfsal);
            fprintf(fpsal, "%s,%d,%s,%s,%s,%s,%s\n", ip_cliente, puerto_cliente,
                        fechahora, dombuscado, recordbuscado, clavebusqueda, valorrecord);
            pthread_mutex_unlock(&mfsal);

            // Enviar respuesta al cliente
            if (es_stream)
            {
                // A RELLENAR
                send(pet->s, msg, strlen(msg), 0);
            }
            else
            {
                // A RELLENAR
                sendto(pet->s, msg, strlen(msg), 0, (struct sockaddr *)&(pet->d_cliente), sizeof(pet->d_cliente));
            }
            
            // Liberar memoria
            if (dombuscado != NULL)
                free(dombuscado);
            if (domleido != NULL)
                free(domleido);
            if (recordbuscado != NULL)
                free(recordbuscado);
            if (recordleido != NULL)
                free(recordleido);
            if (valorrecord != NULL)
                free(valorrecord);
        }
        free(pet);
    }
}

void *AtencionPeticiones(param_hilo_aten *q)
{
    int sock_dat, recibidos;
    struct sockaddr_in d_cliente;
    socklen_t l_dir = sizeof(d_cliente);
    char pantalla[TAMPANTALLA];
    char buffer[TAMMSG];
    dato_cola *p;
    int s; // Variable local para almacenar el socket pasivo

    // Información de depuración
    sprintf(pantalla, "Comienza el Hilo de Atencion de Peticiones %d\n", q->num_hilo);
    log_debug(pantalla);

    // Extraemos el socket pasivo del parámetro y liberamos la memoria que
    // había sido reservada para el parámetro desde main
    s = q->s;

    free(q);

    while (1) // Bucle infinito de atencion de mensajes
    {
        bzero(&d_cliente, sizeof(d_cliente));
        bzero(buffer, TAMMSG);
        if (es_stream)  // TCP
        {
            // Aceptar cliente y leer en buffer el mensaje que éste envíe
            // No se cierra el socket de datos, pues lo necesitará el worker
            // y lo cerrará él
            // A RELLENAR

            // Aceptamos la conexion
            sock_dat = accept(s, 0, 0);
            if (sock_dat < 0)
            {
                perror("Error al aceptar la conexion\n");
                exit(EXIT_FAILURE);
            }


            // Recibimos el mensaje del cliente
            recibidos = read(sock_dat, buffer, sizeof(buffer));
            if (recibidos < 0)
            {
                perror("Error al recibir el mensaje\n");
                exit(EXIT_FAILURE);
            }
            else if (recibidos == 0)
            {
                printf("Cliente desconectado\n");
                close(sock_dat);
                continue;
            }
        }
        else // UDP
        {
            recibidos = recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&d_cliente, &l_dir);
            buffer[recibidos] = 0;          // Añadir el terminador de cadena
            buffer[strlen(buffer) - 1] = 0; // Quitar el retorno de carro
        }

        // Reservar memoria para el dato a meter en la cola
        p = (dato_cola *)malloc(sizeof(dato_cola));
        if (p == NULL)
        {
            fprintf(stderr, "No se pudo reservar memoria para una nueva peticion\n");
            exit(15);
        }

        // Rellenar el dato apuntado por p e insertarlo en la cola

        // Copiar la dirección del cliente y el mensaje
        memcpy(&(p->d_cliente), &d_cliente, sizeof(struct sockaddr_in));
        strncpy(p->msg, buffer, TAMMSG);

        // Copiar el socket que debe usar el worker y meter el dato en la cola
        // A RELLENAR
        
        //Copiamos los datos que faltan
        if(es_stream)
            p->s = sock_dat;
        else
            p->s = s;
        p->d_cliente = d_cliente;

        // Insertamos el dato en la cola
        insertar_dato_cola(&cola_peticiones, p);
    }
}

// ====================================================================
// PROGRAMA PRINCIPAL
// ====================================================================

// Su misión es crear e inicializar los recursos de sincronización globales,
// lanzar todos los hilos

int main(int argc, char *argv[])
{
    register int i;   // Indice para bucles
    int *id;          // para pasar el id del hilo trabajador
    int sock;         // socket para el hilo de atención
    struct sockaddr_in d_local; // para el bind
    param_hilo_aten *q;  // Para crear los parámetros de los hilos de atención

    procesa_argumentos(argc, argv);

    // Append al fichero fpsal. "a+" indica que si no existe se crea uno nuevo.
    fpsal = fopen(fpsal_nombre, "a+");
    if (fpsal == NULL)
    {
        perror("Error al abrir el archivo de registro\n");
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL); // quitamos el buffer de la salida estandar
    signal(SIGINT, handler); // establecemos el comportamiento ante la llegada asíncrona de la señal

    d_local.sin_family = AF_INET;
    d_local.sin_addr.s_addr = htonl(INADDR_ANY);
    d_local.sin_port = htons(puerto);

    // Inicializar el socket (teniendo en cuenta si es orientado a conexión o no)
    // y asignarle el puerto de escucha
    // A RELLENAR

    // Creamos un socket u otro dependiendo del valor de es_stream
    if(es_stream) // Es TCP
    {
        sock = socket(PF_INET, SOCK_STREAM, 0);

    }
    else // Es UDP
    {
        sock = socket(PF_INET, SOCK_DGRAM, 0);
    }
    
    // Comprobamos errores al crear socket
    if (sock < 0)
    {
        perror("Error al crear el socket\n");
        exit(EXIT_FAILURE);
    }

    // Hacemos bind para dar nombre al socket
    if (bind(sock, (struct sockaddr *)&d_local, sizeof(d_local)) < 0)
    {
        perror("Error al realizar el bind\n");
        exit(EXIT_FAILURE);
    }


    // Establecer el socket en modo de escucha si es TCP
    if (es_stream)
    {
        if (listen(sock, 5) < 0)
        {
            perror("Error al poner el socket en modo de escucha\n");
            exit(EXIT_FAILURE);
        }
    }

    // creamos el espacio para los objetos de datos de hilo
    hilos_aten = (pthread_t *)malloc(sizeof(pthread_t) * num_hilos_aten);
    if (hilos_aten == NULL)
    {
        fprintf(stderr, "ERROR: No se pudo reservar memoria para los objetos de datos de hilo de atencion\n");
        exit(13);
    }
    hilos_work = (pthread_t *)malloc(sizeof(pthread_t) * num_hilos_work);
    if (hilos_work == NULL)
    {
        fprintf(stderr, "ERROR: No se pudo reservar memoria para los objetos de datos de hilo trabajadores\n");
        exit(14);
    }

    // inicializamos la cola
    inicializar_cola(&cola_peticiones, tam_cola);

    // Inicializamos los mutex de exclusión al fichero de log
    pthread_mutex_init(&mfsal, NULL);

    // creamos un hilo por cada agente de atencion, pasándole el parámetro apropiado
    for (i = 0; i < num_hilos_aten; i++)
    {
        q = (param_hilo_aten *)malloc(sizeof(param_hilo_aten));
        
        // A RELLENAR
        if (q == NULL)
        {
            perror("Error al asignar memoria para el parámetro del hilo de atención\n");
            exit(EXIT_FAILURE);
        }

        q->s = sock;
        q->num_hilo = i;
        
        if (pthread_create(&hilos_aten[i], NULL, (void *)AtencionPeticiones, q) != 0)
        {
            perror("Error al crear el hilo de atención\n");
            exit(EXIT_FAILURE);
        }
    }

    // creamos un hilo por cada worker, pasándole el parámetro apropiado
    for (i = 0; i < num_hilos_work; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos_work[i], NULL, (void *)Worker, (void *)id);
    }


    // Esperar a que terminen todos los hilos (espera infinita en realidad
    // pues los hilos no terminan nunca, salvo que se reciba una señal)
    for (i = 0; i < num_hilos_aten; i++)
    {
        pthread_join(hilos_aten[i], NULL);
    }
    for (i = 0; i < num_hilos_work; i++)
    {
        pthread_join(hilos_work[i], NULL);
    }
}
