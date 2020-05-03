#include <stdio.h>
#include <stdlib.h>
#include <bvrvar.c>

int main(int argc, char * argv[]) {
	bvr_var_set("abc", "1232");
	printf("%s\n", bvr_var_get("abc"));
	return 0;
}
