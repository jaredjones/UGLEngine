if( CMAKE_COMPILER_IS_GNUCXX )
  add_definitions(--no-warnings)
elseif( MSVC )
  add_definitions(/W0)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  if(CLIENT AND NOT NOJEM)
    add_subdirectory(jemalloc)  
  endif()
endif()

add_subdirectory(glfw)
add_subdirectory(glew)