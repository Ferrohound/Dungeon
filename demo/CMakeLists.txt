# Add executable called "helloDemo" that is built from the source files

#add_definitions( -std=c++11 )
add_executable(DungeonGeneratorDemo main.cpp)

#Link executable to MapGenerator library, since MapGenerator has
#public include directories, we can use those link directories when building
target_link_libraries( DungeonGeneratorDemo LINK_PUBLIC MapGenerator )