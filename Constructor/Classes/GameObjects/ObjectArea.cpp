
#include "GameObjects/ObjectArea.h"
#include "../GameWorld.h"

INVENTORYITEM_GAMEOBJECT_NODE_DECL( AreaInventoryItem, ObjectArea )


////////////////////////////////////////////////////
// ObjectArae init
////////////////////////////////////////////////////
bool ObjectArea::init( std::string spritePath )
{
	m_objectSprite = CCSprite::spriteWithFile( spritePath.c_str() );
	
	//Define texture to fill the fixture ( must be added to node render properly )
	m_fillSprite = CCSprite::spriteWithFile("stripes.png");
	addChild(m_fillSprite);
	m_fillSprite->setIsVisible(false);


	addChild(m_objectSprite);
	m_objectSprite->setIsVisible(false);

	isStatic = true;

	isMovable = true;
	isRotatable = true;
	isDeletable = true;

	rotateButtonOffset = CCPoint(40,0);
	deleteButtonOffset = CCPoint(-40,0);

	m_areaType = WinArea;
	
	// Permits to pin to be always over other objects
	defaultZOrder = 20;

	scheduleUpdate();
	
	m_fixtureFiller = NULL;
	return true;
}

void ObjectArea::setAreaType( AreaType type ){
	m_areaType = type;
//	delete m_fixtureFiller;
	if (type == WinArea) {
		m_fillSprite = CCSprite::spriteWithFile("stripes_win.png");
	}else {
		m_fillSprite = CCSprite::spriteWithFile("stripes_loose.png");
	}
	m_fixtureFiller = new FixtureFiller(m_objectBody->GetFixtureList(), m_fillSprite->getTexture(), ccc4f(255, 255, 255, 255));
}

////////////////////////////////////////////////////
// Creates a dummy sensor object to check collisions
// with other objects
////////////////////////////////////////////////////
bool ObjectArea::createBodyAtPosition( cocos2d::CCPoint position )
{
	b2dJson json;
	m_objectBody = json.j2b2Body( physicsWorld(), prototype() );
	if( ! m_objectBody || ! m_objectBody->GetFixtureList() ) {
		std::cout << "Area inventory item prototype messed up" << std::endl;
		return false;
	}
	m_objectBody->SetUserData(this);
	m_objectBody->SetTransform( b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), m_objectBody->GetAngle() );
	setPosition(position);
	return true;
}
void ObjectArea::draw(){
	CCNode::draw();
	if(m_fixtureFiller!=NULL){
		// Translate draw due anchor point is in the middle point
		glTranslatef(getContentSizeInPixels().width *0.5, getContentSizeInPixels().height *0.5,0);

		m_fixtureFiller->draw();
		
		// Restore to previous state
		glTranslatef(-getContentSizeInPixels().width *0.5, -getContentSizeInPixels().height *0.5,0);		
	}	
}

bool ObjectArea::setBody( b2Body *b )
{
	GameObject::setBody( b );
//	m_fixtureFiller = new FixtureFiller(b->GetFixtureList(), ccc4f(255, 255, 255, 255), ccc4f(255, 0, 0, 255));
	m_fixtureFiller = new FixtureFiller(b->GetFixtureList(), m_fillSprite->getTexture(), ccc4f(255, 255, 255, 255));
	// Adapt gameobject's content size to polygon shape
	setAnchorPoint(CCPoint(0.5,0.5)); // CCNode AP default is 0,0	
	setContentSize(minimumBoundingBoxForPolygon((b2PolygonShape*)b->GetFixtureList()->GetShape()).size);
	return true;
}


CCRect ObjectArea::minimumBoundingBoxForPolygon(b2PolygonShape * shape){
	CCAssert(shape->GetVertexCount()>0, "Invalid shape must have atleast 1 point");
	float minX = shape->GetVertex(0).x;
	float minY = shape->GetVertex(0).y;
	float maxX = shape->GetVertex(0).x;
	float maxY = shape->GetVertex(0).y;
	

	for (int i = 1; i < shape->GetVertexCount(); i++) {
		if (minX > shape->GetVertex(i).x) {
			minX = shape->GetVertex(i).x;
		}
		if (minY > shape->GetVertex(i).y) {
			minY = shape->GetVertex(i).y;
		}
		if (maxX < shape->GetVertex(i).x) {
			maxX = shape->GetVertex(i).x;
		}
		if (maxY < shape->GetVertex(i).y) {
			maxY = shape->GetVertex(i).y;
		}
	}
	CCLog("Bounding box: x:%f, y:%f w:%f, h:%f", minX*PTM_RATIO, minY*PTM_RATIO, (maxX - minX)*PTM_RATIO, (maxY-minY)*PTM_RATIO);
	return CCRect(minX*PTM_RATIO, minY*PTM_RATIO, (maxX - minX)*PTM_RATIO, (maxY-minY)*PTM_RATIO);
}

void ObjectArea::startUnstuckPhase(){
	return;
}

void ObjectArea::unstuckPhaseFinished(){
	return;
}
