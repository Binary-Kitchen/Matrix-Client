#include <math.h>

#include "drawing.h"
#include "graphics.h"

void picture_line(picture_t * pic, pixel start, pixel end, int on)
{
	float i, j, steigung;
	int tmp;
	unsigned char inv;

	if (start.y == end.y) {
        if(start.x > end.x) {
            pixel tmp = start;
            start = end;
            end = tmp;
        }
		for (tmp = start.x; tmp <= end.x; tmp++)
			picture_setPixel(pic, tmp, start.y, on);
		return;
	}
	if (start.x == end.x) {
        if(start.y > end.y) {
            pixel tmp = start;
            start = end;
            end = tmp;
        }
		for (tmp = start.y; tmp <= end.y; tmp++)
			picture_setPixel(pic, start.x, tmp, on);
		return;
	}

	if (fabs(end.y - start.y) > fabs(end.x - start.x)) {
		inv = 1;
		tmp = start.y;
		start.y = start.x;
		start.x = tmp;
		tmp = end.y;
		end.y = end.x;
		end.x = tmp;
	} else
		inv = 0;

	if ((end.x - start.x) == 0)
		steigung = 0;
	else
		steigung =
		    ((float)(end.y - start.y)) / ((float)(end.x - start.x));

	if (end.x < start.x) {
		tmp = start.y;
		start.y = end.y;
		end.y = tmp;
		tmp = start.x;
		start.x = end.x;
		end.x = tmp;
	}
	for (i = 0; i <= end.x - start.x; i++) {
		j = (start.y + ((i) * steigung)) + 0.5;
		if (inv == 0)
			picture_setPixel(pic, i + start.x, j, on);
		else
			picture_setPixel(pic, j, i + start.x, on);
	}
}

void picture_circle(picture_t * pic, pixel center, double radius,
		    unsigned int on)
{
	double alpha;

	for (alpha = 0.001; alpha < M_PI / 2 - (0.0); alpha += 0.002) {
		double a, h;
		a = radius * sqrt(1.0 - pow(tan(alpha), 2));
		h = sqrt(pow(radius, 2) - pow(a, 2));

		if (!(center.x + (int)a >= NUM_COLS
		      || center.y + (int)h >= NUM_ROWS)) {
			picture_setPixel(pic, center.x + (int)a,
					 center.y + (int)h, on);
		}
		if (!((int)center.y - (int)h < 0
		      || center.x + (int)a >= NUM_COLS)) {
			picture_setPixel(pic, center.x + (int)a,
					 center.y - (int)h, on);
		}
		if (!((int)center.x - (int)a < 0 || (int)center.y - (int)h < 0)) {
			picture_setPixel(pic, center.x - (int)a,
					 center.y - (int)h, on);
		}
		if (!(center.x - (int)a < 0
		      || center.y + (int)h >= NUM_ROWS)) {
			picture_setPixel(pic, center.x - (int)a,
					 center.y + (int)h, on);
		}

	}
}
