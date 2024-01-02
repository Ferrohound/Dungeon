/*
	Basis for most of the procedural generators in the suite
	A 2D grid with dimensions x, y made up of Cells

	It is the job of the generators to populate the grid's Cell data
	To save on memory, with static Cell types, just use pointers to point to the same
	instance of the Cell types
*/

#ifndef GRID_H
#define GRID_H
#pragma once

#include <string.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <utility> /*std::pair, std::make_pair */

#include "vec.h"
#include "Cell.h"
#include "Region.h"

using std::cin;
using std::cout;
using std::endl;

using std::queue;
using std::string;
using std::vector;

template <typename T>
class Grid
{
public:
	Grid() : _width(0), _height(0) {}
	Grid(int width, int height, CellFactory<T> *factory);
	// Copy constructor for deep copy
	Grid(const Grid &other) : debug(other.debug), _width(other._width), _height(other._height), _factory(other._factory)
	{

		for (int i = 0; i < _width; i++)
		{
			// vector< Cell* >tmp(_height);
			vector<Cell<T> *> tmp;
			for (int j = 0; j < _height; j++)
			{
				Cell<T> *cell = other.GetCell(i, j);
				tmp.push_back(_factory->CreateCell(cell->pos.x, cell->pos.y, cell->data));
			}
			_map.push_back(tmp);
		}
	}

	Grid<T> *Clone()
	{
		return new Grid<T>(*this);
	}

	~Grid();

	bool InMapRange(int x, int y) const;

	// get regions of the same type of Cell using floodfill
	vector<vec2> GetRegionCells(int sx, int sy, bool (*compare)(const T A, const T B) = NULL);
	vector<vector<vec2>> GetRegions(const T CellType, bool (*compare)(const T MapCell, const T CellType) = NULL);

	// is the given position an outline Cell (reference)
	bool IsOutlineCell(int x, int y, Cell<T> *reference);

	template <size_t N>
	vector<Cell<T> *> GetRegionOutline(Region<T> *region, T (&regionContents)[N])
	{
		vector<Cell<T> *> border;
		// create the border cells list
		for (int i = 0; i < region->cells.size(); i++)
		{
			for (int x = region->cells[i]->pos.x - 1; x < region->cells[i]->pos.x + 1; x++)
			{
				for (int y = region->cells[i]->pos.y - 1; y < region->cells[i]->pos.y + 1; y++)
				{
					if ((x == region->cells[i]->pos.x || y == region->cells[i]->pos.y) && std::find(regionContents, regionContents + N, _map[x][y]->data) != regionContents + N)
					{
						border.push_back(region->cells[i]);
					}
				}
			}
		}
		return border;
	}

	template <size_t N>
	vector<Cell<T> *> GetRegionOutline(vector<Cell<T> *> region, T (&regionContents)[N])
	{
		vector<Cell<T> *> border;
		// create the border cells list
		for (int i = 0; i < region.size(); i++)
		{
			for (int x = region[i]->pos.x - 1; x < region[i]->pos.x + 1; x++)
			{
				for (int y = region[i]->pos.y - 1; y < region[i]->pos.y + 1; y++)
				{
					if ((x == region[i]->pos.x || y == region[i]->pos.y) && std::find(regionContents, regionContents + N, _map[x][y]->data) != regionContents + N)
					{
						border.push_back(region[i]);
					}
				}
			}
		}
		return border;
	}

	template <size_t N>
	vector<Cell<T> *> GetRegionOutline(vector<vec2> region, T (&regionContents)[N])
	{
		vector<Cell<T> *> border;
		// create the border cells list
		for (int i = 0; i < region.size(); i++)
		{
			for (int x = region[i].x - 1; x < region[i].x + 1; x++)
			{
				for (int y = region[i].y - 1; y < region[i].y + 1; y++)
				{
					auto cell = GetCell(x, y);
					if (cell == NULL)
						continue;
					if ((x == region[i].x || y == region[i].y) && std::find(regionContents, regionContents + N,cell->data) != regionContents + N)
					{
						border.push_back(_map[x][y]);
					}
				}
			}
		}
		return border;
	}

	void DrawCircle(vec2 t, int r, T fill);

	vector<Cell<T> *> GetPassageCells(Cell<T> *tA, Cell<T> *tB);
	vector<Cell<T> *> GetPassageCells(vec2 tA, vec2 tB);
	Region<T> *ConnectRegions(Region<T> *A, Region<T> *B, bool angular, T fill);

	vector<vec2> GetLine(vec2 start, vec2 end);

	// save a floor and a load a floor from a given path
	void LoadFloor(string path);
	void SaveFloor(string path);

