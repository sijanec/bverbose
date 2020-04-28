#include <stdlib.h>
#include <stdio.h>
#include "jsmin.c"

extern int main(int argc, char* argv[]) {
	if(argc <= 1) {
		printf("usage: %s source.js output.min.js\n", argv[0]);
		return 1;
	}
	minify_js(argv[1], argv[2]);
	return 0;
}
