#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <fstream>

//#include <atomic>
//#include <chrono>

#include "world.h"

#define offset "		"

using std::string;
using std::cout;
using std::cin;
using std::endl;

struct Action{
	
	union
	{
		
	};
	
	bool _keyDown;
};



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
		///void battle(Enemy** enemies);
		
		//actions within each game state
		//for now just move in explore
		void move();
		
		
		//UI essentially
		void Exit();
		void Save();
		void Load(const string& path);
		
		
		void loadWorld(const string& path);
		
	private:
		//array of all the worlds
		//World** _universe;
		//index of whatever world the player is on
		int _index;
		//the input key
		char _key;
		//battle or explore state
		int _state;
		
		//time
		//std::chrono::high_resolution_clock::time_point _current_time;
	
};