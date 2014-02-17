#ifndef DRAWING_H
#define DRAWING_h

#include "graphics.h"

typedef struct pixel_s {
	int x;
	int y;
} pixel;

void picture_line(picture_t * pic, pixel start, pixel end, int on);
void picture_circle(picture_t * pic, pixel center, unsigned int radius,
		    unsigned int on);

#endif
