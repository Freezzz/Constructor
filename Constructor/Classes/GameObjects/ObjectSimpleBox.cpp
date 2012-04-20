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
	if( m_type == SimpleBox ) {
		m_objectSprite = CCSprite::spriteWithFile( "box.png" );
	}
	else if( m_type == FixedPoint ) {
		m_objectSprite = CCSprite::spriteWithFile( "fixed.png" );
	}
	else if( m_type == Panel ) {
		m_objectSprite = CCSprite::spriteWithFile( "panel_btn.png" );
	}
	else {
		std::cout << "I shouldn't be here" << std::endl;
		* ((int*) 0 ) = 0;
	}
	
	addChild( m_objectSprite );
	
	return true;
}
GameObject* SimpleBoxInventoryItem::gameObjectNode( )
{
	return ObjectSimpleBox::node( m_type );
}

//////////////////////////////////////////////////// 
// ObjectSimpleBox init
//////////////////////////////////////////////////// 
bool ObjectSimpleBox::init(){

	if( m_simpleType == SimpleBox ) {
		m_objectSprite = CCSprite::spriteWithFile("Icon-Small-50.png");

		// Adapt container to the graphical rapresentation
		setContentSize(m_objectSprite->getContentSize());
		m_objectSprite->setAnchorPoint(CCPoint(0,0));
		setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0

		addChild(m_objectSprite);
		
		isStatic = false;
		isMovable = true;
		isRotatable = true;
		isDeletable = true;

		moveButtonOffset = CCPoint(0, 30);
		rotateButtonOffset = CCPoint(0,-30);
		deleteButtonOffset = CCPoint(-30,0);

		defaultZOrder = 1;
	}
	else if( m_simpleType == FixedPoint ) {
		m_objectSprite = CCSprite::spriteWithFile("circle.png");

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

		defaultZOrder = 1;
	}
	else if( m_simpleType == Panel ) {
		// Add sprite and adapt container
		m_objectSprite = CCSprite::spriteWithFile("panel.png");

		// Adapt container to the graphical rapresentation
		setContentSize(m_objectSprite->getContentSize());
		m_objectSprite->setAnchorPoint(CCPoint(0,0));
		setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0

		addChild(m_objectSprite);

		isStatic = false;
		isMovable = true;
		isRotatable = true;
		isDeletable = true;

		moveButtonOffset = CCPoint(0, 10);
		rotateButtonOffset = CCPoint(0,-20);
		deleteButtonOffset = CCPoint(-100,0);

		defaultZOrder = 1;
	}
	else {
		std::cout << "I shouldn't be here" << std::endl;
		* ((int*) 0 ) = 0;
	}

	m_type = m_simpleType;
	m_state = Idile;
	m_moveJoint = NULL;
	
	scheduleUpdate();
	return true;
}


//////////////////////////////////////////////////// 
// 
//////////////////////////////////////////////////// 
void ObjectSimpleBox::createBodyAtPosition(cocos2d::CCPoint position){
	// Player physical body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	m_objectBody = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	
	b2CircleShape circle;
	b2PolygonShape dynamicBox;
	
	if( m_type == SimpleBox ) {
		// Define another box shape for our dynamic body.
		dynamicBox.SetAsBox(m_objectSprite->getContentSize().width/2/PTM_RATIO, m_objectSprite->getContentSize().height/2/PTM_RATIO);
		
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.3f;
		fixtureDef.isSensor = false;
	}
	else if( m_type == FixedPoint ) {
		// Define another box shape for our dynamic body.
		circle.m_radius = m_objectSprite->getContentSize().width/2/PTM_RATIO;

		fixtureDef.shape = &circle;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.1f;
		fixtureDef.restitution = 0.5f;
		fixtureDef.isSensor = false;
	}
	else if( m_type == Panel ) {
		// Define another box shape for our dynamic body.
		dynamicBox.SetAsBox(m_objectSprite->getContentSize().width/2/PTM_RATIO, m_objectSprite->getContentSize().height/2/PTM_RATIO);

		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.3f;
		fixtureDef.isSensor = false;
	}
	else {
		std::cout << "I shouldn't be here" << std::endl;
		* ((int*) 0 ) = 0;
	}
		
	m_objectBody->CreateFixture(&fixtureDef);
	m_objectBody->SetUserData(this);
	
	setPosition(position);	
}