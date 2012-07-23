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
class ObjectSpring : public GameObject
{
private:
	b2Body *m_firstBody, *m_secondBody;
	CCSprite *m_firstBodySprite, *m_secondBodySprite;
	CCSprite *m_springSprite;
	
	b2Vec2 m_firstBodyOriginalLocation;
	float m_firstBodyOriginalRotation;

	b2Vec2 m_secondBodyOriginalLocation;
	float m_secondBodyOriginalRotation;

	b2PrismaticJoint * m_prismaticJoint;
	vector<b2DistanceJoint*> m_joints;
	
	b2Vec2 m_offsetBetweenBodies;
protected:
	bool init( );
	virtual void destroy();
	virtual void update(ccTime dt);

	// State change handlers
	virtual void onSimulationStarted();
	virtual void onSimulationEnded();
	virtual void onMovementStarted();
	virtual void onMovementEnded();

	////////////////////////////////////////////////////
	// Creates object at location
	////////////////////////////////////////////////////
	bool createBodyAtPosition(CCPoint position);
public:
    ObjectSpring( const string& fileName, const Json::Value& prototype )
	: GameObject( fileName, prototype ) { }

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
	// Begins object preparation for unstuck routine
	//////////////////////////////////////////////////// 
	void startUnstuckPhase();
	
	//////////////////////////////////////////////////// 
	// Function to be called after unstuck routine is finished
	//////////////////////////////////////////////////// 
	void unstuckPhaseFinished();
	
	
	//////////////////////////////////////////////////// 
	// Object collided notification
	// @pararm: other Object of the collision
	//////////////////////////////////////////////////// 
	virtual void objectCollided(GameObject * otherObject, b2Contact * contact);

	GAMEOBJECT_NODE_DEF( ObjectSpring )
};

#endif
