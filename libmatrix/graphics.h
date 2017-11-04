#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include <stdlib.h>
#include <string.h>

#include "matrix_ll.h"

#define PIX_FACTOR ((double)31.875)

#define PIX_0 0
#define PIX_1 32
#define PIX_2 64
#define PIX_3 96
#define PIX_4 128
#define PIX_5 160
#define PIX_6 192
#define PIX_7 224

typedef unsigned char picture_t[PICTURE_SIZE];

picture_t *picture_alloc(void);
picture_t *picture_dup(picture_t *src);

static inline void picture_clear(picture_t *picture)
{
	memset(*picture, 0, sizeof(picture_t));
}

static inline void picture_full(picture_t *picture)
{
	memset(*picture, 0xff, sizeof(picture_t));
}

static inline picture_t *picture_copy(picture_t * dst, picture_t * src)
{
	return memcpy(dst, src, sizeof(picture_t));
}

static inline void picture_free(picture_t *picture)
{
	free(picture);
}

void picture_setPixel(picture_t * picture, unsigned int x, unsigned int y,
		      unsigned char on);
unsigned char picture_getPixel(picture_t * picture, unsigned int x,
			       unsigned int y);

void picture_raw2pic(picture_t * pic, const unsigned char *raw);

#define SCROLL_LEFT 0
#define SCROLL_RIGHT 1
#define SCROLL_UP 2
#define SCROLL_DOWN 3

void picture_scroll(picture_t * pic, unsigned direction,
		    const unsigned char *fillup);

#endif /* GRAPHICS_H */
