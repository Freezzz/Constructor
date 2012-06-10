//
//  ObjectSimpleBox.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectSimpleBox_H__
#define __ObjectSimpleBox_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;


////////////////////////////////////////////////////
// ObjectSimpleBox
///////////////////////////////////////////////////
class ObjectSimpleBox : public GameObject {
protected:
	CCSprite *m_sprite;
	b2Body *m_body;

    ObjectSimpleBox(const string& fileName, const Json::Value& prototype)
	: GameObject(fileName, prototype) { }
	bool init( );
    bool createBodyAtPosition( CCPoint position );
	
public:
	GAMEOBJECT_NODE_DEF( ObjectSimpleBox )
};

#endif
