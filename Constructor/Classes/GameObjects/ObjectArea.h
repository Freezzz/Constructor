
#ifndef __ObjectArea_H__
#define __ObjectArea_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;

INVENTORYITEM_CLASS_DEF( AreaInventoryItem , ObjectArea, Area )

////////////////////////////////////////////////////
// ObjectArea
///////////////////////////////////////////////////
class ObjectArea : public GameObject
{

protected:
	bool init( std::string spritePath );

	void createBodyAtPosition(CCPoint position);

public:
	GAMEOBJECT_NODE_DEF( AreaInventoryItem , ObjectArea )
};


#endif
