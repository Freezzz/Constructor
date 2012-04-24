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

INVENTORYITEM_CLASS_DEF( PinInventoryItem , ObjectPin, Pin )


////////////////////////////////////////////////////
// ObjectPin
///////////////////////////////////////////////////
class ObjectPin : public GameObject
{
private:
	bool m_isPinned;
	b2RevoluteJoint * m_pinJoint;	
	
protected:
	bool init( std::string spritePath );
	
	// State change handlers
	virtual void onSimulationStarted();
	virtual void onSimulationEnded();	
	virtual void onMovementStarted();
	virtual void onMovementEnded();
	
	void createBodyAtPosition(CCPoint position);
	void setBody( b2Body *b );
	
public:
	//////////////////////////////////////////////////// 
	// Unpins this pin from body
	//////////////////////////////////////////////////// 
	void unPin(bool destroyJoint);
	void rePin();
	
	GAMEOBJECT_NODE_DEF( PinInventoryItem , ObjectPin )
};

#endif
