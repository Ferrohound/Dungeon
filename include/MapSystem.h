/*
	MapSystem creates a more organic kind of map
		First we fill thhe map up with a random amount of noise

		We then do several steps of smoothing by going through every tile
		and changing its value based on what surrounds it

		Next, we process the map by getting the wall regions
		Any region with less than the threshold number of tiles in it is removed

		Same goes for the spaces

		The regions that aren't removed are added to a vector, which is then sorted
		by the size in descending order: The biggest room is our main room and it is
		set to accessible

		We then go about connecting the closest rooms via a recursive function that
		takes the grid, a list of rooms and a bool asking whether or not to force
		accessibility

		initially our list of rooms is every other room we have and accessibility
		is set to false

		We're looking for the two best rooms to connect and the two best tiles
		one in each room to connect them through so we iterate over every combination
		of unconnected rooms and pick the one with the smallest distance between them

		Continue doing this until all rooms are accessible

	In all honesty, the map system could probably use the same MST method but it
	wouldn't look as natural
*/

#pragma once

#include <unordered_map>

#include "Floor.h"
#include "NumTile.h"

using std::cin;
using std::cout;
using std::endl;

using std::queue;
using std::string;
using std::vector;

// external helper functions inside of MapGenerator
extern void RandomFillMap(Grid<int> *grid, bool useRandomSeed, int seed, int fillPercentage, bool debug);

extern void PerlinFillMap(Grid<int> *grid, int seed);

extern void SmoothRoom(Grid<int> *grid, Region<int> *room, int radius);

//======================================================================
// To create a map
class MapSystem
{
public:
	MapSystem(Cell<int> *tiles) : _tiles(tiles)
	{
		if (tiles)
			tileCount = sizeof(tiles) / sizeof(*tiles);
		else
			tileCount = 0;

		f = new NumTileFactory();
		debug = false;
		perlin = false;
	}

	~MapSystem() { delete f; }

	void setDebug(bool newval) { debug = newval; }
	void setPerlin(bool newval) { perlin = newval; }

	void Generate(Floor<int> *floor, int fillPercentage = 30, bool useRandomSeed = true, int seed = 10, int smoothing = 2, bool connect = true);

	Floor<int> *Generate(int width, int height, int fillPercentage = 30, bool useRandomSeed = true, int seed = 10, int smoothing = 2, bool connect = true);

	// remove a wall or room region that is below a certain threshold and connect
	// rooms
	void ProcessMap(Floor<int> &floor, bool connect = true);
	void SmoothMap(Grid<int> *grid);

	// modify rooms to make them more mountainous, etc..
	void ProcessRooms(Floor<int> &floor, int max = 1, int min = 1, int smoothing = 1);

	void ConnectClosestRooms(Floor<int> &floor, vector<Region<int> *> rooms, bool forceAccessibility = false);
	// void ConnectRooms(Grid* grid, Room* A, Room* B, bool angular = false, int fill = 2);
	// void CreatePassage(Grid* grid, Room* A, Room* B, Tile tA, Tile tB, int fill = 2);
	// void DrawCircle(Grid* grid, Tile t, int r, int fill = 2);

	// vector<Tile> GetLine(Tile start, Tile end);

private:
	bool debug, perlin;
	Cell<int> *_tiles;
	int tileCount;
	NumTileFactory *f;
};
