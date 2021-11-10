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

#include "MapGenerator.h"
#include "argParser.h"

using std::cout;
using std::cin;
using std::endl;

#define offset "		"

//maybe have an outline of some sort
void DrawStyleMap(Grid* t)
{
	for(int i = 0 ; i < t->_map.size(); i++)
	{
		for(int j = 0 ; j < t->_map[i].size(); j++)
		{
			if((t->_map[i][j] == 0 || t->_map[i][j] == 2))
			{
				cout<<".";
			}
			else
			{
				cout<<" ";
			}
		}
		cout<<std::endl;
	}
}

void DrawStyleOutlineMap(Grid* t)
{
	for(int i = 0 ; i < t->_map.size(); i++)
	{
		//cout<<i<<" "<<test->_map[i].size()<<std::endl;
		for(int j = 0 ; j < t->_map[i].size(); j++)
		{
			if((t->_map[i][j] == 0 || t->_map[i][j] == 2) && t->IsOutlineTile(i, j))
			{
				cout<<"x";
			}
			else if((t->_map[i][j] == 0 || t->_map[i][j] == 2))
			{
				cout<<".";
			}
			else
			{
				cout<<" ";
			}
			//cout<<t->_map[i][j];
		}
		cout<<std::endl;
	}
}

//do this eventually,,
void DrawOutline(Grid* t)
{
	
}

int main(int argx, char*argv[])
{

	ArgParser AP;
    AP.AddOptions(
        {
			{ "-d", "--dimensions", "Dimensions of the floor", true },
			{"-f", "--fillpercentage", "What percent to fill the floor up to", false},
			{"-n", "--organic", "Organic styled or room-styled", false},
			{"-c", "--connect", "Fully connect map", false},
			{"-nr", "--norandom", "Don't use random seed", false},
			{"-s", "--smoothing", "Smoothing level", false},
			{"-dn", "--dense", "Dense version of non-organic map", false},
		}
    );

	int fillPercentage, smoothing;
	bool rs, connect, organic, dense;
	
	std::vector<int> dimensions;

	if(!AP.Parse(argx, argv))
	{
		return 0;
	}

	if(AP.IsSet("dimensions"))
		dimensions = AP.GetVector<int>("dimensions");
	else
		dimensions = {60, 50};

	if(AP.IsSet("fillpercentage"))
		fillPercentage = AP.Get<int>("fillpercentage");
	else
		fillPercentage = 50;

	if(AP.IsSet("smoothing"))
		smoothing = AP.Get<int>("smoothing");
	else
		smoothing = 2;


	rs = !(AP.Get<bool>("norandom"));
	connect = AP.Get<bool>("connect");
	organic = AP.Get<bool>("organic");
	dense = AP.Get<bool>("dense");

	RoomSystem RC;
	MapSystem MC;

	MC.setPerlin(true);
	
	/*Grid* test = 
		new Grid(dimensions[0], dimensions[1], fillPercentage, rs, connect);*/
	
	Grid* test = new Grid(dimensions[0], dimensions[1]);

	if(AP.IsSet("fillpercentage"))
		fillPercentage = AP.Get<int>("fillpercentage");
	else
		fillPercentage = 10;
	
	//inorganic dungeon
	if( !organic )
	{
		RC.Generate(test, fillPercentage, dense);
	}
	else
	{
		MC.Generate(test, fillPercentage, rs, 0, smoothing, connect);
	}
	
		
	cout<<(*test)<<std::endl;
		
	int x;
	std::string name;
		
	while(x!=-1)
	{
		cout<<"0. Generate New Map\n1. Save map\n2. Load Map\n3. StyleMap";
		cout<<"\n4. OutlineMap\n5. Process\n6. Toggle Connect\n-1. Quit\n\n";
		cin >> x;

		//need to check if actually read a char, but eh.

		switch(x)
		{
			case 0:
				if (organic)
					MC.Generate(test, fillPercentage, rs, 0, smoothing, connect);
				else
				{
					test->Clear();
					RC.Generate(test, fillPercentage, dense);
				}
				cout<<(*test)<<std::endl;
			break;
				
			case 1:
				cout << "Enter filename."<< std::endl;
				cin >> name;
				test->SaveFloor(name);
				cout << "Saved!" <<std::endl;
			break;
				
			case 2:
				cout << "Enter filename."<< std::endl;
				cin >> name;
				test->LoadFloor(name);
				cout<<(*test)<<std::endl;
			break;
				
			case 3:
				DrawStyleMap(test);
			break;
			
			case 4:
				DrawStyleOutlineMap(test);
			break;
				
			case 5:
				MC.ProcessRooms(test, 9, 3, 1);
				cout<<(*test)<<std::endl;
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

	delete test;

	return 0;
}


