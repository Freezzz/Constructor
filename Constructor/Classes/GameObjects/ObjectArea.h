
#ifndef __ObjectArea_H__
#define __ObjectArea_H__

#include "cocos2d.h"
#include "GameObject.h"
#include "FixtureFiller.h"
using namespace cocos2d;

INVENTORYITEM_CLASS_DEF( AreaInventoryItem , ObjectArea, Area )

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
	CCSprite * m_fillSprite;
	
	FixtureFiller * m_fixtureFiller;
    AreaType m_areaType;
protected:
	bool init( std::string spritePath );

	bool createBodyAtPosition(CCPoint position);
	bool setBody( b2Body *b );
	virtual void draw();
public:
	GAMEOBJECT_NODE_DEF( AreaInventoryItem , ObjectArea )
    AreaType getAreaType(){ return m_areaType; }
	void setAreaType( AreaType type );
	
	static CCRect minimumBoundingBoxForPolygon(b2PolygonShape * shape);
};


#endif
