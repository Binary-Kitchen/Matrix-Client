#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <stdbool.h>

#include "graphics.h"

enum matrix_cmd {
	MATRIX_MODE_MONOCHROME = 1,
	MATRIX_MODE_GRAYSCALE = 2,
	MATRIx_MODE_EXIT = 3,
};

int matrix_init(bool silence, bool develop, const char *server, int port);
void matrix_close(void);

int matrix_cmd(enum matrix_cmd cmd);
void matrix_update(picture_t * pic);

#endif
