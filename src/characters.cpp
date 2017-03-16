
#include <stdio.h>
#include <iostream>
#include <string>
#include <Vector>
#include <fstream>

#include "characters.h"

using std::cout;
using std::cin;

//need to call character constructor seperately in order to change the default values
//only applicable if it isn't inheriting from something that already does this?

Character::Character()
	{
	
	
	}

//===========================================================HERO
Hero::Hero():
	Character()
	{
		
		
	}

//===========================================================ENEMY
Enemy::Enemy():
	Character()
	{
		
		
	}

//===========================================================NPC	
	
NPC::NPC():
	Character()
	{
		
		
	}
	
//===========================================================ALLY	
	
Ally::Ally():
	Character(), NPC()
	{
		
		
	}
	
//===========================================================BOSS
	
Boss::Boss():
	Character(), Enemy()
	{
		
		
	}

//===========================================================RIVAL	
	
Rival::Rival():
	Character(), Enemy(), NPC()
	{
		
		
	}



