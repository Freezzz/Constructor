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
#include "GameLevelScene.h"
#include "Controls/InventoryLayer.h"
#include <iostream>
////////////////////////////////////////////////////
// GameObject init
////////////////////////////////////////////////////
GameObject::GameObject( const std::string &fileName, const Json::Value &prototype )
: m_fileName(fileName), m_prototype(prototype), m_inventoryItem(0), m_state(Idile), isStatic(0), isMovable(1), isRotatable(1), isDeletable(1)
{
	// m_inventoryItem->m_quantity already increased by node()...
}
GameObject::~GameObject( )
{
	if( m_inventoryItem ) {
		m_inventoryItem->m_quantity --;
		m_inventoryItem->updateQuantityLabel( );
	}
}

//////////////////////////////////////////////////// 
// Sets object state
//////////////////////////////////////////////////// 
void GameObject::setObjectState( ObjectState newState )
{
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
void GameObject::onSimulationStarted( )
{
	saveOriginalProperties();
	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;
		body->SetAwake( true );
		body->SetFixedRotation( false );
		if( ! isStatic ) {
			body->SetType( b2_dynamicBody );
		}
	}
}

//////////////////////////////////////////////////// 
// Callback when simulation just ended
//////////////////////////////////////////////////// 
void GameObject::onSimulationEnded( )
{
	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;
		body->SetType( b2_staticBody );
		body->SetFixedRotation( true );
	}
	restoreToOriginalProperties();
}

//////////////////////////////////////////////////// 
// Callback when movement just started
//////////////////////////////////////////////////// 
void GameObject::onMovementStarted( )
{
	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;
		body->SetType( b2_staticBody ); // making it static to avoid movements
		body->SetFixedRotation( true ); // disable rotation
	}
}

//////////////////////////////////////////////////// 
// Callback when movement just ended
//////////////////////////////////////////////////// 
void GameObject::onMovementEnded( )
{
	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;
		body->SetFixedRotation( false ); // enable rotation
		body->SetGravityScale( 1 ); // resume taking in account the gravity
		body->SetType( b2_staticBody ); // set static to avoid further movements
	}
}

//////////////////////////////////////////////////// 
// Callback when rotation just started
//////////////////////////////////////////////////// 
void GameObject::onRotationStarted( )
{
	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;
		body->SetFixedRotation( false );
		body->SetType( b2_staticBody );
	}
}

//////////////////////////////////////////////////// 
// Callback when rotation just ended
//////////////////////////////////////////////////// 
void GameObject::onRotationEnded( )
{
	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;
		body->SetFixedRotation( true ); // enable rotation
		body->SetType( b2_staticBody ); // set static to avoid further movements
	}
}

//////////////////////////////////////////////////// 
// Moves object to new location, if state is idile
// than it is a simple translation, if moving than
// creates a move joint to move object around
//////////////////////////////////////////////////// 
void GameObject::move( CCPoint newPostion )
{
	if( ! getParent() ) {
		// not moving if this object has no parent
		return ;
	}

	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;

		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2Position = b2Vec2(newPostion.x/PTM_RATIO,
		                           newPostion.y/PTM_RATIO);
        
		float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS( getRotation() );
		setPosition( newPostion );
		body->SetTransform( b2Position, b2Angle );
	}
}

//////////////////////////////////////////////////// 
// Rotates object to give angle, if state is idile
// than it is a simple translation, if rotating than
// creates a rotate joint to rotate object along 
// it's axis
//////////////////////////////////////////////////// 
void GameObject::rotate( float newRotation )
{
	if( ! getParent() ) {
		// not rotating if this object has no parent
		return ;
	}

	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;

		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2Position = b2Vec2(getPosition().x/PTM_RATIO,
		                           getPosition().y/PTM_RATIO);
		float32 b2Angle =  -1 * CC_DEGREES_TO_RADIANS( newRotation );
               
		body->SetTransform( b2Position, b2Angle );
	}
}

//////////////////////////////////////////////////// 
// Basic object update loop, moves sprite to body location
//////////////////////////////////////////////////// 
void GameObject::update( ccTime dt )
{
	if( ! getParent() ) {
		// doing nothing if this object has no parent
		return ;
	}

	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;

		// Update posisiotn of node moving it to body postion
		setPosition( CCPointMake( body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO ) );
		setRotation( -1 * CC_RADIANS_TO_DEGREES( body->GetAngle() ) );
	}
}

