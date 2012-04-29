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

INVENTORYITEM_CLASS_DEF( GlueInventoryItem , ObjectGlue, Glue )



////////////////////////////////////////////////////
// ObjectGlue
///////////////////////////////////////////////////
class ObjectGlue : public GameObject {
private:
	bool m_isGlued;
	vector<b2WeldJoint*> m_joints;
	
	void weldObjectsAtContact(b2Contact *contact);
protected:
	bool init( std::string spritePath, b2FixtureDef *fixtureDef );
	// State change handlers
	virtual void onSimulationStarted();
	virtual void onSimulationEnded();	
	virtual void onMovementStarted();
	virtual void onMovementEnded();
	
	void createBodyAtPosition(CCPoint position);
	void setBody( b2Body *b );
	
public:
	void unPin(bool destroyJoint);
	void unGlueJoint(b2WeldJoint * jnt);
	
	GAMEOBJECT_NODE_DEF( GlueInventoryItem , ObjectGlue )
};


#endif
