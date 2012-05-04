
#ifndef  _CONSTRUCTOR_JSON_H_
#define  _CONSTURCTOR_JSON_H_


#include "b2dJson.h"

struct InventoryItem;
class GameObject;
struct LevelDef;

class ConstructorJSon : public b2dJson
{
protected:
    std::map<InventoryItem*,int> m_inventoryItemIndexMap;
    std::map<GameObject*,int> m_gameObjectIndexMap;
	std::vector<InventoryItem*> m_inventoryItems;
	std::vector<GameObject*> m_gameObjects;

    std::map<InventoryItem*,string> m_inventoryItemToNameMap;
    std::map<GameObject*,string> m_gameObjectToNameMap;

public:
	ConstructorJSon( );

	Json::Value cj( LevelDef* levelDef );
	Json::Value b2j( const b2Shape* shape );
	Json::Value b2j( const b2FixtureDef* fixtureDef );
	Json::Value cj( InventoryItem* item );
	// Json::Value cj( GameObject* object );
	using b2dJson::b2j;

	LevelDef* j2cLevelDef( Json::Value levelDefValue );
	b2Shape* j2b2Shape( Json::Value shapeValue );
	b2FixtureDef* j2b2FixtureDef( Json::Value fixtureDefValue );
	InventoryItem* j2cInventoryItem( Json::Value inventoryItemValue, bool *available );
	//GameObject* j2cGameObject( Json::Value gameObjectValue );

	void setInventoryItemName( InventoryItem* item, const char* name );
	void setGameObjectName( GameObject* object, const char* name );

	InventoryItem* getInventoryItemByName( string name );
	GameObject* getGameObjectByName( string name );

protected:
	int lookupInventoryItemIndex( InventoryItem* item );
	int lookupGameObjectIndex( GameObject* object );

    std::string getInventoryItemName( InventoryItem* item );
    std::string getGameObjectName( GameObject* object );

};


#endif
