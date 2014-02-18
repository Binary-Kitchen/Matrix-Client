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

	const char *text = argv[argc - 1];
	const font_t *font = &font_courier_15;

	picture_t *pic = picture_alloc();

	scrollimage *s = scrollimage_alloc(font_getWidth(text, font));

	font_scrollimage(s, text, (pixel) {
			 0, 15}, font, 0);

	unsigned int i;

	for (i = 0; i < s->width; i++) {
		scrollimage_setPicture(s, pic, i);
		matrix_update(pic);
		usleep(40000);
	}

	scrollimage_free(s);
	picture_free(pic);

 out:
	app_close();

	return retval;
}
