#include <stdlib.h>
#include <stdio.h>
#include <tape.c>
extern int main(int argc, char* argv[]) {
	if(argc != 3) {
		printf("usage: %s source-file file-with-commands-replaced-with-a's\n", argv[0]);
		return 1;
	}
	// bvr_var_set("abc", "1234");
	// bvr_var_set("ab", "123");
	// printf("%s\n", bvr_var_get("abc"));
	// printf("%s\n", bvr_var_get("ab"));
	bvr_compose_page(argv[1], 0, argv[2]);
	return 1;
}
