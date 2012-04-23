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
#define PTM_RATIO 32.0f


bool SimpleBoxInventoryItem::init( )
{
	m_fixtureDef = new b2FixtureDef;

	if( m_type == SimpleBox ) {
		m_itemSpritePath = "box.png";
		m_objectSpritePath = "Icon-Small-50.png";

		m_objectSprite = CCSprite::spriteWithFile( m_itemSpritePath.c_str() );
		
		// Define another box shape for our dynamic body.
		CCSprite *s = CCSprite::spriteWithFile( m_objectSpritePath.c_str() );
		b2PolygonShape *dynamicBox = new b2PolygonShape;
		dynamicBox->SetAsBox( s->getContentSize().width/2/PTM_RATIO, s->getContentSize().height/2/PTM_RATIO );

		m_fixtureDef->shape = dynamicBox;
		m_fixtureDef->density = 1.0f;
		m_fixtureDef->friction = 0.3f;
		m_fixtureDef->restitution = 0.3f;
		m_fixtureDef->isSensor = false;
		
		isStatic = false;
		isMovable = true;
		isRotatable = true;
		isDeletable = true;
	}
	else if( m_type == FixedPoint ) {
		m_itemSpritePath = "fixed.png";
		m_objectSpritePath = "circle.png";

		m_objectSprite = CCSprite::spriteWithFile( m_itemSpritePath.c_str() );

		// Define another box shape for our dynamic body.
		CCSprite *s = CCSprite::spriteWithFile( m_objectSpritePath.c_str() );
		b2CircleShape *circle = new b2CircleShape;
		circle->m_radius = s->getContentSize().width/2/PTM_RATIO;

		m_fixtureDef->shape = circle;
		m_fixtureDef->density = 1.0f;
		m_fixtureDef->friction = 0.1f;
		m_fixtureDef->restitution = 0.5f;
		m_fixtureDef->isSensor = false;
		
		isStatic = true;
		isMovable = true;
		isRotatable = false;
		isDeletable = true;
	}
	else if( m_type == Panel ) {
		m_itemSpritePath = "panel_btn.png";
		m_objectSpritePath = "panel.png";

		m_objectSprite = CCSprite::spriteWithFile( m_itemSpritePath.c_str() );

		// Define another box shape for our dynamic body.
		CCSprite *s = CCSprite::spriteWithFile( m_objectSpritePath.c_str() );
		b2PolygonShape *dynamicBox = new b2PolygonShape;
		dynamicBox->SetAsBox( s->getContentSize().width/2/PTM_RATIO, s->getContentSize().height/2/PTM_RATIO );

		m_fixtureDef->shape = dynamicBox;
		m_fixtureDef->density = 1.0f;
		m_fixtureDef->friction = 0.3f;
		m_fixtureDef->restitution = 0.3f;
		m_fixtureDef->isSensor = false;
		
		isStatic = false;
		isMovable = true;
		isRotatable = true;
		isDeletable = true;
	}
	else {
		CCAssert(true, "SimpleBoxInventoryItem::Init - Invalid object type!");
	}
	
	addChild( m_objectSprite );
	
	return true;
}
bool SimpleBoxInventoryItem::init( std::string itemPath, std::string spritePath, b2FixtureDef *fixtureDef )
{
	m_itemSpritePath = itemPath;
	m_objectSpritePath = spritePath;
	m_fixtureDef = fixtureDef;
	
	m_objectSprite = CCSprite::spriteWithFile( m_itemSpritePath.c_str() );

	addChild( m_objectSprite );

	return true;
}
GameObject* SimpleBoxInventoryItem::gameObjectNode( CCPoint p )
{
	GameObject *go = ObjectSimpleBox::node( p, m_objectSpritePath, m_fixtureDef );
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

	moveButtonOffset = CCPoint( 0, getContentSize().height/2 + 10 );
	rotateButtonOffset = CCPoint( 0,-getContentSize().height/2 - 10 );
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