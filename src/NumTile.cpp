#include "NumTile.h"

NumTile numtiles[] = {
	NumTile(0, -1, -1),
	NumTile(1, -1, -1),
	NumTile(2, -1, -1),
	NumTile(3, -1, -1),
	NumTile(4, -1, -1),
	NumTile(5, -1, -1),
	NumTile(6, -1, -1),
	NumTile(7, -1, -1),
	NumTile(8, -1, -1),
	NumTile(9, -1, -1),
};

//================= TO DO =======================
// this function needs to be modified to accomidate the new way the grid works
// get average wall value should probably be a separate function to the grid
// get each tile's x,y position and set its height to the average of the surrounding
// tiles.
//
void SmoothRoom(Grid<int> *grid, Region<int> *room, int radius)
{
	int avg = 0;
	for (int k = 0; k < room->cells.size(); k++)
	{
		avg = GetAvgAreaValue(grid, room->cells[k]->pos.x, room->cells[k]->pos.y, radius);
		grid->_map[room->cells[k]->pos.x][room->cells[k]->pos.y]->data = numtiles[avg].data;
	}
}

// get how many walls immediately surround a given position
int GetSurroundingWallCount(Grid<int> *grid, int sx, int sy)
{
	int wallCount = 0;

	for (int x = sx - 1; x <= sx + 1; x++)
	{
		for (int y = sy - 1; y <= sy + 1; y++)
		{
			if (x == sx && y == sy)
				continue;
			// if out of bounds, increase wallcount to encourage the sides to be wall-filled
			if (!grid->InMapRange(x, y))
				wallCount++;
			else
			{
				Cell<int> *t = grid->_map[x][y];
				if (t->data != 0)
				{
					wallCount++;
				}
			}
		}
	}
	return wallCount;
}

// get the average value of the surrounding tiles within the given radius
int GetAvgAreaValue(Grid<int> *grid, int sx, int sy, int radius)
{
	// average is the number of walls divided by the total number of spots
	int value = 0;
	int count = 0;

	for (int x = sx - radius; x <= sx + radius; x++)
	{
		for (int y = sy - radius; y <= sy + radius; y++)
		{
			if (x == sx && y == sy)
				continue;
			// if out of bounds, increase wallcount to encourage the sides to be wall-filled
			if (!grid->InMapRange(x, y))
				count++;
			else
			{
				count++;
				Cell<int> *t = grid->_map[x][y];
				value += t->data;
			}
		}
	}

	int avg = value / count;
	return avg;
}

void PerlinFillMap(Grid<int> *grid, int seed)
{
	PerlinNoise p;
	float *tmp = RandomArray(grid->GetHeight() * grid->GetWidth(), seed);

	// this gives us values from 0 to 1 but our map will be looking at 0-9 so let's
	// process it a bit..
	float *pMap = p.noise2D(grid->GetWidth(), grid->GetHeight(),
							tmp, 4);

	for (int i = 0; i < grid->GetHeight() * grid->GetWidth(); i++)
	{
		pMap[i] *= 10;
		if (pMap[i] > 10)
		{
			pMap[i] = 9;
		}

		// may need to invert these..
		int x = i / grid->GetWidth();
		int y = i % grid->GetWidth();

		grid->_map[x][y] = &numtiles[(int)pMap[i]];
	}

	// grid->ImportFloor(pMap, grid->GetWidth(), grid->GetHeight());

	delete tmp;
	delete pMap;
}

// randomly fill a map given a fillPercentage
void RandomFillMap(Grid<int> *grid, bool useRandomSeed, int seed, int fillPercentage, bool debug)
{

	// cout<<"Filling with random data"<<std::endl;
	if (useRandomSeed)
		// whatever this is in C++
		seed = time(NULL);

	srand(seed);

	if (debug)
		cout << "have seed..." << std::endl;

	for (int x = 0; x < grid->GetWidth(); x++)
	{
		for (int y = 0; y < grid->GetHeight(); y++)
		{
			if (x == 0 || x == grid->GetWidth() - 1 || y == 0 || y == grid->GetHeight() - 1)
				grid->_map[x][y]->data = numtiles[1].data;
			else
				grid->_map[x][y]->data =
					(rand() % 101 < fillPercentage) ? numtiles[1].data : numtiles[0].data;
		}
	}
	// cout<<"done filling"<<std::endl;
}