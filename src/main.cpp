/*
Sample procedurally generated dungeon crawler

See LICENSE.txt
Author: Ferrohound
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

#include"world.h"
#include "BSP.h"

using std::cout;
using std::cin;
using std::endl;

#define offset "		"

//don't forget to pass by reference, not value~!
//use a hash table to store the ascii letters
//give each character/monster ascii art too

//maybe have an outline of some sort
void DrawStyleMap(Floor* t)
{
	for(int i = 0 ; i < t->_map.size(); i++)
	{
		//cout<<i<<" "<<test->_map[i].size()<<std::endl;
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
			//cout<<t->_map[i][j];
		}
		cout<<std::endl;
	}
}

void DrawStyleOutlineMap(Floor* t)
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
void DrawOutline(Floor* t)
{
	
}

int main(int argx, char*argv[]){
	
	//Engine* engine = new Engine();

	//engine->run(0);
	
	//delete engine;
	
	//organic dungeon
	if(argx == 1 )
	{
		
		Floor* test = new Floor(50, 50, 50, true);
		
		cout<<(*test)<<std::endl;
		
		int x;
		cout<<"0. Generate New Map\n1. Save map\n2. Load Map\n3. StyleMap";
			cout<<"\n4. OutlineMap\n5. Process\n-1. Quit\n\n";
		cin >> x;
		
		while(x!=-1)
		{
			switch(x)
			{
				case 0:
					test->Generate(50, true, 0, 5);
					cout<<(*test)<<std::endl;
				break;
				
				case 1:
					test->SaveFloor("test.txt");
				break;
				
				case 2:
					test->LoadFloor("test.txt");
					cout<<(*test)<<std::endl;
				break;
				
				case 3:
					DrawStyleMap(test);
				break;
				
				case 4:
					DrawStyleOutlineMap(test);
				break;
				
				case 5:
					test->ProcessRooms(9, 3, 1);
					cout<<(*test)<<std::endl;
				break;
				
				case -1:
				break;
				
				default:
				break;
			}
			
			cout<<"0. Generate New Map\n1. Save map\n2. Load Map\n3. StyleMap";
			cout<<"\n4. OutlineMap\n5. Process\n-1. Quit\n\n";
			cin >> x;
		}
		
		delete test;
	}
	
	else
	{
		
	}
	return 0;
}


