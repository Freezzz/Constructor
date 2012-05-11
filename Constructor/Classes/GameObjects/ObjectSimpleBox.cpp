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

INVENTORYITEM_GAMEOBJECT_NODE_DECL( SimpleBoxInventoryItem , ObjectSimpleBox )

//////////////////////////////////////////////////// 
// ObjectSimpleBox init
//////////////////////////////////////////////////// 
bool ObjectSimpleBox::init( std::string spritePath )
{
	m_objectSprite = CCSprite::spriteWithFile( spritePath.c_str() );

	// Adapt container to the graphical rapresentation
	setContentSize(m_objectSprite->getContentSize());
	m_objectSprite->setAnchorPoint(CCPoint(0,0));
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0
	addChild(m_objectSprite);
	defaultZOrder = 1;

	rotateButtonOffset = CCPoint( getContentSize().width/2 + 10, 0 );
	deleteButtonOffset = CCPoint( -getContentSize().width/2 - 10, 0 );

	m_state = Idile;
	m_moveJoint = NULL;
	m_rotationJoin = NULL;
	m_objectBodyPin = NULL;
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

	m_objectBody = json.j2b2Body( physicsWorld(), prototype() );
	if( ! m_objectBody || ! m_objectBody->GetFixtureList() ) {
		std::cout << "Simple inventory item prototype messed up" << std::endl;
		return false;
	}
	m_objectBody->SetUserData( this );
	m_objectBody->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_objectBody->GetAngle() );
	setPosition(position);
	return true;
}