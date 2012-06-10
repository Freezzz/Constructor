//
//  ObjectGlue.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/19/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectGlue_H__
#define __ObjectGlue_H__

#include "cocos2d.h"
#include "GameObject.h"

using namespace cocos2d;



////////////////////////////////////////////////////
// ObjectGlue
///////////////////////////////////////////////////
class ObjectGlue : public GameObject
{
private:
	CCSprite *m_sprite;
	b2Body *m_body;

	bool m_isGlued;
	vector<b2WeldJoint*> m_joints;

	void weldObjectsAtContact(b2Contact *contact);

	ObjectGlue( const std::string &fileName, const Json::Value &prototype )
	: GameObject( fileName, prototype ) { }
protected:
	bool init( );
	// State change handlers
	virtual void onSimulationStarted();
	virtual void onSimulationEnded();	
	virtual void onMovementStarted();
	virtual void onMovementEnded();

	bool createBodyAtPosition(CCPoint position);

public:
	void unPin(bool destroyJoint);
	void unGlueJoint(b2WeldJoint * jnt);

	//////////////////////////////////////////////////// 
	// Begins object preparation for unstuck routine
	//////////////////////////////////////////////////// 
	void startUnstuckPhase();
	
	//////////////////////////////////////////////////// 
	// Function to be called after unstuck routine is finished
	//////////////////////////////////////////////////// 
	void unstuckPhaseFinished();
	
	GAMEOBJECT_NODE_DEF( ObjectGlue )
};


#endif
