//
//  GameObject.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "Constants.h"
using namespace cocos2d;


class GameObject;

#define INVENTORYITEM_CLASS_DEF(INVENTORYITEM,GAMEOBJECT,TYPE) \
	class GAMEOBJECT; \
	class INVENTORYITEM : public InventoryItem \
	{ \
	public: \
		GameObject* gameObjectNode( b2Body *b ); \
		GameObject* gameObjectNode( CCPoint p ); \
		INVENTORYITEM() : InventoryItem(TYPE) {} \
		/* init */ \
		bool init( std::string itemSpritePath, std::string objectSpritePath, b2FixtureDef *fixtureDef, const std::string &name ) { \
			m_itemSpritePath = itemSpritePath; \
			m_objectSpritePath = objectSpritePath; \
			m_fixtureDef = fixtureDef; \
			m_name = name; \
			m_objectSprite = CCSprite::spriteWithFile( itemSpritePath.c_str() ); \
			addChild( m_objectSprite ); \
			CCLOG("INIT"); \
			return true;\
		} \
		/* node */ \
		static INVENTORYITEM* node( std::string itemSpritePath, std::string objectSpritePath, b2FixtureDef *fixtureDef, const std::string &name ) { \
			INVENTORYITEM *r = new INVENTORYITEM(); \
			if( r && r->init( itemSpritePath, objectSpritePath, fixtureDef, name ) ) { \
				r->autorelease(); \
				return r; \
			} \
			\
			delete r; \
			return NULL; \
		}; \
	};

#define GAMEOBJECT_NODE_DEF(INVENTORYITEM,GAMEOBJECT) \
	static GAMEOBJECT* node( InventoryItem *item, CCPoint p, std::string spritePath, b2FixtureDef *fixtureDef ) { \
		GAMEOBJECT *r = new GAMEOBJECT(); \
		if( r ) { \
			++ item->m_quantity; \
			r->m_inventoryItem = item; \
		} \
		if( r && r->init(spritePath, fixtureDef) ) { \
			r->m_inventoryItem = item; \
			r->createBodyAtPosition( p ); \
			r->autorelease(); \
			return r; \
		} \
		if( r ) { \
			delete r; \
		} \
		return NULL; \
	}; \
	static GAMEOBJECT* node( InventoryItem *item, b2Body *b, std::string spritePath, b2FixtureDef *fixtureDef ) { \
		GAMEOBJECT *r = new GAMEOBJECT(); \
		if( r ) { \
			++ item->m_quantity; \
			r->m_inventoryItem = item; \
		} \
		if( r && r->init(spritePath, fixtureDef) ) { \
			r->m_inventoryItem = item; \
			r->setBody( b ); \
			r->autorelease(); \
			return r; \
		} \
		if( r ) { \
			delete r; \
		} \
		return NULL; \
	};

#define INVENTORYITEM_GAMEOBJECT_NODE_DECL(INVENTORYITEM,GAMEOBJECT) \
	GameObject* INVENTORYITEM::gameObjectNode( b2Body *b ) { \
		if( m_quantity >= m_maxQuantity && m_maxQuantity > 0 ) { \
			return NULL; \
		} \
		GAMEOBJECT *go = GAMEOBJECT::node(this, b, m_objectSpritePath, m_fixtureDef); \
		go->isStatic = isStatic; \
		go->isMovable = isMovable; \
		go->isRotatable = isRotatable; \
		go->isDeletable = isDeletable; \
		return go; \
	} \
	GameObject* INVENTORYITEM::gameObjectNode( CCPoint p ) { \
		if( m_quantity >= m_maxQuantity && m_maxQuantity > 0 ) { \
			return NULL; \
		} \
		GAMEOBJECT *go = GAMEOBJECT::node(this, p, m_objectSpritePath, m_fixtureDef); \
		go->isStatic = isStatic; \
		go->isMovable = isMovable; \
		go->isRotatable = isRotatable; \
		go->isDeletable = isDeletable; \
		return go; \
	}



// the inventory item
class InventoryItem : public CCNode
{
public:
	std::string m_name;
	std::string m_itemSpritePath;
	std::string m_objectSpritePath;
	CCSprite * m_objectSprite;
	ObjectType m_type;
	int m_quantity; // how many objects of this type exist?
	int m_maxQuantity; // how many items of this class may be created. 0 means infinity
	bool m_available; // whether this inventory item is avaliable to the user
	b2FixtureDef *m_fixtureDef;

