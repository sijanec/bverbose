#include <stdlib.h>
#include <stdio.h>
#include "randstring.c"

extern int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("usage: %s length\n", argv[0]);
		return 1;
	}
	printf("generated string:\n%s\n", randstring(atoi(argv[1])));
	return 0;
}
