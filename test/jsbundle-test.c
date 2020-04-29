#include <stdlib.h>
#include <stdio.h>
#include "jsbundle.c"

extern int main(int argc, char* argv[]) {
	if(argc != 4) {
		printf("usage: %s bundle.js source_directory/ .js\n", argv[0]);
		return 1;
	}
	bundle_js(argv[1], argv[2], argv[3]);
	return 0;
}
