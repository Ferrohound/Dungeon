
#pragma once

#include "Grid.h"
#include "perlin_noise.h"

// https://stackoverflow.com/questions/979211/struct-inheritance-in-c#:~:text=Yes.,inheritance%20is%20public%20by%20default.
// numbered tile

// template <> used to indicate explicit template specialization
// template<>
// struct NumTile: public Cell<int>{

// 	NumTile(int _value) : data(_value) {}

// 	void print(std::ostream &output) const { output<<data; }
// };

// https://stackoverflow.com/questions/633634/declaring-class-objects-in-a-header-file

class NumTile : public Cell<int>
{
public:
	NumTile(int d, int x = -1, int y = -1) : Cell<int>(d, x, y) {
		data = d;
	}
	NumTile(int d, vec2 position = {-1, -1}) : Cell<int>(d, position) {
		data = d;
	}

	void print(std::ostream &output) const { output << data; }
};

extern NumTile numtiles[];

class NumTileFactory : public CellFactory<int>
{
public:
	NumTileFactory(int defaultValue) : CellFactory<int>(defaultValue) {}
	NumTileFactory() : CellFactory<int>(0) {}
	// the default tile should be a 0 tile
	NumTile *CreateCell() { return CreateCell(0, 0, 0); }
	NumTile *CreateCell(int x = -1, int y = -1, int number = 0)
	{
		auto cell =  new NumTile(number, x, y);
		return cell;
	}
};

// get the average wall value (reference being the wall tile)
extern int GetAvgAreaValue(Grid<int> *grid, int sx, int sy, int radius);
extern int GetSurroundingWallCount(Grid<int> *grid, int sx, int sy);

extern void RandomFillMap(Grid<int> *grid, bool useRandomSeed, int seed, int fillPercentage, bool debug);
extern void PerlinFillMap(Grid<int> *grid, int seed);