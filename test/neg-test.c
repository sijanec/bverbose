#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
extern int main(int argc, char* argv[]) {
	char array[] = {'s','i','j','a','n','e','c',' '};
	printf("it should always be sijanec and a space character for 2000 characters or OB1\n");
	for(int i=0;i<=2000;i++) {
		printf("%c", array[i% sizeof(array)]);
		fflush(stdout);
	}
	return 0;
}
