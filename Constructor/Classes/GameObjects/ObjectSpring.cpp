//
//  ObjectSpring.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectSpring.h"
#include "ObjectSimpleBox.h"
#include "GameLevelScene.h"
#include "../GameWorld.h"

#define MAX_LENGHT 132
#define MIN_LENGHT 5


//////////////////////////////////////////////////// 
// ObjectSpring init
//////////////////////////////////////////////////// 
bool ObjectSpring::init( )
{
	// adapt container to the graphical rapresentation
	m_springSprite = CCSprite::spriteWithFile( m_prototype["spring sprite path"].asCString() );
	m_springSprite->setAnchorPoint(CCPoint(0,0.5));
	m_springSprite->setScaleY(0.3);
	
	setAnchorPoint(CCPoint(0.5,0)); // CCNode AP default is 0,0
	addChild(m_springSprite);

	m_firstBodySprite = CCSprite::spriteWithFile( m_prototype["spring top"]["sprite path"].asCString() );
	addChild(m_firstBodySprite);
	m_firstBodySprite->setAnchorPoint(CCPoint(0.5,0.5));
	
	m_secondBodySprite = CCSprite::spriteWithFile( m_prototype["spring bottom"]["sprite path"].asCString() );
	addChild(m_secondBodySprite);
	m_secondBodySprite->setAnchorPoint(CCPoint(0.5,0.5));	
	
	setContentSize(CCSize(m_secondBodySprite->getContentSize().width, m_secondBodySprite->getContentSize().height * 2));
	
	isStatic = false;
	isMovable = true;
	isRotatable = false;
	isDeletable = true;
	
	rotateButtonOffset = CCPoint(0,-30);
	deleteButtonOffset = CCPoint(-30,0);

	defaultZOrder = 1;
	m_joints = vector<b2DistanceJoint*>();
	
	scheduleUpdate();	
	return true;
}

//////////////////////////////////////////////////// 
// Create two bodyes and a joint to represent a spring
//////////////////////////////////////////////////// 
bool ObjectSpring::createBodyAtPosition( cocos2d::CCPoint position )
{
	b2dJson json;

#define ASSERT_PROTOTYPE(e) \
	if( ! (e) ) { \
		std::cout << "Spring inventory item prototype messed up" << std::endl; \
		return false; \
	}

	ASSERT_PROTOTYPE( m_firstBody = json.j2b2Body( physicsWorld(), prototype()["spring top"] ) );
	ASSERT_PROTOTYPE( m_firstBody->GetFixtureList() );
	m_firstBody->SetUserData(this);
	m_firstBody->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_firstBody->GetAngle() );

	ASSERT_PROTOTYPE( m_secondBody = json.j2b2Body( physicsWorld(), prototype()["spring bottom"] ) );
	ASSERT_PROTOTYPE( m_secondBody->GetFixtureList() );
	m_secondBody->SetUserData(this);
	m_secondBody->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_secondBody->GetAngle() );

	b2DistanceJointDef jointDef1;
	jointDef1.bodyA = m_firstBody;
	jointDef1.bodyB = m_secondBody;
	jointDef1.localAnchorA.Set(1, 0);
	jointDef1.localAnchorB.Set(1, 0);
	jointDef1.length = MIN_LENGHT / PTM_RATIO;
	jointDef1.frequencyHz = 20;
	jointDef1.dampingRatio = 1;
	jointDef1.collideConnected = true;

	b2PrismaticJointDef jointDef2;
	jointDef2.Initialize(m_firstBody, m_secondBody, b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), b2Vec2(0,1));
	jointDef2.collideConnected = true;

	b2DistanceJointDef jointDef3;
	jointDef3.bodyA = m_firstBody;
	jointDef3.bodyB = m_secondBody;
	jointDef3.localAnchorA.Set(-1, 0);
	jointDef3.localAnchorB.Set(-1, 0);
	jointDef3.length = MIN_LENGHT / PTM_RATIO;
	jointDef3.frequencyHz = 20;
	jointDef3.dampingRatio = 1;
	jointDef3.collideConnected = true;
	
	m_offsetBetweenBodies = b2Vec2(0, (m_firstBodySprite->getContentSize().height / PTM_RATIO) + MIN_LENGHT / PTM_RATIO );

#undef ASSERT_PROTOTYPE

	m_joints.push_back((b2DistanceJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef1));
	m_prismaticJoint = (b2PrismaticJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef2);
	m_joints.push_back((b2DistanceJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef3));
	setPosition(position);

	m_bodies.push_back( m_firstBody );
	m_bodies.push_back( m_secondBody );

	return true;
}


