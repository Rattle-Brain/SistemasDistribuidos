#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 10

int main(int argc, char* argv[])
{
    int fds[2];
    pid_t pid_h1, pid_h2;  // Hijo1 = productor; Hijo2 = cosumidor

    if(argc < 2)
    {
        printf("Faltan argumentos. Uso:\n");
        printf("\t./prodcons <nombre_fichero>\n");
    }


    if (pipe(fds) < 0)
    {
        perror("Fallo al crear el pipe\n");
        exit(EXIT_FAILURE);
    }

    if((pid_h1 = fork()) < 0)
    {   // Fallo al crear el primer fork
        perror("Fallo al crear el primer fork\n");
        exit(EXIT_FAILURE + 1);
    }
    else if(pid_h1 == 0)
    {  // Retorno del fork en el 1er hijo
       // Open the file for reading
        int file_fd = open(argv[1], O_RDONLY);
        if (file_fd == -1) {
            perror("Error al abrir el archivo de origen\n");
            exit(EXIT_FAILURE);
        }

        if (dup2(fds[1], 1) == -1) {
            perror("Error al duplicar el descriptor de archivo de origen\n");
            close(file_fd);
            exit(EXIT_FAILURE);
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
           // perror("About to Read chunk from file...\n");
            if (write(fds[1], buffer, bytes_read) == -1) {
                perror("Error al enviar el contendido del fichero\n");
                exit(EXIT_FAILURE);
            }
            //perror("Chunk Read\n");
        }

        // Check for read error
        if (bytes_read == -1) {
            perror("Error leyendo del fichero origen\n");
            exit(EXIT_FAILURE);
        }

        close(file_fd);
    }
    else{
        if((pid_h2 = fork()) < 0)
        {   // Fallo al crear el segundo fork
            perror("Fallo al crear el segundo fork\n");
            exit(EXIT_FAILURE + 1);
        }
        else if(pid_h2 == 0)
        {   // Retorno del fork en el 1er hijo
            // Open the file for reading
            char* target_name = malloc(sizeof(argv[1] + 4));
            strcpy(target_name, argv[1]);
            strcat(target_name, "_bak");
            int file_fd = open(target_name, O_WRONLY | O_CREAT);

            if (file_fd == -1) {
                perror("Error al abrir o crear el archivo de destino\n");
                exit(EXIT_FAILURE);
            }

            if (dup2(fds[0], 0) == -1) {
                perror("Error al duplicar el descriptor de archivo de destino\n");
                close(file_fd);
                exit(EXIT_FAILURE);
            }

            char buffer[BUFFER_SIZE];
            ssize_t bytes_read;
            while ((bytes_read = read(fds[0], buffer, BUFFER_SIZE)) > 0) {
                //perror("About to Write chunk in file...\n");
                if (write(file_fd, buffer, bytes_read) == -1) {
                    perror("Error al escribir en el archivo\n");
                    exit(EXIT_FAILURE);
                }
                //perror("Chunk Written\n");
            }

            // Check for read error
            if (bytes_read == -1) {
                perror("Error leyendo de la Entrada Estandar\n");
                exit(EXIT_FAILURE);
            }

            close(file_fd);
        }
        else
        {   // Cerramos todo
            close(fds[0]);
            close(fds[1]);
            exit(EXIT_SUCCESS);
        }
    }
}