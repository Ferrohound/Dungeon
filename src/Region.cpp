#include "Region.h"

#pragma region helper_functions
//=============================== HELPER FUNCTIONS ================================
//may remove these from the class and have these by themselves if useful enough


void DrawCircle(Grid* grid, std::pair<int, int> t, int r, Tile* fill)
{
	grid->DrawCircle(t, r, fill);	
}

//get the line of tiles connecting start to end, this is copied from grid
vector<std::pair<int, int>> GetLine(std::pair<int, int> start, std::pair<int, int> end)
{
	//cout<<"Getting Line"<<std::endl;
	vector<std::pair<int, int>> line;
		
	int x = start.first;
	int y = start.second;
	
	int dx = end.first - x;
	int dy = end.second - y;
	
	bool inverted = false;
	
	int step = (dx < 0) ? -1:1;
	int gradientStep = (dy < 0) ? -1:1;
	
	int longest = abs(dx);
	int shortest = abs(dy);
	
	if(longest < shortest)
	{
		inverted = true;
		longest = abs(dy);
		shortest = abs(dx);
		
		step = (dy < 0) ? -1:1;
		gradientStep = (dx < 0) ? -1:1;
	}
	
	int gradientAccumulation = longest/2;
	
	for(int i = 0; i<longest ; i++)
	{
		line.push_back(std::make_pair(x,y));
		//cout<<x<< " " << y <<"=>";
		
		if(inverted)
			y+=step;
		else
			x+=step;
		
		gradientAccumulation += shortest;
		
		if(gradientAccumulation >= longest)
		{
			if(inverted)
				x+=gradientStep;
			else
				y+=gradientStep;
			
			gradientAccumulation-=longest;
		}
	}
	//cout<<std::endl;
	return line;
}

void CreatePassage(Grid* grid, Room* A, Room* B, std::pair<int, int> tA, std::pair<int, int> tB, Tile* fill)
{
	//cout<<"Creating Passage"<<std::endl;
	Room::ConnectRooms(A, B);
	
	vector<std::pair<int, int>> line = GetLine(tA, tB);
	
	for(int i = 0; i < line.size(); i++)
	{
		//===================================== TO DO
		//editing a bit..
		DrawCircle(grid, line[i], 1, fill);
	}
}


void ConnectRooms(Grid* grid, Room* A, Room* B, bool angular, Tile* fill)
{
	int lowestD = (int) (pow((A->border[0].first - B->border[0].first), 2) +
		(pow((A->border[0].second - B->border[0].second), 2)));
	
	std::pair<int, int> bestTileA;
	std::pair<int, int> bestTileB;
	
	for(int tA = 0; tA < A->border.size(); tA ++)
	{
		for(int tB = 0; tB < B->border.size(); tB ++)
		{
			std::pair<int, int> tileA =  A->border[tA];
			std::pair<int, int> tileB =  B->border[tB];
						
			int distance = (int) (pow((tileA.first - tileB.first), 2) + 
				 pow((tileA.second - tileB.second), 2));
						 
			if(distance < lowestD)
			{
				lowestD = distance;
						
				bestTileA = tileA;
				bestTileB = tileB;
			}
		}
	}
	
	//==================================================== TO DO
	//create an angular, maybe even jagged connection
	if(angular)
	{
		//CreatePassage(A, B, bestTileA, bestTileB);
		std::pair<int, int> corner = std::make_pair(/*bestTileA.x - */bestTileB.first, bestTileA.second/* - bestTileB.y*/);
		vector<std::pair<int, int>> line1 = GetLine(bestTileA, corner);
		vector<std::pair<int, int>> line2 = GetLine(corner, bestTileB);

		for(int i = 0; i < line1.size(); i++)
		{
			//===================================== TO DO
			DrawCircle(grid, line1[i], 1, fill);
		}

		for(int i = 0; i < line2.size(); i++)
		{
			//===================================== TO DO
			DrawCircle(grid, line2[i], 1, fill);
		}

		Room::ConnectRooms(A, B);

	}
	//otherwise, follow gradient
	else
	{
		CreatePassage(grid, A, B, bestTileA, bestTileB, fill);
	}
}

