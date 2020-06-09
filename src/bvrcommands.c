#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <strlcat.c>
#include <bvr.h>
#include <tape.c>
#include <bvrvar.c>
#include <inarray.c>
#include <uuid/uuid.h>

int bvr_commands_check_for_command(char * input_char, char ** value, int *i, FILE * input, int * value_size) {
	if((*input_char) == LINE_COMMAND_CHAR) {
		FILE * command_return;
		size_t buf_size;
		char * command_output;
		command_return = open_memstream(&command_output, &buf_size);
		if(bvr_command_processor(input, command_return) != SUCCESS) {
			fprintf(stderr, "[bvrcommands.c] bvr_commands_check_for_command: command, passed as argument, didn't return success. Going on.\n");
		}
		fflush(command_return);
		while((*value_size) < ((*i)+buf_size)) { // make room in chunks for command_output
			(*value) = realloc((*value), (*value_size)+BVR_VALUE_CHUNK_SIZE);
			if((*value) == NULL) {
				fprintf(stderr, "[bvrcommands.c] bvr_commands_check_for_command: CRITICAL OUT-OF-MEMORY.\n");
			}
			(*value_size) = (*value_size)+BVR_VALUE_CHUNK_SIZE;
		}
		for(int j = 0; j < buf_size; j++) {
			/// fprintf(stderr, "debug: \"%c\", %ld\n", command_output[j], buf_size);
			(*value)[(*i)++] = command_output[j];
		}

		(*input_char) = CLOSING_COMMAND_TAG_CHAR_1; // da zaključimo loop (drugače ostane notri ?)
		//\\ zelo slabo. znak, ki ga najde izveden ukaz, se izgubi. rešitev bi bila dobiti zadnji input char, ki triggera break
		// prejšnjega ukaza, kar pa je nemogoče.
		fflush(command_return);
		fclose(command_return);
		free(command_output);
		command_output == NULL; // the address/pointer, lost for ever!
		return BVR_CONTINUE;
	}
	return BVR_KEEPGOING;
}
char bvr_commands_skip_separator_chars(FILE * input) {
	char input_char = fgetc(input);
	while(input_char == ' ' || input_char == CLOSING_COMMAND_TAG_CHAR_1 || input_char == ',' || input_char == ';' || input_char == EOF ||
			input_char == '\0'  || input_char == '\n') {
		input_char = fgetc(input);
	}
	return input_char;
}
char * bvr_commands_get_value(FILE * input, char * yeetus_chars) {
	int value_size = BVR_VALUE_CHUNK_SIZE;
	char * value = (char*) malloc(value_size);
	char input_char = bvr_commands_skip_separator_chars(input);
	int i = 0;
	while(1) {
		// i == napisali smo že toliko znakov
		if(i >= value_size) { // <-- todo: uncomment after done debugging
			value = realloc(value, (value_size) * BVR_VALUE_CHUNK_SIZE);
			if(value == NULL) {
				fprintf(stderr, "[bvrcommands.c] bvr_commands_get_value: CRITICAL OUT-OF-MEMORY, FUCK!\n");
			}
			value_size = value_size + BVR_VALUE_CHUNK_SIZE;
		}
		if(char_in_array(input_char, yeetus_chars)) {
			value[i++] = '\0';
			return value; // or yeet!
		}
		if(bvr_commands_check_for_command(&(input_char), &(value), &(i), input, &(value_size)) == BVR_CONTINUE) {
			continue;
		}
		(value)[(i)++] = input_char;
		input_char = fgetc(input);
	}
	// we should not get here!
}
char * bvr_commands_get_string(FILE * input) {
	int value_size = BVR_VALUE_CHUNK_SIZE;
	char * value = (char*) malloc(value_size);
	char input_char = fgetc(input);
	char previous_char = 'a'; // please don't make a an escape char
	int i = 0;
	while(1) {
		// i == napisali smo že toliko znakov
		if(i >= value_size) { // <-- todo: uncomment after done debugging
			value = realloc(value, (value_size) * BVR_VALUE_CHUNK_SIZE);
			if(value == NULL) {
				fprintf(stderr, "[bvrcommands.c] bvr_commands_get_value: CRITICAL OUT-OF-MEMORY, FUCK!\n");
			}
			value_size = value_size + BVR_VALUE_CHUNK_SIZE;
		}
		if(input_char == BVR_BREAK_STRING_CHAR) {
			if(previous_char != BVR_ESCAPE_CHAR) {
				value[i++] = '\0';
				return value;
			} else {
				i--; // da zamenjamo prejšnji ESCAPE_CHAR z BREAK_STRING charom
				(value)[(i)++] = BVR_BREAK_STRING_CHAR;
			}
		} else {
			(value)[(i)++] = input_char;
		}
		previous_char = input_char;
		input_char = fgetc(input);
	}
	// we should not get here!
}
int bvr_handle_substring(FILE * input, FILE * output) { // acts like https://www.php.net/manual/en/function.substr.php
	char chars_to_break_value[69] = ",; ";
	strlcat(chars_to_break_value, BVR_CHARS_TO_BREAK_VALUE, sizeof(chars_to_break_value));
	char * offset = bvr_commands_get_value(input, chars_to_break_value);
	char * length = bvr_commands_get_value(input, chars_to_break_value);
	char * string = bvr_commands_get_value(input, BVR_CHARS_TO_BREAK_VALUE);

	int return_status = SUCCESS;
	int real_offset = atoi(offset);

	if(real_offset < 0) {
		real_offset = strlen(string)-abs(real_offset);
	}

	if(strlen(string) < real_offset) { // catch errors
		fprintf(stderr, "[bvrcommands.c] bvr_handle_substring: string lenght (%ld) is less than offset (%d). fuck you.\n",
			strlen(string), real_offset);
		fprintf(output, "\nbVerbose substring: string length of %ld is less than offset of %d.\n", strlen(string), real_offset);
		return_status = FAILURE;
		goto cleanup_and_return;
	}
	if(real_offset < 0) { // catch errors
		fprintf(stderr, "[bvrcommands.c] bvr_handle_substring: supplied negative offset after calculation (%d) is negative. failing...\n",
			real_offset);
		fprintf(output, "\nbVerbose substring: supplied negative offset after calculation (%d) is negative.\n", real_offset);
		return_status = FAILURE;
		goto cleanup_and_return;
	}

	if(atoi(length) < 0) {
		string[strlen(string)-abs(atoi(length))] = '\0';
		fprintf(output, "%s", string+real_offset);
	} else {
		fprintf(output, "%.*s", atoi(length), string+real_offset);
	}

	cleanup_and_return:
	fflush(output);
	free(offset);
	free(length);
	free(string);
	offset = NULL;
	length = NULL;
	string = NULL;
	return return_status;
}
int bvr_handle_get(FILE * input, FILE * output) {
	char chars_to_break_value[69] = ",; ";
	strlcat(chars_to_break_value, BVR_CHARS_TO_BREAK_VALUE, sizeof(chars_to_break_value));
	char * item = bvr_commands_get_value(input, chars_to_break_value);

	fprintf(output, "%s", bvr_var_get(item));
	fflush(output);
	free(item);
	item = NULL;
	return SUCCESS;
}
int bvr_handle_set(FILE * input, FILE * output) {
	char chars_to_break_value[69] = " ";
	strlcat(chars_to_break_value, BVR_CHARS_TO_BREAK_VALUE, sizeof(chars_to_break_value));
	char * item = bvr_commands_get_value(input, chars_to_break_value);
	char * value = bvr_commands_get_value(input, BVR_CHARS_TO_BREAK_VALUE);
	int return_value = bvr_var_set(item, value);
	fflush(output);
	free(item);
	free(value);
	item = NULL;
	value = NULL;
	return return_value;
}
int bvr_handle_include(FILE * input, FILE * output) {
	char chars_to_break_value[69] = " ";
	strlcat(chars_to_break_value, BVR_CHARS_TO_BREAK_VALUE, sizeof(chars_to_break_value));
	// fprintf(stderr, "debug: \"%s\"\n", chars_to_break_value);
	char * item = bvr_commands_get_value(input, chars_to_break_value);

	FILE * stream = fopen(item, "r");
	char notgoodatnamingvariables[PATH_MAX];
	char * path = bvr_var_get(BVR_INCLUDE_PATH_VAR_NAME);
	if(stream == NULL) {
		strcpy(notgoodatnamingvariables, item);
		strcat(notgoodatnamingvariables, BVR_COMMAND_FILE_EXT);
		stream = fopen(notgoodatnamingvariables, "r");
		if(strcmp(path, BVR_UNDEFINED) == 0 && stream == NULL) {
			fprintf(output, "\nbVerbose include error. File \"%s\" not found. Path is undefined.\n", item);
			fprintf(stderr, "[bvrcommands.c] bvr_handle_include: File \"%s\" not found. Path is undefined.\n", item);
			return FAILURE;
		}
	}
	char * singlepath;
	while(stream == NULL) {
		singlepath = strrchr(path, BVR_PATH_SEPARATOR);
		if(singlepath == NULL) {
			strcpy(notgoodatnamingvariables, path);
			strcat(notgoodatnamingvariables, item);
			stream = fopen(notgoodatnamingvariables, "r"); // ob1 fuckery
			if(stream == NULL) {
				strcpy(notgoodatnamingvariables, path);
				strcat(notgoodatnamingvariables, item);
				strcat(notgoodatnamingvariables, BVR_COMMAND_FILE_EXT);
				stream = fopen(notgoodatnamingvariables, "r"); // ob1 fuckery
				if(stream == NULL) {
					fprintf(output, "\nbVerbose include error. File \"%s\" not found.\n", item);
					fprintf(stderr, "[bvrcommands.c] bvr_handle_include: File \"%s\" not found.\n", item);
					return FAILURE;
				}
				break;
			}
			break;
		}
		strcpy(notgoodatnamingvariables, singlepath);
		strcat(notgoodatnamingvariables, item);
		stream = fopen(notgoodatnamingvariables+1, "r"); // ob1 fuckery
		if(stream == NULL) {
			strcat(notgoodatnamingvariables, BVR_COMMAND_FILE_EXT);
			stream = fopen(notgoodatnamingvariables+1, "r"); // ob1 fuckery
			if(stream != NULL) {
				break;
			}
		}
		*singlepath = '\0';
	}
	int return_status = bvr_compose_stream(stream, output);
	fflush(output);
	free(item);
	item = NULL;
	return return_status;
	fflush(output); // kja?
}
int bvr_handle_move(FILE * input, FILE * output) {
	char chars_to_break_value[69] = " ";
	strlcat(chars_to_break_value, BVR_CHARS_TO_BREAK_VALUE, sizeof(chars_to_break_value));
	char * item = bvr_commands_get_value(input, chars_to_break_value);
	char * value = bvr_commands_get_value(input, chars_to_break_value);

	int return_value = bvr_var_mv(item, value);
	free(item);
	free(value);
	value = NULL;
	item = NULL;
	fflush(output);
	return return_value;

}
int bvr_handle_string(FILE * input, FILE * output) {
	char * item = bvr_commands_get_string(input);
	uuid_t binuuid;
	uuid_generate_random(binuuid);
	char uuid[37];
	uuid_unparse(binuuid, uuid);
	int return_value = bvr_var_set(uuid, item);
	free(item);
	item = NULL;
	fprintf(output, "%s", uuid);
	fflush(output);
	return return_value;
}
int bvr_handle_equals(FILE * input, FILE * output) {
	char chars_to_break_value[69] = " ";
	strlcat(chars_to_break_value, BVR_CHARS_TO_BREAK_VALUE, sizeof(chars_to_break_value));
	char * item = bvr_commands_get_value(input, chars_to_break_value);
	char * value = bvr_commands_get_value(input, chars_to_break_value);
	int return_value = 0;
	char * string1 = bvr_var_get(item);
	char * string2 = bvr_var_get(value);
	if(strcmp(string1, string2) == 0) {
		fprintf(output, "1");
	} else {
		fprintf(output, "0");
	}
	free(item);
	free(value);
	item = NULL;
	value = NULL;
	fflush(output);
	return return_value;
}
int bvr_handle_if(FILE * input, FILE * output) { // ?f 1 <@this is all executed@>
	// first argument must *POINT* to a string which evaluates to 1, the second argument is the value of the endif string, spaces
	// are argument delimeters. endif is only used for skipping forward if if should not execute.
	char * chars_to_break_value = " ";
	strlcat(chars_to_break_value, BVR_CHARS_TO_BREAK_VALUE, sizeof(chars_to_break_value));
	char * item = bvr_commands_get_value(input, chars_to_break_value);
	int return_value = 0;
	if(strcmp(bvr_var_get(item), "1") == 0) {
		char chars_to_break_value[3] = {OPENING_COMMAND_TAG_CHAR_2, EOF, '\0'};
		char * temp = bvr_commands_get_value(input, chars_to_break_value);
		free(temp);
		temp = NULL;
		char copy_buffer[BVR_COPY_BUFFER_SIZE];
		init_tape_copy_buffer(copy_buffer, COPY_BUFFER_SIZE);
		return_value = bvr_inline_command_processor(input, output, copy_buffer);
	} else {
		char input_char = fgetc(input);
		char previous_char = 'a';
		int depth = -1; // to increase to 0 after first <@
		int we_re_in_a_comment = 0;
		while(input_char != BVR_CLOSING_COMMAND_TAG_CHAR_2 && previous_char != BVR_CLOSING_COMMAND_TAG_CHAR_1 && depth == 0 &&
					we_re_in_a_comment == 0) {
			if(previous_char == BVR_OPENING_COMMAND_TAG_CHAR_1 && input_char == BVR_OPENING_COMMAND_TAG_CHAR_2) {
				depth++;
			} // this šubidubi doesn't account for <@ and @> in strings.
			if(previous_char == BVR_CLOSING_COMMAND_TAG_CHAR_1 && input_char== BVR_CLOSING_COMMAND_TAG_CHAR_2) {
				depth--;
			}
			if(previous_char == LINE_COMMAND_CHAR && input_char == BVR_BREAK_STRING_CHAR) {
				char * temp = bvr_commands_get_string(input);
				free(temp);
				temp = NULL;
			}
			if(previous_char == BVR_NEWLINE_CHAR && input_char == LINE_COMMENT_CHAR) {
				we_re_in_a_comment = 1;
			}
			if(we_re_in_a_comment && input_char == BVR_NEWLINE_CHAR) {
				we_re_in_a_comment = 0;
			}
			previous_char = input_char;
			input_char = fgetc(input);
		}
	}
	free(item);
	item = NULL;
	if(return_value != 0) {
		fprintf(stderr, "bvrcommands.c: bvr_handle_if: bvr_compose_stream returned an error status!\n");
	}
	return return_value;
}
int bvr_handle_info(FILE * input, FILE * output) {
	// fprintf(stderr, "[bvrcommands.c] bvr_handle_info: bvr bVerbose HTPCMS %d.%d.%d\n", BVR_VER_MAJOR, BVR_VER_MINOR, BVR_VER_PATCH);
	fprintf(output, "\nbvr bVerbose HTPCMS %d.%d.%d\n", BVR_VER_MAJOR, BVR_VER_MINOR, BVR_VER_PATCH);
	return SUCCESS;
}
