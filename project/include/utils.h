#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>

double get_time();
void read_lines(FILE* file, ssize_t (*getline_func)(char**, size_t*, FILE*));
ssize_t custom_getline(char** line, size_t* n, FILE* file);
// void read_lines_with_custom(FILE* file);
ssize_t getline_with_getc(char** line, size_t* n, FILE* file);

#endif  // UTILS_H_