//////////////////////////////////////////////////// 
// Destroy object and it's physical body
//////////////////////////////////////////////////// 
void GameObject::destroy( )
{
	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;

		// TODO: use polymorphism...
		b2JointEdge * jnt = body->GetJointList();
		while( jnt ) {
			// if it's a pin, unpinning
			ObjectPin * pin = dynamic_cast<ObjectPin*>((GameObject*)jnt->joint->GetUserData());
			if( pin ) {
				// We dont need to destroy joint manualy it will be destroyed with this body
				pin->unPin(false);
			}

			// if it's glue, ungluing
			ObjectGlue * glue = dynamic_cast<ObjectGlue*>((GameObject*)jnt->joint->GetUserData());
			if (glue) {
				// We dont need to destroy joint manualy it will be destroyed with this body
				glue->unGlueJoint((b2WeldJoint*)jnt->joint);
			}

			jnt = jnt->next;
		}

		GameWorld::sharedGameWorld()->physicsWorld->DestroyBody( body );
	}

	removeFromParentAndCleanup(true);
}

//////////////////////////////////////////////////// 
// Save object's properties pre-simulation
//////////////////////////////////////////////////// 
void GameObject::saveOriginalProperties( )
{
	m_originalPosition = getPosition();
	m_originalRotation = getRotation();
}

//////////////////////////////////////////////////// 
// Restore object's properties that where set
// before simulation
//////////////////////////////////////////////////// 
void GameObject::restoreToOriginalProperties( )
{
	move( m_originalPosition );
	rotate( m_originalRotation );

	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;

		body->SetLinearVelocity(b2Vec2(0, 0));
		body->SetAngularVelocity(0);
	}
}

//////////////////////////////////////////////////// 
// Checks if the point is contained inside this node
//////////////////////////////////////////////////// 
bool GameObject::containsPoint( cocos2d::CCPoint location )
{
	location = convertToNodeSpace(location);
	CCRect rect = CCRect(0,0, getContentSize().width, getContentSize().height);
	return CCRect::CCRectContainsPoint(rect, location);
}

//////////////////////////////////////////////////// 
// Begins object preparation for unstuck routine
//////////////////////////////////////////////////// 
void GameObject::startUnstuckPhase( )
{
	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;

		body->SetType(b2_dynamicBody);
		body->SetFixedRotation(false);
		body->SetGravityScale(0);
	}
}

//////////////////////////////////////////////////// 
// Function to be called after unstuck routine is finished
//////////////////////////////////////////////////// 
void GameObject::unstuckPhaseFinished( )
{
	if( ! getParent() ) {
		// doing nothing if this object has no parent
		return ;
	}

	for( std::vector<b2Body*>::iterator bit = m_bodies.begin(); bit != m_bodies.end(); ++bit ) {
		b2Body *body = *bit;

		body->SetGravityScale(1);
		body->SetType(b2_staticBody);
		body->SetFixedRotation(true);
		GameLevelScene::sharedGameScene()->setUtilityButtonsVisibleFoSelectedObject(true);
	}
	CCLog("Unstuck finished");
}

//////////////////////////////////////////////////// 
// Object collided notification
// @pararm: other Object of the collision
//////////////////////////////////////////////////// 
void GameObject::objectCollided(GameObject *otherObject, b2Contact * contact){
//	CCLog("Override me");
}


//////////////////////////////////////////////////// 
// Object collided notification
// @pararm: other Object of the collision
//////////////////////////////////////////////////// 
void GameObject::objectCollisionEnded(GameObject *otherObject, b2Contact * contact){
	//	CCLog("Override me");
}



//////////////////////////////////////////////////// 
// Grapical rapresentation of selected state
//////////////////////////////////////////////////// 
void GameObject::setSelected( bool selected )
{
	// TODO: use polymorphism or share sprites
	/*
	if( selected ) {
		m_objectSprite->setColor(ccc3(255, 255, 0));
	}
	else {
		m_objectSprite->setColor(ccc3(255, 255, 255));
	}
	*/
}
