if( USE_PCH )
	include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

file(GLOB_RECURSE sources_Logging Logging/*.cpp Logging/*.h)
file(GLOB_RECURSE sources_Shared Shared/*.cpp Shared/*.h)
file(GLOB_RECURSE sources_WorldModelContainer WorldModelContainer/*.cpp WorldModelContainer/*.h)

file(GLOB sources_localdir *.cpp *.h)

if (USE_PCH)
  set(uglengine_PCH_HDR PrecompiledHeaders/clientPCH.h)
  set(uglengine_PCH_SRC PrecompiledHeaders/clientPCH.cpp)
endif()

set(uglengine_SRCS
  ${uglengine_SRCS}
  ${sources_Logging}
  ${sources_Shared}
  ${sources_WorldModelContainer}
  ${sources_localdir}
)

if( WIN32 )
  set(uglengine_SRCS
    ${uglengine_SRCS}
    #${sources_windows_Debugging}
  )
  if ( MSVC )
    set(uglengine_SRCS
      ${uglengine_SRCS}
      uglengine.rc
    )
  endif()
endif()

include_directories(
  	${CMAKE_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/src/UGLEngine/Logging
	${CMAKE_SOURCE_DIR}/src/UGLEngine/WorldModelContainer
	${CMAKE_SOURCE_DIR}/src/UGLEngine/Shared
	${CMAKE_CURRENT_SOURCE_DIR}
	${GLM_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/dep/glfw/include
	${CMAKE_SOURCE_DIR}/dep/glm/glm
	${CMAKE_SOURCE_DIR}/dep/glew
)

add_executable(uglengine
	${uglengine_SRCS}
  	${uglengine_PCH_SRC}
)


add_dependencies(uglengine revision.h)

if( UNIX AND NOT NOJEM AND NOT APPLE )
  set(uglengine_LINK_FLAGS "-pthread -lncurses ${uglengine_LINK_FLAGS}")
endif()

set_target_properties(uglengine PROPERTIES LINK_FLAGS "${uglengine_LINK_FLAGS}")

target_link_libraries(uglengine glfw
  glew
  ${JEMALLOC_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
  ${GLFW_LIBRARIES}
)



if( UNIX )
	install(TARGETS uglengine DESTINATION bin)
elseif( WIN32 )
	install(TARGETS uglengine DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()


if( USE_PCH )
	add_cxx_pch(uglengine ${uglengine_PCH_HDR} ${uglengine_PCH_SRC})
endif()
