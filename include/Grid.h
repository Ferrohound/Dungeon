/*
    Basis for most of the procedural generators in the suite
    A 2D grid with dimensions x, y made up of tiles

	It is the job of the generators to populate the grid's tile data
	To save on memory, with static tile types, just use pointers to point to the same
	instance of the tile types
*/

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


using std::cout;
using std::cin;
using std::endl;

using std::string;
using std::vector;
using std::queue;

struct Tile{
	Tile();
	//Tile(Vector2 position);
	~Tile();

	//Vector2 pos;
	// int x, y;

	friend std::ostream &operator<<( std::ostream &output, const Tile &T )
	{
		output<<"<>";
		return output;
	}
};

class TileFactory{
	public:
		virtual Tile* CreateTile();
};

class Grid{
	public:
		Grid( int width, int height, TileFactory* factory );
		~Grid();
		
		bool InMapRange(int x, int y);

		//get regions of the same type of tile using floodfill
		vector< std::pair<int, int> > GetRegionTiles(int sx, int sy, bool (*compare)(Tile* A, Tile* B) = NULL);
		vector< vector< std::pair<int, int>> > GetRegions (Tile* tileType,  bool (*compare)(Tile* A, Tile* B) = NULL);
		
		//is the given position an outline tile (reference)
		bool IsOutlineTile(int x, int y, Tile* reference);
		void DrawCircle(std::pair<int, int> t, int r, Tile* fill);
		
		vector<std::pair<int, int>> GetLine(std::pair<int, int> start, std::pair<int, int> end);

		//save a floor and a load a floor from a given path
		void LoadFloor(string path);
		void SaveFloor(string path);
		
		//load and save floor from cin
		void LoadFloor();
		void SaveFloor();

		//read in and put out an int array grid
		void ImportFloor(Tile** grid, int width, int height);
		Tile** ExportFloor();
		
		void Clear();
		
		vector < vector<int> > GetMap();
		
		int GetWidth() { return _width; }
		int GetHeight() { return _height; }
		
		//overload the cout
		friend std::ostream &operator<<( std::ostream &output, const Grid &F )
		{
			output << "Grid is : "<<std::endl;
			for(int i = 0; i < F._width ; i++)
			{
				for(int j = 0; j < F._height ; j++)
				{
					output << F._map[i][j];
				}
				output << '\n';
			}
			return output; 
		}
		
		//no real reason to use a vector here, could be a float double array and use a
		//get(x, y) function
		std::vector< vector< Tile* > > _map;
		bool debug;
	private:
		int _width, _height;
		TileFactory* _factory;
};