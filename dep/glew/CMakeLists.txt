file(GLOB_RECURSE sources_glew GL/*.c GL/*.h)
file(GLOB sources_localdir *.c *.h)

set(glew_STAT_SRCS
	${glew_STAT_SRCS}
	${sources_glew}
	${sources_localdir}
)

include_directories(
	${CMAKE_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/dep/glew
	${CMAKE_SOURCE_DIR}/dep/glew/GL
	${CMAKE_CURRENT_SOURCE_DIR}/GL
)

add_library(glew STATIC
	${glew_STAT_SRCS}
)
