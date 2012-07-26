//
//  ObjectCrumb.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectCrumb.h"
#include "ObjectFatty.h"
#include "../GameWorld.h"
#include "GameLevelScene.h"
#include "RaysCastCallback.h"

//////////////////////////////////////////////////// 
// ObjectCrumb init
//////////////////////////////////////////////////// 
bool ObjectCrumb::init( )
{
	m_type = Crumb;
	
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
	m_isEaten = false;
	scheduleUpdate();
	return true;
}


//////////////////////////////////////////////////// 
// Body creation from prototype
//////////////////////////////////////////////////// 
bool ObjectCrumb::createBodyAtPosition( cocos2d::CCPoint position )
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

void ObjectCrumb::update(ccTime dt){
	GameObject::update(dt);
	
	if (!GameLevelScene::sharedGameScene()->isSimulating() || isEaten()) {
		return;
	}
	
	ObjectFatty * target = (ObjectFatty*) GameLevelScene::sharedGameScene()->m_target;

	RaysCastCallback callback;
	GameWorld::sharedGameWorld()->physicsWorld->RayCast(&callback, m_bodies[0]->GetPosition(), target->m_bodies[1]->GetPosition());
	
	if (callback.m_fixture && callback.m_fixture->GetBody() == target->m_bodies[1]) {    
		m_canSee = true;
		
		target->applyTorque(m_bodies[0]->GetPosition().x - target->m_bodies[1]->GetPosition().x);
		
    }else {
		m_canSee = false;
	}
	
}

//////////////////////////////////////////////////// 
// Debug purpose only draws los of the mouse
//////////////////////////////////////////////////// 
void ObjectCrumb::draw(){
	GameObject::draw();
	if (isEaten()) {
		return;
	}
	if (m_canSee) {
		glColor4ub(0, 255, 0, 255);
	} else {
		glColor4ub(255, 0, 0, 255);
	}
	
	CCPoint p1 = CCPoint(getPosition().x / PTM_RATIO, getPosition().y / PTM_RATIO);
	CCPoint p2 = convertToNodeSpace(GameLevelScene::sharedGameScene()->m_target->getPosition());
	ccDrawLine(p1, p2);
}

void ObjectCrumb::setIsEaten(bool value){
	m_isEaten = value;
	setIsVisible(!value);
	m_body->GetFixtureList()->SetSensor(value);
}

//////////////////////////////////////////////////// 
// Collision handler
//////////////////////////////////////////////////// 
void ObjectCrumb::objectCollided(GameObject *otherObject, b2Contact * contact){
	if(otherObject->m_type == Fatty){
		setIsEaten(true);
	}
}

//////////////////////////////////////////////////// 
// Callback on simulation ended
//////////////////////////////////////////////////// 
void ObjectCrumb::onSimulationEnded(){
	GameObject::onSimulationEnded();
	setIsEaten(false);
}