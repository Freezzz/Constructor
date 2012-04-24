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

enum ObjectState{
    Idile       = 0,
    Moving      = 1,
    Rotating    = 2,
    Simulating  = 3
};


class GameObject;

#define INVENTORYITEM_CLASS_DEF(INVENTORYITEM,GAMEOBJECT,TYPE) \
	class GAMEOBJECT; \
	class INVENTORYITEM : public InventoryItem \
	{ \
	public: \
		INVENTORYITEM_NODE_DEF(INVENTORYITEM,GAMEOBJECT) \
		INVENTORYITEM() : InventoryItem(TYPE) {} \
		bool init( std::string itemSpritePath, std::string objectSpritePath ) { \
			m_itemSpritePath = itemSpritePath; \
			m_objectSpritePath = objectSpritePath; \
			m_objectSprite = CCSprite::spriteWithFile( itemSpritePath.c_str() ); \
			addChild( m_objectSprite ); \
			CCLOG("INIT"); \
			return true;\
		} \
	};

#define INVENTORYITEM_NODE_DEF(INVENTORYITEM,GAMEOBJECT) \
	static INVENTORYITEM* node( std::string itemPath, std::string spritePath ); \
	GameObject* gameObjectNode( b2Body *b ); \
	GameObject* gameObjectNode( CCPoint p );

#define GAMEOBJECT_NODE_DEF(INVENTORYITEM,GAMEOBJECT) \
	static GAMEOBJECT* node( InventoryItem *item, CCPoint p, std::string spritePath ) \
	{ \
		GAMEOBJECT *r = new GAMEOBJECT(); \
		if( r && r->init(spritePath) ) { \
			r->m_inventoryItem = item; \
			r->createBodyAtPosition( p ); \
			r->autorelease(); \
			return r; \
		} \
		\
		delete r; \
		return NULL; \
	}; \
	static GAMEOBJECT* node( InventoryItem *item, b2Body *b, std::string spritePath ) \
	{ \
		GAMEOBJECT *r = new GAMEOBJECT(); \
		if( r && r->init(spritePath) ) { \
			r->m_inventoryItem = item; \
			r->m_objectBody = b; \
			r->m_objectBody->SetUserData( r ); \
			r->autorelease(); \
			return r; \
		} \
		\
		delete r; \
		return NULL; \
	};

#define INVENTORYITEM_NODE_DECL(layer) \
	layer* layer::node( std::string itemSpritePath, std::string objectSpritePath ) \
	{ \
		layer *r = new layer(); \
		if( r && r->init( itemSpritePath, objectSpritePath ) ) { \
			r->autorelease(); \
			return r; \
		} \
		\
		delete r; \
		return NULL; \
	};

#define INVENTORYITEM_GAMEOBJECT_NODE_DECL(INVENTORYITEM,GAMEOBJECT) \
	INVENTORYITEM_NODE_DECL(INVENTORYITEM) \
	GameObject* INVENTORYITEM::gameObjectNode( b2Body *b ) { return GAMEOBJECT::node(this, b, m_objectSpritePath); } \
	GameObject* INVENTORYITEM::gameObjectNode( CCPoint p ) { return GAMEOBJECT::node(this, p, m_objectSpritePath); } \
	
	

// the inventory item
class InventoryItem : public CCNode
{
public:
	std::string m_itemSpritePath;
	std::string m_objectSpritePath;
	CCSprite * m_objectSprite;
	ObjectType m_type;

	// Is a static object in simulation
	bool isStatic;
	bool isMovable;
	bool isRotatable;
	bool isDeletable;

	InventoryItem( ObjectType type ) : m_type(type), isStatic(0), isMovable(1), isRotatable(1), isDeletable(1) { }
    
public:
	virtual GameObject* gameObjectNode( b2Body *b ) = 0;
	virtual GameObject* gameObjectNode( CCPoint p ) = 0;
	ObjectType getObjectType() { return m_type; }
	
};

////////////////////////////////////////////////////
// GameObject
///////////////////////////////////////////////////
class GameObject : public CCNode {
protected:
	b2MouseJoint * m_moveJoint;

	b2RevoluteJoint * m_objectBodyPin;
	b2MouseJoint * m_rotationJoin;
	
	// Original pre-simulation settings of object
	CCPoint m_originalPosition;
    float m_originalRotation;

	int m_id;
	bool m_mutable;

	
	//////////////////////////////////////////////////// 
	// GameObject init
	////////////////////////////////////////////////////
	GameObject( );
	
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
	int id( ) const { return m_id; }
	void setId( int id ) { m_id = id; }
	bool isMutable( ) const { return m_mutable; }
	void setMutable( bool mut ) { m_mutable = mut; }

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
	CCPoint moveButtonOffset;
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
	ObjectType m_type;
	ObjectType getObjectType(){return m_type;}

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
};

#endif
