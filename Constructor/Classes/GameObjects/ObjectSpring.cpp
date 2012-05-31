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

INVENTORYITEM_GAMEOBJECT_NODE_DECL( SpringInventoryItem, ObjectSpring )

//////////////////////////////////////////////////// 
// ObjectSpring init
//////////////////////////////////////////////////// 
bool ObjectSpring::init( std::string spritePath )
{
	m_objectSprite = CCSprite::spriteWithFile( spritePath.c_str() );
	m_objectSprite->setAnchorPoint(CCPoint(0,0.5));		
	m_objectSprite->setScaleY(0.3);
	
	// Adapt container to the graphical rapresentation

	
	setAnchorPoint(CCPoint(0.5,0)); // CCNode AP default is 0,0
	
	addChild(m_objectSprite);

	m_fistBodySprite = CCSprite::spriteWithFile("spring_solid.png");
	addChild(m_fistBodySprite);
	m_fistBodySprite->setAnchorPoint(CCPoint(0.5,0.5));	
	
	m_secondBodySprite = CCSprite::spriteWithFile("spring_solid.png");
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

	ASSERT_PROTOTYPE( m_objectBody = json.j2b2Body( physicsWorld(), prototype()["spring top"] ) );
	ASSERT_PROTOTYPE( m_objectBody->GetFixtureList() );
	m_objectBody->SetUserData(this);
	m_objectBody->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_objectBody->GetAngle() );

	ASSERT_PROTOTYPE( m_secondBody = json.j2b2Body( physicsWorld(), prototype()["spring bottom"] ) );
	ASSERT_PROTOTYPE( m_secondBody->GetFixtureList() );
	m_secondBody->SetUserData(this);
	m_secondBody->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_secondBody->GetAngle() );

	b2DistanceJointDef jointDef1;
	jointDef1.bodyA = m_objectBody;
	jointDef1.bodyB = m_secondBody;
	jointDef1.localAnchorA.Set(1, 0);
	jointDef1.localAnchorB.Set(1, 0);
	jointDef1.length = MIN_LENGHT / PTM_RATIO;
	jointDef1.frequencyHz = 20;
	jointDef1.dampingRatio = 1;
	jointDef1.collideConnected = true;

	b2PrismaticJointDef jointDef2;
	jointDef2.Initialize(m_objectBody, m_secondBody, b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), b2Vec2(0,1));
	jointDef2.collideConnected = true;

	b2DistanceJointDef jointDef3;
	jointDef3.bodyA = m_objectBody;
	jointDef3.bodyB = m_secondBody;
	jointDef3.localAnchorA.Set(-1, 0);
	jointDef3.localAnchorB.Set(-1, 0);
	jointDef3.length = MIN_LENGHT / PTM_RATIO;
	jointDef3.frequencyHz = 20;
	jointDef3.dampingRatio = 1;
	jointDef3.collideConnected = true;
	
	m_offsetBetweenBodies = b2Vec2(0, (m_fistBodySprite->getContentSize().height / PTM_RATIO) + MIN_LENGHT / PTM_RATIO );

#undef ASSERT_PROTOTYPE

	m_joints.push_back((b2DistanceJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef1));
	m_prismaticJoint = (b2PrismaticJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef2);
	m_joints.push_back((b2DistanceJoint*)GameWorld::sharedGameWorld()->physicsWorld->CreateJoint(&jointDef3));
	setPosition(position);
	return true;
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
		

		m_fistBodySprite->setPosition(convertToNodeSpace(p1));
		m_secondBodySprite->setPosition(convertToNodeSpace(p2));
    }
}

void ObjectSpring::restoreToOriginalProperties(){
    GameObject::restoreToOriginalProperties();
	m_objectBody->SetLinearVelocity(b2Vec2(0, 0));
	m_objectBody->SetAngularVelocity(0);
	m_objectBody->SetTransform(m_firstBodyOriginalLocation, m_firstBodyOriginalRotation);
	
	m_secondBody->SetLinearVelocity(b2Vec2(0, 0));
	m_secondBody->SetAngularVelocity(0);
	m_secondBody->SetTransform(m_secondBodyOriginalLocation, m_secondBodyOriginalRotation);
}

