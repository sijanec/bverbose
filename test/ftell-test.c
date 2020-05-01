#include <stdlib.h>
#include <stdio.h>

extern int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("usage: %s filename-to-read\n", argv[0]);
		return 1;
	}
	FILE * temp_input_file = fopen(argv[1], "r");
	char char_buffer = fgetc(temp_input_file);
	while(char_buffer != EOF) {
		printf("I have character %c on position %ld\n", char_buffer, ftell(temp_input_file)-1);
		char_buffer = fgetc(temp_input_file);
	}
	printf("reached EOF!\n");
	return 0;
}
