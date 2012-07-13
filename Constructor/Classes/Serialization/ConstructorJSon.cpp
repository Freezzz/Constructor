
#include "ConstructorJSon.h"

#include "../GameObjects/GameObject.h"
#include "../GameObjects/ObjectSimpleBox.h"
#include "../GameObjects/ObjectArea.h"
#include "../GameObjects/ObjectGlue.h"
#include "../GameObjects/ObjectPin.h"
#include "../GameObjects/ObjectSpring.h"
#include "../GameObjects/ObjectFatty.h"
#include "../Controls/InventoryLayer.h"
#include "../Serialization/LevelManager.h"

#include "LevelDef.h"
#include "GameWorld.h"
#include "Common.h"

#include <fstream>

namespace Json
{
	// TODO: find a better way to return errors...
	Json::Value jsonFromFile( const char *filename )
	{
		if( ! filename ) {
			return Json::Value();
		}

		std::ifstream ifs;
		ifs.open( filename, std::ios::in );
		if( ! ifs ) {
			std::cout << "Could not open file " << filename << " for reading" << std::endl;
			return Json::Value();
		}

		Json::Value value;
		Json::Reader reader;
		if( ! reader.parse(ifs, value) )
		{
			std::cout  << "Failed to parse " << filename << std::endl << reader.getFormattedErrorMessages();
			ifs.close();
			return Json::Value();
		}
		ifs.close();

		return value;
	}
}

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

    // level game mode configuration
    value["controlled"] = levelDef->controlled;
    
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

	objectValue["object file"] = gameObject->m_fileName;

    vecToJson( "position", 	b2Vec2(gameObject->m_originalPosition.x / PTM_RATIO, gameObject->m_originalPosition.y / PTM_RATIO ), objectValue );
	objectValue["rotation"] = gameObject->m_originalRotation;

	objectValue["isStatic"] = gameObject->isStatic;
	objectValue["isMovable"] = gameObject->isMovable;
	objectValue["isRotatable"] = gameObject->isRotatable;
	objectValue["isDeletable"] = gameObject->isDeletable;
	objectValue["defaultZOrder"] = gameObject->defaultZOrder;
	
	return objectValue;
}



LevelDef* ConstructorJSon::j2cLevelDef( Json::Value value )
{
	LevelDef *l = new LevelDef;

	// level properties
	{
		// description
		l->name = value["name"].asString();
		l->difficulty = value["difficulty"].asFloat();

		// win and lose conditions
		l->winConditions = static_cast<LevelDef::WinConditions>( value["win conditions"].asInt() );
		l->loseConditions = static_cast<LevelDef::LoseConditions>( value["lose conditions"].asInt() );
	}

	// level game mode configurations
	{
		l->controlled = value["controlled"].asBool();
	}
	
	// theme
	{
		std::string filePath = themePath( value["theme"].asString() );
		l->theme = Json::jsonFromFile( filePath.c_str() );
	}

	// world
	{
		l->gameWorld = GameWorld::node( b2Vec2(0,-9.8) );
		
		// TODO: world properties (gravity etc) from a world file...
	}

	// inventory buttons
	{
		int items = value["inventory buttons"].size();
		for( int i = 0; i < items; ++i ) {
			InventoryItem *item = j2cInventoryItem( value["inventory buttons"][i] );
			if( ! item ) {
				return 0;
			}
			l->inventoryItems.push_back( item );
			m_inventoryItems.push_back( item );
		}
	}

	// game objects
	{
		int objects = value["game objects"].size();
		for( int i = 0; i < objects; ++i ) {
			GameObject *object = j2cGameObject( value["game objects"][i] );
			if( ! object ) {
				return 0;
			}
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

	return l;
}
InventoryItem* ConstructorJSon::j2cInventoryItem( Json::Value itemValue )
{
	std::string fileName = itemValue["file"].asString();
	InventoryItem *item = j2cInventoryItem( fileName );
	if( ! item ) {
		return 0;
	}
	
	item->m_maxQuantity = itemValue["max quantity"].asInt();

	return item;
}
InventoryItem* ConstructorJSon::j2cInventoryItem( std::string &fileName )
{
	std::string filePath = inventoryItemPath( fileName );
	Json::Value json = Json::jsonFromFile( filePath.c_str() );
	if( json.empty() ) {
		std::cout << "Failed reading inventory item file: " << filePath << std::endl;
		return 0;
	}

	std::string name = json["name"].asString();
	std::string spritePath = json["sprite path"].asString();
	std::string prototypeName = json["game object file"].asString();

	return InventoryItem::node( fileName, name, prototypeName, spritePath );
}
GameObject* ConstructorJSon::j2cGameObject( Json::Value value )
{
	std::string fileName = value["file"].asString();
	
	CCPoint p = v( jsonToVec("position",value) );
	p.x *= PTM_RATIO;
	p.y *= PTM_RATIO;
	GameObject *object = j2cGameObject( fileName, p );
	if( ! object ) {
		return 0;
	}

	if( value.isMember("movable") ) {
		object->isMovable = value["movable"].asBool();
	}
	if( value.isMember("rotatable") ) {
		object->isRotatable = value["rotatable"].asBool();
	}
	if( value.isMember("deletable") ) {
		object->isDeletable = value["deletable"].asBool();
	}
	if( value.isMember("defaultZOrder") ) {
		object->defaultZOrder = value["defaultZOrder"].asInt();
	}
	
	return object;
}
GameObject* ConstructorJSon::j2cGameObject( std::string &fileName, const cocos2d::CCPoint &p )
{
	std::string filePath = gameObjectPath( fileName );
	Json::Value json = Json::jsonFromFile( filePath.c_str() );
	if( json.empty() ) {
		std::cout << "Failed reading game object file: " << filePath << std::endl;
		return 0;
	}
	
	GameObject *object = 0;

	ObjectType type = static_cast<ObjectType>( json["type"].asInt() );
	Json::Value prototype = json["prototype"];
	switch( type ) {
		case SimpleBox:
			object = ObjectSimpleBox::node( fileName, prototype, p );
			break;
		case Area:
			object = ObjectArea::node( fileName, prototype, p );
			break;
		case Spring:
			object = ObjectSpring::node( fileName, prototype, p );
			break;
		case Pin:
			object = ObjectPin::node( fileName, prototype, p );
			break;
		case Glue:
			object = ObjectGlue::node( fileName, prototype, p );
			break;
		case Fatty:
			object = ObjectFatty::node( fileName, prototype, p );
			break;			
		default:
			CCAssert( false, "Invalid inventory item" );
	}

	object->isStatic = json["static"].asBool();
	object->isMovable = json["movable"].asBool();
	object->isRotatable = json["rotatable"].asBool();
	object->isDeletable = json["deletable"].asBool();

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
