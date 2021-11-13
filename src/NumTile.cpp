#include "NumTile.h"

NumTile numtiles[] = {
	NumTile(0),
	NumTile(1),
	NumTile(2),
	NumTile(3),
	NumTile(4),
	NumTile(5),
	NumTile(6),
	NumTile(7),
	NumTile(8),
	NumTile(9),
};

//get how many walls surround a given position,
//in this case "reference" refers to a wall tile
int GetSurroundingWallCount(Grid* grid,int sx, int sy)
{
	int wallCount = 0;
	
	for(int x = sx-1; x <=sx+1; x++)
	{
		for(int y = sy-1; y <=sy+1; y++)
		{
			if(x == sx && y == sy)
				continue;
			//if out of bounds, increase wallcount to encourage the sides to be wall-filled
			if(!grid->InMapRange(x, y))
				wallCount++;
			else
			{
				NumTile* t = (NumTile*)grid->_map[x][y];
				if(t->value != 0)
					wallCount++;
				/*
					rip this shortcut lol
					wallCount+=_map[x][y];
				*/
			}
		}
		
	}
	return wallCount;
}

//get the average value of the surrounding tiles within the given radius
int GetAvgAreaValue(Grid* grid,int sx, int sy, int radius)
{
	//average is the number of walls divided by the total number of spots
	int value = 0;
	int count = 0;
	
	for(int x = sx-radius; x <=sx+radius; x++)
	{
		for(int y = sy-radius; y <=sy+radius; y++)
		{
			if(x == sx && y == sy)
				continue;
			//if out of bounds, increase wallcount to encourage the sides to be wall-filled
			if(!grid->InMapRange(x, y))
				count++;
			else
			{
				count++;
				NumTile* t = (NumTile*)grid->_map[x][y];
				value += t->value;
			}
		}
	}
	
	int avg = value / count;
	return avg;
}

void PerlinFillMap(Grid* grid, int seed)
{
	PerlinNoise p;
	float* tmp = RandomArray(grid->GetHeight() * grid->GetWidth(), seed);

	//this gives us values from 0 to 1 but our map will be looking at 0-9 so let's
	//process it a bit..
	float* pMap = p.noise2D(grid->GetWidth(), grid->GetHeight(),
	tmp, 4);

	for(int i = 0; i < grid->GetHeight() * grid->GetWidth(); i++)
	{
		pMap[i] *= 10;
		if(pMap[i] > 10){
			pMap[i] = 9;
		}

		//may need to invert these..
		int x = i / grid->GetWidth();
		int y = i % grid->GetWidth();

		grid->_map[x][y] = &numtiles[ (int) pMap[i] ];
	}

	// grid->ImportFloor(pMap, grid->GetWidth(), grid->GetHeight());

	delete tmp;
	delete pMap;
}

//randomly fill a map given a fillPercentage
void RandomFillMap(Grid* grid, bool useRandomSeed, int seed, int fillPercentage, bool debug)
{
	
	//cout<<"Filling with random data"<<std::endl;
	if(useRandomSeed)
		//whatever this is in C++
		seed = time(NULL);
	
	srand(seed);
	
	if (debug)
		cout<<"have seed..."<<std::endl;
	
	for(int x = 0; x < grid->GetWidth(); x++)
	{
		for(int y = 0; y < grid->GetHeight(); y++)
		{
			if(x == 0 || x == grid->GetWidth()-1 || y == 0 || y == grid->GetHeight() - 1)
				grid->_map[x][y] = &numtiles[1];
			else
				grid->_map[x][y] = 
					(rand() % 101 < fillPercentage) ? &numtiles[1]:&numtiles[0];
		}
	}		
	//cout<<"done filling"<<std::endl;
	
}