/*
	Floor is a grid, info on its regions and connections


*/

#pragma once

#include <unordered_map>

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

	void Reset()
	{
		CellFactory<T>* f = grid->_factory;
		delete grid;
		grid = new Grid<T>(width, height, f);
	}

	void ConnectRegions(Region<T> *A, Region<T> *B, T fill, bool angular = false);
	void ConnectRegions(Region<T> *A, Region<T> *B, vec2 cA, vec2 cB, T fill, bool angular = false);
	void ConnectRegions(Region<T> *A, Region<T> *B, Cell<T> *cA, Cell<T> *cB, T fill, bool angular = false);

	void SetMainRegion(Region<T> *A)
	{
		_mainRegion = A;
	}
};

template <typename T>
void Floor<T>::ConnectRegions(Region<T> *A, Region<T> *B, T fill, bool angular)
{
	connectors.push_back(grid->ConnectRegions(A, B, angular, fill));
}

template <typename T>
void Floor<T>::ConnectRegions(Region<T> *A, Region<T> *B, vec2 cA, vec2 cB, T fill, bool angular)
{
	// TODO: this gets reused a lot, maybe have a grid function

	ConnectRegions(A, B, grid->GetCell(cA), grid->GetCell(cB), fill, angular);
}

template <typename T>
void Floor<T>::ConnectRegions(Region<T> *A, Region<T> *B, Cell<T> *cA, Cell<T> *cB, T fill, bool angular)
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
}