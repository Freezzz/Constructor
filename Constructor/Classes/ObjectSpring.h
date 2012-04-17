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


////////////////////////////////////////////////////
// ObjectSpring
///////////////////////////////////////////////////
class ObjectSpring : public GameObject {
private:
    b2Vec2 _secondBodyOriginalLocation;
	float _secondBodyOriginalRotation;
	
	vector<b2DistanceJoint*> * joints;
	vector<b2Body*> *ribs;
protected:
	virtual bool init();
    virtual void destroy();
    virtual void update(ccTime dt);    
    b2Body * secondBody;
	

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

	LAYER_NODE_FUNC(ObjectSpring);
};

#endif
