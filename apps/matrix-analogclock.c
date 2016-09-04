#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "app-common.h"

void picture_circleline(picture_t *pic, pixel start, double angle, int r, int on)
{
    int h, x;
    h = (double)r * sin(angle);
    x = (double)r * cos(angle);

    pixel end = (pixel){start.x+x, start.y+h};

    picture_line(pic, start, end, on);
}

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

    pixel center = (pixel){20,22};

    time_t epoch_time;
    struct tm *tm_p;
    epoch_time = time(NULL);


    for (;;) {
        epoch_time = time(NULL);
        tm_p = localtime(&epoch_time);
        int hour, minute, second;

        hour = tm_p->tm_hour;
        minute = tm_p->tm_min;
        second = tm_p->tm_sec;

        if (hour>12) hour-=12;

        picture_circleline(pic, center, M_PI * (((double)hour/12.0)   * 2.0 - 0.5), 10, PIX_6);
        picture_circleline(pic, center, M_PI * (((double)minute/60.0) * 2.0 - 0.5), 15, PIX_5);
        picture_circleline(pic, center, M_PI * (((double)second/60.0) * 2.0 - 0.5), 19, PIX_4);

        picture_circle(pic, center, 20, PIX_7);

        matrix_update(pic);
        picture_clear(pic);
        sleep(1);
    }


    picture_free(pic);

 out:
    app_close();

    return retval;
}
