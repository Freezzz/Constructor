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
#include <Box2d/Box2d.h>

using namespace cocos2d;

typedef enum ObjectState{
    Idile       = 0,
    Moving      = 1,
    Rotating    = 2,
    Simulating  = 3
}ObjectState;

////////////////////////////////////////////////////
// GameObject
///////////////////////////////////////////////////
class GameObject : public CCNode {
protected:
	b2MouseJoint * m_moveJoint;
	
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
        
	//////////////////////////////////////////////////// 
	// Moves object to new location, if state is idile
	// than it is a simple translation, if moving than
	// creates a move joint to move object around
	//////////////////////////////////////////////////// 
    virtual void move(CCPoint newPostion);
	
	//////////////////////////////////////////////////// 
	// Rotates object to give angle, if state is idile
	// than it is a simple translation, if rotating than
	// creates a rotate joint to rotate object along 
	//	it's axis
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
	virtual void createBodyAtPosition(CCPoint position)=0;
    static GameObject* node();
};

#endif
