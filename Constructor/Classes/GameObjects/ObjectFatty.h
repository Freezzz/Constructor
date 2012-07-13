//
//  ObjectFatty.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectFatty_H__
#define __ObjectFatty_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;


////////////////////////////////////////////////////
// ObjectFatty
///////////////////////////////////////////////////
class ObjectFatty : public GameObject {
protected:
	CCSprite *m_sprite;
	CCSprite *m_fattySprite;
	b2Body *m_body;
	b2Body *m_contactSensor;

	b2RevoluteJoint * m_motorJoint;

    ObjectFatty(const string& fileName, const Json::Value& prototype)
	: GameObject(fileName, prototype) { }
	bool init( );
    bool createBodyAtPosition( CCPoint position );
	
public:
	
	void update(ccTime dt);
	
	void applyTorque(float value);
	
	GAMEOBJECT_NODE_DEF( ObjectFatty )
};

#endif
