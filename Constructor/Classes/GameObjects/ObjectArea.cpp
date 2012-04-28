
#include "GameObjects/ObjectArea.h"
#include "../GameWorld.h"

INVENTORYITEM_GAMEOBJECT_NODE_DECL( AreaInventoryItem, ObjectArea )


////////////////////////////////////////////////////
// ObjectArae init
////////////////////////////////////////////////////
bool ObjectArea::init( std::string spritePath )
{
	m_objectSprite = CCSprite::spriteWithFile( spritePath.c_str() );

	// Adapt container to the graphical rapresentation
	setContentSize(m_objectSprite->getContentSize());
	m_objectSprite->setAnchorPoint(CCPoint(0,0));
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0

	addChild(m_objectSprite);

	isStatic = true;

	isMovable = true;
	isRotatable = true;
	isDeletable = true;

	rotateButtonOffset = CCPoint(30,0);
	deleteButtonOffset = CCPoint(-30,0);

	// Permits to pin to be always over other objects
	defaultZOrder = 20;

	scheduleUpdate();
	return true;
}

////////////////////////////////////////////////////
// Creates a dummy sensor object to check collisions
// with other objects
////////////////////////////////////////////////////
void ObjectArea::createBodyAtPosition( cocos2d::CCPoint position )
{
	// Player physical body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set( position.x/PTM_RATIO, position.y/PTM_RATIO );
	m_objectBody = GameWorld::sharedGameWorld()->physicsWorld->CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
    b2CircleShape circle;
    circle.m_radius = 100/PTM_RATIO;

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 0;
	fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.5f;
	fixtureDef.isSensor = true;
	m_objectBody->CreateFixture(&fixtureDef);
	m_objectBody->SetUserData(this);

	setPosition(position);
}
