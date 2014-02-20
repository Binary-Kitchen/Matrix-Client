#!/bin/bash

while true;
do
	 ffmpeg -f video4linux2 -i /dev/video0 -r 1 -vframes 1 test.jpg
	 convert test.jpg -crop 300x400+180+51 -resize 40x45\! -depth 8 gray:- | ./matrix-grayscale raspberry
	 rm test.jpg
done
