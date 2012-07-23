//
//  ObjectSimpleBox.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectSimpleBox.h"
#include "../GameWorld.h"
#include <iostream>

//////////////////////////////////////////////////// 
// ObjectSimpleBox init
//////////////////////////////////////////////////// 
bool ObjectSimpleBox::init( )
{
	m_type = SimpleBox;
	
	m_sprite = CCSprite::spriteWithFile( m_prototype["sprite path"].asCString() );

	// Adapt container to the graphical rapresentation
	setContentSize(m_sprite->getContentSize());
	m_sprite->setAnchorPoint(CCPoint(0,0));
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0
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
bool ObjectSimpleBox::createBodyAtPosition( cocos2d::CCPoint position )
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