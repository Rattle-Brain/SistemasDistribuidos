#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main ()
{
    int fds[2];
    pid_t pid;

    if(pipe(fds) < 0)
    {   // Comprobar que se crea el pipe correctamente
        perror("Fallo al crear el pipe\n");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) < 0)
    {   // Fallo al crear el primer fork
        perror("Fallo al crear el primer fork");
        exit(EXIT_FAILURE + 1);
    }
    else if(pid == 0)
    {   //Retorno del fork en el 1er hijo
        dup2(fds[0], 0);
        close(fds[1]);
        execlp("sort", "sort", NULL);
    }
    else{
        if((pid = fork()) < 0)
        {   // Fallo al crear el segundo fork
            perror("Fallo al crear el segundo fork");
            exit(EXIT_FAILURE + 1);
        }
        else if(pid == 0)
        {   // Retorno del fork en el segundo hijo
            dup2(fds[1], 1);
            close(fds[0]);
            execlp("who", "who", NULL);
        }
        else
        {   // Cerramos todo
            close(fds[0]);
            close(fds[1]);
            wait(0);
            wait(0);
        }
    }
    return 0;
}