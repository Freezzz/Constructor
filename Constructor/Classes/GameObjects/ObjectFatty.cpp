//
//  ObjectFatty.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectFatty.h"
#include "../GameWorld.h"
#include <iostream>

//////////////////////////////////////////////////// 
// ObjectFatty init
//////////////////////////////////////////////////// 
bool ObjectFatty::init( )
{
	m_type = Fatty;
	
	m_sprite = CCSprite::spriteWithFile( m_prototype["sprite path"].asCString() );

	m_fattySprite = CCSprite::spriteWithFile("fatty.png");
	addChild(m_fattySprite);
	
	// Adapt container to the graphical rapresentation
	setContentSize(m_sprite->getContentSize());
	addChild(m_sprite);
	defaultZOrder = 1;

	rotateButtonOffset = CCPoint( getContentSize().width/2 + 10, 0 );
	deleteButtonOffset = CCPoint( -getContentSize().width/2 - 10, 0 );

	m_state = Idile;
	scheduleUpdate();
	return true;
}


//////////////////////////////////////////////////// 
// 
//////////////////////////////////////////////////// 
bool ObjectFatty::createBodyAtPosition( cocos2d::CCPoint position )
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
	
	b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
	m_contactSensor = physicsWorld()->CreateBody(&bodyDef);
	m_contactSensor->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_body->GetAngle() );
	
	m_bodies.push_back( m_contactSensor );
	m_bodies.push_back( m_body );

	
	b2RevoluteJointDef md;
	md.Initialize(m_body, m_contactSensor, m_body->GetPosition());
	md.maxMotorTorque = 200;
	md.enableMotor = true;
	m_motorJoint = (b2RevoluteJoint *)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&md);
	
	saveOriginalProperties();
	return true;
}


void ObjectFatty::applyTorque(float value){
	if (value >= 0) {
		value = 100;
	}else {
		value = -100;
	}
	m_motorJoint->SetMotorSpeed(value);
//	m_body -> ApplyForceToCenter(b2Vec2(value, 0));
}

//////////////////////////////////////////////////// 
// Basic object update loop, moves sprite to body location
//////////////////////////////////////////////////// 
void ObjectFatty::update( ccTime dt )
{
	if( ! getParent() ) {
		// doing nothing if this object has no parent
		return ;
	}
	
	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;
		
		// Update posisiotn of node moving it to body postion
		setPosition( CCPointMake( body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO ) );

		// Updating only wheel rotation
		m_sprite->setRotation( -1 * CC_RADIANS_TO_DEGREES( body->GetAngle() ) );
		
        // Flip sprite based on moving direction
		m_fattySprite->setFlipX( body->GetLinearVelocity().x < 0 );
	}
}