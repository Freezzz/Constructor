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

	m_bodies.push_back( m_body );

	return true;
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
		
		m_fattySprite->setFlipX( body->GetLinearVelocity().x < 0 );
	}
}