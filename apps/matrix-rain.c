#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "app-common.h"

struct raindrop {
    unsigned char active;
    pixel p;
    unsigned char speed;
    unsigned char brightness;

    unsigned char tick;
};

void drop_init_random(struct raindrop* drop)
{
    drop->p.y = 0;
    drop->p.x = rand() % NUM_COLS;
    drop->speed = rand() % 200;
    drop->tick = 0;
    drop->brightness = rand();
    drop->active = 1;
}

void drop_tick(struct raindrop* drop)
{
    if (++drop->tick == drop->speed) {
        drop->tick = 0;
        drop->p.y++;
        if(drop->p.y > NUM_ROWS)
            drop->active = 0;
    }
}

void drops_to_picture(const struct raindrop* drops, const size_t length, picture_t* pic)
{
    size_t i;
    for (i = 0; i < length; i++) {
        const struct raindrop* drop = drops+i;
        if (drop->active)
            picture_setPixel(pic, drop->p.x, drop->p.y, drop->brightness);
    }
}

int main(int argc, char **argv)
{
    int retval = 0;

    const unsigned char num_drops = 200;
    struct raindrop drops[num_drops];
    picture_t *pic = picture_alloc();

    if (app_init(argc, argv)) {
        retval = -1;
        goto out;
    }

    if (matrix_cmd(MATRIX_MODE_GRAYSCALE)) {
        retval = -1;
        goto out;
    }

    int i;
    for (i=0;i<num_drops;i++)
        drop_init_random(drops+i);

    for(;;) {
        picture_clear(pic);
        for(i = 0; i < num_drops; i++) {
            drop_tick(drops+i);
            if (!drops[i].active)
                drop_init_random(drops+i);
        }
        drops_to_picture(drops, num_drops, pic);
        matrix_update(pic);
        usleep(10000);
    }

out:
    if(pic)
        picture_free(pic);

    app_close();

    return retval;
}
