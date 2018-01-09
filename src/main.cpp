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


int main(int argx, char*argv[]){
	
	//Engine* engine = new Engine();

	//engine->run(0);
	
	//delete engine;
	
	Floor* test = new Floor(50, 50, 50);
	
	for(int i = 0 ; i < test->_map.size(); i++)
	{
		//cout<<i<<" "<<test->_map[i].size()<<std::endl;
		for(int j = 0 ; j < test->_map[i].size(); j++)
		{
			cout<<test->_map[i][j];
		}
		cout<<std::endl;
	}
	
	int x;
	cin >> x;
	
	while(x!=-1)
	{
		test->Generate(50, false, 0, 5);
		for(int i = 0 ; i < test->_map.size(); i++)
		{
			//cout<<i<<" "<<test->_map[i].size()<<std::endl;
			for(int j = 0 ; j < test->_map[i].size(); j++)
			{
				cout<<test->_map[i][j];
			}
			cout<<std::endl;
		}
		cin >> x;
	}
	
	delete test;
	return 0;
}


