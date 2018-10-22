#include <sys/resource.h>
#include <sys/time.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 120
#define DEFAULT_STRING_SIZE 512
#define CHUNK_SIZE 64

double get_time() {
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec*1e-6;
}

void read_lines(FILE* file, ssize_t (*getline_func)(char**, size_t*, FILE*)) {
    size_t len = 0;
    char* line = NULL;
    ssize_t read = getline_func(&line, &len, file);
    while (line) {
        free(line);
        line = NULL;
        if (read == -1) {
            return;
        }
        read = getline_func(&line, &len, file);
    }
}

char* custom_getline(FILE* f) {
    size_t allocated_size = DEFAULT_STRING_SIZE;
    char* str = (char*) malloc(allocated_size * sizeof(char));
    if (str == NULL) {
        return NULL;
    }

    char chunk[CHUNK_SIZE];
    chunk[CHUNK_SIZE - 1] = 0xff;
    size_t readed_bytes = 0;
    while (fgets(chunk, CHUNK_SIZE, f) != NULL) {
        if (readed_bytes + CHUNK_SIZE - 1 >= allocated_size) {
            char* tmp = str;
            allocated_size *= 2;
            str = (char*) realloc(str, allocated_size * sizeof(char));
            if (str == NULL) {
                free(str);
                return NULL;
            }
        }
        memcpy(str + readed_bytes, chunk, CHUNK_SIZE - 1);
        readed_bytes += CHUNK_SIZE - 1;
        if (chunk[CHUNK_SIZE - 1] != '\0') {
            str[readed_bytes] = '\0';
            return str;
        }
        chunk[CHUNK_SIZE - 1] = 0xff;
    }

    free(str);
    return NULL;
}

void read_lines_with_custom(FILE* f) {
    char* line = NULL;
    while ((line = custom_getline(f)) != NULL) {
        free(line);
    }
}

ssize_t getline_with_getc(char** line, size_t* n, FILE* file) {
    char c = 0;
    *line = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    *n = BUFFER_SIZE;
    size_t read = 0;
    while ((c = getc(file)) != EOF) {
        if (read == *n) {
            *n *= 2;
            char* tmp = (char*)realloc(*line, *n * sizeof(char));
            if (!tmp) {
                free(line);
                line = NULL;
                return -1;
            }
            *line = tmp;
        }
        (*line)[read++] = c;
        if (c == '\n') {
            (*line)[read] = '\0';
            return read;
        }
    }
    (*line)[read] = '\0';
    return -1;
}


int main() {
    return 0;
}
