#include <stdlib.h>
#include <string.h>

#include "matrix.h"
#include "scroll.h"

scrollimage *scrollimage_alloc(int width)
{
	scrollimage *tmp = malloc(sizeof(scrollimage));

	tmp->width = width;
	tmp->data = malloc(NUM_ROWS * (tmp->width + NUM_COLS));

	scrollimage_clear(tmp);

	return tmp;
}

void scrollimage_clear(scrollimage * s)
{
	bzero(s->data, NUM_ROWS * (s->width + NUM_COLS));
}

void scrollimage_free(scrollimage * s)
{
	free(s->data);
	free(s);
}

void scrollimage_setPixel(scrollimage * s, unsigned int x, unsigned int y,
			  unsigned char on)
{
	if (x < s->width && y < NUM_ROWS) {
		s->data[y * (s->width + NUM_COLS) + x] = on;
	}
}

void scrollimage_setPicture(scrollimage * s, picture_t * pic, unsigned int step)
{
	if (step > s->width)
		return;

	int y;
	for (y = 0; y < NUM_ROWS; y++) {
		unsigned char *line =
		    &s->data[y * (s->width + NUM_COLS) + step];
		memcpy((*pic) + y * NUM_COLS, line, NUM_COLS);
	}
}
