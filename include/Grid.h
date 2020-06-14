/*
    Basis for most of the procedural generators in the suite

    A simple grid with dimensions x, y with a tile helper class

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

#include "vec.h"
//#include <utility> /*std::pair, std::make_pair */

using std::cout;
using std::cin;
using std::endl;

using std::string;
using std::vector;
using std::queue;

struct Tile{
	Tile();
	Tile(int _x, int _y);
	//Tile(Vector2 position);
	~Tile();

	//Vector2 pos;
	int x, y;
};

class Grid{
	public:
		Grid( int width, int height );
		
		bool InMapRange(int x, int y);

		//get regions of the same type of tile using floodfill
		vector<Tile> GetRegionTiles(int sx, int sy);
		vector< vector<Tile> > GetRegions (int tileType);
		
		//is the given position an outline tile
		bool IsOutlineTile(int x, int y);
		void DrawCircle(Tile t, int r, int fill = 2);
		
		vector<Tile> GetLine(Tile start, Tile end);
        int GetAvgWallValue(int sx, int sy, int radius);
        int GetSurroundingWallCount(int sx, int sy);
		
		//save a floor and a load a floor from a given path
		void LoadFloor(string path);
		void SaveFloor(string path);
		
		void LoadFloor();
		void SaveFloor();

		//read in and put out an int array grid
		void ImportFloor(float* grid, int width, int height);
		float* ExportFloor();
		
		void Clear();
		
		vector < vector<int> > GetMap();
		
		int GetWidth() { return _width; }
		int GetHeight() { return _height; }
		
		//this doesn't work for some reason
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
		std::vector< vector<int> > _map;
		bool debug;
	private:
		int _width, _height;
};