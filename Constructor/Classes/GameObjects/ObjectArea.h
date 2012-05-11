
#ifndef __ObjectArea_H__
#define __ObjectArea_H__

#include "cocos2d.h"
#include "GameObject.h"
#include "FixtureFiller.h"
using namespace cocos2d;

INVENTORYITEM_CLASS_DEF( AreaInventoryItem , ObjectArea, Area )

////////////////////////////////////////////////////
// ObjectArea
///////////////////////////////////////////////////
class ObjectArea : public GameObject
{
private:
	CCSprite * m_fillSprite;
	
	FixtureFiller * m_fixtureFiller;
protected:
	bool init( std::string spritePath );

	bool createBodyAtPosition(CCPoint position);
	bool setBody( b2Body *b );
	virtual void draw();
public:
	GAMEOBJECT_NODE_DEF( AreaInventoryItem , ObjectArea )
};


#endif
