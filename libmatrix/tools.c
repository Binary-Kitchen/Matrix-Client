#include <stdio.h>
#include <string.h>

#include "tools.h"

const char bright0[] = "\e[31m\e[2m.";
const char bright1[] = "\e[31m\e[2mo";
const char bright2[] = "\e[31m\e[2mO";
const char bright3[] = "\e[31m\e[2m0";
const char bright4[] = "\e[31m\e[2mO";
const char bright5[] = "\e[31m\e[1mO";
const char bright6[] = "\e[31m\e[1m0";
const char bright7[] = "\e[31m\e[1mO";

const char *bright_table[] = { bright0,
	bright1,
	bright2,
	bright3,
	bright4,
	bright5,
	bright6,
	bright7
};

void picture_print(picture_t * pic)
{
	char *ptr;

	int x, y;

	// Actually 27045 is enough...
	char buffer[27045 + 3 * 1024];
	ptr = buffer;

	fputs("\e[2J", stdout);

	for (y = 0; y < NUM_ROWS; y++) {
		//ptr = buffer;
		for (x = 0; x < NUM_COLS; x++) {
			int brightness =
			    (double)picture_getPixel(pic, x, y) / PIX_FACTOR;
			if (brightness == 8)
				brightness--;
			int written =
			    sprintf(ptr, "%s\e[0m ", bright_table[brightness]);
			ptr += written;
		}
		ptr += sprintf(ptr, "\n");
	}

	fputs(buffer, stdout);
	fflush(stdout);
}
