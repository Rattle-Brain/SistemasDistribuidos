 # Correccion

- **Nota práctica 1: 7,46**

- **Nota práctica 2: 6,24**

## Observaciones:

Presenta defecto de forma. La carpeta se llama Entrega1 en lugar de entrega1

### Ejecución P1 (TCP)

- Ficheros de salida incompletos. 

- Los ficheros de salida no reflejan la respuesta a la consulta o la reflejan parcialmente.

### Ejecución P1 (UDP)

- Ficheros de salida incompletos. 

- Los ficheros de salida no reflejan la respuesta a la consulta o la reflejan parcialmente.

# P1

## Cliente

Crea una estructura sockaddr_in nueva y la inicializa con los parámetros de línea de comandos
en cada hilo worker, en vez de usar la que le viene como parámetro en `p->dserv`. Dice que no le
funcionó usarlo directamente ¿quizás porque ponía una `&` delante, cuando `p->dserv` no lo necesitaba
porque ya era un puntero?

## Servidor

En el código del servidor (linea 363) a la hora de sacar al archivo de log, no se distingue
entre el caso en que haya clave y el que no lo haya. Además, en caso de que la consulta
tenga respuesta multiple no funcionará bien porque se vuelca `valorrecord` (que contiene
solo el  último encontrado), en vez de `msg` (que contiene todos concatenados por ":")

El servidor tcp no cuerra el socket tras enviar la respuesta. Debería, ya que de lo contrario
se están dejando sockets abiertos.

El servidor no maneja correctamente el caso del "fin-de-transmision" (`recibidos==0`)

La inicialización del parámetro `p->d_cliente` es incorrecto (no le asignas la dirección)

## Cola

No inicializa con NULL todos los punteros de la cola. Tampoco libera esos punteros al destruir la cola.

Están intercambiadas `head` y `tail` en la implementación, pero funciona igualmente.

# P2

## Cliente 

Algunos errores de tipo, como comparar el resultado de `fopen()` con `<0`, cuando es un puntero.

El error en caso de creación de cliente usa `fprintf()` en vez de `clnt_pcreateerror()`

Posible condición de carrera en los resultados de los hilos clientes (acceso al puntero al resultado)

No imprime a salida.dat los casos que no sean de tipo MX o NS (este es el fallo más grave)

## Servicios

Un `if` (linea 199) mira el valor del  record leido en vez del de record consultado.

En la implementación de algunos de los servicios para estadis, accedes a `numdomin` cuando debería
ser `numtiposrec`. Como consecuencia