void ObjectSpring::update(ccTime dt){
	if (getParent() && m_firstBody) {
		// Update posisiotn of node moving it to body postion
		CCPoint p1 = CCPoint(m_firstBody->GetPosition().x * PTM_RATIO, m_firstBody->GetPosition().y * PTM_RATIO);
		CCPoint p2 = CCPoint(m_secondBody->GetPosition().x * PTM_RATIO, m_secondBody->GetPosition().y * PTM_RATIO);

		float distance = sqrt(pow(p1.x-p2.x, 2)+pow(p1.y-p2.y, 2));
		CCPoint midPoint = CCPoint( (p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5);

		setPosition( midPoint );
		setRotation( -CC_RADIANS_TO_DEGREES(atan2(p2.y-p1.y, p2.x-p1.x))+ 90);
		m_springSprite->setScaleY(distance/(MAX_LENGHT));

		m_firstBodySprite->setPosition(convertToNodeSpace(p1));
		m_secondBodySprite->setPosition(convertToNodeSpace(p2));
	}
}

void ObjectSpring::restoreToOriginalProperties(){
    GameObject::restoreToOriginalProperties();
	m_firstBody->SetLinearVelocity(b2Vec2(0, 0));
	m_firstBody->SetAngularVelocity(0);
	m_firstBody->SetTransform(m_firstBodyOriginalLocation, m_firstBodyOriginalRotation);
	
	m_secondBody->SetLinearVelocity(b2Vec2(0, 0));
	m_secondBody->SetAngularVelocity(0);
	m_secondBody->SetTransform(m_secondBodyOriginalLocation, m_secondBodyOriginalRotation);
}

void ObjectSpring::saveOriginalProperties(){
	GameObject::saveOriginalProperties();
	m_firstBodyOriginalLocation = m_firstBody->GetPosition();
	m_firstBodyOriginalRotation = m_firstBody->GetAngle();
	m_secondBodyOriginalLocation = m_secondBody->GetPosition();
	m_secondBodyOriginalRotation = m_secondBody->GetAngle();	
}

void ObjectSpring::onSimulationStarted(){
	saveOriginalProperties();
	m_firstBody->SetAwake(true);
	m_secondBody->SetAwake(true);
	m_firstBody->SetType(b2_dynamicBody);
	m_secondBody->SetType(b2_dynamicBody);  	
	m_firstBody->SetFixedRotation(false);
	m_secondBody->SetFixedRotation(false);	
	for (unsigned int i = 0; i < m_joints.size(); i++) {
		m_joints.at(i)->SetLength(MAX_LENGHT / PTM_RATIO);
	}	
}

void ObjectSpring::onSimulationEnded(){
	m_firstBody->SetType(b2_staticBody);
	m_secondBody->SetType(b2_staticBody);	
	for (unsigned int i = 0; i < m_joints.size(); i++) {
		m_joints.at(i)->SetLength(MIN_LENGHT / PTM_RATIO);
	}	
	restoreToOriginalProperties();
	
}

void ObjectSpring::onMovementStarted(){
	m_firstBody->SetType(b2_staticBody);
	m_secondBody->SetType(b2_staticBody);
	m_firstBody->SetFixedRotation(true);
	m_secondBody->SetFixedRotation(true);
}

void ObjectSpring::onMovementEnded(){
	GameObject::onMovementEnded();
	m_secondBody->SetType(b2_staticBody);
	m_firstBody->SetFixedRotation(false);
	m_secondBody->SetFixedRotation(false);
}

void ObjectSpring::destroy(){
	for (unsigned int i = 0; i < m_joints.size(); i++) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(m_joints.at(i));		
	}
	GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(m_prismaticJoint);			
    GameWorld::sharedGameWorld()->physicsWorld->DestroyBody(m_secondBody);
    GameObject::destroy();
}

//////////////////////////////////////////////////// 
// Moves object to new location, if state is idile
// than it is a simple translation, if moving than
// creates a move joint to move object around
//////////////////////////////////////////////////// 
void ObjectSpring::move( CCPoint newPostion )
{
	if (getParent() && m_firstBody) {
		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2PositionB1 = b2Vec2(newPostion.x/PTM_RATIO,
		                           newPostion.y/PTM_RATIO);
        
		m_firstBody->SetTransform(b2PositionB1, m_firstBody->GetAngle());
		
		b2Vec2 b2PositionB2 = b2PositionB1 + m_offsetBetweenBodies;
		m_secondBody->SetTransform(b2PositionB2, m_secondBody->GetAngle());		
	}
}

//////////////////////////////////////////////////// 
// Rotates object to give angle, if state is idile
// than it is a simple translation, if rotating than
// creates a rotate joint to rotate object along 
// it's axis
//////////////////////////////////////////////////// 
void ObjectSpring::rotate( float newRotation )
{
    if (getParent() && m_firstBody) {
		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2Position = b2Vec2(getPosition().x/PTM_RATIO,
		                           getPosition().y/PTM_RATIO);
		float32 b2Angle =  -1 * CC_DEGREES_TO_RADIANS(newRotation);
		
		m_firstBody->SetTransform(b2Position, b2Angle);
        
	}
}

void ObjectSpring::startUnstuckPhase( )
{
	m_firstBody->SetType(b2_dynamicBody);
	m_firstBody->SetFixedRotation(true);
	m_firstBody->SetGravityScale(0);
	
	m_secondBody->SetType(b2_dynamicBody);
	m_secondBody->SetFixedRotation(true);
	m_secondBody->SetGravityScale(0);
}


void ObjectSpring::unstuckPhaseFinished( )
{
    if (getParent() && m_firstBody) {
		m_firstBody->SetGravityScale(1);
		m_firstBody->SetType(b2_staticBody);
		m_firstBody->SetFixedRotation(true);
		
		
		m_secondBody->SetGravityScale(1);	
		m_secondBody->SetType(b2_staticBody);
		m_secondBody->SetFixedRotation(true);	
		GameLevelScene::sharedGameScene()->setUtilityButtonsVisibleFoSelectedObject(true);
	}
}
