#include "MapSystem.h"

#pragma region ms_methods
//========================MAP SYSTEM METHODS================================

Floor<int> *MapSystem::Generate(int width, int height, int fillPercentage, bool useRandomSeed, int seed, int smoothing, bool connect)
{
	NumTileFactory* f = new NumTileFactory();
	Floor<int> *out = new Floor<int>(width, height, f);

	Generate(out, fillPercentage, useRandomSeed, seed, smoothing, connect);

	return out;
}

// create a floor with the given dimensions and fillPercentage
// randomly generate a room given a fillPercentage, a seed and how much smoothing to use
void MapSystem::Generate(Floor<int> *floor, int fillPercentage, bool useRandomSeed, int seed, int smoothing, bool connect)
{
	cout << "Generating" << std::endl;

	floor->Reset();

	if (!perlin)
	{
		// cout<<"Random Fill"<<std::endl;
		RandomFillMap(floor->grid, useRandomSeed, seed, fillPercentage, debug);
		// cout<<*grid;
	}
	else
	{
		PerlinFillMap(floor->grid, seed);
	}

	for (int i = 0; i < smoothing; i++)
	{
		SmoothMap(floor->grid);
	}

	// cout<<*grid;

	ProcessMap(*floor, connect);

	cout << "Done Generating" << std::endl;
}

// cellular automita; change tile value based on what surrounds it
void MapSystem::SmoothMap(Grid<int> *grid)
{
	// cout<<"smoothing"<<std::endl;
	for (int x = 0; x < grid->GetWidth(); x++)
	{
		for (int y = 0; y < grid->GetHeight(); y++)
		{
			int neighbors = GetSurroundingWallCount(grid, x, y);

			if (neighbors > 4)
			{
				grid->_map[x][y]->data = _tiles[1].data;
			}

			else if (neighbors < 4)
			{
				grid->_map[x][y]->data = _tiles[0].data;
			}
		}
	}
	// cout<<"done smoothing"<<std::endl;
}

