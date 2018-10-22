#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <utils.h>


void make_test(FILE *f, double* result1, double* result2, double* result3) {
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
	rewind(f);
	start = get_time();
	read_lines(f, custom_getline);
	end = get_time();
	*result3 = end - start;
}

void make_benchmark(unsigned count, FILE* f, double* result1, double* result2, double* result3) {
	*result1 = 0;
	*result2 = 0;
	*result3 = 0;
	for (unsigned i = 0; i < count; ++i) {
		double tmp1 = 0;
		double tmp2 = 0;
		double tmp3 = 0;
		make_test(f, &tmp1, &tmp2, &tmp3);
		*result1 += tmp1;
		*result2 += tmp2;
		*result3 += tmp3;
	}
	*result1 /= count;
	*result2 /= count;
	*result3 /= count;
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
	double getline_with_getc_time = 0;
	double getline_time = 0;
	double custom_getline_time = 0;
	make_benchmark(count, f, &getline_with_getc_time, &getline_time, &custom_getline_time);
	printf("Getline with getchar average time: %f sec\n", getline_with_getc_time);
	printf("Unix getline average time: %f sec\n", getline_time);
	printf("Custom getline average time: %f sec\n", custom_getline_time);
	fclose(f);
	return 0;
}