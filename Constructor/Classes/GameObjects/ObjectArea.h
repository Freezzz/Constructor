
#ifndef __ObjectArea_H__
#define __ObjectArea_H__

#include "cocos2d.h"
#include "GameObject.h"
#include "FixtureFiller.h"
using namespace cocos2d;

////////////////////////////////////////////////////
// ObjectArea
///////////////////////////////////////////////////
class ObjectArea : public GameObject
{
private:
	CCSprite *m_sprite;
	b2Body *m_body;
	CCSprite * m_fillSprite;
	
	FixtureFiller * m_fixtureFiller;
protected:
	bool init( std::string spritePath );

	bool createBodyAtPosition(CCPoint position);
	virtual void draw();

public:
    ObjectArea(const string& fileName, const Json::Value& prototype)
	: GameObject(fileName, prototype) { }
	GAMEOBJECT_NODE_DEF( ObjectArea )

	//////////////////////////////////////////////////// 
	// Begins object preparation for unstuck routine
	//////////////////////////////////////////////////// 
	void startUnstuckPhase();

	//////////////////////////////////////////////////// 
	// Function to be called after unstuck routine is finished
	//////////////////////////////////////////////////// 
	void unstuckPhaseFinished();

	bool init( );

	static CCRect minimumBoundingBoxForPolygon(b2PolygonShape * shape);
};


#endif
