#include <time.h>
#include <string.h>
#include "srvdns.h"
#include "util.h"

// Booleano para saber si ya se ha inicializado el servidor
int inicializado = 0;

// matriz rectangular de enteros donde se va a guardar la contabilidad
// de las consultas recibidas, por dominio y tipo de registro
int **contabilidad_consultas = NULL;
int numdominios = 0;
int numtiposrec = 0;

Lista *lnomdominios = NULL; // Lista enlazada de nombres de dominios
Lista *lnomtiposrec = NULL; // Lista enlazada de nombres de tipos de registros

// Nombres de los ficheros de entrada y de salida
char *nomfrecords = "registros.dns";
char *nomflog = "log.dns";

// **********************************************************************
// Funciones auxiliares para inicializar los arrays de estadísticas
// **********************************************************************

// Rellena con ceros la matriz (a la que se le supone previamente reservada memoria)
void inicializar_matriz_consultas(int **p, int numdom, int numtrec)
{
    register int i, j;

    for (i = 0; i < numdom; i++)
        for (j = 0; j < numtrec; j++)
            p[i][j] = 0;
}

// Reserva memoria para la matriz de contadores. No recibe parámetros
// porque utiliza las variables globales numdominios y numtiposrec
void init()
{
    register int i;

    if (inicializado)
        return;

    // Se reserva espacio para los punteros a las filas
    contabilidad_consultas = (int **)malloc(sizeof(int *) * numdominios);
    if (contabilidad_consultas == NULL)
    {
        fprintf(stderr, "No se pudo reservar memoria para representar la contabilidad de los eventos");
        exit(4);
    }
    // Para cada fila se reserva espacio para la fila
    for (i = 0; i < numdominios; i++)
    {
        contabilidad_consultas[i] = (int *)malloc(numtiposrec * sizeof(int));
        if (contabilidad_consultas[i] == NULL)
        {
            fprintf(stderr, "No se pudo reservar memoria para representar la contabilidad de los eventos");
            exit(4);
        }
    }
    // Se rellenan los elementos de la matriz con valores iniciales
    inicializar_matriz_consultas(contabilidad_consultas, numdominios, numtiposrec);
    inicializado = 1;
}

// **********************************************************************
// Implementación de los procedimientos remotos
// **********************************************************************

Resultado *inicializar_srvdns_1_svc(datini *q, struct svc_req *pet)
{
    static Resultado r;

    // En q->nomdominios y q->nomtiporecords tenemos las listas de nombres
    // de dominios y de tipos de registros que se van a manejar en el servidor
    // Tenemos que inicializar las variables globales que guardarán esas
    // listas y la matriz de contabilidad de consultas

    // Inicializar los enteros de indican cuántos elementos hay en cada lista
    numdominios = obtener_longitud_lista(q->nomdominios);
    numtiposrec = obtener_longitud_lista(q->nomtiporecords);

    // Si hay menos de 1, retornamos un error
    if (numdominios < 1)
    {
        r.caso = 2;
        r.Resultado_u.err = "ERROR: Al inicializar srvdns. La lista de nombres de dominios no puede estar vacia.";
    }
    else if (numtiposrec < 1)
    {
        r.caso = 2;
        r.Resultado_u.err = "ERROR: Al inicializar srvdns. La lista de tipos de registros no puede estar vacia.";
    }
    else // En caso contrario, inicializamos la matriz y otras variables globales
    {
        r.caso = 0;
        init();
        printf("Terminada Inicialización SRVDNS\n");

        // Copiar los punteros a las listas
        lnomdominios = q->nomdominios;
        lnomtiposrec = q->nomtiporecords;

        // A modo de depuración, mostramos la matriz de contadores y retornamos
        mostrar_recuento_consultas(numdominios, numtiposrec, q->nomdominios, q->nomtiporecords, contabilidad_consultas);

        // Para evitar  que al retornar esta función se liberen los punteros
        // que hemos copiado, hay que hacerlos NULL en el parámetro
        q->nomdominios = NULL;
        q->nomtiporecords = NULL;
    }
    return &r;
}

