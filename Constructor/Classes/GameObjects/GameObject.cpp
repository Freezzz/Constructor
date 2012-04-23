//
//  GameObject.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "GameObject.h"
#include <GameWorld.h>
#include "ObjectPin.h"
#include "ObjectGlue.h"
#include <iostream>
#define PTM_RATIO 32.0f
////////////////////////////////////////////////////
// GameObject init
////////////////////////////////////////////////////
GameObject::GameObject( )
: m_id(0), m_mutable(true), isStatic(0), isMovable(1), isRotatable(1), isDeletable(1)
{
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
    }if (m_state == Idile && newState == Rotating) {
		onRotationStarted();
    }else if (m_state == Rotating && newState == Idile) {
		onRotationEnded();
    }
	
	m_state = newState;
    
}

//////////////////////////////////////////////////// 
// Callback when simulation just started
//////////////////////////////////////////////////// 
void GameObject::onSimulationStarted(){
	saveOriginalProperties();
	m_objectBody->SetAwake(true);
	if (!isStatic) {
		m_objectBody->SetType(b2_dynamicBody);
	}
}

//////////////////////////////////////////////////// 
// Callback when simulation just ended
//////////////////////////////////////////////////// 
void GameObject::onSimulationEnded(){
	m_objectBody->SetType(b2_staticBody);	
	restoreToOriginalProperties();
}

//////////////////////////////////////////////////// 
// Callback when movement just started
//////////////////////////////////////////////////// 
void GameObject::onMovementStarted(){
	// Movement started set as dynamic so it can interact with other objects
	m_objectBody->SetType(b2_dynamicBody);
	// Disable rotation
	m_objectBody->SetFixedRotation(true);  
}

//////////////////////////////////////////////////// 
// Callback when movement just ended
//////////////////////////////////////////////////// 
void GameObject::onMovementEnded(){
	// Destory helper objects	
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
// Callback when rotation just started
//////////////////////////////////////////////////// 
void GameObject::onRotationStarted(){
	m_objectBody->SetFixedRotation(false);	
	m_objectBody->SetType(b2_dynamicBody);
}

//////////////////////////////////////////////////// 
// Callback when rotation just ended
//////////////////////////////////////////////////// 
void GameObject::onRotationEnded(){
	// Destory helper objects
	if (m_objectBodyPin) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(m_objectBodyPin);
		m_objectBodyPin = NULL;		
	}
	
	if (m_rotationJoin) {
		GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(m_rotationJoin);
		m_rotationJoin = NULL;					
	}
	
	// Enable rotation
	m_objectBody->SetFixedRotation(true);
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
		float32 b2Angle =  -1 * CC_DEGREES_TO_RADIANS(newRotation);
        
        setRotation(newRotation);        
		m_objectBody->SetTransform(b2Position, b2Angle);
        
	}
}

//////////////////////////////////////////////////// 
// Rotates object to give angle creates a rotate 
//	joint to rotate object along it's axis
//////////////////////////////////////////////////// 
void GameObject::rotate(CCPoint location){
	if (getParent() && m_objectBody) {
		b2Vec2 b2Position = b2Vec2(location.x/PTM_RATIO,
		                           location.y/PTM_RATIO);
		
        // Create joint to tap location
        if (!m_rotationJoin) {
            b2MouseJointDef md;
            md.bodyA = GameWorld::sharedGameWorld()->umbelicoDelMondo; // useless but it is a convention
            md.bodyB = m_objectBody;
			md.maxForce = 2000;
            md.frequencyHz = 20;
            md.dampingRatio = 1;
			md.collideConnected = false;
            m_rotationJoin = (b2MouseJoint *)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&md);						
        }
		
		// Pin object to it's position in the world alowing rotation
		if(!m_objectBodyPin){
			b2RevoluteJointDef md;
			md.Initialize(m_objectBody, GameWorld::sharedGameWorld()->umbelicoDelMondo, m_objectBody->GetPosition());
			md.referenceAngle = m_objectBody->GetAngle();
			md.collideConnected = false;
            m_objectBodyPin = (b2RevoluteJoint *)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&md);
		}
		
        m_rotationJoin->SetTarget(b2Position);
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
	b2JointEdge * jnt = m_objectBody->GetJointList();
	while (jnt) {
		ObjectPin * pin = dynamic_cast<ObjectPin*>((GameObject*)jnt->joint->GetUserData());
		if (pin) {
			// We dont need to destroy joint manualy it will be destroyed with this body
			pin->unPin(false);
		}else {
			ObjectGlue * glue = dynamic_cast<ObjectGlue*>((GameObject*)jnt->joint->GetUserData());
			if (glue) {
				// We dont need to destroy joint manualy it will be destroyed with this body
				glue->unGlueJoint((b2WeldJoint*)jnt->joint);
			}
		}
		jnt = jnt->next;
	}
	
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
	move(m_originalPosition);
	rotate(m_originalRotation);
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
