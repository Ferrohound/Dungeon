#include "MapSystem.h"

#pragma region ms_methods
//========================MAP SYSTEM METHODS================================

//create a floor with the given dimensions and fillPercentage
//randomly generate a room given a fillPercentage, a seed and how much smoothing to use
void MapSystem::Generate(Grid* grid, int fillPercentage, bool useRandomSeed, int seed, int smoothing, bool connect)
{
	cout<<"Generating"<<std::endl;
	
	grid->Clear();
	_rooms.clear();
	_spaces.clear();
	
	/*for(int i = 0; i < grid->GetWidth(); i++)
	{
		vector<int>tmp(grid->GetHeight());
		grid->_map.push_back(tmp);
	}*/
	
	if(!perlin)
		RandomFillMap(grid, useRandomSeed, seed, fillPercentage, debug);
	else
	{
		PerlinFillMap(grid, seed);
	}
	
	
	for(int i = 0; i < smoothing; i++)
	{
		SmoothMap( grid );
	}
	
	ProcessMap(grid, connect);
	
	cout<<"Done Generating"<<std::endl;
}

//cellular automita; change tile value based on what surrounds it
void MapSystem::SmoothMap(Grid* grid)
{
	//cout<<"smoothing"<<std::endl;
	for(int x = 0; x < grid->GetWidth(); x++)
	{
		for(int y = 0; y < grid->GetHeight(); y++)
		{
			int neighbors = GetSurroundingWallCount(grid, x, y);
			
			if(neighbors > 4)
			{
				grid->_map[x][y] = &_tiles[1];
			}
			
			else if(neighbors < 4)
			{
				grid->_map[x][y] = 0;
			}
		}
	}
	//cout<<"done smoothing"<<std::endl;
	
}

//clean up, remove wall regions and room regions that are too small
void MapSystem::ProcessMap(Grid* grid, bool connect)
{
	cout<<"Processing"<<std::endl;

	//lambda to group all tiles with > 0 as a wall
	auto general_wall = [](Tile* A, Tile* B) -> bool {

			NumTile* a = (NumTile*)A;
			NumTile* b = (NumTile*)B;

			if(a->value > 0)
			{
				if(b->value > 0)
					return true;
				else
					return false;
			}
			else
			{
				if(b->value == 0)
					return true;
				else
					return false;
			}
		};
	
	vector< vector<std::pair<int, int>> > wallRegions = 
		grid->GetRegions(&_tiles[1], general_wall);
	vector<Region*> remainingSpaces;
		
	//any region with less than threshold tiles, remove it
	int wallThreshold = 4;
	int id = 0;
	for(int i=0 ; i < wallRegions.size(); i++)
	{
		if(wallRegions[i].size() < wallThreshold)
		{
			for(int j = 0; j < wallRegions[i].size(); j++)
			{
				std::pair<int, int> t = wallRegions[i][j];
				grid->_map[t.first][t.second] = &_tiles[0];
			}
		}
		
		else
		{
			/*cout<<"Dealing with the else"<<std::endl;
			remainingSpaces.push_back(new Room(wallRegions[i], _map, id));
			_spaces.push_back(remainingSpaces[id]);
			id++;
			cout<<"Done Dealing with the else"<<std::endl;*/
			continue;
		}
	}
	cout<<"Removed Walls"<<std::endl;
		
	vector< vector<std::pair<int, int>> > roomRegions = 
		grid->GetRegions(&_tiles[1], general_wall);
	
	if (debug)
		cout<<"Initial number of rooms: "<<roomRegions.size() << std::endl;
		
	//any region with less than threshold tiles, remove it
	int roomThreshold = 4;
	vector<Region*> remainingRooms;
	id = 0;
	for(int i=0 ; i < roomRegions.size(); i++)
	{
		if(roomRegions[i].size() < roomThreshold)
		{
			for(int j = 0; j < roomRegions[i].size(); j++)
			{
				std::pair<int, int> t = roomRegions[i][j];
				grid->_map[t.first][t.second] = &_tiles[1];
			}
		}
		else
		{
			remainingRooms.push_back(new Region(roomRegions[i], grid->_map, &_tiles[1], id));
			_rooms.push_back(remainingRooms[id]);
			id++;
		}
	}
	
	if (debug)
		cout<<"Removed "<<roomRegions.size() - remainingRooms.size() << " rooms." << std::endl;

		
	//error check this to make sure the room's aren't all eliminated
	//remainingRooms.Sort();
	
	std::sort (remainingRooms.begin(), remainingRooms.end(), Region::CompareRegions);
	if(remainingRooms.size() > 0)
	{
		remainingRooms[0]->mainRegion = true;
		remainingRooms[0]->accessible = true;
		if (debug)
			cout<<"Biggest of the remaining rooms=> " << remainingRooms[0]->size<<std::endl;
	}
	
	if(connect)
		ConnectClosestRooms(grid, remainingRooms);
	
	cout<<"Done Processing"<<std::endl;
}

