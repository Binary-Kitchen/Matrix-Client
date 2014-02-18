#ifndef FONT_H
#define FONT_H

#include <stdlib.h>

#include "drawing.h"
#include "scroll.h"

typedef struct character_s {
	const unsigned char *ptr;
	const int width;
	const int height;
} character_t;

typedef character_t font_t[128];

const font_t font_courier_12;
const font_t font_courier_15;
const font_t font_courier_20;
const font_t font_AvantGarde_Book_20;

void font(picture_t * pic, const char *text, pixel init, const font_t * f,
	  int full);
void font_scrollimage(scrollimage * s, const char *text, pixel init,
		      const font_t * f, int full);

int font_getWidth(const char *text, const font_t * f);

#endif
