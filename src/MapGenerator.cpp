#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
	//MC.setPerlin(true);
}

MapGenerator::~MapGenerator()
{

}

Grid<int>* MapGenerator::GenerateOrganic(Floor<int>& floor, int fillPercentage, 
	bool useRandomSeed, int seed, int smoothing, bool connect)
{
	// TODO: clean up the memory/type shenanigans
	// floor.Reset();
	// delete floor.grid;
	floor.grid = MC.Generate(floor, floor.width, floor.height, fillPercentage, useRandomSeed, seed, smoothing, connect);

	return floor.grid;
}

Grid<int>* MapGenerator::GenerateRoom(Floor<int>& floor, int fillPercentage,
	bool dense, int minS, int maxS, int numSteps)
{
	// TODO: clean up memory/type shenanigans
	// floor.Reset();
	// delete floor.grid;
	RC.Generate(floor.grid, fillPercentage, dense, minS, maxS, numSteps);

	return floor.grid;
}

void MapGenerator::ProcessRooms(Floor<int>& grid, int max, int min, int smoothing)
{
	MC.ProcessRooms(grid, max, min, smoothing);
}