cmake_minimum_required(VERSION 2.8)

project(Matrix-Client)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_C_FLAGS_DEBUG "-DDEBUG -ggdb -O0 -Wall -Wpedantic -Wextra -pedantic -Weffc++")

set(MATRIX_SRCS src/graphics.c src/graphics.h
                src/matrix.c src/matrix.h
                src/tools.c src/tools.h
                src/drawing.c src/drawing.h
                src/main.c)


add_executable(Matrix-Client ${MATRIX_SRCS})
target_link_libraries(Matrix-Client m)
