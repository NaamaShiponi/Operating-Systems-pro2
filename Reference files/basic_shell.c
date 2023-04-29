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

void handle_pipe_2(char *cmd1[], char *cmd3[]) {
     int pipefd[2];
     pid_t pid1, pid2;

     // Create pipes
     if (pipe(pipefd) == -1) {
         perror("pipe");
         exit(EXIT_FAILURE);
     }

     // Fork first child (cmd1)
     pid1 = fork();
     if (pid1 == -1) {
         perror("fork");
         exit(EXIT_FAILURE);
     }
     if (pid1 == 0) {
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
     if (pid2 == -1) {
         perror("fork");
         exit(EXIT_FAILURE);
     }
     if (pid2 == 0) {
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



void handle_pipe_3(char *cmd1[], char *cmd2[], char *cmd3[]) {
    int pipefd[2];
    pid_t pid1, pid2, pid3;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        /* Child 1: execute cmd1 and write to pipe */
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execvp(cmd1[0], cmd1);
        perror("exec");
        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        /* Child 2: read from pipe, execute cmd2, and write to pipe */
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        int pipefd2[2];
        if (pipe(pipefd2) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        pid3 = fork();
        if (pid3 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid3 == 0) {
            /* Child 3: read from pipe, execute cmd3, and write to stdout */
            close(pipefd2[1]);
            dup2(pipefd2[0], STDIN_FILENO);
            close(pipefd2[0]);
            execvp(cmd3[0], cmd3);
            perror("exec");
            exit(EXIT_FAILURE);
        } else {
            /* Child 2: read from pipe, execute cmd2, and write to pipe */
            close(pipefd2[0]);
            dup2(pipefd2[1], STDOUT_FILENO);
            close(pipefd2[1]);
            execvp(cmd2[0], cmd2);
            perror("exec");
            exit(EXIT_FAILURE);
        }
    }

    /* Parent: wait for child processes to terminate */
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
}


int main()
{
	char path[1000] = "~";
	int i;
	char *argv[10];
	char command[1024];
	char *token;
	int command_has_executed;

	chdir(getenv("HOME"));
	signal(SIGINT, ctrlc_handler);

	while (1)
	{
		command_has_executed = 0;
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
			continue;

		for (size_t j = 0; j < i - 1; j++)
		{
			
			if (strcmp(argv[j], ">") == 0)
			{
				argv[j] = NULL;
				if (argv[j + 1] != NULL)
				{
					handle_input_redirection(argv, 1, argv[j + 1]);
				}
				command_has_executed = 1;
				continue;
			}
			else if (strcmp(argv[j], ">>") == 0)
			{
				argv[j] = NULL;
				if (argv[j + 1] != NULL)
				{
					handle_input_redirection(argv, 2, argv[j + 1]);
				}
				command_has_executed = 1;
				continue;
			}
			else if (strcmp(argv[j], "|") == 0)
			{
				argv[j] = NULL;
				char *argv1[j + 1];
				if (argv[j + 1] != NULL)
				{
					/* handle pipe */
					char *argv1[j + 1];
					char *argv2[i - j];
					char *argv3[i - j];
					int twoCommands=0;
					// printf("\nargv1: ");

					for (int k = 0; k < j; k++)
					{
						printf(" argv1 [%d]%s ,\n",(k),argv[k]);
						argv1[k] = argv[k];
					}
					// printf("\nargv2: ");

					argv1[j] = NULL;
					for (int k = j + 1; k < i ; k++)
					{
						if (strcmp(argv[k], "|") == 0){
							twoCommands=1;
							argv[k]= NULL;
							printf("argv2: [%ld] NULL,\n",(k - j - 1));

							argv2[k - j - 1] =  NULL;

						}else{
							if(twoCommands ==0){
								printf("argv2: [%ld]%s ,\n",(k - j - 1),argv[k]);
								argv2[k - j - 1] = argv[k];
							}else{
								printf("argv3: [%ld]%s ,\n",(k - j - 1-(k - j - 1)),argv[k]);

								argv3[k - j - 1-(k - j - 1)] = argv[k];
							}
						}
					}
					if(twoCommands ==0){
						printf("argv2: [%ld] NULL,\n",(i - j - 1));
						argv2[i - j - 1] = NULL;
						handle_pipe_2(argv1, argv2);
					}else{
						printf("argv3: [%ld] NULL,\n",(i - j - 1));
						argv3[i - j - 1] = NULL;
						handle_pipe_3(argv1, argv2,argv3);
					}
				}
				command_has_executed = 1;
				continue;
			}
		}

		if (command_has_executed == 0)
		{

			if (strcmp(argv[0], "cd") == 0) // cd
			{
				Handle_CD_command(argv);
			}
			else if (strcmp(argv[0], "exit") == 0) // exit
			{
				printf("Bye Bye \n");
				exit(0);
			}
			else
			{
				/* for commands not part of the shell command language */
				pid_t pid = fork();
				if (pid == 0)
				{
					execvp(argv[0], argv);
					exit(EXIT_FAILURE);
				}
				else
				{
					wait(NULL);
				}
			}
		}
	}
}