void ObjectSpring::saveOriginalProperties(){
	GameObject::saveOriginalProperties();
	m_firstBodyOriginalLocation = m_objectBody->GetPosition();
	m_firstBodyOriginalRotation = m_objectBody->GetAngle();
	m_secondBodyOriginalLocation = m_secondBody->GetPosition();
	m_secondBodyOriginalRotation = m_secondBody->GetAngle();	
}

void ObjectSpring::onSimulationStarted(){
	saveOriginalProperties();
	m_objectBody->SetAwake(true);
	m_secondBody->SetAwake(true);
	m_objectBody->SetType(b2_dynamicBody);
	m_secondBody->SetType(b2_dynamicBody);  	
	m_objectBody->SetFixedRotation(false);
	m_secondBody->SetFixedRotation(false);	
	for (unsigned int i = 0; i < m_joints.size(); i++) {
		m_joints.at(i)->SetLength(MAX_LENGHT / PTM_RATIO);
	}	
}

void ObjectSpring::onSimulationEnded(){
	m_objectBody->SetType(b2_staticBody);
	m_secondBody->SetType(b2_staticBody);	
	for (unsigned int i = 0; i < m_joints.size(); i++) {
		m_joints.at(i)->SetLength(MIN_LENGHT / PTM_RATIO);
	}	
	restoreToOriginalProperties();
	
}

void ObjectSpring::onMovementStarted(){
	m_objectBody->SetType(b2_staticBody);
	m_secondBody->SetType(b2_staticBody);
	m_objectBody->SetFixedRotation(true);
	m_secondBody->SetFixedRotation(true);
}

void ObjectSpring::onMovementEnded(){
	GameObject::onMovementEnded();
	m_secondBody->SetType(b2_staticBody);
	m_objectBody->SetFixedRotation(false);
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
	if (getParent() && m_objectBody) {
		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2PositionB1 = b2Vec2(newPostion.x/PTM_RATIO,
		                           newPostion.y/PTM_RATIO);
        
		m_objectBody->SetTransform(b2PositionB1, m_objectBody->GetAngle());
		
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
    if (getParent() && m_objectBody) {
		// Update posisiotn of phisical body moving it to nodes position
		b2Vec2 b2Position = b2Vec2(getPosition().x/PTM_RATIO,
		                           getPosition().y/PTM_RATIO);
		float32 b2Angle =  -1 * CC_DEGREES_TO_RADIANS(newRotation);
		
		m_objectBody->SetTransform(b2Position, b2Angle);
        
	}
}

void ObjectSpring::startUnstuckPhase(){
	m_objectBody->SetType(b2_dynamicBody);
	m_objectBody->SetFixedRotation(true);	
	m_objectBody->SetGravityScale(0);
	
	m_secondBody->SetType(b2_dynamicBody);
	m_secondBody->SetFixedRotation(true);
	m_secondBody->SetGravityScale(0);
}


void ObjectSpring::unstuckPhaseFinished(){
    if (getParent() && m_objectBody) {
		m_objectBody->SetGravityScale(1);	
		m_objectBody->SetType(b2_staticBody);
		m_objectBody->SetFixedRotation(true);	
		
		
		m_secondBody->SetGravityScale(1);	
		m_secondBody->SetType(b2_staticBody);
		m_secondBody->SetFixedRotation(true);	
		GameLevelScene::sharedGameScene()->setUtilityButtonsVisibleFoSelectedObject(true);
	}

}

bool ObjectSpring::setBody( b2Body *b )
{
	GameObject::setBody( b );
	m_secondBody = NULL;
	
	m_joints.clear();
	m_prismaticJoint = NULL;
	
	b2JointEdge *joint = b->GetJointList();
	while( joint ) {
		b2PrismaticJoint* jnt = dynamic_cast<b2PrismaticJoint*>(joint->joint);
		if (jnt) {
			m_prismaticJoint = jnt;
			m_secondBody = joint->other;
			m_secondBodyOriginalLocation = m_secondBody->GetPosition();
			m_secondBodyOriginalRotation = m_secondBody->GetAngle();
		}else {
			b2DistanceJoint * j = dynamic_cast<b2DistanceJoint*>(joint->joint);
			if (j) {
				m_joints.push_back(j);
			}
		}
		joint = joint->next;
	}

	return true;
}

