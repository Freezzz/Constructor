
#ifndef  _CONSTRUCTOR_JSON_H_
#define  _CONSTURCTOR_JSON_H_


#include "Serialization/b2dJson.h"

class InventoryItem;
class GameObject;
class LevelDef;

class ConstructorJSon : public b2dJson
{
protected:
    std::map<InventoryItem*,int> m_inventoryItemIndexMap;
	std::vector<InventoryItem*> m_inventoryItems;
	
    std::map<InventoryItem*,string> m_inventoryItemToNameMap;

public:
	ConstructorJSon( );

	Json::Value cj( LevelDef* levelDef );
	Json::Value b2j( const b2Shape* shape );
	Json::Value b2j( const b2FixtureDef* fixtureDef );
	Json::Value cj( InventoryItem* inventoryItem );
	Json::Value cj( GameObject* gameObject );
	using b2dJson::b2j;

	LevelDef* j2cLevelDef( Json::Value levelDefValue );
	b2Shape* j2b2Shape( Json::Value shapeValue );
	b2FixtureDef* j2b2FixtureDef( Json::Value fixtureDefValue );
	InventoryItem* j2cInventoryItem( Json::Value inventoryItemValue );
	GameObject* j2cGameObject( Json::Value gameObjectValue );
	
	std::string getInventoryItemName( InventoryItem* item );

protected:
	int lookupInventoryItemIndex( InventoryItem* inventoryItem );

};


#endif
