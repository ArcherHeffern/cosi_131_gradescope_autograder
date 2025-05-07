#include "readFile.h"

int countLines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return -1;
    }

    int lineCount = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lineCount++;
        }
    }

    fclose(file);
    return lineCount;
}

int readLines (const char *filename, char **lines, unsigned int* line_sizes, int num_lines){
    FILE *file = fopen(filename, "r");

    char * line = NULL;
    ssize_t read;
    size_t len = 0;
    int i = 0;
    while ((read = getline(&line, &len, file)) != -1) {
	    if (line[read - 1] == '\n') {
	        read--;
    	}

    	line_sizes[i] = read;
    	lines[i] = (char *)malloc(read * sizeof(char));
    	strncpy(lines[i], line, read);
    	i++;
    }

    fclose(file);
    return 0;
}