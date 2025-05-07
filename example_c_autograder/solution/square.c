#include "square.h"
#include <stdlib.h>

struct Square* create_square(int width) {
	struct Square* s = malloc(sizeof(struct Square));
	s->width = width;
	return s;
}

void destroy_square(struct Square *square) {
	free(square);
}

int square_area(struct Square *square) {
	return square->width * square->width;
}
