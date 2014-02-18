#include "font.h"

#include "matrix.h"
#include "graphics.h"

void font(picture_t *pic, const char *text, pixel init, const font_t *f, int full) {
    do {
        int x,y;

        character_t foo = (*f)[(unsigned char)*text];

        for(y=0;y<foo.height;y++) {
            for(x=0; x<foo.width;x++) {
                unsigned char b = 255-foo.ptr[y*foo.width+x];
                picture_setPixel(pic, init.x+x, init.y+y, (b && full) ? 255 : b);
            }
        }
        init.x += foo.width;
    } while(*text++);
}

void font_scrollimage(scrollimage *s, const char *text, pixel init, const font_t *f, int full) {
    do {
        int x,y;

        character_t foo = (*f)[(unsigned char)*text];

        for(y=0;y<foo.height;y++) {
            for(x=0; x<foo.width;x++) {
                unsigned char b = 255-foo.ptr[y*foo.width+x];
                scrollimage_setPixel(s, init.x+x, init.y+y, (b && full) ? 255 : b);
            }
        }
        init.x += foo.width;
    } while(*text++);
}

int font_getWidth(const char *text, const font_t *f) {
    int retval = 0;

    do {
        character_t foo = (*f)[(unsigned char)*text];
        retval += foo.width;
    } while(*text++);

    return retval;
}
