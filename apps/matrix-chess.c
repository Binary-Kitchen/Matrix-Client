#include <math.h>
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

	picture_t *foo = picture_alloc();
	picture_t *bar = picture_alloc();

	int x,y;

	for(y = 0 ; y < 45 ; y++) {
		for(x = 0 ; x < 40 ; x+=2) {
			picture_setPixel(foo,y%2==0?x:x+1,y,1);
			picture_setPixel(bar,y%2==1?x:x+1,y,1);
			matrix_update(foo);
			int waittime = 100000 - y*10000;
			usleep(waittime < 10000 ? 9000 : waittime);
		}
	}

	int i;
        const unsigned int start = 100000;
	int timeval;

        for(i = 0 ; i < 176 ; i++)
        {
        	if(i<60)
	            timeval = (double)start - (double)(start/1000)*pow(i,1.6);
 	        else
        	    timeval = (double)start - (double)(start/1000)*pow(60,1.6) - (double)(start/1000)*pow(i-60,1.2);
		matrix_update(bar);
		usleep(timeval);
		matrix_update(foo);
		usleep(timeval);
	}

	picture_free(bar);
	picture_free(foo);

 out:
	app_close();

	return retval;
}
