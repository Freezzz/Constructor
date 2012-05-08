//
//  GameWorld.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "GameWorld.h"
#include <iostream>

//////////////////////////////////////////////////// 
// GameWorld init
//////////////////////////////////////////////////// 
bool GameWorld::init( b2World *phyWorld, b2Body *nullBody )
{
	gameWorldInstance = this;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8);

	if( ! phyWorld || ! nullBody ) {
		physicsWorld = new b2World(gravity);
		physicsWorld->SetAllowSleeping(true);
		physicsWorld->SetContinuousPhysics(true);
		
		// Create world borders
		createWorldBox(winSize);
	}
	else {
		physicsWorld = phyWorld;
		umbelicoDelMondo = nullBody;
	}
    
	// Box2d debugDraw configuration
	m_debugDraw = new GLESDebugDraw( PTM_RATIO * CC_CONTENT_SCALE_FACTOR() );
	physicsWorld->SetDebugDraw( m_debugDraw );
    
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	m_debugDraw->SetFlags(flags);
    
	scheduleUpdate();
	return true;
}

////////////////////////////////////////////////////
// Main GameWorld event loop
///////////////////////////////////////////////////
void GameWorld::update(ccTime dt){
	int velocityIterations = 8;
	int positionIterations = 1;

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	// physicsWorld->Step(dt, velocityIterations, positionIterations);
	physicsWorld->Step(1.0/60, velocityIterations, positionIterations);
	// TODO: add accumulator to step the world according to dt
}

////////////////////////////////////////////////////
// Generates physical border aroud the world
///////////////////////////////////////////////////
void GameWorld::createWorldBox(CCSize screenSize) {
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(screenSize.width/2/PTM_RATIO,
	                           screenSize.height/2/PTM_RATIO); // bottom-left corner

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
    b2Body* groundBody = physicsWorld->CreateBody(&groundBodyDef);
    
	// Define the ground box shape.
	b2PolygonShape groundBox;
    
	int halfGameZoneHeight = screenSize.height - 60;
	
	// bottom
	groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, .001, b2Vec2(0, -halfGameZoneHeight/2/PTM_RATIO), 0);
 	groundBody->CreateFixture(&groundBox, 0);
    
	// top
	groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, .001, b2Vec2(0, halfGameZoneHeight/2/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);
    
	// left
	groundBox.SetAsBox(.001, halfGameZoneHeight/2/PTM_RATIO, b2Vec2(-screenSize.width/2/PTM_RATIO, 0), 0);
	groundBody->CreateFixture(&groundBox, 0);
    
	// right
	groundBox.SetAsBox(.001, halfGameZoneHeight/2/PTM_RATIO, b2Vec2(screenSize.width/2/PTM_RATIO, 0), 0);
	groundBody->CreateFixture(&groundBox, 0);
    groundBody->SetType(b2_staticBody);
	
	
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(screenSize.width*0.5/PTM_RATIO, screenSize.height*0.5/PTM_RATIO);
	umbelicoDelMondo = physicsWorld->CreateBody(&bodyDef);
	
	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(5/2/PTM_RATIO, 5/2/PTM_RATIO);
	
	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.3f;
	fixtureDef.isSensor = true;
	umbelicoDelMondo->CreateFixture(&fixtureDef);
}

void GameWorld::draw()
{
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states:  GL_VERTEX_ARRAY,
	// Unneeded states: GL_TEXTURE_2D, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    
    // Debug info, comment out this line
//    physicsWorld->DrawDebugData();

    
	// restore default GL states
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

//////////////////////////////////////////////////// 
// Singleton pattern
//////////////////////////////////////////////////// 
GameWorld* GameWorld::gameWorldInstance = NULL;
GameWorld* GameWorld::sharedGameWorld(){
	CCAssert(gameWorldInstance!=NULL, "Game world not yet initialized");
	return gameWorldInstance;
}

//////////////////////////////////////////////////// 
// Static constructor returns autorelesed object
//////////////////////////////////////////////////// 
GameWorld* GameWorld::node( b2World *phyWorld, b2Body *nullBody )
{
	GameWorld * pRet = new GameWorld();
	if ( pRet && pRet->init(phyWorld, nullBody) ) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}