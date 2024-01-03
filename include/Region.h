/*
	A region is simply a collection of Cells
*/

#ifndef REGION_H
#define REGION_H
#pragma once

#include <stdio.h>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm> /*std::find(vector.begin(), vector.end(), item)!=vector.end())*/
#include <numeric> //accumulate

#include <time.h>
// #include <utility> /*std::pair, std::make_pair */

#include "Cell.h"

using std::vector;

//============================ REGION ============================================
// eventually implement different shapes
template <typename T>
struct Region
{
	Region();
	Region(vector<Cell<T> *> fill, vector<Cell<T> *> border = vector<Cell<T> *>());
	~Region();

	// member functions
	// void SetAccessibleFromMainRegion();
	bool isConnected(Region *A);
	static int CompareRegions(Region<T> *A, Region<T> *B);

	void Fill(T fill)
	{
		for (Cell<T> *cell : cells)
		{
			cell->data = fill;
		}
	}

	bool operator==(const Region<T> *other) const;

	static void ConnectRegions(Region<T> *A, Region<T> *B);
	static void SeparateRegions(Region<T> *A, Region<T> *B);

	// member varialbes
	vector<Cell<T> *> cells; // cells is all cells, border is a subset
	vector<Cell<T> *> border;
	vector<Region<T> *> connectedRegions;
	int size, id, mX, mY, CoG;
};

// #include "Region.cpp"

#pragma region Region_methods
//======================Region METHODS==================================

template <typename T>
Region<T>::Region()
{
	// accessible = false;
	// mainRegion = false;

	size = 0;
	connectedRegions.clear();
	border.clear();
}

template <typename T>
Region<T>::Region(vector<Cell<T> *> fill, vector<Cell<T> *> border)
{
	cells = fill;
	border = border;
	size = cells.size();

	auto lambda = [count = 0](double a, int b) mutable
	{ return a + (b - a) / ++count; };

	mX = std::accumulate(cells.begin(), cells.end(), 0,
						 [](int partialSum, const Cell<T> *obj)
						 {
							 return partialSum + obj->pos.x;
						 }) /
		 size;

	mY = std::accumulate(cells.begin(), cells.end(), 0,
						 [](int partialSum, const Cell<T> *obj)
						 {
							 return partialSum + obj->pos.y;
						 }) /
		 size;

	connectedRegions.clear();
}

template <typename T>
Region<T>::~Region()
{
}

template <typename T>
void Region<T>::ConnectRegions(Region<T> *A, Region<T> *B)
{
	A->connectedRegions.push_back(B);
	B->connectedRegions.push_back(A);
}

template <typename T>
void Region<T>::SeparateRegions(Region<T> *A, Region<T> *B)
{
	vector<Region *> aFiltered;
	std::copy_if(A->connectedRegions.begin(), A->connectedRegions.end(), std::back_inserter(aFiltered), [B](Region *i)
				 { return i != B; });
	A->connectedRegions = aFiltered;

	vector<Region *> bFiltered;
	std::copy_if(B->connectedRegions.begin(), B->connectedRegions.end(), std::back_inserter(bFiltered), [A](Region *i)
				 { return i != A; });
	B->connectedRegions = bFiltered;
}

// this is a direct connection
template <typename T>
bool Region<T>::isConnected(Region<T> *B)
{
	for (int i = 0; i < connectedRegions.size(); i++)
	{
		if (connectedRegions[i]->id == B->id)
			return true;
	}
	// return (std::find(connectedRegions.begin(), connectedRegions.end(), B) != connectedRegions.end());
	return false;
}

template <typename T>
int Region<T>::CompareRegions(Region<T> *A, Region<T> *B)
{
	return A->size < B->size;
}

template <typename T>
bool Region<T>::operator==(const Region<T> *other) const
{
	return this == other;
}

#pragma endregion Region_methods

#endif