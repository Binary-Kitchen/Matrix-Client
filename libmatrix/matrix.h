#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

#include "graphics.h"

int matrix_init(int silence, int develop, const char *server, int port);

void matrix_close(void);

#define MATRIX_MODE_MONOCHROME 1
#define MATRIX_MODE_GREYSCALE  2
#define MATRIX_EXIT 3
int matrix_cmd(uint32_t cmd);

void matrix_update(picture_t * pic);

#endif
