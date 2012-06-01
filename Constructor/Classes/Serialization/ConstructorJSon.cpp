
#include "ConstructorJSon.h"

#include "../GameObjects/GameObject.h"
#include "../GameObjects/ObjectSimpleBox.h"
#include "../GameObjects/ObjectArea.h"
#include "../GameObjects/ObjectGlue.h"
#include "../GameObjects/ObjectPin.h"
#include "../GameObjects/ObjectSpring.h"
#include "../Serialization/LevelManager.h"

#include "LevelDef.h"

#include "GameWorld.h"

#include <fstream>

ConstructorJSon::ConstructorJSon( )
	: b2dJson(1)
{
}

Json::Value ConstructorJSon::cj( LevelDef* levelDef )
{
	m_inventoryItemIndexMap.clear();
	m_gameObjectIndexMap.clear();
	
	Json::Value value;
	
	// level description
	value["name"] = levelDef->name;
	value["difficulty"] = levelDef->difficulty;

	// level style
	value["theme"] = levelDef->theme;

	// level physical world
	Json::Value worldJson;
	{
		setBodyName( levelDef->gameWorld->umbelicoDelMondo, "null body" );
		worldJson["physics world"] = b2j( levelDef->gameWorld->physicsWorld );
	}
	value["game world"] = worldJson;

	// inventory items
	for( unsigned int i = 0; i < levelDef->inventoryItems.size(); i++ ) {
		InventoryItem *item = levelDef->inventoryItems.at(i);
		m_inventoryItemIndexMap[item] = i;
		value["inventory items"][i] = cj( item );
	}

	// game objects
	for( unsigned int i = 0; i < levelDef->gameObjects.count(); i++ ) {
		GameObject *object = levelDef->gameObjects.getObjectAtIndex(i);
		m_gameObjectIndexMap[object] = i;
		value["game objects"][i] = cj( object );
	}
	{
		value["target"] = this->lookupGameObjectIndex( levelDef->target );
		value["win area"] = this->lookupGameObjectIndex( levelDef->winArea );
		value["loose area"] = this->lookupGameObjectIndex( levelDef->looseArea );	
	}

	// win and lose conditions
	value["win conditions"] = levelDef->winConditions;
	value["lose conditions"] = levelDef->loseConditions;

	return value;
}
Json::Value ConstructorJSon::cj( InventoryItem* item )
{
	Json::Value itemValue;

	itemValue["max quantity"] = item->m_maxQuantity;
	itemValue["available"] = item->m_available;
	itemValue["file"] = item->m_fileName;

	// the other properties are already defined in the inventory item file
	// it must be edited externally...
	/*
	itemValue["type"] = item->getObjectType();
	itemValue["name"] = item->getName();

	itemValue["isStatic"] = item->isStatic;
	itemValue["isMovable"] = item->isMovable;
	itemValue["isRotatable"] = item->isRotatable;
	itemValue["isDeletable"] = item->isDeletable;
	
	itemValue["item sprite path"] = item->m_itemSpritePath;
	itemValue["object sprite path"] = item->m_objectSpritePath;

	itemValue["prototype"] = item->m_prototype;
	*/

	return itemValue;
}
Json::Value ConstructorJSon::cj( GameObject* gameObject )
{
	Json::Value objectValue;

	objectValue["body"] = lookupBodyIndex( gameObject->getObjectBody() );
	objectValue["inventory item"] = lookupInventoryItemIndex( gameObject->getInventoryItem() );

	objectValue["isStatic"] = gameObject->isStatic;
	objectValue["isMovable"] = gameObject->isMovable;
	objectValue["isRotatable"] = gameObject->isRotatable;
	objectValue["isDeletable"] = gameObject->isDeletable;

	return objectValue;
}



