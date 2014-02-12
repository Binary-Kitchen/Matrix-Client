#include <string.h>
#include <stdio.h>

#include <unistd.h>

#include "matrix.h"
#include "tools.h"

frame_t cur_frame;

void matrix_set_frame(frame_t *frame) {
    memcpy(cur_frame, *frame, sizeof(frame_t));
}

void matrix_update(picture_t *picture) {
    print_picture(picture);
    write(1, *picture, sizeof(picture_t));
}
