#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <matrix-client-config.h>

#include "app-common.h"

int main(int argc, char **argv)
{
	int retval = 0;
	if (app_init(argc, argv)) {
		retval = -1;
		goto out;
	}

	if (matrix_cmd(MATRIX_MODE_GRAYSCALE)) {
		retval = -1;
		goto out;
	}

	picture_t *pic = picture_alloc();

	for (;;) {
		int i;
		picture_scroll(pic, SCROLL_UP, NULL);
		for (i = 0; i < 10; i++) {
			unsigned char val = (rand() % 4) ? 0 : 255;
			picture_setPixel(pic, i, 44, val);
		}
		for (i = 10; i < 30; i++) {
			unsigned char val = (rand() % 3) ? 255 : 0;
			picture_setPixel(pic, i, 44, val);
		}
		for (i = 30; i < 40; i++) {
			unsigned char val = (rand() % 4) ? 0 : 255;
			picture_setPixel(pic, i, 44, val);
		}

		int x, y;
		for (x = 0; x < 40; x++) {
			for (y = 0; y < 45; y++) {
				int s, a, b, c, d;

				s = a = b = c = d = 0;

				if (x != 0)
					b = picture_getPixel(pic, x - 1, y);
				if (x != 39)
					a = picture_getPixel(pic, x + 1, y);
				if (y != 44)
					c = picture_getPixel(pic, x, y + 1);
				if (y != 0)
					d = picture_getPixel(pic, x, y - 1);

				s = picture_getPixel(pic, x, y);

				unsigned char medium = (a + b + c + d + s) / 5;

				int decrease = (45 - y);
				if (medium > decrease)
					medium -= decrease / 5;

				if (s > PIX_7 && rand() % 2 == 0)
					medium = s;

				picture_setPixel(pic, x, y, medium);

			}
		}

		matrix_update(pic);
		usleep(10000);
	}

 out:
	app_close();

	return retval;
}
