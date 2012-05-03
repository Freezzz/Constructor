
#include "GameObjects/ObjectArea.h"
#include "../GameWorld.h"

INVENTORYITEM_GAMEOBJECT_NODE_DECL( AreaInventoryItem, ObjectArea )


////////////////////////////////////////////////////
// ObjectArae init
////////////////////////////////////////////////////
bool ObjectArea::init( std::string spritePath, b2FixtureDef *fixtureDef )
{
	m_objectSprite = CCSprite::spriteWithFile( spritePath.c_str() );
	m_fixtureDef = fixtureDef;
	
	//Define texture to fill the fixture ( must be added to node render properly )
	m_fillSprite = CCSprite::spriteWithFile("stripes.png");
	addChild(m_fillSprite);
	m_fillSprite->setIsVisible(false);


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
	
	m_fixtureFiller = NULL;
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

	m_objectBody->CreateFixture(m_fixtureDef);
	m_objectBody->SetUserData(this);

	setPosition(position);
}
void ObjectArea::draw(){
	CCNode::draw();
	if(m_fixtureFiller!=NULL){
		m_fixtureFiller->draw();
	}
}

void ObjectArea::setBody( b2Body *b )
{
	GameObject::setBody( b );
//	m_fixtureFiller = new FixtureFiller(b->GetFixtureList(), ccc4f(255, 255, 255, 255), ccc4f(255, 0, 0, 255));
	m_fixtureFiller = new FixtureFiller(b->GetFixtureList(), m_fillSprite->getTexture(), ccc4f(255, 0, 0, 255));
}

