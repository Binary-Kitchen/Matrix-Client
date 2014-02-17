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

    if (matrix_cmd(MATRIX_MODE_GREYSCALE)) {
        retval = -1;
        goto out;
    }

    picture_t *pic = picture_alloc();
    pixel center;
    center.x = 20;
    center.y = 22;

    pixel r;
    r.x = 0;
    r.y = 0;

    int state = 0;

    int max = 40;
    int direction = 1;

    for(;;) {
        static int radius = 1;
        picture_clear(pic);
        picture_circle(pic, center, radius , PIX_3);
        picture_circle(pic, center, max-radius , PIX_5);

        switch(state) {
            case 0:
                if(++r.x == NUM_COLS) { r.x = NUM_COLS-1; state++; }
                break;
            case 1:
                if(++r.y == NUM_ROWS) { r.y = NUM_ROWS-1; state++; }
                break;
            case 2:
                if(--r.x == -1) { r.x = 0; state++; }
                break;
            case 3:
                if(--r.y == -1) { r.y = 0; state=0; }
                break;
        }

        picture_line(pic, center, r, PIX_4);
        pixel tmp = r;
        tmp.x = 39-tmp.x;
        tmp.y = 44-tmp.y;
        picture_line(pic, center, tmp, PIX_4);
        tmp.x = 39-tmp.x;
        picture_line(pic, center, tmp, PIX_7);
        tmp.x = 39-tmp.x;
        tmp.y = 44-tmp.y;
        picture_line(pic, center, tmp, PIX_7);

        if(direction) {
            if(++radius == max) {
                direction ^= 1;
                radius = max-1;
            }
        } else {
            if(--radius == 0) {
                direction ^= 1;
                radius = 1;
            }
        }

        matrix_update(pic);
        usleep(10000);
    }

    picture_free(pic);

 out:
    app_close();

    return retval;
}
