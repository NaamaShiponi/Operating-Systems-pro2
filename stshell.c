#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "errno.h"
#include "unistd.h"
#include <signal.h>

#define MAX_ARGS 10


void enterValToargv(char *argv1[MAX_ARGS], char *argv2[MAX_ARGS], char *argv3[MAX_ARGS], char *argv4[MAX_ARGS], char *token, int argvNum, int i)
{
    if (argvNum == 1)
    {
        argv1[i] = token;
    }
    else if (argvNum == 2)
    {
        argv2[i] = token;
    }else if (argvNum == 3)
    {
        argv3[i] = token;
    }
    else
    {
        argv4[i] = token;
    }
}

void Handle_CD_command(char **argv)
{
    if (argv[1] == NULL)
    {
        chdir(getenv("HOME"));
    }
    else
    {
        if (chdir(argv[1]) == -1)
        {
            printf("cd: %s: %s\n", argv[1], strerror(errno));
        }
    }
}

void handle_input_redirection(char **argv, int flag, char *nameFile)
{
    int fd;
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (flag == 1) // overwrite mode
        {
            fd = open(nameFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        else if (flag == 2) // append mode
        {
            fd = open(nameFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }

        if (fd == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }

        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(fd);
        execvp(argv[0], argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL);
    }
}

void handle_pipe_2(char *cmd1[], char *cmd3[])
{
    int pipefd[2];
    pid_t pid1, pid2;

    // Create pipes
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork first child (cmd1)
    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid1 == 0)
    {
        // Redirect output to pipe
        dup2(pipefd[1], STDOUT_FILENO);
        // Close unused read end of pipe
        close(pipefd[0]);
        // Execute cmd1
        execvp(cmd1[0], cmd1);
        perror("execvp cmd1");
        exit(EXIT_FAILURE);
    }

    // Fork second child (cmd3)
    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid2 == 0)
    {
        // Redirect input from pipe
        dup2(pipefd[0], STDIN_FILENO);
        // Close unused write end of pipe
        close(pipefd[1]);
        // Execute cmd3
        execvp(cmd3[0], cmd3);
        perror("execvp cmd3");
        exit(EXIT_FAILURE);
    }

    // Parent process closes unused ends of pipe and waits for children to finish
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}


