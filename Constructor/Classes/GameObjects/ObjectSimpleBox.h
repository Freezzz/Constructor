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


////////////////////////////////////////////////////
// ObjectSimpleBox
///////////////////////////////////////////////////
class ObjectSimpleBox : public GameObject {
protected:
	virtual bool init();
public:    
    void createBodyAtPosition(CCPoint position);
    
	LAYER_NODE_FUNC(ObjectSimpleBox);
};

#endif
