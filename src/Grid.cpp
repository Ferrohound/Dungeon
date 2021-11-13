#include "Grid.h"


#pragma region TILE_METHODS
//=======================TILE========================================

Tile::Tile()
{
	// x = 0;
	// y = 0;
	//pos = Vector2 { 0, 0 };
}

/*Tile::Tile(Vector2 position)
{
	pos = position;
	x = pos.x;
	y = pos.y;
}*/

// Tile::Tile(int _x, int _y)
// {
// 	x = _x;
// 	y = _y;
	
// 	//pos = Vector2{ x, y };
// }

Tile::~Tile()
{
	
}

//========================= TILE FACTORY ===============================
Tile* TileFactory::CreateTile()
{
	return new Tile();
}

#pragma endregion TILE_METHODS

#pragma region GRID_METHODS
//========================GRID METHODS================================
bool Grid::InMapRange(int x, int y)
{
	return x >= 0 && x < _width && y >= 0 && y < _height;
}

//create a floor with the given dimensions
Grid::Grid( int width, int height, TileFactory* factory ) : _width(width), _height(height), _factory(factory)
{
	//shouldn't need this..
    //_map.clear();

    for(int i = 0; i < _width; i++)
	{
		// vector< Tile* >tmp(_height);
		vector<Tile*> tmp;
		for(int j = 0; j < _height; j++)
		{
			tmp.push_back( _factory->CreateTile() );
		}
		_map.push_back(tmp);
	}
}

//free all of our tiles
Grid::~Grid()
{
	//calling vector clear calls the destructor for each element but not delete

}


void Grid::Clear()
{
	cout<<"Clearing"<<std::endl;

	// free all of the tiles
	/*for(int x = 0; x < _width; x++)
	{
		for(int y = 0; y < _height; y++)
		{
			delete _map[x][y];
		}
	}*/
	
	_map.clear();
	
	for(int i = 0; i < _width; i++)
	{
		// vector< Tile* >tmp(_height);
		vector<Tile*> tmp;
		for(int j = 0; j < _height; j++)
		{
			tmp.push_back( _factory->CreateTile() );
		}
		_map.push_back(tmp);
	}
	
	cout<<"Done Clearing"<<std::endl;
}

//a tile is defined as an outline when any of the surrounding
//tiles is an "empty" spot (assuming the tile in question isn't)
//empty itself
bool Grid::IsOutlineTile(int x, int y, Tile* reference)
{
	if(!InMapRange(x,y)) return false;

	//check all of the surrounding tiles,	
	for(int i = x-1; i < x+2; i++)
	{
		for(int j = y-1; j < y+2; j++)
		{
			if( i == x && j == y ) continue;
			if( i != x && j != y ) continue;
			if(!InMapRange(i, j)) continue;
			
			if(_map[i][j] == reference) return true;
		}
	}
	return false;
}

//floodfill to get all tiles in the region
vector< std::pair<int, int> > Grid::GetRegionTiles(int sx, int sy,  bool (*compare)(Tile* A, Tile* B) )
{
	vector<std::pair<int, int>> tiles;

	//https://en.cppreference.com/w/cpp/language/lambda
	//if the compare function is null, just compare pointer values
	if(compare == NULL)
	{
		compare = [](Tile* A, Tile* B) -> bool {
			if(A == B)
				return true;
			else
				return false;
		};
	}
		
	//check if tile was looked at yet
	//need to initialize each value, however, look at alternatives
	vector< vector <int> > flags(_width);
	for(int i = 0; i < _width; i++)
	{
		vector<int> tmp(_height);
		flags.push_back(tmp);
		for(int j = 0; j < _height; j++)
		{
			flags[i][j] = 0;
		}
	}
	
	Tile* tileType = _map[sx][sy];
	
	queue< std::pair<int, int> > q;
	q.push( std::make_pair(sx, sy) );

	flags[sx][sy] = 1;
	
	//breadth first search across tiles
	while(!q.empty())
	{
		std::pair<int, int> t = q.front();
		q.pop();
		tiles.push_back(t);
		
		for(int x = t.first - 1; x <= t.first + 1; x++)
		{
			for(int y = t.second - 1; y <= t.second + 1; y++)
			{
				if(InMapRange(x, y) && (y == t.second || x == t.first) && flags[x][y] == 0 
					&& compare(tileType, _map[x][y]) )
				{
					flags[x][y] = 1;
					q.push(std::make_pair(x, y));
				}
			}
		}
	}
	
	return tiles;
}

