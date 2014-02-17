set(APP_COMMON app-common.c app-common.h)

set(FIRE_SRCS matrix-fire.c ${APP_COMMON})
set(CIRCLE_SRCS matrix-circle.c ${APP_COMMON})
set(TEMPLATE_SRCS matrix-template.c ${APP_COMMON})

set(LINK_LIBS libmatrix m)

add_executable(matrix-fire ${FIRE_SRCS})
add_executable(matrix-circle ${CIRCLE_SRCS})
add_executable(matrix-template ${TEMPLATE_SRCS})

target_link_libraries(matrix-fire ${LINK_LIBS})
target_link_libraries(matrix-circle ${LINK_LIBS})
target_link_libraries(matrix-template ${LINK_LIBS})
