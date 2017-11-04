#include "graphics.h"
#include "tools.h"

picture_t *picture_alloc(void)
{
	picture_t *ret;

	ret = malloc(sizeof(picture_t));
	if (!ret)
		return NULL;
	picture_clear(ret);

	return ret;
}

picture_t *picture_dup(picture_t * src)
{
	picture_t *dup;

	dup = malloc(sizeof(picture_t));
	if (!dup)
		return NULL;

	return picture_copy(dup, src);
}

void picture_setPixel(picture_t * pic, unsigned int x, unsigned int y,
		      unsigned char on)
{
	if (x < NUM_COLS && y < NUM_ROWS) {
		(*pic)[y * NUM_COLS + x] = on;
	}
}

unsigned char picture_getPixel(picture_t * pic, unsigned int x, unsigned int y)
{
	if (x < NUM_COLS && y < NUM_ROWS) {
		return (*pic)[y * NUM_COLS + x];
	}
	return 255;
}

void picture_scroll(picture_t * pic, unsigned direction,
		    const unsigned char *fillup)
{
	int x, y;
	if (direction == SCROLL_LEFT) {
		for (x = 1; x < NUM_COLS; x++) {
			for (y = 0; y < NUM_ROWS; y++) {
				picture_setPixel(pic, x - 1, y,
						 picture_getPixel(pic, x, y));
			}
		}
		if (fillup) {
			int bit;
			for (y = 0; y < BYTES_ROWS; y++) {
				for (bit = 0; bit < 8; bit++) {
					picture_setPixel(pic, NUM_COLS - 1,
							 y * 8 + bit,
							 fillup[y] & (1 <<
								      bit));
				}
			}
		}
	} else if (direction == SCROLL_RIGHT) {
		for (x = NUM_COLS - 2; x >= 0; x--) {
			for (y = 0; y < NUM_ROWS; y++) {
				picture_setPixel(pic, x + 1, y,
						 picture_getPixel(pic, x, y));
			}
		}
		if (fillup) {
			int bit;
			for (y = 0; y < BYTES_ROWS; y++) {
				for (bit = 0; bit < 8; bit++) {
					picture_setPixel(pic, 0, y * 8 + bit,
							 fillup[y] & (1 <<
								      bit));
				}
			}
		}
	} else if (direction == SCROLL_UP) {
		for (y = 1; y < NUM_ROWS; y++) {
			for (x = 0; x < NUM_COLS; x++) {
				picture_setPixel(pic, x, y - 1,
						 picture_getPixel(pic, x, y));
			}
		}
		if (fillup) {
			int bit;
			for (x = 0; x < BYTES_COLS; x++) {
				for (bit = 0; bit < 8; bit++) {
					picture_setPixel(pic, x * 8 + bit,
							 NUM_ROWS - 1,
							 fillup[x] & (1 <<
								      bit));
				}
			}
		}
	} else if (direction == SCROLL_DOWN) {
		for (y = NUM_ROWS - 2; y >= 0; y--) {
			for (x = 0; x < NUM_COLS; x++) {
				picture_setPixel(pic, x, y + 1,
						 picture_getPixel(pic, x, y));
			}
		}
		if (fillup) {
			int bit;
			for (x = 0; x < BYTES_COLS; x++) {
				for (bit = 0; bit < 8; bit++) {
					picture_setPixel(pic, x * 8 + bit, 0,
							 fillup[x] & (1 <<
								      bit));
				}
			}
		}
	}
}

void picture_raw2pic(picture_t * pic, const unsigned char *raw)
{
	int i;
	for (i = 0; i < 225; i++) {
		unsigned char byte = raw[i];
		unsigned char bit;
		for (bit = 0; bit < 8; bit++) {
			picture_setPixel(pic, 8 * (i % 5) + 7 - bit, i / 5,
					 !(byte & (1 << bit)));
		}
	}
}
