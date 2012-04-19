//
//  ObjectFixedPoint.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/14/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectFixedPoint_H__
#define __ObjectFixedPoint_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;

INVENTORYITEM_CLASS_DEF( FixedPointInventoryItem , ObjectFixedPoint, "fixed.png" )

////////////////////////////////////////////////////
// ObjectFixedPoint
///////////////////////////////////////////////////
class ObjectFixedPoint : public GameObject {
protected:
	virtual bool init();
	
public:
    void createBodyAtPosition(CCPoint position);
	// LAYER_NODE_FUNC(ObjectFixedPoint);
	GAMEOBJECT_NODE_DEF( FixedPointInventoryItem , ObjectFixedPoint )
};

#endif
