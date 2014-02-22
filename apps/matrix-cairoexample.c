#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <matrix-client-config.h>

#include <cairo/cairo.h>
#include <math.h>

#include "app-common.h"

void cairo2matrix(cairo_surface_t * surface)
{
	int width = cairo_image_surface_get_width(surface);
	int height = cairo_image_surface_get_height(surface);
	int stride = cairo_image_surface_get_stride(surface);
	unsigned char *foo = cairo_image_surface_get_data(surface);
	picture_t *pic = picture_alloc();
	int x, y;
	for (y = 0; y < height; y++) {
		unsigned char *row = foo + y * stride;
		for (x = 0; x < width; x++) {
			uint32_t *p = ((uint32_t *) row) + x;
			picture_setPixel(pic, x, y, (unsigned char)((*p) >> 16));	// select red channel
		}
	}
	matrix_update(pic);
	picture_free(pic);
}

int main(int argc, char **argv)
{
	int retval = 0;
	if (app_init(argc, argv)) {
		retval = -1;
		goto out;
	}

	matrix_cmd(MATRIX_MODE_GRAYSCALE);

	cairo_surface_t *surface;
	cairo_t *cr;

	surface =
	    cairo_image_surface_create(CAIRO_FORMAT_ARGB32, NUM_COLS, NUM_ROWS);
	cr = cairo_create(surface);

	int size = 20;
	int start_x = 10;
	int start_y = 10;

	cairo_set_line_width(cr, 1.3);

	cairo_set_source_rgba(cr, 1.0, 0, 0, 1);
	cairo_rectangle(cr, start_x, start_y, size, size);
	cairo_stroke(cr);

	cairo_set_source_rgba(cr, 0.5, 0, 0, 1);
	cairo_rectangle(cr, start_x + 5, start_y - 5, size, size);
	cairo_stroke(cr);

	cairo_set_source_rgba(cr, 1.0, 0, 0, 1);
	cairo_move_to(cr, start_x, start_y);
	cairo_line_to(cr, start_x + 5, start_y - 5);
	cairo_stroke(cr);

	cairo_set_source_rgba(cr, 1.0, 0, 0, 1);
	cairo_move_to(cr, start_x + size, start_y);
	cairo_line_to(cr, start_x + size + 5, start_y - 5);
	cairo_stroke(cr);

	cairo_set_source_rgba(cr, 0.5, 0, 0, 1);
	cairo_move_to(cr, start_x, start_y + size);
	cairo_line_to(cr, start_x + 5, start_y + size - 5);
	cairo_stroke(cr);

	cairo_set_source_rgba(cr, 1.0, 0, 0, 1);
	cairo_move_to(cr, start_x + size, start_y + size);
	cairo_line_to(cr, start_x + size + 5, start_y + size - 5);
	cairo_stroke(cr);

	cairo2matrix(surface);

	int w = NUM_COLS, h = NUM_ROWS;

	double angle = 0;

	for (;;) {
		cairo_surface_t *s2;
		s2 = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 40, 45);
		cairo_t *cr2 = cairo_create(s2);

		cairo_translate(cr2, w * 0.5, h * 0.5);
		cairo_rotate(cr2, angle);
		cairo_translate(cr2, -w * 0.5, -h * 0.5);

		cairo_set_source_surface(cr2, surface, 0, 0);
		cairo_set_operator(cr2, CAIRO_OPERATOR_SOURCE);
		cairo_paint(cr2);

		cairo2matrix(s2);
		usleep(50000);
		angle += 0.1;

		cairo_destroy(cr2);
		cairo_surface_destroy(s2);
	}

	cairo_destroy(cr);
	cairo_surface_destroy(surface);
 out:
	return retval;
}
