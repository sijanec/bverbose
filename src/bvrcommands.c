#pragma once
#include <bvr.h>
#include <bvrvar.c>
char bvr_var_skip_separator_chars(FILE * input) {
	char input_char = fgetc(input);
	while(input_char == ' ' || input_char == CLOSING_COMMAND_TAG_CHAR_1 || input_char == ',' || input_char == ';' || input_char == EOF ||
			input_char == '\0'  || input_char == '\n') {
		input_char = fgetc(input);
	}
	return input_char;
}
int bvr_handle_get(FILE * input, FILE * output) {
	char item[BVR_MAX_VARIABLE_SIZE+1];
	char input_char = bvr_var_skip_separator_chars(input);
	int i = 0;
	while(input_char != ' ' && input_char != CLOSING_COMMAND_TAG_CHAR_1 && input_char != ',' && input_char != ';' && input_char != EOF &&
			input_char != '\0' && input_char != '\n' && i < BVR_MAX_VARIABLE_SIZE) {
		item[i++] = input_char;
		input_char = fgetc(input);
	}
	item[i++] = '\0';
	fprintf(output, "%s", bvr_var_get(item));
	fflush(output);
	return SUCCESS;
}
int bvr_handle_set(FILE * input, FILE * output) {
	char item[BVR_MAX_VARIABLE_SIZE+1];
	char value[BVR_MAX_VARIABLE_SIZE+1];
	char input_char = bvr_var_skip_separator_chars(input);
	int i = 0;
	while(input_char != ' ' && input_char != CLOSING_COMMAND_TAG_CHAR_1 && input_char != ',' && input_char != ';' && input_char != EOF &&
			input_char != '\0' && input_char != '\n' && i < BVR_MAX_VARIABLE_SIZE) {
		item[i++] = input_char;
		input_char = fgetc(input);
	}
	item[i++] = '\0';
	i = 0;
	input_char = bvr_var_skip_separator_chars(input);
	while(input_char != CLOSING_COMMAND_TAG_CHAR_1 && input_char != ',' && input_char != ';' && input_char != EOF &&
			input_char != '\0' && input_char != '\n' && i < BVR_MAX_VARIABLE_SIZE) {
		value[i++] = input_char;
		input_char = fgetc(input);
	}
	value[i++] = '\0';
	return bvr_var_set(item, value);
	fflush(output);
	return SUCCESS;
}
int bvr_handle_include(FILE * input, FILE * output) {
	char item[BVR_MAX_VARIABLE_SIZE+1];
	char input_char = bvr_var_skip_separator_chars(input);
	int i = 0;
	while(input_char != ' ' && input_char != CLOSING_COMMAND_TAG_CHAR_1 && input_char != ',' && input_char != ';' && input_char != EOF &&
			input_char != '\0' && input_char != '\n' && i < BVR_MAX_VARIABLE_SIZE) {
		item[++i] = input_char;
		input_char = fgetc(input);
	}
	item[++i] = '\0';
	fprintf(output, "%s", bvr_var_get(item));
	fflush(output);
	return SUCCESS;
}
int bvr_handle_move(FILE * input, FILE * output) {
	char item[BVR_MAX_VARIABLE_SIZE+1];
	char value[BVR_MAX_VARIABLE_SIZE+1];
	char input_char = bvr_var_skip_separator_chars(input);
	int i = 0;
	while(input_char != ' ' && input_char != CLOSING_COMMAND_TAG_CHAR_1 && input_char != ',' && input_char != ';' && input_char != EOF &&
			input_char != '\0' && input_char != '\n' && i < BVR_MAX_VARIABLE_SIZE) {
		item[++i] = input_char;
		input_char = fgetc(input);
	}
	item[++i] = '\0';
	i = 0;
	input_char = bvr_var_skip_separator_chars(input);
	while(input_char != ' ' && input_char != CLOSING_COMMAND_TAG_CHAR_1 && input_char != ',' && input_char != ';' && input_char != EOF &&
			input_char != '\0' && input_char != '\n' && i < BVR_MAX_VARIABLE_SIZE) {
		value[i++] = input_char;
		input_char = fgetc(input);
	}
	value[i++] = '\0';
	return bvr_var_mv(item, value);
	fflush(output);
	return SUCCESS;

}
int bvr_handle_info(FILE * input, FILE * output) {
	// fprintf(stderr, "[bvrcommands.c] bvr_handle_info: bvr bVerbose HTPCMS %d.%d.%d\n", BVR_VER_MAJOR, BVR_VER_MINOR, BVR_VER_PATCH);
	fprintf(output, "\nbvr bVerbose HTPCMS %d.%d.%d\n", BVR_VER_MAJOR, BVR_VER_MINOR, BVR_VER_PATCH);
	return SUCCESS;
}
