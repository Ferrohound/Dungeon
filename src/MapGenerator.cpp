#include "MapGenerator.h"

//=============================== HELPER FUNCTIONS ================================
//may remove these from the class and have these by themselves if useful enough

void DrawCircle(Grid* grid, Tile t, int r, int fill)
{
	grid->DrawCircle(t, r, fill);	
}

//get the line of tiles connecting start to end
vector<Tile> GetLine(Tile start, Tile end)
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
	//cout<<std::endl;
	return line;
}

void CreatePassage(Grid* grid, Room* A, Room* B, Tile tA, Tile tB, int fill)
{
	//cout<<"Creating Passage"<<std::endl;
	Room::ConnectRooms(A, B);
	
	vector<Tile> line = GetLine(tA, tB);
	
	for(int i = 0; i < line.size(); i++)
	{
		//===================================== TO DO
		//editing a bit..
		DrawCircle(grid, line[i], 1, fill);
	}
}


void ConnectRooms(Grid* grid, Room* A, Room* B, bool angular, int fill)
{
	int lowestD = (int) (pow((A->border[0].x - B->border[0].x), 2) +
		(pow((A->border[0].y - B->border[0].y), 2)));
	
	Tile bestTileA = Tile();
	Tile bestTileB = Tile();
	
	for(int tA = 0; tA < A->border.size(); tA ++)
	{
		for(int tB = 0; tB < B->border.size(); tB ++)
		{
			Tile tileA =  A->border[tA];
			Tile tileB =  B->border[tB];
						
			int distance = (int) (pow((tileA.x - tileB.x), 2) + 
				 pow((tileA.y - tileB.y), 2));
						 
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
		Tile corner = Tile(/*bestTileA.x - */bestTileB.x, bestTileA.y/* - bestTileB.y*/);
		vector<Tile> line1 = GetLine(bestTileA, corner);
		vector<Tile> line2 = GetLine(corner, bestTileB);

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
		CreatePassage(grid, A, B, bestTileA, bestTileB, 2);
	}
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
	}

	grid->ImportFloor(pMap, grid->GetWidth(), grid->GetHeight());

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
				grid->_map[x][y] = 1;
			else
				grid->_map[x][y] = (rand() % 101 < fillPercentage) ? 1:0;
		}
	}		
	//cout<<"done filling"<<std::endl;
}

//get each tile's x,y position and set its height to the average of the surrounding
//tiles
void SmoothRoom(Grid* grid, Room* room, int radius)
{
	float avg = 0;
	for(int k = 0; k < room->tiles.size(); k++)
	{
		avg = grid->GetAvgWallValue(room->tiles[k].x, room->tiles[k].y, radius);
		grid->_map[room->tiles[k].x][room->tiles[k].y] = avg;
			
	}
}

