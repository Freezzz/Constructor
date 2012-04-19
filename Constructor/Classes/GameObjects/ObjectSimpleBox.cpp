//
//  ObjectSimpleBox.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectSimpleBox.h"
#include "../GameWorld.h"
#define PTM_RATIO 32.0f

INVENTORYITEM_GAMEOBJECT_NODE_DECL( SimpleBoxInventoryItem, ObjectSimpleBox )

//////////////////////////////////////////////////// 
// ObjectSimpleBox init
//////////////////////////////////////////////////// 
bool ObjectSimpleBox::init(){
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
	m_type = SimpleBox;
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
	
	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_objectSprite->getContentSize().width/2/PTM_RATIO, m_objectSprite->getContentSize().height/2/PTM_RATIO);
	
	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.3f;
	fixtureDef.isSensor = false;
	m_objectBody->CreateFixture(&fixtureDef);
	m_objectBody->SetUserData(this);
	
	setPosition(position);	
}
