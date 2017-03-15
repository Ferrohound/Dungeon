/*
Sample procedurally generated dungeon crawler


Author: Ferrohound
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <fstream>

#include "characters.h"
#include "world.h"
#include "engine.h"


using std::cout;
using std::cin;
using std::endl;

#define offset "		"

//don't forget to pass by reference, not value~!
//use a hash table to store the ascii letters
//give each character/monster ascii art too


int main(int argx, char*argv[]){
	
	intro();
	
	Engine engine();
	
	World** universe = new World*[1];
	universe[0] = new World("Test",1);
	
	universe[0]->getFloor(0)->print();
	
	char key;
	
	engine.run(0);
	
	return 0;
}