	// load and save floor from cin
	void LoadFloor();
	void SaveFloor();

	// read in and put out an int array grid
	void ImportFloor(Cell<T> **grid, int width, int height);
	Cell<T> **ExportFloor();

	void Clear();

	vector<vector<int>> GetMap();

	Cell<T> *GetCell(vec2 pos) const
	{
		return GetCell(pos.x, pos.y);
	}

	Cell<T> *GetCell(int x, int y) const
	{
		if (InMapRange(x, y))
			return _map[x][y];
		return NULL;
	}

	vector<Cell<T> *> GetCells(vector<vec2> positions)
	{
		vector<Cell<T> *> out;

		for (vec2 &i : positions)
		{
			out.push_back(GetCell(i.x, i.y));
		}
	}

	int GetWidth() { return _width; }
	int GetHeight() { return _height; }

	// overload the cout
	friend std::ostream &operator<<(std::ostream &output, const Grid &F)
	{
		output << "Grid is : " << std::endl;
		for (int i = 0; i < F._width; i++)
		{
			for (int j = 0; j < F._height; j++)
			{
				output << *F._map[i][j];
			}
			output << '\n';
		}
		return output;
	}

	std::vector<vector<Cell<T> *>> _map;
	bool debug;
	CellFactory<T> *_factory;

private:
	int _width, _height;
};

#pragma region helper_functions
//=============================== HELPER FUNCTIONS ================================
// may remove these from the class and have these by themselves if useful enough

template <typename T>
vector<Cell<T> *> Grid<T>::GetPassageCells(Cell<T> *tA, Cell<T> *tB)
{
	return GetPassageCells(tA->pos, tB->pos);
}

template <typename T>
vector<Cell<T> *> Grid<T>::GetPassageCells(vec2 tA, vec2 tB)
{
	vector<vec2> line = GetLine(tA, tB);

	vector<Cell<T> *> out;

	for (int i = 0; i < line.size(); i++)
	{
		//===================================== TO DO
		// editing a bit..
		// DrawCircle(line[i], 1, fill);

		out.push_back(GetCell(line[i]));
	}

	return out;
}

template <typename T>
Region<T> *Grid<T>::ConnectRegions(Region<T> *A, Region<T> *B, bool angular, T fill)
{
	int lowestD = (int)(pow((A->border[0]->pos.x - B->border[0]->pos.x), 2) +
						(pow((A->border[0]->pos.y - B->border[0]->pos.y), 2)));

	vec2 bestCellA;
	vec2 bestCellB;

	for (int tA = 0; tA < A->border.size(); tA++)
	{
		for (int tB = 0; tB < B->border.size(); tB++)
		{
			vec2 CellA = A->border[tA]->pos;
			vec2 CellB = B->border[tB]->pos;

			int distance = (int)(pow((CellA.x - CellB.x), 2) +
								 pow((CellA.y - CellB.y), 2));

			if (distance < lowestD)
			{
				lowestD = distance;

				bestCellA = CellA;
				bestCellB = CellB;
			}
		}
	}

	vector<Cell<T> *> regionTiles;

	// TODO: regionTiles shouldn't be empty but sometimes it is, look into this
	// create an angular, maybe even jagged connection
	if (angular)
	{
		// GetPassageCells(A, B, bestCellA, bestCellB);
		vec2 corner = {bestCellB.x, bestCellA.y};
		vector<Cell<T> *> line1 = GetPassageCells(bestCellA, corner);
		vector<Cell<T> *> line2 = GetPassageCells(corner, bestCellB);

		auto it = line2.begin();

		regionTiles.reserve(line1.size() + line2.size()); // preallocate memory
		regionTiles.insert(regionTiles.end(), line1.begin(), line1.end());
		// corner will be duplicated so remove it
		regionTiles.insert(regionTiles.end(), line2.begin()++, line2.end());
	}
	// otherwise, follow gradient
	else
	{
		vector<Cell<T> *> regionTiles = GetPassageCells(bestCellA, bestCellB);
		// return GetPassageCells(A, B, bestCellA, bestCellB, fill);
	}

	if (regionTiles.size() == 0)
		return NULL;

	for (Cell<T> *cell : regionTiles)
	{
		DrawCircle(cell->pos, 1, fill);
	}

	Region<T>::ConnectRegions(A, B);

	Region<T> *out = new Region<T>(regionTiles);

	Region<T>::ConnectRegions(out, A);
	Region<T>::ConnectRegions(out, B);

	return out;
}

#pragma endregion helper_functions

