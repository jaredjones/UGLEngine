if(CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
  add_definitions(-fno-delete-null-pointer-checks)
endif()

add_subdirectory(genrev)
add_subdirectory(UGLEngine)