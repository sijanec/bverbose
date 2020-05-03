#pragma once
#include <bvr.h>
#include <string.h>
#include <stdlib.h>
char * bvr_var_get(char * item) {
	for(int i = 0; i < sizeof(bvr_variables)/sizeof(bvr_variables[0]); i=i+2) {
		// printf("%s, %s, %d, %d\n", bvr_variables[i], item, sizeof(bvr_variables)/sizeof(bvr_variables[0]), i);
		if(strcmp(bvr_variables[i], item) == 0) {
			return bvr_variables[i+1];
		}
  }
	return BVR_UNDEFINED;
}

int bvr_var_set(char * item, char * value) {
	if(bvr_bvrvar_first_time_set == 1) {
		for(int i = 0; i < (sizeof(bvr_variables)/sizeof(bvr_variables[0])); i=i+2) {
			// printf("loop here1\n");
			strlcpy(bvr_variables[i], BVR_UNDEFINED, sizeof(bvr_variables[i]));
		}
		bvr_bvrvar_first_time_set = 0;
	}
	if(strlen(value) >= BVR_MAX_VARIABLE_SIZE) { // >=, ker je še \0, ki ga strlen ne prišteje!
		value[BVR_MAX_VARIABLE_SIZE-1] = '\0';
		fprintf(stderr, "[bvrvar.c] bvr_set: value of variable %s too long, chopped to \"%s\"; increase BVR_MAX_VARIABLE_SIZE (%d). Returning FAILURE and setting anyways.\n",
			item, value, BVR_MAX_VARIABLE_SIZE);
	}
	for(int i = 0; i < (sizeof(bvr_variables)/sizeof(bvr_variables[0])); i=i+2) {
		// printf("loop here2\n");
		if(strcmp(bvr_variables[i], item) == 0) {
			strlcpy(bvr_variables[i+1], value, sizeof(bvr_variables[i+1]));
			return SUCCESS;
		}
  } // could already search for BVR_UNDEFINED here, but idc
	for(int i = 0; i < (sizeof(bvr_variables)/sizeof(bvr_variables[0])); i=i+2) {
		// printf("loop here4\n");
		if(strcmp(bvr_variables[i], BVR_UNDEFINED) == 0) {
			strlcpy(bvr_variables[i], item, sizeof(bvr_variables[i]));
			strlcpy(bvr_variables[i+1], value, sizeof(bvr_variables[i+1]));
			return SUCCESS;
		}
  }
	fprintf(stderr, "[bvrvar.c] bvr_set: no more space on the variable stack for %s. Increase BVR_INITIAL_VARIABLES_COUNT (%d).", item, BVR_INITIAL_VARIABLES_COUNT);
	return FAILURE;
}

int bvr_var_mv(char * item, char * newname) {
	for(int i = 0; i < (sizeof(bvr_variables)/sizeof(bvr_variables[0])); i=i+2) {
		if(strcmp(bvr_variables[i], item) == 0) {
			strlcpy(bvr_variables[i], newname, sizeof(bvr_variables[i]));
			return SUCCESS;
		}
	}
	fprintf(stderr, "[bvrvar.c] bvr_mv: variable %s not found!", item);
	return FAILURE;
}
