#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
	//MC.setPerlin(true);
}

MapGenerator::~MapGenerator()
{

}

Grid<int>* MapGenerator::CreateGrid(int width, int height)
{
	Grid<int>* out = new Grid<int>(width, height, &factory);
	return out;
}

Grid<int>* MapGenerator::GenerateOrganic(int width, int height, int fillPercentage, 
	bool useRandomSeed, int seed, int smoothing, bool connect, Grid<int>* grid)
{
	//if grid is null, create it
	if(!grid)
	{
		grid = CreateGrid(width, height);
	}

	return MC.Generate(width, height, fillPercentage, useRandomSeed, seed, smoothing, connect);
}

Grid<int>* MapGenerator::GenerateRoom(int width, int height, int fillPercentage,
	bool dense, int minS, int maxS, int numSteps, Grid<int>* grid)
{
	//if grid is null, create it
	if(!grid)
	{
		grid = CreateGrid(width, height);
	}

	return RC.Generate(width, height, fillPercentage, dense, minS, maxS, numSteps);
}

void MapGenerator::ProcessRooms(Grid<int>* grid, int max, int min, int smoothing)
{
	MC.ProcessRooms(grid, max, min, smoothing);
}