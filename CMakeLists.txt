project(UGLEngine)

cmake_minimum_required(VERSION 2.8.9)
cmake_policy(SET CMP0005 OLD)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")

if( NOT CMAKE_BUILD_TYPE )
    set(CMAKE_BUILD_TYPE "Release")
endif()

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)

include(cmake/options.cmake)

if ( NOPCH )
	set(USE_PCH 0)
endif()

include(CheckPlatform)

find_package(PCHSupport)
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

if( NOT WITHOUT_GIT )
	find_package(git)
endif()

include(cmake/genrev.cmake)
include(cmake/showoptions.cmake)

add_subdirectory(dep)
add_subdirectory(src)

