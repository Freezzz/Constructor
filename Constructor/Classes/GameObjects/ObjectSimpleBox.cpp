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


bool SimpleBoxInventoryItem::init( std::string itemPath, std::string spritePath, b2FixtureDef *fixtureDef )
{
	m_itemSpritePath = itemPath;
	m_objectSpritePath = spritePath;
	m_fixtureDef = fixtureDef;
	
	m_objectSprite = CCSprite::spriteWithFile( m_itemSpritePath.c_str() );

	addChild( m_objectSprite );

	return true;
}
GameObject* SimpleBoxInventoryItem::gameObjectNode( b2Body *b )
{
	GameObject *go = ObjectSimpleBox::node( this, b, m_objectSpritePath, m_fixtureDef );
	go->isStatic = isStatic;
	go->isMovable = isMovable;
	go->isRotatable = isRotatable;
	go->isDeletable = isDeletable;
	return go;
}
GameObject* SimpleBoxInventoryItem::gameObjectNode( CCPoint p )
{
	GameObject *go = ObjectSimpleBox::node( this, p, m_objectSpritePath, m_fixtureDef );
	go->isStatic = isStatic;
	go->isMovable = isMovable;
	go->isRotatable = isRotatable;
	go->isDeletable = isDeletable;
	return go;
}

//////////////////////////////////////////////////// 
// ObjectSimpleBox init
//////////////////////////////////////////////////// 
bool ObjectSimpleBox::init( std::string spritePath, b2FixtureDef *fixtureDef )
{
	m_objectSprite = CCSprite::spriteWithFile( spritePath.c_str() );

	m_fixtureDef = fixtureDef;

	// Adapt container to the graphical rapresentation
	setContentSize(m_objectSprite->getContentSize());
	m_objectSprite->setAnchorPoint(CCPoint(0,0));
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0

	addChild(m_objectSprite);

	defaultZOrder = 1;

	rotateButtonOffset = CCPoint( getContentSize().width/2 + 10, 0 );
	deleteButtonOffset = CCPoint( -getContentSize().width/2 - 10, 0 );

	m_type = SimpleBox;
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
void ObjectSimpleBox::createBodyAtPosition( cocos2d::CCPoint position )
{
	// Player physical body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set( position.x/PTM_RATIO, position.y/PTM_RATIO );
	m_objectBody = GameWorld::sharedGameWorld()->physicsWorld->CreateBody( &bodyDef );

	m_objectBody->CreateFixture( m_fixtureDef );
	m_objectBody->SetUserData( this );
	
	setPosition(position);	
}