#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
	//MC.setPerlin(true);
}

MapGenerator::~MapGenerator()
{

}

Grid* MapGenerator::CreateGrid(int width, int height)
{
	Grid* out = new Grid(width, height, &factory);
	return out;
}

Grid* MapGenerator::GenerateOrganic(int width, int height, int fillPercentage, 
	bool useRandomSeed, int seed, int smoothing, bool connect, Grid* grid)
{
	//if grid is null, create it
	if(!grid)
	{
		grid = CreateGrid(width, height);
	}

	MC.Generate(grid, fillPercentage, useRandomSeed, seed, smoothing, connect);

	return grid;
}

Grid* MapGenerator::GenerateRoom(int width, int height, int fillPercentage,
	bool dense, int minS, int maxS, int numSteps, Grid* grid)
{
	//if grid is null, create it
	if(!grid)
	{
		grid = CreateGrid(width, height);
	}

	RC.Generate(grid, fillPercentage, dense, minS, maxS, numSteps);

	return grid;
}

void MapGenerator::ProcessRooms(Grid* grid, int max, int min, int smoothing)
{
	MC.ProcessRooms(grid, max, min, smoothing);
}