//
//  GameObject.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __GameObject_H__
#define __GameObject_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "Constants.h"
using namespace cocos2d;

typedef enum ObjectState{
    Idile       = 0,
    Moving      = 1,
    Rotating    = 2,
    Simulating  = 3
}ObjectState;


class GameObject;

#define INVENTORYITEM_CLASS_DEF(INVENTORYITEM,GAMEOBJECT,SPRITE) \
	class GAMEOBJECT; \
	class INVENTORYITEM : public InventoryItem \
	{ \
	public: \
		INVENTORYITEM_NODE_DEF(INVENTORYITEM,GAMEOBJECT) \
		bool init( ) { \
			m_objectSprite = CCSprite::spriteWithFile(SPRITE); \
			addChild( m_objectSprite ); \
			CCLOG("INIT"); \
			return true;\
		} \
	};

#define INVENTORYITEM_NODE_DEF(INVENTORYITEM,GAMEOBJECT) \
	static INVENTORYITEM* node(); \
	INVENTORYITEM* nodeV(); \
	GameObject* gameObjectNodeV();

#define GAMEOBJECT_NODE_DEF(INVENTORYITEM,GAMEOBJECT) \
	static GAMEOBJECT* node(); \
	GAMEOBJECT* nodeV(); \
	INVENTORYITEM* inventoryItemNodeV();

#define GENERIC_NODE_DECL(layer) \
	layer* layer::node() \
	{ \
		layer *r = new layer(); \
		if( r && r->init() ) { \
			r->autorelease(); \
			return r; \
		} \
		\
		delete r; \
		return NULL; \
	};

#define INVENTORYITEM_GAMEOBJECT_NODE_DECL(INVENTORYITEM,GAMEOBJECT) \
	GENERIC_NODE_DECL(INVENTORYITEM) \
	GENERIC_NODE_DECL(GAMEOBJECT) \
	GameObject* INVENTORYITEM::gameObjectNodeV( ) { return GAMEOBJECT::node(); } \
	INVENTORYITEM* INVENTORYITEM::nodeV( ) { return INVENTORYITEM::node(); } \
	INVENTORYITEM* GAMEOBJECT::inventoryItemNodeV( ) { return INVENTORYITEM::node(); } \
	GAMEOBJECT* GAMEOBJECT::nodeV( ) { return GAMEOBJECT::node(); }
	
	

// the inventory item
class InventoryItem : public CCNode
{
public:
    CCSprite * m_objectSprite;
    
public:
	virtual bool init( ) = 0;
	
	virtual GameObject* gameObjectNodeV( ) = 0;
	virtual InventoryItem* nodeV( ) = 0;
	
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

	
	//////////////////////////////////////////////////// 
	// GameObject init
	//////////////////////////////////////////////////// 
	virtual bool init();
	
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
	
	virtual GameObject* nodeV( ) = 0;
	virtual InventoryItem* inventoryItemNodeV( ) = 0;
};

#endif