//================= to do =======================
//just iterate over the tiles and draw them
void AddRoom(Grid* grid, Room* room)
{
	for (auto& tile : room->tiles)
	{
		
		if(grid->InMapRange(tile.x, tile.y))
			grid->_map[tile.x][tile.y] = 0;
	}
	//might relocate this to the system itself
	//_rooms.push_back(room);
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

Room::Room(int x, int y, int width, int height, vector< vector<int> > map )
{
	tiles.clear();
	
	//something about tiles is getting corrupted, would be nice to not use pointers
	//pushback tiles from that region
	for(int i = x; i < x+width; i++)
	{
		for(int j = y; j < y+height; j++)
		{
			tiles.push_back(Tile(i, j));
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

//================================== ROOM SYSTEM METHODS ================================
//might want to cap out the size of the room for whatever reason later on so keep min and maxS
void RoomSystem::Generate(Grid* grid, int fill, bool dense, int minS, int maxS, int numSteps)
{
	int med, numRooms, fillVolume;

	med = grid->GetWidth() * grid->GetWidth();
	fillVolume = (int) (med * ((float)fill/100.0));

	if(minS == -1 || maxS == -1)
	{
		std::cout<<"Sqrt = "<<sqrt(fillVolume)<<std::endl;
		med = sqrt(sqrt(fillVolume));
		minS = med - 5;
		
		(minS <= 0) ? minS = 4 : minS = minS;

		maxS = med + 5;

		std::cout<<"Med: "<<med<<"MaxS "<<maxS<<" MINS "<<minS<<std::endl;
	}

	nodes.clear();
	cmX = cmY = 0;
	bounds.x = grid->GetWidth() - 1;
	bounds.y = grid->GetHeight() - 1;

	PopulateSystem(minS, maxS, grid->GetWidth(), grid->GetHeight(), fillVolume);
	for(int i = 0; i < numSteps; i++) Tick();

	for( auto& RN : nodes ) AddRoomToFloor(grid, RN);
	
	Graph<RoomNode> G = ConnectSystem();
	auto mst = G.MST();
	
	if(!dense)
		AddCycles(mst);
	else
		AddDenseCycles(mst);

	auto links = mst.GetEdges();

	std::cout<<"Final graph has "<<links.size()<<" edges"<<std::endl;
	AddEdgesToFloor(grid, links);
}

//===================== TO DO : free up memory properly ====================
void RoomSystem::PopulateSystem(int minSize, int maxSize, int dimX, int dimY, int maxFill)
{
	vector< std::pair<RoomNode* , int> > elements;
	vector< RoomNode* > tmp;

	int totalVolume = 0;

	while(totalVolume < maxFill * 2)
	{
		//get a random x, y  in the range of the room dimensions
		int s = minSize + rand() % ( maxSize - minSize + 1 );
		//keep the room in range
		int x = rand() % (dimX - s - 1) + 1;
		int y = rand() % (dimY - s - 1) + 1;

		RoomNode* r = new RoomNode(x, y, s);
		tmp.push_back(r);

		totalVolume += (s * s);
	}

	//sort based on fewest surrounding things
	for(int i = 0; i < tmp.size(); i++)
	{
		int surrounding = GetSurroundingNodes(tmp[i], tmp[i]->GetSize() * 2, tmp).size();
		elements.push_back(std::make_pair(tmp[i], surrounding));
	}

	//sort in ascending order
	std::sort( elements.begin(), elements.end(), 
		[]( const std::pair<RoomNode* , int>& A, 
			const std::pair<RoomNode* , int>& B) -> bool 
			{
				return A.second > B.second;
			});

	for(int i = 0; i < tmp.size()/2; i++)
	{
		nodes.push_back(elements[i].first);
	}

	std::cout<<"Total of "<<nodes.size()<<" nodes."<<std::endl;
}

void RoomSystem::Tick() 
{
	CalculateCoM(cmX, cmY);

	for( int i = 0; i < nodes.size(); i++)
	{
		//should actually have to do with node's size
		AdjustNode(nodes[i], GetSurroundingNodes(nodes[i], 10, nodes));
	}
}

std::vector<RoomNode*> RoomSystem::GetSurroundingNodes(RoomNode* R, int distance, std::vector<RoomNode*> s)
{
	std::vector<RoomNode*> out;

	for( int i = 0; i < s.size(); i++)
	{
		if(vec2::VecDistance(R->GetPosition(), s[i]->GetPosition()) < distance)
			out.push_back(s[i]);
	}
	
	return out;
}

//==================== to do =========================
//apply the vector of the surrounding nodes to the main node
//maybe apply the "elastic" model later on if it isn't as nice
void RoomSystem::AdjustNode(RoomNode* node, std::vector<RoomNode*> surrounding)
{
	for( auto& n : surrounding)
	{
		vec2 pos = node->GetPosition();
		vec2 dir = n->GetPosition() - pos;

		if(dir.magnitude() != 0)
			dir = dir.normalize();

		dir.scale(n->GetSize());

		pos = pos - dir;

		if(pos.x + node->GetSize() > bounds.x)
			pos.x = bounds.x - node->GetSize();

		if(pos.x < 1 )
			pos.x = 1;

		if(pos.y + node->GetSize() > bounds.y)
			pos.y = bounds.y - node->GetSize();

		if(pos.y < 1)
			pos.y = 1;


		node->SetPosition(pos);
	}

	vec2 endpos = node->GetPosition();
}

void RoomSystem::CalculateCoM(int& x, int& y)
{
	int M= 0, X = 0, Y = 0;
	int tx, ty;

	for(int i = 0; i < nodes.size(); i++)
	{
		vec2 pos = nodes[i]->GetPosition();
		X += pos.x * nodes[i]->GetSize();
		Y += pos.y * nodes[i]->GetSize();

		M += nodes[i]->GetSize();
	}

	x = std::floor((float)X/(float)M);
	y = std::floor((float)Y/(float)M);
}

//create a graph out of the nodes, then connect all of them with the others
Graph<RoomNode> RoomSystem::ConnectSystem() 
{
	Graph<RoomNode> G;

	//avoid this somehow..
	for( int j = 0 ; j < nodes.size(); j++ )
	{
		G.AddNode(nodes[j]);
	}

	for( int i = 0 ; i < nodes.size() - 1; i++ )
	{
		for( int j = i+1 ; j < nodes.size(); j++ )
		{
			Node<RoomNode>* A = G.GetNode(nodes[i]);
			Node<RoomNode>* B = G.GetNode(nodes[j]);

			if(A == NULL || B == NULL)
			{
				std::cout<<"We have a problem here..."<<std::endl;
				continue;
			}

			G.AddEdge(A, B, 
				vec2::VecDistance(
					nodes[i]->GetPosition(), 
					nodes[j]->GetPosition()
					)
				);
		}
	}	

	return G;
}

//re-add edges to the graph
//========== TO DO : get a better factor for re-adding edges =================
//maybe something like if the distance is less than the shortest edge already attached
// multiplied by some factor add it
void RoomSystem::AddCycles(Graph<RoomNode>& MST)
{
	for(int i = 0; i < nodes.size() - 1; i++)
	{
		vector<RoomNode*> sur = GetSurroundingNodes(nodes[i], nodes[i]->GetSize() * 2, nodes);
		for(auto& node : sur)
		{
			int distance = MST.Distance(MST.GetNode(nodes[i]), MST.GetNode(node));
			if( distance > nodes.size()/2 )
			{
				MST.AddEdge(MST.GetNode(nodes[i]), MST.GetNode(node), distance);
			}
		}
	}
}

//add cycles in a way that the floor is noisy in a way that is pleasing
//once no notion of how rooms and corridors are distinguished
void RoomSystem::AddDenseCycles(Graph<RoomNode>& MST)
{
	auto nodes = MST.GetNodes();
	for(int i = 0; i < nodes.size() - 1; i++)
	{
		for(int j = i+1; j < nodes.size(); j++)
		{
			//some asinine factor here
			int distance = MST.Distance(nodes[i], nodes[j]);
			if( distance > nodes.size()/2 )
			{
				MST.AddEdge(nodes[i], nodes[j], distance);
			}
		}
	}
}

//======================= to do =========================
void RoomSystem::AddEdgesToFloor(Grid* grid, vector< Edge<RoomNode> >& edges)
{
	for(auto& edge : edges)
	{
		ConnectRooms(grid, edge.to->data->GetRoom(),
		edge.from->data->GetRoom(), true, 2);
	}
}

//================= to do ====================
//need to create room then add it to the floor
void RoomSystem::AddRoomToFloor(Grid* grid, RoomNode* room)
{
	Room* rm;
	vec2 pos = room->GetPosition();
	rm = new Room(pos.x, pos.y, room->GetSize(), room->GetSize(), grid->_map);

	room->SetRoom(rm);

	AddRoom( grid, room->GetRoom() );
}


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
			int neighbors = grid->GetSurroundingWallCount(x, y);
			
			if(neighbors > 4)
			{
				grid->_map[x][y] = 1;
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
	
	vector< vector<Tile> > wallRegions = grid->GetRegions(1);
	vector<Room*> remainingSpaces;
		
	//any region with less than threshold tiles, remove it
	int wallThreshold = 4;
	int id = 0;
	for(int i=0 ; i < wallRegions.size(); i++)
	{
		if(wallRegions[i].size() < wallThreshold)
		{
			for(int j = 0; j < wallRegions[i].size(); j++)
			{
				Tile t = wallRegions[i][j];
				grid->_map[t.x][t.y] = 0;
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
		
	vector< vector<Tile> > roomRegions = grid->GetRegions(0);
	
	if (debug)
		cout<<"Initial number of rooms: "<<roomRegions.size() << std::endl;
		
	//any region with less than threshold tiles, remove it
	int roomThreshold = 4;
	vector<Room*> remainingRooms;
	id = 0;
	for(int i=0 ; i < roomRegions.size(); i++)
	{
		if(roomRegions[i].size() < roomThreshold)
		{
			for(int j = 0; j < roomRegions[i].size(); j++)
			{
				Tile t = roomRegions[i][j];
				grid->_map[t.x][t.y] = 1;
			}
		}
		else
		{
			remainingRooms.push_back(new Room(roomRegions[i], grid->_map, id));
			_rooms.push_back(remainingRooms[id]);
			id++;
		}
	}
	
	if (debug)
		cout<<"Removed "<<roomRegions.size() - remainingRooms.size() << " rooms." << std::endl;

		
	//error check this to make sure the room's aren't all eliminated
	//remainingRooms.Sort();
	
	std::sort (remainingRooms.begin(), remainingRooms.end(), Room::CompareRooms);
	if(remainingRooms.size() > 0)
	{
		remainingRooms[0]->mainRoom = true;
		remainingRooms[0]->accessible = true;
		if (debug)
			cout<<"Biggest of the remaining rooms=> " << remainingRooms[0]->size<<std::endl;
	}
	
	if(connect)
		ConnectClosestRooms(grid, remainingRooms);
	
	cout<<"Done Processing"<<std::endl;
}

void MapSystem::ConnectClosestRooms(Grid* grid, vector<Room*> rooms, bool forceAccessibility)
{
	
	//cout<<"Connecting rooms.."<<std::endl;
	vector<Room*> roomListA;
	vector<Room*> roomListB;

	//separate the accessible rooms from the non-accessible ones		
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
	
	//go through every non-accessible room we have to work with
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
		
		//for every room that's accessible
		for(int j = 0; j < roomListB.size(); j++)
		{
			Room* B = roomListB[j];
			
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
		
		//if a connection is found and we're not forcing accessibility, connect them
		//and go onto the next room
		if(connectionFound && !forceAccessibility)
			CreatePassage(grid, bestRoomA, bestRoomB, bestTileA, bestTileB, 2);
	}
		
		
	if(connectionFound && forceAccessibility)
	{
		CreatePassage(grid, bestRoomA, bestRoomB, bestTileA, bestTileB, 2);
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
					grid->_map[_rooms[j]->tiles[k].x][_rooms[j]->tiles[k].y] = 
						(rand() % (max - min) + min);
				}
			}
			else
			{
				SmoothRoom(grid, _rooms[j], 1);
			}
		}
	}
}