	// Is a static object in simulation
	bool isStatic;
	bool isMovable;
	bool isRotatable;
	bool isDeletable;

	InventoryItem( ObjectType type ) : m_type(type), m_quantity(0), m_maxQuantity(0), m_available(1), isStatic(0), isMovable(1), isRotatable(1), isDeletable(1) { }
	virtual ~InventoryItem( ) { }
    
public:
	virtual GameObject* gameObjectNode( b2Body *b ) = 0;
	virtual GameObject* gameObjectNode( CCPoint p ) = 0;
	ObjectType getObjectType() { return m_type; }
	std::string getName() const { return m_name; }
	
};

////////////////////////////////////////////////////
// GameObject
///////////////////////////////////////////////////
class GameObject : public CCNode
{
public:
	enum ObjectState
	{
		Idile       = 0,
		Moving      = 1,
		Rotating    = 2,
		Simulating  = 3
	};

protected:
	b2FixtureDef *m_fixtureDef;
	
	b2MouseJoint * m_moveJoint;

	b2RevoluteJoint * m_objectBodyPin;
	b2MouseJoint * m_rotationJoin;
	
	// Original pre-simulation settings of object
	CCPoint m_originalPosition;
    float m_originalRotation;

	
	//////////////////////////////////////////////////// 
	// GameObject init
	////////////////////////////////////////////////////
	GameObject( );
	~GameObject( );
	
	//////////////////////////////////////////////////// 
	// Basic object update loop, moves sprite to body 
	// location
	//////////////////////////////////////////////////// 
    virtual void update(ccTime dt);
	
	// State change handlers
	virtual void onSimulationStarted();
	virtual void onSimulationEnded();	
	virtual void onMovementStarted();
	virtual void onMovementEnded();	
	virtual void onRotationStarted();
	virtual void onRotationEnded();

public:
	// Properties
	b2Body * m_objectBody;
	b2Body* getObjectBody(){return m_objectBody;}

	CCSprite * m_objectSprite;
	CCSprite* getObjectSprite(){return m_objectSprite;}

	// Is a static object in simulation
	bool isStatic;
	bool isMovable;
	bool isRotatable;
	bool isDeletable;

	int defaultZOrder;

	// Utility buttons offsets
	CCPoint rotateButtonOffset;
	CCPoint deleteButtonOffset;

	// Current object state
	ObjectState m_state;
	ObjectState getObjectState(){return m_state;}
	////////////////////////////////////////////////////
	// Sets object state, and calls coresponding handlers
	////////////////////////////////////////////////////
	virtual void setObjectState(ObjectState newState);


	// Object type
	//ObjectType m_type;
	//ObjectType getObjectType(){return m_type;}
	ObjectType getObjectType(){return m_inventoryItem->getObjectType();}

	InventoryItem *m_inventoryItem;
	InventoryItem *getInventoryItem(){return m_inventoryItem;}

	////////////////////////////////////////////////////
	// Moves object to new location, if state is idile
	// than it is a simple translation, if moving than
	// creates a move joint to move object around
	////////////////////////////////////////////////////
	virtual void move(CCPoint newPostion);

	////////////////////////////////////////////////////
	// Rotates object to give angle with simple translation
	////////////////////////////////////////////////////
	virtual void rotate(float newRotation);

	////////////////////////////////////////////////////
	// Rotates object to give angle creates a rotate
	//	joint to rotate object along it's axis
	////////////////////////////////////////////////////
	virtual void rotate(CCPoint location);

	////////////////////////////////////////////////////
	// Destroy object and it's physical body
	////////////////////////////////////////////////////
	virtual void destroy();

	////////////////////////////////////////////////////
	// Save object's properties pre-simulation
	////////////////////////////////////////////////////
	virtual void saveOriginalProperties();

	////////////////////////////////////////////////////
	// Restore object's properties that where set
	// before simulation
	////////////////////////////////////////////////////
	virtual void restoreToOriginalProperties();

	////////////////////////////////////////////////////
	// Grapical rapresentation of selected state
	////////////////////////////////////////////////////
	void setSelected(bool selected);


	////////////////////////////////////////////////////
	// Creates object at location
	////////////////////////////////////////////////////
	virtual void createBodyAtPosition(CCPoint position)=0;
	virtual void setBody( b2Body *b ) {
		m_objectBody = b;
		m_objectBody->SetUserData( this );
	}
};

#endif
