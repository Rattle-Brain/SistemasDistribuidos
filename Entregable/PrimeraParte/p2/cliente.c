/*
  Cliente de RPC que simula las operaciones de varios clientes del
  servidor de DNS
*/
#include <rpc/rpc.h>
#include <sys/types.h>
#include <sys/times.h>
#include <unistd.h>
#include <pthread.h>
#include "srvdns.h"
#include "util.h"

#define TAMLINEA 1024
#define TAMMSG 1024
#define MAXHILOSCLIENTE 10

// Variables globales

// IP del proceso srvdns
char *ip_srv;

// numero de clientes
int num_clientes;

// tipo de datos que recibiran los hilos cliente
struct datos_hilo
{
    unsigned char id_cliente;
    char *nom_fichero_consultas;
};

typedef struct datos_hilo datos_hilo;

pthread_mutex_t m; // mutex para serializar la invocación remota

char *hilos_file_names[MAXHILOSCLIENTE] = {
    "sal00.dat",
    "sal01.dat",
    "sal02.dat",
    "sal03.dat",
    "sal04.dat",
    "sal05.dat",
    "sal06.dat",
    "sal07.dat",
    "sal08.dat",
    "sal09.dat"};

// Función de utilidad para dividir una línea en campos, los cuales
// retorna a través de los tres últimos parámetros por referencia
void obtener_campos_consulta(int id_cliente, char *linea, 
                    char **nomdominio, char **tiporecord, char **clave)
{
    char *token;
    char *loc;
    char msg[TAMMSG];  // Para mensajes de depuración

    linea[strlen(linea) - 1] = 0;           // Eliminar el salto de línea
    token = strtok_r(linea, ", \n", &loc);  // Extraemos el primer token del mensaje
    if (token == NULL)
    {
        sprintf(msg, "Cliente %d: Error al extraer el primer token de la consulta\n", id_cliente);
        log_debug(msg);
        exit(12);
    }
    *nomdominio = strdup(token);
    
    // Extraemos el segundo token del mensaje
    token = strtok_r(NULL, ", \n", &loc);
    if (token == NULL)
    {
        sprintf(msg, "Cliente %d: Error al extraer el segundo token de la consulta\n", id_cliente);
        log_debug(msg);
        exit(13);
    }
    *tiporecord = strdup(token);

    // Extraemos el tercer token del mensaje
    if (es_MX_o_NS(*tiporecord))
    {
        // Para los tipos MX y NS no hay clave
        *clave = strdup("");
    }
    else
    {
        token = strtok_r(NULL, ", \n", &loc);
        *clave = strdup(token);
    }
}

