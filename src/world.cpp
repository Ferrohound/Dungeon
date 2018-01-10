#include "world.h"

//=======================TILE========================================

Tile::Tile()
{
	x = 0;
	y = 0;
	pos = Vector2 { 0, 0 };
}

Tile::Tile(Vector2 position)
{
	pos = position;
	x = pos.x;
	y = pos.y;
}

Tile::Tile(int _x, int _y)
{
	x = _x;
	y = _y;
	
	pos = Vector2{ x, y };
}

Tile::~Tile()
{
	
}

//======================ROOM METHODS==================================

Room::Room()
{
	accessible = false;
	mainRoom = false;

	size = tiles.size();
	connectedRooms.clear();
	border.clear();
}
Room::Room(vector<Tile> _tiles, vector< vector<int> > map, int _id)
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
		for(int x = tiles[i].x - 1; x < tiles[i].x +1 ; x++)
		{
			for(int y = tiles[i].y - 1; y < tiles[i].y + 1 ; y++)
			{
				if((x == tiles[i].x || y == tiles[i].y) && map[x][y] == 1)
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


//========================FLOOR METHODS================================
bool Floor::InMapRange(int x, int y)
{
	return x >= 0 && x < _width && y >= 0 && y < _height;
}

//create a floor with the given dimensions and fillPercentage
Floor::Floor(int width, int height, int fillPercentage, bool useRandom) : _width(width), _height(height)
{	
	Generate(fillPercentage, useRandom, 0, 5);
}

//randomly generate a room given a fillPercentage, a seed and how much smoothing to use
void Floor::Generate(int fillPercentage, bool useRandomSeed, int seed, int smoothing)
{
	cout<<"Generating"<<std::endl;
	
	_map.clear();
	
	for(int i = 0; i < _height; i++)
	{
		vector<int>tmp(_width);
		_map.push_back(tmp);
	}
	
	RandomFillMap(useRandomSeed, seed, fillPercentage);
	
	for(int i = 0; i < smoothing; i++)
	{
		SmoothMap();
	}
	
	ProcessMap();
	
	cout<<"Done Generating"<<std::endl;
}

//randomly fill a map given a fillPercentage
void Floor::RandomFillMap(bool useRandomSeed, int seed, int fillPercentage)
{
	//cout<<"Filling with random data"<<std::endl;
	if(useRandomSeed)
		//whatever this is in C++
		seed = time(NULL);
	
	srand(seed);
	
	cout<<"have seed..."<<std::endl;
	
	for(int x = 0; x < _width; x++)
	{
		for(int y = 0; y < _height; y++)
		{
			if(x == 0 || x == _width-1 || y == 0 || y == _height - 1)
				_map[x][y] = 1;
			else
				_map[x][y] = (rand() % 101 < fillPercentage) ? 1:0;
		}
	}		
	//cout<<"done filling"<<std::endl;
}

//cellular automita; change tile value based on what surrounds it
void Floor::SmoothMap()
{
	//cout<<"smoothing"<<std::endl;
	for(int x = 0; x < _width; x++)
	{
		for(int y = 0; y < _height; y++)
		{
			int neighbors = GetSurroundingWallCount(x, y);
			
			if(neighbors > 4)
			{
				_map[x][y] = 1;
			}
			
			else if(neighbors < 4)
			{
				_map[x][y] = 0;
			}
		}
	}
	//cout<<"done smoothing"<<std::endl;
	
}

//clean up, remove wall regions and room regions that are too small
void Floor::ProcessMap()
{
	//cout<<"Processing"<<std::endl;
	
	vector< vector<Tile> > wallRegions = GetRegions(1);
		
	//any region with less than threshold tiles, remove it
	int wallThreshold = 4;
	for(int i=0 ; i < wallRegions.size(); i++)
	{
		if(wallRegions[i].size() < wallThreshold)
		{
			for(int j = 0; j < wallRegions[i].size(); j++)
			{
				Tile t = wallRegions[i][j];
				_map[t.x][t.y] = 0;
			}
		}
	}
	
	//cout<<"Removed Walls"<<std::endl;
		
	vector< vector<Tile> > roomRegions = GetRegions(0);
	
	//cout<<"Initial number of rooms: "<<roomRegions.size() << std::endl;
		
	//any region with less than threshold tiles, remove it
	int roomThreshold = 4;
	vector<Room*> remainingRooms;
	int id = 0;
	for(int i=0 ; i < roomRegions.size(); i++)
	{
		if(roomRegions[i].size() < roomThreshold)
		{
			for(int j = 0; j < roomRegions[i].size(); j++)
			{
				Tile t = roomRegions[i][j];
				_map[t.x][t.y] = 1;
			}
		}
		else
		{
			remainingRooms.push_back(new Room(roomRegions[i], _map, id));
			id++;
		}
	}
	
	cout<<"Removed "<<roomRegions.size() - remainingRooms.size() << " rooms." << std::endl;
		
	//error check this to make sure the room's aren't all eliminated
	//remainingRooms.Sort();
	
	//std::sort (remainingRooms.begin(), remainingRooms.end(), Room::CompareRooms);
	if(remainingRooms.size() > 0)
	{
		remainingRooms[0]->mainRoom = true;
		remainingRooms[0]->accessible = true;
		
		ConnectClosestRooms(remainingRooms);
	}
	
	//cout<<"Done Processing"<<std::endl;
}

//floodfill to get all tiles in the region
vector<Tile> Floor::GetRegionTiles(int sx, int sy)
{
	vector<Tile> tiles;
		
	//check if tile was looked at yet
	//need to initialize each value, however, look at alternatives
	int flags[_width][_height];
	for(int i = 0; i < _width; i++)
	{
		for(int j = 0; j < _height; j++)
		{
			flags[i][j] = 0;
		}
	}
	
	int tileType = _map[sx][sy];
	
	queue<Tile*> q;
	
	q.push(new Tile(sx,sy));
	flags[sx][sy] = 1;
	
	//breadth first search across tiles
	while(!q.empty())
	{
		Tile* t = q.front();
		q.pop();
		tiles.push_back(*t);
		
		for(int x = t->x-1; x <= t->x+1; x++)
		{
			for(int y = t->y-1; y <= t->y + 1; y++)
			{
				if(InMapRange(x, y) && (y == t->y || x == t->x) && flags[x][y] == 0 
					&& tileType == _map[x][y])
				{
					flags[x][y] = 1;
					q.push(new Tile(x, y));
				}
			}
		}
	}
	
	return tiles;
}

vector< vector<Tile> > Floor::GetRegions(int tileType)
{
	//cout<<"Getting Regions"<<std::endl;
	vector< vector<Tile> > regions;
		
	//check if tile was looked at yet
	int flags[_width][_height];
		
	for(int x = 0; x < _width; x++)
	{
		for(int y = 0; y < _height; y++)
		{
			if(flags[x][y] == 0 && _map[x][y] == tileType)
			{
				vector<Tile> newRegion = GetRegionTiles(x, y);
				regions.push_back(newRegion);
				
				for(int i = 0 ; i < newRegion.size(); i++)
				{
					Tile t = newRegion[i];
					flags[t.x][t.y] = 1;
				}
			}
		}
	}
	//cout<<"Done Getting " << regions.size()<<" Regions"<<std::endl;
	return regions;
}

//get how many walls surround a given position
int Floor::GetSurroundingWallCount(int sx, int sy)
{
	int wallCount = 0;
	
	for(int x = sx-1; x <=sx+1; x++)
	{
		for(int y = sy-1; y <=sy+1; y++)
		{
			if(x == sx && y == sy)
				continue;
			//if out of bounds, increase wallcount to encourage the sides to be wall-filled
			if(!InMapRange(x, y))
				wallCount++;
			else
				wallCount+=_map[x][y];
		}
		
	}
	return wallCount;
}

void Floor::ConnectClosestRooms(vector<Room*> rooms, bool forceAccessibility)
{
	
	//cout<<"Connecting rooms.."<<std::endl;
	vector<Room*> roomListA;
	vector<Room*> roomListB;
		
	if(forceAccessibility)
	{
		for(int i = 0; i < rooms.size(); i++)
		{
			Room* R = rooms[i];
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
	
	Tile bestTileA = Tile();
	Tile bestTileB = Tile();
	
	Room* bestRoomA = new Room();
	Room* bestRoomB = new Room();
		
	bool connectionFound = false;
	
	for(int i = 0; i < roomListA.size(); i++)
	{
		Room* A = roomListA[i];
		if(!forceAccessibility)
		{
			connectionFound = false;
			if(A->connectedRooms.size() > 0)
			{
				continue;
			}
		}
		
		for(int j = 0; j < roomListB.size(); j++)
		{
			Room* B = roomListB[j];
			
			if(A->id == B->id || A->isConnected(B))
			{
				continue;
			}
			
			for(int tA = 0; tA < A->border.size(); tA ++)
			{
				for(int tB = 0; tB < B->border.size(); tB ++)
				{
					Tile tileA =  A->border[tA];
					Tile tileB =  B->border[tB];
						
					int distance = (int) (pow((tileA.x - tileB.x), 2) + 
						 pow((tileA.y - tileB.y), 2));
						 
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
		
		if(connectionFound && !forceAccessibility)
			CreatePassage(bestRoomA, bestRoomB, bestTileA, bestTileB);
	}
		
		
	if(connectionFound && forceAccessibility)
	{
		CreatePassage(bestRoomA, bestRoomB, bestTileA, bestTileB);
		ConnectClosestRooms(rooms, true);
	}
	
	if(!forceAccessibility)
	{
		ConnectClosestRooms(rooms, true);
	}
	
	//cout<<"Done Connecting Rooms"<<std::endl;
}

void Floor::CreatePassage(Room* A, Room* B, Tile tA, Tile tB)
{
	//cout<<"Creating Passage"<<std::endl;
	Room::ConnectRooms(A, B);
	
	vector<Tile> line = GetLine(tA, tB);
	
	for(int i = 0; i < line.size(); i++)
	{
		DrawCircle(line[i], 2);
	}
}

//get the line of tiles connecting start to end
vector<Tile> Floor::GetLine(Tile start, Tile end)
{
	//cout<<"Getting Line"<<std::endl;
	vector<Tile> line;
		
	int x = start.x;
	int y = start.y;
	
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	
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
		line.push_back(Tile(x,y));
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
	cout<<std::endl;
	return line;
}

void Floor::DrawCircle(Tile t, int r)
{
	//cout<<"Drawing circle at point "<<t.x<<" "<<t.y<<" "<<"with radius "<<r<<std::endl;
	for(int x = -r; x <=r ; x++)
	{
		for(int y = -r; y <=r ; y++)
		{
			if(x*x + y*y <= r*r)
			{
				int mapX = t.x + x;
				int mapY = t.y + y;
				
				if(InMapRange(mapX, mapY))
				{
					_map[mapX][mapY] = 2;
				}
			}
		}
	}		
}

void Floor::LoadFloor()
{
	string s;
	cout<<"Path to file: ";
	getline (cin, s);
	
	LoadFloor(s);
}

void Floor::LoadFloor(string path)
{
	std::ifstream file;
	file.open(path.c_str());
	
	string line;
	char l[100000];
	
	if(!file.is_open())
		return;
	
	_map.clear();
	
	while(getline (file, line))
	{
		strcpy(l,line.c_str());
		vector<int> col;
		
		for(int y = 0 ; y < strlen(l) ; y++)
		{
			col.push_back((l[y] - '0'));
		}
		
		//for now only read 0, 1
		_map.push_back(col);
	}
	
	/*for(int i = 0 ; i < _map.size(); i++)
	{
		for(int j = 0 ; j < _map[i].size(); j++)
		{
			cout<<_map[i][j];
		}
		cout<<std::endl;
	}
	
	cout<<"Finished printing"<<std::endl;*/
	
	_width = _map.size();
	
	if(_width > 0)
		_height = _map[0].size();
	else
		_height = 0;
	
	file.close();
	
	cout<<"Finished reading"<<std::endl;
}

void Floor::SaveFloor()
{
	string s;
	cout<<"Path to file: ";
	getline (cin, s);
	
	SaveFloor(s);
}

void Floor::SaveFloor(string path)
{
	std::ofstream file;
	file.open(path.c_str());
	
	if(!file.is_open())
		return;
	
	for(int i = 0; i < _width ; i++)
	{
		for(int j = 0; j < _height ; j++)
		{
			file << _map[i][j];
		}
		file << '\n';
	}
	file.close();
}

//======================WORLD METHODS=================================
//constructor
/*
World::World(const string& name= "Earth", int floors, int difficulty= 1)
{
	_name = name;
	_data = new Floor*[floors];
	_floors = floors;
	_current = 0;
	_difficulty = difficulty;
	cout<<"Generating..."<<endl;
	generate();
}

World::~World(){
	
}

Floor* World::getFloor(int index)
{
	return _data[index];
}

//generate all the floors for the world
void World::generate()
{
	//think of a better equation for scaling difficulty
	int diff = 1;
	//iterate over every floor, delete it and create a new one
	for(int i=0; i<_floors; i++){
		delete _data[i];
		_data[i] = new Floor(diff);
	}
	return;
}

void World::print()
{
	_data[_current]->print();
}

void World::clear()
{
	
	return;
}

void World::update()
{
	
}

void World::move()
{
	_data[_current]->move();
}*/
