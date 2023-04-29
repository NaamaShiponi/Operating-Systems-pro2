#ifndef FILE_COPY_H
#define FILE_COPY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SUCCESS 0
#define FAILURE 1

int copy_file(const char* src_filename, const char* dest_filename, int force);

#endif /* FILE_COPY_H */
