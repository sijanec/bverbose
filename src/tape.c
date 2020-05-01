#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <mkdirp.c>
#include <fopenmkdir.c>
#include <limits.h>
#include <randstring.c>
#define BVR_MAX_VARIABLE_SIZE 128
#define BVR_INITIAL_VARIABLES_COUNT 128
#define SUCCESS 0
#define FAILURE -1
#define COPY_BUFFER_SIZE 2
#define OPENING_COMMAND_TAG_LENGTH 2
#define OPENING_COMMAND_TAG_CHAR_1 '<'
#define OPENING_COMMAND_TAG_CHAR_2 '@'
#define CLOSING_COMMAND_TAG_CHAR_1 '@'
#define CLOSING_COMMAND_TAG_CHAR_2 '>'
char bvr_variables[BVR_INITIAL_VARIABLES_COUNT][BVR_MAX_VARIABLE_SIZE];
// char* bvr_variables[128] = malloc(BVR_INITIAL_VARIABLES_COUNT * BVR_MAX_VARIABLE_SIZE);
int bvr_variables_multiplier = 1;

// int bvr_increase_variables() { // this can be costly https://stackoverflow.com/a/3827922
// 	char* more_variables = realloc(bvr_variables, ++bvr_variables_multiplier * BVR_INITIAL_VARIABLES_COUNT * sizeof(BVR_MAX_VARIABLE_SIZE));
// 	if (!more_variables) {
// 		printf("[tape.c] bvr_increase_variables: unable to increase variables count. Program will continue, but may run out of variable space and fail\n");
// 		bvr_variables_multiplier = bvr_variables_multiplier - 1;
// 		return FAILURE;
// 	}
// 	return SUCCESS;
// }


int bvr_command_processor(FILE * page_source_file, FILE * temp_output_file, char copy_buffer[]) {
	for(int i = 1; i <= OPENING_COMMAND_TAG_LENGTH; i++) {
		copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file); // remove opening command tag characters
	}
	while(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] != CLOSING_COMMAND_TAG_CHAR_1 || copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] != CLOSING_COMMAND_TAG_CHAR_2) {
		copy_buffer[(ftell(page_source_file))% COPY_BUFFER_SIZE] = fgetc(page_source_file); // advance file pointer to the end of command without rewriting.
		fputc('a', temp_output_file);
		if(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] == EOF) {
			fprintf(temp_output_file, "\nbVerbose syntax error: EOF reached before closing command tag --- stopped with this bvr file\n");
			fprintf(stderr, "[tape.c] bvr_command_processor: syntax error: EOF reached before @> --- stopped with this bvr file\n");
			return FAILURE;
		}
	}
	copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file); // remove closing command tag character
	return SUCCESS;
}

int bvr_compose_page(char page_source_file_path[], int this_is_a_top_level_page, char * temp_output_path) {
	if(temp_output_path != NULL) {
		char * temp_output_filename = randstring(16);
		char temp_output_path[80] = "tmp/";
		strcat(temp_output_path, temp_output_filename);
	}
	char copy_buffer[COPY_BUFFER_SIZE];
	FILE * temp_output_file = fopen_mkdir(temp_output_path, "w");
	FILE * page_source_file = fopen(page_source_file_path, "r");

	for(int i = 0; i < sizeof(copy_buffer); i++) { // da garbage vrednosti ne bodo slučajno ukazi!
		copy_buffer[i] = ' ';
	}
 	copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file);
	if(copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] == EOF) {
		goto done_reading_write_file;
	}
  while (1) {
  	copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file);
		if(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] == OPENING_COMMAND_TAG_CHAR_1 && copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] == OPENING_COMMAND_TAG_CHAR_2) {
		 	if(bvr_command_processor(page_source_file, temp_output_file, copy_buffer)) {
		 		fprintf(temp_output_file, "\nbvr_command_processor returned an error status whilst composing %s\n", page_source_file_path);
		 	}
		} else {
			if(copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] == EOF) {
				break;
			}
			fputc(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE], temp_output_file);
		}
  }


	done_reading_write_file:
	if(this_is_a_top_level_page) {
		char folder_of_file[PATH_MAX];
		strcpy(folder_of_file, page_source_file_path);
		char * p;
		p = strrchr(folder_of_file, '/');
		if (!p) {
			printf("[fopenmkdir.c] filename contains no slash.\n");
			return FAILURE;
		} else {
			*p = '\0';
		}
		char *type_of_page = strrchr(folder_of_file, '/');
		if (type_of_page == NULL) {
  		printf("[tape.c] bvr_compose_page: folder of file contains no slash, hope %s is a page category.", folder_of_file);
			type_of_page = folder_of_file;
		} else {
			type_of_page++;
		}
		// type_of_page+1 is a page category (skip delimeter pointer.)
	}
	return SUCCESS;
}
