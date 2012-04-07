//
//  GameWorld.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __GameWorld_H__
#define __GameWorld_H__

#import "cocos2d.h"
#include "Box2D.h"
#include "GLES-Render.h"

using namespace cocos2d;
class GameObject;

////////////////////////////////////////////////////
// GameWorld
///////////////////////////////////////////////////
class GameWorld : public CCNode {
private:
    GLESDebugDraw * m_debugDraw;
    bool _isRunning;
    
	virtual void update(ccTime dt);
	virtual bool init();
	void createWorldBox(CCSize screenSize);
	void draw();
public:
	b2World * physicsWorld;
    
    void runWorld();
    void pauseWorld();
    bool isRunning(){return _isRunning;}
    
	static GameWorld* node();
	static GameWorld* gameWorldInstance;
	static GameWorld* sharedGameWorld();    
};

#endif
