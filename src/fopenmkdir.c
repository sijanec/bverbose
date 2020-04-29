#include <mkdirp.c>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * fopen_mkdir(char* filename, char* mode) {
	FILE * file_to_return;
	file_to_return = fopen(filename, mode);
	if(file_to_return == NULL) {
		char folder_of_file[256];
		strcpy(folder_of_file, filename);
		char * p;
		p = strrchr(folder_of_file, '/');
		if (!p) {
			printf("[fopenmkdir.c] filename contains no slash.\n");
			return NULL;
		} else {
			printf("folder is %s\n", folder_of_file);
			printf("filename is %s\n", filename);
			*p = '\0';
		}
		if(mkdir_p(folder_of_file) != 0) {
			return NULL;
		}
		file_to_return = fopen(filename, mode);
		if(file_to_return == NULL) {
			return NULL;
		}
		return file_to_return;
	} else {
		return file_to_return;
	}
}
