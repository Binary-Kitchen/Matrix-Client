set(LIBMATRIX_SRCS graphics.c graphics.h
                   tools.c tools.h
                   drawing.c drawing.h
                   matrix.c matrix.h)

add_library(libmatrix STATIC ${LIBMATRIX_SRCS})
