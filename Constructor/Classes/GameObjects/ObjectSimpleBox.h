//
//  ObjectSimpleBox.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/7/12.
//  Copyright 2012 The Epic Bean. All rights reserved.
//

#ifndef __ObjectSimpleBox_H__
#define __ObjectSimpleBox_H__

#include "cocos2d.h"
#include "GameObject.h"
using namespace cocos2d;

INVENTORYITEM_CLASS_DEF( SimpleBoxInventoryItem, ObjectSimpleBox, "box.png" )


////////////////////////////////////////////////////
// ObjectSimpleBox
///////////////////////////////////////////////////
class ObjectSimpleBox : public GameObject {
protected:
	virtual bool init();
public:    
    void createBodyAtPosition(CCPoint position);
    
	GAMEOBJECT_NODE_DEF( SimpleBoxInventoryItem, ObjectSimpleBox )
};

#endif
