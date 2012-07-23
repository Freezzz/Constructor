//
//  ObjectOven.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectOven.h"
#include "GameLevelScene.h"
#include "../GameWorld.h"
#include <iostream>

//////////////////////////////////////////////////// 
// ObjectOven init
//////////////////////////////////////////////////// 
bool ObjectOven::init( )
{
	m_type = SimpleBox;
	
	m_sprite = CCSprite::spriteWithFile( m_prototype["oven"]["sprite path"].asCString() );
    m_flameSprite = CCSprite::spriteWithFile( m_prototype["flames"]["sprite path"].asCString() );
	
	// Adapt container to the graphical rapresentation
	setContentSize(m_sprite->getContentSize());
	m_sprite->setAnchorPoint(CCPoint(0,0));
	m_flameSprite->setAnchorPoint(CCPoint(0,0));
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0
	addChild(m_sprite);
	addChild(m_flameSprite);
	defaultZOrder = 1;

	rotateButtonOffset = CCPoint( getContentSize().width/2 + 10, 0 );
	deleteButtonOffset = CCPoint( -getContentSize().width/2 - 10, 0 );

	m_state = Idile;
	m_affectedObjects = std::vector<GameObject*>();
	scheduleUpdate();
	setIsOn(false);
	return true;
}


//////////////////////////////////////////////////// 
// Body creation from prototype
//////////////////////////////////////////////////// 
bool ObjectOven::createBodyAtPosition( cocos2d::CCPoint position )
{
	b2dJson json;

	// Flames
	m_flameSensorOffset = CCPoint(m_prototype["flames"]["offset"]["x"].asFloat(), m_prototype["flames"]["offset"]["y"].asFloat());
	m_flameSprite->setPosition(m_flameSensorOffset);
	
	CCPoint flameSensorPosition = CCPoint(position.x + m_flameSensorOffset.x, position.y + m_flameSensorOffset.y);
	
	m_flameSensor = json.j2b2Body( physicsWorld(), prototype()["flames"] );
	if( ! m_flameSensor || ! m_flameSensor->GetFixtureList() ) {
		std::cout << "Simple inventory item prototype messed up" << std::endl;
		return false;
	}
	m_flameSensor->SetUserData( this );
	m_flameSensor->SetTransform( b2Vec2(flameSensorPosition.x/PTM_RATIO, flameSensorPosition.y/PTM_RATIO), m_flameSensor->GetAngle() );
	m_bodies.push_back( m_flameSensor );
	
	
	// Button
	m_buttonSensorOffset = CCPoint(m_prototype["button"]["offset"]["x"].asFloat(), m_prototype["button"]["offset"]["y"].asFloat());
	
	CCPoint buttonSensorPosition = CCPoint(position.x + m_buttonSensorOffset.x, position.y + m_buttonSensorOffset.y);
	
	m_buttonSensor = json.j2b2Body( physicsWorld(), prototype()["button"] );
	if( ! m_buttonSensor || ! m_buttonSensor->GetFixtureList() ) {
		std::cout << "Simple inventory item prototype messed up" << std::endl;
		return false;
	}
	m_buttonSensor->SetUserData( this );
	m_buttonSensor->SetTransform( b2Vec2(buttonSensorPosition.x/PTM_RATIO, buttonSensorPosition.y/PTM_RATIO), m_buttonSensor->GetAngle() );
	m_bodies.push_back( m_buttonSensor );
	
	// Main oven
	m_body = json.j2b2Body( physicsWorld(), prototype()["oven"] );
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

void ObjectOven::setIsOn(bool value){
	if (!value) {
		m_flameSprite->stopAllActions();
		m_flameSprite->setIsVisible(false);
	}else {
		m_flameSprite->setIsVisible(true);
		CCActionInterval*  action1 = CCTintTo::actionWithDuration(0.3, 255, 0, 0);
		CCActionInterval*  action1Back = CCTintTo::actionWithDuration(0.3, 255, 255, 255);
		CCFiniteTimeAction * seq = CCSequence::actions( action1, action1Back, NULL);
		CCRepeatForever *repeat = CCRepeatForever::actionWithAction((CCActionInterval*)seq);
		m_flameSprite->runAction(repeat);
		
		CCActionInterval*  action2 = CCScaleTo::actionWithDuration(0.3, 1, 0.8);
		CCActionInterval*  action2Back = CCScaleTo::actionWithDuration(0.3, 1, 1);
		CCFiniteTimeAction * seq2 = CCSequence::actions( action2, action2Back, NULL);
		CCRepeatForever *repeat2 = CCRepeatForever::actionWithAction((CCActionInterval*)seq2);
		m_flameSprite->runAction(repeat2);
	}
	
	m_isOn = value;
}

//////////////////////////////////////////////////// 
// Collision handler
//////////////////////////////////////////////////// 
void ObjectOven::objectCollided(GameObject *otherObject, b2Contact * contact){
	// Fing wich of oven object collided
	b2Body * ovenPart;
	if( contact->GetFixtureA()->GetBody()->GetUserData() == otherObject ){
		ovenPart = contact->GetFixtureB()->GetBody();
	}else {
		ovenPart = contact->GetFixtureA()->GetBody();
	}
	
	if (ovenPart == m_flameSensor && getIsOn()) {
		CCLog("BUUUURN");
	}else if (ovenPart == m_buttonSensor) {
		setIsOn(true);
	}
	
	if(otherObject->m_type == Fatty && getIsOn()){
		GameLevelScene::sharedGameScene()->enterDefeat();
	}
}

//////////////////////////////////////////////////// 
// Collision ended handler
//////////////////////////////////////////////////// 
void ObjectOven::objectCollisionEnded(GameObject *otherObject, b2Contact * contact){
//	for (int i = 0; i < otherObject->m_bodies.size(); i++) {
//		otherObject->m_bodies[i]->SetGravityScale(1);
//	}
//	
//	for (int i = 0; i < m_affectedObjects.size(); i++) {
//		if( m_affectedObjects[i] == otherObject){
//			m_affectedObjects.erase(m_affectedObjects.begin() + i);
//			return;
//		}
//	}
//	
}

void ObjectOven::onSimulationEnded(){
//	for (int i = 0; i < m_affectedObjects.size(); i++) {
//		GameObject * obj =	m_affectedObjects[i];
//		for (int i = 0; i < obj->m_bodies.size(); i++) {
//			obj->m_bodies[i]->SetGravityScale(1);
//		}
//	}
//	m_affectedObjects.clear();
	setIsOn(false);
}


////////////////////////////////////////////////////
// Moves object to new location, if state is idile
// than it is a simple translation, if moving than
// creates a move joint to move object around
////////////////////////////////////////////////////
void ObjectOven::move(cocos2d::CCPoint newPostion){
	if( ! getParent() ) {
		// not moving if this object has no parent
		return ;
	}
	b2Vec2 b2Position = b2Vec2(newPostion.x/PTM_RATIO,
							   newPostion.y/PTM_RATIO);
	
	float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS( getRotation() );
	setPosition( newPostion );
	m_body->SetTransform( b2Position, b2Angle );
	
	CCPoint flameSensorPosition = CCPoint(newPostion.x + m_flameSensorOffset.x, newPostion.y + m_flameSensorOffset.y);
	m_flameSensor->SetTransform( b2Vec2(flameSensorPosition.x/PTM_RATIO, flameSensorPosition.y/PTM_RATIO), m_flameSensor->GetAngle() );

	CCPoint buttonSensorPosition = CCPoint(newPostion.x + m_buttonSensorOffset.x, newPostion.y + m_buttonSensorOffset.y);
	m_buttonSensor->SetTransform( b2Vec2(buttonSensorPosition.x/PTM_RATIO, buttonSensorPosition.y/PTM_RATIO), m_flameSensor->GetAngle() );
	
}
