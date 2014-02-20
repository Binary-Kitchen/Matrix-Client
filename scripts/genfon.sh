#!/bin/bash

FONT=$1
SIZE=$2

FILENAME="font-$FONT-$SIZE.c"

rm $FILENAME;

echo -en "#include <libmatrix/font.h>\n\n" >> $FILENAME;

for i in `seq 32 127`;
do
	HEX=$(printf "%x\n" $i)
	echo -en "\x$HEX" > label.txt
	convert -background white -fill black -font $FONT -pointsize $SIZE "label:@label.txt" $i.png
	convert $i.png -depth 8 $i.gray

	echo -en "static const unsigned char ${FONT}_${SIZE}_${i}[] = {\n" >> $FILENAME;
	cat $i.gray | xxd -i >> $FILENAME;
	echo -en "};\n\n" >> $FILENAME;
done

echo -en "\n\nconst font_t font_${FONT}_${SIZE} = {\n" >> $FILENAME;

for i in `seq 0 31`;
do
	echo -en "    {NULL, 0, 0},\n" >> $FILENAME
done

for i in `seq 32 127`;
do
	convert $i.png -print "    {$FONT\_$SIZE\_$i, %w, %h }, \n" /dev/null >> $FILENAME
done

echo -en "};\n" >> $FILENAME;

#rm *.png
#rm *.gray
#rm label.txt