//================= to do =======================
//just iterate over the tiles and draw them
void AddRoom(Grid* grid, Room* room, Tile* fill)
{
	for (auto& tile : room->tiles)
	{
		
		if(grid->InMapRange(tile.first, tile.second))
			grid->_map[tile.first][tile.second] = fill;
	}
	//might relocate this to the system itself
	//_rooms.push_back(room);
}
#pragma endregion helper_functions

#pragma region room_methods
//======================ROOM METHODS==================================

Room::Room()
{
	accessible = false;
	mainRoom = false;

	size = tiles.size();
	connectedRooms.clear();
	border.clear();
}

Room::Room(int x, int y, int width, int height, vector< vector<Tile*> > map, Tile* fill )
{
	tiles.clear();
	
	//something about tiles is getting corrupted, would be nice to not use pointers
	//pushback tiles from that region
	for(int i = x; i < x+width; i++)
	{
		for(int j = y; j < y+height; j++)
		{
			tiles.push_back(std::make_pair(i, j));
		}
	}

	mX = x + width/2;
	mY = y + height/2;

	//cout<<"Room midpoint is "<<mX<<" "<<mY<<std::endl;
	
	size = tiles.size();
	connectedRooms.clear();
	border.clear();
	
	//create the border tiles list
	for(int i = 0; i < size ; i++)
	{
		for(int x = tiles[i].first - 1; x < tiles[i].first +1 ; x++)
		{
			for(int y = tiles[i].second - 1; y < tiles[i].second + 1 ; y++)
			{
				if((x == tiles[i].first || y == tiles[i].second) && map[x][y] == fill)
				{
					border.push_back(tiles[i]);
				}
			}
		}
	}
}

Room::Room(vector<std::pair<int, int>> _tiles, vector< vector<Tile*> > map, Tile* fill, int _id)
{
	accessible = false;
	mainRoom = false;
	
	id = _id;
	
	tiles = _tiles;
	size = tiles.size();
	connectedRooms.clear();
	border.clear();
	
	//create the border tiles list
	for(int i = 0; i < size ; i++)
	{
		for(int x = tiles[i].first - 1; x < tiles[i].first +1 ; x++)
		{
			for(int y = tiles[i].second - 1; y < tiles[i].second + 1 ; y++)
			{
				if((x == tiles[i].first || y == tiles[i].second) && map[x][y] == fill)
				{
					border.push_back(tiles[i]);
				}
			}
		}
	}
}

Room::~Room()
{
	
}

void Room::SetAccessibleFromMainRoom()
{
	if(!accessible)
	{
		accessible = true;
		for(int i = 0; i < connectedRooms.size(); i++)
		{
			connectedRooms[i]->SetAccessibleFromMainRoom();
		}
	}
}

void Room::ConnectRooms(Room* A, Room* B)
{
	if(A->accessible)
	{
		B->SetAccessibleFromMainRoom();
	}
	if(B->accessible)
	{
		A->SetAccessibleFromMainRoom();
	}
	
	A->connectedRooms.push_back(B);
	B->connectedRooms.push_back(A);
}

//this is a direct connection
bool Room::isConnected(Room* B)
{
	for(int i = 0; i < connectedRooms.size(); i++)
	{
		if(connectedRooms[i]->id == B->id)
			return true;
	}
	//return (std::find(connectedRooms.begin(), connectedRooms.end(), B) != connectedRooms.end());
	return false;
}

int Room::CompareRooms(Room* A, Room* B)
{
	return A->size < B->size;
}

bool Room::operator==(const Room &other) const 
{
	return id == other.id;
}

#pragma endregion room_methods