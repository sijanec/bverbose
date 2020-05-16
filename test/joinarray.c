#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <joinarrays.c>
// do not use this, it is idiotic, you have strlcpy!
extern int main(int argc, char* argv[]) {
	char a1[3] = {1, 2, 3};
	char a2[5] = {4, 5, 6, 7, 8};
	int * joined = join_null_terminated_arrays(a1, a2);
	joined[8] = '\n';
	fprintf(stdout, "%ld\n", joined);
	return 0;
}