void handle_pipe_3(char *cmd1[], char *cmd2[], char *cmd3[])
{
    int pipefd1[2], pipefd2[2];
    pid_t pid1, pid2, pid3;

    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid1 == 0)
    {
        /* Child 1: execute cmd1 and write to pipe1 */
        close(pipefd1[0]);
        dup2(pipefd1[1], STDOUT_FILENO);
        close(pipefd1[1]);
        close(pipefd2[0]);
        close(pipefd2[1]);
        execvp(cmd1[0], cmd1);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid2 == 0)
    {
        /* Child 2: read from pipe1, execute cmd2, and write to pipe2 */
        close(pipefd1[1]);
        dup2(pipefd1[0], STDIN_FILENO);
        close(pipefd1[0]);
        close(pipefd2[0]);
        dup2(pipefd2[1], STDOUT_FILENO);
        close(pipefd2[1]);
        execvp(cmd2[0], cmd2);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    pid3 = fork();
    if (pid3 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid3 == 0)
    {
        /* Child 3: read from pipe2 and execute cmd3 */
        close(pipefd1[0]);
        close(pipefd1[1]);
        close(pipefd2[1]);
        dup2(pipefd2[0], STDIN_FILENO);
        close(pipefd2[0]);
        execvp(cmd3[0], cmd3);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    /* Parent process closes unused pipe ends and waits for all child processes */
    close(pipefd1[0]);
    close(pipefd1[1]);
    close(pipefd2[0]);
    close(pipefd2[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
}



void initializeVar(char *argv1[], char *argv2[], char *argv3[], char *argv4[],char whatSign[]){
    whatSign[0] = '\0';
    for (int j=0; j<MAX_ARGS; j++){
        argv1[j]=NULL;
        argv2[j]=NULL;
        argv3[j]=NULL;
        argv4[j]=NULL;

    }
}
int main()
{
    char path[1000] = "~";
    char *token;
    char command[1024];
    char *argv1[MAX_ARGS];
    char *argv2[MAX_ARGS];
    char *argv3[MAX_ARGS];
    char *argv4[MAX_ARGS];
    char whatSign[MAX_ARGS];
    signal(SIGINT, SIG_IGN);

    while (1)
    {
        int argvNum = 1;
        int i = 0;

        initializeVar(argv1, argv2, argv3,argv4,whatSign);
        getcwd(path, 1000);
        printf("%s$ ", path);
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        token = strtok(command, " ");
        while (token != NULL)
        {
            if (strcmp(token, ">") == 0 || (strcmp(token, ">>") == 0) || (strcmp(token, "|") == 0))
            {
                enterValToargv(argv1, argv2, argv3,argv4, NULL, argvNum, i);
                strcat(whatSign, token);
                i = 0;
                argvNum++;
            }
            else
            {
                enterValToargv(argv1, argv2, argv3,argv4, token, argvNum, i);
                i++;
            }
            token = strtok(NULL, " ");
        }
        enterValToargv(argv1, argv2, argv3,argv4, NULL, argvNum, i);



        if (strcmp(whatSign, "|") == 0)
        {
            handle_pipe_2(argv1, argv2);

        }
        else if (strcmp(whatSign, "||") == 0)
        {
            handle_pipe_3(argv1, argv2, argv3);
        
        }
        else if (strcmp(whatSign, ">") == 0)
        {
            handle_input_redirection(argv1, 1, argv2[0]);

        }
        else if (strcmp(whatSign, ">>") == 0)
        {
            handle_input_redirection(argv1, 2, argv2[0]);

        }
        else if ((strcmp(whatSign, "|>") == 0) || (strcmp(whatSign, "|>>") == 0))
        {
            int fd1;
            pid_t pid1 = fork();
            if (pid1 == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            else if (pid1 == 0)
            {

                if (strcmp(whatSign, "|>") == 0) // overwrite mode
                {
                    fd1 = open(argv3[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                else  //append mode
                {
                    fd1 = open(argv3[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
                }
                if (fd1 == -1)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }

                if (dup2(fd1, STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }

                close(fd1);
                handle_pipe_2(argv1, argv2);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
            }
        }
        else if ((strcmp(whatSign, "||>") == 0) || (strcmp(whatSign, "||>>") == 0))
        {
            int fd1;
            pid_t pid1 = fork();
            if (pid1 == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            else if (pid1 == 0)
            {

                if (strcmp(whatSign, "||>") == 0) // overwrite mode
                {
                    fd1 = open(argv4[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                else  //append mode
                {
                    fd1 = open(argv4[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
                }
                if (fd1 == -1)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }

                if (dup2(fd1, STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }

                close(fd1);
                handle_pipe_3(argv1, argv2,argv3);
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
            }


        }
        else if (strcmp(whatSign, ">|") == 0)
        {
            handle_input_redirection(argv1, 1, argv2[0]);
            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(argv3[0], argv3);
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
            }
        }
        else if (strcmp(whatSign, ">>|") == 0)
        {
            handle_input_redirection(argv1, 2, argv2[0]);
            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(argv3[0], argv3);
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
            }

        }
        else if (strcmp(whatSign, ">||") == 0)
        {
            handle_input_redirection(argv1, 1, argv2[0]);
            handle_pipe_2(argv3, argv4);

        }
        else if (strcmp(whatSign, ">>||") == 0)
        {
            handle_input_redirection(argv1, 2, argv2[0]);
            handle_pipe_2(argv3, argv4);

        }
        else if (strcmp(argv1[0], "cd") == 0) // cd
        {
            Handle_CD_command(argv1);
        }
        else if (strcmp(argv1[0], "exit") == 0) // exit
        {
            printf("Bye Bye \n");
            exit(0);
        }
        else if (strcmp(whatSign, "") == 0)
        {
            /* for commands not part of the shell command language */
            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(argv1[0], argv1);
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
            }
        }else{
            printf("Sorry in this version I don't know how to execute this command \nMaybe they will teach me in the next version :)\n");
        }


    }

    return 0;
}
