
#ifndef __LevelDef_H__
#define __LevelDef_H__


#include "cocos2d.h"
using namespace cocos2d;

class InventoryItem;
class GameWorld;

struct LevelDef
{
	enum WinConditions
	{
		UnknownWin = -1,
		EnterAreaWin = 0,
	};
	enum LoseConditions
	{
		UnknownLose = -1,
		EnterAreaLose = 0,
	};

	// level description
	std::string name;
	float difficulty;

	// level style
	std::string theme;

	// level physical world
	GameWorld *gameWorld;

	// inventory items
	vector< InventoryItem* > inventoryItems;

	// win and lose conditions
	WinConditions winConditions;
	LoseConditions loseConditions;

public:
	LevelDef( );
	~LevelDef( );

public:
	static LevelDef *loadFromFile( const char *fileName );
	bool saveToFile( const char *fileName );

};


#endif
