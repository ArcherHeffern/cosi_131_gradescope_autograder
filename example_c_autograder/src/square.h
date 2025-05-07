#ifndef __SQUARE_H_
#define __SQUARE_H_

struct Square {
	int width;
};

struct Square* create_square(int width);
void destroy_square(struct Square *square);

int square_area(struct Square *square);

#endif // __SQUARE_H_