// Esta es la RPC principal, la que recibe una consulta dns y la resuelve
Resultado *consulta_record_1_svc(paramconsulta *q, struct svc_req *peticion)
{
    static Resultado res;        // Estructura que se retornará
    char mensaje[TAMMSG];        // Para mensajes de depuración
    FILE *fp;                    // Fichero de registros
    FILE *fpsal;                 // Fichero de log
    char linea[TAMLINEA];        // Para leer líneas del fichero de registros
    char *token;                 // Para separar la línea en campos
    char *domleido = NULL;       // Campos leidos del fichero una vez separados
    char *recordleido = NULL;    // -
    char claveleida[256];        // -
    char *valorrecord = NULL;    // -
    time_t timeraw;              // Para obtener la fecha
    char *fechahora;             // Para el log
    unsigned char primera;       // Para el caso con resultado múltiple
    unsigned char fila, columna; // Para la contabilidad de consultas

    fp = fopen(nomfrecords, "r");
    if (fp == NULL)
    {
        sprintf(mensaje, "Error: No se pudo abrir el fichero %s\n", nomfrecords);
        log_debug(mensaje);
        exit(5);
    }
    // Si el fichero ha podido ser abierto, continuamos


    if (es_MX_o_NS(q->tiporecord)) // En este caso no se usa el campo clave
    {
        claveleida[0] = 0;
    }
    primera = CIERTO;
    res.Resultado_u.msg = "";  // Respuesta por defecto, si no se encuentra nada

    // Iterar por todas las líneas del fichero de registros
    // Cada línea se tokeniza (separa en campos) y los campos se comparan
    // con los de la consulta, para así ir construyendo el mensaje de respuesta
    // directamente sobre el campo apropiado en la estructura res
    while (fgets(linea, TAMLINEA, fp) != NULL)
    {
        linea[strlen(linea) - 1] = 0; // quitamos el salto de linea final
        token = strtok(linea, ", \n");
        domleido = strdup(token);
        token = strtok(NULL, ", \n");
        recordleido = strdup(token);
        if (es_MX_o_NS(recordleido))
        {
            // No hay clave, solamente el valor del registro
            token = strtok(NULL, ", \n");
            valorrecord = strdup(token);
            claveleida[0] = 0;
        }
        else // no es un registro NS o MX
        {
            token = strtok(NULL, ", \n");
            strcpy(claveleida, token);
            token = strtok(NULL, ", \n");
            valorrecord = strdup(token);
        }
        // Si coinciden los tres campos con los de la consulta...
        if ------------------------------------------- // A RELLENAR
        {
            // hemos encontrado un valor para responder a la consulta
            // vamos añadiéndolo a la respuesta
            if (primera) // si es el primer resultado de la consulta
            {
                res.caso = 0;
                res.Resultado_u.msg = (char *)malloc(sizeof(char) * TAMMSG);
                sprintf(res.Resultado_u.msg, "%s", valorrecord);
                primera = FALSE;
            }
            else // no es el primer resultado
            {
                char aux[TAMMSG];
                sprintf(aux, "%s:%s", res.Resultado_u.msg, valorrecord);
                strcpy(res.Resultado_u.msg, aux);
            }

            // Si el registro buscado no es NS o MX dejamos de buscar
            // A RELLENAR
            |
            |
            |
            |
        }
    }
    fclose(fp);

    // Actualizar el elemento adecuado de la matriz de contadores
    // La fila y columna son índices que no se saben a priori, es necesario
    // buscarlos en las listas de nombres de dominios y de tipos de registros
    // con ayuda de la función posicion_en_lista
    // A RELLENAR
    |
    |
    |
    |


    // Añadir la consulta al fichero de log
    if ((fpsal = fopen(nomflog, "a")) != NULL)
    {
        timeraw = time(NULL);
        fechahora = ctime(&timeraw);
        fechahora[strlen(fechahora) - 1] = '\0';
        if (q->clave[0] != 0)
        {
            fprintf(fpsal, "%s,%s,%s,%s,%s\n", fechahora, q->nomdominio, q->tiporecord, q->clave, res.Resultado_u.msg);
        }
        else
        {
            fprintf(fpsal, "%s,%s,%s,%s\n", fechahora, q->nomdominio, q->tiporecord, res.Resultado_u.msg);
        }
        fclose(fpsal);
    }
    else
    {
        sprintf(mensaje, "Error: No se pudo abrir el fichero %s\n", nomflog);
        log_debug(mensaje);
    }
    return (&res);
}

