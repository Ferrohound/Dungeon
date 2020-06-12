cmake_minimum_required (VERSION 2.6)
project (DG)


add_definitions( -std=c++11 )

file(GLOB SRC_FILES "src/*.cpp" )

include_directories("include")

add_executable(DG ${SRC_FILES})