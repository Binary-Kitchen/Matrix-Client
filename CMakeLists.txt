cmake_minimum_required(VERSION 2.8)

project(Matrix-Client)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_C_FLAGS "-DDEBUG -ggdb -O0 -Wall -Wextra")

set(MATRIX_CLIENT_VERSION_MAJOR "\"0\"")
set(MATRIX_CLIENT_VERSION_MINOR "\"5\"")

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(GIT_BRANCH "\"${GIT_BRANCH}\"")

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(GIT_COMMIT_HASH "\"${GIT_COMMIT_HASH}\"")

configure_file(
  "${PROJECT_SOURCE_DIR}/matrix-client-config.h.in"
  "${PROJECT_BINARY_DIR}/matrix-client-config.h"
)

configure_file(${PROJECT_SOURCE_DIR}/scripts/matrix-pic-url.sh ${PROJECT_BINARY_DIR}/apps/matrix-pic-url.sh COPYONLY)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(apps)
add_subdirectory(libmatrix)
