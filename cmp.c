#include "cmp.h"


int cmp(char* file1, char* file2, int ignore_case);

int main(int argc, char** argv) {
    int verbose = 0;
    int ignore_case = 0;
    
    if ((argc == 4) || (argc == 5)) {
        for(int i = 3; i < argc; i++) {
            if (strcmp(argv[i], "-v") == 0) {
                verbose = 1;
            } else if (strcmp(argv[i], "-i") == 0) {
                ignore_case = 1;
            } else {
                printf("Error: Invalid flag %s\n", argv[i]);
                return 1;
            }
        }

        // Parse command-line arguments
    } else if (argc != 3) {
        printf("Error: Invalid number of arguments\n");
        printf("Usage: %s <file1> <file2> [-v] [-i]\n", argv[0]);
        return 1;
    }

    int result = cmp(argv[1], argv[2], ignore_case);

    if (verbose) {
        if (result == 1) {
            printf("distinct\n");
        } else {
            printf("equal\n");
        }
    }

    return result;
}

int cmp(char* file1, char* file2, int ignore_case) {
    FILE* fp1 = fopen(file1, "r");
    FILE* fp2 = fopen(file2, "r");
    if (!fp1 || !fp2) {
        printf("Error: Could not open files\n");
        return -1;
    }
    int result = 0;
    int c1, c2;

    do {
        c1 = fgetc(fp1);
        c2 = fgetc(fp2);

        if (ignore_case) {
            c1 = tolower(c1);
            c2 = tolower(c2);
        }
        if (c1 != c2) {
            result = 1;
            break;
        }
    }while ((c1 != EOF) && (c2 != EOF));

    if (c1 != c2) {
        result = 1;
    }

    fclose(fp1);
    fclose(fp2);
    return result;
}
