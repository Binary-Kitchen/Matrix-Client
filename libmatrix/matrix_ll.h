#ifndef MATRIX_LL_H
#define MATRIX_LL_H

#define NUM_PANELS_ROWS 5
#define NUM_PANELS_COLS 5

#define BYTES_PER_PANEL 10
#define BITS_PER_PANEL (BYTES_PER_PANEL*8)
#define NUM_PANELS (NUM_PANELS_ROWS*NUM_PANELS_COLS)

#define NUM_SHIFTERS ((NUM_PANELS+7)/8)

#define NUM_ROWS (NUM_PANELS_ROWS*9)
#define NUM_COLS (NUM_PANELS_COLS*8)

#define PANEL_NO(x,y) ( (x/8) + NUM_PANELS_ROWS*(y/9) )

#define PICTURE_SIZE (NUM_SHIFTERS * BITS_PER_PANEL)

#define BYTES_ROWS ((NUM_ROWS+7)/8)
#define BYTES_COLS ((NUM_COLS+7)/8)

#define MATRIX_PICTURES_PER_FRAME 3

typedef unsigned char ll_picture_t[PICTURE_SIZE];
typedef ll_picture_t ll_frame_t[MATRIX_PICTURES_PER_FRAME];

#endif /* MATRIX_LL_H */
