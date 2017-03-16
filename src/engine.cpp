#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>

#include "engine.h"

#define offset "		"

using std::string;
using std::cout;
using std::cin;
using std::endl;

Engine::Engine()
{
	cout<<"Creating..."<<endl;
	_universe = new World*[1];
	_universe[0] = new World("Earth", 1);
	cout<<"Done creating..."<<endl;
	
	_index = 0;
}

Engine::~Engine()
{
	//clear everything
	delete _universe[0];
	delete _universe;
}

//===========================Run====================================
void Engine::run(int state)
{
	//pretty much just transitions to the game states
	switch(state)
	{
		case 0:
		explore();
		break;
		
		case 1:
		explore();
		break;
		
		case 2:
		explore();
		break;
		
		case 3:
		explore();
		break;
	}
	
}

//===============================Game States=================================== 

//ideally load this from a file 
void Engine::intro()
{
	system("cls");
	cout<<"Here we go, kiddo!"<<endl;
	cin.ignore();
	cout<<"This is you. ------------->	O"<<endl;
	cout<<"These are your enemies.------------>	x y z"<<endl;
	cin.ignore();
	system("cls");
	cout<<"Wipe them out."<<endl;
	cin.ignore();
	system("cls");
	cout<<"Use arrow keys or WASD to move"<<endl;
	cout<<"Press escape to quit"<<endl;
	cin.ignore();
}


void Engine::explore()
{
	char key;
	system("cls");
	_universe[0]->print();
	float cooldown = 0.0f;
	while (true)
	{
		
		SHORT W = GetAsyncKeyState( 0x57 );
		SHORT A = GetAsyncKeyState( 0x41 );
		SHORT S = GetAsyncKeyState( 0x53 );
		SHORT D = GetAsyncKeyState( 0x44 );
		
		SHORT ESC = GetAsyncKeyState( VK_ESCAPE );
		SHORT SPC = GetAsyncKeyState( 0x20 );
		
		SHORT ENTR = GetAsyncKeyState( 0x0D );
		
		bool keyDown = false;
		
		//add cases for each direction pressed, pause menu, etc...
		if( ( 1 << 16 ) & ESC && !keyDown)
		{
			keyDown = true;
			system("cls");
			Exit();
			_universe[0]->print();
			keyDown = false;
		}
		//player movement (WASD)
		else if( (( 1 << 16 ) & W || ( 1 << 16 ) & A
			|| ( 1 << 16 ) & S || ( 1 << 16 ) & D) && !keyDown&&cooldown>1000)
		{
			keyDown = true;
			system("cls");
			_universe[0]->print();
			cout<<"MOVED!"<<endl;
			move();
			keyDown = false;
			cooldown = 0;
		}
		//pause/menu
		else if(( 1 << 16 ) & ENTR && !keyDown && cooldown>1000)
		{
			keyDown = true;
			system("cls");
			menu();
			cooldown = 0;
			_universe[0]->print();
			keyDown = false;
		}
		
		//space bar
		else if( ( 1 << 16 ) & SPC && !keyDown &&cooldown>1000)
		{
			keyDown = true;
			system("cls");
			cooldown = 0;
			_universe[0]->print();
			cout<<"ACTION!"<<endl;
			keyDown = false;
		}
		cooldown+=0.01f;
	}
	
	// ==================movement========================
	//system("cls") is clear screen
	//system("pause") probably freezes a while loop or whatever
	//Sleep(10) pauses the system for 10 milliseconds
	
	return;
}

void Engine::battle(Enemy** enemies)
{
	
	
}

//================================MOVE=========================================================

void Engine::move()
{
	_universe[_index]->move();
	
}

//================================Menu, Exit, Load, Save=========================================

void Engine::menu()
{
	char key;
	system("cls");
	cout<<"MENU"<<endl<<"Return to game? y/n"<<endl;
	cin>>key;
	switch(key){
		case 'n':
			exit(0);
		case 'y':
		default:
			break;
	}
	return;
}

void Engine::Exit()
{
	char key;
	system("cls");
	cout<<"Do you want to quit? (y/n + Enter)"<<endl;
	cin>>key;
	switch(key){
		case 'y':
			exit(0);
		case 'n':
		default:
			break;
	}
	return;
}

void Engine::Save()
{
	
}

void Engine::Load(const string& path)
{
	
}