void *Cliente(datos_hilo *p)
{
    CLIENT *cl;             // Estructura CLIENT para la rpc
    FILE *fpin;             // Fichero de donde leer las consultas
    FILE *fpsal;            // Fichero donde escribir los resultados
    int id_cliente;         // Identificador del cliente
    char buffer[TAMLINEA];  // Buffer de lectura de lineas del fichero de eventos
    char msg[TAMLINEA];     // Mensaje para el servidor

    Resultado *res;         // Resultado de la invocacion remota
    char *s;                // Puntero a la linea leida del fichero

    paramconsulta q;        // Argumento para la RPC

    id_cliente = p->id_cliente; // Capturar el id del cliente en una variable local

    // Intentar abrir el fichero de salida (cuyo nombre depende del id del cliente)
    if ---------------------- // A RELLENAR
    {
        fprintf(stderr, "Error: cliente %d no pudo abrir el fichero de salida %s\n", id_cliente, hilos_file_names[id_cliente]);
        exit(10);
    }
    // y el fichero de consultas
    if ---------------------- // A RELLENAR
    {
        fprintf(stderr, "Error: cliente %d no pudo abrir el fichero de entrada %s\n", id_cliente, p->nom_fichero_consultas);
        exit(11);
    }
    free(p); // Ya no necesitamos el parámetro recibido, lo liberamos

    do  // Repetir para cada línea leída del fichero de consultas
    {
        // Inicializar la estructura CLIENT (controlando posibles errores)
        // A RELLENAR
        |
        |
        |
        |
        |
        |

        // Leer una línea del fichero de consultas
        bzero(buffer, TAMLINEA);
        s = fgets(buffer, TAMLINEA, fpin);
        if (s != NULL)
        {
            // Extraer los campos de la línea leída para dejarlos en los
            // campos de la estructura paramconsulta
            // A RELLENAR
            |
            |

            // Invocación remota del servicio consulta_record, protegiendo la llamada con un mutex
            // para evitar que dos hilos hagan la RPC a la vez
            // A RELLENAR
            |
            |
            |

            // Procesar la respuesta recibida, según el caso de la unión
            switch (res->caso)
            {
            case 0:
                // Volcar a fpsal el dominio consultado, el tipo de record, la clave (solo
                // si la consulta no fue tipo MX o NS) y el resultado recibido
                // A RELLENAR
                |
                |
                |
                |
                |
                |
                |
                break;
            case 2:
                sprintf(msg, "Error: Cliente %d Resultado invocacion remota: %s\n", id_cliente, res->Resultado_u.err);
                log_debug(msg);
                break;
            default:
                sprintf(msg, "Error: Cliente %d Resultado invocacion remota devuelve un valor desconocido: %d\n", id_cliente, res->caso);
                log_debug(msg);
                break;
            }

            // Liberar recursos
            if (q.nomdominio != NULL)
                free(q.nomdominio);
            if (q.tiporecord != NULL)
                free(q.tiporecord);
            if (q.clave != NULL)
                free(q.clave);
            xdr_free((xdrproc_t)xdr_Resultado, (char *)res);
        }
        clnt_destroy(cl);
    } while (s);

    // Cerrar ficheros y retornar
    fclose(fpin);
    fclose(fpsal);
    return NULL;
}

int main(int argc, char *argv[])
{
    register int i; // Indice para bucles
    pthread_t *th;
    datos_hilo *q;
    FILE *fp;
    char msg[TAMLINEA];

    // El programa comienza verificando que los argumentos de entrada son correctos
    if (argc != 4)
    {
        fprintf(stderr, "Forma de uso: %s <numero_clientes> <ip_serv_srvdns> <fich_consultas>\n", argv[0]);
        exit(1);
    }

    // Valida cada argumento y lo asigna a la variable apropiada
    // A RELLENAR
    |
    |
    |
    |
    |
    |
    |
    |


    // Intenta abrir el fichero por si hubiera problemas abortar (aunque main
    // no usa este fichero sino que se lo pasará a los hilos Cliente)
    if ((fp = fopen(argv[3], "r")) == NULL)
    {
        fprintf(stderr, "No se puede abrir el fichero con los registros DNS a consultar\n");
        exit(5);
    }
    fclose(fp);

    // Ya que los stub de cliente comparten una variable estática, para evitar que los
    // diferentes hilos cliente se pisen entre ellos, se usa un mutex para evitar
    // hacer la misma RPC desde dos hilos a la vez
    if (pthread_mutex_init(&m, NULL) != 0)
    {
        fprintf(stderr, "No se puedo inicializar el mutex que protege el acceso a la memoria estatica donde se devuelve el resultado de la invocacion remota\n");
        exit(6);
    }

    // Reservamos memoria para los objetos de datos de hilo
    th = (pthread_t *)malloc(sizeof(pthread_t) * num_clientes);
    if (th == NULL)
    {
        sprintf(msg, "Error: no hay memoria suficiente para los objetos de datos de hilo\n");
        log_debug(msg);
        exit(7);
    }

    // Creación de un hilo para cada cliente. Estos sí reciben como parámetro
    // un puntero a la estructura con su id de cliente (igual al valor del índice del bucle)
    // y el nombre del fichero de consultas
    for (i = 0; i < num_clientes; i++)
    {
        // A RELLENAR
        |
        |
        |
        |
        |
        |
        |
        |
        |
        |
        |
        |
    }

    // Esperar a que terminen los hilos Cliente
    for (i = 0; i < num_clientes; i++)
    {
        pthread_join(th[i], NULL);
    }
    pthread_mutex_destroy(&m);
    free(th);
}
