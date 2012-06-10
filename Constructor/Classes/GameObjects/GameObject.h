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
#include "Serialization/ConstructorJSon.h" // for the prototype
#include "Constants.h"
#include "GameWorld.h"
using namespace cocos2d;


class GameObject;

#define GAMEOBJECT_NODE_DEF(GAMEOBJECT) \
	static GAMEOBJECT* node( const std::string &fileName, const Json::Value &prototype, CCPoint p ) { \
		GAMEOBJECT *r = new GAMEOBJECT(fileName, prototype); \
		if( ! r ) { \
			return NULL; \
		} \
		if( r->init() && r->createBodyAtPosition(p) ) { \
			r->autorelease(); \
			return r; \
		} \
		delete r; \
		return NULL; \
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

public:
	std::string m_fileName; // the file where this object is defined
	// when saving a level, will not create new files for each game
	// object: will use existing file instead

	ObjectType m_type; // TODO: why? isn't polymorphism + RTTI enough?
	Json::Value m_prototype; // TODO: no need to duplicate it for every instance

	// Original pre-simulation settings of object
	std::vector< b2Body* > m_bodies;

	CCPoint m_originalPosition;
	float m_originalRotation;

	// Current object state
	ObjectState m_state;

	bool isStatic; // is a static object in simulation
	bool isMovable, isRotatable, isDeletable; // what user can do

	int defaultZOrder;

	// Utility buttons offsets
	CCPoint rotateButtonOffset;
	CCPoint deleteButtonOffset;

protected:
	////////////////////////////////////////////////////
	// GameObject init
	////////////////////////////////////////////////////
	GameObject( const std::string &fileName, const Json::Value &prototype );
	virtual ~GameObject( );

public:
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
	b2World * physicsWorld() { return GameWorld::sharedGameWorld()->physicsWorld; }
	Json::Value prototype() { return m_prototype; }

	// Is a static object in simulation

	ObjectState getObjectState(){return m_state;}
	////////////////////////////////////////////////////
	// Sets object state, and calls coresponding handlers
	////////////////////////////////////////////////////
	virtual void setObjectState(ObjectState newState);


	// Object type
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
	virtual bool createBodyAtPosition( CCPoint position ) = 0;
	
	//////////////////////////////////////////////////// 
	// Begins object preparation for unstuck routine
	//////////////////////////////////////////////////// 
	virtual void startUnstuckPhase();
	
	//////////////////////////////////////////////////// 
	// Function to be called after unstuck routine is finished
	//////////////////////////////////////////////////// 
	virtual void unstuckPhaseFinished();
	
	//////////////////////////////////////////////////// 
	// Checks if the point is contained inside this node
	//////////////////////////////////////////////////// 
	bool containsPoint(CCPoint location);
};

#endif
