#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// void pipeline(char *cmd1[], char *cmd2[], char *cmd3[]) {
//     int pipefd[2];
//     pid_t pid1, pid2, pid3;

//     // Create pipes
//     if (pipe(pipefd) == -1) {
//         perror("pipe");
//         exit(EXIT_FAILURE);
//     }

//     // Fork first child (cmd1)
//     pid1 = fork();
//     if (pid1 == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }
//     if (pid1 == 0) {
//         // Redirect output to pipe
//         dup2(pipefd[1], STDOUT_FILENO);
//         // Close unused read end of pipe
//         close(pipefd[0]);
//         // Execute cmd1
//         execvp(cmd1[0], cmd1);
//         perror("execvp cmd1");
//         exit(EXIT_FAILURE);
//     }

//     // Fork second child (cmd2)
//     pid2 = fork();
//     if (pid2 == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }
//     if (pid2 == 0) {
//         // Redirect input from pipe
//         dup2(pipefd[0], STDIN_FILENO);
//         // Redirect output to stdout (for 2 command pipeline)
//         if (cmd3 == NULL) {
//             dup2(STDOUT_FILENO, STDOUT_FILENO);
//         } else { // Redirect output to pipe (for 3 command pipeline)
//             dup2(pipefd[1], STDOUT_FILENO);
//             // Close unused write end of pipe
//             close(pipefd[0]);
//         }
//         // Execute cmd2
//         execvp(cmd2[0], cmd2);
//         perror("execvp cmd2");
//         exit(EXIT_FAILURE);
//     }

//     if (cmd3 != NULL) { // Three command pipeline
//         // Fork third child (cmd3)
//         pid3 = fork();
//         if (pid3 == -1) {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }
//         if (pid3 == 0) {
//             // Redirect input from pipe
//             dup2(pipefd[0], STDIN_FILENO);
//             // Close unused write end of pipe
//             close(pipefd[1]);
//             // Execute cmd3
//             execvp(cmd3[0], cmd3);
//             perror("execvp cmd3");
//             exit(EXIT_FAILURE);
//         }
//     }

//     // Parent process closes unused ends of pipe and waits for children to finish
//     close(pipefd[0]);
//     close(pipefd[1]);
//     waitpid(pid1, NULL, 0);
//     waitpid(pid2, NULL, 0);
//     if (cmd3 != NULL) { // Three command pipeline
//         waitpid(pid3, NULL, 0);
//     }
// }

// int main() {
//     char *cmd1[] = {"ls", NULL};
//     char *cmd2[] = {"grep", "t", NULL};
//     char *cmd3[] = {"grep", "te", NULL};
    
//     // Two command pipeline
//     // pipeline(cmd1, cmd2, NULL);
    
//     // Three command pipeline
//     pipeline(cmd1, cmd2, cmd3);
    
//     return 0;
// }


// void pipeline(char *cmd1[], char *cmd2[], char *cmd3[]) {
//     int pipefd[2], pipefd2[2];
//     pid_t pid1, pid2, pid3;

//     // Create pipes
//     if (pipe(pipefd) == -1 || pipe(pipefd2) == -1) {
//         perror("pipe");
//         exit(EXIT_FAILURE);
//     }

//     // Fork first child (cmd1)
//     pid1 = fork();
//     if (pid1 == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }
//     if (pid1 == 0) {
//         // Redirect output to first pipe
//         dup2(pipefd[1], STDOUT_FILENO);
//         // Close unused read and write ends of both pipes
//         close(pipefd[0]);
//         close(pipefd2[0]);
//         close(pipefd2[1]);
//         // Execute cmd1
//         execvp(cmd1[0], cmd1);
//         perror("execvp cmd1");
//         exit(EXIT_FAILURE);
//     }

//     // Fork second child (cmd2)
//     pid2 = fork();
//     if (pid2 == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }
//     if (pid2 == 0) {
//         // Redirect input from first pipe
//         dup2(pipefd[0], STDIN_FILENO);
//         // Redirect output to second pipe
//         dup2(pipefd2[1], STDOUT_FILENO);
//         // Close unused read and write ends of both pipes
//         close(pipefd[1]);
//         close(pipefd2[0]);
//         // Execute cmd2
//         execvp(cmd2[0], cmd2);
//         perror("execvp cmd2");
//         exit(EXIT_FAILURE);
//     }

//     if (cmd3 != NULL) { // Three command pipeline
//         // Fork third child (cmd3)
//         pid3 = fork();
//         if (pid3 == -1) {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }
//         if (pid3 == 0) {
//             // Redirect input from second pipe
//             dup2(pipefd2[0], STDIN_FILENO);
//             // Close unused write end of second pipe
//             close(pipefd2[1]);
//             // Execute cmd3
//             execvp(cmd3[0], cmd3);
//             perror("execvp cmd3");
//             exit(EXIT_FAILURE);
//         }
//     }

//     // Parent process closes all pipe ends and waits for children to finish
//     close(pipefd[0]);
//     close(pipefd[1]);
//     close(pipefd2[0]);
//     close(pipefd2[1]);
//     waitpid(pid1, NULL, 0);
//     waitpid(pid2, NULL, 0);
//     if (cmd3 != NULL) { // Three command pipeline
//         waitpid(pid3, NULL, 0);
//     }
// }

// int main() {
//     char *cmd1[] = {"ls", NULL};
//     char *cmd2[] = {"grep", "t", NULL};
//     char *cmd3[] = {"grep", "te", NULL};

//     pipeline(cmd1, cmd2, cmd3);
    
//     return 0;
// }


