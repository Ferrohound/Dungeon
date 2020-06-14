#Create two libraries, one called Grid and one called MapGenerator

add_library(Grid STATIC Grid.cpp)
#perlin_noise.cpp should probably be moved, but i'm not sure where to right now...
add_library(MapGenerator STATIC MapGenerator.cpp perlin_noise.cpp)

target_link_libraries(MapGenerator Grid)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (MapGenerator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#[[Have the libraries built placed in lib folder
#LIBRARY_OUTPUT_DIRECTORY doesn't seem to work with static libs
set_target_properties(Grid 
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${library_dir}")
set_target_properties(MapGenerator 
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${library_dir}")]]
