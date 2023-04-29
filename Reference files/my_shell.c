#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <signal.h>

void ctrlc_handler(int signum)
{
    printf("\nCtrl+C was pressed. Use 'exit' to quit.\n");
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

void handle_pipe(char **argv1, char **argv2)
{
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        /* child process */
        /* redirect stdout to the write end of the pipe */
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(fd[0]); /* close read end of the pipe */
        close(fd[1]); /* close write end of the pipe */
        execvp(argv1[0], argv1);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        /* parent process */
        pid_t pid2 = fork();
        if (pid2 == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid2 == 0)
        {
            /* child process */
            /* redirect stdin to the read end of the pipe */
            if (dup2(fd[0], STDIN_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }

            close(fd[0]); /* close read end of the pipe */
            close(fd[1]); /* close write end of the pipe */
            execvp(argv2[0], argv2);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            /* parent process */
            close(fd[0]);           /* close read end of the pipe */
            close(fd[1]);           /* close write end of the pipe */
            waitpid(pid, NULL, 0);  /* wait for the first command to finish */
            waitpid(pid2, NULL, 0); /* wait for the second command to finish */
        }
    }
}
int main()
{
    int i;
    char path[1000];
    char *argv[10];
    char command[1024];
    char *token;

    while (1)
    {
        getcwd(path, 1000);
        printf("%s$ ", path);
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        /* parse command line */
        i = 0;
        token = strtok(command, " ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;

        if (argv[0] == NULL)
        { // empty
            continue;
        }

        if (strcmp(argv[0], "cd") == 0) // cd
        {
            Handle_CD_command(argv);
            continue;
        }
        if (strcmp(argv[0], "exit") == 0) // exit
        {
            printf("Bye Bye \n");
            exit(0);
        }

        for (size_t j = 0; j < i - 1; j++)
        {
            if (strcmp(argv[j], ">") == 0)
            {
                
            }
            else if (strcmp(argv[j], ">>") == 0)
            {
                
            }
            else if (strcmp(argv[j], "|") == 0)
            {
                
            }
        }

    }

}

// int main()
// {
// 	char path[1000] = "~";
// 	int i;
// 	char *argv[10];
// 	char command[1024];
// 	char *token;
// 	int command_has_executed;

// 	chdir(getenv("HOME"));
// 	signal(SIGINT, ctrlc_handler);

// 	while (1)
// 	{
// 		command_has_executed = 0;
// 		getcwd(path, 1000);
// 		printf("%s$ ", path);
// 		fgets(command, 1024, stdin);
// 		command[strlen(command) - 1] = '\0'; // replace \n with \0

// 		/* parse command line */
// 		i = 0;
// 		token = strtok(command, " ");
// 		while (token != NULL)
// 		{
// 			argv[i] = token;
// 			token = strtok(NULL, " ");
// 			i++;
// 		}
// 		argv[i] = NULL;

// 		if (argv[0] == NULL)
// 			continue;

// 		for (size_t j = 0; j < i - 1; j++)
// 		{

// 			if (strcmp(argv[j], ">") == 0)
// 			{
// 				argv[j] = NULL;
// 				if (argv[j + 1] != NULL)
// 				{
// 					handle_input_redirection(argv, 1, argv[j + 1]);
// 				}
// 				command_has_executed = 1;
// 				continue;
// 			}
// 			else if (strcmp(argv[j], ">>") == 0)
// 			{
// 				argv[j] = NULL;
// 				if (argv[j + 1] != NULL)
// 				{
// 					handle_input_redirection(argv, 2, argv[j + 1]);
// 				}
// 				command_has_executed = 1;
// 				continue;
// 			}
// 			else if (strcmp(argv[j], "|") == 0)
// 			{
// 				argv[j] = NULL;
// 				char *argv1[j + 1];

// 				if (argv[j + 1] != NULL && argv[j + 2] != NULL)
// 				{
// 					/* handle pipe */
// 					char *argv1[j + 1];
// 					char *argv2[i - j];
// 					for (int k = 0; k < j; k++)
// 					{
// 						argv1[k] = argv[k];
// 					}
// 					argv1[j] = NULL;
// 					for (int k = j + 1; k < i; k++)
// 					{
// 						argv2[k - j - 1] = argv[k];
// 					}
// 					argv2[i - j - 1] = NULL;
// 					handle_pipe(argv1, argv2);
// 				}
// 				command_has_executed = 1;
// 				continue;
// 			}
// 		}

// 		if (command_has_executed == 0)
// 		{

// 			if (strcmp(argv[0], "cd") == 0) // cd
// 			{
// 				Handle_CD_command(argv);
// 			}
// 			else if (strcmp(argv[0], "exit") == 0) // exit
// 			{
// 				printf("Bye Bye \n");
// 				exit(0);
// 			}
// 			else
// 			{
// 				/* for commands not part of the shell command language */
// 				pid_t pid = fork();
// 				if (pid == 0)
// 				{
// 					execvp(argv[0], argv);
// 					exit(EXIT_FAILURE);
// 				}
// 				else
// 				{
// 					wait(NULL);
// 				}
// 			}
// 		}
// 	}
// }