Resultado *obtener_total_dominio_1_svc(int *ndxdom, struct svc_req *peticion)
{
    // Esta función obtiene el total de consultas recibidas para un dominio dado
    // por su índice

    static Resultado res;
    register int i;

    if ((*ndxdom < 0) || (*ndxdom > (numdominios - 1)))
    {
        res.caso = 2;
        res.Resultado_u.err = (char *)malloc(sizeof(char) * TAMMSG);
        sprintf(res.Resultado_u.err, "ERROR: El valor %d de numero de dominio esta fuera de rango", *ndxdom);
    }
    else
    {
        // A RELLENAR
        |
        |
        |
        |
        |
        |
    }
    return (&res);
}

Resultado *obtener_total_registro_1_svc(int *ndxrec, struct svc_req *peticion)
{
    // Esta función obtiene el total de consultas recibidas para un tipo de registro
    // dado por su índice

    static Resultado res;
    register int i;

    if ((*ndxrec < 0) || (*ndxrec > (numtiposrec - 1)))
    {
        res.caso = 2;
        res.Resultado_u.err = (char *)malloc(sizeof(char) * TAMMSG);
        sprintf(res.Resultado_u.err, "ERROR: El valor %d de tipo de registro esta fuera de rango", *ndxrec);
    }
    else
    {
        // A RELLENAR
        |
        |
        |
        |
        |
    }
    return (&res);
}

Resultado *obtener_total_dominioregistro_1_svc(domrecord *q, struct svc_req *peticion)
{
    // Esta función retorna el contador asociado a una pareja dominio-tipo de registro
    // dados por sus índices

    static Resultado res;

    if ((q->ndxdom < 0) || (q->ndxdom > (numdominios - 1)))
    {
        res.caso = 2;
        res.Resultado_u.err = (char *)malloc(sizeof(char) * TAMMSG);
        sprintf(res.Resultado_u.err, "ERROR: El valor %d de numero de dominio esta fuera de rango", q->ndxdom);
    }
    else if ((q->ndxrecord < 0) || (q->ndxrecord > (numtiposrec - 1)))
    {
        res.caso = 2;
        res.Resultado_u.err = (char *)malloc(sizeof(char) * TAMMSG);
        sprintf(res.Resultado_u.err, "ERROR: El valor %d de numero de tipo de registro esta fuera de rango", q->ndxrecord);
    }
    else
    {
        // A RELLENAR
        |
        |
        |
    }
    return (&res);
}

Resultado *obtener_num_dominios_1_svc(void *q, struct svc_req *peticion)
{
    // Retorna el número total de dominios con que el servidor ha sido inicializado
    static Resultado res;

    res.caso = 1;
    res.Resultado_u.val = numdominios;

    return (&res);
}

Resultado *obtener_num_records_1_svc(void *q, struct svc_req *peticion)
{
    // Retorna el número total de tipos de registros con que el servidor ha sido inicializado
    static Resultado res;

    res.caso = 1;
    res.Resultado_u.val = numtiposrec;

    return (&res);
}

Resultado *obtener_nombre_dominio_1_svc(int *n, struct svc_req *peticion)
{
    // Retorna el nombre de dominio asociado a un índice dado

    static Resultado res;

    if ((*n < 0) || (*n > (numdominios - 1)))
    {
        res.caso = 2;
        res.Resultado_u.err = "ERROR en el numero de dominio";
    }
    else
    {
        res.Resultado_u.msg = obtener_dato_en_posicion(*n, lnomdominios);
        if (res.Resultado_u.msg == NULL)
        {
            res.caso = 2;
            res.Resultado_u.err = "ERROR en el numero de dominio";
        }
        else
        {
            res.caso = 0;
        }
    }
    return (&res);
}

Resultado *obtener_nombre_record_1_svc(int *n, struct svc_req *peticion)
{
    // Retorna el nombre de tipo de registro asociado a un índice dado
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

    return (&res);
}
