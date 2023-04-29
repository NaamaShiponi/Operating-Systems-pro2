#include "codecB.h"
#include <stdio.h>

void encode_codeB(char* massage){
    int i=0;
    char ch;
    while ((ch = massage[i]) != '\0')
    {
        massage[i] = (char)((int)ch + 3);
        ++i;
    }
    printf("%s\n",massage);

}

void decode_codeB(char* massage){
    int i=0;
    char ch;
    while ((ch = massage[i]) != '\0')
    {
        massage[i] = (char)((int)ch - 3);
        ++i;
    }
    printf("%s\n",massage);

}