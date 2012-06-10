//
//  ObjectGlue.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/19/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectGlue.h"
#include "../GameWorld.h"

//////////////////////////////////////////////////// 
// ObjectGlue init
//////////////////////////////////////////////////// 
bool ObjectGlue::init( )
{
	m_sprite = CCSprite::spriteWithFile( m_prototype["sprite path"].asCString() );
    
	// Adapt container to the graphical rapresentation
	setContentSize(m_sprite->getContentSize());
	m_sprite->setAnchorPoint(CCPoint(0,0));
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0
	
	addChild(m_sprite);
	
	isStatic = false;
	
	isMovable = true;
	isRotatable = false;
	isDeletable = true;
	
	rotateButtonOffset = CCPoint(0,-30);
	deleteButtonOffset = CCPoint(-30,0);
	
	// Permits to pin to be always over other objects
	defaultZOrder = 20;
	
	m_isGlued = false;
	
	scheduleUpdate();	
	return true;
}

//////////////////////////////////////////////////// 
// Creates a dummy sensor object to check collisions
// with other objects
//////////////////////////////////////////////////// 
bool ObjectGlue::createBodyAtPosition( cocos2d::CCPoint position )
{
	b2dJson json;
	m_body = json.j2b2Body( physicsWorld(), prototype() );
	if( ! m_body || ! m_body->GetFixtureList() ) {
		std::cout << "Glue inventory item prototype messed up" << std::endl;
		return false;
	}
	m_body->SetUserData(this);
	m_body->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_body->GetAngle() );
	setPosition(position);

	m_bodies.push_back( m_body );

	return true;
}

void ObjectGlue::onSimulationStarted( )
{
	saveOriginalProperties();
	m_body->SetAwake(true);
	
	if (!m_isGlued) {
		m_body->SetType(b2_staticBody);
	}else {
		m_body->SetType(b2_dynamicBody);
	}
	
	// Hide glue if it is no glued
	m_sprite->setIsVisible(m_isGlued);
}

void ObjectGlue::onSimulationEnded( )
{
	m_sprite->setIsVisible(true);
	m_body->SetType(b2_staticBody);
	restoreToOriginalProperties();
}

void ObjectGlue::onMovementStarted( )
{
	unPin( true );

	// Sensor objects like pin/grue need collision detection to pin them
	m_body->SetType( b2_dynamicBody );
	// Setting gravity scale to 0 avoid them to fall down while user drags them
	m_body->SetGravityScale( 0 );

	m_body->SetFixedRotation(true); // disable rotation
}

void ObjectGlue::onMovementEnded( )
{
	// Check if our sensor object collides with other GameObjects
	b2Contact * cont = GameWorld::sharedGameWorld()->physicsWorld->GetContactList();
	while (cont != NULL) {
		
		// If contanc between two gameObjects. TODO: Improve check
		if (cont->IsTouching() && (cont->GetFixtureA()->GetBody() == m_body || cont->GetFixtureB()->GetBody() == m_body)) {
			weldObjectsAtContact(cont);
		}
		cont = cont->GetNext();
	}
	GameObject::onMovementEnded();	
}



//////////////////////////////////////////////////// 
// Unglue this glue from bodys
//////////////////////////////////////////////////// 
void ObjectGlue::unPin(bool destroyJoint){
	if (destroyJoint) {
		for (unsigned int i = 0; i < m_joints.size(); i++) {
			GameWorld::sharedGameWorld()->physicsWorld->DestroyJoint(m_joints.at(i));		
		}	
		m_joints.clear();
	}
	m_isGlued = false;
}

void ObjectGlue::unGlueJoint(b2WeldJoint *jnt){
	for (vector<b2WeldJoint*>::iterator i = m_joints.begin(); i != m_joints.end(); i++)
    {
		if (jnt == *i) {
			m_joints.erase(i);
			if (m_joints.size() == 0) {
				m_isGlued = false;
			}
			return;
		}
    }
	
}

void ObjectGlue::weldObjectsAtContact( b2Contact *contact )
{
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	b2Vec2 pnt1 = worldManifold.points[0];
	b2Vec2 pnt2 = worldManifold.points[1];
	b2Vec2 midPoint = b2Vec2((pnt1.x + pnt2.x) * 0.5, (pnt1.y + pnt2.y) * 0.5);

	CCLog("MIDPOINT: %f, %f", midPoint.x, midPoint.y);
	b2Body * bodyA = contact->GetFixtureA()->GetBody();
	b2Body * bodyB = contact->GetFixtureB()->GetBody();

	b2WeldJointDef jointDef1;
	jointDef1.bodyA = bodyA;
	jointDef1.bodyB = bodyB;
	jointDef1.localAnchorA = bodyA->GetLocalPoint(bodyA->GetPosition());
	jointDef1.localAnchorB = bodyB->GetLocalPoint(bodyA->GetPosition());

	jointDef1.userData = this;
	jointDef1.collideConnected = false;

	m_joints.push_back((b2WeldJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef1));
	m_isGlued = true;

	CCLog("Objects welded!");
}

void ObjectGlue::startUnstuckPhase(){
	return;
}


void ObjectGlue::unstuckPhaseFinished(){
	return;
}

