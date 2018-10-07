#define _GNU_SOURCE

#include <sys/resource.h>
#include <sys/time.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 120

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

ssize_t getline_with_getc(char** line, size_t* n, FILE* file) {
	char c = 0;
	*line = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	*n = BUFFER_SIZE;
	size_t read = 0;
	while ((c = getc(file)) != EOF) {
		if (read == *n) {
			*n *= 2;
			char* tmp = realloc(*line, *n * sizeof(char));
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

void make_test(FILE *f, double* result1, double* result2) {
	rewind(f);
	double start = get_time();
	read_lines(f, getline_with_getc);
	double end = get_time();
	*result1 = end - start;
	rewind(f);
	start = get_time();
	read_lines(f, getline);
	end = get_time();
	*result2 = end - start;
}

void make_benchmark(unsigned count, FILE* f, double* result1, double* result2) {
	*result1 = 0;
	*result2 = 0;
	for (unsigned i = 0; i < count; ++i) {
		double tmp1 = 0;
		double tmp2 = 0;
		make_test(f, &tmp1, &tmp2);
		*result1 += tmp1;
		*result2 += tmp2;
	}
	*result1 /= count;
	*result2 /= count;
}

int main(int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <filename> <count of benchmark>\n", argv[0]);  // TODO: add getopt
		return 0;
	}

	FILE* f = fopen(argv[1], "r");
	if (!f) {
		perror("Error");
		return 0;
	}
	unsigned count = strtol(argv[2], NULL, 10);
	double custom_getline_time = 0;
	double getline_time = 0;
	make_benchmark(count, f, &custom_getline_time, &getline_time);
	printf("Custom getline average time: %f sec\n", custom_getline_time);
	printf("Unix getline average time: %f sec\n", getline_time);
	fclose(f);
	return 0;
}