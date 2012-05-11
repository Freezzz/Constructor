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
	b2ContactEdge * cont = m_objectBody->GetContactList();
	while (cont != NULL) {

		// If contanc between two gameObjects. TODO: Improve check
		if (cont->contact->GetFixtureA()->GetBody()->GetUserData() != NULL
			&& cont->contact->GetFixtureB()->GetBody()->GetUserData() != NULL
			&& cont->contact->IsTouching()) {
			if (cont->contact->GetFixtureA()->GetBody() == m_objectBody) {
				otherBody = cont->contact->GetFixtureB()->GetBody(); // Pin is A, other is B
			}
			else if (cont->contact->GetFixtureB()->GetBody() == m_objectBody) {
				otherBody = cont->contact->GetFixtureA()->GetBody(); // Pin is B, other is A
			}
		}
		cont = cont->next;
	}
	if (otherBody) {
		// Pin object to it's position in the world alowing rotation		
		b2RevoluteJointDef md;
		md.Initialize(m_objectBody, otherBody, m_objectBody->GetPosition());
		md.referenceAngle = m_objectBody->GetAngle();
		m_pinJoint = (b2RevoluteJoint *)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&md);
		m_pinJoint->SetUserData(this);
		m_isPinned = true;
	}
}




//////////////////////////////////////////////////// 
// Creates a dummy sensor object to check collisions
// with other objects
//////////////////////////////////////////////////// 
bool ObjectPin::createBodyAtPosition( cocos2d::CCPoint position )
{
	b2dJson json;
	m_objectBody = json.j2b2Body( physicsWorld(), prototype() );
	if( ! m_objectBody || ! m_objectBody->GetFixtureList() ) {
		std::cout << "Pin inventory item prototype messed up" << std::endl;
		return false;
	}
	m_objectBody->SetUserData(this);
	m_objectBody->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_objectBody->GetAngle() );
	setPosition(position);
	return true;
}
bool ObjectPin::setBody( b2Body *b )
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
	return true;
}
