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
#define PTM_RATIO 32.0f

INVENTORYITEM_GAMEOBJECT_NODE_DECL( PinInventoryItem , ObjectPin )

//////////////////////////////////////////////////// 
// ObjectPin init
//////////////////////////////////////////////////// 
bool ObjectPin::init( std::string spritePath )
{
	m_objectSprite = CCSprite::spriteWithFile( spritePath.c_str() );

	// Adapt container to the graphical rapresentation
	setContentSize(m_objectSprite->getContentSize());
	m_objectSprite->setAnchorPoint(CCPoint(0,0));
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0

	addChild(m_objectSprite);

	isStatic = true;

	isMovable = true;
	isRotatable = false;
	isDeletable = true;

	rotateButtonOffset = CCPoint(30,0);
	deleteButtonOffset = CCPoint(-30,0);

	// Permits to pin to be always over other objects
	defaultZOrder = 20;

	m_type = Pin;
	m_isPinned = false;

	scheduleUpdate();
	return true;
}

void ObjectPin::onSimulationStarted(){
	saveOriginalProperties();
	m_objectBody->SetAwake(true);
	m_objectBody->SetType(b2_staticBody);
	// Hide pin if it is no pinned
	m_objectSprite->setIsVisible(m_isPinned);
}

void ObjectPin::onSimulationEnded(){
	m_objectSprite->setIsVisible(true);
	m_objectBody->SetType(b2_staticBody);
	restoreToOriginalProperties();
}

void ObjectPin::onMovementStarted(){
	unPin( 1 );
	GameObject::onMovementStarted();
}

void ObjectPin::onMovementEnded(){
	GameObject::onMovementEnded();
	rePin( );
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
	b2ContactEdge * cont = m_objectBody->GetContactList();
	if (cont != NULL) {

		// If contanc between two gameObjects. TODO: Improve check
		if (cont->contact->GetFixtureA()->GetBody()->GetUserData() != NULL && cont->contact->GetFixtureB()->GetBody()->GetUserData() != NULL) {

			b2Body * otherBody;
			if (cont->contact->GetFixtureA()->GetBody() == m_objectBody) {
				otherBody = cont->contact->GetFixtureB()->GetBody(); // Pin is A, other is B
			}
			else if (cont->contact->GetFixtureB()->GetBody() == m_objectBody) {
				otherBody = cont->contact->GetFixtureA()->GetBody(); // Pin is B, other is A
			}

			// Pin object to it's position in the world alowing rotation
			b2RevoluteJointDef md;
			md.Initialize(m_objectBody, otherBody, m_objectBody->GetPosition());
			md.referenceAngle = m_objectBody->GetAngle();
			m_pinJoint = (b2RevoluteJoint *)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&md);
			m_pinJoint->SetUserData(this);
			m_isPinned = true;
		}
		if (cont->next){
			CCLog("There are more objects coliding with pin but we ignore them");
		}
	}
}

//////////////////////////////////////////////////// 
// Creates a dummy sensor object to check collisions
// with other objects
//////////////////////////////////////////////////// 
void ObjectPin::createBodyAtPosition(cocos2d::CCPoint position){
	// Player physical body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	m_objectBody = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);
	
	// Define another box shape for our dynamic body.
    b2CircleShape circle;
    circle.m_radius = 1/2/PTM_RATIO;
	
	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.5f;
	fixtureDef.isSensor = true;
	m_objectBody->CreateFixture(&fixtureDef);
	m_objectBody->SetUserData(this);
	
	setPosition(position);	
}
void ObjectPin::setBody( b2Body *b )
{
	GameObject::setBody( b );

	m_pinJoint = NULL;
	m_isPinned = false;
	b2JointEdge *joint = b->GetJointList();
	if( joint ) {
		CCAssert( ! joint->next, "A pin is supposed to have at most one joint" );
		CCAssert( dynamic_cast<b2RevoluteJoint*>(joint->joint), "This pin has a joint which is not a revoletu one" );
		m_pinJoint = static_cast<b2RevoluteJoint*>( joint->joint );
		m_isPinned = true;
	}
}
