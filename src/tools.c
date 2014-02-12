#include <stdio.h>
#include <string.h>

#include "tools.h"

static int frame = 0;

void print_picture(picture_t* pic) {
    unsigned int x,y;
    char line[NUM_COLS*2+2];

    fputs( "\e[32m\e[2J" , stderr);
    fprintf(stderr, "Frame #%d\n", frame++);

    for( y=0; y<NUM_ROWS; y++ ){
        for( x=0; x<NUM_COLS; x++ ){
            line[2*x]   = picture_getPixel( pic, x, y ) ? 'X' : '.';
            line[2*x+1] = 0x20;
        }
        line[2*NUM_COLS] = '\n';
        line[2*NUM_COLS+1] = 0;
        fputs(line, stderr);
    }
}
