#ifndef SCROLL_H
#define SCROLL_H

#include "graphics.h"

typedef struct scrollimage_s {
	unsigned char *data;
	unsigned int width;
} scrollimage;

scrollimage *scrollimage_alloc(int width);
void scrollimage_clear(scrollimage * s);
void scrollimage_free(scrollimage * s);

void scrollimage_setPixel(scrollimage * s, unsigned int x, unsigned int y,
			  unsigned char on);
unsigned char scrollimage_getPixel(scrollimage * s, unsigned int x,
				   unsigned int y);

void scrollimage_setPicture(scrollimage * s, picture_t * pic,
			    unsigned int step);

#endif
