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

#include "engine.h"

using std::cout;
using std::cin;
using std::endl;

#define offset "		"

//don't forget to pass by reference, not value~!
//use a hash table to store the ascii letters
//give each character/monster ascii art too


int main(int argx, char*argv[]){
	
	Engine* engine = new Engine();

	engine->run(0);
	
	delete engine;
	return 0;
}


