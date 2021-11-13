/*
	So this will serve as our hub of sorts for our data types, factories,
	etc.. for this specific 0, 1 map/terrain generator

	We need to define pointers for 3 different types of tiles
	one that is just 0, one that is 1, and one that can have any [0-9] value
	same tile type but different data, or y'know, just have 0-9 for now..

	The two (or more) systems will exist within this generator

	The generator spits out a new grid but has the option to modify
	an existing grid to save on memory
*/
#pragma once

#include "MapSystem.h"
#include "RoomSystem.h"
#include "NumTile.h"

using std::cout;
using std::cin;
using std::endl;

using std::string;

/*
	Map generator should return a grid
*/
class MapGenerator{
	public:
		MapGenerator();
		~MapGenerator();

		Grid* GenerateOrganic(int width, int height, int fillPercentage, 
			bool useRandomSeed, int seed, int smoothing, bool connect, Grid* grid = NULL);
		Grid* GenerateRoom(
			int width, int height, int fillPercentage,
			bool dense, int minS, int maxS, int numSteps, Grid* grid = NULL);
		void ProcessRooms(Grid* grid, int max = 1, int min = 1, int smoothing = 1);
	private:
		Grid* CreateGrid(int width, int height);
		NumTileFactory factory;
		RoomSystem RC = RoomSystem(&numtiles[1], &numtiles[0]);
		MapSystem MC = MapSystem(numtiles);
};
