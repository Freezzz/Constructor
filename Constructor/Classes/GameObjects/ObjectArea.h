
#ifndef __ObjectArea_H__
#define __ObjectArea_H__

#include "cocos2d.h"
#include "GameObject.h"
#include "FixtureFiller.h"
using namespace cocos2d;

typedef enum AreaType{
	WinArea = 0,
	LooseArea = 1
}AreaType;

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
    AreaType m_areaType;
protected:
	bool init( std::string spritePath );

	bool createBodyAtPosition(CCPoint position);
	virtual void draw();

public:
    ObjectArea(const string& fileName, const Json::Value& prototype)
	: GameObject(fileName, prototype) { }
	GAMEOBJECT_NODE_DEF( ObjectArea )

	AreaType getAreaType(){ return m_areaType; }
	void setAreaType( AreaType type );

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
