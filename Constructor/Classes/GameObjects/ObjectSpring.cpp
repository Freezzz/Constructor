//
//  ObjectSpring.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectSpring.h"
#include "ObjectSimpleBox.h"
#include "../GameWorld.h"
#define PTM_RATIO 32.0f
#define MAX_LENGHT 132

INVENTORYITEM_GAMEOBJECT_NODE_DECL( SpringInventoryItem, ObjectSpring )

//////////////////////////////////////////////////// 
// ObjectSpring init
//////////////////////////////////////////////////// 
bool ObjectSpring::init(){
	m_objectSprite = CCSprite::spriteWithFile("spring.png");
	m_objectSprite->setScaleY(0.3);

	// Adapt container to the graphical rapresentation
	setContentSize(m_objectSprite->getContentSize());
	m_objectSprite->setAnchorPoint(CCPoint(0,0.5));	
	setAnchorPoint(CCPoint(0.5,0)); // CCNode AP default is 0,0

	addChild(m_objectSprite);
	
	isStatic = false;
	isMovable = true;
	isRotatable = false;
	isDeletable = true;
	
	moveButtonOffset = CCPoint(0, 10);
	rotateButtonOffset = CCPoint(0,-30);
	deleteButtonOffset = CCPoint(-30,0);

	scheduleUpdate();
	m_joints = vector<b2DistanceJoint*>();
	m_ribs = vector<b2Body*>();
	return true;
}

//////////////////////////////////////////////////// 
// Create two bodyes and a joint to represent a spring
//////////////////////////////////////////////////// 
void ObjectSpring::createBodyAtPosition(cocos2d::CCPoint position){
	// Player physical body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	
	// Rib shape
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_objectSprite->getContentSize().width/2/PTM_RATIO, 10/2/PTM_RATIO);
	
	// Rib body def
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.3f;
	
	m_objectBody = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);
	m_objectBody->CreateFixture(&fixtureDef);
	m_objectBody->SetUserData(this);
    m_objectBody->SetFixedRotation(true);
		
    
	m_secondBody = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);
	m_secondBody->CreateFixture(&fixtureDef);
	m_secondBody->SetUserData(this);
    m_secondBody->SetFixedRotation(true);
    
    b2DistanceJointDef jointDef1;
    jointDef1.bodyA = m_objectBody;
    jointDef1.bodyB = m_secondBody;
	jointDef1.localAnchorA.Set(1, -1);
	jointDef1.localAnchorB.Set(1, 0);	
    jointDef1.length = 5 / PTM_RATIO;
    jointDef1.frequencyHz = 20;
	jointDef1.dampingRatio = 1;
    jointDef1.collideConnected = true;

	b2DistanceJointDef jointDef2;
    jointDef2.bodyA = m_objectBody;
    jointDef2.bodyB = m_secondBody;
	jointDef2.localAnchorA.Set(0, -1);
	jointDef2.localAnchorB.Set(0, 0);	
    jointDef2.length = 5 / PTM_RATIO;
    jointDef2.frequencyHz = 20;
	jointDef2.dampingRatio = 1;
    jointDef2.collideConnected = true;

	b2DistanceJointDef jointDef3;
    jointDef3.bodyA = m_objectBody;
    jointDef3.bodyB = m_secondBody;
	jointDef3.localAnchorA.Set(-1, -1);
	jointDef3.localAnchorB.Set(-1, 0);	
    jointDef3.length = 5 / PTM_RATIO;
    jointDef3.frequencyHz = 20;
	jointDef3.dampingRatio = 1;
    jointDef3.collideConnected = true;

	m_joints.push_back((b2DistanceJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef1));
	m_joints.push_back((b2DistanceJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef2));
	m_joints.push_back((b2DistanceJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef3));	
	setPosition(position);	
}


void ObjectSpring::update(ccTime dt){
    if (getParent() && m_objectBody) {
        // Update posisiotn of node moving it to body postion
        CCPoint p1 = CCPoint(m_objectBody->GetPosition().x * PTM_RATIO, m_objectBody->GetPosition().y * PTM_RATIO);
        CCPoint p2 = CCPoint(m_secondBody->GetPosition().x * PTM_RATIO, m_secondBody->GetPosition().y * PTM_RATIO);        

        float distance = sqrt(pow(p1.x-p2.x, 2)+pow(p1.y-p2.y, 2));
        CCPoint midPoint = CCPoint( (p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5);

        setPosition( midPoint );
        setRotation( -CC_RADIANS_TO_DEGREES(atan2(p2.y-p1.y, p2.x-p1.x))+ 90);
        m_objectSprite->setScaleY(distance/(MAX_LENGHT));
    }
}

void ObjectSpring::restoreToOriginalProperties(){
    GameObject::restoreToOriginalProperties();
	m_secondBody->SetLinearVelocity(b2Vec2(0, 0));
	m_secondBody->SetAngularVelocity(0);
	m_secondBody->SetTransform(m_secondBodyOriginalLocation, m_secondBodyOriginalRotation);
}

void ObjectSpring::saveOriginalProperties(){
	GameObject::saveOriginalProperties();
	m_secondBodyOriginalRotation = m_secondBody->GetAngle();
	m_secondBodyOriginalLocation = m_secondBody->GetPosition();
}

void ObjectSpring::onSimulationStarted(){
	saveOriginalProperties();
	m_objectBody->SetAwake(true);
	m_secondBody->SetAwake(true);
	m_objectBody->SetType(b2_dynamicBody);
	m_secondBody->SetType(b2_dynamicBody);  	
	for (unsigned int i = 0; i < m_joints.size(); i++) {
		m_joints.at(i)->SetLength(MAX_LENGHT / PTM_RATIO);
	}	
}

void ObjectSpring::onSimulationEnded(){
	for (unsigned int i = 0; i < m_joints.size(); i++) {
		m_joints.at(i)->SetLength(5 / PTM_RATIO);
	}
	m_objectBody->SetType(b2_staticBody);
	m_secondBody->SetType(b2_staticBody);	
	restoreToOriginalProperties();
}

void ObjectSpring::onMovementStarted(){
	m_objectBody->SetType(b2_dynamicBody);
	m_secondBody->SetType(b2_dynamicBody);
	m_objectBody->SetFixedRotation(true);  
	m_secondBody->SetFixedRotation(true);  
}

void ObjectSpring::onMovementEnded(){
	GameObject::onMovementEnded();
	m_secondBody->SetType(b2_staticBody);
	m_objectBody->SetFixedRotation(true);			
	m_secondBody->SetFixedRotation(true);	
}

void ObjectSpring::destroy(){
	for (unsigned int i = 0; i < m_joints.size(); i++) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(m_joints.at(i));		
	}
	for (unsigned int i = 0; i < m_ribs.size(); i++) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyBody(m_ribs.at(i));		
	}    
    GameWorld::sharedGameWorld()->physicsWorld->DestroyBody(m_secondBody);
    GameObject::destroy();
}
