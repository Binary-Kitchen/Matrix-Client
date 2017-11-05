#include <stdio.h>

#include "graphics.h"
#include "matrix.h"

const char bright0[] = "\e[31m\e[2m.";
const char bright1[] = "\e[31m\e[2mo";
const char bright2[] = "\e[31m\e[2mO";
const char bright3[] = "\e[31m\e[2m0";
const char bright4[] = "\e[31m\e[2mO";
const char bright5[] = "\e[31m\e[1mO";
const char bright6[] = "\e[31m\e[1m0";
const char bright7[] = "\e[31m\e[1mO";

static const char *bright_table[] = {
	bright0,
	bright1,
	bright2,
	bright3,
	bright4,
	bright5,
	bright6,
	bright7,
};

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

void picture_print(picture_t *pic, int m_mode)
{
	char *ptr;
	int x, y, brightness, written;
	char buffer[27045 + 3 * 1024];

	ptr = buffer;
	fputs("\e[2J", stdout);

	for (y = 0; y < NUM_ROWS; y++) {
		for (x = 0; x < NUM_COLS; x++) {
			brightness = picture_getPixel(pic, x, y);

			if (m_mode == MATRIX_MODE_GRAYSCALE)
				brightness = (double)brightness / PIX_FACTOR;
			if (brightness == 8)
				brightness--;
			else
				brightness = brightness ? 7 : 0;

			written = sprintf(ptr, "%s\e[0m ", bright_table[brightness]);
			ptr += written;
		}

		ptr += sprintf(ptr, "\n");
	}

	fputs(buffer, stdout);
	fflush(stdout);
}
