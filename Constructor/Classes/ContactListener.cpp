//
//  ContactListener.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 7/17/12.
//  Copyright (c) 2012 The Epic Bean. All rights reserved.
//

#include "ContactListener.h"
#include "GameObject.h"
#include "GameLevelScene.h"

//////////////////////////////////////////////////// 
// Default contructor
//////////////////////////////////////////////////// 
CContactListener::CContactListener(){
}

//////////////////////////////////////////////////// 
// Default destructor
//////////////////////////////////////////////////// 
CContactListener::~CContactListener() {
}


//////////////////////////////////////////////////// 
// On contact started handler
//////////////////////////////////////////////////// 
void CContactListener::BeginContact(b2Contact* contact) {
	if (!GameLevelScene::sharedGameScene()->isSimulating()) {
		return;
	}
	
	GameObject * gameObjectA = (GameObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	GameObject * gameObjectB = (GameObject*)contact->GetFixtureB()->GetBody()->GetUserData();
	
    if (gameObjectA && gameObjectB) {
		gameObjectA->objectCollided(gameObjectB);
		gameObjectB->objectCollided(gameObjectA);
    }
}


//////////////////////////////////////////////////// 
// On contact ended handler
//////////////////////////////////////////////////// 
void CContactListener::EndContact(b2Contact* contact){
	if (!GameLevelScene::sharedGameScene()->isSimulating()) {
		return;
	}
	
	GameObject * gameObjectA = (GameObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	GameObject * gameObjectB = (GameObject*)contact->GetFixtureB()->GetBody()->GetUserData();
	
    if (gameObjectA && gameObjectB) {
		gameObjectA->objectCollisionEnded(gameObjectB);
		gameObjectB->objectCollisionEnded(gameObjectA);
    }

}

void CContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) 
{
}

void CContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) 
{
}