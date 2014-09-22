#include <stdio.h>
#include <math.h>
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

	/*if (matrix_cmd(MATRIX_MODE_GRAYSCALE)) {
		retval = -1;
		goto out;
	}*/

	picture_t *pic = picture_alloc();

	int i;

	int timeval;

	const unsigned int start = 100000;
	
	for(i = 0 ; i < 176 ; i++)
	{
	  if(i<60)
            timeval = (double)start - (double)(start/1000)*pow(i,1.6);
	  else
            timeval = (double)start - (double)(start/1000)*pow(60,1.6) - (double)(start/1000)*pow(i-60,1.2);

    	  picture_full(pic);
  	  matrix_update(pic);
  	  usleep(timeval);
  	  picture_clear(pic);
  	  matrix_update(pic);
          usleep(timeval);
	}

	picture_free(pic);

 out:
	app_close();

	return retval;
}