LevelDef* ConstructorJSon::j2cLevelDef( Json::Value value )
{
	LevelDef *l = new LevelDef;
	
	// level description
	l->name = value["name"].asString();
	l->difficulty = value["difficulty"].asFloat();

	// level style
	l->theme = value["theme"].asString();

	// level physical world
	{
		Json::Value worldJson;
		worldJson = value["game world"];

		b2World *physicsWorld = j2b2World( worldJson["physics world"] );
		b2Body *nullBody = getBodyByName( "null body" );
		l->gameWorld = GameWorld::node( physicsWorld, nullBody );
	}

	// inventory items
	{
		int items = value["inventory items"].size();
		for( int i = 0; i < items; ++i ) {
			InventoryItem *item = j2cInventoryItem( value["inventory items"][i] );
			l->inventoryItems.push_back( item );
			m_inventoryItems.push_back( item );
		}
	}

	// game objects
	{
		int items = value["game objects"].size();
		for( int i = 0; i < items; ++i ) {
			GameObject *object = j2cGameObject( value["game objects"][i] );
			l->gameObjects.addObject( object );
			m_gameObjects.push_back( object );
		}
	}
	{
		int targetIndex = value["target"].asInt();
		CCAssert( targetIndex < (int) m_gameObjects.size(), "Wrong \"target\"" );
		l->target = m_gameObjects[targetIndex];
		
		targetIndex = value["win area"].asInt();
		CCAssert( targetIndex < (int) m_gameObjects.size(), "Wrong \"target\"" );
		if(targetIndex == 0){
			l->winArea = NULL;
		}else {
			l->winArea = m_gameObjects[targetIndex];
		}
		
		targetIndex = value["loose area"].asInt();
		CCAssert( targetIndex < (int) m_gameObjects.size(), "Wrong \"target\"" );
		if(targetIndex == 0){
			l->looseArea = NULL;
		}else {
			l->looseArea = m_gameObjects[targetIndex];
		}


	}

	// win and lose conditions
	l->winConditions = static_cast<LevelDef::WinConditions>( value["win conditions"].asInt() );
	l->loseConditions = static_cast<LevelDef::LoseConditions>( value["lose conditions"].asInt() );

	return l;
}
InventoryItem* ConstructorJSon::j2cInventoryItem( Json::Value itemValue )
{
	InventoryItem *item;
	std::string fileName = itemValue["file"].asString();

	// reading the inventory item file
	{
		std::string filePath = inventoryItemPath( fileName );
		std::ifstream ifs;
		ifs.open( filePath.c_str(), std::ios::in );
		if (!ifs) {
			std::cout << "Could not open inventory item file " << filePath << " for reading" << std::endl;
			return NULL;
		}

		Json::Reader reader;
		ConstructorJSon cjs;
		Json::Value json;

		if( ! reader.parse(ifs, json) )
		{
			std::cout  << "Failed to parse inventory item file " << filePath << std::endl << reader.getFormattedErrorMessages();
			ifs.close();
			return NULL;
		}

		ObjectType type = static_cast<ObjectType>( json["type"].asInt() );
		std::string name = json["name"].asString();
		std::string itemSprite = json["item sprite path"].asString();
		std::string objectSprite = json["object sprite path"].asString();
		
		Json::Value prototype = json["prototype"];

		switch( type ) {
			case SimpleBox:
				item = SimpleBoxInventoryItem::node( itemSprite, objectSprite, prototype, fileName, name );
				break;
			case Area:
				item = AreaInventoryItem::node( itemSprite, objectSprite, prototype, fileName, name );
				break;
			case Spring:
				item = SpringInventoryItem::node( itemSprite, objectSprite, prototype, fileName, name );
				break;
			case Pin:
				item = PinInventoryItem::node( itemSprite, objectSprite, prototype, fileName, name );
				break;
			case Glue:
				item = GlueInventoryItem::node( itemSprite, objectSprite, prototype, fileName, name );
				break;
			default:
				CCAssert( false, "Invalid inventory item" );
		}

		item->isStatic = json["isStatic"].asBool();
		item->isMovable = json["isMovable"].asBool();
		item->isRotatable = json["isRotatable"].asBool();
		item->isDeletable = json["isDeletable"].asBool();

		ifs.close();
	}

	item->m_maxQuantity = itemValue["max quantity"].asInt();
	if( itemValue["available"].isIntegral() ) {
		item->m_available = itemValue["available"].asBool();
	} else {
		item->m_available = true;
	}

	return item;
}
GameObject* ConstructorJSon::j2cGameObject( Json::Value objectValue )
{
	int itemIndex = objectValue["inventory item"].asInt();
	int bodyIndex = objectValue["body"].asInt();

	if( bodyIndex >= (int) m_bodies.size() ) {
		return NULL;
	}
	if( itemIndex >= (int) m_inventoryItems.size() ) {
		return NULL;
	}

	InventoryItem *item = m_inventoryItems[itemIndex];
	b2Body *body = m_bodies[bodyIndex];
	
	GameObject *object = item->gameObjectNode( body );

	object->isStatic = objectValue["isStatic"].asBool();
	object->isMovable = objectValue["isMovable"].asBool();
	object->isRotatable = objectValue["isRotatable"].asBool();
	object->isDeletable = objectValue["isDeletable"].asBool();
	
	return object;
}


void ConstructorJSon::setInventoryItemName( InventoryItem* item, const char* name )
{
    m_inventoryItemToNameMap[item] = name;
}
void ConstructorJSon::setGameObjectName( GameObject* object, const char* name )
{
    m_gameObjectToNameMap[object] = name;
}


InventoryItem* ConstructorJSon::getInventoryItemByName( std::string name )
{
	std::map<InventoryItem*,std::string>::iterator it = m_inventoryItemToNameMap.begin();
	while( it != m_inventoryItemToNameMap.end() ) {
		if( it->second == name )
			return it->first;
		++it;
	}
	return 0;
}
GameObject* ConstructorJSon::getGameObjectByName( std::string name )
{
	std::map<GameObject*,std::string>::iterator it = m_gameObjectToNameMap.begin();
	while( it != m_gameObjectToNameMap.end() ) {
		if( it->second == name )
			return it->first;
		++it;
	}
	return 0;
}


int ConstructorJSon::lookupInventoryItemIndex( InventoryItem* item )
{
	std::map<InventoryItem*,int>::iterator it = m_inventoryItemIndexMap.find( item );
	if( it != m_inventoryItemIndexMap.end() ) {
		return it->second;
	}
	return 0;
}
int ConstructorJSon::lookupGameObjectIndex( GameObject* object )
{
	std::map<GameObject*,int>::iterator it = m_gameObjectIndexMap.find( object );
	if( it != m_gameObjectIndexMap.end() ) {
		return it->second;
	}
	return 0;
}


std::string ConstructorJSon::getInventoryItemName( InventoryItem* item )
{
	std::map<InventoryItem*,std::string>::iterator it = m_inventoryItemToNameMap.find( item );
	if ( it == m_inventoryItemToNameMap.end() ) {
		return "";
	}
	return it->second;
}
std::string ConstructorJSon::getGameObjectName( GameObject* object )
{
	std::map<GameObject*,std::string>::iterator it = m_gameObjectToNameMap.find( object );
	if ( it == m_gameObjectToNameMap.end() ) {
		return "";
	}
	return it->second;
}
