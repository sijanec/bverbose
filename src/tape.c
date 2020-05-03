#pragma once
#include <bvr.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <mkdirp.c>
#include <fopenmkdir.c>
#include <limits.h>
#include <randstring.c>
#include <bvrcommands.c>
#include <bvrvar.c>

int bvr_inline_command_processor(FILE * page_source_file, FILE * output_file, char copy_buffer[]) {
	FILE * temp_output_file = output_file;
	int what_to_return = SUCCESS;
	for(int i = 1; i <= OPENING_COMMAND_TAG_LENGTH; i++) {
		copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file); // remove opening command tag characters
	}
	int command_processor_status = WAITING_FOR_COMMAND;
	int argument_length = 0;
	char command_entered;
	while(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] != CLOSING_COMMAND_TAG_CHAR_1 ||
			copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] != CLOSING_COMMAND_TAG_CHAR_2 ||
			command_processor_status == PROCESSING_COMMAND) {
		copy_buffer[(ftell(page_source_file))% COPY_BUFFER_SIZE] = fgetc(page_source_file);
		if(copy_buffer[(ftell(page_source_file)-2)% COPY_BUFFER_SIZE] == LINE_COMMENT_CHAR && command_processor_status == WAITING_FOR_COMMAND) {
			temp_output_file = fopen(THE_VOID, "w");
			continue;
		}
		if(copy_buffer[(ftell(page_source_file)-2)% COPY_BUFFER_SIZE] == LINE_COMMAND_CHAR && command_processor_status == WAITING_FOR_COMMAND) {
			command_entered = copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE];
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
				char argument_string[argument_length+1];
				for(int i=0; i<argument_length; i++) {
					argument_string[i] = copy_buffer[0+(ftell(page_source_file)-(argument_length-(1+i)))% COPY_BUFFER_SIZE];
				}
				argument_string[argument_length-2] = '\0'; // -2, ker imamo še CLOSING_COMMAND_TAG_CHAR_1 in CLOSING_COMMAND_TAG_CHAR_2
				int command_handler_output = SUCCESS;
				FILE * argument_feed;
				argument_feed = fmemopen (argument_string, strlen (argument_string), "r");
				switch (command_entered) { // switch command
					case 'g':
						command_handler_output = bvr_handle_get(argument_feed, temp_output_file);
						break;
					case 's':
						command_handler_output = bvr_handle_set(argument_feed, temp_output_file);
						break;
					case 'i':
						command_handler_output = bvr_handle_include(argument_feed, temp_output_file);
						break;
					case 'm':
						command_handler_output = bvr_handle_move(argument_feed, temp_output_file);
						break;
					case 'b':
						// fprintf(stderr, "bunden %c\n", command_entered);
						command_handler_output = bvr_handle_info(argument_feed, temp_output_file);
						break;
					default:
						fprintf(stderr, "[tape.c] bvr_inline_command_processor: unknown command %c\n", command_entered);
						fprintf(temp_output_file, "\nbVerbose unknown command %c\n", command_entered);
				}
				if(command_handler_output != SUCCESS) {
					fprintf(stderr, "[tape.c] bvr_inline_command_processor: command handler for %c with argument \"%s\" returned an error code.\n", copy_buffer[(ftell(page_source_file)-(1+argument_length))% COPY_BUFFER_SIZE],
						argument_string);
					fprintf(temp_output_file,  "command handler for %c with argument \"%s\" returned an error code.\n", copy_buffer[(ftell(page_source_file)-(1+argument_length))% COPY_BUFFER_SIZE],
						argument_string);
					return FAILURE;
				}
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
	copy_buffer[(ftell(page_source_file)% COPY_BUFFER_SIZE)] = fgetc(page_source_file); // remove closing command tag character
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

	for(int i = 0; i < sizeof(copy_buffer); i++) { // da garbage vrednosti ne bodo slučajno ukazi! --- useless!, todo: delete
		copy_buffer[i] = ' ';
	}
 	copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file);
	if(copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] == EOF) {
		goto done_reading_write_file;
	}
  while (1) {
  	copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file);
		if(copy_buffer[(ftell(page_source_file)-1)% COPY_BUFFER_SIZE] == OPENING_COMMAND_TAG_CHAR_1 &&
				copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] == OPENING_COMMAND_TAG_CHAR_2) {
		 	if(bvr_inline_command_processor(page_source_file, temp_output_file, copy_buffer) == FAILURE) {
		 		fprintf(temp_output_file, "\nbvr_inline_command_processor returned an error status whilst composing %s\n", page_source_file_path);
				fprintf(stderr, "[tape.c] bvr_inline_command_processor returned an error status whilst composing %s\n", page_source_file_path);
		 	}
		  copy_buffer[ftell(page_source_file)% COPY_BUFFER_SIZE] = fgetc(page_source_file); // remove last > that just wants to be there... OB1
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