void MapSystem::ConnectClosestRooms(Grid* grid, vector<Region*> rooms, bool forceAccessibility)
{
	
	//cout<<"Connecting rooms.."<<std::endl;
	vector<Region*> roomListA;
	vector<Region*> roomListB;

	//separate the accessible rooms from the non-accessible ones		
	if(forceAccessibility)
	{
		for(int i = 0; i < rooms.size(); i++)
		{
			Region* R = rooms[i];
			if(R->accessible)
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
	
	std::pair<int, int> bestTileA;
	std::pair<int, int> bestTileB;
	
	Region* bestRoomA = new Region();
	Region* bestRoomB = new Region();
		
	bool connectionFound = false;
	
	//go through every non-accessible room we have to work with
	for(int i = 0; i < roomListA.size(); i++)
	{
		Region* A = roomListA[i];

		if(!forceAccessibility)
		{
			connectionFound = false;
			if(A->connectedRegions.size() > 0)
			{
				continue;
			}
		}
		
		//for every room that's accessible
		for(int j = 0; j < roomListB.size(); j++)
		{
			Region* B = roomListB[j];
			
			//if the room is itself or already connected, skip this one
			if(A->id == B->id || A->isConnected(B))
			{
				continue;
			}
			
			//find the two best tiles and their distance
			for(int tA = 0; tA < A->border.size(); tA ++)
			{
				for(int tB = 0; tB < B->border.size(); tB ++)
				{
					std::pair<int, int> tileA =  A->border[tA];
					std::pair<int, int> tileB =  B->border[tB];
						
					int distance = (int) (pow((tileA.first - tileB.first), 2) + 
						 pow((tileA.second - tileB.second), 2));
						 
					if(distance < lowestD  || !connectionFound)
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
		
		//if a connection is found and we're not forcing accessibility, connect them
		//and go onto the next room
		if(connectionFound && !forceAccessibility)
			CreatePassage(grid, bestRoomA, bestRoomB, bestTileA, bestTileB, &_tiles[2]);
	}
		
		
	if(connectionFound && forceAccessibility)
	{
		CreatePassage(grid, bestRoomA, bestRoomB, bestTileA, bestTileB, &_tiles[2]);
		ConnectClosestRooms(grid, rooms, true);
	}
	
	if(!forceAccessibility)
	{
		ConnectClosestRooms(grid, rooms, true);
	}
	
	//cout<<"Done Connecting Rooms"<<std::endl;
}

void MapSystem::ProcessRooms(Grid* grid, int max, int min, int smoothing)
{
	
	int seed = time(NULL);
	srand(seed);
	
	for(int i = -1; i < smoothing; i++)
	{
		for(int j = 0; j < _rooms.size(); j++)
		{
			if(i == -1)
			{
				//iterate over every tile in the room and set its value to a random number
				//between the given range
				for(int k = 0; k < _rooms[j]->tiles.size(); k++)
				{
					grid->_map[_rooms[j]->tiles[k].first][_rooms[j]->tiles[k].second] = 
						&_tiles[(rand() % (max - min) + min)];
				}
			}
			else
			{
				SmoothRoom(grid, _rooms[j], 1);
			}
		}
	}
}
#pragma endregion ms_methods