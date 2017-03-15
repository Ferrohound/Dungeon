#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <fstream>

#include "characters.h"
#include "world.h"
#include "engine.h"

#define offset "		"

using std::string;
using std::cout;
using std::cin;
using std::endl;

class Engine{
	public:
		//constructor and destructor
		Engine();
		~Engine();
		
		void run();
		
		//the four gamestates
		void intro();
		void menu();
		void explore();
		void battle(Enemy** enemies);
		
		void exit();
		void save();
		void load();
		
		
		void loadWorld(const string& path);
		
	private:
		//array of all the worlds
		World** _universe;
		//index of whatever world the player is on
		int _worldIndex;
		//the input key
		char _key;
		//battle or explore state
		int _state;
	
	
};