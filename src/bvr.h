#pragma once
#include <strlcpy.c>
#include <stdlib.h>
#include <stdio.h>
#define SUCCESS 0
#define FAILURE -1
#define COPY_BUFFER_SIZE 128
#define BVR_COPY_BUFFER_SIZE COPY_BUFFER_SIZE
#define OPENING_COMMAND_TAG_LENGTH 2
#define OPENING_COMMAND_TAG_CHAR_1 '<'
#define OPENING_COMMAND_TAG_CHAR_2 '@'
#define CLOSING_COMMAND_TAG_CHAR_1 '@'
#define CLOSING_COMMAND_TAG_CHAR_2 '>'
#define BVR_OPENING_COMMAND_TAG_LENGTH OPENING_COMMAND_TAG_LENGTH
#define BVR_OPENING_COMMAND_TAG_CHAR_1 OPENING_COMMAND_TAG_CHAR_1
#define BVR_OPENING_COMMAND_TAG_CHAR_2 OPENING_COMMAND_TAG_CHAR_2
#define BVR_CLOSING_COMMAND_TAG_CHAR_1 CLOSING_COMMAND_TAG_CHAR_1
#define BVR_CLOSING_COMMAND_TAG_CHAR_2 CLOSING_COMMAND_TAG_CHAR_2
#define LINE_COMMENT_CHAR '#'
#define LINE_COMMAND_CHAR '?'
#define WAITING_FOR_COMMAND 8922
#define READING_COMMAND 2343
#define PROCESSING_COMMAND 346 // not needed
#define THE_VOID "/dev/null"

#define BVR_INITIAL_VARIABLES_COUNT 128
#define BVR_MAX_VARIABLE_SIZE 128
#define BVR_UNDEFINED "BVR_UNDEFINED"

char bvr_variables[BVR_INITIAL_VARIABLES_COUNT*2][BVR_MAX_VARIABLE_SIZE];
int bvr_bvrvar_first_time_set = 1;

#define BVR_VER_MAJOR 0
#define BVR_VER_MINOR 3
#define BVR_VER_PATCH 0

#define BVR_PATH_SEPARATOR ' '
#define BVR_INCLUDE_PATH_VAR_NAME "bvr_include_path"
#define BVR_COMMAND_FILE_EXT ".bvr"

#define BVR_CONTINUE 1328
#define BVR_KEEPGOING 132
char BVR_CHARS_TO_BREAK_VALUE[3] = {CLOSING_COMMAND_TAG_CHAR_1, EOF, '\0'}; // 0x00 mora biti na koncu (null terminated array)

#define BVR_VALUE_CHUNK_SIZE 256
int bvr_compose_stream(FILE *, FILE *);
int bvr_command_processor(FILE *, FILE *);

#define BVR_BREAK_STRING_CHAR '"'
#define BVR_ESCAPE_CHAR '\\'

#define BVR_NEWLINE_CHAR '\n'

int init_tape_copy_buffer(char [], int);
int bvr_inline_command_processor(FILE *, FILE *, char []);
