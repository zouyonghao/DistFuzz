#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main()
{
	char *file_path = "zoo1";
	const char *no_fault_files_env = getenv("NO_FAULT_FILES");
	/** Skip files in get_no_fault_files */
	if (no_fault_files_env != NULL && strlen(no_fault_files_env) > 0) {
		int length = strlen(no_fault_files_env);
		char *no_fault_files = malloc(length + 1);
		strcpy(no_fault_files, no_fault_files_env);
		fprintf(stderr, "no_fault_files is %s\n", no_fault_files);
		const char *delim = ",";
		char *no_fault_file = strtok(no_fault_files, delim);
		while (no_fault_file != NULL) {
			fprintf(stderr, "no_fault_file is %s\n", no_fault_file);
			if (strstr(file_path, no_fault_file) != NULL) {
				fprintf(stderr, "%s is skipped\n", no_fault_file);
				return 0;
			}
			no_fault_file = strtok(NULL, delim);
		}
	}
	return 0;
}
