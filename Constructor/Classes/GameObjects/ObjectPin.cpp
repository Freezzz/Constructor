//
//  ObjectPin.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/19/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include <iostream>
#include "ObjectPin.h"
#include "../GameWorld.h"

//////////////////////////////////////////////////// 
// ObjectPin init
//////////////////////////////////////////////////// 
bool ObjectPin::init( )
{
	m_type = Pin;
	
	m_sprite = CCSprite::spriteWithFile( m_prototype["sprite path"].asCString() );

	// Adapt container to the graphical rapresentation
	setContentSize(m_sprite->getContentSize());
	m_sprite->setAnchorPoint(CCPoint(0,0));
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0

	addChild(m_sprite);

	m_pinJoint = 0;

	rotateButtonOffset = CCPoint(30,0);
	deleteButtonOffset = CCPoint(-30,0);

	// Permits to pin to be always over other objects
	defaultZOrder = 20;

	m_isPinned = false;

	scheduleUpdate();
	return true;
}

void ObjectPin::onSimulationStarted(){
	saveOriginalProperties();
	m_body->SetAwake(true);
	m_body->SetType(b2_staticBody);
	// Hide pin if it is no pinned
	m_sprite->setIsVisible(m_isPinned);
}

void ObjectPin::onSimulationEnded(){
	m_sprite->setIsVisible(true);
	m_body->SetType(b2_staticBody);
	restoreToOriginalProperties();
}

void ObjectPin::onMovementStarted(){
	unPin( 1 );

	// Sensor objects like pin/grue need collision detection to pin them
	m_body->SetType( b2_dynamicBody );
	// Setting gravity scale to 0 avoid them to fall down while user drags them
	m_body->SetGravityScale( 0 );

	m_body->SetFixedRotation(true); // disable rotation
}

void ObjectPin::onMovementEnded(){
	rePin( );	
	GameObject::onMovementEnded();
}

//////////////////////////////////////////////////// 
// Unpins this pin from body
//////////////////////////////////////////////////// 
void ObjectPin::unPin( bool destroyJoint )
{
	if (destroyJoint && m_pinJoint) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(m_pinJoint);
	}
	m_pinJoint = NULL;
	m_isPinned = false;
}
void ObjectPin::rePin( )
{
	// Check if our sensor object collides with other GameObjects
	b2Body * otherBody = NULL;
	b2ContactEdge * cont = m_body->GetContactList();
	while (cont != NULL) {

		// If contanc between two gameObjects. TODO: Improve check
		if (cont->contact->GetFixtureA()->GetBody()->GetUserData() != NULL
			&& cont->contact->GetFixtureB()->GetBody()->GetUserData() != NULL
			&& cont->contact->IsTouching()) {
			if (cont->contact->GetFixtureA()->GetBody() == m_body) {
				otherBody = cont->contact->GetFixtureB()->GetBody(); // Pin is A, other is B
			}
			else if (cont->contact->GetFixtureB()->GetBody() == m_body) {
				otherBody = cont->contact->GetFixtureA()->GetBody(); // Pin is B, other is A
			}
		}
		cont = cont->next;
	}
	if (otherBody) {
		// Pin object to it's position in the world alowing rotation		
		b2RevoluteJointDef md;
		md.Initialize(m_body, otherBody, m_body->GetPosition());
		md.referenceAngle = m_body->GetAngle();
		m_pinJoint = (b2RevoluteJoint *)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&md);
		m_pinJoint->SetUserData(this);
		m_isPinned = true;
	}
}


void ObjectPin::startUnstuckPhase(){
	return;
}


void ObjectPin::unstuckPhaseFinished(){
	return;
}

//////////////////////////////////////////////////// 
// Creates a dummy sensor object to check collisions
// with other objects
//////////////////////////////////////////////////// 
bool ObjectPin::createBodyAtPosition( cocos2d::CCPoint position )
{
	b2dJson json;
	m_body = json.j2b2Body( physicsWorld(), prototype() );
	if( ! m_body || ! m_body->GetFixtureList() ) {
		std::cout << "Pin inventory item prototype messed up" << std::endl;
		return false;
	}
	m_body->SetUserData(this);
	m_body->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_body->GetAngle() );
	setPosition(position);

	m_bodies.push_back( m_body );

	return true;
}
