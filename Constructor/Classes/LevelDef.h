
#ifndef __LevelDef_H__
#define __LevelDef_H__


#include "cocos2d.h"
#include "Serialization/json/json.h"
using namespace cocos2d;

class InventoryItem;
class GameWorld;
class GameObject;

struct LevelDescription;

namespace Json {
	class Value;
}

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
	
	// level game mode configuration
	bool controlled;

	// level style
	Json::Value theme;

	// level physical world
	GameWorld *gameWorld;

	// inventory items
	vector< InventoryItem* > inventoryItems;

	// game objects
	CCMutableArray<GameObject*> gameObjects;
	GameObject *target;
	GameObject *winArea;
	GameObject *looseArea;
	
	// win and lose conditions
	WinConditions winConditions;
	LoseConditions loseConditions;

public:
	LevelDef( );
	~LevelDef( );

public:
	static LevelDef *loadFromFile( LevelDescription *p );
	bool saveToFile( LevelDescription *p );

};


#endif
