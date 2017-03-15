#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <fstream>

#include "characters.h"
#include "world.h"

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
		
		void run(int state);
		
		//the four gamestates
		void intro();
		void menu();
		void explore();
		void battle(Enemy** enemies);
		
		void Exit();
		void Save();
		void Load();
		
		
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