#pragma region GRID_METHODS
//========================GRID METHODS================================
template <typename T>
bool Grid<T>::InMapRange(int x, int y) const
{
	return x >= 0 && x < _width && y >= 0 && y < _height;
}

// create a floor with the given dimensions
template <typename T>
Grid<T>::Grid(int width, int height, CellFactory<T> *factory) : _width(width), _height(height), _factory(factory)
{
	for (int x = 0; x < _width; x++)
	{
		// vector< Cell* >tmp(_height);
		vector<Cell<T> *> tmp;
		for (int y = 0; y < _height; y++)
		{
			tmp.push_back(_factory->CreateCell(x, y));
		}
		_map.push_back(tmp);
	}
}

// free all of our Cells
template <typename T>
Grid<T>::~Grid()
{
	// calling vector clear calls the destructor for each element but not delete
	for (auto &row : _map)
	{
		for (Cell<T> *cell : row)
		{
			delete cell;
		}
	}
}

template <typename T>
void Grid<T>::Clear()
{
	// free all of the Cells
	/*for(int x = 0; x < _width; x++)
	{
		for(int y = 0; y < _height; y++)
		{
			delete _map[x][y];
		}
	}*/

	_map.clear();

	for (int i = 0; i < _width; i++)
	{
		// vector< Cell* >tmp(_height);
		vector<Cell<T> *> tmp;
		for (int j = 0; j < _height; j++)
		{
			tmp.push_back(_factory->CreateCell(i, j));
		}
		_map.push_back(tmp);
	}

	// cout<<"DEBUG"<<std::endl;
	// cout<<*this;
}

// a Cell is defined as an outline when any of the surrounding
// Cells is an "empty" spot (assuming the Cell in question isn't)
// empty itself
template <typename T>
bool Grid<T>::IsOutlineCell(int x, int y, Cell<T> *reference)
{
	if (!InMapRange(x, y))
		return false;

	// check all of the surrounding Cells,
	for (int i = x - 1; i < x + 2; i++)
	{
		for (int j = y - 1; j < y + 2; j++)
		{
			if (i == x && j == y)
				continue;
			if (i != x && j != y)
				continue;
			if (!InMapRange(i, j))
				continue;

			if (_map[i][j] == reference)
				return true;
		}
	}
	return false;
}

// floodfill to get all Cells in the region
template <typename T>
vector<vec2> Grid<T>::GetRegionCells(int sx, int sy, bool (*compare)(const T A, const T B))
{
	vector<vec2> Cells;

	// https://en.cppreference.com/w/cpp/language/lambda
	// if the compare function is null, just compare pointer values
	auto default_compare = [](const T A, const T B) -> bool
	{
		if (A == B)
			return true;
		else
			return false;
	};

	if (compare == NULL)
	{
		cout << "Compare function null" << std::endl;
	}

	// check if Cell was looked at yet
	// need to initialize each value, however, look at alternatives
	vector<vector<int>> flags;
	for (int i = 0; i < _width; i++)
	{
		vector<int> tmp;

		for (int j = 0; j < _height; j++)
		{
			tmp.push_back(0);
		}
		flags.push_back(tmp);
	}

	T CellType = _map[sx][sy]->data;

	queue<vec2> q;
	q.push(vec2{sx, sy});

	flags[sx][sy] = 1;

	// breadth first search across Cells
	while (!q.empty())
	{
		vec2 t = q.front();
		q.pop();
		Cells.push_back(t);

		for (int x = t.x - 1; x <= t.x + 1; x++)
		{
			for (int y = t.y - 1; y <= t.y + 1; y++)
			{
				if (InMapRange(x, y) && (y == t.y || x == t.x) && flags[x][y] == 0)
				{
					if (compare)
					{
						if (compare(CellType, _map[x][y]->data))
						{
							flags[x][y] = 1;
							q.push(vec2{x, y});
						}
					}
					else
					{
						if (default_compare(CellType, _map[x][y]->data))
						{
							flags[x][y] = 1;
							q.push(vec2{x, y});
						}
					}
				}
			}
		}
	}

	return Cells;
}

