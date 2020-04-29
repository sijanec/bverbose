#include <stdlib.h>
#include <stdio.h>
#include "mkdirp.c"

extern int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("usage: %s directory\n", argv[0]);
		return 1;
	}
	mkdir_p(argv[1]);
	return 0;
}
