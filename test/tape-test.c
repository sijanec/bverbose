#include <stdlib.h>
#include <stdio.h>
#include <tape.c>
extern int main(int argc, char* argv[]) {
	if(argc != 3) {
		printf("usage: %s source-file file-with-commands-replaced-with-a's\n", argv[0]);
		return 1;
	}
	return bvr_compose_page(argv[1], 0, argv[2]);
}
