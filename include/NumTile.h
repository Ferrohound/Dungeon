#pragma once

#include "Grid.h"
#include "perlin_noise.h"

//https://stackoverflow.com/questions/979211/struct-inheritance-in-c#:~:text=Yes.,inheritance%20is%20public%20by%20default.
//numbered tile
struct NumTile: Tile{
	
	NumTile(int _value) : value(_value) {} 
	int value;

	friend std::ostream &operator<<( std::ostream &output, const NumTile &T )
	{
		output<<T.value;
		return output;
	}
};

//https://stackoverflow.com/questions/633634/declaring-class-objects-in-a-header-file
extern NumTile numtiles[];

class NumTileFactory: public TileFactory{
	public:
		//the default tile should be a 0 tile
		Tile* CreateTile() { return CreateTile(0); }
		Tile* CreateTile(int number = 0) 
		{ 
			if(number >= 0 && number <=9)
				return &numtiles[number]; 
			else
				return &numtiles[0];
		}
};

//get the average wall value (reference being the wall tile)
extern int GetAvgAreaValue(Grid* grid, int sx, int sy, int radius);
extern int GetSurroundingWallCount(Grid* grid, int sx, int sy);

extern void RandomFillMap(Grid* grid, bool useRandomSeed, int seed, int fillPercentage, bool debug);
extern void PerlinFillMap(Grid* grid, int seed);