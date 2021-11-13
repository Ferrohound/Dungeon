# Simple Procedural room-based Dungeon and organic Map Generator

Based on [this blog post](https://www.gamasutra.com/blogs/AAdonaac/20150903/252889/Procedural_Dungeon_Generation_Algorithm.php), [this reddit post](https://www.reddit.com/r/gamedev/comments/1dlwc4/procedural_dungeon_generation_algorithm_explained/) and [this youtube series](https://www.youtube.com/watch?v=eJEpeUH1EMg)

I highly recommend you take a look at those!

---------------------

## To-do
* Package as library
* Finish debugging
* Do something with the .cc files
* Perlin noise instead of pure random for organic map

## Compilation

Example compilation using MSYS2 MinGW-w64 (note that you need to use the mingw built cmake for the below to work)
```
cmake . -G "MSYS Makefiles" -D build_demo=TRUE
make
```

or enter ./src

```
g++ -c -fPIC grid.cpp -o grid.o -I../include -std=c++11
ar rcs libGrid.a Grid.o
g++ -c -fPIC MapGenerator.cpp perlin_noise.cpp -o MapGenerator.o
g++ -fPIC MapGenerator.cpp perlin_noise.cpp Region.cpp MapSystem.cpp RoomSystem cpp NumTile.cpp -I../include -o generators.o -std=c++11
ar rcs libMapGenerator.a MapGenerator.o
```

enter ./demo

`g++ main.cpp MapGenerator.cpp -I include -o main.exe -std=c++11 -L../lib -lGrid -lMapGenerator`
## Usage

`main.exe --help`

### Flags
* -d or --dimensions [width] [height] : size of map (mandatory)
* -f or --fillpercentage [int] : fill percentage: what percentage to fill the floor up to
* -n or --organic : organic mode. Omit this and the map will be filled room-styled
* -c or --connect : connect. Add this flag to fully connect the map
* -nr or --norandom : Non random seed
* -s or --smoothing [int] : how many smoothing steps to use
* -dn or --dense : Dense version of non-organic map
