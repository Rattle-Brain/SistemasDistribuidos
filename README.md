# Sistemas Distribuidos

<img src="https://www.codewithharry.com/img/notes/c.webp" align="right" />

¡Bienvenido al repositorio **"Sistemas Distribuidos"**! Este proyecto explora la implementación de tecnologías de sistemas distribuidos desde cero, utilizando el lenguaje de programación C.

```C
#define PORT 9090

void initTCPConx() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Crear socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Conectado al socket TCP\n");
    close(sock);
}
```

## Descripción General

Este repositorio reúne ejercicios y proyectos de la asignatura de **Sistemas Distribuidos** impartida en la Universidad de Oviedo, EPI, durante el Curso Académico 2023-2024. Su contenido ha estado privado para evitar problemas de plagio, pero ahora se hace público para que pueda servir como referencia o recurso educativo.

El código proporcionado aquí es funcional y abarca tanto la parte práctica de las sesiones de PL como las entregas de proyectos. **Se proporciona sin ninguna garantía de éxito académico ni responsabilidad por detecciones de plagio.** Se recomienda usarlo como una referencia para verificar y comparar resultados.

## Características Clave

- **Lenguaje C**: Todo el proyecto está escrito en C, enfocándose en el rendimiento y las interacciones de bajo nivel con el sistema.
- **Construcción desde Cero**: Aprende a implementar sistemas distribuidos desde los fundamentos básicos.
- **Optimización y Rendimiento**: Explora técnicas para asegurar la eficiencia y robustez de los sistemas distribuidos bajo diversas condiciones.

## Contenidos del Proyecto

El repositorio incluye:

- **Programación de Sockets**: Implementaciones para la gestión de peticiones en servidores TCP y UDP.
- **Modelos P2P**: Ejemplos y prácticas de sistemas punto a punto.
- **Colas de Mensajes**: Ejercicios con RabbitMQ para gestionar la comunicación en sistemas distribuidos.
- **Algoritmos de Consenso**: Implementación y análisis de algoritmos para alcanzar consenso en sistemas distribuidos.
- **Entregables**: Los dos entregables completos de la parte práctica de la asignatura.
- **Correcciones**: Notas de las entrgas y correcciones proporcionadas por el profesorado.

## Empezar

Para comenzar con este proyecto:

1. Clona el repositorio en tu máquina local:
   ```bash
   git clone https://github.com/Rattle-Brain/SistemasDistribuidos.git
   ```

2. Explora el código fuente, la documentación y los ejemplos proporcionados en el repositorio.

3. Sumérgete en el desarrollo y optimización de sistemas distribuidos con tus propias implementaciones.

## Contribuciones

Las contribuciones a este proyecto son bienvenidas. Si deseas proponer nuevas características o correcciones, sigue estos pasos para hacer un Pull Request:

1. **Forkea** el repositorio.
2. **Clona** tu fork en tu máquina local.
   ```bash
   git clone https://github.com/tu-usuario/SistemasDistribuidos.git
   ```
3. Crea una **nueva rama** para tu trabajo.
   ```bash
   git checkout -b feature/mi-nueva-caracteristica
   ```
4. Realiza tus **cambios** y **commits**.
   ```bash
   git commit -am 'Añadida nueva característica X'
   ```
5. **Empuja** tus cambios a tu fork.
   ```bash
   git push origin feature/mi-nueva-caracteristica
   ```
6. Abre un **Pull Request** desde tu fork hacia este repositorio.

## Disclaimers

- Este README ha sido generado automáticamente por una IA.
- El uso del código presente en este repositorio es bajo tu propia responsabilidad.
- No me responsabilizo de suspensos o detecciones de plagio derivadas del uso de este código.

## License

Este proyecto está bajo la [*Unlicense License*](LICENSE). Consulta la licencia provista en el repositorio para más detalles.
