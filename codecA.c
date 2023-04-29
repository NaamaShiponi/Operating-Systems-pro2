#include "codecA.h"
#include <stdio.h>

void encode_codeA(char* massage){
    int i = 0;
    char ch;
    while ((ch = massage[i]) != '\0')
    {
        if (isupper(ch))
        {
            massage[i] = tolower(ch);
        }
        else if (islower(ch))
        {
            massage[i] = toupper(ch);
        }
        ++i;
    }
    printf("%s\n",massage);
}

void decode_codeA(char* massage){
    encode_codeA(massage);
}