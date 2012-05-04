
#ifndef __LevelDef_H__
#define __LevelDef_H__

#include <string>
#include <vector>

class InventoryItem;
class GameWorld;
class GameObject;

struct LevelDescription;
struct b2FixtureDef;


enum ObjectType
{
	Undefined   = -1,
	SimpleBox   = 0,
	Area		= 1,
	Spring      = 3,
	Pin			= 4,
	Glue		= 5
};

struct InventoryItem
{
	std::string name;
	ObjectType type;
	std::string itemSpritePath, objectSpritePath;

	bool available;
	int maxQuantity;

	bool isStatic, isMovable, isRotatable, isDeletable;

	b2FixtureDef *fixtureDef;
};

struct LevelDef
{
	enum WinConditions
	{
		UnknownWin = -1,
		EnterAreaWin = 0
	};
	enum LoseConditions
	{
		UnknownLose = -1,
		EnterAreaLose = 0
	};

	// level description
	std::string name;
	float difficulty;

	// level style
	std::string theme;

	// level physical world
	GameWorld *gameWorld;

	// inventory items
	std::vector< InventoryItem* > inventoryItems;

	// game objects
	std::vector<GameObject*> gameObjects;
	GameObject *target;
	GameObject *winArea;

	// win and lose conditions
	WinConditions winConditions;
	LoseConditions loseConditions;

public:
	LevelDef( );
	~LevelDef( );

public:
	static LevelDef *loadFromFile( const std::string &s );
	bool saveToFile( const std::string &p );

};

std::ostream& operator<<( std::ostream &out, LevelDef &l );


#endif
