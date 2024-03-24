 #include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cola.h"

void inicializar_cola(Cola *cola, int tam_cola)
{
    register int i;

    if (tam_cola < 1)
    {
        perror("Error: El tamaño de la cola debe ser mayor o igual que 1");
        exit(1);
    }
    if (cola == NULL)
    {
        perror("Error: El puntero a la cola es NULL en inicializar_cola");
        exit(2);
    }
    // A RELLENAR

    cola->datos = malloc(tam_cola * sizeof(dato_cola *)); // Reservar memoria para el array de punteros a dato_cola
    if (cola->datos == NULL)
    {
        perror("Fallo al asignar memoria para crear la cola.\n");
        exit(EXIT_FAILURE);
    }

    // Inicializar los campos de la cola
    cola->head = 0;
    cola->tail = 0;
    cola->tam_cola = tam_cola;

    // Inicializar los mutex y semáforos
    pthread_mutex_init(&cola->mutex_head, NULL);
    pthread_mutex_init(&cola->mutex_tail, NULL);
    sem_init(&cola->num_huecos, 0, tam_cola); // El número inicial de huecos es igual al tamaño de la cola
    sem_init(&cola->num_ocupados, 0, 0);      // No hay elementos ocupados inicialmente
}

void destruir_cola(Cola *cola)
{
    // Debe liberarse la memoria apuntada por cada puntero guardado en la cola
    // y la propia memoria donde se guardan esos punteros, así como
    // destruir los semáforos y mutexes
    
    // A RELLENAR
    if (cola == NULL)
    {
        perror("Puntero a cola es NULL. No destruible.\n");
        exit(EXIT_FAILURE);
    }

    // Liberar la memoria apuntada por cada puntero guardado en la cola
    for (int i = 0; i < cola->tam_cola; i++)
    {
        free(cola->datos[i]); // Liberar la memoria de cada elemento
    }
    free(cola->datos); // Liberar la memoria del array de punteros


    // Destruir los mutex y semáforos
    pthread_mutex_destroy(&cola->mutex_head);
    pthread_mutex_destroy(&cola->mutex_tail);
    sem_destroy(&cola->num_huecos);
    sem_destroy(&cola->num_ocupados);

}

void insertar_dato_cola(Cola *cola, dato_cola *dato)
{
    // A RELLENAR
    if (cola == NULL || dato == NULL)
    {
        perror("Puntero a cola o a dato es NULL. No se puede insertar\n");
        exit(1);
    }

    // Esperar a que haya hueco en la cola
    sem_wait(&cola->num_huecos);

    // Bloquear el acceso al tail
    pthread_mutex_lock(&cola->mutex_tail);

    // Insertar el dato en la cola
    cola->datos[cola->tail] = dato;

    // Actualizar el tail
    cola->tail = (cola->tail + 1) % cola->tam_cola;

    // Desbloquear el acceso al tail
    pthread_mutex_unlock(&cola->mutex_tail);

    // Aumentar el número de elementos ocupados
    sem_post(&cola->num_ocupados);

}

dato_cola *obtener_dato_cola(Cola *cola)
{
    dato_cola *p;

    // A RELLENAR
    if (cola == NULL)
    {
        perror("Puntero a cola es NULL. No se puede obtener dato\n");
        exit(1);
    }

    // Esperar a que haya datos en la cola
    sem_wait(&cola->num_ocupados);

    // Bloquear el acceso al head
    pthread_mutex_lock(&cola->mutex_head);

    // Obtener el dato de la cola
    p = cola->datos[cola->head];

    // Actualizar el head
    cola->head = (cola->head + 1) % cola->tam_cola;

    // Desbloquear el acceso al head
    pthread_mutex_unlock(&cola->mutex_head);

    // Incrementar el número de huecos disponibles
    sem_post(&cola->num_huecos);

    return p;
}
