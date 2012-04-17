//
//  GameObject.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "GameObject.h"
#include "GameWorld.h"

#define PTM_RATIO 32.0f
//////////////////////////////////////////////////// 
// GameObject init
//////////////////////////////////////////////////// 
bool GameObject::init(){
	CCLog("GameObject::init -- OVERRIDE ME!");
	return true;
}



//////////////////////////////////////////////////// 
// Sets object state
//////////////////////////////////////////////////// 
void GameObject::setObjectState(ObjectState newState){
	// Movement started
	if (this->state == idile && newState == moving) {
		this->onMovementStarted();
    }else if (this->state == moving && newState == idile) {
		this->onMovementEnded();
    }else if (this->state == idile && newState == simulating) {
		this->onSimulationStarted();
    }else if (this->state == simulating && newState == idile) {
		this->onSimulationEnded();
    } 
    this->state = newState;
    
}

void GameObject::onSimulationStarted(){
	this->saveOriginalProperties();
	this->objectBody->SetAwake(true);
	if (!this->isStatic) {
		this->objectBody->SetType(b2_dynamicBody);
	}
}

void GameObject::onSimulationEnded(){
	this->objectBody->SetType(b2_staticBody);	
	this->restoreToOriginalProperties();
}

void GameObject::onMovementStarted(){
	// Movement started set as dynamic so it can interact with other objects
	this->objectBody->SetType(b2_dynamicBody);
	// Disable rotation
	this->objectBody->SetFixedRotation(true);  
}

void GameObject::onMovementEnded(){
	// Movement ended
	if (moveJoint) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(moveJoint);
		moveJoint = NULL;
	}
	// Enable rotation
	this->objectBody->SetFixedRotation(false);
	// Set static to avoid further movements        
	this->objectBody->SetType(b2_staticBody);	
}

//////////////////////////////////////////////////// 
// Moves object to new location, if state is idile
// than it is a simple translation, if moving than
// creates a move joint to move object around
//////////////////////////////////////////////////// 
void GameObject::move(cocos2d::CCPoint newPostion){
	if (this->getParent() && objectBody) {
		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2Position = b2Vec2(newPostion.x/PTM_RATIO,
		                           newPostion.y/PTM_RATIO);
        
        // Case when it instant translation
        if(this->state != moving){
            float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS(this->getRotation());
            this->setPosition(newPostion);        
            objectBody->SetTransform(b2Position, b2Angle);
            return;
        }
		
        // Case when dragging around
        if (!moveJoint) {
            b2MouseJointDef md;
            md.bodyA = GameWorld::sharedGameWorld()->umbelicoDelMondo; // useless but it is a convention
            md.bodyB = this->objectBody;
            md.target = b2Position;
            md.maxForce = 2000;
            md.frequencyHz = 20;
            md.dampingRatio = 1;
            moveJoint = (b2MouseJoint *)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&md);
        }
        moveJoint->SetTarget(b2Position);
	}
}

//////////////////////////////////////////////////// 
// Rotates object to give angle, if state is idile
// than it is a simple translation, if rotating than
// creates a rotate joint to rotate object along 
// it's axis
//////////////////////////////////////////////////// 
void GameObject::rotate(float newRotation){
    if (this->getParent() && objectBody) {
		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2Position = b2Vec2(this->getPosition().x/PTM_RATIO,
		                           this->getPosition().y/PTM_RATIO);
		float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS(newRotation);
        
        this->setRotation(newRotation);        
		objectBody->SetTransform(b2Position, b2Angle);
        
	}
}

//////////////////////////////////////////////////// 
// Basic object update loop, moves sprite to body location
//////////////////////////////////////////////////// 
void GameObject::update(ccTime dt){
    if (this->getParent() && objectBody) {
        // Update posisiotn of node moving it to body postion
        this->setPosition( CCPointMake( objectBody->GetPosition().x * PTM_RATIO, objectBody->GetPosition().y * PTM_RATIO) );
        this->setRotation( -1 * CC_RADIANS_TO_DEGREES(objectBody->GetAngle()) );
    }
}

//////////////////////////////////////////////////// 
// Destroy object and it's physical body
//////////////////////////////////////////////////// 
void GameObject::destroy(){
    GameWorld::sharedGameWorld()->physicsWorld->DestroyBody(this->objectBody);	
    this->removeFromParentAndCleanup(true);
}

//////////////////////////////////////////////////// 
// Save object's properties pre-simulation
//////////////////////////////////////////////////// 
void GameObject::saveOriginalProperties(){
	this->_originalPosition = this->getPosition();
	this->_originalRotation = this->getRotation();
}

//////////////////////////////////////////////////// 
// Restore object's properties that where set
// before simulation
//////////////////////////////////////////////////// 
void GameObject::restoreToOriginalProperties(){
    this->move(this->_originalPosition);
    this->rotate(this->_originalRotation);
    this->objectBody->SetLinearVelocity(b2Vec2(0, 0));
    this->objectBody->SetAngularVelocity(0);
}

//////////////////////////////////////////////////// 
// Grapical rapresentation of selected state
//////////////////////////////////////////////////// 
void GameObject::setSelected(bool selected){
	if (!this->objectSprite) {
		return;
	}
    if (selected) {
        this->objectSprite->setColor(ccc3(255, 255, 0));
    }else {
        this->objectSprite->setColor(ccc3(255, 255, 255));
    }
}
