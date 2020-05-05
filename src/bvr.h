#pragma once
#include <strlcpy.c>
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
#define THE_VOID "/dev/null"

#define BVR_INITIAL_VARIABLES_COUNT 128
#define BVR_MAX_VARIABLE_SIZE 128
#define BVR_UNDEFINED "BVR_UNDEFINED"

char bvr_variables[BVR_INITIAL_VARIABLES_COUNT*2][BVR_MAX_VARIABLE_SIZE];
int bvr_bvrvar_first_time_set = 1;

#define BVR_VER_MAJOR 0
#define BVR_VER_MINOR 0
#define BVR_VER_PATCH 0

#define BVR_PATH_SEPARATOR ';'
#define BVR_INCLUDE_PATH_VAR_NAME "bvr_include_path"

int bvr_compose_stream(FILE *, FILE *);
