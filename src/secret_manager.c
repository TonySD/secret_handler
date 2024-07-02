#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../headers/utils.h"
#include "../headers/config.h"

struct IndexPart {
    int32_t uid;
    int64_t address;
    size_t size;
};

int init_procfs() {
    FILE *fptr;
    char* filename;
    int pid = getpid();

    // Create secrets file
    filename = get_procfs_filename(SECRET_FILENAME);
    fptr = fopen(filename, "w");
    check_file_open_error(fptr, "Secret file creation error");
    fclose(fptr);

    // Create indexes file
    filename = get_procfs_filename(INDEX_FILENAME);
    fptr = fopen(filename, "w");
    check_file_open_error(fptr, "Secret index file creation error");
    fclose(fptr);

    free(filename);
}

struct IndexPart* get_secret_addr(int32_t uid, int index) {
    size_t counter = 0;
    struct IndexPart* current_index = malloc(sizeof(struct IndexPart));
    FILE* fptr = fopen(get_procfs_filename(INDEX_FILENAME), "r");
    check_file_open_error(fptr, "Secret indexes opening error");
    while (fread(current_index, sizeof(struct IndexPart), 1, fptr)) {
        if (current_index->uid != uid) continue;

        if (counter == index) return current_index;
        counter++;
    }

    free(current_index);
    fclose(fptr);
    return NULL;
}

void create_secret(int32_t uid, const char* secret) {
    char* filename = get_procfs_filename(SECRET_FILENAME);

    struct IndexPart index;
    index.uid = uid;
    index.address = get_file_size(filename);
    index.size = strlen(secret);

    free(filename);

    filename = get_procfs_filename(INDEX_FILENAME);
    FILE* fptr = fopen(filename, "a");
    check_file_open_error(fptr, "Secret indexes opening error");
    fwrite(&index, sizeof(struct IndexPart), 1, fptr);
    fclose(fptr);
    free(filename);

    filename = get_procfs_filename(SECRET_FILENAME);
    fptr = fopen(filename, "a");
    check_file_open_error(fptr, "Secret file opening error");
    fwrite(secret, strlen(secret), 1, fptr);
    fclose(fptr);
    free(filename);
}

char* read_secret(int32_t uid, int index) {
    struct IndexPart* secret_info = get_secret_addr(uid, index);
    if (secret_info == 0) return NULL;

    char* filename = get_procfs_filename(SECRET_FILENAME);
    char* buffer = malloc(secret_info->size + 1);

    FILE* fptr = fopen(filename, "r");
    check_file_open_error(fptr, "Secret indexes opening error");
    fseek(fptr, secret_info->address, SEEK_SET);
    fread(buffer, 1, secret_info->size, fptr);
    fclose(fptr);
    free(filename);

    return buffer;
}

