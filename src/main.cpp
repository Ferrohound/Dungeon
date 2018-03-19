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

using std::cout;
using std::cin;
using std::endl;

#define offset "		"

//don't forget to pass by reference, not value~!
//use a hash table to store the ascii letters
//give each character/monster ascii art too

void DrawMap(Floor* t)
{
	for(int i = 0 ; i < t->_map.size(); i++)
	{
		//cout<<i<<" "<<test->_map[i].size()<<std::endl;
		for(int j = 0 ; j < t->_map[i].size(); j++)
		{
			cout<<t->_map[i][j];
		}
		cout<<std::endl;
	}
}


int main(int argx, char*argv[]){
	
	//Engine* engine = new Engine();

	//engine->run(0);
	
	//delete engine;
	
	Floor* test = new Floor(50, 50, 50, true);
	test->ProcessRooms(9, 3, 1);
	
	cout<<(*test)<<std::endl;
	
	int x;
	cin >> x;
	
	while(x!=-1)
	{
		switch(x)
		{
			case 0:
				test->Generate(50, true, 0, 5);
				test->ProcessRooms(9, 3, 1);
				cout<<(*test)<<std::endl;
			break;
			
			case 1:
				test->SaveFloor("test.txt");
			break;
			
			case 2:
				test->LoadFloor("test.txt");
				cout<<(*test)<<std::endl;
			break;
			
			case -1:
			break;
			
			default:
			break;
		}

		cin >> x;
	}
	
	delete test;
	return 0;
}


