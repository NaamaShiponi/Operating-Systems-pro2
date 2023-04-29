#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codecA.h"
#include "codecB.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: decode <codec> <message>\n");
        return 1;
    }

    char* codec = argv[1];
    char* message = argv[2];

    if (strcmp(codec, "codecA") == 0) {
        decode_codeA(message);
    } else if (strcmp(codec, "codecB") == 0) {
        decode_codeB(message);
    } else {
        printf("Invalid codec: %s\n", codec);
        return 1;
    }

    return 0;
}
