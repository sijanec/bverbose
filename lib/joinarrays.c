#pragma once
#include <bvr.h>
// do not use this, this is idiotic, use strlcpy
char * join_null_terminated_arrays(char * a1, char * a2) { // returns null terminated array
	char sizeof0 = BVR_VALUE_CHUNK_SIZE;
	char * a0 = malloc(sizeof(char) * sizeof0);
	char i = 0;
	char position = 0;
	while(a1[i] != 0) {
		a0[position++] = a1[i++];
	}
	i = 0;
	while(a2[i] != 0) {
		a0[position++] = a2[i++];
	}
	a0[position++] = 0;
	return a0;
}
