cmake_minimum_required (VERSION 2.8.12)
project(DungeonGenerator)

#set generator
set(CMAKE_GENERATOR "MSYS Makefiles" CACHE INTERNAL "" #[[FORCE: sets it so that unless you use -G "makefile type" it defaults to this]])

#can set from command line by cmake -D VARNAME=value
set(build_demo TRUE CACHE BOOL "Build demo")
set(library_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib CACHE PATH "Where to put built libraries")

set(DEBUG_BUILD TRUE CACHE BOOL "Debug Build")

message( STATUS ${library_dir} )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${library_dir})

#file(GLOB GRID_SRC "src/*.cpp" )
#file(GLOB GENERATOR_SRC "src/*.cpp" )

#[[
    let's see if this applies to everything..
    this works in later versions of cmake
    set( CMAKE_CXX_STANDARD 11)
]]

#[[
if (WIN32)
    #do something
endif (WIN32)

elseif (UNIX)
    #do something
endif (UNIX)
]]

# if we're in debug mode pass th(e g flag
if(DEBUG_BUILD)
    add_definitions( -g )
endif()

add_definitions( -std=c++11 )
include_directories("include")

add_subdirectory(src)

if(${build_demo})
    add_subdirectory(demo)
endif()