#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <strlcat.c>
#include <bvr.h>
#include <tape.c>
#include <bvrvar.c>
#include <inarray.c>
int bvr_commands_check_for_command(char * input_char, char ** value, int *i, FILE * input) {
	if((*input_char) == LINE_COMMAND_CHAR) {
		FILE * command_return;
		size_t buf_size;
		command_return = open_memstream(&(*value)+((*i)++), &buf_size); // i bajtov smo že napisali.
		if(bvr_command_processor(input, command_return) != SUCCESS) {
			fprintf(stderr, "[bvrcommands.c] bvr_commands_check_for_command: command, passed as argument, didn't return success. Going on.\n");
		}
		fflush(command_return);
		(*i) = (*i)+buf_size;
		(*input_char) = CLOSING_COMMAND_TAG_CHAR_1; // da zaključimo loop (drugače ostane notri ?)
		//\\ zelo slabo. znak, ki ga najde izveden ukaz, se izgubi. rešitev bi bila dobiti zadnji input char, ki triggera break
		// prejšnjega ukaza, kar pa je nemogoče.
		fflush(command_return);
		fclose(command_return);
		// fprintf(stderr, "debug: \"%s\"\n", (*value));
		return BVR_CONTINUE;
	}
	return BVR_KEEPGOING;
}
char bvr_var_skip_separator_chars(FILE * input) {
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
	char input_char = bvr_var_skip_separator_chars(input);
	int i = 0;
	while(1) {
		// i == napisali smo že toliko znakov
		if(i >= value_size) { // <-- todo: uncomment after done debugging
			value = realloc(value, (value_size) * BVR_VALUE_CHUNK_SIZE);
			value_size = value_size + BVR_VALUE_CHUNK_SIZE;
		}
		if(char_in_array(input_char, yeetus_chars)) {
			value[i++] = '\0';
			return value; // or yeet!
		}
		if(bvr_commands_check_for_command(&(input_char), &(value), &(i), input) == BVR_CONTINUE) {
			continue;
		}
		(value)[(i)++] = input_char;
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
int bvr_handle_info(FILE * input, FILE * output) {
	// fprintf(stderr, "[bvrcommands.c] bvr_handle_info: bvr bVerbose HTPCMS %d.%d.%d\n", BVR_VER_MAJOR, BVR_VER_MINOR, BVR_VER_PATCH);
	fprintf(output, "\nbvr bVerbose HTPCMS %d.%d.%d\n", BVR_VER_MAJOR, BVR_VER_MINOR, BVR_VER_PATCH);
	return SUCCESS;
}
