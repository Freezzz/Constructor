//
//  ObjectPin.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/19/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectPin.h"
#include "../GameWorld.h"
#define PTM_RATIO 32.0f

INVENTORYITEM_GAMEOBJECT_NODE_DECL( PinInventoryItem , ObjectPin )

//////////////////////////////////////////////////// 
// ObjectPin init
//////////////////////////////////////////////////// 
bool ObjectPin::init(){
	m_objectSprite = CCSprite::spriteWithFile("pin.png");
    
	// Adapt container to the graphical rapresentation
	setContentSize(m_objectSprite->getContentSize());
	m_objectSprite->setAnchorPoint(CCPoint(0,0));	
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0
	
	addChild(m_objectSprite);
	
	isStatic = true;
	
	isMovable = true;
	isRotatable = false;
	isDeletable = true;
	
	moveButtonOffset = CCPoint(0, 30);
	rotateButtonOffset = CCPoint(0,-30);
	deleteButtonOffset = CCPoint(-30,0);

	// Permits to pin to be always over other objects
	defaultZOrder = 20;
	
	m_type = Pin;
	
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
	if (m_isPinned && m_pinJoint) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(m_pinJoint);		
		m_pinJoint = NULL;
	}
	m_isPinned = false;
	GameObject::onMovementStarted();
}

void ObjectPin::onMovementEnded(){
	GameObject::onMovementEnded();
	
	// Check if our sensor object collides with other GameObjects
	b2ContactEdge * cont = m_objectBody->GetContactList();
	if (cont != NULL) {
		b2Vec2 pnt1 = cont->contact->GetManifold()->points[0].localPoint;		
		b2Vec2 pnt2 = cont->contact->GetManifold()->points[1].localPoint;		
		
		// If contanc between two gameObjects. TODO: Improve check
		if (cont->contact->GetFixtureA()->GetBody()->GetUserData() != NULL && cont->contact->GetFixtureB()->GetBody()->GetUserData() != NULL) {
			
			b2Body * otherBody;
			if (cont->contact->GetFixtureA()->GetBody() == m_objectBody) {
				// Pin is A, other is B
				otherBody = cont->contact->GetFixtureB()->GetBody();
				
			}else if (cont->contact->GetFixtureB()->GetBody() == m_objectBody) {
				// Pin is B, other is A				
				otherBody = cont->contact->GetFixtureA()->GetBody();
			}
			
			CCPoint midPoint = CCPoint( (pnt1.x + pnt2.x) * 0.5, (pnt1.y + pnt2.y) * 0.5);
			
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
// Unpins this pin from body
//////////////////////////////////////////////////// 
void ObjectPin::unPin(bool destroyJoint){
	if (destroyJoint && m_pinJoint) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(m_pinJoint);
	}
	m_pinJoint = NULL;
	m_isPinned = false;
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