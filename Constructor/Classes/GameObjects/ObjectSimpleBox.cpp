//
//  ObjectSimpleBox.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectSimpleBox.h"
#include "GameWorld.h"
#define PTM_RATIO 32.0f

//////////////////////////////////////////////////// 
// ObjectSimpleBox init
//////////////////////////////////////////////////// 
bool ObjectSimpleBox::init(){
    this->objectSprite = CCSprite::spriteWithFile("Icon-Small-50.png");
    
	// Adapt container to the graphical rapresentation
	this->setContentSize(objectSprite->getContentSize());
	this->objectSprite->setAnchorPoint(CCPoint(0,0));	
	this->setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0

    this->addChild(objectSprite);
    this->isStatic = false;
    this->scheduleUpdate();
	return true;
}


//////////////////////////////////////////////////// 
// <#var#>
//////////////////////////////////////////////////// 
void ObjectSimpleBox::createBodyAtPosition(cocos2d::CCPoint position){
	// Player physical body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	objectBody = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);
	
	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(objectSprite->getContentSize().width/2/PTM_RATIO, objectSprite->getContentSize().height/2/PTM_RATIO);
	
	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.3f;
	fixtureDef.isSensor = false;
	objectBody->CreateFixture(&fixtureDef);
	objectBody->SetUserData(this);
}