template <typename T>
vector<vector<vec2>> Grid<T>::GetRegions(const T CellType, bool (*compare)(const T MapCell, const T CellType))
{
	vector<vector<vec2>> regions;

	auto default_compare = [](const T A, T B) -> bool
	{
		if (A == B)
			return true;
		else
			return false;
	};

	if (compare == NULL)
	{
		cout << "Compare function null" << std::endl;
	}

	// flags to determine which Cells have already been visited
	vector<vector<int>> flags;
	for (int i = 0; i < _width; i++)
	{
		vector<int> tmp;

		for (int j = 0; j < _height; j++)
		{
			tmp.push_back(0);
		}
		flags.push_back(tmp);
	}

	// iterate over every Cell, could optimize this fairly easily
	// by removing locations from the set as we touch them
	for (int x = 0; x < _width; x++)
	{
		for (int y = 0; y < _height; y++)
		{
			if (compare && compare(_map[x][y]->data, CellType))
			{
				if (flags[x][y] == 0)
				{
					// cout<<"getting region..."<<std::endl;
					vector<vec2> newRegion = GetRegionCells(x, y, compare);
					// cout<<"got region"<<std::endl;
					regions.push_back(newRegion);

					for (int i = 0; i < newRegion.size(); i++)
					{
						vec2 t = newRegion[i];
						flags[t.x][t.y] = 1;
					}
				}
			}
			else
			{
				if (default_compare(_map[x][y]->data, CellType))
				{
					if (flags[x][y] == 0)
					{
						// cout<<"getting region..."<<std::endl;
						vector<vec2> newRegion = GetRegionCells(x, y, compare);
						// cout<<"got region"<<std::endl;
						regions.push_back(newRegion);

						for (int i = 0; i < newRegion.size(); i++)
						{
							vec2 t = newRegion[i];
							flags[t.x][t.y] = 1;
						}
					}
				}
			}
		}
	}
	cout << "Done Getting " << regions.size() << " Regions" << std::endl;
	return regions;
}

// get the line of Cells connecting start to end
template <typename T>
vector<vec2> Grid<T>::GetLine(vec2 start, vec2 end)
{
	// cout<<"Getting Line"<<std::endl;
	vector<vec2> line;

	int x = start.x;
	int y = start.y;

	int dx = end.x - x;
	int dy = end.y - y;

	bool inverted = false;

	int step = (dx < 0) ? -1 : 1;
	int gradientStep = (dy < 0) ? -1 : 1;

	int longest = abs(dx);
	int shortest = abs(dy);

	if (longest < shortest)
	{
		inverted = true;
		longest = abs(dy);
		shortest = abs(dx);

		step = (dy < 0) ? -1 : 1;
		gradientStep = (dx < 0) ? -1 : 1;
	}

	int gradientAccumulation = longest / 2;

	for (int i = 0; i < longest; i++)
	{
		line.push_back(vec2{x, y});
		// cout<<x<< " " << y <<"=>";

		if (inverted)
			y += step;
		else
			x += step;

		gradientAccumulation += shortest;

		if (gradientAccumulation >= longest)
		{
			if (inverted)
				x += gradientStep;
			else
				y += gradientStep;

			gradientAccumulation -= longest;
		}
	}
	// cout<<std::endl;
	return line;
}

template <typename T>
void Grid<T>::DrawCircle(vec2 t, int r, T fill)
{
	// cout<<"Drawing circle at point "<<t.x<<" "<<t.y<<" "<<"with radius "<<r<<std::endl;
	for (int x = -r; x <= r; x++)
	{
		for (int y = -r; y <= r; y++)
		{
			if (x * x + y * y <= r * r)
			{
				int mapX = t.x + x;
				int mapY = t.y + y;

				if (InMapRange(mapX, mapY))
				{
					// if (debug)
					//	_map[mapX][mapY] = fill;
					// else
					_map[mapX][mapY]->data = fill;
				}
			}
		}
	}
}

template <typename T>
void Grid<T>::LoadFloor()
{
	string s;
	cout << "Path to file: ";
	getline(cin, s);

	LoadFloor(s);
}

// TODO: rewrite this
// how floors are loaded and serialized will depend on the project tbh
template <typename T>
void Grid<T>::LoadFloor(string path)
{
}

template <typename T>
void Grid<T>::SaveFloor()
{
	string s;
	cout << "Path to file: ";
	getline(cin, s);

	SaveFloor(s);
}

// TODO: rewrite this
// how floors are loaded and serialized will depend on the project tbh
template <typename T>
void Grid<T>::SaveFloor(string path)
{
}

template <typename T>
void Grid<T>::ImportFloor(Cell<T> **grid, int width, int height)
{
	Clear();

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			_map[x][y] = grid[y * width + x];
		}
	}

	cout << "Done importing floor" << std::endl;
}

template <typename T>
Cell<T> **Grid<T>::ExportFloor()
{
	Cell<T> **output = new Cell<T> *[_width * _height];

	for (int x = 0; x < _width; x++)
	{
		for (int y = 0; y < _height; y++)
		{
			output[y * _width + x] = _map[x][y];
		}
	}

	return output;
}

#pragma endregion GRID_METHODS

#endif