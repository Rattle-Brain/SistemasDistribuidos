# Corrección

- **Nota P1: 9,8**

- **Nota P2: 9,31**

## Observaciones:

## P2

- En el mensaje de que el filósofo está pensando, no se indica qué palillos ha soltado

- No usa strtok ni strtok_r porque en su lugar utiliza `sscanf()`

- No obstante, la inicialización de los punteros donde va a recibir el resultado de sscanf no es correcta porque reserva sitio para un solo char (no tiene en cuenta el terminador). Es curioso que no produzca segfault en tiempo de ejecución debido a esto.

### Fase 4

Se razona que no puede ocurrir deadlock. Se explica correctamente que el único caso posible sería cuando todos los filósofos empiecen a comer a la vez. 
Se intenta justificar que aún en ese caso no se produce deadlock en el maestro debido a que las peticiones van por socket y se secuencian en el socket,
por lo que aún si dos filósofos se sientan a la vez, las peticiones de estos no llegan a la vez a los hilos que van a actuar sobre los cerrojos.

Esta explicación es correcta, pero no garantiza la ausencia de deadlocks. No es necesario que ocurra todo exactamente a la vez, basta con que el entrelazado de la ejecución 
de los hilos sea desafortunada. Por ejemplo, podrían llegar (secuencialmente) las cinco peticiones de los cinco filósofos, y el hilo maestro lanzar (secuencialmente) a los cinco hilos de atención.
Después, el planificador de linux ejecuta un poquito de cada hilo, y pudiera suceder que se ejecute de tal forma que todos los hilos toman el primer cerrojo, y luego son desalojados por el planificador
para dar el turno a otro hilo. Cuando sea el turno de reanudarse, se encontrará con el otro cerrojo bloqueado y esto puede suceder en todos los hilos.

La situación es altamente improbable y por ello por mucho que se intente no se va a observar. Pero el programa no está libre de deadlocks.

No obstante el haber detectado la secuenciación de las llegadas ha sido un razonamiento ingenioso.

