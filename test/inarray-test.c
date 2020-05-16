#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <inarray.c>
extern int main(int argc, char* argv[]) {
	if(argc != 3) {
		printf("%s c string\n", argv[0]);
		return 1;
	}
	if(char_in_array(argv[1][0], argv[2])) {
		printf("%c is in %s\n", argv[1][0], argv[2]);
	} else {
		printf("%c is NOT in %s\n", argv[1][0], argv[2]);
	}
	return 0;
}
