//
//  GameObject.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "GameObject.h"
#include <GameWorld.h>

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
	if (m_state == Idile && newState == Moving) {
		onMovementStarted();
    }else if (m_state == Moving && newState == Idile) {
		onMovementEnded();
    }else if (m_state == Idile && newState == Simulating) {
		onSimulationStarted();
    }else if (m_state == Simulating && newState == Idile) {
		onSimulationEnded();
    } 
	m_state = newState;
    
}

void GameObject::onSimulationStarted(){
	saveOriginalProperties();
	m_objectBody->SetAwake(true);
	if (!isStatic) {
		m_objectBody->SetType(b2_dynamicBody);
	}
}

void GameObject::onSimulationEnded(){
	m_objectBody->SetType(b2_staticBody);	
	restoreToOriginalProperties();
}

void GameObject::onMovementStarted(){
	// Movement started set as dynamic so it can interact with other objects
	m_objectBody->SetType(b2_dynamicBody);
	// Disable rotation
	m_objectBody->SetFixedRotation(true);  
}

void GameObject::onMovementEnded(){
	// Movement ended
	if (m_moveJoint) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(m_moveJoint);
		m_moveJoint = NULL;
	}
	// Enable rotation
	m_objectBody->SetFixedRotation(false);
	// Set static to avoid further movements        
	m_objectBody->SetType(b2_staticBody);	
}

//////////////////////////////////////////////////// 
// Moves object to new location, if state is idile
// than it is a simple translation, if moving than
// creates a move joint to move object around
//////////////////////////////////////////////////// 
void GameObject::move(CCPoint newPostion){
	if (getParent() && m_objectBody) {
		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2Position = b2Vec2(newPostion.x/PTM_RATIO,
		                           newPostion.y/PTM_RATIO);
        
        // Case when it instant translation
        if(m_state != Moving){
            float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS(getRotation());
            setPosition(newPostion);        
            m_objectBody->SetTransform(b2Position, b2Angle);
            return;
        }
		
        // Case when dragging around
        if (!m_moveJoint) {
            b2MouseJointDef md;
            md.bodyA = GameWorld::sharedGameWorld()->umbelicoDelMondo; // useless but it is a convention
            md.bodyB = m_objectBody;
            md.target = b2Position;
            md.maxForce = 2000;
            md.frequencyHz = 20;
            md.dampingRatio = 1;
            m_moveJoint = (b2MouseJoint *)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&md);
        }
        m_moveJoint->SetTarget(b2Position);
	}
}

//////////////////////////////////////////////////// 
// Rotates object to give angle, if state is idile
// than it is a simple translation, if rotating than
// creates a rotate joint to rotate object along 
// it's axis
//////////////////////////////////////////////////// 
void GameObject::rotate(float newRotation){
    if (getParent() && m_objectBody) {
		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2Position = b2Vec2(getPosition().x/PTM_RATIO,
		                           getPosition().y/PTM_RATIO);
		float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS(newRotation);
        
        setRotation(newRotation);        
		m_objectBody->SetTransform(b2Position, b2Angle);
        
	}
}

//////////////////////////////////////////////////// 
// Basic object update loop, moves sprite to body location
//////////////////////////////////////////////////// 
void GameObject::update(ccTime dt){
    if (getParent() && m_objectBody) {
        // Update posisiotn of node moving it to body postion
        setPosition( CCPointMake( m_objectBody->GetPosition().x * PTM_RATIO, m_objectBody->GetPosition().y * PTM_RATIO) );
        setRotation( -1 * CC_RADIANS_TO_DEGREES(m_objectBody->GetAngle()) );
    }
}

//////////////////////////////////////////////////// 
// Destroy object and it's physical body
//////////////////////////////////////////////////// 
void GameObject::destroy(){
    GameWorld::sharedGameWorld()->physicsWorld->DestroyBody(m_objectBody);	
	removeFromParentAndCleanup(true);
}

//////////////////////////////////////////////////// 
// Save object's properties pre-simulation
//////////////////////////////////////////////////// 
void GameObject::saveOriginalProperties(){
	m_originalPosition = getPosition();
	m_originalRotation = getRotation();
}

//////////////////////////////////////////////////// 
// Restore object's properties that where set
// before simulation
//////////////////////////////////////////////////// 
void GameObject::restoreToOriginalProperties(){
	move(this->m_originalPosition);
	rotate(this->m_originalRotation);
	m_objectBody->SetLinearVelocity(b2Vec2(0, 0));
	m_objectBody->SetAngularVelocity(0);
}

//////////////////////////////////////////////////// 
// Grapical rapresentation of selected state
//////////////////////////////////////////////////// 
void GameObject::setSelected(bool selected){
	if (!m_objectSprite) {
		return;
	}
    if (selected) {
        m_objectSprite->setColor(ccc3(255, 255, 0));
    }else {
        m_objectSprite->setColor(ccc3(255, 255, 255));
    }
}
