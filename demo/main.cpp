/*
	Sample procedurally generated dungeon crawler

	See LICENSE.txt
	Author: Ferrohound

	Seems like floor generates organic maps
	RoomSystem generates the inorganic, no binary space partitioning used

*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

#include "Floor.h"
#include "MapSystem.h"
#include "PseudoPhysics.h"
#include "NumTile.h"
#include "argParser.h"

using std::cin;
using std::cout;
using std::endl;

#define offset "		"

// maybe have an outline of some sort
void DrawStyleMap(Floor<int> *f)
{
	Grid<int> *t = f->grid;
	for (int i = 0; i < t->_map.size(); i++)
	{
		for (int j = 0; j < t->_map[i].size(); j++)
		{
			if ((t->_map[i][j]->data == numtiles[1].data || t->_map[i][j]->data == numtiles[2].data))
			{
				cout << ".";
			}
			else
			{
				cout << " ";
			}
		}
		cout << std::endl;
	}
}

void DrawStyleOutlineMap(Floor<int> *f)
{
	Grid<int> *t = f->grid;
	for (int i = 0; i < t->_map.size(); i++)
	{
		// cout<<i<<" "<<floor->_map[i].size()<<std::endl;
		for (int j = 0; j < t->_map[i].size(); j++)
		{
			if ((t->_map[i][j]->data == numtiles[1].data ||
				 t->_map[i][j]->data == numtiles[2].data) &&
				t->IsOutlineCell(i, j, &numtiles[1]))
			{
				cout << "x";
			}
			else if ((t->_map[i][j]->data == numtiles[1].data || t->_map[i][j]->data == numtiles[2].data))
			{
				cout << ".";
			}
			else
			{
				cout << " ";
			}
			// cout<<t->_map[i][j];
		}
		cout << std::endl;
	}
}

// do this eventually,,
void DrawOutline(Grid<int> *t)
{
}

int main(int argx, char *argv[])
{

	ArgParser AP;
	AP.AddOptions(
		{
			{"-d", "--dimensions", "Dimensions of the floor", true},
			{"-f", "--fillpercentage", "What percent to fill the floor up to", false},
			{"-n", "--organic", "Organic styled or room-styled", false},
			{"-c", "--connect", "Fully connect map", false},
			{"-nr", "--norandom", "Don't use random seed", false},
			{"-s", "--smoothing", "Smoothing level", false},
			{"-dn", "--dense", "Dense version of non-organic map", false},
		});

	int fillPercentage, smoothing;
	bool rs, connect, organic, dense;

	std::vector<int> dimensions;

	if (!AP.Parse(argx, argv))
	{
		return 0;
	}

	if (AP.IsSet("dimensions"))
		dimensions = AP.GetVector<int>("dimensions");
	else
		dimensions = {60, 50};

	if (AP.IsSet("fillpercentage"))
		fillPercentage = AP.Get<int>("fillpercentage");
	else
		fillPercentage = 50;

	if (AP.IsSet("smoothing"))
		smoothing = AP.Get<int>("smoothing");
	else
		smoothing = 1;

	rs = !(AP.Get<bool>("norandom"));
	connect = AP.Get<bool>("connect");
	organic = AP.Get<bool>("organic");
	dense = AP.Get<bool>("dense");

	// RoomSystem RC;
	// MapSystem MC;

	PhysicsSystem InorganicGenerator;
	MapSystem OrganicGenerator(numtiles);

	NumTileFactory f;

	Floor<int> *floor;

	if (AP.IsSet("fillpercentage"))
		fillPercentage = AP.Get<int>("fillpercentage");
	else
		fillPercentage = 10;

	// inorganic dungeon
	if (!organic)
	{
		floor = InorganicGenerator.Generate(dimensions[0], dimensions[1], fillPercentage, dense, -1, -1, 4);
		// RC.Generate(floor, fillPercentage, dense);
	}
	else
	{
		floor = OrganicGenerator.Generate(dimensions[0], dimensions[1], fillPercentage, rs, 0, smoothing, connect);
		// MC.Generate(floor, fillPercentage, rs, 0, smoothing, connect);
	}

	cout << (*floor) << std::endl;

	int x;
	std::string name;

	while (x != -1)
	{
		cout << "0. Generate New Map\n1. Save map\n2. Load Map\n3. StyleMap";
		cout << "\n4. OutlineMap\n5. Process\n6. Toggle Connect\n-1. Quit\n\n";
		cin >> x;

		// need to check if actually read a char, but eh.

		switch (x)
		{
		case 0:
			if (organic)
			{
				OrganicGenerator.Generate(floor, fillPercentage, rs, 0, smoothing, connect);
				// MC.Generate(floor, fillPercentage, rs, 0, smoothing, connect);
			}
			else
			{
				InorganicGenerator.Generate(floor, fillPercentage, dense, -1, -1, 4);
			}
			cout << (*floor) << std::endl;
			break;

		case 1:
			cout << "Enter filename." << std::endl;
			cin >> name;
			floor->Save(name);
			cout << "Saved!" << std::endl;
			break;

		case 2:
			cout << "Enter filename." << std::endl;
			cin >> name;
			floor->Load(name);
			cout << (*floor) << std::endl;
			break;

		case 3:
			DrawStyleMap(floor);
			break;

		case 4:
			DrawStyleOutlineMap(floor);
			break;

		case 5:
			// MC.ProcessRooms(floor, 9, 3, 1);
			OrganicGenerator.ProcessRooms(*floor, 9, 3, 1);
			cout << (*floor) << std::endl;
			break;

		case 6:
			connect = !connect;
			break;

		case -1:
			break;

		default:
			break;
		}
	}

	delete floor;

	return 0;
}
