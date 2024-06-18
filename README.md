# Sistemas Distribuidos

<img src="https://www.codewithharry.com/img/notes/c.webp" align="right" />

¡Bienvenido al repositorio "Sistemas Distribuidos"! Este proyecto se dedica a explorar e implementar sistemas distribuidos eficientes desde cero utilizando el lenguaje de programación C.

```C
void initTCPConx() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Connected to TCP socket\n");
    close(sock);
}
```

## Descripción General

Este repositorio es una recopilación de los ejercicios realizados en clase y las entregas de PL de la asignatura de Sistemas Distribuidos impartida por la Universidad de Oviedo en la EPI a lo largo del Curso Académico 2023-2024. El repositorio ha sido privado a lo largo del año para evitar detecciones de plagio. Ahora que ya no lo necesito, puede que alguien le encuentre utilidad. No hice todo este trabajo para nada.

El código es funcional y tanto la parte Práctica de PL como ambas entregas se ejecutan perfectamente. Tienes completa libertad para clonar, copiar, modificar, jugar, romper o corregir el código aquí presente pero **estoy exento de cualquier responsabilidad por suspensos o detecciones de plagio si decides usarlo.** Al final el código es público para todos y cada uno decide como usarlo.

Mi consejo es que se tome como una forma de verificar y comparar tus resultados o tu código y no como una base de la que partir para realizar las entregas.

## Características Clave

- **Programado en C**: Todo el proyecto está escrito en el lenguaje de programación C, haciendo hincapié en el rendimiento y las interacciones de sistema de bajo nivel.

- **Desde Cero**: Aprende a construir sistemas distribuidos desde cero, obteniendo conocimientos sobre los principios fundamentales que subyacen a su funcionalidad.

- **La Eficiencia Importa**: Explora técnicas para optimizar sistemas distribuidos, asegurando que operen sin problemas y funcionen eficientemente bajo diversas condiciones.

## Empezar

Para comenzar con este proyecto, sigue estos pasos:

1. Clona el repositorio en tu máquina local:

   ```bash
   git clone https://github.com/Rattle-Brain/SisDis.git
   ```

2. Explora el código fuente, la documentación y los ejemplos proporcionados en el repositorio.

3. Sumérgete en el mundo de los sistemas distribuidos construyendo, probando y optimizando tus propias implementaciones.

### **DISCLAIMER**
- Este READMe ha sido generado automáticamente por IA

### **LICENSE**

Este proyecto está bajo la [*Unlicense License*](LICENSE). Consulta la licencia provista en el repositorio para más detalles.
