
#ifndef  _CONSTRUCTOR_JSON_H_
#define  _CONSTRUCTOR_JSON_H_


#include "b2dJson.h"

class InventoryItem;
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
	Json::Value cj( InventoryItem* item );
	Json::Value cj( GameObject* object );
	using b2dJson::b2j;

	LevelDef* j2cLevelDef( Json::Value levelDefValue );
	InventoryItem* j2cInventoryItem( Json::Value inventoryItemValue );
	GameObject* j2cGameObject( Json::Value gameObjectValue );

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
