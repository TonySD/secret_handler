#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

extern inline void check_file_open_error(FILE* status, const char* error_msg) {
    if (status == NULL) {
        perror(error_msg);
        exit(EXIT_FAILURE);
    }
}

extern inline char* get_procfs_filename(char* filename) {
    char buffer[256];
    sprintf(buffer, "/proc/%d/%s", getpid(), filename);
    return buffer;
}