vector< vector<std::pair<int, int>> > Grid::GetRegions(Tile* tileType,  bool (*compare)(Tile* A, Tile* B) )
{
	//cout<<"Getting Regions"<<std::endl;
	vector< vector<std::pair<int, int>> > regions;
	
	//flags to determine which tiles have already been visited
	vector< vector <int> > flags(_width);
	for(int i = 0; i < _width; i++)
	{
		vector<int> tmp(_height);
		flags.push_back(tmp);
		for(int j = 0; j < _height; j++)
		{
			flags[i][j] = 0;
		}
	}
	
	//iterate over every tile, could optimize this fairly easily
	//by removing locations from the set as we touch them
	for(int x = 0; x < _width; x++)
	{
		for(int y = 0; y < _height; y++)
		{
			if(flags[x][y] == 0 && compare(_map[x][y], tileType) )
			{
				vector< std::pair<int, int> > newRegion = GetRegionTiles(x, y, compare);
				regions.push_back(newRegion);
				
				for(int i = 0 ; i < newRegion.size(); i++)
				{
					std::pair<int, int> t = newRegion[i];
					flags[t.first][t.second] = 1;
				}
			}
		}
	}
	//cout<<"Done Getting " << regions.size()<<" Regions"<<std::endl;
	return regions;
}

//get the line of tiles connecting start to end
vector<std::pair<int, int>> Grid::GetLine(std::pair<int, int> start, std::pair<int, int> end)
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

void Grid::DrawCircle(std::pair<int, int> t, int r, Tile* fill)
{
	//cout<<"Drawing circle at point "<<t.x<<" "<<t.y<<" "<<"with radius "<<r<<std::endl;
	for(int x = -r; x <=r ; x++)
	{
		for(int y = -r; y <=r ; y++)
		{
			if(x*x + y*y <= r*r)
			{
				int mapX = t.first + x;
				int mapY = t.second + y;
				
				if(InMapRange(mapX, mapY))
				{
					//if (debug)
					//	_map[mapX][mapY] = fill;
					//else
						_map[mapX][mapY] = fill;
				}
			}
		}
	}		
}

void Grid::LoadFloor()
{
	string s;
	cout<<"Path to file: ";
	getline (cin, s);
	
	LoadFloor(s);
}

//how floors are loaded and serialized will depend on the project tbh
void Grid::LoadFloor(string path)
{
	/*std::ifstream file;
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
	
	_width = _map.size();
	
	if(_width > 0)
		_height = _map[0].size();
	else
		_height = 0;
	
	file.close();
	
	cout<<"Finished reading"<<std::endl;
	*/
}

void Grid::SaveFloor()
{
	string s;
	cout<<"Path to file: ";
	getline (cin, s);
	
	SaveFloor(s);
}

//how floors are loaded and serialized will depend on the project tbh
void Grid::SaveFloor(string path)
{
	/*std::ofstream file;
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
	*/
}

void Grid::ImportFloor(Tile** grid, int width, int height)
{
	Clear();

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			_map[x][y] = grid[ y * width + x];
		}
	}

	cout<<"Done importing floor"<<std::endl;
}

Tile** Grid::ExportFloor()
{
	Tile** output = new Tile*[ _width * _height ];

	for(int x = 0; x < _width; x++)
	{
		for(int y = 0; y < _height; y++)
		{
			output[ y * _width + x] = _map[x][y];
		}
	}

	return output;
}

#pragma endregion GRID_METHODS