#include <stdio.h>
#include <iostream>
#include <string>
#include <Vector>
#include <fstream>

using std::cout;
using std::cin;

/*
	Characters should have an inventory
	max storage
	hp, exp, defense, attack, agility, luck, wisdom(?)
	npcs should have a roam feature - speed
	you should be able to talk with NPCs
	attack/battle
*/

class Character{
	public:
		Character();
		int X(){ return _x;}
		int Y() { return _y;}
		int hp() { return _health;}
		
	private:
		float _health, _exp, _def, _atk, _agl, _lck, _wis, _spd;
		int _x, _y;
	
};


class Hero: public Character{
	public:
		Hero();
	
};

class Enemy: public virtual Character{
	public:
		Enemy();
	
};

class NPC: public virtual Character{
	public:
		NPC();
	
};

class Ally: public NPC{
	public:
		Ally();
};

class Boss: public Enemy{
	public:
		Boss();
};

class Rival: public NPC, public Enemy{
	public:
		Rival();
};

