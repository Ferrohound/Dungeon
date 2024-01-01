/*
	Basis for most of the procedural generators in the suite
	A 2D grid with dimensions x, y made up of tiles
*/

#ifndef CELL_H
#define CELL_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "vec.h"

template <typename T>
struct Cell
{
	Cell(T initData, vec2 position = {-1, -1}) : pos(position), data(initData)
	{
	}

	Cell(T initData, int x = -1, int y = -1) : pos({x, y}), data(initData)
	{
	}

	~Cell()
	{
	}

	virtual void print(std::ostream &output) const { output << "<>"; }

	vec2 pos;
	T data;

	friend std::ostream &operator<<(std::ostream &output, const Cell &t)
	{
		t.print(output);
		return output;
	}
};

template <typename T>
class CellFactory
{
	T _defaultValue;

public:
	CellFactory(T dv) : _defaultValue(dv) {}
	virtual Cell<T> *CreateCell(int x = -1, int y = -1, T data = T())
	{
		if (data == T{})
		{
			data = _defaultValue;
		}

		return new Cell<T>(data, x, y);
	}
};

// #include "Cell.cpp"

#endif