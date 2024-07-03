#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "config.h"

extern inline void check_file_open_error(FILE* status, const char* error_msg) {
    if (status == NULL) {
        perror(error_msg);
        exit(EXIT_FAILURE);
    }
}

extern inline char* get_procfs_filename(char* filename) {
    char* buffer = (char*) malloc(strlen(filename) + 10);
    sprintf(buffer, FILENAME_FORMAT, getpid(), filename);
    return buffer;
}

size_t get_file_size(const char* filename) {
    size_t size;
    
    FILE* fptr = fopen(filename, "r");
    check_file_open_error(fptr, "Get file size error");
    fseek(fptr, 0, SEEK_END); 
    size = ftell(fptr); 
    fclose(fptr);

    return size;
}