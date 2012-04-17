//
//  GameWorld.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __GameWorld_H__
#define __GameWorld_H__

#include "cocos2d.h"
#include <Box2d/Box2d.h>
#include "GLES-Render.h"

using namespace cocos2d;
class GameObject;

////////////////////////////////////////////////////
// GameWorld wrapper to phyisycal world 
// interactions
///////////////////////////////////////////////////
class GameWorld : public CCNode {
private:
    GLESDebugDraw * m_debugDraw;
    
    
    //////////////////////////////////////////////////// 
    // Create world internal bounds
    //////////////////////////////////////////////////// 
	void createWorldBox(CCSize screenSize);
    
	virtual void update(ccTime dt);
	virtual bool init();

	void draw();
public:
	b2World * physicsWorld;
	b2Body* umbelicoDelMondo;    
    
    //////////////////////////////////////////////////// 
    // Static factory constructor
    //////////////////////////////////////////////////// 
	static GameWorld* node();
    
    //////////////////////////////////////////////////// 
    // Singleton pattern
    //////////////////////////////////////////////////// 
	static GameWorld* gameWorldInstance;
	static GameWorld* sharedGameWorld();    
};

#endif
