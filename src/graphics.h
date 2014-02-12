#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include "matrix.h"

picture_t* picture_alloc();
void picture_free(picture_t* picture);
void picture_copy(picture_t *dst, picture_t *src);
picture_t* picture_dup(picture_t *src);

void picture_clear(picture_t *picture);
void picture_full(picture_t *picture);

void picture_setPixel(picture_t *picture, unsigned int x, unsigned int y, unsigned char on);
unsigned int picture_getPixel(picture_t *picture, unsigned int x, unsigned int y);

void picture_raw2pic(picture_t *pic, const unsigned char *raw);

#define SCROLL_LEFT 0
#define SCROLL_RIGHT 1
#define SCROLL_UP 2
#define SCROLL_DOWN 3

void picture_scroll(picture_t *pic, unsigned direction, const unsigned char* fillup);

#endif	/* GRAPHICS_H */

