/*
	Floor is a grid, info on its regions and connections


*/

#pragma once

#include <unordered_map>
#include <iostream>
#include <fstream>

#include "Graph.h"
#include "Grid.h"

using std::cin;
using std::cout;
using std::endl;

using std::queue;
using std::string;
using std::vector;

//======================================================================
template <typename T>
class Floor
{

	Region<T> *_mainRegion;

public:
	std::unordered_map<Region<T> *, T> regionAccesibility;
	std::vector<Region<T> *> regions;
	std::vector<Region<T> *> connectors;

	int width, height;
	Grid<T> *grid;

	Floor(int w, int h, CellFactory<T> *factory) : width(w), height(h)
	{
		grid = new Grid<T>(width, height, factory);
	}

	Floor(Grid<T> *g) : grid(g)
	{
		width = grid->width;
		height = grid->height;
	}

	// TODO: what to do about numtile factory
	~Floor()
	{
		for (auto &r : regions)
		{
			delete r;
		}

		for (auto &r : connectors)
		{
			delete r;
		}
		delete grid;
	}

	void Reset()
	{
		// TODO: this may cause problems... the factory has to
		// be made from the top level or this will be null
		CellFactory<T> *f = grid->_factory;
		delete grid;
		grid = new Grid<T>(width, height, f);
	}

	bool Save(std::string name, std::string path = "");
	bool Load(std::string name, std::string path = "");

	void AddRegion(Region<T> *A)
	{
		regions.push_back(A);
	}

	Region<T> *AddRegion(vector<Cell<T> *> cells, vector<Cell<T> *> border = vector<Cell<T> *>())
	{
		Region<T> *out = new Region<T>(cells, border);
		AddRegion(out);
		return out;
	}

	Region<T> *ConnectRegions(Region<T> *A, Region<T> *B, T fill, bool angular = false);
	Region<T> *ConnectRegions(Region<T> *A, Region<T> *B, vec2 cA, vec2 cB, T fill, bool angular = false);
	Region<T> *ConnectRegions(Region<T> *A, Region<T> *B, Cell<T> *cA, Cell<T> *cB, T fill, bool angular = false);

	void SetMainRegion(Region<T> *A)
	{
		_mainRegion = A;
	}

	// overload the cout
	friend std::ostream &operator<<(std::ostream &output, const Floor &F)
	{
		output << (*F.grid) << std::endl;
		return output;
	}
};

template <typename T>
Region<T> *Floor<T>::ConnectRegions(Region<T> *A, Region<T> *B, T fill, bool angular)
{
	auto regionTiles = grid->GetRegionConnection(A, B, angular, fill);

	if (regionTiles.size() == 0)
		return NULL;

	Region<T> *region = new Region<T>(regionTiles);

	if (region == NULL)
		return NULL;

	for (Cell<T> *cell : regionTiles)
	{
		grid->DrawCircle(cell->pos, 1, fill);
	}

	Region<T>::ConnectRegions(A, B);
	Region<T>::ConnectRegions(region, A);
	Region<T>::ConnectRegions(region, B);

	connectors.push_back(region);

	return region;
}

template <typename T>
Region<T> *Floor<T>::ConnectRegions(Region<T> *A, Region<T> *B, vec2 cA, vec2 cB, T fill, bool angular)
{
	// TODO: this gets reused a lot, maybe have a grid function

	ConnectRegions(A, B, grid->GetCell(cA), grid->GetCell(cB), fill, angular);
}

template <typename T>
Region<T> *Floor<T>::ConnectRegions(Region<T> *A, Region<T> *B, Cell<T> *cA, Cell<T> *cB, T fill, bool angular)
{
	// TODO: this gets reused a lot, maybe have a grid function

	vector<Cell<T> *> regionCells = grid->GetPassageCells(cA, cB);

	for (Cell<T> *cell : regionCells)
	{
		grid->DrawCircle(cell->pos, 1, fill);
	}

	Region<T>::ConnectRegions(A, B);

	Region<T> *newRegion = new Region<T>(regionCells);

	Region<T>::ConnectRegions(newRegion, A);
	Region<T>::ConnectRegions(newRegion, B);

	connectors.push_back(newRegion);

	return newRegion;
}

template <typename T>
bool Floor<T>::Save(std::string name, std::string path) { return true; }

template <typename T>
bool Floor<T>::Load(std::string name, std::string path) { return true; }
