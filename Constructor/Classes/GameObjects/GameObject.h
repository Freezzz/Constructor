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
    idile       = 0,
    moving      = 1,
    rotating    = 2,
    simulating  = 3
}ObjectState;

////////////////////////////////////////////////////
// GameObject
///////////////////////////////////////////////////
class GameObject : public CCNode {
protected:
	b2MouseJoint * moveJoint;
	
	// Original pre-simulation settings of object
	CCPoint _originalPosition;
    float _originalRotation;

	
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
    b2Body * objectBody;
    CCSprite * objectSprite;
	
	// Is a static object in simulation
    bool isStatic;
	// Current object state
    ObjectState state;
        
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
	// Sets object state, and calls coresponding handlers
	//////////////////////////////////////////////////// 
    virtual void setObjectState(ObjectState newState);
    
	//////////////////////////////////////////////////// 
	// Creates object at location
	//////////////////////////////////////////////////// 
	virtual void createBodyAtPosition(CCPoint position)=0;
    static GameObject* node();
};

#endif
