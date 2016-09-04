set(APP_COMMON app-common.c app-common.h)
set(LINK_LIBS matrix m)

set(APPS
	matrix-analogclock
	matrix-blank
	matrix-cairoexample
	matrix-chess
	matrix-circle
	matrix-clock
	matrix-fire
	matrix-fullblink
	matrix-grayscale
	matrix-rain
	matrix-scrolltext
	matrix-template
	matrix-zwickl)

foreach(APP ${APPS})
	add_executable(${APP} ${APP}.c ${APP_COMMON})
	target_link_libraries(${APP} ${LINK_LIBS})
endforeach(APP)

target_link_libraries(matrix-cairoexample ${LINK_LIBS} cairo)
target_link_libraries(matrix-zwickl ${LINK_LIBS} cairo)

install(TARGETS ${APPS} DESTINATION bin/)
