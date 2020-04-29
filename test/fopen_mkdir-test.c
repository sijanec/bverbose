#include <stdlib.h>
#include <stdio.h>
#include "fopenmkdir.c"

extern int main(int argc, char* argv[]) {
	if(argc != 3) {
		printf("usage: %s file_to_write string_to_write\n", argv[0]);
		return 1;
	}
	FILE * test_file;
	test_file = fopen_mkdir(argv[1], "w");
	fprintf(test_file, "%s\n", argv[2]);
	return 0;
}
