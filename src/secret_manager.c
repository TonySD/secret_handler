#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../headers/utils.h"
#include "../headers/config.h"

struct IndexPart {
    long uid;
    long address;
    size_t byte_size;
};

int init_procfs() {
    FILE *fptr;
    char filename[128];
    int pid = getpid();

    // Create secrets file
    sprintf(filename, "/proc/%d/%s", pid, SECRET_FILENAME);
    fptr = fopen(filename, "w");
    check_file_open_error(fptr, "Secret file creation error");
    fclose(fptr);

    // Create indexes file
    sprintf(filename, "/proc/%d/%s", pid, INDEX_FILENAME);
    fptr = fopen(filename, "w");
    check_file_open_error(fptr, "Secret index file creation error");
    fclose(fptr);
}

void create_secret(int uid, const char* secret) {
    char* filename = get_procfs_filename(INDEX_FILENAME);

    struct IndexPart index;
    index.uid = uid;
    index.address = get_file_size(filename);
    index.byte_size = strlen(secret);
    
    FILE* fptr = fopen(filename, "a");
    check_file_open_error(fptr, "Secret indexes opening error");
    fwrite(&index, sizeof(struct IndexPart), 1, fptr);
    fclose(fptr);
    free(filename);

    filename = get_procfs_filename(SECRET_FILENAME);
    FILE* fptr = fopen(filename, "a");
    check_file_open_error(fptr, "Secret file opening error");
    fwrite(secret, strlen(secret), 1, fptr);
    fclose(fptr);
    free(filename);
}

char* read_secret(struct IndexPart secret_addr) {
    
}

struct IndexPart* get_secret_addr(int uid, int index) {
    size_t counter;
    struct IndexPart* current_index = malloc(sizeof(struct IndexPart));
    FILE* fptr = fopen(get_procfs_filename(INDEX_FILENAME), "r");
    check_file_open_error(fptr, "Secret indexes opening error");
    while (fgets((char *) current_index, sizeof(struct IndexPart), fptr)) {
        if (current_index->uid != uid) continue;
        
        counter++;
        if (counter == index) return current_index;
    }

    free(current_index);
    fclose(fptr);
    return NULL;
}

int main() {
    printf("%d", get_file_size("README.md"));
    return 0;
}