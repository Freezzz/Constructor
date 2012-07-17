//
//  ContactListener.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 7/17/12.
//  Copyright (c) 2012 The Epic Bean. All rights reserved.
//

#ifndef Constructor_ContactListener_h
#define Constructor_ContactListener_h

#include "cocos2d.h"
#include <Box2D/Box2D.h>

using namespace cocos2d;

//////////////////////////////////////////////////// 
// CContactListener class to handle contact between 
// objects and their triggers
//////////////////////////////////////////////////// 
class CContactListener : public b2ContactListener {
	
public:
    CContactListener();
    ~CContactListener();
    
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);    
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif