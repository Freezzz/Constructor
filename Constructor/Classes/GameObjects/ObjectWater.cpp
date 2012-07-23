//
//  ObjectWater.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectWater.h"
#include "../GameWorld.h"
#include <iostream>

//////////////////////////////////////////////////// 
// ObjectWater init
//////////////////////////////////////////////////// 
bool ObjectWater::init( )
{
	m_type = SimpleBox;
	
	m_sprite = CCSprite::spriteWithFile( m_prototype["sprite path"].asCString() );

    m_sprite->setScaleY(4);
    m_sprite->setOpacity(100);
    
	// Adapt container to the graphical rapresentation
	setContentSize(m_sprite->getContentSize());
	m_sprite->setAnchorPoint(CCPoint(0,0.5));
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0
	addChild(m_sprite);
	defaultZOrder = 1;

	rotateButtonOffset = CCPoint( getContentSize().width/2 + 10, 0 );
	deleteButtonOffset = CCPoint( -getContentSize().width/2 - 10, 0 );

	m_state = Idile;
	m_affectedObjects = std::vector<GameObject*>();
	scheduleUpdate();
	return true;
}


//////////////////////////////////////////////////// 
// Body creation from prototype
//////////////////////////////////////////////////// 
bool ObjectWater::createBodyAtPosition( cocos2d::CCPoint position )
{
	// Player physical body
	b2dJson json;

	m_body = json.j2b2Body( physicsWorld(), prototype() );
	if( ! m_body || ! m_body->GetFixtureList() ) {
		std::cout << "Simple inventory item prototype messed up" << std::endl;
		return false;
	}
	m_body->SetUserData( this );
	m_body->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_body->GetAngle() );
	setPosition(position);

	m_bodies.push_back( m_body );

	saveOriginalProperties();
	return true;
}


//////////////////////////////////////////////////// 
// Collision handler
//////////////////////////////////////////////////// 
void ObjectWater::objectCollided(GameObject *otherObject, b2Contact * contact){
	if(otherObject->m_type == Fatty){
		for (int i = 0; i < otherObject->m_bodies.size(); i++) {
			otherObject->m_bodies[i]->SetGravityScale(0.1);
		}
		m_affectedObjects.push_back(otherObject);
	}
}

//////////////////////////////////////////////////// 
// Collision ended handler
//////////////////////////////////////////////////// 
void ObjectWater::objectCollisionEnded(GameObject *otherObject, b2Contact * contact){
	for (int i = 0; i < otherObject->m_bodies.size(); i++) {
		otherObject->m_bodies[i]->SetGravityScale(1);
	}
	
	for (int i = 0; i < m_affectedObjects.size(); i++) {
		if( m_affectedObjects[i] == otherObject){
			m_affectedObjects.erase(m_affectedObjects.begin() + i);
			return;
		}
	}
	
}

void ObjectWater::onSimulationEnded(){
	for (int i = 0; i < m_affectedObjects.size(); i++) {
		GameObject * obj =	m_affectedObjects[i];
		for (int i = 0; i < obj->m_bodies.size(); i++) {
			obj->m_bodies[i]->SetGravityScale(1);
		}
	}
	m_affectedObjects.clear();
}