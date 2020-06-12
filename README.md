# Simple Procedural room-based Dungeon and organic Map Generator

Loosely based on [this blog post](https://www.gamasutra.com/blogs/AAdonaac/20150903/252889/Procedural_Dungeon_Generation_Algorithm.php) and [this reddit post](https://www.reddit.com/r/gamedev/comments/1dlwc4/procedural_dungeon_generation_algorithm_explained/)

---------------------

## To-do
* Package as library
* Finish debugging
* Do something with the .cc files

## Compilation

`CMake

or

`g++ main.cpp floor.cpp BSP.cpp -I include -o main.exe -std=c++11

## Usage

`main.exe --help

### Flags
* -d or --dimensions [width] [height] : size of map (mandatory)
* -f or --fillpercentage [int] : fill percentage: what percentage to fill the floor up to
* -n or --organic : organic mode. Omit this and the map will be filled room-styled
* -c or --connect : connect. Add this flag to fully connect the map
* -nr or --norandom : Non random seed
* -s or --smoothing [int] : how many smoothing steps to use
* -dn or --dense : Dense version of non-organic map