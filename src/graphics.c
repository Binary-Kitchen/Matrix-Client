#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "graphics.h"


picture_t* picture_alloc() {
    picture_t *foo = (picture_t*)malloc(sizeof(picture_t));
    picture_clear(foo);
    return foo;
}

void picture_free(picture_t* picture) {
    free(picture);
}

void picture_clear(picture_t *picture) {
    memset(*picture, 0, PICTURE_SIZE);
}

void picture_full(picture_t *picture) {
    memset(*picture, 0xff, PICTURE_SIZE);
}

void picture_copy(picture_t *dst, picture_t *src) {
    memcpy(dst, src, sizeof(picture_t) );
}

picture_t* picture_dup(picture_t *src) {
    picture_t *dup = (picture_t*)malloc(sizeof(picture_t));
    picture_copy(dup, src);
    return dup;
}

void picture_setPixel(picture_t *picture, unsigned int x, unsigned int y, unsigned char on) {
    if(x >= NUM_COLS || y >= NUM_ROWS) {
        //fprintf(stderr, "Ey pass auf wo du Pixel setzt: x=%u y=%u\n", x, y);
        return;
    }
   unsigned int x_o = x % 8;
   unsigned int y_o = y % 9;

   unsigned int panel_no = PANEL_NO(x,y);

   unsigned int array_offset = (((x_o * 10) + y_o) * 4) + 4;
   unsigned int bit_offset = panel_no;
   
   array_offset += 3-(panel_no/8);
   bit_offset %= 8;
   if(on)
     *(*picture+array_offset) |= (1<<bit_offset);
   else
     *(*picture+array_offset) &= ~(1<<bit_offset);
}

unsigned int picture_getPixel(picture_t *picture, unsigned int x, unsigned int y) {
   unsigned int x_o = x % 8;
   unsigned int y_o = y % 9;

   unsigned int panel_no = PANEL_NO(x,y);

   unsigned int array_offset = (((x_o * 10) + y_o) * 4) + 4;
   unsigned int bit_offset = panel_no;
   
   array_offset += 3-(panel_no/8);
   bit_offset %= 8;
   
   return *(*picture+array_offset) & (1<<bit_offset);
}

void picture_scroll(picture_t *pic, unsigned direction, const unsigned char* fillup) {
    int x,y;
    if(direction == SCROLL_LEFT) {
        for(x=1 ; x<NUM_COLS ; x++) {
            for(y=0 ; y<NUM_ROWS ; y++) {
                picture_setPixel(pic,x-1,y, picture_getPixel(pic,x,y) );
            }
        }
        if(fillup) {
            int bit;
            for(y=0 ; y<BYTES_ROWS ; y++) {
                for(bit=0 ; bit<8 ; bit++) {
                    picture_setPixel(pic, NUM_COLS-1, y*8+bit, fillup[y] & (1<<bit));
                }
            }
        }
    } else if (direction == SCROLL_RIGHT) {
        for(x=NUM_COLS-2 ; x>=0 ; x--) {
            for(y=0 ; y<NUM_ROWS ; y++) {
                picture_setPixel(pic,x+1,y, picture_getPixel(pic,x,y) );
            }
        }
        if(fillup) {
            int bit;
            for(y=0 ; y<BYTES_ROWS ; y++) {
                for(bit=0 ; bit<8 ; bit++) {
                    picture_setPixel(pic, 0, y*8+bit, fillup[y] & (1<<bit));
                }
            }
        }
    } else if (direction == SCROLL_UP) {
        for(y=1 ; y<NUM_ROWS ; y++) {
            for(x=0 ; x<NUM_COLS ; x++) {
                picture_setPixel(pic,x,y-1, picture_getPixel(pic,x,y) );
            }
        }
        if(fillup) {
            int bit;
            for(x=0 ; x<BYTES_COLS ; x++) {
                for(bit=0 ; bit<8 ; bit++) {
                    picture_setPixel(pic, x*8+bit, NUM_ROWS-1, fillup[x] & (1<<bit));
                }
            }
        }
    } else if (direction == SCROLL_DOWN) {
        for(y=NUM_ROWS-2 ; y>=0 ; y--) {
            for(x=0 ; x<NUM_COLS ; x++) {
                picture_setPixel(pic,x,y+1, picture_getPixel(pic,x,y) );
            }
        }
        if(fillup) {
            int bit;
            for(x=0 ; x<BYTES_COLS ; x++) {
                for(bit=0 ; bit<8 ; bit++) {
                    picture_setPixel(pic, x*8+bit, 0, fillup[x] & (1<<bit));
                }
            }
        }
    }
}

void picture_raw2pic(picture_t *pic, const unsigned char *raw) {
    int i;
    for(i=0 ; i<225 ; i++) {
        unsigned char byte = raw[i];
        unsigned char bit;
        for(bit=0 ; bit<8 ; bit++) {
            picture_setPixel(pic, 8*(i%5)+7-bit, i/5, !(byte & (1<<bit)));
        }
    }
}
