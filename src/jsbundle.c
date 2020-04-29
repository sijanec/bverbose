#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <mkdirp.c>
#include <fopenmkdir.c>

int bundle_js(char* bundle_filename, char* js_source_folder_name, char* file_extension) {
	// char bundle_filename[64] = "../dist/js/bundle.js";
	// char js_source_folder_name[64] = "../assets/js/";
	// char file_extension[64] = ".js";
	char source_filename[64];
	FILE * bundle_file;
	FILE * source_file;
	bundle_file = fopen_mkdir(bundle_filename, "w");
	if(bundle_file == NULL) {
		char bundle_folderpath[256];
		strcpy(bundle_folderpath, bundle_filename);
		char *p = strchr(bundle_folderpath, '/');
		if (!p) {
		} else {
			*p = '\0';
		}
		mkdir_p(bundle_filename);
	}
	char copy_buffer;
	DIR *dir;
	struct dirent *ent;
  // printf ("[jsbundle.c] bundle_filename: %s\n", bundle_filename);
	if ((dir = opendir (js_source_folder_name)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
				char *dot = strrchr(ent->d_name, '.');
				if(dot && !strcmp(dot, file_extension)) {
					strcpy(source_filename, js_source_folder_name);
					strcat(source_filename, ent->d_name);
					source_file = fopen(source_filename, "r");
					// printf("[jsbundle.c] source_filename: %s\n", source_filename);
					fprintf(bundle_file, "\n// %s\n", source_filename);
					copy_buffer = fgetc(source_file);
    			while (copy_buffer != EOF) {
        		fputc(copy_buffer, bundle_file);
        		copy_buffer = fgetc(source_file);
    			}
					fclose(source_file);
				}
			}
		}
		closedir (dir);
		fclose(bundle_file);
	} else {
		/* could not open directory */
		return 1;
	}
	return 0;
}
