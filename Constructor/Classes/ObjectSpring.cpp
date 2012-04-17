//
//  ObjectSpring.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectSpring.h"
#include "GameWorld.h"
#include "ObjectSimpleBox.h"
#define PTM_RATIO 32.0f
#define MAX_LENGHT 30

//////////////////////////////////////////////////// 
// ObjectSpring init
//////////////////////////////////////////////////// 
bool ObjectSpring::init(){
    this->objectSprite = CCSprite::spriteWithFile("spring.png");
    this->objectSprite->setScaleY(0.3);

	// Adapt container to the graphical rapresentation
	this->setContentSize(objectSprite->getContentSize());
	this->objectSprite->setAnchorPoint(CCPoint(0,0));	
	this->setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0

    this->addChild(objectSprite);
	
    this->isStatic = false;
    this->scheduleUpdate();
	joints = new vector<b2DistanceJoint*>();
	ribs = new vector<b2Body*>();
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
	dynamicBox.SetAsBox(objectSprite->getContentSize().width/2/PTM_RATIO, 10/2/PTM_RATIO);
	
	// Rib body def
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.3f;
	fixtureDef.isSensor = false;
	
	objectBody = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);
	objectBody->CreateFixture(&fixtureDef);
	objectBody->SetUserData(this);
    objectBody->SetFixedRotation(true);

	b2Body * objectBody1 = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);
	objectBody1->CreateFixture(&fixtureDef);
    objectBody1->SetFixedRotation(true);
	ribs->push_back(objectBody1);
	
	b2Body * objectBody2 = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);
	objectBody2->CreateFixture(&fixtureDef);
    objectBody2->SetFixedRotation(true);	
	ribs->push_back(objectBody2);	
    
	secondBody = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);
	secondBody->CreateFixture(&fixtureDef);
	secondBody->SetUserData(this);
    secondBody->SetFixedRotation(true);
    
    b2DistanceJointDef jointDef1;
    jointDef1.bodyA = objectBody;
    jointDef1.bodyB = objectBody1;
    jointDef1.length = 10 / PTM_RATIO;
    jointDef1.frequencyHz = 4;
	jointDef1.dampingRatio = 0.15;
    jointDef1.collideConnected = true;

	b2DistanceJointDef jointDef2;
    jointDef2.bodyA = objectBody1;
    jointDef2.bodyB = objectBody2;
    jointDef2.length = 10 / PTM_RATIO;
    jointDef2.frequencyHz = 4;
	jointDef2.dampingRatio = 0.15;
    jointDef2.collideConnected = true;

	b2DistanceJointDef jointDef3;
    jointDef3.bodyA = objectBody2;
    jointDef3.bodyB = secondBody;
    jointDef3.length = 10 / PTM_RATIO;
    jointDef3.frequencyHz = 4;
	jointDef3.dampingRatio = 0.15;
    jointDef3.collideConnected = true;

	joints->push_back((b2DistanceJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef1));
	joints->push_back((b2DistanceJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef2));
	joints->push_back((b2DistanceJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef3));
}


void ObjectSpring::update(ccTime dt){
    if (this->getParent() && objectBody) {
        // Update posisiotn of node moving it to body postion
        CCPoint p1 = CCPoint(objectBody->GetPosition().x * PTM_RATIO, objectBody->GetPosition().y * PTM_RATIO);
        CCPoint p2 = CCPoint(secondBody->GetPosition().x * PTM_RATIO, secondBody->GetPosition().y * PTM_RATIO);        

        float distance = sqrt(pow(p1.x-p2.x, 2)+pow(p1.y-p2.y, 2));
        CCPoint midPoint = CCPoint( (p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5);
        
        this->setPosition( midPoint );
        this->setRotation( -CC_RADIANS_TO_DEGREES(atan2(p2.y-p1.y, p2.x-p1.x))+ 90);
        this->objectSprite->setScaleY(distance*1.3/MAX_LENGHT);
    }
}

void ObjectSpring::restoreToOriginalProperties(){
    GameObject::restoreToOriginalProperties();
    this->secondBody->SetLinearVelocity(b2Vec2(0, 0));
    this->secondBody->SetAngularVelocity(0);
	this->secondBody->SetTransform(_secondBodyOriginalLocation, _secondBodyOriginalRotation);
}

void ObjectSpring::saveOriginalProperties(){
	GameObject::saveOriginalProperties();
	_secondBodyOriginalRotation = secondBody->GetAngle();
	_secondBodyOriginalLocation = secondBody->GetPosition();
}

void ObjectSpring::onSimulationStarted(){
	this->saveOriginalProperties();
	for (int i = 0; i < joints->size(); i++) {
		joints->at(i)->SetLength(MAX_LENGHT / PTM_RATIO);
	}
	objectBody->SetAwake(true);
	secondBody->SetAwake(true);
	objectBody->SetType(b2_dynamicBody);
	secondBody->SetType(b2_dynamicBody);
}

void ObjectSpring::onSimulationEnded(){
	for (int i = 0; i < joints->size(); i++) {
		joints->at(i)->SetLength(10 / PTM_RATIO);
	}
	objectBody->SetType(b2_staticBody);
	secondBody->SetType(b2_staticBody);	
	this->restoreToOriginalProperties();
}

void ObjectSpring::onMovementStarted(){
	objectBody->SetType(b2_dynamicBody);
	secondBody->SetType(b2_dynamicBody);
	objectBody->SetFixedRotation(true);  
	secondBody->SetFixedRotation(true);  
}

void ObjectSpring::onMovementEnded(){
	GameObject::onMovementEnded();
	secondBody->SetType(b2_staticBody);
	secondBody->SetFixedRotation(false);	
}

void ObjectSpring::destroy(){
	for (int i = 0; i < joints->size(); i++) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(joints->at(i));		
	}
	for (int i = 0; i < ribs->size(); i++) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyBody(ribs->at(i));		
	}    
    GameWorld::sharedGameWorld()->physicsWorld->DestroyBody(secondBody);
    GameObject::destroy();
}
