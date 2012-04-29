//
//  ObjectGlue.cpp
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/19/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#include "ObjectGlue.h"
#include "../GameWorld.h"

INVENTORYITEM_GAMEOBJECT_NODE_DECL( GlueInventoryItem , ObjectGlue )

//////////////////////////////////////////////////// 
// ObjectGlue init
//////////////////////////////////////////////////// 
bool ObjectGlue::init( std::string spritePath, b2FixtureDef *fixtureDef )
{
	m_objectSprite = CCSprite::spriteWithFile( spritePath.c_str() );

	m_fixtureDef = fixtureDef;
    
	// Adapt container to the graphical rapresentation
	setContentSize(m_objectSprite->getContentSize());
	m_objectSprite->setAnchorPoint(CCPoint(0,0));	
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0
	
	addChild(m_objectSprite);
	
	isStatic = false;
	
	isMovable = true;
	isRotatable = false;
	isDeletable = true;
	
	rotateButtonOffset = CCPoint(0,-30);
	deleteButtonOffset = CCPoint(-30,0);
	
	// Permits to pin to be always over other objects
	defaultZOrder = 20;
	
	m_type = Glue;
	m_isGlued = false;
	
	scheduleUpdate();	
	return true;
}

//////////////////////////////////////////////////// 
// Creates a dummy sensor object to check collisions
// with other objects
//////////////////////////////////////////////////// 
void ObjectGlue::createBodyAtPosition( cocos2d::CCPoint position )
{
	// Player physical body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	m_objectBody = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);

	m_objectBody->CreateFixture(m_fixtureDef);
	m_objectBody->SetUserData(this);

	setPosition(position);
}

void ObjectGlue::onSimulationStarted(){
	saveOriginalProperties();
	m_objectBody->SetAwake(true);
	
	if (!m_isGlued) {
		m_objectBody->SetType(b2_staticBody);		
	}else {
		m_objectBody->SetType(b2_dynamicBody);
	}
	
	// Hide glue if it is no glued
	m_objectSprite->setIsVisible(m_isGlued);
}

void ObjectGlue::onSimulationEnded(){
	m_objectSprite->setIsVisible(true);
	m_objectBody->SetType(b2_staticBody);
	restoreToOriginalProperties();
}

void ObjectGlue::onMovementStarted(){
	unPin(true);
	GameObject::onMovementStarted();
}

void ObjectGlue::onMovementEnded(){

	
	// Check if our sensor object collides with other GameObjects
	b2Contact * cont = GameWorld::sharedGameWorld()->physicsWorld->GetContactList();
	while (cont != NULL) {
		
		// If contanc between two gameObjects. TODO: Improve check
		if (cont->GetFixtureA()->GetBody() == m_objectBody || cont->GetFixtureB()->GetBody() == m_objectBody) {
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

void ObjectGlue::weldObjectsAtContact(b2Contact *contact){
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
	
    jointDef1.frequencyHz = 20;
	jointDef1.dampingRatio = 1;
	jointDef1.userData = this;
    jointDef1.collideConnected = false;
	
	m_joints.push_back((b2WeldJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef1));
	m_isGlued = true;
	
	CCLog("Objects welded!");
}

void ObjectGlue::setBody( b2Body *b )
{
	GameObject::setBody( b );
	
	m_joints.clear();
	m_isGlued = false;
	
	b2JointEdge *joint = b->GetJointList();
	while( joint ) {
		b2WeldJoint* jnt = dynamic_cast<b2WeldJoint*>(joint->joint);
		if (jnt) {
			m_joints.push_back(jnt);
			m_isGlued = true;			
		}
		joint = joint->next;
	}
}

