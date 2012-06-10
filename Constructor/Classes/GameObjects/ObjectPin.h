//
//  ObjectPin.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/19/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectPin_H__
#define __ObjectPin_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;


////////////////////////////////////////////////////
// ObjectPin
///////////////////////////////////////////////////
class ObjectPin : public GameObject
{
private:
	CCSprite *m_sprite;
	b2Body *m_body;

	bool m_isPinned;
	b2RevoluteJoint * m_pinJoint;

	ObjectPin( const std::string &fileName, const Json::Value &prototype )
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
	//////////////////////////////////////////////////// 
	// Unpins this pin from body
	//////////////////////////////////////////////////// 
	void unPin(bool destroyJoint);
	void rePin();
	
	//////////////////////////////////////////////////// 
	// Begins object preparation for unstuck routine
	//////////////////////////////////////////////////// 
	void startUnstuckPhase();
	
	//////////////////////////////////////////////////// 
	// Function to be called after unstuck routine is finished
	//////////////////////////////////////////////////// 
	void unstuckPhaseFinished();
	
	GAMEOBJECT_NODE_DEF( ObjectPin )
};

#endif
