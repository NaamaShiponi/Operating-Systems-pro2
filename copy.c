#include "copy.h"

int copy_file(const char* src_filename, const char* dest_filename, int force);

int main(int argc, char *argv[]) {
    int verbose = 0;
    int force = 0;
    int status = SUCCESS;

    if ((argc == 4) || (argc == 5)) {
        for(int i = 3; i < argc; i++) {
            if (strcmp(argv[i], "-v") == 0) {
                verbose = 1;
            } else if (strcmp(argv[i], "-f") == 0) {
                force = 1;
            } else {
                printf("Error: Invalid flag %s\n", argv[i]);
                return FAILURE;
            }
        }
    
    // Parse command-line arguments
    } else if (argc != 3) {
        printf("Error: Invalid number of arguments\n");
        printf("Usage: %s <src_filename> <dest_filename> [-v] [-f]\n", argv[0]);
        return FAILURE;
    }

    status = copy_file(argv[1], argv[2], force);

    if (verbose) {     
        if (status == FAILURE) {
            printf("Failed\n");
        }
        else{
            printf("Success\n");
        }
    }

    return status;
}

int copy_file(const char* src_file_name, const char* dest_file_name, int force) {
    char buf[4096];
    size_t n;

    // Open source file for reading
    FILE *src_file = fopen(src_file_name, "r");
    if (src_file == NULL) {
        printf("Error: Failed to open source file\n");
        return FAILURE;
    }

    // Check if destination file exists
    FILE *dest_file = fopen(dest_file_name, "r");
    if (dest_file != NULL) {
        fclose(dest_file);
        if (force) {
            if (remove(dest_file_name) != 0) {
                printf("Error: Failed to remove existing file\n");
                fclose(src_file);
                return FAILURE;
            }
        } else {
            printf("Target file already exists\n");
            fclose(src_file);
            return FAILURE;
        }
    }

    // Open destination file for writing
    dest_file = fopen(dest_file_name, "w");
    if (dest_file == NULL) {
        printf("Error: Failed to create destination file\n");
        fclose(src_file);
        return FAILURE;
    }

    // Copy data from source file to destination file
    do {
        n = fread(buf, 1, sizeof(buf), src_file);
        if (fwrite(buf, 1, n, dest_file) != n) {
            printf("Error: Failed to write to destination file\n");
            fclose(src_file);
            fclose(dest_file);
            remove(dest_file_name);
            return FAILURE;
        }
    }while (n > 0);

    // Close files
    fclose(src_file);
    fclose(dest_file);

    return SUCCESS;
}
