#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char* SECRET_FILENAME = "secrets";
char* INDEX_FILENAME = "indexes";

int init_procfs() {
    FILE *fptr;
    char* filename[128];
    int pid = getpid();

    // Create secrets file
    sprintf(filename, "/proc/%d/%s", pid, SECRET_FILENAME);
    fptr = fopen(filename, "w");
    fclose(fptr);

    // Create indexes file
    sprintf(filename, "/proc/%d/%s", pid, INDEX_FILENAME);
    fptr = fopen(filename, "w");
    fclose(fptr);
}

