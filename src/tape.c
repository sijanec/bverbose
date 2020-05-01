#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <mkdirp.c>
#include <fopenmkdir.c>
#include <limits.h>
#include <randstring.c>
#include <strlcpy.c>
#define BVR_MAX_VARIABLE_SIZE 128
#define BVR_INITIAL_VARIABLES_COUNT 128
#define SUCCESS 0
#define FAILURE -1
#define COPY_BUFFER_SIZE 128
#define OPENING_COMMAND_TAG_LENGTH 2
#define OPENING_COMMAND_TAG_CHAR_1 '<'
#define OPENING_COMMAND_TAG_CHAR_2 '@'
#define CLOSING_COMMAND_TAG_CHAR_1 '@'
#define CLOSING_COMMAND_TAG_CHAR_2 '>'
#define LINE_COMMENT_CHAR '#'
#define LINE_COMMAND_CHAR '?'
#define WAITING_FOR_COMMAND 8922
#define READING_COMMAND 2343
#define PROCESSING_COMMAND 346
char bvr_variables[BVR_INITIAL_VARIABLES_COUNT][BVR_MAX_VARIABLE_SIZE];
// char* bvr_variables[128] = malloc(BVR_INITIAL_VARIABLES_COUNT * BVR_MAX_VARIABLE_SIZE);
// int bvr_variables_multiplier = 1;

// int bvr_increase_variables() { // this can be costly https://stackoverflow.com/a/3827922
// 	char* more_variables = realloc(bvr_variables, ++bvr_variables_multiplier * BVR_INITIAL_VARIABLES_COUNT * sizeof(BVR_MAX_VARIABLE_SIZE));
// 	if (!more_variables) {
// 		printf("[tape.c] bvr_increase_variables: unable to increase variables count. Program will continue, but may run out of variable space and fail\n");
// 		bvr_variables_multiplier = bvr_variables_multiplier - 1;
// 		return FAILURE;
// 	}
// 	return SUCCESS;
// }

int bvr_inline_command_processor(FILE * page_source_file, FILE * temp_output_file, char copy_buffer[]) {
	int what_to_return = SUCCESS;
	for(int i = 1; i <= OPENING_COMMAND_TAG_LENGTH; i++) {
		copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file); // remove opening command tag characters
	}
	int command_processor_status = WAITING_FOR_COMMAND;
	int argument_length = 0;
	while(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] != CLOSING_COMMAND_TAG_CHAR_1 ||
			copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] != CLOSING_COMMAND_TAG_CHAR_2 ||
			command_processor_status == PROCESSING_COMMAND) {
		copy_buffer[(ftell(page_source_file))% COPY_BUFFER_SIZE] = fgetc(page_source_file); // advance file pointer to the end of command without rewriting.
		// printf("received command char %c\n", copy_buffer[(ftell(page_source_file)-2)% COPY_BUFFER_SIZE]);
		if(copy_buffer[(ftell(page_source_file)-2)% COPY_BUFFER_SIZE] == '?' && command_processor_status == WAITING_FOR_COMMAND) {
			command_processor_status = READING_COMMAND;
			continue;
		}
		if(command_processor_status == READING_COMMAND) {
			if(++argument_length > COPY_BUFFER_SIZE) {
				fprintf(temp_output_file, "\nbVerbose %d bytes till buffer overflow. Decrease argument length (%d), increase COPY_BUFFER_SIZE (%d) or take cover.\n",
					COPY_BUFFER_SIZE-argument_length, argument_length, COPY_BUFFER_SIZE);
				fprintf(stderr, "[tape.c] bvr_inline_command_processor: %d bytes till buffer overflow. Decrease argument length (%d), increase COPY_BUFFER_SIZE (%d) or take cover.\n",
					COPY_BUFFER_SIZE-argument_length, argument_length, COPY_BUFFER_SIZE);
				what_to_return = FAILURE;
			}
			if(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] == CLOSING_COMMAND_TAG_CHAR_1 &&
					copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] == CLOSING_COMMAND_TAG_CHAR_2) { // end of arguments!
				/// char argument_string[argument_length+1];
				/// strncpy(argument_string, copy_buffer+(ftell(page_source_file)-(argument_length-1))% COPY_BUFFER_SIZE, argument_length);
				copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] = '\0';
				/// argument_string[argument_length-2] = '\0';
				printf("end of command, command was %c, argument was %s\n", copy_buffer[(ftell(page_source_file)-(1+argument_length))% COPY_BUFFER_SIZE],
				/// 	argument_string);
					copy_buffer+(ftell(page_source_file)-(argument_length-1))% COPY_BUFFER_SIZE);
				// switch (copy_buffer[(ftell(page_source_file)-(1+argument_length))]) { // switch command
					// 
				// }
				return SUCCESS;
			}
		}
		// fputc('a', temp_output_file);
		if(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] == EOF) {
			fprintf(temp_output_file, "\nbVerbose syntax error: EOF reached before closing command tag --- stopped with this bvr file\n");
			fprintf(stderr, "[tape.c] bvr_inline_command_processor: syntax error: EOF reached before @> --- stopped with this bvr file\n");
			return FAILURE;
		}
	}
	copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file); // remove closing command tag character
	return what_to_return;
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
		 	if(bvr_inline_command_processor(page_source_file, temp_output_file, copy_buffer) == FAILURE) {
		 		fprintf(temp_output_file, "\nbvr_inline_command_processor returned an error status whilst composing %s\n", page_source_file_path);
				fprintf(stderr, "[tape.c] bvr_inline_command_processor returned an error status whilst composing %s\n", page_source_file_path);
		 	}
			continue;
		}
		if(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] == '\n' && copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] == LINE_COMMENT_CHAR) {
			copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file); // idk, OB1 is a bitch
			while(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] != '\n') { // idk, OB1 is a bitch
 				copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file); // idk, OB1 is a bitch
			}
			continue;
		}
		// if(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] == '\n' && copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] == LINE_COMMAND_CHAR) {
		// 	if(bvr_command_processor(page_source_file, temp_output_file, copy_buffer) == FAILURE) {
		//  		fprintf(temp_output_file, "\nbvr_command_processor returned an error status whilst composing %s\n", page_source_file_path);
		// 		fprintf(stderr, "[tape.c] bvr_command_processor returned an error status whilst composing %s\n", page_source_file_path);
		// 	}
		// 	continue;
		// }
		if(copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] == EOF) {
			fputc('\n', temp_output_file);
			break;
		}
		fputc(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE], temp_output_file);
  }


	done_reading_write_file:
	if(this_is_a_top_level_page) {
		char folder_of_file[PATH_MAX];
		strlcpy(folder_of_file, page_source_file_path, PATH_MAX);
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
