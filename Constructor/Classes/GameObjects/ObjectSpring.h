//
//  ObjectSpring.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectSpring_H__
#define __ObjectSpring_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;

INVENTORYITEM_CLASS_DEF( SpringInventoryItem, ObjectSpring, "spring_btn.png" )


////////////////////////////////////////////////////
// ObjectSpring
///////////////////////////////////////////////////
class ObjectSpring : public GameObject {
private:
    b2Body * m_secondBody;
	
    b2Vec2 m_secondBodyOriginalLocation;
	float m_secondBodyOriginalRotation;
	
	vector<b2DistanceJoint*> m_joints;
protected:
	virtual bool init();
    virtual void destroy();
    virtual void update(ccTime dt);    

	

	// State change handlers
	virtual void onSimulationStarted();
	virtual void onSimulationEnded();	
	virtual void onMovementStarted();
	virtual void onMovementEnded();	
public:
	//////////////////////////////////////////////////// 
	// Creates object at location
	//////////////////////////////////////////////////// 
    void createBodyAtPosition(CCPoint position);

	//////////////////////////////////////////////////// 
	// Save object's properties pre-simulation
	//////////////////////////////////////////////////// 
	virtual void saveOriginalProperties(); 
	
	//////////////////////////////////////////////////// 
	// Restore object's properties that where set
	// before simulation
	//////////////////////////////////////////////////// 
    virtual void restoreToOriginalProperties();
    
	GAMEOBJECT_NODE_DEF( SpringInventoryItem, ObjectSpring )
};

#endif
