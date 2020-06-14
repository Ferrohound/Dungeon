#include "Grid.h"

//=======================TILE========================================

Tile::Tile()
{
	x = 0;
	y = 0;
	//pos = Vector2 { 0, 0 };
}

/*Tile::Tile(Vector2 position)
{
	pos = position;
	x = pos.x;
	y = pos.y;
}*/

Tile::Tile(int _x, int _y)
{
	x = _x;
	y = _y;
	
	//pos = Vector2{ x, y };
}

Tile::~Tile()
{
	
}

//========================GRID METHODS================================
bool Grid::InMapRange(int x, int y)
{
	return x >= 0 && x < _width && y >= 0 && y < _height;
}

//create a floor with the given dimensions and fillPercentage
Grid::Grid( int width, int height ) : _width(width), _height(height)
{
    _map.clear();

    for(int i = 0; i < _width; i++)
	{
		vector<int>tmp(_height);
		_map.push_back(tmp);
	}
}


void Grid::Clear()
{
	cout<<"Clearing"<<std::endl;
	
	_map.clear();
	
	for(int i = 0; i < _width; i++)
	{
		vector<int>tmp(_height);
		_map.push_back(tmp);
	}
	
	for(int x = 0; x < _width; x++)
	{
		for(int y = 0; y < _height; y++)
		{
			_map[x][y] = 1;
		}
	}
	
	cout<<"Done Clearing"<<std::endl;
}

bool Grid::IsOutlineTile(int x, int y)
{
	if(!InMapRange(x,y)) return false;
	
	for(int i = x-1; i < x+2; i++)
	{
		for(int j = y-1; j < y+2; j++)
		{
			if( i == x && j == y ) continue;
			if( i != x && j != y ) continue;
			if(!InMapRange(i, j)) continue;
			
			if(_map[i][j] == 1) return true;
		}
	}
	return false;
}

//floodfill to get all tiles in the region
vector<Tile> Grid::GetRegionTiles(int sx, int sy)
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

vector< vector<Tile> > Grid::GetRegions(int tileType)
{
	//cout<<"Getting Regions"<<std::endl;
	vector< vector<Tile> > regions;
		
	//check if tile was looked at yet
	int flags[_width][_height];
	
	for(int i = 0; i < _width; i++)
	{
		for(int j = 0; j < _height; j++)
		{
			flags[i][j] = 0;
		}
	}
		
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
int Grid::GetSurroundingWallCount(int sx, int sy)
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

//get the average value of the surrounding tiles within the given radius
int Grid::GetAvgWallValue(int sx, int sy, int radius)
{
	int avg = 0;
	int wallCount = 0;
	
	for(int x = sx-radius; x <=sx+radius; x++)
	{
		for(int y = sy-radius; y <=sy+radius; y++)
		{
			if(x == sx && y == sy)
				continue;
			//if out of bounds, increase wallcount to encourage the sides to be wall-filled
			if(!InMapRange(x, y))
				wallCount++;
			else
			{
				wallCount++;
				avg+=_map[x][y];
			}
		}
		
	}
	
	avg/=wallCount;
	return avg;
}

//get the line of tiles connecting start to end
vector<Tile> Grid::GetLine(Tile start, Tile end)
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

void Grid::DrawCircle(Tile t, int r, int fill)
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

void Grid::LoadFloor(string path)
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

void Grid::SaveFloor()
{
	string s;
	cout<<"Path to file: ";
	getline (cin, s);
	
	SaveFloor(s);
}

void Grid::SaveFloor(string path)
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

void Grid::ImportFloor(float* grid, int width, int height)
{
	_map.clear();
	_width = width;
	_height = height;
	
	for(int i = 0; i < _width; i++)
	{
		vector<int>tmp(_height);
		_map.push_back(tmp);
	}

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			_map[x][y] = grid[ y * width + x];
		}
	}

	cout<<"Done importing floor"<<std::endl;
}

float* Grid::ExportFloor()
{
	float* output = new float[ _width * _height ];

	for(int x = 0; x < _width; x++)
	{
		for(int y = 0; y < _height; y++)
		{
			output[ y * _width + x] = _map[x][y];
		}
	}

	return output;
}