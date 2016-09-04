#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

	matrix_update(pic);

	picture_free(pic);

 out:
	app_close();

	return retval;
}
