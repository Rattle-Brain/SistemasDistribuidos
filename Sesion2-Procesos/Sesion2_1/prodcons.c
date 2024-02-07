#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void main()
{
    int fds[2];
    pid_t pid_h1, pid_h2;
    FILE f;

    if (pipe(fds) < 0)
    {
        perror("Fallo al crear el pipe");
        exit(EXIT_FAILURE);
    }

    if((pid_h1 = fork()) < 0)
    {   // Fallo al crear el primer fork
        perror("Fallo al crear el primer fork");
        exit(EXIT_FAILURE + 1);
    }
    else if(pid_h1 == 0)
    {   //Retorno del fork en el 1er hijo
        dup2(fds[0], 0);
        close(fds[1]);
        execlp("sort", "sort", NULL);
    }
    else{
        if((pid_h2 = fork()) < 0)
        {   // Fallo al crear el segundo fork
            perror("Fallo al crear el segundo fork");
            exit(EXIT_FAILURE + 1);
        }
        else if(pid_h2 == 0)
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
}