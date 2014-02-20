#!/bin/bash

curl $1 | convert /dev/stdin -colorspace Gray -resize 40x45\! -depth 8 gray:- | ./matrix-grayscale raspberry
