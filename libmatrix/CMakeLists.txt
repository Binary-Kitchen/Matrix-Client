set(LIBMATRIX_SRCS graphics.c graphics.h
                   tools.c tools.h
                   drawing.c drawing.h
                   matrix.c matrix.h
                   font.c font.h
                   scroll.h scroll.c

                   fonts/font-courier-12.c
                   fonts/font-courier-15.c
                   fonts/font-courier-20.c
                   fonts/font-AvantGarde-Book-20.c)

add_library(matrix SHARED ${LIBMATRIX_SRCS})

install(TARGETS matrix
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
