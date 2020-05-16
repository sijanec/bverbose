#include <bvr.h>
#include <stdlib.h>
#include <stdio.h>
#include <tape.c>
#include <dirent.h>
#include <string.h>
#include <limits.h>
extern int main(int argc, char* argv[]) {
	if(argc != 3 && argc != 4) {
		printf("usage: %s source-dir/ destination-dir/ [destination .extension=.html]\n", argv[0]);
		return 1;
	}
	char* destination_extension = ".html";
	if(argc == 4) {
		destination_extension = argv[3];
	}
	char* source_folder_name = argv[1];
	char* destination_folder_name = argv[2];
	char* file_extension = ".bvr";
	char* file_extension2 = ".bverbose"; // unofficial
	char source_filename[NAME_MAX];
	char destination_filename[NAME_MAX];
	int response_from_composer;
	char copy_buffer;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (source_folder_name)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
				char *dot = strrchr(ent->d_name, '.');
				if(dot && ( !strcmp(dot, file_extension) || !strcmp(dot, file_extension2) ) ) {
					strcpy(source_filename, source_folder_name);
					strcat(source_filename, ent->d_name);
					strcpy(destination_filename, destination_folder_name);
					strcat(destination_filename, ent->d_name);
					char* temp_pointer = strrchr(destination_filename, '.');
					if (!temp_pointer) {
					} else {
						*temp_pointer = '\0';
					}
					strcat(destination_filename, destination_extension);
					fprintf(stderr, "[compose-all-in-dir.c] %s -> %s ", source_filename, destination_filename);
					fflush(stderr);
					if(bvr_compose_page(source_filename, 0, destination_filename) == SUCCESS) {
						fprintf(stderr, "SUCCESS!\n");
					} else {
						fprintf(stderr, "FAILURE!!!\n");
					}
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

