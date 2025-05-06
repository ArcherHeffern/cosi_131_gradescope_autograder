#define MAX_LINE_LENGTH 1024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int countLines(const char *filename);
int readLines (const char *filename, char **lines, unsigned int* line_sizes, int num_lines);