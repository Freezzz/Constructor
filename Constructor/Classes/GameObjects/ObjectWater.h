//
//  ObjectWater.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectWater_H__
#define __ObjectWater_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;


////////////////////////////////////////////////////
// ObjectWater
///////////////////////////////////////////////////
class ObjectWater : public GameObject {
protected:
	CCSprite *m_sprite;
	b2Body *m_body;

    ObjectWater(const string& fileName, const Json::Value& prototype)
	: GameObject(fileName, prototype) { }
	bool init( );
    bool createBodyAtPosition( CCPoint position );
	
public:
	GAMEOBJECT_NODE_DEF( ObjectWater )
	
	//////////////////////////////////////////////////// 
	// Object collided notification
	// @pararm: other Object of the collision
	//////////////////////////////////////////////////// 
	virtual void objectCollided(GameObject * otherObject);
	
	//////////////////////////////////////////////////// 
	// Object collision ended notification
	// @pararm: other Object of the collision
	//////////////////////////////////////////////////// 
	virtual void objectCollisionEnded(GameObject * otherObject);
};

#endif