// clean up, remove wall regions and room regions that are too small
void MapSystem::ProcessMap(Floor<int> &floor, bool connect)
{
	cout << "Processing" << std::endl;

	// lambda to group all tiles with > 0 as a wall
	//  bool (*general_wall)(const Cell<int>* A, const Cell<int>* B) = NULL;

	auto general_wall = [](const int MapCell, const int CellType) -> bool
	{
		return MapCell > CellType ? true : false;
	};

	cout << "Getting regions." << std::endl;

	vector<vector<vec2>> wallRegions = floor.grid->GetRegions(_tiles->data, general_wall);
	// vector<Region*> remainingSpaces;

	cout << "Regions attained." << std::endl;

	// any region with less than threshold tiles, remove it
	int wallThreshold = 4;
	int id = 0;
	for (int i = 0; i < wallRegions.size(); i++)
	{
		if (wallRegions[i].size() < wallThreshold)
		{
			for (int j = 0; j < wallRegions[i].size(); j++)
			{
				vec2 t = wallRegions[i][j];
				// remember that 1 is space in this for some insane reason
				if (floor.grid->InMapRange(t.x, t.y))
				{
					floor.grid->_map[t.x][t.y] = &_tiles[1];
				}
			}
		}

		else
		{
			continue;
		}
	}
	cout << "Removed Walls" << std::endl;

	cout << *(floor.grid);

	vector<vector<vec2>> roomRegions =
		floor.grid->GetRegions(_tiles[1].data, general_wall);

	// if (debug)
	cout << "Initial number of rooms: " << roomRegions.size() << std::endl;

	int roomThreshold = 4;
	vector<Region<int> *> remainingRooms;
	id = 0;
	for (int i = 0; i < roomRegions.size(); i++)
	{
		// any region with less than threshold tiles, remove it
		if (roomRegions[i].size() < roomThreshold)
		{
			for (int j = 0; j < roomRegions[i].size(); j++)
			{
				vec2 t = roomRegions[i][j];
				floor.grid->_map[t.x][t.y]->data = _tiles[1].data;
			}
		}
		// otherwise create a region with those tiles
		else
		{
			int fillValues[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
			remainingRooms.push_back(new Region<int>(floor.grid->GetCells(roomRegions[i]), floor.grid->GetRegionOutline(roomRegions[i], fillValues)));
			floor.regions.push_back(remainingRooms[id]);
			id++;
		}
	}

	// if (debug)
	cout << "Removed " << roomRegions.size() - remainingRooms.size() << " rooms." << std::endl;

	// error check this to make sure the room's aren't all eliminated
	// remainingRooms.Sort();

	std::sort(remainingRooms.begin(), remainingRooms.end(), Region<int>::CompareRegions);
	if (remainingRooms.size() > 0)
	{
		// TODO: room accessibility stuff
		floor.SetMainRegion(remainingRooms[0]);
		floor.regionAccesibility[remainingRooms[0]] = true;
		// if (debug)
		cout << "Biggest of the remaining rooms=> " << remainingRooms[0]->size << std::endl;
	}

	if (connect)
		ConnectClosestRooms(floor, remainingRooms);

	cout << "Done Processing" << std::endl;
}

void MapSystem::ConnectClosestRooms(Floor<int> &floor, vector<Region<int> *> rooms, bool forceAccessibility)
{

	// cout<<"Connecting rooms.."<<std::endl;
	vector<Region<int> *> roomListA;
	vector<Region<int> *> roomListB;

	// separate the accessible rooms from the non-accessible ones
	if (forceAccessibility)
	{
		for (int i = 0; i < rooms.size(); i++)
		{
			Region<int> *R = rooms[i];
			if (floor.regionAccesibility[R])
			{
				roomListB.push_back(R);
			}
			else
			{
				roomListA.push_back(R);
			}
		}
	}
	else
	{
		roomListA = roomListB = rooms;
	}

	int lowestD = 1000000;

	vec2 bestTileA{0, 0};
	vec2 bestTileB{0, 0};

	Region<int> *bestRoomA;
	Region<int> *bestRoomB;

	bool connectionFound = false;

	// go through every non-accessible room we have to work with
	for (int i = 0; i < roomListA.size(); i++)
	{
		Region<int> *A = roomListA[i];

		if (!forceAccessibility)
		{
			connectionFound = false;
			if (A->connectedRegions.size() > 0)
			{
				continue;
			}
		}

		// for every room that's accessible
		for (int j = 0; j < roomListB.size(); j++)
		{
			Region<int> *B = roomListB[j];

			// if the room is itself or already connected, skip this one
			if (A->id == B->id || A->isConnected(B))
			{
				continue;
			}

			// find the two best tiles and their distance
			for (int tA = 0; tA < A->border.size(); tA++)
			{
				for (int tB = 0; tB < B->border.size(); tB++)
				{
					vec2 tileA = A->border[tA]->pos;
					vec2 tileB = B->border[tB]->pos;

					int distance = (int)(pow((tileA.x - tileB.x), 2) +
										 pow((tileA.y - tileB.y), 2));

					if (distance < lowestD || !connectionFound)
					{
						lowestD = distance;
						connectionFound = true;

						bestTileA = tileA;
						bestTileB = tileB;

						bestRoomA = A;
						bestRoomB = B;
					}
				}
			}
		}

		// if a connection is found and we're not forcing accessibility, connect them
		// and go onto the next room
		if (connectionFound && !forceAccessibility)
			floor.ConnectRegions(bestRoomA, bestRoomB, bestTileA, bestTileB, _tiles[2].data);
		// grid->CreatePassage(bestRoomA, bestRoomB, grid->GetCell(bestTileA.x, bestTileA.y), grid->GetCell(bestTileB.x, bestTileB.y), _tiles[2].data);
	}

	if (connectionFound && forceAccessibility)
	{
		floor.ConnectRegions(bestRoomA, bestRoomB, bestTileA, bestTileB, _tiles[2].data);
		// grid->CreatePassage(bestRoomA, bestRoomB, grid->GetCell(bestTileA.x, bestTileA.y), grid->GetCell(bestTileB.x, bestTileB.y), _tiles[2].data);
		ConnectClosestRooms(floor, rooms, true);
	}

	if (!forceAccessibility)
	{
		ConnectClosestRooms(floor, rooms, true);
	}

	// cout<<"Done Connecting Rooms"<<std::endl;
}

void MapSystem::ProcessRooms(Floor<int> &floor, int max, int min, int smoothing)
{

	int seed = time(NULL);
	srand(seed);

	for (int i = -1; i < smoothing; i++)
	{
		for (int j = 0; j < floor.regions.size(); j++)
		{
			if (i == -1)
			{
				// iterate over every tile in the room and set its value to a random number
				// between the given range
				for (int k = 0; k < floor.regions[j]->cells.size(); k++)
				{
					floor.grid->_map[floor.regions[j]->cells[k]->pos.x][floor.regions[j]->cells[k]->pos.y] =
						&_tiles[(rand() % (max - min) + min)];
				}
			}
			else
			{
				SmoothRoom(floor.grid, floor.regions[j], 1);
			}
		}
	}
}
#pragma endregion ms_methods