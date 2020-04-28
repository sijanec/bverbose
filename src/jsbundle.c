#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char* argv[]) {
	char bundle_filename[64] = "../dist/js/bundle.js";
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("../layout/js/")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
				char *dot = strrchr(ent->d_name, '.');
				if(dot && !strcmp(dot, ".js")) {
					printf ("%s\n", ent->d_name);
				}
			}
		}
		closedir (dir);
	} else {
		/* could not open directory */
		return 1;
	}
	return 0;
}
