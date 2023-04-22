#include "stdio.h"
#include <string.h>

int main()
{
    char command[1024];
    printf("enter: ");
    fgets(command, 1024, stdin);
    command[strlen(command) - 1] = '\0'; // replace \n with \0
    printf("OK %s \n",command);


}